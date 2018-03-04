//
//  ViewController.m
//  dotEngine-demo
//
//  Created by xiang on 04/03/2018.
//  Copyright © 2018 dotEngine. All rights reserved.
//

#import "ViewController.h"

#import <UIView+Toast.h>

#import <libdotEngine/DotEngine.h>
#import <libdotEngine/DotStream.h>
#import <libdotEngine/DotView.h>
#import <libdotEngine/DotVideoCapturer.h>

#include <stdlib.h>


static  NSString*   APP_SECRET = @"dotEngine_secret";
static  NSString*   ROOM = @"default";
static NSString*    TOKEN_URL = @"https://dotengine2.dot.cc/api/generateToken";

static  BOOL    USE_CUSTOM_CAPTURE_MODE = false;

@interface ViewController () <DotEngineDelegate,DotStreamDelegate>
{
    
    BOOL connected;
    NSString* _token;
}


@property (weak, nonatomic) IBOutlet UIButton *joinButton;

@property (nonatomic,strong) DotEngine *dotEngine;
@property (nonatomic,strong) DotStream *localStream;
@property (nonatomic,strong) UIView *localVideoView;
@property (nonatomic,strong) NSMutableDictionary *remoteVideoViews;
@property (nonatomic,strong) NSString *userId;
@property (nonatomic,strong) NSString *room;

@end

@implementation ViewController

- (void)viewDidLoad {
    [super viewDidLoad];
    // Do any additional setup after loading the view, typically from a nib.
    
    _dotEngine = [DotEngine sharedInstanceWithDelegate:self];
    
    _localStream = [[DotStream alloc] initWithAudio:YES video:YES];
    
    [_localStream setDelegate:self];
    
    [_localStream setupVideoProfile:DotEngine_VideoProfile_240P];
    
    // this may block a while
    [_localStream setupLocalMedia];  // for video preview
    
    
    self.localVideoView = _localStream.view;
    
    [self.view addSubview:self.localVideoView];
    
    _remoteVideoViews = [NSMutableDictionary dictionary];
    
    connected = FALSE;
    uint32_t randomNum = arc4random_uniform(10000);
    
    self.userId = [NSString stringWithFormat:@"ios%d",randomNum];
    self.room = ROOM;
    
    [UIApplication sharedApplication].idleTimerDisabled = YES;
    
    
    [self.view setNeedsLayout];
}


- (void)didReceiveMemoryWarning {
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}


-(BOOL)prefersStatusBarHidden{
    return NO;
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


- (IBAction)joinRoom:(id)sender {

    if (!connected) {
        
        [sender setTitle:@"joining" forState:UIControlStateNormal];
        
        [self getDotEngineTokenWithRoom:self.room andUserId:self.userId];
        
    }  else {
        [self startDisconnect];
    }
    
    self.joinButton.enabled = false;
}



-(void)startDisconnect
{
    
    [self.dotEngine leaveRoom];
    
    [self.remoteVideoViews removeAllObjects];
    
    [self.joinButton setTitle:@"leave" forState:UIControlStateNormal];
    
    self.joinButton.enabled = NO;
    
}



-(void)getDotEngineTokenWithRoom:(NSString*)room andUserId:(NSString*)userId
{
    
    
    [self.dotEngine generateTestToken:TOKEN_URL
                            appsecret:APP_SECRET
                                 room:room
                               userId:userId
                            withBlock:^(NSString *token, NSError *error) {
                                      if (error) {
                                          
                                          [self.joinButton setTitle:@"join" forState:UIControlStateNormal];
                                          
                                          self.joinButton.enabled = TRUE;
                                          
                                          [self.view makeToast:@"can not get token"];
                                          
                                          return;
                                      }
                                
                                      _token = token;
                                      [self.dotEngine joinRoomWithToken:_token];
                                
                            }];
}


#pragma delegate

-(void)dotEngine:(DotEngine *)engine didJoined:(NSString *)userId
{
    
    NSLog(@"didJoined %@",userId);
    
    if ([self.userId isEqualToString:userId]) {
        
        connected = true;
        
        [self.joinButton setTitle:@"leave" forState:UIControlStateNormal];
        
        self.joinButton.enabled = true;
        
        [self.dotEngine addStream:_localStream];
        
        
        
    }
    
    
    
}


-(void)dotEngine:(DotEngine *)engine didLeave:(NSString *)userId
{
    
    NSLog(@"didLeave %@",userId);
    
    
    if ([self.userId isEqualToString:userId]) {
        
        [self.joinButton setTitle:@"join" forState:UIControlStateNormal];
        
        self.joinButton.enabled = true;
        
        connected = false;
        
        
    }
    
}


-(void)dotEngine:(DotEngine* _Nonnull) engine  stateChange:(DotStatus)state
{
    
    if (state == DotStatusConnected) {
        NSLog(@"DotStatusConnected");
        
        connected = true;
        
        [self.joinButton setTitle:@"leave" forState:UIControlStateNormal];
        
        self.joinButton.enabled = true;
        
    }
    
    if (state == DotStatusConnecting) {
        NSLog(@"DotStatusConnecting");
    }
    
    
    if (state == DotStatusDisConnected) {
        NSLog(@"DotStatusDisConnected");
    }
    
    
}




-(void)dotEngine:(DotEngine* _Nonnull) engine didAddRemoteStream:(DotStream* _Nonnull)stream
{
    
    [stream setDelegate:self];
    
    UIView* view = stream.view;
    
    [self.view addSubview:view];
    
    [self.remoteVideoViews setObject:view forKey:stream.streamId];
    
    [self.view setNeedsLayout];
    
}




-(void)dotEngine:(DotEngine* _Nonnull) engine didRemoveRemoteStream:(DotStream* _Nonnull) stream
{
    
    if ([self.remoteVideoViews objectForKey:stream.streamId]) {
        
        UIView* view = stream.view;
        
        [view removeFromSuperview];
        
        [self.remoteVideoViews removeObjectForKey:stream.streamId];
        
        [self.view setNeedsLayout];
        
    }
}


-(void)dotEngine:(DotEngine *)engine didOccurError:(DotEngineErrorCode)errorCode
{
    
    NSLog(@"didOccurError");
    [self.view makeToast:@"didOccurError"];
    
}

- (void)dotEngine:(DotEngine * _Nonnull)engine didAddLocalStream:(DotStream * _Nonnull)stream {
    
    NSLog(@"didAddLocalStream");
    
}


- (void)dotEngine:(DotEngine * _Nonnull)engine didRemoveLocalStream:(DotStream * _Nonnull)stream {
    
    NSLog(@"didRemoveLocalStream");
}



#pragma DotStream delegate


-(void)stream:(DotStream* _Nullable)stream  didMutedVideo:(BOOL)muted
{
    
    [self.view makeToast:@"stream video muted"];
    
}

-(void)stream:(DotStream* _Nullable)stream  didMutedAudio:(BOOL)muted
{
    
    [self.view makeToast:@"stream audio muted"];
}

-(void)stream:(DotStream* _Nullable)stream  didGotAudioLevel:(int)audioLevel
{
    
    NSLog(@"stream  %@  %d", stream.streamId, audioLevel);
}





@end
