import React, { Component } from 'react'
import PropTypes from 'prop-types'

import './index.less'

class RadioInput extends Component {
  static propTypes = {
    style: PropTypes.object,
    checked: PropTypes.bool,
  };

  static defaultProps = {
    style: {},
    checked: false,
  };

  render() {
    return (
      <div className="radio-input-component-wrap" style={this.props.style}>
        <img
          src={this.props.checked === true ? require('../../static/image/icon-radio-input-checked-42-42.png') : require('../../static/image/icon-radio-input-unchecked-42-42.png')}
        />
      </div>
    )
  }
}

export default RadioInput
