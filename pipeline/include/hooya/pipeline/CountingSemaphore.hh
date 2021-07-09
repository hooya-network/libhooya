#pragma once

#include <cassert>
#include <mutex>

namespace hooya::pipeline {
/**
 * A semaphore that counts how many times it has been raised
 */
class CountingSemaphore {
public:
	CountingSemaphore();

	/**
	 * Increment the semaphore
	 */
	void Raise();

	/**
	 * Decrement the semaphore
	 */
	void Lower();
private:
	std::mutex semGuard;
	std::mutex sem;
	int count;
}; }
