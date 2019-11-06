//
//  DbyPlaybackViewController.swift
//  DbyPaas_Demo
//
//  Created by yxibng on 2019/10/30.
//  Copyright © 2019 yxibng. All rights reserved.
//

import UIKit
import DbyPaas_iOS
import MBProgressHUD

class DbyPlaybackViewController: UIViewController {

    @IBOutlet weak var bigView: UIView!
    @IBOutlet weak var tableView: UITableView!
    @IBOutlet weak var playControlBar: PlayControlBar!
    
    @IBOutlet weak var noOneLabel: UILabel!
    @IBOutlet weak var stackView: UIStackView!
    @IBOutlet weak var bigUidLabel: UILabel!
    var sliderBeginTouch :Bool = false
    var playEnd = false
    var startTimestamp:Double = 0
    
    var allMessages : [Dictionary<String, Any>]?
    var messages : [Dictionary<String, Any>]?
    var isPlaying: Bool = false {
        didSet {
            self.playControlBar.isPlaying = isPlaying
        }
    }
    
    var rate: Float = 1.0 {
        didSet {
            self.playControlBar.rate = rate
        }
    }
    
    var recordId: String?
    lazy var manager: PlaybackManager = {
        let manager = PlaybackManager.init()
        manager.delegate = self
        return manager
    }()
    
    lazy var collectionView: UICollectionView = {
        
        let layout = UICollectionViewFlowLayout.init()
        layout.itemSize = CGSize.init(width: 76+20, height: 134)
        layout.minimumLineSpacing = 0
        layout.minimumInteritemSpacing = 0
        layout.scrollDirection = .horizontal
        let collectionView = UICollectionView.init(frame: CGRect.zero, collectionViewLayout: layout)
        collectionView.delegate = self
        collectionView.dataSource = self
        collectionView.contentInset = UIEdgeInsets.init(top: 0, left: 20, bottom: 0, right: 20)
        collectionView.register(UINib.init(nibName: "VideoCollectionViewCell", bundle: nil), forCellWithReuseIdentifier: "VideoCollectionViewCell")
        collectionView.backgroundColor = .clear
        return collectionView
    }()
    
    lazy var playbackEngine: DbyPlaybackEngine = {
        let engine = DbyPlaybackEngine.sharedEngine(withAppId: KeyCenter.AppId, appkey: KeyCenter.AppKey, delegate: self)
        return engine
    }()
    
    
 
    
    deinit {
        NotificationCenter.default.removeObserver(self)
    }
    
    override func viewDidLoad() {
        super.viewDidLoad()
        
        playControlBar.delegate = self
        
        NotificationCenter.default.addObserver(self, selector: #selector(active(_:)), name: UIApplication.didBecomeActiveNotification, object: nil)
        NotificationCenter.default.addObserver(self, selector: #selector(inactive(_:)), name: UIApplication.willResignActiveNotification, object: nil)
        
        self.navigationItem.leftBarButtonItem = UIBarButtonItem.init(barButtonSystemItem: .stop, target: self, action: #selector(quit(item:)))
        
        tableView.register(UINib.init(nibName: "ChatTableViewCell", bundle: nil), forCellReuseIdentifier: "ChatTableViewCell")
        tableView.allowsSelection = false
        tableView.estimatedRowHeight = 44
        tableView.rowHeight = UITableView.automaticDimension
        
        
        self.view.addSubview(collectionView)
        collectionView.snp.makeConstraints { (make) in
            make.left.right.equalTo(self.view)
            make.height.equalTo(134)
                if #available(iOS 11.0, *) {
                    make.top.equalTo(self.view.safeAreaLayoutGuide.snp.top).offset(30)
                } else {
                    // Fallback on earlier versions
                    make.top.equalTo(topLayoutGuide.snp.top).offset(94)
                }
            
        }
        
        startEngine()
    }
    
    func startEngine() {
        if let rid = self.recordId {
            MBProgressHUD.startLoading(withMessage: "loading", in: self.view)
            let status = playbackEngine.join(withRecordId: rid)
            if status == 0 {
                MBProgressHUD.stopLoading(in: self.view)
            }
        } else {
            MBProgressHUD.showMessage(msg: "recordid is empty", inView: self.view)
            DispatchQueue.main.asyncAfter(deadline: .now() + 1.0, execute: {
                // code to execute
                self.navigationController?.popViewController(animated: true)
            })
        }
    }
    
    @objc func quit(item: UIBarButtonItem) {
        MBProgressHUD.startLoading(withMessage: "正在退出...", in: self.navigationController?.view)
        self.playbackEngine.leaveChannel()
    }
}


extension DbyPlaybackViewController : DbyPlaybackEngineDelegate {
    
    func playbackEngine(_ playbackEngine: DbyPlaybackEngine, didJoinWithRecordId recordId: String, status statusCode: Int) {
        
        if statusCode == 0 {
            MBProgressHUD.showMessage(msg: "join success", inView: self.view)
            self.isPlaying = true
        } else {
            MBProgressHUD.showMessage(msg: "join error, code \(statusCode)", inView: self.view)
            DispatchQueue.main.asyncAfter(deadline: .now() + 1.0, execute: {
                self.navigationController?.popViewController(animated: true)
            })
        }
    }
    
    
    func playbackEngine(_ playbackEngine: DbyPlaybackEngine, didLeaveWithRecordId recordId: String) {
        DbyPlaybackEngine.destroy()
        MBProgressHUD.stopLoading(in: self.navigationController?.view)
        self.navigationController?.popViewController(animated: true)
    }
    
    func playbackEngine(_ playbackEngine: DbyPlaybackEngine, didJoinedOfUid uid: String) {
        manager.join(uid: uid)
    }
    
    func playbackEngine(_ playbackEngine: DbyPlaybackEngine, didOfflineOfUid uid: String) {
        manager.leave(uid: uid)
    }
    
    func playbackEngine(_ playbackEngine: DbyPlaybackEngine, videoStateChangedOfUid uid: String, state enabled: Bool) {
        manager.videoChangeof(uid: uid, isOn: enabled)
        //print("uid \(uid) hasVideo: \(enabled)")

    }
    
    func playbackEngine(_ playbackEngine: DbyPlaybackEngine, didGetVideoTotalTime totalTime: TimeInterval) {
        playControlBar.videoLength = totalTime/1000
    }
    
    func playbackEngine(_ playbackEngine: DbyPlaybackEngine, seekResult statusCode: Int) {
        
    }
    
    func playbackEngine(_ playbackEngine: DbyPlaybackEngine, didPlayAtTime currentTime: TimeInterval) {
        
        if !sliderBeginTouch {
            self.playControlBar.currentTime = currentTime/1000
        }

        self.isPlaying = true
        
        // 过滤一分钟前的 聊天记录，排序展示
        
        guard let all = self.allMessages else {
            return;
        }
        
        let startTime = self.startTimestamp
        if startTime <= 0 {
            return
        }
        
        //一秒以内的
        let range = startTime + 1000 + currentTime
        
        self.messages = all.filter({ (msg) -> Bool in
            if let recordTime = (msg["recordTime"] as? Double) {
                return recordTime <= range
            }
            return false
        })
        
        self.tableView .reloadData()
    }
    
    func didPlayEnd(with playbackEngine: DbyPlaybackEngine) {
        
        self.isPlaying = false
        self.playEnd = true
        self.playControlBar.currentTime = 0
        self.playControlBar.isPlaying = false
        self.playControlBar.rate = 1.0
        self.bigUidLabel.text = ""
        
        self.messages = nil
        self.tableView .reloadData()
        
        self.stackView.isHidden = true
                
        manager.leaveChannel()
        
        self.noOneLabel.text = "播放结束，请点击重新开始"
        self.noOneLabel.isHidden = false
    }
    
    internal func playbackEngine(_ playbackEngine: DbyPlaybackEngine, didReceiveMessages messages: [[String : Any]]) {
        self.allMessages = messages
    }
    
    func playbackEngine(_ playbackEngine: DbyPlaybackEngine, didGetStartTime startTime: TimeInterval) {
        self.startTimestamp = startTime
    }
    func playbackEngine(_ playbackEngine: DbyPlaybackEngine, seekTo targetTime: TimeInterval, result statusCode: Int) {
        
        MBProgressHUD.stopLoading(in: self.view)
        if statusCode == 0 {
            MBProgressHUD.showMessage(msg: "seek success", inView: self.view)
        } else {
            MBProgressHUD.showMessage(msg: "seek failed: \(statusCode)", inView: self.view)
        }
    }
    func playbackEngine(_ playbackEngine: DbyPlaybackEngine, setSpeed speed: CGFloat, result statusCode: Int) {
        if statusCode == 0 {
            self.rate = Float(speed)
        }
    }
}



extension DbyPlaybackViewController : UICollectionViewDelegate {
    
}

extension DbyPlaybackViewController : UICollectionViewDataSource {
    func collectionView(_ collectionView: UICollectionView, cellForItemAt indexPath: IndexPath) -> UICollectionViewCell {
        let cell = collectionView.dequeueReusableCell(withReuseIdentifier: "VideoCollectionViewCell", for: indexPath) as! VideoCollectionViewCell
        let item = manager.smallItems[indexPath.item]
        item.view = cell.videoView
        cell.hasVideo = item.hasVideo
        cell.isLocal = false
        cell.uidLabel.text = item.uid
        cell.audioStateIcon.isHidden = true
        cell.callback = { [weak self] in
            
            guard let playing = self?.isPlaying else {
                return
            }
            
            if !playing {
                return
            }
            self?.manager.changeToLarge(uid: item.uid)
        }
        if item.hasVideo {
            self.playbackEngine.setVideoStream(item.uid, type: .low)
        }
        return cell
    }
    
    func collectionView(_ collectionView: UICollectionView, numberOfItemsInSection section: Int) -> Int {
        return manager.smallItems.count
    }
}


extension DbyPlaybackViewController : PlaybackManagerDelegate {
    
    func didJoinWith(item: PlaybackItem) {
        playbackEngine.setVideoView(item.canvas)
    }
    
    func didLeaveWith(item: PlaybackItem) {
        item.canvas.view .removeFromSuperview()
    }
    
    func didBigAreaChangeWith(item: PlaybackItem, type: DataEventType) {
        
        switch type {
        case .Insert, .Update:
            item.view = self.bigView
            self.bigUidLabel.text = item.uid
            if item.hasVideo {
                self.playbackEngine.setVideoStream(item.uid, type: .high)
            }
            self.bigView.isHidden = !item.hasVideo
            self.stackView.isHidden = item.hasVideo
            self.noOneLabel.isHidden = true
        default:
            item.canvas.view.removeFromSuperview()
            self.stackView.isHidden = true
            self.bigView.isHidden = true
            self.bigUidLabel.text = nil
            self.noOneLabel.text = "No one in channel!"
            self.noOneLabel.isHidden = false
        }
    }
    
    func didSmallAreaChangeWith(index: Int, type: DataEventType) {
        switch type {
        case .Insert:
            self.collectionView.insertItems(at: [IndexPath.init(item: index, section: 0)])
        case .Update:
            self.collectionView.reloadItems(at: [IndexPath.init(item: index, section: 0)])
        case .Delete:
            self.collectionView.deleteItems(at: [IndexPath.init(item: index, section: 0)])
        default:
            break
        }
        
    }
}

extension DbyPlaybackViewController : PlayControlBarDelegate {
    func onSlideTouchBegin() {
        sliderBeginTouch = true
    }
    
    func onSlideTouchEnd() {
        sliderBeginTouch = false
    }
    
    func onclickPlay(button: UIButton) {
        
        if playEnd {
            startEngine()
            playEnd = false
            return
        }
        
        if isPlaying {
            playbackEngine.pause()
            isPlaying = false
        } else {
            playbackEngine.play()
            isPlaying = true
        }
    }
    
    func onSlideChangeTo(value: Double) {
        //print("---------onSlideChangeTo,value\(value * 1000)")

        let ret = self.playbackEngine.seek(to: value * 1000)
        if ret == 0 {
            MBProgressHUD.startLoading(withMessage: "seeking", in: self.view)
        } else {
            // seek failed
            MBProgressHUD.showMessage(msg: "seek failed", inView: self.view)
        }
    }
    
    func onClickRate(button: UIButton) {
        
        let sheet = UIAlertController.init(title: "提示", message: "选择播放速率", preferredStyle: .actionSheet)
        sheet.addAction(UIAlertAction.init(title: "1.0", style: .default, handler: { (action) in
            self.playbackEngine.setSpeed(1.0)
        }))
        sheet.addAction(UIAlertAction.init(title: "1.5", style: .default, handler: { (action) in
            self.playbackEngine.setSpeed(1.5)
        }))
        
        sheet.addAction(UIAlertAction.init(title: "2.0", style: .default, handler: { (action) in
            self.playbackEngine.setSpeed(2.0)
        }))
        
        sheet.addAction(UIAlertAction.init(title: "cancel", style: .destructive, handler: nil))
        if let popoverPresentationController = sheet.popoverPresentationController {
            popoverPresentationController.sourceView = button
            popoverPresentationController.sourceRect = button.bounds
        }
        self.present(sheet, animated: true, completion: nil)
    }
}

extension DbyPlaybackViewController {
    @objc func active(_ notification: Notification) {
        self.playbackEngine.resumeApi()
        print("active")
        
    }
    @objc func inactive(_ notification: Notification) {
        self.playbackEngine.pauseApi()
        print("inactive")
    }
}


extension DbyPlaybackViewController: UITableViewDataSource {
    func tableView(_ tableView: UITableView, cellForRowAt indexPath: IndexPath) -> UITableViewCell {
        let cell = tableView.dequeueReusableCell(withIdentifier: "ChatTableViewCell") as! ChatTableViewCell
        cell.messageLabel.text = nil
        if let msgs = self.messages {
            let msg = msgs[indexPath.row]
            
            if let uid = msg["uid"], let content = msg["content"] {
                cell.messageLabel.text = "\(uid) : \(content)"
            }
        }
        return cell
    }
    
    func tableView(_ tableView: UITableView, numberOfRowsInSection section: Int) -> Int {
        if let messages = self.messages {
            return messages.count
        }
        return 0
    }
}

