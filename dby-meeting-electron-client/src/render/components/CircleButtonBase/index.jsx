import React, { Component, memo } from 'react'
import PropTypes from 'prop-types'
import _ from 'lodash'

import './index.less'

class CircleButtonBase extends Component {
  static getDerivedStateFromProps(props, state) {
    if (_.isBoolean(props.isActive)) {
      return {
        ...state,
        isActive: props.isActive === true,
      }
    }
    return null
  }

  static propTypes = {
    style: PropTypes.object,
    text: PropTypes.string,
    normalIcon: PropTypes.string.isRequired, // url
    activeIcon: PropTypes.string.isRequired, // url
    onClick: PropTypes.func,
    isInitActive: PropTypes.oneOf([false, true, null]),
    isActive: PropTypes.oneOf([false, true, null]),
  };

  static defaultProps = {
    style: {},
    text: '',
    onClick: _.noop,
    isInitActive: false,
    isActive: null,
  };

  state = {
    isActive: this.props.isInitActive === true,
  }

  componentDidMount() {
  }

  resetActiveState = () => {
    this.setState({
      isActive: false,
    })
  }

  handleClick = () => {
    const isActive = !this.state.isActive
    this.setState({
      isActive,
    })
    this.props.onClick()
  }

  render() {
    return (
      <div
        className="circle-button-base-wrap"
        style={this.props.style}
        title={this.props.text}
        onClick={this.handleClick}
      >
        <img src={this.state.isActive ? this.props.activeIcon : this.props.normalIcon} alt="" className="icon"/>
      </div>
    )
  }
}

export default memo(CircleButtonBase)
