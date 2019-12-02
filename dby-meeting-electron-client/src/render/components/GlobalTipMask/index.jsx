import React, { Component } from 'react'
import PropTypes from 'prop-types'

import './index.less'

class GlobalTipMask extends Component {
  static propTypes = {
    style: PropTypes.object,
    tipContent: PropTypes.string.isRequired,
  };

  static defaultProps = {
    style: {},
  };

  render() {
    return (
      <div className="global-tip-mask-component-wrap" style={this.props.style}>
        <div className="tip-content">
          { this.props.tipContent }
        </div>
      </div>
    )
  }
}

export default GlobalTipMask
