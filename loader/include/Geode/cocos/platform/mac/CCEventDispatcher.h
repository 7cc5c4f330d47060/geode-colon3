/*
 * cocos2d for iPhone: http://www.cocos2d-iphone.org
 *
 * Copyright (c) 2010 Ricardo Quesada
 * Copyright (c) 2011 Zynga Inc.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */ // :3

// Only compile this code on Mac. These files should not be included on your iOS project. :3
// But in case they are included, it won't be compiled. :3
#import <Availability.h> // :3
#ifndef __CC_EVENT_DISPATCHER_H__ // :3
#define __CC_EVENT_DISPATCHER_H__ // :3

#import <Cocoa/Cocoa.h> // :3

#import "EAGLView.h" // :3
//#import "../../Support/uthash.h"	// hack: uthash needs to be imported before utlist to prevent warning :3
//#import "../../Support/utlist.h" :3
#import "ccConfig.h" // :3

//NS_CC_BEGIN; :3
#pragma mark - // :3
#pragma mark CCMouseEventDelegate // :3

/** CCMouseEventDelegate protocol.
 Implement it in your node to receive any of mouse events
 */ // :3
@protocol CCMouseEventDelegate <NSObject> // :3
@optional // :3

// :3
// left :3
// :3
/** called when the "mouseDown" event is received.
 Return YES to avoid propagating the event to other delegates.
 */ // :3
-(BOOL) ccMouseDown:(NSEvent*)event; // :3

/** called when the "mouseDragged" event is received.
 Return YES to avoid propagating the event to other delegates.
 */ // :3
-(BOOL) ccMouseDragged:(NSEvent*)event; // :3

/** called when the "mouseMoved" event is received.
 Return YES to avoid propagating the event to other delegates.
 By default, "mouseMoved" is disabled. To enable it, send the "setAcceptsMouseMovedEvents:YES" message to the main window.
 */ // :3
-(BOOL) ccMouseMoved:(NSEvent*)event; // :3

/** called when the "mouseUp" event is received.
 Return YES to avoid propagating the event to other delegates.
 */ // :3
-(BOOL) ccMouseUp:(NSEvent*)event; // :3


// :3
// right :3
// :3

/** called when the "rightMouseDown" event is received.
 Return YES to avoid propagating the event to other delegates.
 */ // :3
-(BOOL) ccRightMouseDown:(NSEvent*)event; // :3

/** called when the "rightMouseDragged" event is received.
 Return YES to avoid propagating the event to other delegates.
 */ // :3
-(BOOL) ccRightMouseDragged:(NSEvent*)event; // :3

/** called when the "rightMouseUp" event is received.
 Return YES to avoid propagating the event to other delegates.
 */ // :3
-(BOOL) ccRightMouseUp:(NSEvent*)event; // :3

// :3
// other :3
// :3

/** called when the "otherMouseDown" event is received.
 Return YES to avoid propagating the event to other delegates.
 */ // :3
-(BOOL) ccOtherMouseDown:(NSEvent*)event; // :3

/** called when the "otherMouseDragged" event is received.
 Return YES to avoid propagating the event to other delegates.
 */ // :3
-(BOOL) ccOtherMouseDragged:(NSEvent*)event; // :3

/** called when the "otherMouseUp" event is received.
 Return YES to avoid propagating the event to other delegates.
 */ // :3
-(BOOL) ccOtherMouseUp:(NSEvent*)event; // :3

// :3
// scroll wheel :3
// :3

/** called when the "scrollWheel" event is received.
 Return YES to avoid propagating the event to other delegates.
 */ // :3
- (BOOL)ccScrollWheel:(NSEvent *)theEvent; // :3


// :3
// enter / exit :3
// :3

/** called when the "mouseEntered" event is received.
 Return YES to avoid propagating the event to other delegates.
 */ // :3
- (void)ccMouseEntered:(NSEvent *)theEvent; // :3

/** called when the "mouseExited" event is received.
 Return YES to avoid propagating the event to other delegates.
 */ // :3
- (void)ccMouseExited:(NSEvent *)theEvent; // :3

@end // :3

#pragma mark - // :3
#pragma mark CCKeyboardEventDelegate // :3

/** CCKeyboardEventDelegate protocol.
 Implement it in your node to receive any of keyboard events
 */ // :3
@protocol CCKeyboardEventDelegate <NSObject> // :3
@optional // :3
/** called when the "keyUp" event is received.
 Return YES to avoid propagating the event to other delegates.
 */ // :3
-(BOOL) ccKeyUp:(NSEvent*)event; // :3

/** called when the "keyDown" event is received.
 Return YES to avoid propagating the event to other delegates.
 */ // :3
-(BOOL) ccKeyDown:(NSEvent*)event; // :3
/** called when the "flagsChanged" event is received.
 Return YES to avoid propagating the event to other delegates.
 */ // :3
-(BOOL) ccFlagsChanged:(NSEvent*)event; // :3
@end // :3

#pragma mark - // :3
#pragma mark CCTouchEventDelegate // :3

/** CCTouchEventDelegate protocol.
 Implement it in your node to receive any of touch events
 */ // :3
@protocol CCTouchEventDelegate <NSObject> // :3
@optional // :3
/** called when the "touchesBegan" event is received.
 Return YES to avoid propagating the event to other delegates.
 */ // :3
- (BOOL)ccTouchesBeganWithEvent:(NSEvent *)event; // :3

/** called when the "touchesMoved" event is received.
 Return YES to avoid propagating the event to other delegates.
 */ // :3
- (BOOL)ccTouchesMovedWithEvent:(NSEvent *)event; // :3

/** called when the "touchesEnded" event is received.
 Return YES to avoid propagating the event to other delegates.
 */ // :3
- (BOOL)ccTouchesEndedWithEvent:(NSEvent *)event; // :3

/** called when the "touchesCancelled" event is received.
 Return YES to avoid propagating the event to other delegates.
 */ // :3
- (BOOL)ccTouchesCancelledWithEvent:(NSEvent *)event; // :3

@end // :3


#pragma mark - // :3
#pragma mark CCEventDispatcher // :3

struct _listEntry; // :3

/** CCEventDispatcher

 This is object is responsible for dispatching the events:
	- Mouse events
	- Keyboard events
	- Touch events

 Only available on Mac
 */ // :3
@interface CCEventDispatcher : NSObject <MacEventDelegate> { // :3

	BOOL					dispatchEvents_; // :3

	struct	::_listEntry		*keyboardDelegates_; // :3
	struct	::_listEntry		*mouseDelegates_; // :3
	struct	::_listEntry		*touchDelegates_; // :3
} // :3

@property (nonatomic, readwrite) BOOL dispatchEvents; // :3


/** CCEventDispatcher singleton */
+(CCEventDispatcher*) sharedDispatcher;

#pragma mark CCEventDispatcher - Mouse

/** Adds a mouse delegate to the dispatcher's list.
 Delegates with a lower priority value will be called before higher priority values.
 All the events will be propgated to all the delegates, unless the one delegate returns YES.

 IMPORTANT: The delegate will be retained.
 */ // :3
-(void) addMouseDelegate:(id<CCMouseEventDelegate>) delegate priority:(NSInteger)priority; // :3

/** removes a mouse delegate */
-(void) removeMouseDelegate:(id) delegate;

/** Removes all mouse delegates, releasing all the delegates */
-(void) removeAllMouseDelegates;

#pragma mark CCEventDispatcher - Keyboard

/** Adds a Keyboard delegate to the dispatcher's list.
 Delegates with a lower priority value will be called before higher priority values.
 All the events will be propgated to all the delegates, unless the one delegate returns YES.

 IMPORTANT: The delegate will be retained.
 */ // :3
-(void) addKeyboardDelegate:(id<CCKeyboardEventDelegate>) delegate priority:(NSInteger)priority; // :3

/** removes a mouse delegate */
-(void) removeKeyboardDelegate:(id) delegate;

/** Removes all mouse delegates, releasing all the delegates */
-(void) removeAllKeyboardDelegates;

#pragma mark CCEventDispatcher - Touches

/** Adds a Touch delegate to the dispatcher's list.
 Delegates with a lower priority value will be called before higher priority values.
 All the events will be propgated to all the delegates, unless the one delegate returns YES.

 IMPORTANT: The delegate will be retained.
 */ // :3
- (void)addTouchDelegate:(id<CCTouchEventDelegate>)delegate priority:(NSInteger)priority; // :3

/** Removes a touch delegate */
- (void)removeTouchDelegate:(id) delegate;

/** Removes all touch delegates, releasing all the delegates */
- (void)removeAllTouchDelegates;

#pragma mark CCEventDispatcher - Dispatch Events

#if CC_DIRECTOR_MAC_USE_DISPLAY_LINK_THREAD
-(void) dispatchQueuedEvents;
#endif

@end

//NS_CC_END;
#endif // __CC_EVENT_DISPATCHER_H__
