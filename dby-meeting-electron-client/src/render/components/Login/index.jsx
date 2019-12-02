import React, { Component, createRef } from 'react'
import PropTypes from 'prop-types'
import _ from 'lodash'
import { remote } from 'electron'
import { observer } from 'mobx-react'

import Input from '../Input'
import CircleRectButtonBase from '../CircleRectButtonBase'
import RadioInput from '../RadioInput'

import sdkWrapper from '../../sdkwrapper'
import { SDK_LINES } from '../../../consts'
import store from '../../store'
import pkgJson from '../../../../package'

import './index.less'

@observer
class Login extends Component {
  static propTypes = {
    style: PropTypes.object,
    onLoginSuccess: PropTypes.func,
  };

  static defaultProps = {
    style: {},
    onLoginSuccess: _.noop,
  };

  state = {
    channelId: '',
    userId: '',
    sdkLine: store.currentSDKLine,
    isLoggingIn: false,
    joinChannelResultCode: 0,
  }

  componentDidMount() {
    const currentWindow = remote.getCurrentWindow()
    currentWindow.setContentSize(360, 490, true)
    currentWindow.center()
    currentWindow.setResizable(false)

    const userLoginInfo = localStorage.getItem('DBY_MEETING_USER_LOGIN_INFO')
    try {
      const info = JSON.parse(userLoginInfo)
      this.setState({
        channelId: info.channelId,
        userId: info.userId,
      })
    } catch (e) {
      Log.info('init user login info error ', e.message)
    }
  }

  componentWillUnmount() {
  }

  handleChooseSDKLine = (line) => {
    this.setState({
      sdkLine: line,
    })
  }

  inputChannelRef = createRef()

  inputIDRef = createRef()

  render() {
    return (
      <div className="login-component-wrap" style={this.props.style}>
        <span className="title">登入</span>
        <div className="input-form-wrap">
          <Input
            placeholder="频道号"
            rules={[
              { test: /.+/, level: 'error', message: '输入不能为空' }
            ]}
            defaultValue={this.state.channelId}
            ref={this.inputChannelRef}
          />
          <Input
            placeholder="ID"
            rules={[
              { test: /.+/, level: 'error', message: '输入不能为空' },
              { test: /^\d+$/, level: 'error', message: 'ID只支持纯数字' }
            ]}
            defaultValue={this.state.userId}
            ref={this.inputIDRef}
          />
          <div className="join-channel-error-tip">
            { this.state.joinChannelResultCode === 0 ? '' : `进入频道失败 ${this.state.joinChannelResultCode}` }
          </div>
          <CircleRectButtonBase
            text="进入频道"
            style={{
              width: '180px',
            }}
            enable={this.state.isLoggingIn === false}
            onClick={() => {
              const channelCheckResult = this.inputChannelRef.current.checkData()
              const idCheckResult = this.inputIDRef.current.checkData()
              const loginData = {
                channel: this.inputChannelRef.current.getValue(),
                uid: this.inputIDRef.current.getValue(),
                nickname: this.inputIDRef.current.getValue(),
              }
              localStorage.setItem('DBY_MEETING_USER_LOGIN_INFO', JSON.stringify({
                channelId: loginData.channel,
                userId: loginData.uid,
              }))
              if (this.state.isLoggingIn === false && channelCheckResult === true && idCheckResult === true) {
                this.setState({
                  isLoggingIn: true,
                })
                sdkWrapper.switchSDK(this.state.sdkLine)
                const now = Date.now()
                sdkWrapper.joinChannel(loginData).then((result) => {
                  Log.info('joinChannel use time: ', result, Date.now() - now)
                  if (result === 0) {
                    store.update({
                      selfUid: parseInt(loginData.uid, 10),
                      currentSDKLine: this.state.sdkLine,
                    })
                    this.props.onLoginSuccess()
                  } else {
                    this.setState({
                      joinChannelResultCode: result,
                    })
                  }
                }).finally(() => {
                  this.setState({
                    isLoggingIn: false,
                  })
                })
              }
            }}
          />
        </div>
        {
          true && (
            <div className="choose-line-wrap">
              <div
                className="line-info-wrap"
                onClick={() => {
                  this.handleChooseSDKLine(SDK_LINES.DBY)
                }}
              >
                <RadioInput checked={this.state.sdkLine === SDK_LINES.DBY} />
                <span className="line-info">线路1-多贝</span>
              </div>
              <div
                className="line-info-wrap"
                onClick={() => {
                  this.handleChooseSDKLine(SDK_LINES.AGORA)
                }}
              >
                <RadioInput checked={this.state.sdkLine === SDK_LINES.AGORA} />
                <span className="line-info">线路2-声网</span>
              </div>
            </div>
          )
        }
        <span className="app-version">v{pkgJson.version}</span>
      </div>
    )
  }
}

export default Login
