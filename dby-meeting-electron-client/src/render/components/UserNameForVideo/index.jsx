import React, { Component } from 'react'
import PropTypes from 'prop-types'
import classnames from 'classnames'
import { observer } from 'mobx-react'

import BackgroundBlur from '../BackgroundBlur'

import store from '../../store'

import './index.less'

@observer
class UserNameForVideo extends Component {
  static propTypes = {
    style: PropTypes.object,
    uid: PropTypes.number.isRequired,
    showIcon: PropTypes.bool,
  };

  static defaultProps = {
    style: {},
    showIcon: false,
  };

  render() {
    const { style, uid, showIcon } = this.props
    const volume = store.userVolumes.get(uid) || 0

    return (
      <div
        className={classnames({
          'user-name-for-video-component-wrap': true,
          'icon-mode': showIcon === true,
        })}
        style={style}
      >
        {
          showIcon === true && (
            <img src={require('../../static/image/icon-user-white-30-30.png')} className="user-icon" alt=""/>
          )
        }
        <span className="user-name">{ uid }&nbsp;|&nbsp;音量：{ volume }</span>
        <BackgroundBlur
          style={{
            borderRadius: showIcon ? '0 11px 11px 0' : '4px 4px 0 0',
          }}
        />
      </div>
    )
  }
}

export default UserNameForVideo
