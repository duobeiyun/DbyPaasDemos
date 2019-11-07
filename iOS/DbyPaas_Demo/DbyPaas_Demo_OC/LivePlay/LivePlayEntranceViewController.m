//
//  LivePlayEntranceViewController.m
//  DbyPaas_Demo_OC
//
//  Created by 姚晓丙 on 2019/11/6.
//  Copyright © 2019 yxibng. All rights reserved.
//

#import "LivePlayEntranceViewController.h"
#import "LivePlayViewController.h"
#import <DbyPaas_iOS/DbyPaas_iOS.h>


static NSString *kUid = @"dby_uid";
static NSString *kChannelId = @"dby_channelId";

@interface LivePlayEntranceViewController ()

@property (weak, nonatomic) IBOutlet UITextField *channelIdTextField;
@property (weak, nonatomic) IBOutlet UITextField *uidTextField;

@end

@implementation LivePlayEntranceViewController

- (void)viewDidLoad {
    [super viewDidLoad];
    
    NSUserDefaults *defauts = [NSUserDefaults standardUserDefaults];
    self.channelIdTextField.text = [defauts objectForKey:kChannelId];
    self.uidTextField.text = [defauts objectForKey:kUid];
    
    
}



- (IBAction)enter:(id)sender {
    
    NSString *channelId = self.channelIdTextField.text;
    NSString *uid = self.uidTextField.text;
    
    if ([self isValidUid:uid] && [self isValidChannelId:channelId]) {
        
        NSUserDefaults *defauts = [NSUserDefaults standardUserDefaults];
        [defauts setObject:channelId forKey:kChannelId];
        [defauts setObject:uid forKey:kUid];
        [defauts synchronize];
        
        
        UIStoryboard * sb = [UIStoryboard storyboardWithName:@"Main" bundle:nil];
        LivePlayViewController *vc = [sb instantiateViewControllerWithIdentifier:@"LivePlayViewController"];
        vc.channelId = channelId;
        vc.uid = uid;
        [self.navigationController pushViewController:vc animated:YES];

    } else {
        
        //输入不合法
        
        
    }
    
    
}


- (BOOL)isValidChannelId:(NSString *)channelId
{
    if (!channelId) {
        return NO;
    }

    NSError *error;
    NSRegularExpression *regex = [NSRegularExpression regularExpressionWithPattern:DbyChannelIdRegex options:NSRegularExpressionCaseInsensitive error:&error];
    if (error) {
        return NO;
    }
    NSArray<NSTextCheckingResult *> *results = [regex matchesInString:channelId options:NSMatchingReportCompletion range:NSMakeRange(0, channelId.length)];
    if (results.count > 0) {
        return YES;
    }
    return NO;
}

- (BOOL)isValidUid:(NSString *)uid
{
    if (!uid) {
        return NO;
    }

    NSError *error;
    NSRegularExpression *regex = [NSRegularExpression regularExpressionWithPattern:DbyUidRegex options:NSRegularExpressionCaseInsensitive error:&error];
    if (error) {
        return NO;
    }
    NSArray<NSTextCheckingResult *> *results = [regex matchesInString:uid options:NSMatchingReportCompletion range:NSMakeRange(0, uid.length)];
    if (results.count > 0) {
        return YES;
    }
    return NO;
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
