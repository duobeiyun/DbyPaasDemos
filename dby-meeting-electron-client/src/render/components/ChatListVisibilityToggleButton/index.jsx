import React, { Component } from 'react'
import PropTypes from 'prop-types'
import _ from 'lodash'

import CircleButtonBase from '../CircleButtonBase'

import './index.less'

class ChatListVisibilityToggleButton extends Component {
  static propTypes = {
    style: PropTypes.object,
    onClick: PropTypes.func,
    isActive: PropTypes.bool,
  };

  static defaultProps = {
    style: {},
    onClick: _.noop,
    isActive: false,
  };

  handleClick = () => {
    this.props.onClick()
  }

  render() {
    return (
      <div className="chat-list-visibility-toggle-button" style={this.props.style}>
        <CircleButtonBase
          onClick={this.handleClick}
          normalIcon={require('../../static/image/icon-message-normal-72-72.png')}
          activeIcon={require('../../static/image/icon-message-active-72-72.png')}
          isActive={this.props.isActive === true}
        />
      </div>
    )
  }
}

export default ChatListVisibilityToggleButton
