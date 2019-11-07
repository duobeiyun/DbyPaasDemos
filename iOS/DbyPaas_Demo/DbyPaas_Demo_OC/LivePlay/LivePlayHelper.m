//
//  LivePlayHelper.m
//  DbyPaas_Demo_OC
//
//  Created by yxibng on 2019/11/7.
//  Copyright © 2019 yxibng. All rights reserved.
//

#import "LivePlayHelper.h"

@interface LivePlayHelper ()

@property (nonatomic, strong) NSMutableDictionary *userMap;
@property (nonatomic, strong) NSLock *lock;
@property (nonatomic, copy) NSString *localUid;

@end

@implementation LivePlayHelper

- (instancetype)initWithDelegate:(id<LivePlayHelperDelegate>)delegate
{
    self = [super init];
    if (self) {
        _userMap = @{}.mutableCopy;
        _lock = [[NSLock alloc] init];
        _delegate = delegate;
    }
    return self;
}

- (DbyUserModel *)modelWithUid:(NSString *)uid
{
    if (!uid) {
        return nil;
    }
    [self.lock lock];
    DbyUserModel *model = self.userMap[uid];
    [self.lock unlock];
    return model;
}


- (void)joinWithUid:(NSString *)uid isLocal:(BOOL)isLocal
{
    if (!uid) {
        return;
    }
    
    if (isLocal) {
        self.localUid = uid;
    }
    
    DbyUserModel *model = [self modelWithUid:uid];
    if (!model) {
        model = [[DbyUserModel alloc] initWithUid:uid isLocal:isLocal];
        [self.lock lock];
        self.userMap[uid] = model;
        [self.lock unlock];
    }
    
    if (isLocal) {
        //本地用户上线
        if (self.userMap.count == 1) {
            model.isLarge = YES;
        }
    } else {
        //远端用户上线，显示为大视图
        model.isLarge = YES;
        
        //本地用户，变为小视图
        DbyUserModel *localUser = [self modelWithUid:self.localUid];
        if (localUser) {
            localUser.isLarge = NO;
            if ([self.delegate respondsToSelector:@selector(videoChangeOfUser:)]) {
                [self.delegate videoChangeOfUser:localUser];
            }
        }
    }

    if ([self.delegate respondsToSelector:@selector(didJoinOfUser:)]) {
        [self.delegate didJoinOfUser:model];
    }
    
}

- (void)LeaveWithUid:(NSString *)uid
{
    if (!uid) {
        return;
    }
    DbyUserModel *model = [self modelWithUid:uid];
    
    if (!model) {
        return;
    }
    [self.lock lock];
    [self.userMap removeObjectForKey:uid];
    [self.lock unlock];
    
    if ([self.delegate respondsToSelector:@selector(didLeaveOfUser:)]) {
        [self.delegate didLeaveOfUser:model];
    }
    
    if (self.userMap.count == 1) {
        //本地用户，变为大视图
        DbyUserModel *localUser = [self modelWithUid:self.localUid];
        if (localUser) {
            localUser.isLarge = YES;
            if ([self.delegate respondsToSelector:@selector(videoChangeOfUser:)]) {
                [self.delegate videoChangeOfUser:localUser];
            }
        }
    }
}


- (void)audioChangeOfUid:(NSString *)uid enabled:(BOOL)enabled
{
    if (!uid) {
        return;
    }
    DbyUserModel *model = [self modelWithUid:uid];
    
    if (!model) {
        return;
    }
    
    model.micOn = enabled;
    if ([self.delegate respondsToSelector:@selector(audioChangeOfUser:)]) {
        [self.delegate audioChangeOfUser:model];
    }
    
    
}

- (void)videoChangeOfUid:(NSString *)uid enabled:(BOOL)enabled
{
    if (!uid) {
        return;
    }
    DbyUserModel *model = [self modelWithUid:uid];
    
    if (!model) {
        return;
    }
    
    model.camOn = enabled;
    if ([self.delegate respondsToSelector:@selector(videoChangeOfUser:)]) {
        [self.delegate videoChangeOfUser:model];
    }
}

@end

