import React, { Component } from 'react'
import PropTypes from 'prop-types'
import _ from 'lodash'

import UserVideoListVisibilityToggleButton from '../UserVideoListVisibilityToggleButton'
import OnlineUserListVisibilityToggleButton from '../OnlineUserListVisibilityToggleButton'
import ChatListVisibilityToggleButton from '../ChatListVisibilityToggleButton'
import CameraOpenStateToggleButton from '../CameraOpenStateToggleButton'
import MicOpenStateToggleButton from '../MicOpenStateToggleButton'
import HangUpButton from '../HangUpButton'
import ChooseSDK from '../ChooseSDK'
import BackgroundBlur from '../BackgroundBlur'

import './index.less'

class MainControlPanel extends Component {
  static propTypes = {
    style: PropTypes.object,
    onButtonStateChange: PropTypes.func,
    onClickHangUpButton: PropTypes.func,
  };

  static defaultProps = {
    style: {},
    onButtonStateChange: _.noop,
    onClickHangUpButton: _.noop,
  };

  state = {
    isUserVideoListButtonActive: true,
    isCameraOpenButtonActive: false,
    isMicOpenButtonActive: false,
    isChatListButtonActive: true,
    isOnlineUserListButtonActive: false,
  }

  BUTTON_NAMES = {
    USER_VIDEO_LIST: 'USER_VIDEO_LIST',
    CAMERA_OPEN: 'CAMERA_OPEN',
    MIC_OPEN: 'MIC_OPEN',
    CHAT_LIST: 'CHAT_LIST',
    ONLINE_USER_LIST: 'ONLINE_USER_LIST',
  }

  handleClickButtons = (buttonName) => {
    const newState = { ...this.state }
    switch (buttonName) {
      case this.BUTTON_NAMES.USER_VIDEO_LIST:
        newState.isUserVideoListButtonActive = !newState.isUserVideoListButtonActive
        break
      case this.BUTTON_NAMES.CAMERA_OPEN:
        newState.isCameraOpenButtonActive = !newState.isCameraOpenButtonActive
        break
      case this.BUTTON_NAMES.MIC_OPEN:
        newState.isMicOpenButtonActive = !newState.isMicOpenButtonActive
        break
      case this.BUTTON_NAMES.CHAT_LIST:
        newState.isChatListButtonActive = !newState.isChatListButtonActive
        newState.isOnlineUserListButtonActive = false
        break
      case this.BUTTON_NAMES.ONLINE_USER_LIST:
        newState.isChatListButtonActive = false
        newState.isOnlineUserListButtonActive = !newState.isOnlineUserListButtonActive
        break
    }
    this.setState(newState)
    this.props.onButtonStateChange({ ...newState })
  }

  render() {
    return (
      <div className="main-control-panel-component-wrap" style={this.props.style}>
        <div className="buttons-wrap">
          <UserVideoListVisibilityToggleButton
            onClick={() => {
              this.handleClickButtons(this.BUTTON_NAMES.USER_VIDEO_LIST)
            }}
          />
          <CameraOpenStateToggleButton
            style={{
              marginLeft: '40px',
            }}
            onClick={() => {
              this.handleClickButtons(this.BUTTON_NAMES.CAMERA_OPEN)
            }}
          />
          <MicOpenStateToggleButton
            style={{
              marginLeft: '40px',
            }}
            onClick={() => {
              this.handleClickButtons(this.BUTTON_NAMES.MIC_OPEN)
            }}
          />
          <ChatListVisibilityToggleButton
            style={{
              marginLeft: '40px',
            }}
            onClick={() => {
              this.handleClickButtons(this.BUTTON_NAMES.CHAT_LIST)
            }}
            isActive={this.state.isChatListButtonActive === true}
          />
          <OnlineUserListVisibilityToggleButton
            style={{
              marginLeft: '40px',
            }}
            onClick={() => {
              this.handleClickButtons(this.BUTTON_NAMES.ONLINE_USER_LIST)
            }}
            isActive={this.state.isOnlineUserListButtonActive === true}
          />
          <HangUpButton
            style={{
              marginLeft: '40px',
            }}
            onClick={this.props.onClickHangUpButton}
          />
        </div>
        {
          false && (
            <div className="choose-sdk-wrap">
              <ChooseSDK />
            </div>
          )
        }
        <BackgroundBlur />
      </div>
    )
  }
}

export default MainControlPanel
