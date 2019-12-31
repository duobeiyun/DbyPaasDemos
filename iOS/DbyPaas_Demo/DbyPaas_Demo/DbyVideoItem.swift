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
    
    var uid: UInt = 0 {
        didSet {
            view.translatesAutoresizingMaskIntoConstraints = false
            view.backgroundColor = .clear
            
            //agora
            agoraCanvas.uid = uid
            agoraCanvas.view = view
            agoraCanvas.renderMode = renderMode
            
            //dby
            dbyCanvas.uid = String(uid)
            dbyCanvas.view = view
            dbyCanvas.renderMode = .aspectFit
            
        }
    }
    let view:UIView = UIView.init()
    var renderMode: AgoraVideoRenderMode = .fit
}

class DbyVideoItem: NSObject {
    
    var uid:UInt = 0 {
        didSet {
            canvas.uid = uid
        }
    }
    
    var hasVideo: Bool = false
    var hasAudio: Bool = false
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
