//
//  DotVideoCapturer.h
//  dot-engine-ios
//
//  Created by xiang on 18/08/2017.
//  Copyright © 2017 dotEngine. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <CoreVideo/CoreVideo.h>

typedef NS_ENUM(NSInteger, VideoRotation){
    
    VideoRoation_0 = 0,
    VideoRoation_90 = 90,
    VideoRoation_180 = 180,
    VideoRoation_270 = 270,
};



// DotStream is is DotCapturerConsumer
@protocol DotCapturerConsumer <NSObject>

-(void) gotCVPixelBuffer:(CVPixelBufferRef _Nonnull) sampleBuffer rotation:(VideoRotation)rotation;

@end



@interface DotVideoCapturer : NSObject

@property(atomic, assign) id<DotCapturerConsumer> _Nullable videoConsumer;

-(void) sendCVPixelBuffer:(CVPixelBufferRef _Nonnull)pixelBuffer rotation:(VideoRotation)rotation;

@end
