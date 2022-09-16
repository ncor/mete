#pragma once

#include <vector>


namespace utils {
	/*
	 * Array(vector) functions.
	 */

	template <typename T>
	T average(std::vector<T> v);

	template <typename T>
	T median(std::vector<T> v);

	template <typename T>
	T max(std::vector<T> v);

	template <typename T>
	T min(std::vector<T> v);
}
