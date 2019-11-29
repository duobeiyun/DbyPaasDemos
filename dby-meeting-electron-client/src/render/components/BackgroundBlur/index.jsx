import React, { Component } from 'react'
import PropTypes from 'prop-types'

import './index.less'

class BackgroundBlur extends Component {
  static propTypes = {
    style: PropTypes.object,
  };

  static defaultProps = {
    style: {},
  };

  render() {
    return (
      <div className="background-blur-component-wrap" style={this.props.style} />
    )
  }
}

export default BackgroundBlur
