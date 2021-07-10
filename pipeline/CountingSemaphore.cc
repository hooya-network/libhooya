#include "hooya/pipeline/CountingSemaphore.hh"
#include <iostream>

namespace hooya::pipeline {
void CountingSemaphore::Raise() {
	const std::lock_guard<std::mutex> l(semGuard);

	// Critical section
	if (++count == 1) sem.unlock();
}

CountingSemaphore::CountingSemaphore() {
	// Start with the semaphore lowered
	count = 0;

	// Take posession so we can wait for a Raise()
	sem.lock();
}

void CountingSemaphore::Lower() {
	sem.lock();
	{
		const std::lock_guard<std::mutex> l(semGuard);

		// Critical section
		if (--count) sem.unlock();
	}
} }
