namespace hooya::pipeline {
template <class T>
inline FIFO<T>::FIFO() :
toInterrupt(0)
{ }
template <class T>
inline void FIFO<T>::Name(const std::string& n) {
	name = n;
}

template <class T>
inline void FIFO<T>::Interrupt(int count) {
	const std::lock_guard<std::mutex> lock(interruptLock);
	toInterrupt = count;
	while (count--) {
		qAlert.Raise();
	}
}

template <class T>
inline T FIFO<T>::Pop() {
	/* Wait on an alert from Push(...) call */
	qAlert.Lower();

	{
		/* Check if we were awakened by an interrupt */
		const std::lock_guard<std::mutex> lock(interruptLock);
		if (toInterrupt) {
			toInterrupt--;
			throw exception::Interrupt("FIFO Pop() interrupted");
		}
	}

	/* dequeue() and process received message... */
	T curr = dequeue();
	return curr;
}

template <class T>
inline void FIFO<T>::ShowSummary() {
	printf("[%s]: FIFO summary\n", Name());
	printf("[%s]: Items in FIFO: %d\n", Name(), queue.size());
}

template <class T>
inline const char *FIFO<T>::Name() {
	if (name.empty())
		return "No-name";
	return name.c_str();
}

template <class T>
inline void FIFO<T>::Push(T m) {
	{
		const std::lock_guard<std::mutex> lock(qLock);
		queue.push_back(m);
	}

	/* Alert threads about the message's existence */
	qAlert.Raise();
}

template <class T>
inline size_t FIFO<T>::Length() {
	return queue.size();
}

template <class T>
inline T FIFO<T>::dequeue() {
	const std::lock_guard<std::mutex> lock(qLock);
	auto ret = std::move(queue.front());
	queue.pop_front();

	return ret;
} }
