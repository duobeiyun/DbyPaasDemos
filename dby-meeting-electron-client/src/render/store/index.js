import { types } from 'mobx-state-tree'
import _ from 'lodash'
import uuid from 'uuid/v1'

import BaseType from './BaseType'
import { SDK_LINES } from '../../consts'

const Store = BaseType.named('Store').props({
  selfUid: types.number, // uid
  currentSDKLine: types.enumeration(_.keys(SDK_LINES)),
  cameraDevices: types.array(types.frozen()), // [{ deviceName. deviceId }]
  currentCameraDeviceName: types.string,
  micDevices: types.array(types.string), // [{ deviceName. deviceId }]
  currentMicDeviceName: types.string,
  onlineUsers: types.array(types.number), // [uid]  所有在线的用户，不包含本端自己
  videoStreams: types.array(types.number), // [uid]
  audioStreams: types.array(types.number), // [uid]
  currentFullScreenVideoUid: types.number, // 当前正在最大化的用户
  chatList: types.array(types.frozen(types.model({
    uid: types.number,
    content: types.string,
  }))),
  tipList: types.array(types.frozen(types.model({
    id: types.string,
    content: types.string,
  }))), // 消息提醒列表
  showUserVideoList: types.boolean,
  userVolumes: types.map(types.number),
  systemStatus: BaseType.named('systemStatus').props({
    sendQuality: types.number,
    receiveQuality: types.number,
    cpuUsage: types.number,
    memoryUsage: types.number,
  })
}).views(self => ({
  get currentCameraDeviceInfo() {
    return _.find(self.cameraDevices.toJSON(), ['deviceName', self.currentCameraDeviceName])
  },
  get currentCameraDeviceIndex() {
    return _.findIndex(self.cameraDevices.toJSON(), ['deviceName', self.currentCameraDeviceName])
  },
  get currentMicDeviceInfo() {
    return _.find(self.micDevices.toJSON(), ['deviceName', self.currentMicDeviceName])
  },
  get currentMicDeviceIndex() {
    return _.indexOf(self.micDevices.toJSON(), ['deviceName', self.currentMicDeviceName])
  },
  get selfCameraOpened() {
    return _.indexOf(self.videoStreams.toJSON(), self.selfUid) !== -1
  }
})).actions(self => ({
  setCurrentSDKLine(line) {
    self.currentSDKLine = line
  },
  setCameraDevices(devices) {
    self.cameraDevices.splice(0, self.cameraDevices.length, ...devices)
  },
  setCurrentCameraDeviceName(name) {
    self.currentCameraDeviceName = name
  },
  setMicDevices(devices) {
    self.micDevices.splice(0, self.micDevices.length, ...devices)
  },
  setCurrentMicDeviceName(name) {
    self.currentMicDeviceName = name
  },
  addOnlineUser(uid) {
    if (_.indexOf(self.onlineUsers.toJSON(), uid) === -1) {
      self.onlineUsers.push(uid)
    }
  },
  removeOnlineUser(uid) {
    const index = _.indexOf(self.onlineUsers.toJSON(), uid)
    if (index >= 0) {
      self.onlineUsers.splice(index, 1)
    }
  },
  addVideoStream(uid) {
    if (_.indexOf(self.videoStreams.toJSON(), uid) === -1) {
      self.videoStreams.push(uid)
    }
  },
  removeVideoStream(uid) {
    const index = _.indexOf(self.videoStreams.toJSON(), uid)
    if (index >= 0) {
      self.videoStreams.splice(index, 1)
    }
  },
  addAudioStream(uid) {
    if (_.indexOf(self.audioStreams.toJSON(), uid) === -1) {
      self.audioStreams.push(uid)
    }
  },
  removeAudioStream(uid) {
    const index = _.indexOf(self.audioStreams.toJSON(), uid)
    if (index >= 0) {
      self.audioStreams.splice(index, 1)
    }
  },
  setCurrentFullScreenVideoUid(uid) {
    console.log('call setCurrentFullScreenVideoUid ', uid)
    store.currentFullScreenVideoUid = uid
  },
  addTip(content) {
    const id = uuid()
    self.tipList.push({
      id,
      content,
    })
    return id
  },
  removeTip(id) {
    const index = _.findIndex(self.tipList, ['id', id])
    self.tipList.splice(index, 1)
  },
  addChat(chatInfo) {
    self.chatList.push(chatInfo)
  },
  updateUserVolume(uid, volume) {
    self.userVolumes.set(uid, volume)
  }
}))

const store = Store.create({
  selfUid: -1, // uid
  currentSDKLine: SDK_LINES.AGORA,
  cameraDevices: [],
  currentCameraDeviceName: '',
  micDevices: [],
  currentMicDeviceName: '',
  onlineUsers: [],
  videoStreams: [],
  audioStreams: [],
  currentFullScreenVideoUid: -1,
  chatList: [],
  tipList: [],
  showUserVideoList: true,
  systemStatus: {
    sendQuality: 0,
    receiveQuality: 0,
    cpuUsage: 0,
    memoryUsage: 0,
  }
})
window.store = store

export default store
