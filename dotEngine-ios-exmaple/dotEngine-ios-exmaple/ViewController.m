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

#import <GPUImage.h>
#import <DotGPUImageBeautyFilter.h>

#import <DOTEngine.h>

#include <stdlib.h>




@import AVFoundation;

@interface ViewController ()<DotEngineDelegate>

{
    
    BOOL connected;
    NSString* _token;
    
    BOOL    speekerPhoneMode;
    BOOL    videoMode;
    
    
    GPUImageView  *cameraView;
    GPUImageVideoCamera *videoCamera;
    
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

static  BOOL    USE_CUSTOM_CAPTURE_MODE = true;


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
    
    if (USE_CUSTOM_CAPTURE_MODE) { // 这里需要注意
        
        [self.dotEngine setCaptureMode:DotEngine_Capture_Custom_Video];
    }
    
}



-(BOOL)prefersStatusBarHidden{
    
    return NO;
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


-(void) startLocalMedia
{
    
    if(!USE_CUSTOM_CAPTURE_MODE){
        
        [self.dotEngine startLocalMedia];
        
    } else {
        
        
        videoCamera = [[GPUImageVideoCamera alloc] initWithSessionPreset:AVCaptureSessionPreset640x480 cameraPosition:AVCaptureDevicePositionFront];
        videoCamera.outputImageOrientation = UIInterfaceOrientationPortrait;
        
        videoCamera.horizontallyMirrorFrontFacingCamera = YES;
        
        videoCamera.frameRate = 15;
        
        DotGPUImageBeautyFilter *filter = [[DotGPUImageBeautyFilter alloc] init];
        
        filter.toneLevel = 0.5;  // 红润
        filter.beautyLevel = 0.5; // 美颜
        filter.brightLevel = 0.5;  // 亮度
        
        float halfWidth = self.view.frame.size.width / 2;
        
        cameraView = [[GPUImageView alloc] initWithFrame:CGRectMake(0, halfWidth, halfWidth, halfWidth)];
        
        GPUImageCropFilter *cropfilter = [[GPUImageCropFilter alloc] initWithCropRegion:CGRectMake(0, 0, 1, 0.75)];

        int outWidth = 320;
        int outHeight = 320;
        
        GPUImageRawDataOutput *outfilter = [[GPUImageRawDataOutput alloc] initWithImageSize:CGSizeMake(outWidth, outHeight) resultsInBGRAFormat:YES];
        
        [videoCamera addTarget:cropfilter];
        [cropfilter addTarget:filter];
        
        [filter addTarget:cameraView];
        
        [filter addTarget:outfilter];
        
        __unsafe_unretained GPUImageRawDataOutput * weakOutput = outfilter;
        
        [outfilter setNewFrameAvailableBlock:^{
            
            [weakOutput lockFramebufferForReading];
            
            GLubyte *outputBytes = [weakOutput rawBytesForImage];
            
            CVPixelBufferRef pixelBufferCopy = NULL;
            
            CVPixelBufferCreate(kCFAllocatorDefault, outWidth, outHeight,
                                kCVPixelFormatType_32BGRA, NULL, &pixelBufferCopy);
            
            CVPixelBufferLockBaseAddress(pixelBufferCopy, 0);
            
            uint8_t*  baseAddress = CVPixelBufferGetBaseAddress(pixelBufferCopy);
            
            memcpy(baseAddress, outputBytes, outWidth * outHeight *4);
            
            CVPixelBufferUnlockBaseAddress(pixelBufferCopy, 0);
            
            [self.dotEngine sendPixelBuffer:pixelBufferCopy rotation:VideoRoation_0];
            
            [weakOutput unlockFramebufferAfterReading];
            
            CVPixelBufferRelease(pixelBufferCopy);
            
        }];
        
        [videoCamera startCameraCapture];
        
        [self.view addSubview:cameraView];
        
    }
    
}



-(void) stopLocalMedia
{
    if(!USE_CUSTOM_CAPTURE_MODE){
        
        [self.dotEngine stopLocalMedia];
        
    } else {
        
        [videoCamera stopCameraCapture];
        
        [cameraView removeFromSuperview];
    }
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
    
    [self stopLocalMedia];
    
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
    
    [self startLocalMedia];
    
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
    
    if (USE_CUSTOM_CAPTURE_MODE) {
        
        return;
    }
    
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






































