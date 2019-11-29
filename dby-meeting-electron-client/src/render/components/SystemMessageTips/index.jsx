import React, { Component, createRef } from 'react'
import PropTypes from 'prop-types'
import _ from 'lodash'
import { observer } from 'mobx-react'

import store from '../../store'

import './index.less'

@observer
class SystemMessageTips extends Component {
  static propTypes = {
    style: PropTypes.object,
  }

  static defaultProps = {
    style: {},
  }

  wrapElemRef = createRef()

  componentDidUpdate() {
    this.wrapElemRef.current.scrollTop = 10e10
  }

  render() {
    return (
      <div className="system-message-tips-component-wrap" ref={this.wrapElemRef} style={this.props.style}>
        {
          _.map(store.tipList.toJSON(), (tipInfo) => (
            <div className="tip-item-wrap" key={tipInfo.id}>
              { tipInfo.content }
            </div>
          ))
        }
      </div>
    )
  }
}

export default SystemMessageTips
