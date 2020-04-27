//
// Created by Michael Ong on 27/4/20.
//
#import "metal/mtl_cpu_fence.h"

#import <chrono>

#import <mutex>
#import <condition_variable>

using namespace std::chrono_literals;

@implementation MTLCpuFence {

	std::mutex                _lock;
	std::condition_variable   _wait;

	uint64_t                  _value;
}

- (instancetype)init {

	if (self = [super init]) {
		_value = 0;
	}

	return self;
}

- (void)gpuSignal:  (uint64_t)value {

	_value = value;
	_wait.notify_all();
}

- (void)cpuWait:    (uint64_t) value {

	std::unique_lock<std::mutex> lock{ _lock };
	while (value < _value) {
		_wait.wait_for(lock, 1ms);
	}

	_value = value;
}

@end
