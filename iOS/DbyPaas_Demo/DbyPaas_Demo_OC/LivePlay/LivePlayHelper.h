//
//  LivePlayHelper.h
//  DbyPaas_Demo_OC
//
//  Created by yxibng on 2019/11/7.
//  Copyright © 2019 yxibng. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "DbyUserModel.h"
NS_ASSUME_NONNULL_BEGIN
@class LivePlayHelper;

@protocol LivePlayHelperDelegate <NSObject>
@optional
- (void)didJoinOfUser:(DbyUserModel *)user;
- (void)didLeaveOfUser:(DbyUserModel *)user;
- (void)videoChangeOfUser:(DbyUserModel *)user;
- (void)audioChangeOfUser:(DbyUserModel *)user;

@end




@interface LivePlayHelper : NSObject

@property (nonatomic, weak) id<LivePlayHelperDelegate>delegate;


- (instancetype)initWithDelegate:(id<LivePlayHelperDelegate>)delegate;

- (DbyUserModel *)modelWithUid:(NSString *)uid;

- (void)joinWithUid:(NSString *)uid isLocal:(BOOL)isLocal;
- (void)LeaveWithUid:(NSString *)uid;
- (void)videoChangeOfUid:(NSString *)uid enabled:(BOOL)enabled;
- (void)audioChangeOfUid:(NSString *)uid enabled:(BOOL)enabled;

@end

NS_ASSUME_NONNULL_END
