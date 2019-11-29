import React, { Component } from 'react'
import PropTypes from 'prop-types'
import _ from 'lodash'

import CircleButtonBase from '../CircleButtonBase'

import './index.less'

class HangUpButton extends Component {
  static propTypes = {
    style: PropTypes.object,
    onClick: PropTypes.func,
  };

  static defaultProps = {
    style: {},
    onClick: _.noop,
  };

  handleClick = () => {
    this.props.onClick()
  }

  render() {
    return (
      <div className="hang-up-button-component-wrap" style={this.props.style}>
        <CircleButtonBase
          onClick={this.handleClick}
          normalIcon={require('../../static/image/icon-hang-up-72-72.png')}
          activeIcon={require('../../static/image/icon-hang-up-72-72.png')}
        />
      </div>
    )
  }
}

export default HangUpButton
