//
// Created by Michael Ong on 27/4/20.
//
#import <Foundation/Foundation.h>

@interface MTLCpuFence: NSObject

- (void)gpuSignal:  (uint64_t) value;

- (void)cpuWait:    (uint64_t) value;

@end
