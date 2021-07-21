#pragma once

#include <iostream>
#include <string>
#include <cstdio>
#include <mutex>
#include <list>
#include <memory>
#include "hooya/pipeline/CountingSemaphore.hh"
#include "hooya/pipeline/Exception.hh"

namespace hooya::pipeline {
/**
 * \class FIFO
 * First-in first-out queue with blocking Pop()
 */
template <class T>
class FIFO {
public:
	FIFO();

	/**
	 * Push onto beginning of queue
	 * \param i Thing being emplaced onto queue
	 */
	void Push(T i);

	/**
	 * Interrupt count threads waiting on a Pop().
	 * Useful when wrangling a number of threads back to the caller for a join()
	 * \param count Number of threads to awaken
	 */
	void Interrupt(int count);

	/**
	 * Diagnostic Summary of the queue
	 */
	void ShowSummary();

	/**
	 * Pop off the back of the queue
	 * \return Oldest item no longer on queue
	 */
	T Pop();

	/**
	 * Set diagnostic name of the FIFO
	 * \arg n Name
	 */
	void Name(const std::string &n);

	/**
	 * Diagnostic name of the FIFO
	 * \return Name
	 */
	const char *Name();

	/**
	 * Number of items that have been Push()'d but not Pop()'d
	 * \return Length of the FIFO buffer
	 */
	size_t Length();

private:

	/* Pop off the front item */
	T dequeue();

	/* Managing FIFO Pop() interrupts */
	int toInterrupt;
	std::mutex interruptLock;

	/* Queue and its [writer] mutual exclusion */
	std::list<T> queue;
	std::mutex qLock;

	/* Raised for each incoming item */
	CountingSemaphore qAlert;

	std::string name;
};

}

#include "FIFO.inl"

