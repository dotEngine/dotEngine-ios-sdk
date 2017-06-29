//
//  DotCapturer.h
//
//  Created by xiang on 27/05/2017.
//  Copyright © 2017 dotEngine. All rights reserved.
//


#import <Foundation/Foundation.h>
#import <CoreVideo/CoreVideo.h>


typedef NS_ENUM(int, VideoRotation){
    
    VideoRoation_0 = 0,
    VideoRoation_90 = 90,
    VideoRoation_180 = 180,
    VideoRoation_270 = 270,
    
};


@protocol DotCapturerConsumer <NSObject>


/**
 <#Description#>
 
 @param sample_buffer sample_buffer description
 */
-(void) sendSampleBuffer:(CMSampleBufferRef _Nonnull) sampleBuffer rotation:(VideoRotation)rotation;


@end



@protocol DotVideoCapturer <NSObject>


@property(atomic, assign) id<DotCapturerConsumer> _Nullable videoCapturerConsumer;


-(void)initCapturer;



-(void)releaseCapturer;



-(int32_t)startCapture;



-(int32_t)stopCapture;



-(BOOL)isCaptureStarted;


@end










