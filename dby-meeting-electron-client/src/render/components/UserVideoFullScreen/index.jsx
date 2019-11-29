import _ from 'lodash'
import React, { Component, memo } from 'react'
import PropTypes from 'prop-types'
import { observer } from 'mobx-react'
import { trace, observable, autorun, action, computed } from 'mobx'

import UserVideoSmall from '../UserVideoSmall'
import UserNameForVideo from '../UserNameForVideo'

import sdkWrapper from '../../sdkwrapper'
import store from '../../store'

import './index.less'

@observer
class UserVideoFullScreen extends Component {
  static propTypes = {
    style: PropTypes.object,
  };

  static defaultProps = {
    style: {},
  };

  cancelAutoRuns = []

  isBigStream = false

  @observable.ref
  videoPlayerRef = null

  @computed
  get hasVideoFrame() {
    const result = _.indexOf(store.videoStreams.toJSON(), store.currentFullScreenVideoUid) !== -1
    Log.info('UserVideoFullScreen get hasVideoFrame ', store.videoStreams.toJSON(), store.currentFullScreenVideoUid, result)
    return result
  }

  @computed
  get shouldSubscribeRemoteVideo() {
    let showRemoteVideo = false
    if (store.currentFullScreenVideoUid !== -1 && store.currentFullScreenVideoUid !== store.selfUid) {
      showRemoteVideo = true
    }
    return showRemoteVideo && this.hasVideoFrame
  }

  @computed
  get shouldSetUpLocalVideo() {
    let showLocalVideo = false
    if (store.currentFullScreenVideoUid === store.selfUid) {
      showLocalVideo = true
    }
    return showLocalVideo && this.hasVideoFrame
  }

  @action
  getVideoPlayerRef = (ref) => {
    Log.info('UserVideoFullScreen getVideoPlayerRef ', _.isNil(ref))
    this.videoPlayerRef = ref
  }

  componentDidMount() {
    this.cancelAutoRuns = [
      autorun(() => {
        trace()
        const { shouldSubscribeRemoteVideo, videoPlayerRef } = this
        Log.info('UserVideoFullScreen shouldSubscribeRemoteVideo ', shouldSubscribeRemoteVideo, _.isNil(videoPlayerRef))
        if (shouldSubscribeRemoteVideo === true) {
          Log.info('UserVideoFullScreen setRemoteVideoStreamType ', store.currentFullScreenVideoUid, 0)
          const result = sdkWrapper.setRemoteVideoStreamType(store.currentFullScreenVideoUid, 0) // 切成大流
          Log.info('UserVideoFullScreen setRemoteVideoStreamType result ', result)
          sdkWrapper.subscribe(store.currentFullScreenVideoUid, videoPlayerRef)
          this.isBigStream = true
        }
      }),
      autorun(() => {
        trace()
        const { shouldSetUpLocalVideo, videoPlayerRef } = this
        Log.info('UserVideoFullScreen shouldSetUpLocalVideo ', shouldSetUpLocalVideo, _.isNil(videoPlayerRef))
        if (shouldSetUpLocalVideo === true) {
          sdkWrapper.setupLocalVideo(videoPlayerRef)
        }
      })
    ]

    const buttonElem = document.createElement('button')
    buttonElem.innerText = 'toggle'
    buttonElem.style.position = 'fixed'
    buttonElem.style.top = '10px'
    buttonElem.style.left = '10px'
    buttonElem.style.padding = '10px'
    buttonElem.style.zIndex = '200'
    // document.body.appendChild(buttonElem)
    buttonElem.onclick = () => {
      const streamType = this.isBigStream ? 1 : 0
      Log.info('UserVideoFullScreen setRemoteVideoStreamType clicked ', streamType)
      const result = sdkWrapper.setRemoteVideoStreamType(store.currentFullScreenVideoUid, this.isBigStream ? 1 : 0) // 切成大流
      Log.info('UserVideoFullScreen setRemoteVideoStreamType clicked result ', streamType, result)
      this.isBigStream = !this.isBigStream
    }
  }

  componentWillUnmount() {
    _.forEach(this.cancelAutoRuns, f => f())
  }

  render() {
    trace()
    return (
      <div className="user-video-full-screen-component-wrap" style={this.props.style}>
        <UserNameForVideo
          uid={store.currentFullScreenVideoUid}
          showIcon={true}
          style={{
            position: 'absolute',
            top: 40,
            left: store.showUserVideoList === true ? '220px' : '0px',
            zIndex: 1,
          }}
        />
        <UserVideoSmall
          style={{
            width: '100%',
            height: '100%',
          }}
          key={store.currentFullScreenVideoUid}
          getVideoPlayerRef={this.getVideoPlayerRef}
          hasVideoFrame={this.hasVideoFrame}
        />
      </div>
    )
  }
}

export default memo(UserVideoFullScreen, (prevProps, nextProps) => {
  return _.isEqual(prevProps, nextProps)
})
