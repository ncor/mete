#include <algorithm>
#include <numeric>
#include <vector>
#include <chrono>
#include <string>


/*
 * Idk if it will work with non-numeric types.
 * So please don't use it with non-numeric types.
 *
 * Applies to all functions below.
 */

template <typename T>
inline T utils::average(std::vector<T> v) {
	T sum = std::accumulate(v.begin(), v.end(), (T)0.0);

	return (T)(sum / (float)v.size());
}

template <typename T>
inline T utils::median(std::vector<T> v) {
	int n = v.size();

	if (n % 2 == 0) {
		std::nth_element(v.begin(), v.begin() + n / 2, v.end());
		std::nth_element(v.begin(), v.begin() + (n - 1) / 2, v.end());
  
		return (unsigned long long)((float)(v[(n - 1) / 2] + v[n / 2]) / 2.0);
	} else {
		std::nth_element(v.begin(), v.begin() + n / 2, v.end());
  
		return v[n / 2];
	}
}

template <typename T>
inline T utils::max(std::vector<T> v) {
	return *std::max_element(v.begin(), v.end());
}

template <typename T>
inline T utils::min(std::vector<T> v) {
	return *std::min_element(v.begin(), v.end());
}
