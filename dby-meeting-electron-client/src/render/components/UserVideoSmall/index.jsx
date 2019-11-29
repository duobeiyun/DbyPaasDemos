import React, {PureComponent} from 'react'
import PropTypes from 'prop-types'
import _ from 'lodash'
import classnames from 'classnames'

import './index.less'

class UserVideoSmall extends PureComponent {
  static propTypes = {
    style: PropTypes.object,
    getVideoPlayerRef: PropTypes.func,
    hasVideoFrame: PropTypes.bool,
  };

  static defaultProps = {
    style: {},
    getVideoPlayerRef: _.noop,
    hasVideoFrame: false,
  };

  render() {
    return (
      <div className="user-video-small-wrap" style={this.props.style}>
        <div
          className={classnames({
            'no-frame-tip': true,
            hide: this.props.hasVideoFrame === true
          })}
        >
          <img className="tip-icon" src={require('../../static/image/icon-camera-close-138-153.png')} alt=""/>
          <span className="tip-content">摄像头未开启</span>
        </div>
        <div
          className={classnames({
            'user-video-player-wrap': true,
            hide: this.props.hasVideoFrame === false
          })}
          ref={(ref) => {
            if (_.isNil(ref) === false) {
              this.props.getVideoPlayerRef(ref)
            }
          }}
        />
      </div>
    )
  }
}

export default UserVideoSmall
