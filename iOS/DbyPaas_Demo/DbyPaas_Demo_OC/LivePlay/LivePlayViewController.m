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
#import "LivePlayHelper.h"
@interface LivePlayViewController ()<DbyEngineDelegate, LivePlayHelperDelegate>
@property (weak, nonatomic) IBOutlet UIView *bigView;
@property (nonatomic, strong) DbyEngine *dbyEngine;
@property (weak, nonatomic) IBOutlet UIView *smallView;
@property (weak, nonatomic) IBOutlet UIButton *camButton;
@property (weak, nonatomic) IBOutlet UIButton *micButton;
@property (nonatomic, strong) LivePlayHelper *livePlayHelper;


@property (nonatomic, assign) BOOL micOn;
@property (nonatomic, assign) BOOL camOn;

@end

@implementation LivePlayViewController

- (void)viewDidLoad {
    [super viewDidLoad];
    
    
    
    
    [self startEngine];
    
    // Do any additional setup after loading the view.
}


#pragma mark -
- (IBAction)onClickCamButton:(id)sender {
    
    BOOL nextState = !self.camOn;
    [self.dbyEngine enableLocalVideo:nextState];
}

- (IBAction)onClickMicButton:(id)sender {
    BOOL nextState = !self.micOn;
    [self.dbyEngine enableLocalAudio:nextState];
}

- (IBAction)onClickSwitchCamButton:(id)sender {
    [self.dbyEngine switchCamera];
}

- (IBAction)onClickQuitButton:(id)sender {
    [self.dbyEngine leaveChannel:nil];
}

#pragma mark -
- (void)startEngine
{
    int status = [self.dbyEngine joinChannelById:self.channelId userId:self.uid completeHandler:nil];
    if (status == 0) {
        DbyVideoEncoderConfiguration *config = [[DbyVideoEncoderConfiguration alloc] initWithSize:DbyVideoDimension640x360 frameRate:DbyVideoFrameRateFps15 orientationMode:DbyVideoOutputOrientationModeAdaptative];
        [self.dbyEngine setVideoEncoderConfiguration:config];
        
        [self.dbyEngine enableDualStreamMode:YES];
        [self.dbyEngine enableLocalAudio:YES];
        [self.dbyEngine enableLocalVideo:YES];
        
        //设置本地预览
        DbyVideoCanvas *canvas = [DbyVideoCanvas canvasWithView:self.bigView uid:self.uid];
        [self.dbyEngine setupLocalVideo:canvas];
        
    } else {
        //验证不通过。 1. 输入不合法 2. 重复进入频道
        
    }
}

- (void)stopEngine
{
    [self.dbyEngine leaveChannel:nil];
}

#pragma mark -

- (void)didJoinOfUser:(DbyUserModel *)user
{
    
    NSLog(@"didJoinOfUser, %@",user);
    
    DbyVideoCanvas *canvas = [DbyVideoCanvas canvasWithView:user.videoView uid:user.uid];
    if (user.isLocal) {
        [self.dbyEngine setupLocalVideo:canvas];
    } else {
        [self.dbyEngine setupRemoteVideo:canvas];
    }
    
    if (user.isLarge) {
        user.videoContainerView = self.bigView;
    } else {
        user.videoContainerView = self.smallView;
    }
    
}

- (void)didLeaveOfUser:(DbyUserModel *)user
{
    NSLog(@"didLeaveOfUser, %@",user);
}


- (void)audioChangeOfUser:(DbyUserModel *)user
{
    NSLog(@"audioChangeOfUser, %@",user);
}

- (void)videoChangeOfUser:(DbyUserModel *)user
{
    
    NSLog(@"videoChangeOfUser, %@",user);
    
    if (user.isLarge) {
        user.videoContainerView = self.bigView;
    } else {
        user.videoContainerView = self.smallView;
    }
    
    if (user.isLarge && !user.isLocal && user.camOn) {
        [self.dbyEngine setRemoteVideoStream:user.uid type:DbyVideoStreamTypeHigh];
    }
}

#pragma mark -
- (void)dbyEngine:(DbyEngine *)engine didJoinChannel:(NSString *)channel withUid:(NSString *)uid
{
    [self.livePlayHelper joinWithUid:uid isLocal:YES];
}

- (void)dbyEngine:(DbyEngine *)engine joinWithError:(NSInteger)errorCode
{
    
}

- (void)dbyEngine:(DbyEngine *)engine didLeaveChannel:(NSString *)channel withUid:(NSString *)uid
{
    [self.livePlayHelper LeaveWithUid:uid];
    [DbyEngine destroy];
    [self.navigationController popViewControllerAnimated:YES];
}

- (void)dbyEngineDidKickedOff:(DbyEngine *)engine
{
    
}

- (void)dbyEngine:(DbyEngine *)engine didReceiveSDKConnectError:(NSInteger)errorCode
{
    [self.dbyEngine leaveChannel:nil];
}

- (void)dbyEngine:(DbyEngine *)engine didJoinedOfUid:(NSString *)uid
{
    [self.livePlayHelper joinWithUid:uid isLocal:NO];
}
- (void)dbyEngine:(DbyEngine *)engine didOfflineOfUid:(NSString *)uid
{
    [self.livePlayHelper LeaveWithUid:uid];
}

- (void)dbyEngine:(DbyEngine *)engine localAudioStateChange:(DbyAudioLocalState)state error:(DbyAudioLocalError)error
{
    if (state == DbyAudioLocalStateRecording) {
        self.micOn = YES;
    } else {
        self.micOn = NO;
    }
}

- (void)dbyEngine:(DbyEngine *)engine localVideoStateChange:(DbyLocalVideoStreamState)state error:(DbyLocalVideoStreamError)error
{
    if (state == DbyLocalVideoStreamStateCapturing) {
        self.camOn = YES;
    } else {
        self.camOn = NO;
    }
}

- (void)dbyEngine:(DbyEngine *)engine remoteAudioStateChangedOfUid:(NSString *)uid state:(BOOL)enabled
{
    [self.livePlayHelper audioChangeOfUid:uid enabled:enabled];
}

- (void)dbyEngine:(DbyEngine *)engine remoteVideoStateChangedOfUid:(NSString *)uid device:(NSString *)device state:(BOOL)enabled
{
    [self.livePlayHelper videoChangeOfUid:uid enabled:enabled];
}
- (void)dbyEngine:(DbyEngine *)engine firstRemoteVideoDecodedOfUid:(NSString *)uid device:(NSString *)deviceId size:(CGSize)size
{
    
}

#pragma mark -
- (void)setCamOn:(BOOL)camOn
{
    _camOn = camOn;
    self.camButton.selected = !camOn;
    [self.livePlayHelper videoChangeOfUid:self.uid enabled:camOn];
}

- (void)setMicOn:(BOOL)micOn
{
    _micOn = micOn;
    self.micButton.selected = !micOn;
    [self.livePlayHelper audioChangeOfUid:self.uid enabled:micOn];
}


#pragma mark -
- (DbyEngine *)dbyEngine
{
    if (!_dbyEngine) {
        _dbyEngine = [DbyEngine sharedEngineWithAppId:AppId appkey:AppKey delegate:self];
    }
    return _dbyEngine;
}
- (LivePlayHelper *)livePlayHelper
{
    if (!_livePlayHelper) {
        _livePlayHelper = [[LivePlayHelper alloc] initWithDelegate:self];
    }
    return _livePlayHelper;
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
