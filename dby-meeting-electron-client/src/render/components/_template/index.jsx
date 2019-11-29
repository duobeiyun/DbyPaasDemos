import React, { Component } from 'react'
import PropTypes from 'prop-types'

import './index.less'

class template extends Component {
  static propTypes = {
    style: PropTypes.object,
  };

  static defaultProps = {
    style: {},
  };

  render() {
    return (
      <div className="template-wrap" style={this.props.style}>
        template
      </div>
    )
  }
}

export default template
