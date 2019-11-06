//
//  VideoCollectionViewCell.swift
//  dby_compare
//
//  Created by yxibng on 2019/6/11.
//  Copyright © 2019 yxibng. All rights reserved.
//

import UIKit

class VideoCollectionViewCell: UICollectionViewCell {

    @IBOutlet weak var uidLabel: UILabel!
    @IBOutlet weak var videoView: UIView!
    @IBOutlet weak var icon: UIImageView!
    @IBOutlet weak var button: UIButton!
    @IBOutlet weak var separateBar: UIView!
    @IBOutlet weak var volumeLabel: UILabel!
    
    @IBOutlet weak var loadingIcon: UIActivityIndicatorView!
    
    var callback:(() ->())?
    
    var hasVideo: Bool = false {
        didSet {
            self.icon.isHidden = hasVideo
            self.videoView.isHidden = !hasVideo
        }
    }
    
    var isLocal: Bool = false {
        didSet {
            self.button.isHidden = isLocal
            self.separatorView.isHidden = !isLocal
        }
    }
    
    var isLoading: Bool = false {
        didSet {
            if isLoading {
                self.loadingIcon.startAnimating()
            } else {
                self.loadingIcon.stopAnimating()
            }
        }
    }
    
    
    @IBOutlet weak var audioStateIcon: UIImageView!
    
    var isAudioOn: Bool = false {
        didSet {
            if isAudioOn {
                audioStateIcon.image = UIImage.init(named: "audio_on")
            } else {
                audioStateIcon.image = UIImage.init(named: "audio_mute")
                volumeLabel.text = "0"
            }
        }
    }
    
    
    @IBOutlet weak var separatorView: UIView!
    @IBAction func onClickSwitchMode(_ sender: UIButton) {
        self.callback?()
    }
    
    override func awakeFromNib() {
        super.awakeFromNib()
        
        separateBar.layer.cornerRadius = 0.5
        separateBar.layer.shadowColor = UIColor.black.withAlphaComponent(0.5).cgColor
        separateBar.layer.shadowOffset = CGSize.init(width: 0, height: 2)
        separateBar.layer.shadowRadius = 1.5
        separateBar.layer.shadowOpacity = 1.0
        
    }

}
