import React, {Component} from 'react'
import PropTypes from 'prop-types'

import './index.less'

import sdk from '../../sdkwrapper'

const micOpenIcon = require('../../static/image/icon-mic-open-active-72-72.png')
const micCloseIcon = require('../../static/image/icon-mic-closed-active-72-72.png')
const cameraOpenIcon = require('../../static/image/icon-camera-open-active-72-72.png')
const cameraCloseIcon = require('../../static/image/icon-camera-closed-active-72-72.png')

class OnlineUserListItem extends Component {
  static propTypes = {
    style: PropTypes.object,
    uid: PropTypes.string.isRequired,
  }

  static defaultProps = {
    style: {},
  }

  state = {
    micOpening: true,
    cameraOpening: true,
  }

  handleClickCameraButton = () => {
    sdk.muteRemoteVideoStream(this.props.uid, this.state.cameraOpening)
    this.setState({
      cameraOpening: !this.state.cameraOpening
    })
  }

  handleClickMicButton = () => {
    const nextState = !this.state.micOpening
    sdk.muteRemoteAudioStream(this.props.uid, nextState)
    this.setState({
      micOpening: nextState
    })
  }

  render() {
    const { uid, style } = this.props
    const { micOpening, cameraOpening } = this.state

    return (
      <div className="online-user-list-item-component-wrap" style={style}>
        <div className="user-info" key={uid}>
          <span className="icon-video-button" onClick={this.handleClickCameraButton}>
            <img src={cameraOpening ? cameraOpenIcon : cameraCloseIcon} alt=""/>
          </span>
          <span className="icon-audio-button" onClick={this.handleClickMicButton}>
            <img src={micOpening ? micOpenIcon : micCloseIcon} alt=""/>
          </span>
          <span className="user-name">{uid}</span>
        </div>
      </div>
    )
  }
}

export default OnlineUserListItem
