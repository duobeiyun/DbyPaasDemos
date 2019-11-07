//
//  DbyUserModel.m
//  DbyPaas_Demo_OC
//
//  Created by yxibng on 2019/11/7.
//  Copyright © 2019 yxibng. All rights reserved.
//

#import "DbyUserModel.h"

@implementation DbyUserModel

- (NSString *)description
{
    NSString *str = [NSString stringWithFormat:@"uid = %@, isLocal = %d, camOn = %d, micOn = %d, videoView = %@,videoContainerView = %@, isLarge = %d", self.uid,
                     self.isLocal,
                     self.camOn,
                     self.micOn,
                     self.videoView,
                     self.videoContainerView,
                     self.isLarge];
    return str;
}

- (instancetype)initWithUid:(NSString *)uid isLocal:(BOOL)isLocal
{
    if (self = [super init]) {
        _uid = uid;
        _isLocal = isLocal;
        _videoView = [UIView new];
    }
    return self;
}

- (void)setVideoContainerView:(UIView *)videoContainerView
{
    _videoContainerView = videoContainerView;
    [_videoView removeFromSuperview];
    if (videoContainerView) {
        [videoContainerView addSubview:_videoView];
        _videoView.frame = videoContainerView.bounds;
        _videoView.autoresizingMask = UIViewAutoresizingFlexibleWidth | UIViewAutoresizingFlexibleHeight;
    }
}

@end
