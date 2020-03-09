//
//  JoinChannelInfo.swift
//  DbyPaas_Demo
//
//  Created by yxibng on 2020/3/9.
//  Copyright © 2020 yxibng. All rights reserved.
//

import DbyPaas_iOS
import AgoraRtcEngineKit

enum JoinChannelProfile {
    case broadcast, communaction
}

enum JoinStreamType {
    case dual, single
}

enum JoinRole {
    case broadcaster, audience
}

class JoinChannelInfo {

    /*!
    允许的组合模式:
    1. 直播模式, 主播, 单流模式
    2. 直播模式, 听众, 单流模式
    3. 通信模式, 大小流
    4. 通信模式非大小流
     
    关于模式互通:
     1. 直播模式和通信模式不互通
     2. 通信模式, 双流模式和单流模式不通
    */

    var channelProfile: JoinChannelProfile = .broadcast
    var role: JoinRole = .broadcaster
    var streamType: JoinStreamType = .dual

    var channelId: String
    var uid: String
    var line: VideoLine

    init(channelId: String, uid: String, line: VideoLine, channelProfile: JoinChannelProfile, role: JoinRole, streamType: JoinStreamType) {
        self.channelId = channelId
        self.uid = uid
        self.line = line
        self.channelProfile = channelProfile
        self.role = role
        self.streamType = streamType
    }

    //agora
    var channelProfile_Agora: AgoraChannelProfile {
        if self.channelProfile == .broadcast {
            return .liveBroadcasting
        } else {
            return .communication
        }
    }
    var clientRole_Agora: AgoraClientRole {
        if self.role == .broadcaster {
            return .broadcaster
        } else {
            return .audience
        }
    }
    var videoProfile_Agora = AgoraVideoDimension640x360

    var enableDualStream_Agora: Bool {
        if self.channelProfile == .broadcast {
            return false
        }

        if self.streamType == .dual {
            return true
        }

        return false
    }

    //dby
    var channelProfile_Dby: DbyChannelProfile {
        if self.channelProfile == .broadcast {
            return .liveBroadcasting
        } else {
            return .communication
        }
    }

    var clientRole_Dby: DbyClientRole {
        if self.role == .broadcaster {
            return .broadcaster
        } else {
            return .audience
        }
    }

    var enableDualStream_Dby: Bool {
        if self.channelProfile == .broadcast {
            return false
        }
        if self.streamType == .dual {
            return true
        }
        return false
    }
    var videoProfile_Dby = CGSize.init(width: 640, height: 360)
}
