//
//  ViewController.swift
//  DbyPaas_Demo
//
//  Created by yxibng on 2019/10/29.
//  Copyright © 2019 yxibng. All rights reserved.
//

import UIKit
import DbyPaas_iOS

class LivePlayEntranceViewController: UIViewController {

    @IBOutlet weak var channelIdTextField: UITextField!
    @IBOutlet weak var uidTextField: UITextField!

    @IBOutlet weak var modeSeg: UISegmentedControl!
    @IBOutlet weak var roleSeg: UISegmentedControl!
    @IBOutlet weak var streamSeg: UISegmentedControl!

    let channelKey = "dby_channel_key"
    let uidKey = "dby_uid_key"

    var channelProfile: JoinChannelProfile = .broadcast {
        didSet {

            if self.channelProfile == .broadcast {
                self.roleSeg.selectedSegmentIndex = self.role == .broadcaster ? 0 : 1
                self.roleSeg.isHidden = false
                self.streamSeg.isHidden = true
            } else {
                self.streamSeg.selectedSegmentIndex = self.stream == .dual ? 0 : 1
                self.roleSeg.isHidden = true
                self.streamSeg.isHidden = false
            }
        }

    }
    var role: JoinRole = .broadcaster
    var stream: JoinStreamType = .dual

    override func viewDidLoad() {
        super.viewDidLoad()

        self.channelIdTextField.text = UserDefaults.standard.object(forKey: channelKey) as? String
        self.uidTextField.text = UserDefaults.standard.object(forKey: uidKey) as? String

        // Do any additional setup after loading the view.
    }

    @IBAction func onClickEnter(_ sender: UIButton) {

        guard let channelId = channelIdTextField.text, let uid = uidTextField.text else {
            return
        }

        if isValidChannelId(channelId) && isValidUid(uid) {

            let title = sender.currentTitle!
            var line: VideoLine
            if title == "多贝" {
                line = .DBY
            } else {
                line = .AGORA
            }

            let storyboard = UIStoryboard.init(name: "Main", bundle: nil)
            let videoChatVC = storyboard.instantiateViewController(withIdentifier: "DbyVideoChatViewController")
            if let chatVC = videoChatVC as? DbyVideoChatViewController {
                let joinInfo = JoinChannelInfo.init(channelId: channelId,
                                                    uid: uid,
                                                    line: line,
                                                    channelProfile: self.channelProfile,
                                                    role: self.role,
                                                    streamType: self.stream)

                chatVC.joinChannelInfo = joinInfo
                chatVC.modalPresentationStyle = .fullScreen
                present(chatVC, animated: true, completion: nil)
            }

            UserDefaults.standard.set(self.channelIdTextField.text, forKey: channelKey)
            UserDefaults.standard.set(uid, forKey: uidKey)
            UserDefaults.standard.synchronize()

        } else {
            //invalid channelId or uid
        }
    }

    @IBAction func onChangeMode(_ sender: UISegmentedControl) {
        let isBrocastMode = sender.selectedSegmentIndex == 0
        self.channelProfile = isBrocastMode ? .broadcast : .communaction
    }
    @IBAction func onChangeRole(_ sender: UISegmentedControl) {
        self.role = sender.selectedSegmentIndex == 0 ? .broadcaster : .audience
    }

    @IBAction func onChangeStreamType(_ sender: UISegmentedControl) {
        self.stream = sender.selectedSegmentIndex == 0 ? .dual : .single
    }

}

extension LivePlayEntranceViewController {
    func isValidChannelId(_ channelId: String) -> Bool {
        if let regex = try? NSRegularExpression.init(pattern: "^[\\w!#$%&()+-:;<=.>?@\\[\\]^_{}|~, ]{1,64}$", options: .caseInsensitive) {

            let results = regex.matches(in: channelId, options: .reportCompletion, range: NSRange.init(location: 0, length: channelId.count))

            if results.count > 0 {
                return true
            }
        }
        return false
    }

    func isValidUid(_ uid: String) -> Bool {
        if let regex = try? NSRegularExpression.init(pattern: "[0-9]{1,32}$", options: .caseInsensitive) {

            let results = regex.matches(in: uid, options: .reportCompletion, range: NSRange.init(location: 0, length: uid.count))

            if results.count > 0 {
                return true
            }
        }
        return false
    }

}
