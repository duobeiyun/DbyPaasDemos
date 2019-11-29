import React, { Component, Fragment } from 'react'
import PropTypes from 'prop-types'
import _ from 'lodash'
import classnames from 'classnames'

import BackgroundBlur from '../BackgroundBlur'

import './index.less'

class DeviceList extends Component {
  static propTypes = {
    style: PropTypes.object,
    data: PropTypes.array.isRequired,
    defaultIndex: PropTypes.number,
    onSelect: PropTypes.func,
  };

  static defaultProps = {
    style: {},
    defaultIndex: 0,
    onSelect: _.noop,
  };

  state = {
    currentIndex: this.props.defaultIndex,
  }

  handleSelectDevice = (i) => {
    Log.info('handleSelectDevice ', i)
    this.setState({
      currentIndex: i,
    })
    this.props.onSelect(i)
  }

  render() {
    return (
      <div className="device-list-component-wrap" style={this.props.style}>
        {
          _.map(this.props.data, (deviceName, i, source) => {
            return (
              <Fragment key={i}>
                <div
                  className={classnames({
                    'device-item-wrap': true,
                    'current': this.state.currentIndex === i,
                  })}
                  onClick={this.handleSelectDevice.bind(this, i)}
                >
                  <span className="name">{ deviceName }</span>
                </div>
                {
                  i !== source.length - 1 && (
                    <div className="divider" />
                  )
                }
              </Fragment>
            )
          })
        }
        <BackgroundBlur
          style={{
            borderRadius: '4px',
          }}
        />
      </div>
    )
  }
}

export default DeviceList
