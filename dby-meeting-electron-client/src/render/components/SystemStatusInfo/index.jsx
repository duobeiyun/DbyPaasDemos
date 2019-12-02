import React, { Component } from 'react'
import PropTypes from 'prop-types'
import { observer } from 'mobx-react'

import { NETWORK_QUALITY_TYPE } from '../../../consts'
import store from '../../store'

import './index.less'

@observer
class SystemStatusInfo extends Component {
  static propTypes = {
    style: PropTypes.object,
  };

  static defaultProps = {
    style: {},
  };

  state = {
    duration: 0,
  }

  networkQuality = _.invert(NETWORK_QUALITY_TYPE)

  updateDurationIntervalFlag

  formatSecondsToMMSS = (seconds) => {
    const m = Math.floor(seconds / 60)
    const s = Math.floor(seconds % 60)
    return `${m >= 10 ? m : `0${m}`}:${s >= 10 ? s : `0${s}`}`
  }

  componentDidMount() {
    const startTimestamp = Date.now()
    this.updateDurationIntervalFlag = setInterval(() => {
      this.setState({
        duration: Date.now() - startTimestamp
      })
    }, 1000)
  }

  componentWillUnmount() {
    clearInterval(this.updateDurationIntervalFlag)
  }

  render() {
    const { sendQuality, receiveQuality, cpuUsage, memoryUsage } = store.systemStatus

    return (
      <div className="system-status-info-component-wrap" style={this.props.style}>
        <span className="item">上传：{this.networkQuality[sendQuality].split('_')[1].toLowerCase()}</span>
        <span className="item">下载：{this.networkQuality[receiveQuality].split('_')[1].toLowerCase()}</span>
        <span className="item">&nbsp;|&nbsp;</span>
        <span className="item">CPU：{cpuUsage}%</span>
        <span className="item">内存：{memoryUsage}MB </span>
        <span className="item">&nbsp;|&nbsp;</span>
        <span className="item">时长：{this.formatSecondsToMMSS(Math.floor(this.state.duration / 1000))}</span>
      </div>
    )
  }
}

export default SystemStatusInfo
