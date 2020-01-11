//
//  PlaybackManager.swift
//  dby_compare
//
//  Created by yxibng on 2019/7/19.
//  Copyright © 2019 yxibng. All rights reserved.
//

import UIKit
import DbyPaas_iOS
import SnapKit

class PlaybackItem: NSObject {
    let uid: String
    let canvas: DbyVideoCanvas
    var isLarge: Bool = false
    var hasVideo: Bool = false
    init(uid: String) {
        self.uid = uid
        self.canvas = DbyVideoCanvas.init(view: UIView.init(), uid: uid)
    }
    var view: UIView? {
        didSet {
            if let v = view {
                canvas.view.removeFromSuperview()
                v.addSubview(canvas.view)
                canvas.view.snp.makeConstraints { (make) in
                    make.edges.equalTo(v)
                }
            }
        }
    }
}

@objc protocol PlaybackManagerDelegate {
    
    @objc optional func didBigAreaChangeWith(item: PlaybackItem, type: DataEventType)
    @objc optional func didSmallAreaChangeWith(index:Int, type: DataEventType)
    @objc optional func didJoinWith(item: PlaybackItem)
    @objc optional func didLeaveWith(item: PlaybackItem)
}



class PlaybackManager: NSObject {
    
    var largetItem: PlaybackItem? = nil
    var smallItems = [PlaybackItem]()
    
    weak var delegate: PlaybackManagerDelegate?
    
    func leaveChannel() {
        for item in smallItems {
            self .leave(uid: item.uid)
        }
        
        if let item  = largetItem {
            self.leave(uid: item.uid)
        }
    }
    
    
    func join(uid: String) {
        
        //print("join \(uid)")
        if let large = largetItem {
            if large.uid == uid {
                //已经是大图
                return;
            }
            //大图被占了，显示为小图
            let item = PlaybackItem.init(uid: uid)
            item.isLarge = false
            if smallItems.contains(where: { $0.uid == uid }) {
                //已经存在小图列表里，不处理了
                return;
            }
            self.smallItems.append(item)
            self.delegate?.didJoinWith?(item: item)
            self.delegate?.didSmallAreaChangeWith?(index: self.smallItems.count - 1, type: .Insert)
        } else {
            //大图为空，展示为大图
            largetItem = PlaybackItem.init(uid: uid)
            largetItem?.isLarge = true
            self.delegate?.didJoinWith?(item: largetItem!)
            self.delegate?.didBigAreaChangeWith?(item: largetItem!, type: .Insert)
        }
    }
    
    func leave(uid: String) {
        //print("leave \(uid)")
        if let large = largetItem, large.uid == uid {
            //移出的是大图, //有小图，填补为大图
            if let nextBig = self.smallItems.first {
                largetItem = nextBig
                nextBig.isLarge = true
                self.smallItems.remove(at: 0)
                self.delegate?.didBigAreaChangeWith?(item: nextBig, type: .Update)
                self.delegate?.didSmallAreaChangeWith?(index: 0, type: .Delete)
                self.delegate?.didLeaveWith?(item: large)
            } else {
                self.delegate?.didLeaveWith?(item: large)
                self.delegate?.didBigAreaChangeWith?(item: large, type: .Delete)
                largetItem = nil
            }
            return;
        }
        //移出的是小图
        if let tuple = itemWith(uid: uid) {
            self.smallItems.remove(at: tuple.index)
            self.delegate?.didLeaveWith?(item: tuple.item)
            self.delegate?.didSmallAreaChangeWith?(index: tuple.index, type: .Delete)
            return;
        }
        print("leave \(uid), not found")
    }
    
    func videoChangeof(uid: String, isOn: Bool) {
        if let large = largetItem, large.uid == uid {
            large.hasVideo = isOn
            self.delegate?.didBigAreaChangeWith?(item: large, type: .Update)
            return;
        }
        
        if let tuple = itemWith(uid: uid) {
            tuple.item.hasVideo = isOn
            self.delegate?.didSmallAreaChangeWith?(index: tuple.index, type: .Update)
        }
    }
    
    
    func changeToLarge(uid: String) {
        
        if let large = largetItem, large.uid == uid {
            //已经是大图
            return;
        }
        
        if let tuple = itemWith(uid: uid) {
          
            if let large = largetItem {
                large.isLarge = false
                tuple.item.isLarge = true
                
                smallItems[tuple.index] = large
                largetItem = tuple.item
    
                self.delegate?.didBigAreaChangeWith?(item: tuple.item, type: .Update)
                self.delegate?.didSmallAreaChangeWith?(index: tuple.index, type: .Update)
                
            } else {
                tuple.item.isLarge = true
                largetItem = tuple.item
                smallItems.remove(at: tuple.index)
                self.delegate?.didSmallAreaChangeWith?(index: tuple.index, type: .Delete)
                self.delegate?.didBigAreaChangeWith?(item: tuple.item, type: .Insert)
            }
        }
    }
    
    
    func itemWith(uid: String) -> (index: Int, item: PlaybackItem)? {
        for (index, item) in  self.smallItems.enumerated() {
            if item.uid == uid {
                return (index, item);
            }
        }
        return nil;
    }
}
