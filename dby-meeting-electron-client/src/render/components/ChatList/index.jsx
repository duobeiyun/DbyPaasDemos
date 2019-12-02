import React, { Component, createRef } from 'react'
import PropTypes from 'prop-types'
import _ from 'lodash'
import { observer } from 'mobx-react'

import ChatItemRemote from '../ChatItemRemote'
import ChatItemSelf from '../ChatItemSelf'
import BackgroundBlur from '../BackgroundBlur'

import store from '../../store'
import sdkWrapper from '../../sdkwrapper'

import './index.less'

@observer
class ChatList extends Component {
  static propTypes = {
    style: PropTypes.object,
  };

  static defaultProps = {
    style: {},
  };

  inputElemRef = createRef()

  chatListElemRef = createRef()

  handleSendChat = () => {
    const content = this.inputElemRef.current.value
    this.inputElemRef.current.value = ''
    if (content !== '') {
      sdkWrapper.sendStreamMessage(content)
      store.addChat({
        uid: store.selfUid,
        content: content,
      })
    }
  }

  handleInputKeyUp = (e) => {
    if (e.keyCode === 13) {
      this.handleSendChat()
    }
  }

  componentDidUpdate() {
    this.chatListElemRef.current.scrollTop = 10e10
  }

  render() {
    return (
      <div className="chat-list-component-wrap" style={this.props.style}>
        <span className="title">消息</span>
        <div className="chat-list-wrap" ref={this.chatListElemRef}>
          {
            _.map(store.chatList.toJSON(), (chatInfo, i) => {
              return chatInfo.uid === store.selfUid ? (
                <ChatItemSelf
                  style={{
                    marginTop: i === 0 ? '0' : '14px',
                  }}
                  content={chatInfo.content}
                />
              ) : (
                <ChatItemRemote
                  style={{
                    marginTop: i === 0 ? '0' : '14px',
                  }}
                  content={chatInfo.content}
                  uid={chatInfo.uid}
                />
              )
            })
          }
        </div>
        <div className="chat-input-wrap">
          <input type="text" ref={this.inputElemRef}  onKeyUp={this.handleInputKeyUp} className="input-area" placeholder="说点什么…" />
          <span className="btn-submit-input" onClick={this.handleSendChat}>发送</span>
        </div>
        <BackgroundBlur />
      </div>
    )
  }
}

export default ChatList
