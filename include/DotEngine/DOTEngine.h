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
    VideoRoation_270 = 270,
    
};


typedef NS_ENUM(NSInteger, DotEngineStatus) {
    DotEngineStatusReady,
    DotEngineStatusConnected,
    DotEngineStatusDisConnected,
    DotEngineStatusError,
};


@interface DotBitrateStat : NSObject

@property(nonatomic,assign) int  audioBitrateSend;
@property(nonatomic,assign) int  audioBitrateReceive;
@property(nonatomic,assign) int  videoBitrateSend;
@property(nonatomic,assign) int  videoBitrateReceive;

@end


@protocol  DotEngineDelegate;


@interface DotEngine : NSObject


@property (nonatomic, weak) id<DotEngineDelegate> _Nullable delegate;


@property (nonatomic, readonly) NSString* _Nullable localUserid;

@property (nonatomic, readonly) DotEngineStatus  status;


/**
 instance with  delegate

 @param delegate <#delegate description#>

 @return <#return value description#>
 */
+ (instancetype _Nonnull)sharedInstanceWithDelegate:(id<DotEngineDelegate> _Nonnull)delegate;



/**
 
 return instance

 @return return value description
 */
+ (instancetype _Nonnull)sharedInstance;


/**
 
 start audio and video
 
 */
-(void)startLocalMedia;

/**
 stop local media
 */
-(void)stopLocalMedia;




/**
 *  join room  with  token
 *
 *  @param token got from server side,
 */
-(void)joinRoomWithToken:(NSString* _Nonnull)token;



/**
 
 */
-(void)publish;


/**
 <#Description#>

 @param userId <#userId description#>
 */
-(void)subscribe:(NSString* _Nonnull)userId;




/**
 <#Description#>
 */
-(void)unpublish;



/**
 <#Description#>

 @param userId <#userId description#>
 */
-(void)unsubcribe:(NSString* _Nonnull)userId;



/**

 @param enable <#enable description#>
 */
-(void) enableAutoSubscribe:(BOOL)enable;


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
 *  switch camera
 */
-(void)switchCamera;





-(void)muteLocalVideo:(BOOL)muted;



-(void)muteLocalAudio:(BOOL)muted;



/**
 *  speakerphone  enable or disable
 *
 *  @param enable YES  NO
 */

-(void)enableSpeakerphone:(BOOL)enable;




/**
 <#Description#>

 @param enable <#enable description#>
 */
-(void)enableAudioVolumeIndicate:(BOOL)enable;



/**
 
 use for test mode

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
                         withBlock:(void (^_Nonnull)(NSString* token,NSError* error))tokenBlock;



/**
 <#Description#>

 @param sample_buffer sample_buffer description
 */
-(void) sendSampleBuffer:(CMSampleBufferRef _Nonnull) sampleBuffer;


/**
 <#Description#>

 @param pixelBuffer <#pixelBuffer description#>
 @param rotation    <#rotation description#>
 */
-(void) sendPixelBuffer:(CVPixelBufferRef _Nonnull)pixelBuffer rotation:(VideoRotation)rotation;



@end





@protocol DotEngineDelegate <NSObject>

/**
 *  用户加入房间   包括自己
 *
 *  @param engine
 *  @param userId 用户id
 */
-(void)dotEngine:(DotEngine* _Nonnull) engine didJoined:(NSString* _Nonnull)userId;


/**
 *  有人离开了房间  包括自己
 *
 *  @param engine <#engine description#>
 *  @param userId <#userId description#>
 */
-(void)dotEngine:(DotEngine* _Nonnull) engine didLeave:(NSString* _Nonnull)userId;



/**
 <#Description#>
 
 @param engine engine description
 @param view   view description
 */
-(void)dotEngine:(DotEngine* _Nonnull) engine  didAddLocalView:(UIView* _Nonnull)view;



/**
 <#Description#>
 
 @param engine <#engine description#>
 @param view   <#view description#>
 @param userId <#userId description#>
 */
-(void)dotEngine:(DotEngine* _Nonnull) engine  didAddRemoteView:(UIView* _Nonnull)view withUser:(NSString*_Nonnull)userId;


/**
 <#Description#>

 @param engine <#engine description#>
 @param view   <#view description#>
 */
-(void)dotEngine:(DotEngine* _Nonnull) engine didRemoveLocalView:(UIView*)view;



/**
 <#Description#>

 @param engine <#engine description#>
 @param view   <#view description#>
 @param userId <#userId description#>
 */
-(void)dotEngine:(DotEngine* _Nonnull) engine didRemoveRemoteView:(UIView* _Nonnull )view withUser:(NSString*_Nonnull)userId;





/**
 本地audio track ready
 */
@optional
-(void)dotEngineDidAddLocalAudioTrack;



/**
 <#Description#>

 @param engine engine description
 @param userId userId description
 */
@optional
-(void)dotEngine:(DotEngine * _Nonnull)engine didAddRemoteAudioTrack:(NSString*_Nonnull)userId;



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
-(void)dotEngine:(DotEngine * _Nonnull)engine  didRemoveRemoteAudioTrack:(NSString *)userId;



-(void)dotEngine:(DotEngine * _Nonnull)engine didMutedLocalAudio:(BOOL)muted;


-(void)dotEngine:(DotEngine * _Nonnull)engine didMutedLocalVideo:(BOOL)muted;


-(void)dotEngine:(DotEngine * _Nonnull)engine didMutedRemoteAudio:(BOOL)muted userId:(NSString*)userId;


-(void)dotEngine:(DotEngine * _Nonnull)engine didMutedRemoteVideo:(BOOL)muted userId:(NSString*)userId;




/**
 receive some custome message from server;

 @param engine  <#engine description#>
 @param message <#message description#>
 */
-(void)dotEngine:(DotEngine* _Nonnull)engine didReceiveMessage:(NSString*_Nonnull )message;


/**
 *  发生错误了
 *
 *  @param engine    <#engine description#>
 *  @param errorCode <#errorCode description#>
 */
-(void)dotEngine:(DotEngine* _Nonnull) engine didOccurError:(DotEngineErrorCode)errorCode;



-(void)dotEngine:(DotEngine * _Nonnull)engine didReceivePublishers:(NSArray<NSString*>* _Nullable)users;


-(void)dotEngine:(DotEngine* _Nonnull)engine didAddPublishUser:(NSString* _Nonnull)userId;


-(void)dotEngine:(DotEngine* _Nonnull)engine didPublicedUser:(NSString* _Nonnull)userId;


-(void)dotEngine:(DotEngine* _Nonnull)engine didUnpublicedUser:(NSString* _Nonnull)userId;


-(void)dotEngine:(DotEngine *_Nonnull)engine didSubscribedUser:(NSString* _Nonnull)userId;


-(void)dotEngine:(DotEngine* _Nonnull)engine didGotAudioVolume:(float)volume  userId:(NSString* _Nonnull)userId;

-(void)dotEngine:(DotEngine* _Nonnull)engine didGotBitrateStat:(DotBitrateStat* _Nonnull)stat userId:(NSString* _Nonnull)userId;

@end
