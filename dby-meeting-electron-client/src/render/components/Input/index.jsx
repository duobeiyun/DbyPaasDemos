import React, { Component, createRef } from 'react'
import PropTypes from 'prop-types'
import classnames from 'classnames'
import _ from 'lodash'

import './index.less'

class Input extends Component {
  static propTypes = {
    style: PropTypes.object,
    placeholder: PropTypes.string,
    rules: PropTypes.array, // { test, level, message }, level: [error, warn]
    onChange: PropTypes.func,
    defaultValue: PropTypes.string,
  };

  static defaultProps = {
    style: {},
    placeholder: '',
    rules: [],
    onChange: _.noop,
    defaultValue: '',
  };

  state = {
    showInvalidTip: false,
    tipContent: '',
    tipLevel: '', // error, warn
  }

  inputElemRef = createRef()

  handleInputChange = () => {
    this.props.onChange(this.inputElemRef.current.value)
  }

  checkData = () => {
    const { rules } = this.props
    const { inputElemRef } = this
    const inputContent = inputElemRef.current.value
    let inputContentValid = true
    if (rules.length > 0) {
      for (let i = 0; i < rules.length; i += 1) {
        const rule = rules[i]
        if (rule.test.test(inputContent) === false) {
          inputContentValid = false
          this.setState({
            showInvalidTip: true,
            tipContent: rule.message,
            tipLevel: rule.level,
          })
          break
        }
      }
    }
    if (inputContentValid === true) {
      this.setState({
        showInvalidTip: false,
        tipContent: '',
        tipLevel: '',
      })
    }
    return inputContentValid
  }

  getValue = () => {
    return this.inputElemRef.current.value
  }

  render() {
    return (
      <div className="input-component-wrap" style={this.props.style}>
        <span
          className={classnames({
            'tip-content': true,
            'level-error': this.state.tipLevel === 'error',
            'level-warn': this.state.tipLevel === 'warn'
          })}
        >
          { this.state.tipContent }
        </span>
        <span
          className={classnames({
            'tip-icon': true,
            'level-error': this.state.tipLevel === 'error',
            'level-warn': this.state.tipLevel === 'warn'
          })}
        />
        <input
          type="text"
          className="input-content"
          ref={this.inputElemRef}
          onChange={this.handleInputChange}
          defaultValue={this.props.defaultValue}
          placeholder={this.props.placeholder}
        />
      </div>
    )
  }
}

export default Input
