import React, { Component } from 'react'
import PropTypes from 'prop-types'

import './index.less'

class ChatItemRemote extends Component {
  static propTypes = {
    style: PropTypes.object,
    content: PropTypes.string.isRequired,
    uid: PropTypes.number.isRequired,
  };

  static defaultProps = {
    style: {},
  };

  render() {
    return (
      <div className="chat-item-remote-component-wrap" style={this.props.style}>
        <span className="user-name">{ this.props.uid }</span>
        <span className="chat-content">{ this.props.content }</span>
      </div>
    )
  }
}

export default ChatItemRemote
