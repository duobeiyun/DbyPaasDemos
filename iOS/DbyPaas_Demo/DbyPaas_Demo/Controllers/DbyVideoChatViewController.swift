//
//  DbyVideoChatViewController.swift
//  DbyPaas_Demo
//
//  Created by yxibng on 2019/10/29.
//  Copyright © 2019 yxibng. All rights reserved.
//

import UIKit
import DbyPaas_iOS
import AgoraRtcEngineKit
import MBProgressHUD

enum VideoLine {
    case DBY, AGORA
}

class DbyVideoChatViewController: UIViewController {

    @IBOutlet weak var collectionView: UICollectionView!
    @IBOutlet weak var bigStreamView: DbyVideoView!
    @IBOutlet weak var localSmallView: DbyVideoView!

    var joinChannelInfo: JoinChannelInfo!

    var videoManager: DbyVideoChatManager!

    var dbyEngine: DbyEngine!
    var agoraEngine: AgoraRtcEngineKit!

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

    @IBOutlet weak var camButton: UIButton!
    @IBOutlet weak var micButton: UIButton!

    deinit {
        print("\(#function)")
        DbyEngine.destroy()
        AgoraRtcEngineKit.destroy()
    }

    override func viewDidLoad() {
        super.viewDidLoad()

        let layout = UICollectionViewFlowLayout.init()
        layout.itemSize = CGSize.init(width: 76, height: 134)
        layout.minimumLineSpacing = 20
        layout.minimumInteritemSpacing = 0
        layout.scrollDirection = .horizontal
        collectionView.setCollectionViewLayout(layout, animated: false)
        collectionView.contentInset = UIEdgeInsets.init(top: 0, left: 20, bottom: 0, right: 20)
        for index in 0...50 {
            collectionView.register(UINib.init(nibName: "VideoCollectionViewCell", bundle: nil), forCellWithReuseIdentifier: "VideoCollectionViewCell\(index)")
        }

        videoManager = DbyVideoChatManager.init(delegate: self)
        setupEngines()
        startProbeTest()
    }

    func setupEngines() {
        switch joinChannelInfo.line {
        case .DBY:
            dbyEngine = DbyEngine.sharedEngine(withAppId: KeyCenter.AppId, appkey: KeyCenter.AppKey, delegate: self)
        case .AGORA:
            agoraEngine = AgoraRtcEngineKit.sharedEngine(withAppId: KeyCenter.AppId_Agora, delegate: self)
        }
    }

    @IBAction func onClickCam(_ sender: Any) {
        let nextState = !self.videoManager.localItem.hasVideo
        switch joinChannelInfo.line {
        case .DBY:
            self.dbyEngine.enableLocalVideo(nextState)
        default:
            self.agoraEngine.enableLocalAudio(nextState)
        }
    }

    @IBAction func onClickMic(_ sender: Any) {
        let nextState = !self.videoManager.localItem.hasAudio
        switch joinChannelInfo.line {
        case .DBY:
            self.dbyEngine.enableLocalAudio(nextState)
        default:
            self.agoraEngine.enableLocalAudio(nextState)
        }

    }

    @IBAction func onClickSwitch(_ sender: Any) {
        switch joinChannelInfo.line {
        case .DBY:
            self.dbyEngine.switchCamera()
        default:
            self.agoraEngine.switchCamera()
        }
    }
    @IBAction func onClickExit(_ sender: Any) {
        switch joinChannelInfo.line {
        case .DBY:
            self.dbyEngine.leaveChannel(nil)
        default:
            self.agoraEngine.leaveChannel(nil)
        }
    }
}

extension DbyVideoChatViewController: UICollectionViewDelegate, UICollectionViewDataSource {
    func collectionView(_ collectionView: UICollectionView, numberOfItemsInSection section: Int) -> Int {
        return self.videoManager.remoteSmallItems.count
    }

    func collectionView(_ collectionView: UICollectionView, cellForItemAt indexPath: IndexPath) -> UICollectionViewCell {

        let identifier = "VideoCollectionViewCell\(indexPath.item)"
        let cell = collectionView.dequeueReusableCell(withReuseIdentifier: identifier, for: indexPath) as! VideoCollectionViewCell
        let item = self.videoManager.remoteSmallItems[indexPath.item]
        configCell(cell, at: indexPath, with: item)
        return cell
    }

    func configCell(_ cell: VideoCollectionViewCell, at indexPath: IndexPath, with item: DbyVideoItem) {
        cell.videoView.configWith(item: item)
        cell.callback = { [weak self] in
            self?.videoManager.swithToLargeWith(uid: item.uid)
        }
    }
}

extension DbyVideoChatViewController: DbyEngineDelegate {

    func dbyEngine(_ engine: DbyEngine, lastmileQuality quality: DbyNetworkQuality) {
        if quality == .detecting {
            return
        }

        MBProgressHUD.stopLoading(in: self.view)
        dbyEngine.disableLastmileTest()

        switch quality {
        case .down:
            MBProgressHUD.showMessage(msg: "无网络", inView: self.view)
            DispatchQueue.main.asyncAfter(deadline: .now() + 1.0) {
                self.dismiss(animated: true, completion: nil)
            }
        default:
            MBProgressHUD.showMessage(msg: "测速结果：\(quality)", inView: self.navigationController?.view)
            self.startEngine()
        }
    }

    // MARK: local
    func dbyEngine(_ engine: DbyEngine, didJoinChannel channel: String, withUid uid: String, nickname: String) {
        self.videoManager.localJoinWith(uid: uid, nickname: nickname)
    }

    func dbyEngine(_ engine: DbyEngine, joinWithError errorCode: Int) {
        if errorCode != 0 {
            MBProgressHUD.showMessage(msg: "failed to join with error \(errorCode)", inView: self.view)
            self.dismiss(animated: true, completion: nil)
        }
    }

    func dbyEngine(_ engine: DbyEngine, didLeaveChannel channel: String, withUid uid: String) {

        DbyEngine.destroy()
        self.dismiss(animated: true, completion: nil)
    }

    //同一个用户，在别处登录，当前登录被踢出的回调
    func dbyEngineDidKickedOff(_ engine: DbyEngine) {

    }

    func dbyEngine(_ engine: DbyEngine, localAudioStateChange state: DbyAudioLocalState, error: DbyAudioLocalError) {

        switch state {
        case .recording:
            self.videoManager.localAudioChange(enabled: true)
            self.micOn = true
        default:
            self.videoManager.localAudioChange(enabled: false)
            self.micOn = false
        }
    }

    func dbyEngine(_ engine: DbyEngine, localVideoStateChange state: DbyLocalVideoStreamState, error: DbyLocalVideoStreamError) {
        switch state {
        case .capturing:
            self.videoManager.localVideoChange(enabled: true)
            self.camOn = true
        default:
            self.videoManager.localVideoChange(enabled: false)
            self.camOn = false
        }
    }

    // MARK: remote
    func dbyEngine(_ engine: DbyEngine, didJoinedOfUid uid: String, nickname: String) {
        self.videoManager.remoteJoinWith(uid: uid, nickname: nickname)

    }

    func dbyEngine(_ engine: DbyEngine, didOfflineOfUid uid: String) {
        self.videoManager.remoteLeaveWith(uid: uid)
    }

    ///远端用户首帧视频被解码
    ///先收到 remoteVideoStateChangedOfUid enabled = true
    ///之后会收到 firstRemoteVideoDecodedOfUid

    func dbyEngine(_ engine: DbyEngine, firstRemoteVideoDecodedOfUid uid: String, identifier: String?, size: CGSize) {
        self.videoManager.receiveRemoteFirstVideoFrameOf(uid: uid)
    }
    //远端用户音频流开关状态
    func dbyEngine(_ engine: DbyEngine, remoteAudioStateChangedOfUid uid: String, state enabled: Bool) {
        self.videoManager.remoteAudioChangeWith(uid: uid, enabled: enabled)
    }

    //远端用户视频流开关状态
    func dbyEngine(_ engine: DbyEngine, remoteVideoStateChangedOfUid uid: String, identifier: String?, state enabled: Bool) {

        print("\(#function), uid = \(uid), enable = \(enabled)")
        self.videoManager.remoteVideoChangeWith(uid: uid, enabled: enabled)
    }

    //sdk 由于网络连接问题，经过多次重试，无法连接消息服务器，主动断开连接。此时内部已经停止工作，需要主动调用离开频道。
    func dbyEngine(_ engine: DbyEngine, didReceiveSDKConnectError errorCode: Int) {
        dbyEngine.leaveChannel(nil)
    }

}

extension DbyVideoChatViewController: AgoraRtcEngineDelegate {

    func rtcEngine(_ engine: AgoraRtcEngineKit, lastmileQuality quality: AgoraNetworkQuality) {
        if quality == .detecting {
            return
        }

        MBProgressHUD.stopLoading(in: self.view)
        agoraEngine.stopLastmileProbeTest()

        switch quality {
        case .down:
            MBProgressHUD.showMessage(msg: "无网络", inView: self.view)
            DispatchQueue.main.asyncAfter(deadline: .now() + 1.0) {
                self.dismiss(animated: true, completion: nil)
            }
        default:
            MBProgressHUD.showMessage(msg: "测速结果：\(quality)", inView: self.navigationController?.view)
            self.startEngine()
        }
    }

    // MARK: local
    func rtcEngine(_ engine: AgoraRtcEngineKit, didJoinChannel channel: String, withUid uid: UInt, elapsed: Int) {
        self.videoManager.localJoinWith(uid: String(uid), nickname: "")
    }

    func rtcEngine(_ engine: AgoraRtcEngineKit, didLeaveChannelWith stats: AgoraChannelStats) {

        agoraEngine.enableLocalAudio(false)
        agoraEngine.enableLocalVideo(false)

        AgoraRtcEngineKit.destroy()
        MBProgressHUD.stopLoading(in: self.view)
        self.dismiss(animated: true, completion: nil)
    }

    func rtcEngine(_ engine: AgoraRtcEngineKit, localAudioStateChange state: AgoraAudioLocalState, error: AgoraAudioLocalError) {
        if state == .failed || state == .stopped {
            self.videoManager.localAudioChange(enabled: false)
            self.micOn = false
        } else {
            self.videoManager.localAudioChange(enabled: true)
            self.micOn = true
        }
    }

    func rtcEngine(_ engine: AgoraRtcEngineKit, localVideoStateChange state: AgoraLocalVideoStreamState, error: AgoraLocalVideoStreamError) {
        if state == .stopped || state == .failed {
            self.videoManager.localVideoChange(enabled: false)
            self.camOn = false
        } else {
            self.videoManager.localVideoChange(enabled: true)
            self.camOn = true
        }
    }

    // MARK: remote
    func rtcEngine(_ engine: AgoraRtcEngineKit, didJoinedOfUid uid: UInt, elapsed: Int) {
        self.videoManager.remoteJoinWith(uid: String(uid), nickname: "")
    }

    func rtcEngine(_ engine: AgoraRtcEngineKit, didOfflineOfUid uid: UInt, reason: AgoraUserOfflineReason) {
        self.videoManager.remoteLeaveWith(uid: String(uid))
    }

    func rtcEngine(_ engine: AgoraRtcEngineKit, remoteAudioStateChangedOfUid uid: UInt, state: AgoraAudioRemoteState, reason: AgoraAudioRemoteStateReason, elapsed: Int) {
        switch state {
        case .stopped, .failed:
            self.videoManager.remoteAudioChangeWith(uid: String(uid), enabled: false)
        default:
            self.videoManager.remoteAudioChangeWith(uid: String(uid), enabled: true)
        }
    }

    func rtcEngine(_ engine: AgoraRtcEngineKit, remoteVideoStateChangedOfUid uid: UInt, state: AgoraVideoRemoteState, reason: AgoraVideoRemoteStateReason, elapsed: Int) {
        switch state {
        case .failed, .stopped:
            self.videoManager.remoteVideoChangeWith(uid: String(uid), enabled: false)
        default:
            self.videoManager.remoteVideoChangeWith(uid: String(uid), enabled: true)
        }
    }

    func rtcEngine(_ engine: AgoraRtcEngineKit, firstRemoteVideoFrameOfUid uid: UInt, size: CGSize, elapsed: Int) {
        self.videoManager.receiveRemoteFirstVideoFrameOf(uid: String(uid))
    }
}

extension DbyVideoChatViewController: DbyVideoChatManagerDelegate {

    func didJoinWith(item: DbyVideoItem) {

        switch joinChannelInfo.line {
        case .DBY:
            if item.isLocal {
                dbyEngine.setupLocalVideo(item.canvas.dbyCanvas)
            } else {
                dbyEngine.setupRemoteVideo(item.canvas.dbyCanvas)
            }
        default:
            if item.isLocal {
                agoraEngine.setupLocalVideo(item.canvas.agoraCanvas)
            } else {
                agoraEngine.setupRemoteVideo(item.canvas.agoraCanvas)
            }
        }

    }

    func didLeaveWith(item: DbyVideoItem) {
        item.canvas.view.removeFromSuperview()
    }

    func didBigAreaChangeWith(item: DbyVideoItem, type: DataEventType) {
        switch type {
        case .Insert, .Update:
            self.bigStreamView.configWith(item: item)
        default:
            item.canvas.view.removeFromSuperview()
        }
    }

    func didLocalAreaChangeWith(item: DbyVideoItem, type: DataEventType) {
        switch type {
        case .Insert, .Update:
            localSmallView.isHidden = false
            localSmallView.configWith(item: item)
        default:
            localSmallView.isHidden = true
        }
    }

    func didRemoteAreaChangeWith(index: Int, type: DataEventType) {
        switch type {
        case .Insert:
            self.collectionView.insertItems(at: [IndexPath.init(item: index, section: 0)])
        case .Update:
            let indexPath = IndexPath.init(row: index, section: 0)
            if let cell = self.collectionView.cellForItem(at: indexPath) {
                let item = self.videoManager.remoteSmallItems[indexPath.item]
                configCell(cell as! VideoCollectionViewCell, at: indexPath, with: item)
            }
        default:
            self.collectionView.deleteItems(at: [IndexPath.init(item: index, section: 0)])
        }
    }

    func didChangeStreamOf(item: DbyVideoItem, to streamType: StreamType) {
        switch joinChannelInfo.line {
        case .DBY:
            if streamType == .high {
                self.dbyEngine.setRemoteVideoStream(item.uid, type: .high)
            } else {
                self.dbyEngine.setRemoteVideoStream(item.uid, type: .low)
            }
        default:
            if streamType == .high {
                self.agoraEngine.setRemoteVideoStream(UInt(item.uid)!, type: .high)
            } else {
                self.agoraEngine.setRemoteVideoStream(UInt(item.uid)!, type: .low)
            }
        }
    }
}

extension DbyVideoChatViewController {
    func startProbeTest() {
        MBProgressHUD.startLoading(withMessage: "测速中...", in: self.view)
        switch joinChannelInfo.line {
        case .DBY:
            
            if joinChannelInfo.channelProfile == .communaction {
                dbyEngine.setChannelProfile(.communication)
            } else {
                dbyEngine.setChannelProfile(.liveBroadcasting)
            }
            dbyEngine.enableLastmileTest()
            break
        default:
            agoraEngine.startLastmileProbeTest(nil)
            break
        }
    }

    func startEngine() {
        switch joinChannelInfo.line {
        case .DBY:
            startDbyEngine()
        default:
            startAgoraEngine()
        }
     }

    func startDbyEngine() {

        /*
         1. 优先设置频道模式, 直播模式 或者通信模式
         2. 直播模式只能和直播模式互通, 直播模式不支持大小流
         3. 通信模式只能和通信模式互通,开启大小流只能和开启大小流的互通, 大小流和非大小流不互通
         */
        dbyEngine.setChannelProfile(joinChannelInfo.channelProfile_Dby)
        if joinChannelInfo.channelProfile == .broadcast {
            //只有直播模式, 才可以设置用户的角色
            dbyEngine.setClientRole(joinChannelInfo.clientRole_Dby)
        }

        /*
         只有通信模式才可以开启大小流
         */
        dbyEngine.enableDualStreamMode(joinChannelInfo.enableDualStream_Dby)

        let config = DbyVideoEncoderConfiguration.init(size: DbyVideoDimension1280x720, frameRate: .fps15, orientationMode: .adaptative)
        dbyEngine.setVideoEncoderConfiguration(config)

        /*
         1. 直播模式,听众不可以开关摄像头和麦克风, 调用会出错
         2. 直播模式和直播模式互通
         3. 直播模式,听众,不可以打开麦克风和摄像头, 操作会出错
         */

        let canSendMedia = self.joinChannelInfo.channelProfile == .communaction || (self.joinChannelInfo.channelProfile == .broadcast && self.joinChannelInfo.role == .broadcaster)

        if canSendMedia {
            dbyEngine.enableLocalAudio(true)
            dbyEngine.enableLocalVideo(true)
        }

        let code = dbyEngine.joinChannel(byId: self.joinChannelInfo.channelId, userId: self.joinChannelInfo.uid, nickname: nil, completeHandler: nil)
        if code == 0 {
            //进入的时候就设置本地预览
            let canvas = DbyVideoCanvas.init(view: self.bigStreamView.videoContentView, uid: self.joinChannelInfo.uid)
            dbyEngine.setupLocalVideo(canvas)
        } else {
            MBProgressHUD.showMessage(msg: "Join channel failed: \(code)", inView: self.view)
            self.dismiss(animated: true, completion: nil)

        }
    }

    func startAgoraEngine() {
        agoraEngine.setChannelProfile(joinChannelInfo.channelProfile_Agora)
        agoraEngine.setClientRole(joinChannelInfo.clientRole_Agora)
        agoraEngine.enableDualStreamMode(joinChannelInfo.enableDualStream_Agora)

        agoraEngine.enableVideo()
        agoraEngine.enableLocalVideo(true)

        agoraEngine.enableAudio()
        agoraEngine.enableLocalAudio(true)

        agoraEngine.setVideoEncoderConfiguration(
            AgoraVideoEncoderConfiguration(
                size: AgoraVideoDimension640x360,
                frameRate: .fps24,
                bitrate: AgoraVideoBitrateStandard,
                orientationMode: .adaptative
            )
        )

        agoraEngine.enableAudioVolumeIndication(200, smooth: 3, report_vad: true)

        if self.joinChannelInfo.channelProfile == .communaction || (self.joinChannelInfo.channelProfile == .broadcast && self.joinChannelInfo.role == .broadcaster) {
            agoraEngine.startPreview()
        }

        let code = agoraEngine.joinChannel(byToken: nil, channelId: self.joinChannelInfo.channelId, info: nil, uid: UInt(self.joinChannelInfo.uid) ?? 0, joinSuccess: nil)

        if code == 0 {
            agoraEngine.setEnableSpeakerphone(true)
            //设置本地预览视图
            let canvas = AgoraRtcVideoCanvas.init()
            canvas.view = self.bigStreamView.videoContentView
            canvas.renderMode = .hidden
            canvas.uid = UInt(self.self.joinChannelInfo.uid) ?? 0
            agoraEngine.setupLocalVideo(canvas)
        } else {
            MBProgressHUD.showMessage(msg: "Join channel failed: \(code)", inView: self.view)
            self.dismiss(animated: true, completion: nil)
        }
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
