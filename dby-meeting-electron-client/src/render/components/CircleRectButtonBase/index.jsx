import React, { Component, memo } from 'react'
import PropTypes from 'prop-types'
import _ from 'lodash'
import cx from 'classnames'

import './index.less'

class CircleRectButtonBase extends Component {
  static propTypes = {
    style: PropTypes.object,
    text: PropTypes.string,
    onClick: PropTypes.func,
    enable: PropTypes.bool,
  };

  static defaultProps = {
    style: {},
    text: '',
    onClick: _.noop,
    enable: true,
  };

  handleClick = () => {
    this.props.onClick()
  }

  render() {
    return (
      <div
        className={cx({
          'circle-rect-button-base-wrap': true,
          disabled: this.props.enable !== true,
        })}
        style={this.props.style}
        title={this.props.text}
        onClick={this.handleClick}
      >
        { this.props.text }
      </div>
    )
  }
}

export default memo(CircleRectButtonBase)
