import _ from 'lodash'

import SDKWrapper from './wrapper'

import store from '../store'

const sdk = new SDKWrapper
window.sdkwrapper = sdk

function getCurrentTime() {
  return new Date().toTimeString().split(' ')[0]
}

sdk.onEvent('joinedChannel', (channel, uid, elapsed) => {
  console.log('----- joinedChannel ', channel, uid, elapsed)
  store.addTip(`本端 ${uid} 上线 ${getCurrentTime()}`)
})
sdk.onEvent('leaveChannel', () => {
  console.log('----- leaveChannel')
  store.addTip(`本端下线 ${getCurrentTime()}`)
})


sdk.onEvent('userJoined', (uid, elapsed) => {
  console.log('----- userJoined ', uid, elapsed)
  // 如果没有设置过 currentFullScreenVideoUid 或者此 uid 是除了本人之外的第一个上线用户，则初始化成此 uid

  if (store.currentFullScreenVideoUid === store.selfUid || store.currentFullScreenVideoUid === -1) {
    store.setCurrentFullScreenVideoUid(uid)
  }

  store.addOnlineUser(uid)
  store.addTip(`用户 ${uid} 上线了 ${getCurrentTime()}`)
})
sdk.onEvent('removeStream', (uid, reason) => {
  console.log('----- removeStream ', uid, reason)
  store.removeOnlineUser(uid)
  store.addTip(`用户 ${uid} 下线了 ${getCurrentTime()}`)
  // 如果没有别人在线了，则把自己的视频放大
  if (store.onlineUsers.length === 0) {
    store.setCurrentFullScreenVideoUid(store.selfUid)
  } else if (store.currentFullScreenVideoUid === uid) {
    // 如果退出的是正在全屏的用户，则把剩下列表里的第一个放大
    store.setCurrentFullScreenVideoUid(store.onlineUsers[0])
  }
})

// todo
sdk.onEvent('firstLocalVideoFrame', (width, height, elapsed) => {
  console.log('----- firstLocalVideoFrame ', width, height, elapsed, store.selfUid, store.currentFullScreenVideoUid === -1)
  store.addTip(`本端第一帧渲染 ${getCurrentTime()}`)
  // 如果没有设置过 currentFullScreenVideoUid，则初始化一下
  if (store.currentFullScreenVideoUid === -1) {
    store.setCurrentFullScreenVideoUid(store.selfUid)
  }
})
// todo
sdk.onEvent('firstRemoteVideoFrame', (uid, width, height, elapsed) => {
  console.log('----- firstRemoteVideoFrame ', uid, width, height, elapsed)
  store.addTip(`用户 ${uid} 第一帧渲染 ${getCurrentTime()}`)
})


// todo
sdk.onEvent('localVideoStateChanged', (status, error) => {
  console.log('----- localVideoStateChanged ', store.selfUid, status, error)
  if (status === 2) {
    store.addVideoStream(store.selfUid)
    if (store.currentFullScreenVideoUid === -1) {
      store.setCurrentFullScreenVideoUid(store.selfUid)
    }
  }

})
// todo
sdk.onEvent('remoteVideoStateChanged', (uid, state, reason, elapsed) => { // enabled = 1/0 true/false
  console.log('----- remoteVideoStateChanged ', uid, state, reason, elapsed)
  if (state === 0) {
    store.removeVideoStream(uid)
  } else if(state === 2) {
    store.addVideoStream(uid)
  }
})


// todo
sdk.onEvent('localAudioStateChanged', (uid, enabled) => {
  console.log('----- localAudioStateChanged ', uid, enabled)
})
// todo
sdk.onEvent('remoteAudioStateChanged', (uid, state, reason) => {
  console.log('----- remoteAudioStateChanged ', uid, state, reason)
})


sdk.onEvent('streamMessage', (uid, content) => {
  Log.info('sdk wrapper streamMessage ', uid, content)
  store.addChat({
    uid,
    content,
  })
})


sdk.onEvent('audioVolumeIndication', (state) => {
  // console.log('----- audioVolumeIndication ', state)
  _.forEach(state, (volume, uid) => {
    store.updateUserVolume(uid, volume)
  })
})


sdk.onEvent('connectionStateChanged', (state) => {
  // console.log('----- connectionStateChanged ', state)
})
sdk.onEvent('networkQuality', (uid, txQuality, rxQuality) => {
  // console.log('----- networkQuality ', uid, txQuality, rxQuality)
  store.systemStatus.update({
    sendQuality: txQuality,
    receiveQuality: rxQuality,
  })
})


export default sdk
