import React, { Component } from 'react'
import PropTypes from 'prop-types'
import { observer } from 'mobx-react'
import { remote } from 'electron'
import _ from 'lodash'

import SystemMessageTips from '../SystemMessageTips'
import SystemStatusInfo from '../SystemStatusInfo'
import UserVideoFullScreen from '../UserVideoFullScreen'
import UserVideoList from '../UserVideoList'
import ChatList from '../ChatList'
import OnlineUserList from '../OnlineUserList'
import MainControlPanel from '../MainControlPanel'
import SelfVideo from '../SelfVideo'
import GlobalTipMask from '../GlobalTipMask'

import store from '../../store'
import sdkWrapper from '../../sdkwrapper'

import './index.less'

const { onlineUsers } = store

@observer
class App extends Component {
  static propTypes = {
    style: PropTypes.object,
    onHangup: PropTypes.func,
  };

  static defaultProps = {
    style: {},
    onHangup: _.noop,
  };

  state = {
    showUserVideoList: true,
    showOnlineUserList: false,
    showChatList: true,
    showGlobalTipMask: false,
  }

  componentDidMount() {
    const currentWindow = remote.getCurrentWindow()
    currentWindow.setResizable(true)
    currentWindow.maximize()
  }

  handleMainControlButtonStateChange = (state) => {
    store.update({
      showUserVideoList: state.isUserVideoListButtonActive === true,
    })
    this.setState({
      showUserVideoList: state.isUserVideoListButtonActive === true,
      showOnlineUserList: state.isOnlineUserListButtonActive === true,
      showChatList: state.isChatListButtonActive === true,
    })
  }

  isLeavingChannel = false

  handleHangUp = () => {
    if (this.isLeavingChannel === false) {
      this.isLeavingChannel = true
      sdkWrapper.leaveChannel().then(() => {
        this.props.onHangup()
      }).finally(() => {
        this.isLeavingChannel = false
      })
    }
  }

  render() {
    return (
      <div className="app-component-wrap" style={this.props.style}>
        <div className="main-video-wrap">
          <UserVideoFullScreen
            style={{
              position: 'relative',
              zIndex: 1,
            }}
          />
          {
            onlineUsers.length > 0 && (
              <SelfVideo
                style={{
                  position: 'absolute',
                  left: '50%',
                  bottom: '90px',
                  transform: 'translate(-50%, 0)',
                  zIndex: 2,
                }}
              />
            )
          }
          <SystemStatusInfo
            style={{
              position: 'absolute',
              top: 0,
              left: '50%',
              transform: 'translate(-50%, 0)',
              zIndex: 2,
            }}
          />
        </div>
        <div className="interactive-area-wrap">
          <SystemMessageTips
            style={{
              position: 'absolute',
              top: '30px',
              right: 300,
              zIndex: 2,
            }}
          />
          <div className="left-panel-wrap">
            {
              this.state.showUserVideoList === true && (
                <UserVideoList />
              )
            }
          </div>
          <div className="center-panel-wrap">
            <MainControlPanel
              style={{
                position: 'absolute',
                bottom: 0,
                left: 0,
              }}
              onButtonStateChange={this.handleMainControlButtonStateChange}
              onClickHangUpButton={this.handleHangUp}
            />
          </div>
          <div className="right-panel-wrap">
            {
              this.state.showOnlineUserList === true && (
                <OnlineUserList />
              )
            }
            {
              this.state.showChatList === true && (
                <ChatList />
              )
            }
          </div>
        </div>
        {
          this.state.showGlobalTipMask === true && (
            <GlobalTipMask
              style={{
                position: 'absolute',
                top: 0,
                left: 0,
                zIndex: 3
              }}
            />
          )
        }
      </div>
    )
  }
}

export default App
