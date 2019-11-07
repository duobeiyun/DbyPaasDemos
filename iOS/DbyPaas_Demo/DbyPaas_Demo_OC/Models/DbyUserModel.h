//
//  DbyUserModel.h
//  DbyPaas_Demo_OC
//
//  Created by yxibng on 2019/11/7.
//  Copyright © 2019 yxibng. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>

NS_ASSUME_NONNULL_BEGIN

@interface DbyUserModel : NSObject

@property (nonatomic, copy) NSString *uid;
@property (nonatomic, assign) BOOL isLocal;

@property (nonatomic, assign) BOOL camOn;
@property (nonatomic, assign) BOOL micOn;

@property (nonatomic, strong, readonly) UIView *videoView;

@property (nonatomic, weak) UIView *videoContainerView;

@property (nonatomic, assign) BOOL isLarge;
 
- (instancetype)initWithUid:(NSString *)uid isLocal:(BOOL)isLocal;

@end

NS_ASSUME_NONNULL_END
