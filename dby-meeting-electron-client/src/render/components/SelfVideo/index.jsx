import { autorun, observable, action, computed } from 'mobx'
import React, { Component, memo } from 'react'
import PropTypes from 'prop-types'
import { observer } from 'mobx-react'
import _ from 'lodash'

import UserVideoSmall from '../UserVideoSmall'
import UserNameForVideo from '../UserNameForVideo'

import sdkWrapper from '../../sdkwrapper'
import store from '../../store'

import './index.less'

@observer
class SelfVideo extends Component {
  static propTypes = {
    style: PropTypes.object,
  }

  static defaultProps = {
    style: {},
  }

  @observable.ref
  videoPlayerRef = null

  @computed
  get shouldSetUpLocalVideo() {
    let showLocalVideo = false
    let hasVideoFrame = false
    if (store.currentFullScreenVideoUid !== store.selfUid) {
      showLocalVideo = true
      if (_.indexOf(store.videoStreams.toJSON(), store.selfUid) !== -1) {
        hasVideoFrame = true
      }
    }
    return showLocalVideo && hasVideoFrame
  }

  @action
  getVideoPlayerRef = (ref) => {
    // Log.info('SelfVideo getVideoPlayerRef ', ref)
    this.videoPlayerRef = ref
  }

  cancelAutoRuns = []

  componentDidMount() {
    this.cancelAutoRuns = [
      autorun(() => {
        const { shouldSetUpLocalVideo, videoPlayerRef } = this
        Log.info('SelfVideo shouldSetUpLocalVideo ', shouldSetUpLocalVideo)
        if (this.shouldSetUpLocalVideo === true && videoPlayerRef !== null) {
          Log.info('SelfVideo call sdkWrapper.setupLocalVideo')
          sdkWrapper.setupLocalVideo(videoPlayerRef)
        }
      }, { name: 'auto set local video' })
    ]
  }

  componentWillUnmount() {
    _.forEach(this.cancelAutoRuns, f => f())
  }

  render() {
    return (
      <div className="self-video-component-wrap" style={this.props.style}>
        <UserNameForVideo
          uid={store.selfUid}
          style={{
            position: 'absolute',
            top: 0,
            left: 0,
            zIndex: 1,
            width: '100%'
          }}
        />
        <UserVideoSmall
          style={{
            width: '200px',
            height: '112px',
          }}
          getVideoPlayerRef={this.getVideoPlayerRef}
          hasVideoFrame={store.selfCameraOpened === true}
        />
      </div>
    )
  }
}

export default memo(SelfVideo, (prevProps, nextProps) => {
  return _.isEqual(prevProps, nextProps)
})
