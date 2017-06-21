//
//  DotView.h
//  dot-engine-ios
//
//  Created by xiang on 03/04/2017.
//  Copyright © 2017 dotEngine. All rights reserved.
//

#import <Foundation/Foundation.h>


@class DotView;
@class DotStream;

typedef NS_ENUM(NSUInteger, DotVideoViewScaleMode)
{
    
    DotVideoViewScaleModeFit = 0,
    DotVideoViewScaleModeFill = 1,
};


@protocol DotViewDelegate <NSObject>

- (void)videoViewDidReceiveData:(DotView *)renderer withSize:(CGSize)dimensions;
- (void)videoView:(DotView *)renderer streamDimensionsDidChange:(CGSize)dimensions;

@end

@interface DotView : UIView


@property (nonatomic, readonly) CGSize videoSize;
@property (nonatomic, readonly) BOOL hasVideoData;
@property (nonatomic, weak)     id<DotViewDelegate>  dotViewDelegate;
@property (nonatomic, assign)   DotVideoViewScaleMode scaleMode;
@property (nonatomic, assign)   BOOL mirror;


- (instancetype)initWithFrame:(CGRect)frame;

- (void)setStream:(DotStream *)stream;

@end
