#pragma once

template<typename T>
struct MutexType {
	T data;
	std::mutex mutex;

	operator T*() {
		return &data;
	}
};
