//
//  DbyPlaybackEntranceViewController.swift
//  DbyPaas_Demo
//
//  Created by yxibng on 2019/10/30.
//  Copyright © 2019 yxibng. All rights reserved.
//

import UIKit
import AVFoundation
import QRCodeReader
import MBProgressHUD

class DbyPlaybackEntranceViewController: UIViewController {

    let recordIdKey = "dby_pb_recordid_key"
    
    lazy var readerVC: QRCodeReaderViewController = {
        let builder = QRCodeReaderViewControllerBuilder {
            $0.reader = QRCodeReader(metadataObjectTypes: [.qr], captureDevicePosition: .back)
            
            // Configure the view controller (optional)
            $0.showTorchButton        = false
            $0.showSwitchCameraButton = false
            $0.showCancelButton       = true
            $0.showOverlayView        = true
            $0.rectOfInterest         = CGRect(x: 0.2, y: 0.2, width: 0.6, height: 0.6)
        }
        
        return QRCodeReaderViewController(builder: builder)
    }()
    
    
    @IBOutlet weak var textField: UITextField!
    override func viewDidLoad() {
        super.viewDidLoad()
        self.title = "回放"
        self.navigationItem.leftBarButtonItem = UIBarButtonItem.init(barButtonSystemItem: .stop, target: self, action: #selector(quit(item:)))
        self.textField.text = UserDefaults.standard.string(forKey: recordIdKey)
    }
    
    @IBAction func enterPlayback(_ sender: UIButton) {

        guard let rid = self.textField.text?.trimmingCharacters(in: CharacterSet.whitespacesAndNewlines) else {
            MBProgressHUD.showMessage(msg: "please input recordId", inView: self.view)
            return
        }
        
        
        if rid.count == 0 {
            MBProgressHUD.showMessage(msg: "please input recordId", inView: self.view)
            return
        }
        
        UserDefaults.standard.setValue(rid, forKey: recordIdKey)
        UserDefaults.standard.synchronize()
        let storyboard = UIStoryboard.init(name: "Main", bundle: nil)
        let playbackVC = storyboard.instantiateViewController(withIdentifier: "DbyPlaybackViewController") as! DbyPlaybackViewController
        playbackVC.recordId = rid
        self.navigationController?.pushViewController(playbackVC, animated: true)
    
    }
    
    @IBAction func scanQRcode(_ sender: Any) {
        readerVC.delegate = self
        readerVC.modalPresentationStyle = .formSheet
        present(readerVC, animated: true, completion: nil)
    }

    @objc func quit(item: UIBarButtonItem) {
        self.navigationController?.popViewController(animated: true)
    }
    
}


extension DbyPlaybackEntranceViewController : QRCodeReaderViewControllerDelegate {
    func reader(_ reader: QRCodeReaderViewController, didScanResult result: QRCodeReaderResult) {
        reader.stopScanning()
        self.textField.text = result.value
        dismiss(animated: true, completion: nil)
    }
    

    func readerDidCancel(_ reader: QRCodeReaderViewController) {
        reader.stopScanning()
        
        dismiss(animated: true, completion: nil)
    }
}
