import React, { Component } from 'react'
import PropTypes from 'prop-types'
import _ from 'lodash'

import CircleButtonBase from '../CircleButtonBase'

import './index.less'

class UserVideoListVisibilityToggleButton extends Component {
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
      <div className="user-video-list-visibility-toggle-button" style={this.props.style}>
        <CircleButtonBase
          onClick={this.handleClick}
          isInitActive={true}
          normalIcon={require('../../static/image/icon-side-bar-normal-72-72.png')}
          activeIcon={require('../../static/image/icon-side-bar-active-72-72.png')}
        />
      </div>
    )
  }
}

export default UserVideoListVisibilityToggleButton
