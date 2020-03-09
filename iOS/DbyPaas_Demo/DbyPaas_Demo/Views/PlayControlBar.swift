//
//  PlayControlBar.swift
//  dby_compare
//
//  Created by yxibng on 2019/7/19.
//  Copyright © 2019 yxibng. All rights reserved.
//

import UIKit

@objc protocol PlayControlBarDelegate {
    @objc optional

    func onclickPlay(button: UIButton)
    func onClickRate(button: UIButton)
    /// 进度条变更的回调
    ///
    /// - Parameter value: 秒
    func onSlideChangeTo(value: Double)
    func onSlideTouchBegin()
    func onSlideTouchEnd()
}

class PlayControlBar: UIView {

    let playControlHeight = 44.0

    var isPlaying: Bool = false {
        didSet {
           self.playButton.isSelected = isPlaying
        }
    }

    var rate: Float = 1.0 {
        didSet {
            self.rateButton.setTitle("\(rate)x", for: .normal)
        }
    }

    let numberFormatter: NumberFormatter = {
        let numberFormatter = NumberFormatter()
        //设置number显示样式
        numberFormatter.numberStyle = .none //四舍五入的整数
        numberFormatter.formatWidth = 2 //补齐10位
        numberFormatter.paddingCharacter = "0" //不足位数用0补
        numberFormatter.paddingPosition = .beforePrefix  //补在前面
        return numberFormatter
    }()
    //seconds
    var currentTime = 0.0 {
        didSet {
            self.startLabel.text = secondsToTime(seconds: Int(currentTime))
            self.slide.setValue(Float(currentTime), animated: true)
        }
    }
    //seconds
    var videoLength = 0.0 {
        didSet {
            self.endLabel.text = secondsToTime(seconds: Int(videoLength))
            self.slide.maximumValue = Float(videoLength)
        }
    }

    func secondsToTime(seconds: Int) -> String? {
        let tuple = (hour: seconds / 3600, minute: (seconds % 3600) / 60, seconds: (seconds % 3600) % 60)

        if let hour = numberFormatter.string(from: NSNumber.init(value: tuple.hour)),
        let minute = numberFormatter.string(from: NSNumber.init(value: tuple.minute)),
            let seconds = numberFormatter.string(from: NSNumber.init(value: tuple.seconds)) {
            return "\(hour):\(minute):\(seconds)"
        }
        return nil
    }

    @objc func clickPlay(sender: UIButton) {
        if let onClickPlay = self.delegate?.onclickPlay {
            onClickPlay(sender)
        }
    }

    @objc func changeProgressWith(slide: UISlider) {
        self.delegate?.onSlideChangeTo(value: Double(slide.value))
    }

    @objc func touchSlider(slide: UISlider, forEvent event: UIEvent) {

        if let touchEvent = event.allTouches?.first {
            switch touchEvent.phase {
            case .began:
                print("touches began")
                self.delegate?.onSlideTouchBegin()
            case .moved:
                print("\(slide.value)")
                self.startLabel.text = secondsToTime(seconds: Int(slide.value))
            case .ended:
                print("touches ended")
                self.delegate?.onSlideTouchEnd()
            default:
                break
            }
        }

    }

    @objc func changeRate(sender: UIButton) {
        self.delegate?.onClickRate(button: sender)
    }

    weak var delegate: PlayControlBarDelegate?

    lazy var slide: UISlider = {
        let slide = UISlider.init()
        slide.addTarget(self, action: #selector(changeProgressWith(slide:)), for: .valueChanged)
        slide.addTarget(self, action: #selector(touchSlider(slide:forEvent:)), for: .allEvents)
        slide.minimumValue = 0
        slide.isContinuous = false
        return slide
    }()
    lazy var playButton: UIButton = {
        let button = UIButton.init()
        button.setImage(UIImage.init(named: "start"), for: .normal)
        button.setImage(UIImage.init(named: "stop"), for: .selected)
        button.addTarget(self, action: #selector(clickPlay(sender:)), for: .touchUpInside)
        return button
    }()

    lazy var rateButton: UIButton = {
        let button = UIButton.init()
        button.setTitle("1.0x", for: .normal)
        button.addTarget(self, action: #selector(changeRate(sender:)), for: .touchUpInside)
        return button
    }()

    let startLabel: UILabel = {
       let label = UILabel.init()
       return label
    }()

    let endLabel: UILabel = {
        let label = UILabel.init()
        return label
    }()

    override init(frame: CGRect) {
        super.init(frame: frame)
        self .setup()
    }

    required init?(coder aDecoder: NSCoder) {
        super.init(coder: aDecoder)
        self.setup()
    }

    func setup() {

        let blurView = UIVisualEffectView.init(effect: UIBlurEffect.init(style: .dark))
        addSubview(blurView)
        blurView.snp.makeConstraints { (make) in
            make.edges.equalTo(self)
        }

        addSubview(slide)
        addSubview(playButton)
        addSubview(startLabel)
        addSubview(endLabel)
        addSubview(rateButton)

        playButton.snp.makeConstraints { (make) in
            make.left.top.bottom.equalTo(self)
            make.size.equalTo(CGSize.init(width: playControlHeight, height: playControlHeight))
        }

        rateButton.snp.makeConstraints { (make) in
            make.right.top.equalTo(self)
            make.size.equalTo(CGSize.init(width: playControlHeight, height: playControlHeight))
        }

        slide.snp.makeConstraints { (make) in
            make.left.equalTo(playButton.snp_right)
            make.right.equalTo(rateButton.snp_left)
            make.top.equalTo(self)
            make.height.equalTo(playControlHeight/2)
        }

        startLabel.snp.makeConstraints { (make) in
            make.left.equalTo(slide)
            make.bottom.equalTo(self)
        }

        endLabel.snp.makeConstraints { (make) in
            make.right.equalTo(slide)
            make.bottom.equalTo(self)
        }
    }

}
