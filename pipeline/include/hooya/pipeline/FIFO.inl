namespace hooya::pipeline {
template <class T>
inline void FIFO<T>::Name(const std::string& n) {
	name = n;
}

template <class T>
inline T FIFO<T>::Pop() {
	// Wait on an alert from Push(...) call
	qAlert.Lower();

	// dequeue() and process received message...
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
		queue.push_back(std::move(m));
	}

	// Alert threads about the message's existence
	qAlert.Raise();
}

template <class T>
inline T FIFO<T>::dequeue() {
	const std::lock_guard<std::mutex> lock(qLock);
	auto ret = std::move(queue.front());
	queue.pop_front();

	return ret;
} }
