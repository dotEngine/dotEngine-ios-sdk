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
@property (nonatomic,readonly) NSString* _Nonnull streamId;
@property (nonatomic,readonly) NSString* _Nullable peerId;
@property (nonatomic,readonly) DotView* _Nullable view;

@property (nonatomic,weak) id<DotStreamDelegate> _Nullable delegate;
@property (nonatomic,retain) id<DotVideoCapturer> _Nullable videoCapturer;


-(instancetype)initWithAudio:(BOOL)audio video:(BOOL)video;

-(void)setupVideoProfile:(DotEngineVideoProfile)profile;

-(void)setupLocalMedia;

-(void)shutdownLocalMedia;

-(void)switchCamera;

-(void)muteAudio:(BOOL)muted;

-(void)muteVideo:(BOOL)muted;


@end



