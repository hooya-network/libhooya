#include "hooya/pipeline/CountingSemaphore.hh"

namespace hooya::pipeline {
void CountingSemaphore::Raise() {
	++count;

	// Do a little dance...
	semGuard.lock();
	sem.unlock();
}

CountingSemaphore::CountingSemaphore() {
	// Start with the semaphore lowered
	count = 0;

	// Take posession so we can wait for a Raise()
	sem.lock();
}

void CountingSemaphore::Lower() {
	// (Un)Do a little dance...
	semGuard.unlock();
	sem.lock();

	assert(count > 0);
	--count;
} }
