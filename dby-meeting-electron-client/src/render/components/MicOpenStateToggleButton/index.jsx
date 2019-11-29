import React, {Component, createRef} from 'react'
import PropTypes from 'prop-types'
import _ from 'lodash'

import sdkWrapper from '../../sdkwrapper'
import store from '../../store'
import CircleButtonBase from '../CircleButtonBase'
import DeviceList from '../DeviceList'

import './index.less'

class MicOpenStateToggleButton extends Component {
  static propTypes = {
    style: PropTypes.object,
    onClick: PropTypes.func,
  };

  static defaultProps = {
    style: {},
    onClick: _.noop,
  };

  state = {
    showMicList: false,
    micList: [],
    currentMicIndex: 0,
  }

  wrapRef = createRef()

  buttonRef = createRef()

  handleClick = () => {
    this.props.onClick()
    this.setState({
      showMicList: !this.state.showMicList,
    })
  }

  handleSelectMicDevice = (i) => {
    const prevMicIndex = this.state.currentMicIndex
    this.setState({
      showMicList: false,
      currentMicIndex: i,
    })
    this.resetButtonActiveState()

    // 修改 sdk 麦克风设置
    if (i === this.state.micList.length - 1) {
      // 禁用
      sdkWrapper.enableLocalAudio(false)
    } else if (i !== prevMicIndex) {
      // 修改
      sdkWrapper.enableLocalAudio(true)
      sdkWrapper.setAudioRecordingDevice(i)
    }
  }

  resetButtonActiveState = () => {
    this.buttonRef.current.resetActiveState()
  }

  handleClickBody = (e) => {
    if (this.wrapRef.current.contains(e.target) === false) {
      this.setState({
        showMicList: false,
      })
      this.resetButtonActiveState()
    }
  }

  componentDidMount() {
    document.body.addEventListener('click', this.handleClickBody)

    // 初始化设备列表
    sdkWrapper.getAudioRecordingDevices().then((devices) => {
      this.setState({
        micList: _.map(devices, 'deviceName').concat('禁用')
      })
    })
  }

  componentWillUnmount() {
    document.body.removeEventListener('click', this.handleClickBody)
  }

  render() {
    const isMicClosed = this.state.currentMicIndex === this.state.micList.length - 1 // 最后一个为关闭

    return (
      <div className="mic-open-state-toggle-button" style={this.props.style} ref={this.wrapRef}>
        {
          this.state.showMicList === true && (
            <DeviceList
              style={{
                position: 'absolute',
                bottom: '45px',
                left: '-22px',
                zIndex: 1,
              }}
              data={this.state.micList}
              defaultIndex={this.state.currentMicIndex}
              onSelect={this.handleSelectMicDevice}
            />
          )
        }
        <CircleButtonBase
          ref={this.buttonRef}
          onClick={this.handleClick}
          normalIcon={
            isMicClosed === true
              ? require('../../static/image/icon-mic-closed-normal-72-72.png')
              : require('../../static/image/icon-mic-open-normal-72-72.png')
          }
          activeIcon={
            isMicClosed === true
              ? require('../../static/image/icon-mic-closed-active-72-72.png')
              : require('../../static/image/icon-mic-open-active-72-72.png')
          }
        />
      </div>
    )
  }
}

export default MicOpenStateToggleButton
