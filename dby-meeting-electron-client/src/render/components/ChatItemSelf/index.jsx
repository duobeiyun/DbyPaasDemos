import React, { Component } from 'react'
import PropTypes from 'prop-types'

import './index.less'

class ChatItemSelf extends Component {
  static propTypes = {
    style: PropTypes.object,
    content: PropTypes.string.isRequired,
  };

  static defaultProps = {
    style: {},
  };

  render() {
    return (
      <div className="chat-item-self-component-wrap" style={this.props.style}>
        <span className="chat-content">{ this.props.content }</span>
      </div>
    )
  }
}

export default ChatItemSelf
