//
//  LivePlayViewController.m
//  DbyPaas_Demo_OC
//
//  Created by 姚晓丙 on 2019/11/6.
//  Copyright © 2019 yxibng. All rights reserved.
//

#import "LivePlayViewController.h"
#import <DbyPaas_iOS/DbyPaas_iOS.h>
#import "DbyConfig.h"
@interface LivePlayViewController ()<DbyEngineDelegate>
@property (nonatomic, strong) DbyEngine *dbyEngine;
@end

@implementation LivePlayViewController

- (void)viewDidLoad {
    [super viewDidLoad];

    
    // Do any additional setup after loading the view.
}

#pragma mark -

- (void)startEngine
{
    int status = [self.dbyEngine joinChannelById:@"" userId:@"" completeHandler:nil];
    if (status == 0) {
        
        
        
    } else {
        //验证不通过。 1. 输入不合法 2. 重复进入频道
        
    }
}

- (void)stopEngine
{
    
}


#pragma mark -
- (void)dbyEngine:(DbyEngine *)engine didJoinChannel:(NSString *)channel withUid:(NSString *)uid
{
    
}

- (void)dbyEngine:(DbyEngine *)engine joinWithError:(NSInteger)errorCode
{
    
}

- (void)dbyEngine:(DbyEngine *)engine didLeaveChannel:(NSString *)channel withUid:(NSString *)uid
{
    
}

- (void)dbyEngineDidKickedOff:(DbyEngine *)engine
{
    
}

- (void)dbyEngine:(DbyEngine *)engine didReceiveSDKConnectError:(NSInteger)errorCode
{
    
}

- (void)dbyEngine:(DbyEngine *)engine didJoinedOfUid:(NSString *)uid
{
    
}
- (void)dbyEngine:(DbyEngine *)engine didOfflineOfUid:(NSString *)uid
{
    
}

- (void)dbyEngine:(DbyEngine *)engine localAudioStateChange:(DbyAudioLocalState)state error:(DbyAudioLocalError)error
{
    
}

- (void)dbyEngine:(DbyEngine *)engine localVideoStateChange:(DbyLocalVideoStreamState)state error:(DbyLocalVideoStreamError)error
{
    
}

- (void)dbyEngine:(DbyEngine *)engine remoteAudioStateChangedOfUid:(NSString *)uid state:(BOOL)enabled
{
    
}

- (void)dbyEngine:(DbyEngine *)engine remoteVideoStateChangedOfUid:(NSString *)uid device:(NSString *)device state:(BOOL)enabled
{
    
}
- (void)dbyEngine:(DbyEngine *)engine firstRemoteVideoDecodedOfUid:(NSString *)uid device:(NSString *)deviceId size:(CGSize)size
{
    
}

- (DbyEngine *)dbyEngine
{
    if (!_dbyEngine) {
        _dbyEngine = [DbyEngine sharedEngineWithAppId:AppId appkey:AppKey delegate:self];
    }
    return _dbyEngine;
}

/*
#pragma mark - Navigation

// In a storyboard-based application, you will often want to do a little preparation before navigation
- (void)prepareForSegue:(UIStoryboardSegue *)segue sender:(id)sender {
    // Get the new view controller using [segue destinationViewController].
    // Pass the selected object to the new view controller.
}
*/

@end
