//
//  DotStream.h
//  dot-engine-ios
//
//  Created by xiang on 30/04/2017.
//  Copyright © 2017 dotEngine. All rights reserved.
//

//#import <Foundation/Foundation.h>

#import "DotView.h"
#import "DotEngine.h"
#import "DotVideoCapturer.h"

@class DotStream;

@protocol DotStreamDelegate <NSObject>

-(void)stream:(DotStream* _Nullable)stream  didMutedVideo:(BOOL)muted;

-(void)stream:(DotStream* _Nullable)stream  didMutedAudio:(BOOL)muted;

-(void)stream:(DotStream* _Nullable)stream  didGotAudioLevel:(int)audioLevel;


@end




@interface DotStream : NSObject

@property (nonatomic,readonly) BOOL local;
@property (nonatomic,readonly) BOOL audio;
@property (nonatomic,readonly) BOOL video;
@property (nonatomic,readonly) NSString* streamId;
@property (nonatomic,readonly) NSString* peerId;
@property (nonatomic,readonly) DotView*  view;

@property (nonatomic,weak) id<DotStreamDelegate> delegate;
@property (nonatomic,retain) id<DotVideoCapturer> _Nullable videoCapturer;


-(instancetype)initWithAudio:(BOOL)audio video:(BOOL)video;

-(void)setupVideoProfile:(DotEngineVideoProfile)profile;

-(void)switchCamera;

-(void)muteAudio:(BOOL)muted;

-(void)muteVideo:(BOOL)muted;


@end



