import AgoraRtcEngine from 'agora-electron-sdk'
import { DBYElectronSdk } from 'dby-paas-electron-sdk/render'
import _ from 'lodash'
import os from 'os'
import EventEmitter from 'events'
import path from 'path'

import { SDK_LINES } from '../../consts'
import pkgJson from '../../../package'

class SDKWrapper extends EventEmitter {
  dbySDKInstance

  agoraSDKInstance

  currentSDKLine = SDK_LINES.DBY

  currentSDKInstance

  micDevices = []

  cameraDevices = []

  getSDKInstance(sdkLine) {
    if (sdkLine === SDK_LINES.AGORA) {
      if (_.isNil(this.agoraSDKInstance) === true) {
        const instance = new AgoraRtcEngine()
        instance.initialize('<agora appid>')
        this.agoraSDKInstance = instance
        this.subscribeEvents(instance)
        window.agoraSDKInstance = instance
      }
      return this.agoraSDKInstance
    } else if (sdkLine === SDK_LINES.DBY) {
      if (_.isNil(this.dbySDKInstance) === true) {
        const instance = new DBYElectronSdk({
          isDev: false,
          isWebM: false,
          isDual: true,
          domainPostfix: '.net',  // .net 或者 .com
        })
        instance.setAppInfo({
          name: pkgJson.build.productName, // 'Electron',
          version: pkgJson.version,
        })
        instance.enableAudioVolumeIndication(500)
        instance.initialize('<dby appid>', '<dby appkey>')
        this.dbySDKInstance = instance
        this.subscribeEvents(instance)
        window.dbySDKInstance = instance
      }
      return this.dbySDKInstance
    }
    return this.currentSDKInstance
  }

  switchSDK = (sdkLine) => {
    if (_.isNil(this.currentSDKInstance) === false) {
      this.currentSDKInstance.release()
    }
    this.currentSDKLine = sdkLine
    this.currentSDKInstance = this.getSDKInstance(sdkLine)
  }

  subscribeEvents = (sdk) => {
    sdk.on('joinedChannel', (channel, uid, elapse) => {
      Log.info('joinedChannel ', channel, uid, elapse)
      this.emit('joinedChannel', channel, uid, elapse)
    })
    sdk.on('leaveChannel', () => {
      Log.info('leaveChannel')
      this.emit('leaveChannel')
    })

    sdk.on('userJoined', (uid, elapsed) => {
      Log.info('userJoined ', uid, elapsed)
      this.emit('userJoined', uid, elapsed)
    })
    sdk.on('removeStream', (uid, reason) => {
      Log.info('removeStream ', uid, reason)
      this.emit('removeStream', uid, reason)
    })

    sdk.on('error', (err, msg) => {
      Log.info('error ', err, msg)
      this.emit('error', err, msg)
    })

    sdk.on('streamMessage', (uid, streamId, msg, len) => {
      Log.info('streamMessage ', uid, streamId, msg, len)
      this.emit('streamMessage', uid, streamId, msg, len)
    })

    sdk.on('firstLocalVideoFrame', (width, height, elapsed) => {
      Log.info('firstLocalVideoFrame ', width, height, elapsed)
      this.emit('firstLocalVideoFrame', width, height, elapsed)
    })
    sdk.on('firstRemoteVideoFrame', (uid, width, height, elapsed) => {
      Log.info('firstRemoteVideoFrame ', uid, width, height, elapsed)
      this.emit('firstRemoteVideoFrame', uid, width, height, elapsed)
    })

    sdk.on('remoteVideoStateChanged', (uid, state, reason, elapsed) => {
      Log.info('remoteVideoStateChanged ', uid, state, reason, elapsed)
      this.emit('remoteVideoStateChanged', uid, state, reason, elapsed)
    })
    sdk.on('localVideoStateChanged', (uid, state) => {
      Log.info('**** localVideoStateChanged ', uid, state)
      this.emit('localVideoStateChanged', uid, state)
    })

    sdk.on('localAudioStateChanged', (state, reason) => {
      this.emit('localAudioStateChanged', state, reason)
    })
    sdk.on('remoteAudioStateChanged', (uid, state, reason) => {
      this.emit('remoteAudioStateChanged', uid, state, reason)
    })

    sdk.on('videoDeviceStateChanged', (deviceId, deviceType, deviceState) => {
      Log.info('videoDeviceStateChanged ', deviceId, deviceType, deviceState)
      this.emit('videoDeviceStateChanged', deviceId, deviceType, deviceState)
    })
    sdk.on('audioDeviceStateChanged', (deviceId, deviceType, deviceState) => {
      Log.info('audioDeviceStateChanged ', deviceId, deviceType, deviceState)
      this.emit('audioDeviceStateChanged', deviceId, deviceType, deviceState)
    })

    sdk.on('dumpProtocol', (content) => {
      Log.info('dumpProtocol ', content, sdk.getVersion())
    })

    sdk.on('audioVolumeIndication', (state) => {
      this.emit('audioVolumeIndication', state)
    })
    sdk.on('connectionStateChanged', (state) => {
      this.emit('connectionStateChanged', state)
    })
    sdk.on('networkQuality', (uid, txQuality, rxQuality) => {
      this.emit('networkQuality', uid, txQuality, rxQuality)
    })
  }

  onEvent = (eventName, callback) => {
    this.on(eventName, callback)
  }

  getVideoDevices = () => {
    return new Promise((resolve, reject) => {
      let sdk = this.getSDKInstance()
      let devices
      if (this.currentSDKLine === SDK_LINES.AGORA) {
        devices = sdk.getVideoDevices()
        devices = _.map(devices, device => {
          return {
            deviceName: device.devicename,
            deviceId: device.deviceid,
          }
        })
        this.cameraDevices = devices
        resolve(devices)
      } else {
        sdk.getVideoDevices().then((result) => {
          devices = _.map(result, device => {
            return {
              deviceName: device.name,
              deviceId: null,
            }
          })
          this.cameraDevices = devices
          resolve(devices)
        })
      }
    })
  }

  getAudioRecordingDevices = () => {
    return new Promise((resolve) => {
      let sdk = this.getSDKInstance()
      let devices
      if (this.currentSDKLine === SDK_LINES.AGORA) {
        devices = sdk.getAudioRecordingDevices()
        devices = _.map(devices, device => {
          return {
            deviceName: device.devicename,
            deviceId: device.deviceid,
          }
        })
        this.micDevices = devices
        resolve(devices)
      } else {
        sdk.getAudioRecordingDevices().then((result) => {
          devices = _.map(result, device => {
            return {
              deviceName: device,
              deviceId: null,
            }
          })
          this.micDevices = devices
          resolve(devices)
        })
      }
    })
  }

  setVideoDevice = (deviceIndex) => {
    const sdk = this.currentSDKInstance
    if (this.currentSDKLine === SDK_LINES.AGORA) {
      sdk.setVideoDevice(this.cameraDevices[deviceIndex].deviceId)
    } else {
      sdk.setVideoDevice(deviceIndex)
    }
  }

  setAudioRecordingDevice = (deviceIndex) => {
    const sdk = this.currentSDKInstance
    if (this.currentSDKLine === SDK_LINES.AGORA) {
      sdk.setAudioRecordingDevice(this.micDevices[deviceIndex].deviceId)
    } else {
      sdk.setAudioRecordingDevice(deviceIndex)
    }
  }

  joinChannel = (options = {token: null, channel: '', info:'', uid: '', nickname: '', role: 2}) => {
    _.defaults(options, {token: null, channel: '', info:'', uid: '', nickname: '', role: 2})
    let result
    return new Promise((resolve, reject) => {
      const sdk = this.currentSDKInstance
      try {
        if (this.currentSDKLine === SDK_LINES.AGORA) {
          sdk.setChannelProfile(1)
          sdk.setClientRole(1)
          sdk.setAudioProfile(0, 1)
          sdk.enableVideo()
          const logPath = path.resolve(os.homedir(), "./agoramain.sdk")
          sdk.setLogFile(logPath)
          sdk.enableLocalVideo(true)
          sdk.setVideoDevice(0)
          sdk.enableWebSdkInteroperability(true)
          sdk.setVideoEncoderConfiguration({frameRate: 15, bitrate: 65})
          sdk.setLocalVoiceChanger(0)
          sdk.setLocalVoiceReverbPreset(0)
          sdk.enableDualStreamMode(true)
          sdk.enableAudioVolumeIndication(1000, 3)
          sdk.setBeautyEffectOptions(true, {
            lighteningContrastLevel: 2,
            lighteningLevel: 1,
            smoothnessLevel: 1,
            rednessLevel: 0
          })
          result = sdk.joinChannel(options.token, options.channel, options.info, parseInt(options.uid, 10))
          Log.info('agora join channel result ', result)
          if (result === 0) {
            resolve(0)
          } else {
            reject(result)
          }
        } else {
          sdk.setVideoEncoderConfiguration({
            width: 1280,
            height: 720,
          })
          this.dbySDKInstance.joinChannel(options.channel, options.uid, options.nickname, options.role).then((result) => {
            Log.info('dby join channel result ', result)
            if (result === 0) {
              sdk.enableLocalVideo(true)
              sdk.setVideoDevice(0)
            }
            resolve(result)
          })
        }
      } catch (e) {
        reject(e)
      }
    })
  }

  leaveChannel = () => {
    return new Promise((resolve, reject) => {
      if (this.currentSDKLine === SDK_LINES.AGORA) {
        try {
          const result = this.currentSDKInstance.leaveChannel()
          resolve(result)
        } catch (e) {
          reject(e)
        }
      } else {
        this.currentSDKInstance.leaveChannel().then((result) => {
          resolve(result)
        }).catch((e) => {
          reject(e)
        })
      }
    })
  }

  setupLocalVideo = (dom) => {
    this.currentSDKInstance.setupLocalVideo(dom)
  }

  subscribe = (uid, dom) => {
    this.currentSDKInstance.subscribe(uid, dom)
  }

  destroyRender = (uid) => {
    this.currentSDKInstance.destroyRender(uid)
  }

  enableLocalVideo = (enable) => {
    this.currentSDKInstance.enableLocalVideo(enable === true)
  }

  enableLocalAudio = (enable) => {
    this.currentSDKInstance.enableLocalAudio(enable === true)
  }

  setRemoteVideoStreamType = (uid, type) => {
    return this.currentSDKInstance.setRemoteVideoStreamType(uid, type)
  }

  release = () => {
    return new Promise(resolve => {
      if (this.currentSDKLine === SDK_LINES.AGORA) {
        this.currentSDKInstance.release()
        resolve(0)
      } else {
        this.currentSDKInstance.release().then(() => {
          resolve(0)
        })
      }
    })
  }

  sendStreamMessage = (msg) => {
    if (this.currentSDKLine === SDK_LINES.AGORA) {
    } else {
      this.currentSDKInstance.sendStreamMessage(msg)
    }
  }

  enableAudioVolumeIndication(interval) {
    if (this.currentSDKLine === SDK_LINES.AGORA) {
      this.currentSDKInstance.enableAudioVolumeIndication(interval)
    } else {
      this.currentSDKInstance.enableAudioVolumeIndication(interval)
    }
  }

  muteRemoteVideoStream(uid, mute) {
    if (this.currentSDKLine === SDK_LINES.AGORA) {
      this.currentSDKInstance.muteRemoteVideoStream(uid, mute)
    } else {
      this.currentSDKInstance.muteRemoteVideoStream(uid, mute)
    }
  }

  muteRemoteAudioStream(uid, mute) {
    if (this.currentSDKLine === SDK_LINES.AGORA) {
      this.currentSDKInstance.muteRemoteAudioStream(uid, mute)
    } else {
      this.currentSDKInstance.muteRemoteAudioStream(uid, mute)
    }
  }
}

export default SDKWrapper
