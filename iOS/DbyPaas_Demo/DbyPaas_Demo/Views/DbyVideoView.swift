//
//  DbyVideoView.swift
//  dby_compare
//
//  Created by yxibng on 2019/12/25.
//  Copyright © 2019 yxibng. All rights reserved.
//

import UIKit
import SnapKit

class DbyVideoView: UIView {

    var uid: String = "" {
        didSet {
            self.uidLabel.text = uid
        }
    }
    
    var volume: String = "" {
        didSet {
            self.volumeLabel.text = volume
        }
    }
    
    var dimension: String = "" {
        didSet {
            self.dimensionLabel.text = dimension
        }
    }
    
    var hasVideo: Bool = false {
        didSet {
            self.videoClosedView.isHidden = hasVideo
            self.videoContentView.isHidden = !hasVideo
        }
    }
    
    var canBeLarge: Bool = false {
        didSet {
            self.switchToLargeButton.isHidden = !canBeLarge
        }
    }
    
    
    var isLarge: Bool = false {
        didSet {
            self.camCloseTipLabel.isHidden = !isLarge
            if isLarge {
                self.camCloseIcon.image = UIImage.init(named: "screen_closed")
            } else {
                self.camCloseIcon.image = UIImage.init(named: "screen_closed_other")
            }
        }
    }
    
    var isLoading: Bool = false {
        didSet {
            if isLoading {
                self.loadingView.startAnimating()
            } else {
                self.loadingView.stopAnimating()
            }
        }
    }
    
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
    
    lazy var uidLabel: UILabel = {
        let label =  UILabel.init(frame: .zero)
        label.textColor = UIColor.white
        label.font = UIFont.systemFont(ofSize: 12)
        return label
    }()
    
    lazy var uidView : UIView = {
        let icon = UIImageView.init(frame: .zero)
        icon.image = UIImage.init(named: "user_icon")
    
        let view = UIView.init()
        view.backgroundColor = UIColor.black.withAlphaComponent(0.6)
        view.addSubview(icon)
        view.addSubview(self.uidLabel)
        
        icon.snp.makeConstraints({ (make) in
            make.width.height.equalTo(10)
            make.left.equalTo(view).offset(6)
            make.centerY.equalTo(view)
        })
        
        self.uidLabel.snp.makeConstraints({ (make) in
            make.left.equalTo(icon.snp_right).offset(6)
            make.right.equalTo(view).offset(-6)
            
            make.centerY.equalTo(view)
        })
        
        return view
    }()
    
    lazy var loadingView: UIActivityIndicatorView = {
        let view = UIActivityIndicatorView.init(style: .whiteLarge)
        view.hidesWhenStopped = true
        return view
    }()
    
    lazy var volumeLabel: UILabel = {
        let label =  UILabel.init(frame: .zero)
        label.textColor = UIColor.white
        label.font = UIFont.systemFont(ofSize: 12)
        label.backgroundColor = UIColor.black.withAlphaComponent(0.5)
        return label
    }()
    
    lazy var dimensionLabel: UILabel = {
        let label =  UILabel.init(frame: .zero)
        label.textColor = UIColor.white
        label.font = UIFont.systemFont(ofSize: 12)
        label.backgroundColor = UIColor.black.withAlphaComponent(0.5)
        return label
    }()
    
    lazy var audioStateIcon: UIImageView = {
        let icon = UIImageView.init(frame: .zero)
        return icon;
    }()
    
    let videoContentView = UIView.init()
    
    lazy var switchToLargeButton: UIButton = {
        let button = UIButton.init()
        button.backgroundColor = UIColor.black.withAlphaComponent(0.5)
        button.setImage(UIImage.init(named: "qiehuan"), for: .normal)
        button.addTarget(self, action: #selector(onClickSwitchToLargeButton(_:)), for: .touchUpInside)
        button.isHidden = true
        return button
    }()
    
    let camCloseTipLabel = UILabel.init()
    let camCloseIcon = UIImageView.init()
    
    
    lazy var videoClosedView: UIStackView = makeVideoClosedView()
    
    func makeVideoClosedView() -> UIStackView {
        
        let icon = self.camCloseIcon
        icon.image = UIImage.init(named: "screen_closed_other")
        
        let label = self.camCloseTipLabel
        label.text = "摄像头未开启"
        label.textColor = UIColor.init(red: 144.0/255, green: 144.0/255, blue: 144.0/255, alpha: 1.0)
        label.textColor = UIColor.white
        label.font = UIFont.systemFont(ofSize: 17)
        label.isHidden = true
        
        let stackView = UIStackView.init()
        stackView.addArrangedSubview(icon)
        stackView.addArrangedSubview(label)
        
        
        stackView.axis = .vertical
        stackView.alignment = .center
        stackView.spacing = 20
        stackView.distribution = .fillProportionally
        
        return stackView
    }
    
    override init(frame: CGRect) {
        super.init(frame: frame)
        self .setup()
    }
    
    required init?(coder: NSCoder) {
        super.init(coder: coder)
        self.setup()
    }
    
    func setup() {
    
        self.backgroundColor = UIColor.black
        self.layer.cornerRadius = 4
        self.layer.masksToBounds = true
        
        //video closed view
        addSubview(videoClosedView)
        //video view
        addSubview(videoContentView)
        //uid view
        addSubview(uidView)
        //mic icon
        addSubview(audioStateIcon)
        //volome
        addSubview(volumeLabel)
        //dimension
        addSubview(dimensionLabel)
        //loading
        addSubview(loadingView)
        //switch to large
        addSubview(switchToLargeButton)
        
        
        videoClosedView.snp.makeConstraints { (make) in
            make.center.equalTo(self)
        }
        
        videoContentView.snp.makeConstraints { (make) in
            make.edges.equalTo(self)
        }
        
        uidView.snp.makeConstraints { (make) in
            if #available(iOS 11, *) {
                make.top.equalTo(self.safeAreaLayoutGuide.snp.top)
            } else {
                make.top.equalTo(self)
            }
            make.left.equalTo(self)
            make.height.equalTo(22)
        }
        
        audioStateIcon.snp.makeConstraints { (make) in
            make.left.equalTo(self.uidView)
            make.top.equalTo(self.uidView.snp_bottom)
        }
        
        self.volumeLabel.snp.makeConstraints { (make) in
            make.left.equalTo(self.audioStateIcon.snp_right).offset(5)
            make.centerY.equalTo(self.audioStateIcon)
        }
                
        dimensionLabel.snp.makeConstraints { (make) in
            make.left.equalTo(self.uidView)
            make.top.equalTo(self.audioStateIcon.snp_bottom)
        }
        
        switchToLargeButton.snp_makeConstraints { (make) in
            make.right.equalTo(self).offset(-10)
            make.bottom.equalTo(self).offset(-10)
        }
        
        
        loadingView.snp.makeConstraints { (make) in
            make.center.equalTo(self)
        }
        
        
    }
    
    
    @objc func onClickSwitchToLargeButton(_ sender: UIButton) {
        
    }
    
    
        
}
