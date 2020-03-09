//
//  DbyVideoItem.swift
//  DbyPaas_Demo
//
//  Created by yxibng on 2019/12/31.
//  Copyright © 2019 yxibng. All rights reserved.
//

import UIKit
import AgoraRtcEngineKit
import DbyPaas_iOS

class VideoCanvas {
    let agoraCanvas: AgoraRtcVideoCanvas = AgoraRtcVideoCanvas.init()
    let dbyCanvas: DbyVideoCanvas = DbyVideoCanvas.init()

    var uid: String = "" {
        didSet {
            //agora
            agoraCanvas.uid = UInt(self.uid) ?? 0
            agoraCanvas.view = view
            agoraCanvas.renderMode = renderMode

            //dby
            dbyCanvas.uid = String(uid)
            dbyCanvas.view = view
            dbyCanvas.renderMode = .aspectFit

        }
    }
    let view: UIView = UIView.init()
    var renderMode: AgoraVideoRenderMode = .fit
}

class DbyVideoItem: NSObject {

    var uid: String = "" {
        didSet {
            canvas.uid = uid
        }
    }

    var nickname: String = ""
    var hasVideo: Bool = false
    var hasAudio: Bool = false

    var muteAudio: Bool = false
    var muteVideo: Bool = false

    var isLocal: Bool = false
    var isLarge: Bool = false
    var volume: String = ""
    var dimension: String = ""
    var isLoading: Bool = false
    let canvas: VideoCanvas = VideoCanvas.init()
    var videoHostView: UIView? {
        didSet {
            if let videoView = videoHostView {
                for view in videoView.subviews {
                    view.removeFromSuperview()
                }
                canvas.view.removeFromSuperview()
                videoView.addSubview(canvas.view)
                canvas.view.frame = videoView.bounds
                canvas.view.autoresizingMask = [.flexibleWidth, .flexibleHeight]
            }
        }
    }

}
