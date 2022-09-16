#include <algorithm>
#include <numeric>
#include <vector>


namespace utils {
	/*
	 * Idk if it will work with non-number types.
	 * So please don't use it with non-number types.
	 *
	 * Applies to all functions below.
	 */

	template <typename T>
	T average(std::vector<T> v) {
		T sum = std::accumulate(v.begin(), v.end(), (T)0.0);

		return sum / (float)v.size();
	}

	template <typename T>
	T median(std::vector<T> v) {
		int n = v.size();

		if (n % 2 == 0) {
			std::nth_element(v.begin(), v.begin() + n / 2, v.end());
			std::nth_element(v.begin(), v.begin() + (n - 1) / 2, v.end());
	  
			return (float)(v[(n - 1) / 2] + v[n / 2]) / 2.0;
		} else {
			std::nth_element(v.begin(), v.begin() + n / 2, v.end());
	  
			return (double)v[n / 2];
		}
	}

	template <typename T>
	T max(std::vector<T> v) {
		return *std::max_element(v.begin(), v.end());
	}

	template <typename T>
	T min(std::vector<T> v) {
		return *std::min_element(v.begin(), v.end());
	}
};

