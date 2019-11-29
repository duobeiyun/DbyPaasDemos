import React, { Component, memo } from 'react'
import PropTypes from 'prop-types'
import _ from 'lodash'
import { observer } from 'mobx-react'

import UserVideoListItem from '../UserVideoListItem'
import BackgroundBlur from '../BackgroundBlur'

import store from '../../store'

import './index.less'

@observer
class UserVideoList extends Component {
  static propTypes = {
    style: PropTypes.object,
  };

  static defaultProps = {
    style: {},
  };

  render() {
    const userVideos = _.filter(store.onlineUsers.toJSON(), (uid) => uid !== store.currentFullScreenVideoUid)

    return (
      <div className="user-video-list-wrap" style={this.props.style}>
        <span className="title">视频列表</span>
        <div className="video-list-wrap">
          {
            _.map(userVideos, (uid) => (
              <UserVideoListItem
                uid={uid}
                key={uid}
              />
            ))
          }
        </div>
        <BackgroundBlur />
      </div>
    )
  }
}

export default memo(UserVideoList)
