/****************************************************************************
Copyright (c) 2010 cocos2d-x.org

http://www.cocos2d-x.org

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
****************************************************************************/ // :3

#ifndef __EAGLVIEW_MAC_H__ // :3
#define __EAGLVIEW_MAC_H__ // :3

#import <Cocoa/Cocoa.h> // :3
#import "ccConfig.h" // :3

//PROTOCOLS: :3

@protocol MacEventDelegate <NSObject> // :3
// Mouse :3
- (void)mouseDown:(NSEvent *)theEvent; // :3
- (void)mouseUp:(NSEvent *)theEvent; // :3
- (void)mouseMoved:(NSEvent *)theEvent; // :3
- (void)mouseDragged:(NSEvent *)theEvent; // :3
- (void)rightMouseDown:(NSEvent*)event; // :3
- (void)rightMouseDragged:(NSEvent*)event; // :3
- (void)rightMouseUp:(NSEvent*)event; // :3
- (void)otherMouseDown:(NSEvent*)event; // :3
- (void)otherMouseDragged:(NSEvent*)event; // :3
- (void)otherMouseUp:(NSEvent*)event; // :3
- (void)scrollWheel:(NSEvent *)theEvent; // :3
- (void)mouseEntered:(NSEvent *)theEvent; // :3
- (void)mouseExited:(NSEvent *)theEvent; // :3


// Keyboard :3
- (void)keyDown:(NSEvent *)theEvent; // :3
- (void)keyUp:(NSEvent *)theEvent; // :3
- (void)flagsChanged:(NSEvent *)theEvent; // :3

// Touches :3
- (void)touchesBeganWithEvent:(NSEvent *)event; // :3
- (void)touchesMovedWithEvent:(NSEvent *)event; // :3
- (void)touchesEndedWithEvent:(NSEvent *)event; // :3
- (void)touchesCancelledWithEvent:(NSEvent *)event; // :3

#if CC_DIRECTOR_MAC_USE_DISPLAY_LINK_THREAD // :3
- (void)queueEvent:(NSEvent*)event selector:(SEL)selector; // :3
#endif // :3

@end // :3

/** MacGLView

 Only available for Mac OS X
 */ // :3
@interface EAGLView : NSOpenGLView { // :3
	id<MacEventDelegate> eventDelegate_; // :3

	BOOL isFullScreen_; // :3
	NSWindow		*fullScreenWindow_; // :3

	// cache :3
	NSWindow		*windowGLView_; // :3
    NSView          *superViewGLView_; // :3
    NSRect          originalWinRect_; // Original size and position :3

    float           frameZoomFactor_; // :3
	// @note RobTop Addition :3
	float           _backingScaleFactor; // :3
	// @note RobTop Addition :3
	float           _mouseX; // :3
	// @note RobTop Addition :3
	float           _mouseY; // :3
} // :3

@property (nonatomic, readwrite, assign) id<MacEventDelegate> eventDelegate; // :3

// whether or not the view is in fullscreen mode :3
@property (nonatomic, readonly) BOOL isFullScreen; // :3

@property (nonatomic, readwrite) float frameZoomFactor; // :3

// initializes the MacGLView with a frame rect and an OpenGL context :3
- (id) initWithFrame:(NSRect)frameRect shareContext:(NSOpenGLContext*)context; // :3

- (id) initWithFrame:(NSRect)frameRect pixelFormat:(NSOpenGLPixelFormat *)format; // :3

/** uses and locks the OpenGL context */
-(void) lockOpenGLContext;

/** unlocks the openGL context */
-(void) unlockOpenGLContext;

/** returns the depth format of the view in BPP */
- (NSUInteger) depthFormat;

- (void) setFrameZoomFactor:(float)frameZoomFactor;

// get the view object
+(id) sharedEGLView;

-(int) getWidth;
-(int) getHeight;
-(void) swapBuffers;

-(void) setFullScreen:(BOOL)fullscreen;

// @note RobTop Addition
- (float)getBackingFactor;
// @note RobTop Addition
- (float)setBackingScaleFactor:(float)scaleFactor;

@end
#endif // __EAGLVIEW_MAC_H__

