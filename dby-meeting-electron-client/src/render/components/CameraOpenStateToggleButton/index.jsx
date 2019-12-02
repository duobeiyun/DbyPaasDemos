import React, { Component, createRef } from 'react'
import PropTypes from 'prop-types'
import _ from 'lodash'

import CircleButtonBase from '../CircleButtonBase'
import DeviceList from '../DeviceList'

import store from '../../store'
import sdkWrapper from '../../sdkwrapper'
import './index.less'

class CameraOpenStateToggleButton extends Component {
  static propTypes = {
    style: PropTypes.object,
    onClick: PropTypes.func,
  };

  static defaultProps = {
    style: {},
    onClick: _.noop,
  };

  state = {
    showCameraList: false,
    cameraList: [],
    currentCameraIndex: 0,
  }

  wrapRef = createRef()

  buttonRef = createRef()

  handleClickButton = () => {
    this.props.onClick()
    this.setState({
      showCameraList: !this.state.showCameraList,
    })
  }

  handleSelectCameraDevice = (i) => {
    Log.info('handleSelectCameraDevice ', i)
    const prevCameraIndex = this.state.currentCameraIndex
    this.setState({
      showCameraList: false,
      currentCameraIndex: i,
    })
    this.resetButtonActiveState()

    // 修改 sdk 摄像头设置
    if (i === this.state.cameraList.length - 1) {
      // 禁用
      sdkWrapper.enableLocalVideo(false)
      store.removeVideoStream(store.selfUid) // 把自己从 videoStreams 里移除
    } else if (i !== prevCameraIndex) {
      // 修改
      sdkWrapper.enableLocalVideo(true)
      sdkWrapper.setVideoDevice(i)
    }
  }

  resetButtonActiveState = () => {
    this.buttonRef.current.resetActiveState()
  }

  handleClickBody = (e) => {
    if (this.wrapRef.current.contains(e.target) === false) {
      this.setState({
        showCameraList: false,
      })
      this.resetButtonActiveState()
    }
  }

  componentDidMount() {
    document.body.addEventListener('click', this.handleClickBody)

    // 初始化设备列表
    sdkWrapper.getVideoDevices().then((cameraDevices) => {
      this.setState({
        cameraList: _.map(cameraDevices, 'deviceName').concat('禁用')
      })
    })
  }

  componentWillUnmount() {
    document.body.removeEventListener('click', this.handleClickBody)
  }

  render() {
    const isCameraClosed = this.state.currentCameraIndex === this.state.cameraList.length - 1 // 最后一个为关闭

    return (
      <div className="camera-open-state-toggle-button" style={this.props.style} ref={this.wrapRef}>
        {
          this.state.showCameraList === true && (
            <DeviceList
              style={{
                position: 'absolute',
                bottom: '45px',
                left: '-22px',
                zIndex: 1,
              }}
              data={this.state.cameraList}
              defaultIndex={this.state.currentCameraIndex}
              onSelect={this.handleSelectCameraDevice}
            />
          )
        }
        <CircleButtonBase
          ref={this.buttonRef}
          onClick={this.handleClickButton}
          normalIcon={
            isCameraClosed === true
              ? require('../../static/image/icon-camera-closed-normal-72-72.png')
              : require('../../static/image/icon-camera-open-normal-72-72.png')
          }
          activeIcon={
            isCameraClosed === true
              ? require('../../static/image/icon-camera-closed-active-72-72.png')
              : require('../../static/image/icon-camera-open-active-72-72.png')
          }
        />
      </div>
    )
  }
}

export default CameraOpenStateToggleButton
