//
//  ViewController.m
//  dotEngine-ios-exmaple
//
//  Created by xiang on 8/23/16.
//  Copyright © 2016 dotEngine. All rights reserved.
//

#import "ViewController.h"
#import <AFNetworking.h>
#import <UIView+Toast.h>


#import <DOTEngine.h>

#include <stdlib.h>




@import AVFoundation;

@interface ViewController ()<DotEngineDelegate>

{
    
    BOOL connected;
    NSString* _token;
    
    BOOL    speekerPhoneMode;
    BOOL    videoMode;
    
}


@property (nonatomic,strong) UILabel *roomInfoLabel;
@property (nonatomic,strong) DotEngine *dotEngine;
@property (nonatomic,strong) UIView *localVideoView;
@property (nonatomic,strong) NSMutableDictionary *remoteVideoViews;
@property (nonatomic,strong) NSString *userId;
@property (nonatomic,strong) NSString *room;
@property (weak, nonatomic) IBOutlet UIButton *joinButton;
@property (weak, nonatomic) IBOutlet UIButton *enableVideoButton;
@property (weak, nonatomic) IBOutlet UIButton *muteAudioButton;
@property (weak, nonatomic) IBOutlet UIButton *speekerModeButton;
@property (weak, nonatomic) IBOutlet UIButton *previewButton;
@property (weak, nonatomic) IBOutlet UIButton *cameraSwitchButton;

@end


static  NSString*  APP_KEY = @"45";
static  NSString*  APP_SECRET = @"dc5cabddba054ffe894ba79c2910866c";
static  NSString*  ROOM = @"dotcc";


@implementation ViewController

- (void)viewDidLoad {
    [super viewDidLoad];
    // Do any additional setup after loading the view, typically from a nib.
    
    
    self.dotEngine = [DotEngine sharedInstanceWithDelegate:self];
    
    _remoteVideoViews = [NSMutableDictionary dictionary];
    
    [self.dotEngine setupVideoProfile:DotEngine_VideoProfile_360P];
    
    connected = FALSE;
    
    speekerPhoneMode = YES;
    
    videoMode = YES;
    
    int randomNum = arc4random()  % 1000;
    
    self.userId = [NSString stringWithFormat:@"%@%d",[UIDevice currentDevice].name,randomNum];
    self.room = ROOM;
    
    [UIApplication sharedApplication].idleTimerDisabled = YES;
    
    [self hideMenuButtons];
    
}



-(BOOL)prefersStatusBarHidden{
    
    return YES;
}


-(void)showMenuButtons{
    
    
    self.muteAudioButton.hidden = NO;
    self.speekerModeButton.hidden = NO;
    self.previewButton.hidden = NO;
    self.cameraSwitchButton.hidden = NO;
    
}


-(void)hideMenuButtons{
    
    
    self.muteAudioButton.hidden = YES;
    self.speekerModeButton.hidden = YES;
    self.previewButton.hidden = YES;
    self.cameraSwitchButton.hidden = YES;
    
    
}



-(void)viewWillLayoutSubviews
{
    
    [super viewWillLayoutSubviews];
    
    [self layoutVideoViews];
    
}



-(void)layoutVideoViews
{
    
    NSMutableArray *videoViews = [NSMutableArray array];
    
    if (self.localVideoView) {
        
        [videoViews addObject:self.localVideoView];
    }
    
    [videoViews addObjectsFromArray:[self.remoteVideoViews allValues]];
    
    for (int i=0; i < [videoViews count]; i++) {
        
        CGRect frame = [self frameAtPosition:i];
        
        ((UIView*)videoViews[i]).frame = frame;
        ((UIView*)videoViews[i]).contentMode =  UIViewContentModeScaleAspectFill;
        
    }
}


-(CGRect)frameAtPosition:(int)postion
{
    
    
    CGRect bounds = self.view.bounds;
    
    CGFloat width = bounds.size.width / 2;
    CGFloat height = bounds.size.width / 2;
    
    CGFloat x = (postion%2) * width;
    CGFloat y = (postion/2) * height;
    
    CGRect frame = CGRectMake(x, y, width, height);
    return frame;
}



- (IBAction)connectPress:(id)sender {
    
    if (!connected) {
        
        [sender setTitle:@"加入中" forState:UIControlStateNormal];
        
        [self getDotEngineTokenWithRoom:self.room andUserId:self.userId];
        
    } else {
        
        [self startDisconnect];
    }
    
    self.joinButton.enabled = false;
    
    
}



-(void)startDisconnect
{
    
    [self.dotEngine leaveRoom];
    
    
    [self.dotEngine stopLocalMedia];
    
    [self.joinButton setTitle:@"离开" forState:UIControlStateNormal];
    
    self.joinButton.enabled = NO;
    
    
    
}


- (void)connectWithPermission
{
    [AVCaptureDevice requestAccessForMediaType:AVMediaTypeAudio completionHandler:^(BOOL audioGranted) {
        [AVCaptureDevice requestAccessForMediaType:AVMediaTypeVideo completionHandler:^(BOOL videoGranted) {
            dispatch_async(dispatch_get_main_queue(), ^{
                if (audioGranted && videoGranted) {
                    [self connectToRoom];
                }
                else {
                    
                    [self.view makeToast:@"无法得到音视频权限"];
                    
                }
            });
        }];
    }];
}


-(void)connectToRoom{
    
    [self.dotEngine startLocalMedia];
    
    [self.dotEngine joinRoomWithToken:_token];
    
    
    
}





-(void)getDotEngineTokenWithRoom:(NSString*)room andUserId:(NSString*)userId
{
    
    
    [self.dotEngine generateTestTokenWithAppKey:APP_KEY
                                      appsecret:APP_SECRET
                                           room:room
                                         userId:userId
                                      withBlock:^(NSString *token, NSError *error) {
                                          
                                          if (error) {
                                              [self.joinButton setTitle:@"加入" forState:UIControlStateNormal];
                                              self.joinButton.enabled = TRUE;
                                              [self.view makeToast:@"can not get token"];
                                              
                                          } else {
                                              
                                              _token = token;
                                              
                                              [self connectWithPermission];
                                              
                                          }
                                          
                                      }];
    
}






// 开启/关闭静音 只mute 自己
- (IBAction)muteAudio:(id)sender {
    
    BOOL muted = [self.dotEngine isAudioEnable:self.userId];
    [self.dotEngine enableAudio:!muted];
    muted = [self.dotEngine isAudioEnable:self.userId];
    [self.view makeToast:[NSString stringWithFormat:@"mute auido %d",muted]];
    
}



// 扬声器模式

- (IBAction)speekerMode:(id)sender {
    
    speekerPhoneMode = !speekerPhoneMode;
    
    [self.dotEngine enableSpeakerphone:speekerPhoneMode];
    
    [self.view makeToast:[NSString stringWithFormat:@"speekerMode  %d",speekerPhoneMode]];
}


// 视频mute/unmute 开关  先只mute 自己
- (IBAction)preview:(id)sender {
    
    BOOL muted = [self.dotEngine isVideoEnable:self.userId];
    
    
    [self.dotEngine  enableVideo:!muted];
    
    
    muted = [self.dotEngine isVideoEnable:self.userId];
    
    [self.view makeToast:[NSString stringWithFormat:@"muted video"]];
    
    
    
}


// 前后摄像头切换
- (IBAction)cemaraSwitch:(id)sender {
    
    [self.dotEngine switchCamera];
    
}



// 音视频模式切换

- (IBAction)enableVideo:(id)sender {
    
    videoMode = !videoMode;
    
    //TODO
    
}






#pragma delegate




-(void)dotEngine:(DotEngine *)engine didJoined:(NSString *)userId
{
    
    NSLog(@"didJoined %@",userId);
    
    if ([self.userId isEqualToString:userId]) {
        
        connected = true;
        
        [self.joinButton setTitle:@"离开" forState:UIControlStateNormal];
        
        self.joinButton.enabled = true;
        
        [self showMenuButtons];
        
    }
    
    
    
}


-(void)dotEngine:(DotEngine *)engine didLeave:(NSString *)userId
{
    
    NSLog(@"didLeave %@",userId);
    
    
    if ([self.userId isEqualToString:userId]) {
        
        [self.joinButton setTitle:@"加入" forState:UIControlStateNormal];
        
        self.joinButton.enabled = true;
        
        connected = false;
        
        
        [self hideMenuButtons];
        
    }
    
}




/**
 <#Description#>
 
 @param engine <#engine description#>
 @param view   <#view description#>
 */
-(void)dotEngine:(DotEngine*) engine  didAddLocalView:(UIView*)view
{
    
    
    connected = true;
    
    [self.joinButton setTitle:@"离开" forState:UIControlStateNormal];
    
    self.joinButton.enabled = true;
    
    
    [self showMenuButtons];
    
    
    _localVideoView = view;
    
    if (_localVideoView != nil ) {
        
        [self.view addSubview:_localVideoView];
    }
    
}



/**
 <#Description#>
 
 @param engine <#engine description#>
 @param view   <#view description#>
 @param userId <#userId description#>
 */
-(void)dotEngine:(DotEngine*) engine  didAddRemoteView:(UIView*)view withUser:(NSString*)userId
{
    
    [self.view addSubview:view];
    
    [self.remoteVideoViews setObject:view forKey:userId];
    
    [self.view setNeedsLayout];
    
}


/**
 <#Description#>
 
 @param engine <#engine description#>
 @param view   <#view description#>
 */
-(void)dotEngine:(DotEngine*) engine didRemoveLocalView:(UIView*)view
{
    
    if (self.localVideoView) {
        
        [self.localVideoView removeFromSuperview];
        
        self.localVideoView = nil;
    }
    
}



/**
 <#Description#>
 
 @param engine <#engine description#>
 @param view   <#view description#>
 @param userId <#userId description#>
 */
-(void)dotEngine:(DotEngine*) engine didRemoveRemoteView:(UIView*)view withUser:(NSString*)userId
{
    
    if ([self.remoteVideoViews objectForKey:userId]) {
        
        [view removeFromSuperview];
        
        [self.remoteVideoViews removeObjectForKey:userId];
        
        [self.view setNeedsLayout];
        
    }
}






-(void)dotEngine:(DotEngine *)engine didEnableVideo:(BOOL)enable userId:(NSString *)userId
{
    
    NSLog(@"didEnableVideo");
    
    NSString* content;
    
    if ([userId isEqualToString:self.userId]) {
        content = [NSString stringWithFormat:@"本地用户 %@  视频发生改变  %@",userId, enable? @"开启" : @"关闭"];
    } else {
        content = [NSString stringWithFormat:@"远程用户 %@ 视频发生改变 %@",userId, enable ? @"开启" : @"关闭"];
    }
    
    [self.view makeToast:content];
    
    // 可以进行其他更多的操作  比如替换试图等操作
}



-(void)dotEngine:(DotEngine *)engine didEnableAudio:(BOOL)enable userId:(NSString *)userId
{
    
    NSLog(@"didEnableAudio");
    
    NSString* content;
    
    if ([userId isEqualToString:self.userId]) {
        content = [NSString stringWithFormat:@"本地用户 %@  音频发生改变  %@",userId, enable? @"开启" : @"关闭"];
    } else {
        content = [NSString stringWithFormat:@"远程用户 %@  音频发生改变 %@",userId, enable ? @"开启" : @"关闭"];
    }
    
    [self.view makeToast:content];
    
    
    // 可以进行其他更多的操作  比如修改是否显示音频的状态
    
}



-(void)dotEngine:(DotEngine *)engine didOccurError:(DotEngineErrorCode)errorCode
{
    
    NSLog(@"didOccurError");
    
    
    [self.view makeToast:@"didOccurError"];
    
    
}




@end






































