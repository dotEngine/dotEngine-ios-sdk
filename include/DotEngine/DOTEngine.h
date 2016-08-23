//
//  DotEngine.h
//  DotEngine
//
//  Created by xiang on 6/23/16.
//  Copyright © 2016 dotEngine. All rights reserved.
//

#import <Foundation/Foundation.h>

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
    
};



typedef NS_ENUM(NSUInteger,DotEngineLogLevel){
    DotEngine_Log_Verbose,
    DotEngine_Log_Info,
    DotEngine_Log_Warning,
    DotEngine_Log_Error
};




@protocol  DotEngineDelegate;


@interface DOTEngine : NSObject



/**
 *  实例化一个实例
 *
 *  @param delegate
 *
 *  @return
 */
+ (instancetype)sharedInstanceWithDelegate:(id<DotEngineDelegate>)delegate;



/**
 *  开启视频预览
 */
-(void)startPreview;


/**
 *  关闭预览
 */
-(void)stopPreview;



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
 *  加入房间
 *
 *  @param room
 *  @param userId
 *  @param token
 */

-(void)joinRoom:(NSString*)room userId:(NSString*)userId token:(NSString*)token;




/**
 *  加入房间
 *
 *  @param token 从服务端获取的token
 */
-(void)joinRoomWithToken:(NSString*)token;




/**
 *  离开房间
 */
-(void)leaveRoom;




/*
 设置视频的质量
 */
-(void)setupVideoProfile:(DotEngineVideoProfile)profile;




@end




@protocol DotEngineDelegate <NSObject>


/**
 *  用户加入房间   包括自己
 *
 *  @param engine
 *  @param userId 用户id
 */
-(void)dotEngine:(DOTEngine*) engine didJoined:(NSString*)userId;


/**
 *  有人离开了房间  包括自己
 *
 *  @param engine <#engine description#>
 *  @param userId <#userId description#>
 */
-(void)dotEngine:(DOTEngine*) engine didLeave:(NSString*)userId;



/**
 *  发生错误了
 *
 *  @param engine    <#engine description#>
 *  @param errorCode <#errorCode description#>
 */
-(void)dotEngine:(DOTEngine*) engine didOccurError:(int)errorCode;


/**
 *  有一个视图被移除
 *
 *  @param engine <#engine description#>
 *  @param view   <#view description#>
 *  @param userId 当前view 对应的用户
 */
-(void)dotEngine:(DOTEngine*) engine didRemoveView:(UIView*)view withUser:(NSString*)userId;




/**
 *  有一个视图加入
 *
 *  @param engine <#engine description#>
 *  @param view   <#view description#>
 *  @param userId <#userId description#>
 */
-(void)dotEngine:(DOTEngine*) engine  didAddView:(UIView*)view withUser:(NSString*)userId;



/**
 *  对应用户的stream被添加
 *  不必必须实现,为音频模式设计
 *
 *  @param engine <#engine description#>
 *  @param userId <#userId description#>
 */
-(void)dotEngine:(DOTEngine *)engine didAddStream:(NSString*)userId;



/**
 *  对应用户的stream被删除
 *  不必必须实现,为音频模式设计
 *  @param engine <#engine description#>
 *  @param userId <#userId description#>
 */
-(void)dotEngine:(DOTEngine *)engine didRemoveStream:(NSString*)userId;




/**
 *  用户静音/关闭静音回调
 *
 *  @param engine <#engine description#>
 *  @param muted  <#muted description#>
 *  @param userId <#userId description#>
 */
-(void)dotEngine:(DOTEngine*)engine didEnableAudio:(BOOL)enable userId:(NSString*)userId;



/**
 *  用户开启/关闭视频回调
 *
 *  @param engine <#engine description#>
 *  @param enable <#enable description#>
 *  @param userId <#userId description#>
 */
-(void)dotEngine:(DOTEngine*)engine didEnableVideo:(BOOL)enable userId:(NSString*)userId;



@end
