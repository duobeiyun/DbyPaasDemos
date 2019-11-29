import React, { Component } from 'react'
import PropTypes from 'prop-types'
import { observer } from 'mobx-react'

import RadioInput from '../RadioInput'

import { SDK_LINES } from '../../../consts'

import store from '../../store'

import './index.less'

@observer
class ChooseSDK extends Component {
  static propTypes = {
    style: PropTypes.object,
  };

  static defaultProps = {
    style: {},
  };

  state = {
    sdkLine: store.currentSDKLine,
  }

  chooseSDKLine = (line) => {
    this.setState({
      sdkLine: line,
    })
    store.setCurrentSDKLine(line)
  }

  render() {
    return (
      <div className="choose-sdk-component-wrap" style={this.props.style}>
        <span className="tip">线路：</span>
        {
          this.state.sdkLine === SDK_LINES.DBY && (
            <div
              className="choose-line-wrap"
              onClick={() => {
                this.chooseSDKLine(SDK_LINES.DBY)
              }}
            >
              <RadioInput checked={this.state.sdkLine === SDK_LINES.DBY} />
              <span className="line-name">线路1-多贝云</span>
            </div>
          )
        }
        {
          this.state.sdkLine === SDK_LINES.AGORA && (
            <div
              className="choose-line-wrap"
              onClick={() => {
                this.chooseSDKLine(SDK_LINES.AGORA)
              }}
            >
              <RadioInput checked={this.state.sdkLine === SDK_LINES.AGORA} />
              <span className="line-name">线路2-声网</span>
            </div>
          )
        }
      </div>
    )
  }
}

export default ChooseSDK
