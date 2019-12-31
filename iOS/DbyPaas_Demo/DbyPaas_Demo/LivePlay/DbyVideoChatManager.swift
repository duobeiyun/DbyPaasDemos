//
//  DbyVideoChatManager.swift
//  DbyPaas_Demo
//
//  Created by yxibng on 2019/12/31.
//  Copyright © 2019 yxibng. All rights reserved.
//

import UIKit

@objc enum DataEventType: Int {
    case Insert
    case Update
    case Delete
}

enum DataAreaType: Int {
    case Big
    case Local
    case Remote
}

@objc enum StreamType: Int {
    case high, low
}


@objc protocol DbyVideoChatManagerDelegate: NSObjectProtocol {
    
    @objc optional func didJoinWith(item: DbyVideoItem)
    @objc optional func didLeaveWith(item: DbyVideoItem)
    @objc optional func didBigAreaChangeWith(item: DbyVideoItem, type: DataEventType)
    @objc optional func didRemoteAreaChangeWith(index:Int, type: DataEventType)
    @objc optional func didLocalAreaChangeWith(item: DbyVideoItem, type: DataEventType)
    @objc optional func didChangeStreamOf(item: DbyVideoItem, to streamType: StreamType)
}


class DbyVideoChatManager: NSObject {

    weak var delegate: DbyVideoChatManagerDelegate?
    init(delegate: DbyVideoChatManagerDelegate?) {
        self.delegate = delegate
        super.init()
    }
    
    let localItem: DbyVideoItem = {
        let item = DbyVideoItem.init()
        item.isLocal = true
        item.hasVideo = true
        item.hasAudio = true
        item.isLarge = true
        return item
    }()
    
    var largeItem: DbyVideoItem?
    var remoteSmallItems = [DbyVideoItem]()
    
    func hasAnyoneRemoteOnline() -> Bool {
        
        if let large = largeItem {
            if !large.isLocal {
                return true
            }
        }
        return remoteSmallItems.count > 0;
    }
    
    func remoteItemFromSmallItemsWith(uid: UInt) -> (index: Int, item: DbyVideoItem)? {
        
        for (index, item) in self.remoteSmallItems.enumerated() {
            if item.uid == uid {
                return (index, item)
            }
        }
        return nil
    }
    
    func remoteItemFromAllRemoteItems(uid: UInt) -> DbyVideoItem? {
        
        if largeItem?.uid == uid {
            return largeItem
        }
        
        if let (_, item) = remoteItemFromSmallItemsWith(uid: uid) {
            return item
        }
        return nil
    }
    
    
    
    func localJoinWith(uid: UInt)  {
        
        if localItem.uid == uid {
            return
        }
        localItem.uid = uid
        localItem.isLarge = !hasAnyoneRemoteOnline()
        delegate?.didJoinWith?(item: localItem)
        if localItem.isLarge {
            largeItem = localItem
            delegate?.didBigAreaChangeWith?(item: localItem, type: .Insert)
        } else {
            delegate?.didLocalAreaChangeWith?(item: localItem, type: .Insert)
        }
    }
    
    func localAudioChange(enabled: Bool) {
        localItem.hasAudio = enabled
        if localItem.isLarge {
            delegate?.didBigAreaChangeWith?(item: localItem, type: .Update)
        } else {
            delegate?.didLocalAreaChangeWith?(item: localItem, type: .Update)
        }
    }
    
    func localVideoChange(enabled: Bool) {
        localItem.hasVideo = enabled
        if localItem.isLarge {
            delegate?.didBigAreaChangeWith?(item: localItem, type: .Update)
        } else {
            delegate?.didLocalAreaChangeWith?(item: localItem, type: .Update)
        }
    }
    
    func remoteJoinWith(uid: UInt) {
        
        if let _ = self.remoteItemFromSmallItemsWith(uid: uid) {
            return;
        }
        
        let item = DbyVideoItem.init()
        item.uid = uid
        item.isLocal = false
        
        if let largeItem = largeItem {
            if largeItem.isLocal {
                item.isLarge = true
                self.largeItem = item
                self.delegate?.didJoinWith?(item: item)
                self.delegate?.didBigAreaChangeWith?(item: item, type: .Update)
                
                localItem.isLarge = false
                self.delegate?.didLocalAreaChangeWith?(item: localItem, type: .Insert)
                
            } else {
                item.isLarge = false
                self.remoteSmallItems.append(item)
                self.delegate?.didJoinWith?(item: item)
                self.delegate?.didRemoteAreaChangeWith?(index: self.remoteSmallItems.count - 1, type: .Insert)
            }
        } else {
            item.isLarge = true
            self.largeItem = item
            self.delegate?.didJoinWith?(item: item)
            self.delegate?.didBigAreaChangeWith?(item: item, type: .Insert)
        }
    }
    
    func remoteLeaveWith(uid: UInt)  {
        
        if largeItem?.uid == uid {
            
            self.delegate?.didLeaveWith?(item: largeItem!)
            
            //find next remote, make it to be large
            if let nextItem = self.remoteSmallItems.first {
                nextItem.isLarge = true
                self.largeItem = nextItem
                self.remoteSmallItems.remove(at: 0)
                self.delegate?.didRemoteAreaChangeWith?(index: 0, type: .Delete)
                self.delegate?.didBigAreaChangeWith?(item: nextItem, type: .Update)
            } else {
                //can not find next remote item, make local to be large
                self.delegate?.didLocalAreaChangeWith?(item: localItem, type: .Delete)
                localItem.isLarge = true
                self.largeItem = localItem
                self.delegate?.didBigAreaChangeWith?(item: localItem, type: .Update)
            }
        } else {
            
            if let (index, item) = self.remoteItemFromSmallItemsWith(uid: uid) {
                self.delegate?.didLeaveWith?(item: item)
                
                self.remoteSmallItems.remove(at: index)
                self.delegate?.didRemoteAreaChangeWith?(index: index, type: .Delete)
            }
        }
        
    }
    
    func remoteAudioChangeWith(uid: UInt, enabled: Bool) {
        
        if largeItem?.uid == uid {
            largeItem?.hasAudio = enabled
            self.delegate?.didBigAreaChangeWith?(item: largeItem!, type: .Update)
            return
        }
        
        if let (index, item) = self.remoteItemFromSmallItemsWith(uid: uid) {
            item.hasAudio = enabled
            self.delegate?.didRemoteAreaChangeWith?(index: index, type: .Update)
        }
        
        
    }
    
    func remoteVideoChangeWith(uid: UInt, enabled: Bool) {
        
        if largeItem?.uid == uid {
            
            largeItem?.isLoading = enabled
            largeItem?.hasVideo = enabled
            
            if enabled && (!largeItem!.isLocal) {
                //切换到大流
                self.delegate?.didChangeStreamOf?(item: largeItem!, to: .high)
            }
            
            self.delegate?.didBigAreaChangeWith?(item: largeItem!, type: .Update)
            return
        }
        
        if let (index, item) = self.remoteItemFromSmallItemsWith(uid: uid) {
            item.isLoading = enabled
            item.hasVideo = enabled
            if enabled {
                //切换到小流
                self.delegate?.didChangeStreamOf?(item: item, to: .low)
            }
            self.delegate?.didRemoteAreaChangeWith?(index: index, type: .Update)
        }
        
    }
    
    func receiveRemoteFirstVideoFrameOf(uid: UInt) {
        if largeItem?.uid == uid {
            largeItem?.isLoading = false
            self.delegate?.didBigAreaChangeWith?(item: largeItem!, type: .Update)
            return
        }
        
        if let (index, item) = self.remoteItemFromSmallItemsWith(uid: uid) {
            item.isLoading = false
            self.delegate?.didRemoteAreaChangeWith?(index: index, type: .Update)
        }
    }

    func swithToLargeWith(uid:UInt) {
        
        if largeItem?.uid == uid {
            return
        }
        
        guard let latestLargeItem = largeItem else {
            return;
        }
        
        guard let (index, nextLargeItem) = self.remoteItemFromSmallItemsWith(uid: uid) else {
            return;
        }
        
        latestLargeItem.isLarge = false
        nextLargeItem.isLarge = true
        if latestLargeItem.hasVideo {
            self.delegate?.didChangeStreamOf?(item: latestLargeItem, to: .low)
        }
        
        if nextLargeItem.hasVideo {
            self.delegate?.didChangeStreamOf?(item: nextLargeItem, to: .high)
        }
        
        self.largeItem = nextLargeItem
        self.remoteSmallItems[index] = latestLargeItem
        self.delegate?.didBigAreaChangeWith?(item: nextLargeItem, type: .Update)
        self.delegate?.didRemoteAreaChangeWith?(index: index, type: .Update)
    }
}



