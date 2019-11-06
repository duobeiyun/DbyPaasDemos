//
//  DbyVideoChatViewController.swift
//  DbyPaas_Demo
//
//  Created by yxibng on 2019/10/29.
//  Copyright © 2019 yxibng. All rights reserved.
//

import UIKit
import DbyPaas_iOS
import MBProgressHUD

class DbyVideoChatViewController: UIViewController {

    var channelId = ""
    var uid = ""
    var hasRemote = false
    
    var camOn = false {
        didSet {
            self.camButton.isSelected = !camOn
        }
    }
    var micOn = false {
        didSet {
            self.micButton.isSelected = !micOn
        }
    }
    
    var dbyEngine: DbyEngine!
    
    @IBOutlet weak var bigView: UIView!
    @IBOutlet weak var smallView: UIView!
    @IBOutlet weak var camButton: UIButton!
    @IBOutlet weak var micButton: UIButton!
    override func viewDidLoad() {
        super.viewDidLoad()
        dbyEngine = DbyEngine.sharedEngine(withAppId: KeyCenter.AppId, appkey: KeyCenter.AppKey, delegate: self)
        self.startEngine()
    }
    
    
    @IBAction func onClickCam(_ sender: Any) {
        let nextState = !self.camOn
        self.dbyEngine.enableLocalVideo(nextState)
    }
    
    @IBAction func onClickMic(_ sender: Any) {
        let nextState = !self.micOn
        self.dbyEngine.enableLocalAudio(nextState)
    }
    
    @IBAction func onClickSwitch(_ sender: Any) {
        self.dbyEngine.switchCamera()
    }
    @IBAction func onClickExit(_ sender: Any) {
        self.dbyEngine.leaveChannel(nil)
    }
    
    func startEngine() {
        let config = DbyVideoEncoderConfiguration.init(size: DbyVideoDimension1280x720, frameRate: .fps15, orientationMode: .adaptative)
        dbyEngine.setVideoEncoderConfiguration(config)
        let status = dbyEngine.joinChannel(byId: self.channelId, userId: self.uid) { [weak self] (code) in
            //stop loading
            MBProgressHUD.stopLoading(in: self?.view)
            //show error msg
            if code != DbyErrorCode.noError.rawValue {
                let msg = "join channel error, code = \(code)"
                MBProgressHUD.showMessage(msg: msg, inView: self?.view)
                DispatchQueue.main.asyncAfter(deadline: .now() + 1.0, execute: {
                    // code to execute
                    self?.dismiss(animated: true, completion: nil)
                })
            }
        }

        if status == 0 {
            dbyEngine.enableDualStreamMode(true)
            dbyEngine.enableLocalAudio(true)
            dbyEngine.enableLocalVideo(true)
            //进入的时候就设置本地预览
            dbyEngine.setupLocalVideo(DbyVideoCanvas.init(view: self.bigView, uid: uid))
        }
    }
    
    func localJoin(uid: String) {
        if hasRemote {
            let canvas = DbyVideoCanvas.init(view: self.smallView, uid: self.uid)
            self.dbyEngine.setupLocalVideo(canvas)
        } else {
            let canvas = DbyVideoCanvas.init(view: self.bigView, uid: self.uid)
            self.dbyEngine.setupLocalVideo(canvas)
        }
    }
    
    func remoteJoin(uid: String) {
        hasRemote = true
        
        let remoteCanvas = DbyVideoCanvas.init(view: self.bigView, uid: uid)
        let localCanvas = DbyVideoCanvas.init(view: self.smallView, uid: self.uid)
        
        self.dbyEngine.setupRemoteVideo(remoteCanvas)
        self.dbyEngine.setupLocalVideo(localCanvas)
        
    }
    
    func remoteLeave(uid: String)  {
        hasRemote = false
        let localCanvas = DbyVideoCanvas.init(view: self.bigView, uid: self.uid)
        self.dbyEngine.setupLocalVideo(localCanvas)
    }
    
}


extension DbyVideoChatViewController: DbyEngineDelegate {
    
    func dbyEngine(_ engine: DbyEngine, didJoinChannel channel: String, withUid uid: String) {
        localJoin(uid: uid)
        print("didJoinChannel \(uid)")
    }
    
    func dbyEngine(_ engine: DbyEngine, didLeaveChannel channel: String, withUid uid: String) {
        print("didLeaveChannel \(uid)")
        
        //销毁单例，释放资源
        DbyEngine.destroy()
        
        self.dismiss(animated: true, completion: nil)
        
    }
    
    func dbyEngine(_ engine: DbyEngine, didJoinedOfUid uid: String) {
        remoteJoin(uid: uid)
           print("didJoinedOfUid \(uid)")
    }
    
    func dbyEngine(_ engine: DbyEngine, didOfflineOfUid uid: String) {
        remoteLeave(uid: uid)
        print("didOfflineOfUid \(uid)")
    }
    
    //本地音频开关状态变更回调
    func dbyEngine(_ engine: DbyEngine, localAudioStateChange state: DbyAudioLocalState, error: DbyAudioLocalError) {
        
        switch state {
        case .recording:
            self.micOn = true
        default:
            self.micOn = false
        }
    }
    
    //本地视频开关状态变更回调
    func dbyEngine(_ engine: DbyEngine, localVideoStateChange state: DbyLocalVideoStreamState, error: DbyLocalVideoStreamError) {
        switch state {
        case .capturing:
            self.camOn = true
        default:
            self.camOn = false
        }
    }
    
    
    ///远端用户首帧视频被解码
    ///先收到 remoteVideoStateChangedOfUid enabled = true
    ///之后会收到 firstRemoteVideoDecodedOfUid
    
    func dbyEngine(_ engine: DbyEngine, firstRemoteVideoDecodedOfUid uid: String, device deviceId: String?, size: CGSize) {
        
    }
    //远端用户音频流开关状态
    func dbyEngine(_ engine: DbyEngine, remoteAudioStateChangedOfUid uid: String, state enabled: Bool) {
        
    }
    
    //远端用户视频流开关状态
    func dbyEngine(_ engine: DbyEngine, remoteVideoStateChangedOfUid uid: String, device: String?, state enabled: Bool) {
        if enabled {
            self.dbyEngine.setRemoteVideoStream(uid, type: .high)
        }
    }
    
    //sdk 由于网络连接问题，经过多次重试，无法连接消息服务器，主动断开连接。此时内部已经停止工作，需要主动调用离开频道。
    func dbyEngine(_ engine: DbyEngine, didReceiveSDKConnectError errorCode: Int) {
        dbyEngine.leaveChannel(nil)
    }
    
    
    //同一个用户，在别处登录，当前登录被踢出的回调
    func dbyEngineDidKickedOff(_ engine: DbyEngine) {
    }
    
    
}

func runOnMainQueueWithoutDeadlocking(_ block: (() -> Void)? ) {
    
    guard let callback = block else {
        return
    }
    
    if Thread.isMainThread {
        callback()
    } else {
        DispatchQueue.main.async {
            callback()
        }
    }
}

