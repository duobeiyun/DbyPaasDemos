import React, { Component } from 'react'
import PropTypes from 'prop-types'
import _ from 'lodash'
import { observer } from 'mobx-react'

import BackgroundBlur from '../BackgroundBlur'
import OnlineUserListItem from '../OnlineUserListItem'

import store from '../../store'

import './index.less'

@observer
class OnlineUserList extends Component {
  static propTypes = {
    style: PropTypes.object,
  };

  static defaultProps = {
    style: {},
  };

  render() {
    return (
      <div className="online-user-list-component-wrap" style={this.props.style}>
        <span className="title">用户列表</span>
        <div className="user-list-wrap">
          {
            _.map(store.onlineUsers.toJSON(), (uid) => {
              return (
                <OnlineUserListItem
                  uid={uid}
                />
              )
            })
          }
        </div>
        <BackgroundBlur />
      </div>
    )
  }
}

export default OnlineUserList
