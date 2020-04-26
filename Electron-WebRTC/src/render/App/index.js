import React, { Component, createRef } from 'react'
import md5 from 'md5'
import { Input, Button, Select, Checkbox, Radio, Comment } from 'antd'
import moment from 'moment'
import { observer } from 'mobx-react'
import _ from 'lodash'
import { ipcRenderer } from 'electron'

import DBYPaasWebSDK from 'dby-paas-web-sdk'

import { CAMERA_OUTPUT_OPTIONS } from '../consts'
import store from '../store'
import { IPC_EVENT } from '../consts'

import './index.less'

window.store = store

const { Option } = Select
const { TextArea } = Input
const { Group: RadioGroup } = Radio

const sdk = new DBYPaasWebSDK()
window.sdk = sdk
sdk.initialize(#appID#, #appKey#) // fixme 这里填 appID 和 appKey
sdk.enableDualStream(true)
sdk.enableAudioVolumeIndication(2000)

sdk.getVideoDevices().then((devices) => {
  store.setCameraDevices(devices)
  console.log('getVideoDevices ', devices)
  if (devices.length > 0) {
    store.setCurrentCameraDeviceName(devices[0].deviceName)
    store.setCurrentCameraOutputOptionIndex(1) // 默认用中档清晰度

    // 设置 SDK
    sdk.setVideoDevice(0)
    const config = CAMERA_OUTPUT_OPTIONS[store.currentCameraOutputOptionIndex]
    sdk.setVideoEncoderConfiguration({
      width: config.width,
      height: config.height,
    })
  }
})
sdk.getAudioRecordingDevices().then((devices) => {
  store.setMicDevices(devices)
  console.log('getAudioRecordingDevices ', devices)
  if (devices.length > 0) {
    store.setCurrentMicDeviceName(devices[0].deviceName)
    // 设置 SDK
    sdk.setAudioRecordingDevice(0)
  }
})

@observer
class App extends Component {
  state = {
    channel: '',
    uid: '',
    chatContent: '',
    messageList: [],
    joinChannelInfo: '', // 加入频道结果提示
    users: [], // 加入频道的用户的uid
    dumpProtocolContent: '',
    showDebugInfo: false,
    isJoiningChannel: false,
  }

  componentDidMount() {
    this.subscribeEvents()

    ipcRenderer.on(IPC_EVENT.BEFORE_QUIT, () => {
      Log.log('******render on BEFORE_QUIT')
      sdk.leaveChannel().then((res) => {
        console.log('********close client leaveChannel res', res)
        ipcRenderer.send(IPC_EVENT.APP_QUIT, true)
      }).catch((e) => {
        ipcRenderer.send(IPC_EVENT.APP_QUIT, true) // 失败依然退出
        console.log('****close client leaveChannel err', e)
      })
    })

    // 切换是否显示 debug 信息
    ipcRenderer.on(IPC_EVENT.TOGGLE_SHOW_DEBUG_INFO, (e, show) => {
      this.setState({
        showDebugInfo: show === true,
      })
    })
  }

  componentWillUnmount() {
    sdk.stop()
  }

  addMessage = (msg, author) => {
    this.setState(prevState => ({
      messageList: prevState.messageList.concat([{
        msg,
        author,
        createTime: moment().format('MM-DD HH:mm:ss'),
      }]),
    }), () => {
      this.messageListWrapRef.current.scrollTop = 1e10
    })
  }

  subscribeEvents = () => {
    sdk.on('streamMessage',(uid, msg) => {
      console.log('******demo streamMessage',uid, msg)
      this.addMessage(msg, uid)
    })
    sdk.on('joinedChannel',(channel, uid, elapsed) => {
      console.log('******joinedChannel', channel, uid, elapsed)
      this.addMessage(`${uid} join this channel!`, 'system')
      sdk.setupLocalVideo(this.selfVideoRef.current)
    })

    sdk.on('leaveChannel', () => {
      console.log('******leaveChannel')
      this.addMessage('Same uid join this channel somewhere else. You have left the channel', 'system')
      this.setState({
        joinChannelInfo: ''
      })
    })

    sdk.on('userJoined', (uid, elapsed) => {
      console.log('******userJoined', uid)
      this.addMessage(`${uid} join this channel!`, 'system')
      if (_.indexOf(this.state.users, uid) === -1) {
        this.setState(prevState => ({
          users: [...prevState.users, uid],
        }))
      }
    })

    sdk.on('removeStream', (uid, elapsed) => {
      console.log('******userOffline', uid)
      if (_.indexOf(this.state.users, uid) > -1) {
        this.setState(prevState => ({
          users: _.filter(prevState.users, (user) => (user !== uid)),
        }))
        this.addMessage(`${uid} leave this channel!`, 'system')
      }
    })

    sdk.on('firstRemoteVideoFrame', (uid) => {
      console.log('******firstRemoteVideoFrame', uid)
    })

    sdk.on('firstLocalVideoFrame', (uid) => {
      console.log('******firstLocalVideoFrame', uid)
    })

    sdk.on('firstRemoteAudioFrame', (uid) => {
      console.log('******firstRemoteAudioFrame', uid)
    })

    sdk.on('firstLocalAudioFrame', (uid) => {
      console.log('******firstLocalAudioFrame', uid)
    })

    sdk.on('groupAudioVolumeIndication', (result) => {
      console.log('groupAudioVolumeIndication ', result)
    })
  }

  onChannelChange = (e) => {
    this.setState({
      channel: e.target.value,
    })
  }

  onUidChange = (e) => {
    this.setState({
      uid: e.target.value,
    })
  }

  handleClickJoinChannel = () => {
    if (this.state.isJoiningChannel === true) {
      return
    } else {
      this.setState({isJoiningChannel: true, joinChannelInfo: ''}, () => {
        const { channel, uid } = this.state
        if (_.trim(channel) === '') {
          this.setState({
            joinChannelInfo: '加入频道失败！频道名不能为空',
            isJoiningChannel: false,
          })
          return
        }

        if (_.trim(channel) !== channel ) {
          this.setState({
            joinChannelInfo: '加入频道失败！频道名开头结尾不能包含不可见字符',
            isJoiningChannel: false,
          })
          return
        }

        if (_.trim(uid) === '') {
          this.setState({
            joinChannelInfo: '加入频道失败！用户ID不能为空',
            isJoiningChannel: false,
          })
          return
        }

        if (_.trim(uid) !== uid ) {
          this.setState({
            joinChannelInfo: '加入频道失败！用户ID开头结尾不能包含不可见字符',
            isJoiningChannel: false,
          })
          return
        }

        sdk.joinChannel(channel, uid, uid).then((code) => {
          console.log('success join channel', code);
          if(code === 0) {
            this.setState({
              joinChannelInfo: '加入频道成功！',
              isJoiningChannel: false,
            })
            ipcRenderer.send(IPC_EVENT.DISABLE_QUIT)
          } else {
            this.setState({
              joinChannelInfo: `加入频道失败！${code}`,
              isJoiningChannel: false,
            })
          }
        }).catch((err) => {
          console.log('****demo joinChannel err', err)
          this.setState({
            joinChannelInfo: `加入频道失败！err: ${err}`,
            isJoiningChannel: false,
          })
        })
      })
    }
  }


  onChatContentChange = (e) => {
    this.setState({
      chatContent: e.target.value,
    })
  }

  onClickSendChatMsg = () => {
    const { chatContent } = this.state
    if(chatContent !== '') {
      sdk.sendStreamMessage(chatContent)
      this.addMessage(chatContent, this.state.uid)
      this.setState({ chatContent: '' })
      this.textAreaRef.current.value = ''
    }
  }

  selfVideoRef = createRef()

  textAreaRef = createRef()

  messageListWrapRef = createRef()

  render() {
    const cameraDevices = store.cameraDevices.toJSON()
    const micDevices = store.micDevices.toJSON()

    return (
      <div className="app-component-wrap">
        <div className="menus-wrap">
          <div className="camera-settings-wrap">
            <p className="label">摄像头设备列表</p>
            <Select
              value={store.currentCameraDeviceIndex}
              onChange={(index) => {
                sdk.setVideoDevice(index);
                store.setCurrentCameraDeviceName(cameraDevices[index].deviceName)

                sdk.setVideoDevice(index)
              }}
            >
              {
                _.map(cameraDevices, (device, index) => {
                  return (
                    <Option value={index} key={index}>{device.deviceName}</Option>
                  )
                })
              }
            </Select>
            <p className="label" style={{display: 'none'}}>摄像头清晰度</p>
            <Select
              style={{display: 'none'}}
              value={store.currentCameraOutputOptionIndex}
              onChange={(index) => {
                store.setCurrentCameraOutputOptionIndex(index)

                const config = CAMERA_OUTPUT_OPTIONS[index]
                sdk.setVideoEncoderConfiguration({
                  width: config.width,
                  height: config.height,
                })
              }}
            >
              {
                _.map(_.values(CAMERA_OUTPUT_OPTIONS), (option, index) => {
                  return (
                    <Option value={index} key={index}>{`${option.width}*${option.height}`}</Option>
                  )
                })
              }
            </Select>
          </div>
          <div className="mic-settings-wrap">
            <p className="label">麦克风设备列表</p>
            <Select
              value={store.currentMicDeviceIndex}
              onChange={(index) => {
                store.setCurrentMicDeviceName(micDevices[index].deviceName)
                sdk.setAudioRecordingDevice(index)
              }}
            >
              {
                _.map(micDevices, (device, index) => {
                  return (
                    <Option value={index} key={index}>{device.deviceName}</Option>
                  )
                })
              }
            </Select>
          </div>
          <div className="join-channel-wrap">
            <p className="label">频道名</p>
            <Input
              placeholder="请输入频道名"
              onChange={this.onChannelChange}
            />
            <p className="label">用户ID</p>
            <Input
              placeholder="请输入用户ID"
              onChange={this.onUidChange}
            />
            <Button
              type="primary"
              className="btn-join-channel"
              loading={this.state.isJoiningChannel === true}
              onClick={this.handleClickJoinChannel}
            >
              加入
            </Button>
            <p className="join-channel-result-tip">{this.state.joinChannelInfo}</p>
          </div>
          {
            this.state.showDebugInfo === true && (
              <div className="debug-info-wrap">
                <p className="dump-protocol-content">{ this.state.dumpProtocolContent }</p>
              </div>
            )
          }
        </div>
        <div className="content-wrap">
          <div className="cameras-wrap">
            <div className="camera-content-container">
              <div className="camera-content-wrap" ref={this.selfVideoRef} />
            </div>
            {
              _.map(this.state.users, (uid) => {
                return (
                  <RemoteVideo key={uid} uid={uid} sdk={sdk}/>
                )
              })
            }
          </div>
          <div className="chats-wrap">
            <div className="chat-list-wrap" ref={this.messageListWrapRef}>
              {_.map(this.state.messageList, (msgItem, i) => {
                return (
                  <Comment
                    key={i}
                    style={{
                      color: msgItem.author === 'system' ? '#a00': 'rgba(0, 0, 0, 0.65)',
                    }}
                    actions={null}
                    author={msgItem.author}
                    content={(
                      <p className="chat-item">{msgItem.msg}</p>
                    )}
                    datetime={(
                      <span>{msgItem.createTime}</span>
                    )}
                  />
                )
              })}
            </div>
            <div className="chat-input-wrap">
              <TextArea
                placeholder="请输入聊天内容"
                value={this.state.chatContent}
                rows={4}
                className="input-chat-content"
                onChange={this.onChatContentChange}
                ref={this.textAreaRef}
              />
              <Button
                type="primary"
                className="btn-send-chat"
                onClick={this.onClickSendChatMsg}
                disabled={this.state.chatContent === ''}
              >
                发送
              </Button>
            </div>
          </div>
        </div>
      </div>
    )
  }
}

export default App

class RemoteVideo extends Component {
  constructor(props) {
    super(props)
  }

  componentDidMount() {
    let dom = document.querySelector(`#video-${md5(this.props.uid)}`)
    this.props.sdk.subscribe(this.props.uid, dom)
  }

  render() {
    return (
      <div className="camera-content-container">
        <div className="camera-content-wrap" id={`video-${md5(this.props.uid)}`} />
        <RadioGroup
          defaultValue={1}
          onChange={(e) => {
            console.log('call sdk.setRemoteVideoStreamType ', this.props.uid, e.target.value)
            sdk.setRemoteVideoStreamType(this.props.uid, e.target.value)
          }}
          className="radio-choose-stream-type"
          style={{
            // display: 'none',
          }}
        >
          <Radio value={1}>小流</Radio>
          <Radio value={0}>大流</Radio>
        </RadioGroup>
      </div>
    )
  }
}
