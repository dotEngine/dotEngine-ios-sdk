//
//  DotEngineKit.h
//  DotEngineKit
//
//  Created by xiang on 6/23/16.
//  Copyright © 2016 dotEngine. All rights reserved.
//

#import <Foundation/Foundation.h>

@import AVFoundation;

typedef NS_ENUM(NSInteger, DotEngineVideoProfile)
{
    
    DotEngine_VideoProfile_120P = 0,        // 160x120   15   80
    DotEngine_VideoProfile_120P_2 = 1,		// 120x160   15   80
    DotEngine_VideoProfile_120P_3 = 2,		// 120x120   15   60
    
    DotEngine_VideoProfile_180P = 10,		// 320x180   15   160
    DotEngine_VideoProfile_180P_2 = 11,		// 180x320   15   160
    DotEngine_VideoProfile_180P_3 = 12,		// 180x180   15   120
    
    DotEngine_VideoProfile_240P = 20,		// 320x240   15   200
    DotEngine_VideoProfile_240P_2 = 21,		// 240x320   15   200
    DotEngine_VideoProfile_240P_3 = 22,		// 240x240   15   160
    
    DotEngine_VideoProfile_360P = 30,		// 640x360   15   400
    DotEngine_VideoProfile_360P_2 = 31,		// 360x640   15   400
    DotEngine_VideoProfile_360P_3 = 32,		// 360x360   15   300
  
    DotEngine_VideoProfile_360P_4 = 33,     // 640x360   30   800
    DotEngine_VideoProfile_360P_5 = 34,     // 360x640   30   800
    DotEngine_VideoProfile_360P_6 = 35,     // 360x360   30   600
    
    DotEngine_VideoProfile_480P = 40,		// 640x480   15   500
    DotEngine_VideoProfile_480P_2 = 41,		// 480x640   15   500
    DotEngine_VideoProfile_480P_3 = 42,		// 480x480   15   400
    
    DotEngine_VideoProfile_480P_4 = 43,		// 640x480   30   750
    DotEngine_VideoProfile_480P_5 = 44,		// 480x640   30   750
    DotEngine_VideoProfile_480P_6 = 45,		// 480x480   30   680
    DotEngine_VideoProfile_480P_7 = 46,		// 640x480   15   1000
    
    DotEngine_VideoProfile_720P = 50,		// 1280x720  15   1000
    DotEngine_VideoProfile_720P_2 = 51,		// 720x1280  15   1000
    DotEngine_VideoProfile_720P_3 = 52,		// 1280x720  30   1700
    DotEngine_VideoProfile_720P_4 = 53,		// 720x1280  30   1700
    
};



typedef NS_ENUM(NSInteger, DotEngineRenderMode) {
    DotEngine_RenderResize = 0,
    DotEngine_RenderResizeAspect = 1,
    DotEngine_RenderResizeAspectFill = 2,
    
};



typedef NS_ENUM(NSInteger,DotEngineWarnCode){
    DotEngine_Warn_RoomIsFull   = 0,

};



typedef NS_ENUM(NSInteger, DotEngineErrorCode) {
    DotEngine_Error_NoError = 0,
    DotEngine_Error_Failed = 1,
    DotEngine_Error_InvalidArgument = 2,
    DotEngine_Error_NotReady = 3,
    DotEngine_Error_NotSupported = 4,
    DotEngine_Error_Refused = 5,
    DotEngine_Error_TokenExpire =  6,
    DotEngine_Error_TokenError = 7,
    DotEngine_Error_AccountDisableError = 8,
    DotEngine_Error_MediaPermissionRefused = 9,
    DotEngine_Error_ServerError = 10,
    DotEngine_Error_RoomFullError = 11,
    
};



typedef NS_ENUM(NSUInteger,DotEngineLogLevel){
    DotEngine_Log_Verbose,
    DotEngine_Log_Info,
    DotEngine_Log_Warning,
    DotEngine_Log_Error
};




typedef NS_ENUM(NSInteger,DotEngineCaptureMode){
    
    DotEngine_Capture_Default,
    DotEngine_Capture_Custom_Video,
    DotEngine_Capture_Custom_Video_Audio

};


typedef NS_ENUM(int, VideoRotation){
    
    VideoRoation_0 = 0,
    VideoRoation_90 = 90,
    VideoRoation_180 = 180,
    VideoRoation_270 = 270
    
};




@interface DotEngineAudioFrame : NSObject

@property(nonatomic, readonly ,nullable)  int16_t *data;
@property(nonatomic, readonly) size_t samples_per_channel;
@property(nonatomic, readonly) int sample_rate;
@property(nonatomic, readonly) size_t channels;
@property(nonatomic, readonly) int  bits_per_sample;

@end


@interface DotEngineVideoFrame : NSObject

/** width without rotation applied. */
@property(nonatomic, readonly) size_t width;

/** Height without rotation applied. */
@property(nonatomic, readonly) size_t height;

// These can return NULL if the object is not backed by a buffer.
@property(nonatomic, readonly, nullable) const uint8_t *yPlane;
@property(nonatomic, readonly, nullable) const uint8_t *uPlane;
@property(nonatomic, readonly, nullable) const uint8_t *vPlane;
@property(nonatomic, readonly) int32_t yPitch;
@property(nonatomic, readonly) int32_t uPitch;
@property(nonatomic, readonly) int32_t vPitch;

@property(nonatomic, readonly) int rotation;


@end








@protocol  DotEngineDelegate;


@interface DotEngine : NSObject


@property (nonatomic, weak) id<DotEngineDelegate> delegate;


@property (nonatomic, readonly) BOOL  isJoined;

@property (nonatomic, readonly) BOOL  isSetupLocalMedia;




/**
 instance with  delegate

 @param delegate <#delegate description#>

 @return <#return value description#>
 */
+ (instancetype _Nonnull)sharedInstanceWithDelegate:(id<DotEngineDelegate>)delegate;




/**
 
 return instance

 @return <#return value description#>
 */
+ (instancetype _Nonnull)sharedInstance;


/**
 
 start audio and video
 
 */
-(void)startLocalMedia;


/**
 start only audio
 */

//-(void)startLocalAudio;


/**
 start only video 
 */

//-(void)startLocalVideo;


/**
 stop local media
 */
-(void)stopLocalMedia;





/**
 *  join room  with  token
 *
 *  @param token got from server side,
 */
-(void)joinRoomWithToken:(NSString*)token;




/**
 *  leave room
 */
-(void)leaveRoom;




/**
 <#Description#>
 
 @param profile profile description
 */
-(void)setupVideoProfile:(DotEngineVideoProfile)profile;



/**
 <#Description#>
 
 @param captureMode captureMode description
 */
-(void)setCaptureMode:(DotEngineCaptureMode)captureMode;





/**
 *  切换摄像头
 */
-(void)switchCamera;




/**
 *  enable/disable 本地视频
 *
 *  @param enable true  or  false
 */
-(void)enableVideo:(BOOL)enable;



/**
 *  enable/disable  本地音频
 *
 *  @param enable true or false
 */
-(void)enableAudio:(BOOL)enable;



/**
 *  得到某个用户的音频状态
 *
 *  @param userId
 *
 *  @return true or  false
 */
-(BOOL)isAudioEnable:(NSString*)userId;



/**
 *  得到某个用户的视频状态
 *
 *  @param userId
 *
 *  @return true or false
 */
-(BOOL)isVideoEnable:(NSString*)userId;




/**
 *  开启/关闭扬声器
 *
 *  @param enable YES  NO
 */

-(void)enableSpeakerphone:(BOOL)enable;



/**
 <#Description#>

 @param appkey     <#appkey description#>
 @param appsecret  <#appsecret description#>
 @param room       <#room description#>
 @param userid     <#userid description#>
 @param tokenBlock <#tokenBlock description#>
 */
-(void)generateTestTokenWithAppKey:( NSString* _Nonnull )appkey
                         appsecret:(NSString* _Nonnull )appsecret
                              room:(NSString* _Nonnull )room
                            userId:(NSString* _Nonnull )userid
                         withBlock:(void (^)(NSString* token,NSError* error))tokenBlock;



/**
 <#Description#>

 @param sample_buffer sample_buffer description
 */
-(void) sendSampleBuffer:(CMSampleBufferRef ) sampleBuffer;




/**
 <#Description#>

 @param pixelBuffer <#pixelBuffer description#>
 @param rotation    <#rotation description#>
 */
-(void) sendPixelBuffer:(CVPixelBufferRef)pixelBuffer rotation:(VideoRotation)rotation;



@end





@protocol DotEngineDelegate <NSObject>

/**
 *  用户加入房间   包括自己
 *
 *  @param engine
 *  @param userId 用户id
 */
-(void)dotEngine:(DotEngine*) engine didJoined:(NSString*)userId;


/**
 *  有人离开了房间  包括自己
 *
 *  @param engine <#engine description#>
 *  @param userId <#userId description#>
 */
-(void)dotEngine:(DotEngine*) engine didLeave:(NSString*)userId;




/**
 <#Description#>
 
 @param engine <#engine description#>
 @param view   <#view description#>
 */
-(void)dotEngine:(DotEngine*) engine  didAddLocalView:(UIView*)view;



/**
 <#Description#>
 
 @param engine <#engine description#>
 @param view   <#view description#>
 @param userId <#userId description#>
 */
-(void)dotEngine:(DotEngine*) engine  didAddRemoteView:(UIView*)view withUser:(NSString*)userId;


/**
 <#Description#>

 @param engine <#engine description#>
 @param view   <#view description#>
 */
-(void)dotEngine:(DotEngine*) engine didRemoveLocalView:(UIView*)view;



/**
 <#Description#>

 @param engine <#engine description#>
 @param view   <#view description#>
 @param userId <#userId description#>
 */
-(void)dotEngine:(DotEngine*) engine didRemoveRemoteView:(UIView*)view withUser:(NSString*)userId;





/**
 本地audio track ready
 */
@optional
-(void)dotEngineDidAddLocalAudioTrack;



/**
 <#Description#>

 @param engine <#engine description#>
 @param userId <#userId description#>
 */
@optional
-(void)dotEngine:(DotEngine *)engine didAddRemoteAudioTrack:(NSString*)userId;



/**
 <#Description#>
 */
@optional
-(void)dotEngineDidRemoveLocalAudioTrack;




/**
 <#Description#>

 @param engine <#engine description#>
 @param userId <#userId description#>
 */
@optional
-(void)dotEngine:(DotEngine *)engine  didRemoveRemoteAudioTrack:(NSString *)userId;








/**
 *  用户静音/关闭静音回调
 *
 *  @param engine <#engine description#>
 *  @param muted  <#muted description#>
 *  @param userId <#userId description#>
 */
-(void)dotEngine:(DotEngine*)engine didEnableAudio:(BOOL)enable userId:(NSString*)userId;



/**
 *  用户开启/关闭视频回调
 *
 *  @param engine <#engine description#>
 *  @param enable <#enable description#>
 *  @param userId <#userId description#>
 */
-(void)dotEngine:(DotEngine*)engine didEnableVideo:(BOOL)enable userId:(NSString*)userId;



/**
 *  发生错误了
 *
 *  @param engine    <#engine description#>
 *  @param errorCode <#errorCode description#>
 */
-(void)dotEngine:(DotEngine*) engine didOccurError:(DotEngineErrorCode)errorCode;




# pragma 

/**
 *
 *  capture mode  分为三类    1, default   dotEngine处理音频和视频的输入   2, custom_video   用户处理视频输入 dotEngine处理音频输入
 *                           3, custom_audio_video   用户处理和视频的输入
 */



/**
 视频view 默认被添加到parent view 之后才会渲染 否则 做丢弃处理   add a new renderer or  based the view renderer
 
 audio
 
 onLocalAudioFrame
 
 onRemoteAudioFrame
 
 
 video
 
 onLocalVideoFrame
 
 onRemoteVideoFrame
 
 */



@optional
-(void)dotEngine:(DotEngine*)engine onLocalAudioFrame:(DotEngineAudioFrame*)frame;



@optional
-(void)dotEngine:(DotEngine*)engine onRemoteAudioFrame:(DotEngineAudioFrame*)frame userId:(NSString*)userId;




@optional
-(void)dotEngine:(DotEngine*)engine onLocalVideoFrame:(DotEngineVideoFrame*)frame;


@optional
-(void)dotEngine:(DotEngine*)engine onRemoteVideoFrame:(DotEngineVideoFrame*)frame userId:(NSString*)userId;


@end
