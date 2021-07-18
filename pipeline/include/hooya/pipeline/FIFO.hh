#pragma once

#include <iostream>
#include <string>
#include <cstdio>
#include <mutex>
#include <list>
#include <memory>
#include "hooya/pipeline/CountingSemaphore.hh"
#include "hooya/sock/DGram.hh"

namespace hooya::pipeline {
/**
 * \class FIFO
 * First-in first-out queue with blocking Pop()
 */
template <class T>
class FIFO {
public:
	/*
	 * Push onto beginning of queue
	 * \arg i Thing being emplaced onto queue
	 */
	void Push(T i);

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
	T dequeue();

	std::mutex qLock;
	CountingSemaphore qAlert;
	std::list<T> queue;
	std::string name;
};

using DGramFIFO = FIFO<hooya::sock::DGram>;

}

#include "FIFO.inl"

