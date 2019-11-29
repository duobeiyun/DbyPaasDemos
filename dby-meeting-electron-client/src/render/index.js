import 'babel-polyfill'
import React, { Component } from 'react'
import ReactDOM from 'react-dom'
import { ipcRenderer } from 'electron'

import { IPC_EVENT } from '../consts'
import log from '../utils/log'

import App from './components/App'
import Login from './components/Login'

import pkgJSON from '../../package'
import sdkWrapper from './sdkwrapper'
import './calcCPUMemoryUsage'

import './crashReporter'

// fixme 测试阶段把所有日志重定向到文件
// console.log = log.log

const appContainer = document.getElementById('app-container')

class Index extends Component {
  state = {
    loginSuccess: false,
  }

  handleHangUp = () => {
    this.setState({
      loginSuccess: false,
    })
  }

  render() {
    if (this.state.loginSuccess !== true) {
      return (
        <Login
          onLoginSuccess={() => {
            this.setState({
              loginSuccess: true,
            })
            ipcRenderer.send(IPC_EVENT.DISABLE_QUIT)
          }}
        />
      )
    }

    return (
      <App
        onHangup={this.handleHangUp}
      />
    )
  }
}

Log.info('PaaS 1vN demo version: ', pkgJSON.version)

ipcRenderer.on(IPC_EVENT.BEFORE_QUIT, () => {
  sdkWrapper.release().then(() => {
    ipcRenderer.send(IPC_EVENT.APP_QUIT)
  })
})

ReactDOM.render(
  <Index />,
  appContainer,
  () => {
    Log.info('rendered!')
  },
)
