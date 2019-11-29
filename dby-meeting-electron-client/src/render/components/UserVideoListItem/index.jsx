import React, { Component, memo } from 'react'
import PropTypes from 'prop-types'
import _ from 'lodash'
import { observer } from 'mobx-react'
import { autorun, action, observable, computed } from 'mobx'

import UserNameForVideo from '../UserNameForVideo'
import UserVideoSmall from '../UserVideoSmall'
import sdkWrapper from '../../sdkwrapper'

import store from '../../store'

import './index.less'

@observer
class UserVideoListItem extends Component {
  static propTypes = {
    style: PropTypes.object,
    uid: PropTypes.number.isRequired,
  };

  static defaultProps = {
    style: {},
  };

  cancelAutoRuns = []

  @computed
  get remoteVideoHasFrame() {
    const hasVideoFrame = _.indexOf(store.videoStreams.toJSON(), this.props.uid) !== -1
    return hasVideoFrame
  }

  @observable.ref
  videoPlayerRef = null

  @action
  getVideoPlayerRef = (ref) => {
    Log.info('UserVideoListItem getVideoPlayerRef ', _.isNil(ref))
    this.videoPlayerRef = ref
  }

  handleClickMaximizeBtn = () => {
    store.update({
      currentFullScreenVideoUid: this.props.uid,
    })
  }

  componentDidMount() {
    this.cancelAutoRuns = [
      autorun(() => {
        const { remoteVideoHasFrame, videoPlayerRef } = this
        const { uid } = this.props
        Log.info('UserVideoListItem remoteVideoHasFrame ', remoteVideoHasFrame, _.isNil(videoPlayerRef))
        if (remoteVideoHasFrame === true && videoPlayerRef !== null) {
          Log.info('UserVideoListItem setRemoteVideoStreamType ', uid, 1)
          const result = sdkWrapper.setRemoteVideoStreamType(uid, 1) // 切成小流
          Log.info('UserVideoListItem setRemoteVideoStreamType result ', result)
          sdkWrapper.subscribe(uid, videoPlayerRef)
        }
      }, { name: 'auto subscribe video stream' }),
    ]
  }

  componentWillUnmount() {
    _.forEach(this.cancelAutoRuns, f => f())
  }

  render() {
    Log.info('UserVideoListItem render ', this.props.uid)
    return (
      <div className="user-video-list-item-wrap" style={this.props.style}>
        <UserNameForVideo
          uid={this.props.uid}
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
            width: '100%',
            height: '100%',
          }}
          key={this.props.uid}
          hasVideoFrame={this.remoteVideoHasFrame === true}
          getVideoPlayerRef={this.getVideoPlayerRef}
        />
        <span
          className="btn-maximize-video"
          onClick={this.handleClickMaximizeBtn}
        >
          <img src={require('../../static/image/icon-max-video-60-60.png')} alt=""/>
        </span>
      </div>
    )
  }
}

export default memo(UserVideoListItem)
