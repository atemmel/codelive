#pragma once

template<typename T>
struct MutexType {
	T data;
	std::mutex mutex;

	operator std::mutex & () {
		return mutex;
	}
};

using Guard = std::lock_guard<std::mutex>;
