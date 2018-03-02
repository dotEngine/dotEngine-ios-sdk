//
//  DotEngineKit.h
//  DotEngineKit
//
//  Created by xiang on 6/23/16.
//  Copyright © 2016 dotEngine. All rights reserved.
//

#import <Foundation/Foundation.h>
@import AVFoundation;



@class DotStream;

typedef NS_ENUM(NSInteger)
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
    
} DotEngineVideoProfile;
 




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



typedef NS_ENUM(NSInteger, DotStatus) {
    DotStatusConnecting,
    DotStatusConnected,
    DotStatusDisConnected,
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

@property (nonatomic, readonly) DotStatus  status;


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
 <#Description#>

 @param stream stream description
 */
-(void)addStream:(DotStream* _Nonnull)stream;



/**
 <#Description#>

 @param stream <#stream description#>
 */
-(void)removeStream:(DotStream* _Nonnull)stream;



/**
 *  join room  with  token
 *
 *  @param token got from server side,
 */
-(void)joinRoomWithToken:(NSString* _Nonnull)token;


/**
 *  leave room
 */
-(void)leaveRoom;


/**
 *  speakerphone  enable or disable
 *
 *  @param enable YES  NO
 */

-(void)enableSpeakerphone:(BOOL)enable;



///**
//
// use for test mode
//
// @param appkey     <#appkey description#>
// @param appsecret  <#appsecret description#>
// @param room       <#room description#>
// @param userid     <#userid description#>
// @param tokenBlock <#tokenBlock description#>
// */
//-(void)generateTestTokenWithAppKey:( NSString* _Nonnull )appkey
//                         appsecret:(NSString* _Nonnull )appsecret
//                              room:(NSString* _Nonnull )room
//                            userId:(NSString* _Nonnull )userid
//                         withBlock:(void (^_Nonnull)(NSString* token,NSError* error))tokenBlock;
//

-(void)generateTestToken:(NSString* _Nonnull)tokenUrl
               appsecret:(NSString* _Nonnull )appsecret
                    room:(NSString* _Nonnull )room
                  userId:(NSString* _Nonnull )userId
               withBlock:(void (^_Nonnull)(NSString* token,NSError* error))tokenBlock;

@end



@protocol DotEngineDelegate <NSObject>


-(void)dotEngine:(DotEngine* _Nonnull) engine didJoined:(NSString* _Nonnull)peerId;

-(void)dotEngine:(DotEngine* _Nonnull) engine didLeave:(NSString* _Nonnull)peerId;

-(void)dotEngine:(DotEngine* _Nonnull) engine  stateChange:(DotStatus)state;

-(void)dotEngine:(DotEngine* _Nonnull) engine didAddLocalStream:(DotStream* _Nonnull)stream;

-(void)dotEngine:(DotEngine* _Nonnull) engine didRemoveLocalStream:(DotStream* _Nonnull)stream;

-(void)dotEngine:(DotEngine* _Nonnull) engine didAddRemoteStream:(DotStream* _Nonnull)stream;

-(void)dotEngine:(DotEngine* _Nonnull) engine didRemoveRemoteStream:(DotStream* _Nonnull) stream;

-(void)dotEngine:(DotEngine* _Nonnull) engine didOccurError:(DotEngineErrorCode)errorCode;


@end
