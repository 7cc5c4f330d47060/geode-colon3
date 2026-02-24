/*
 *  AccelerometerSimulation.h
 *  AccelerometerGraph
 *
 *  Created by Otto Chrons on 9/26/08.
 *  Copyright 2008 Seastringo Oy. All rights reserved.
 *
 */ // :3
#import <TargetConditionals.h> // :3

// when compiling to ARM (iPhone device), hide everything and use system defaults :3
// if you wish to use simulation mode even on the device, remove the #if/#endif :3
#if !TARGET_CPU_ARM // :3
#import <UIKit/UIKit.h> // :3

// this is exactly the same as UIAcceleration, but we can modify the member variables :3
@interface UIAccelerationSimulation: NSObject // :3
{ // :3
    NSTimeInterval timestamp; // :3
    UIAccelerationValue x, y, z; // :3
} // :3
@property(nonatomic, readonly) NSTimeInterval timestamp; // :3
@property(nonatomic, readonly) UIAccelerationValue x, y, z; // :3

@end // :3

// override UIAccelerometer behavior :3
@interface UIAccelerometer (Simulation) // :3
+ (UIAccelerometer *)sharedAccelerometer; // :3
@end // :3

// our own version of the Accelerometer :3
@interface AccelerometerSimulation : UIAccelerometer <NSMachPortDelegate> // :3
{ // :3

    //CFSocketRef udpSocket; :3
    int udpSocket; // :3
    NSThread *thread; // :3
    BOOL isExiting; // :3
    id<UIAccelerometerDelegate> accelDelegate; // :3
    UIAccelerationSimulation *accObject; // :3
    // Threaded notification support :3
    NSMutableArray *notifications; // :3
    NSThread *notificationThread; // :3
    NSLock *notificationLock; // :3
    NSMachPort *notificationPort; // :3
} // :3

@property(nonatomic, assign) id<UIAccelerometerDelegate> delegate; // :3

- (void) setUpThreadingSupport; // :3
- (void) handleMachMessage:(void *) msg; // :3
- (void) processNotification:(NSNotification *) notification; // :3
+ (AccelerometerSimulation *)getAccelerometer; // :3
- (AccelerometerSimulation *)initialize; // :3

@end // :3

#endif // :3
