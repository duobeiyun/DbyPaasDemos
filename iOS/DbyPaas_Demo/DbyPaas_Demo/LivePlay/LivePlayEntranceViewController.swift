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
    
    let channelKey = "dby_channel_key"
    let uidKey = "dby_uid_key"
    
    
    override func viewDidLoad() {
        super.viewDidLoad()
        
        self.channelIdTextField.text = UserDefaults.standard.object(forKey: channelKey) as? String
        self.uidTextField.text = UserDefaults.standard.object(forKey: uidKey) as? String
        
        // Do any additional setup after loading the view.
    }
    
    @IBAction func onClickEnter(_ sender: Any) {
        
        guard let channelId = channelIdTextField.text, let uid = uidTextField.text else {
            return;
        }
        
        if isValidChannelId(channelId) && isValidUid(uid) {
            let storyboard = UIStoryboard.init(name: "Main", bundle: nil)
            let videoChatVC = storyboard.instantiateViewController(withIdentifier: "DbyVideoChatViewController")
            if let chatVC = videoChatVC as? DbyVideoChatViewController {
                chatVC.channelId = channelId
                chatVC.uid = uid
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
        if let regex = try? NSRegularExpression.init(pattern:"[0-9]{1,32}$" , options: .caseInsensitive) {
            
            let results = regex.matches(in: uid, options: .reportCompletion, range: NSRange.init(location: 0, length: uid.count))
            
            if results.count > 0 {
                return true
            }
        }
        return false
    }
    
}

