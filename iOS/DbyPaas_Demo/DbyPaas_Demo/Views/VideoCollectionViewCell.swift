//
//  VideoCollectionViewCell.swift
//  dby_compare
//
//  Created by yxibng on 2019/6/11.
//  Copyright © 2019 yxibng. All rights reserved.
//

import UIKit

class VideoCollectionViewCell: UICollectionViewCell {

    @IBOutlet weak var videoView: DbyVideoView!

    var callback:(() ->Void)?
    override func awakeFromNib() {
        super.awakeFromNib()
        self.videoView.switchToLargeButton.addTarget(self, action: #selector(onClickSwitchMode(_:)), for: .touchUpInside)
    }

    @objc func onClickSwitchMode(_ sender: UIButton) {
        self.callback?()
    }
}
