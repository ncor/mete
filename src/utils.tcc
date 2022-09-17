#include <algorithm>
#include <numeric>
#include <vector>
#include <chrono>
#include <string>


inline std::vector<std::string> utils::LoadingIndicator::braileSymbols = {
	"⠟",
    	"⠯",
    	"⠷",
    	"⠾",
   	"⠽",
	"⠻"
};

inline utils::LoadingIndicator::LoadingIndicator() {
	this->index = 0;
}

inline std::string utils::LoadingIndicator::render() {
	return utils::LoadingIndicator::braileSymbols[this->index] + " Processing \n";
}

inline std::string utils::LoadingIndicator::next() {
	this->index++;

	if (this->index == utils::LoadingIndicator::braileSymbols.size()) {
		this->index = 0;
	}

	return this->purge() + this->render();
}

inline std::string utils::LoadingIndicator::purge() {
	return "\033[F\33[2K\r";
}

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

/*
 * Formating nanoseconds count
 * to readable time in mcs, ms, s & etc.
 */
inline std::string utils::formatTime(std::chrono::nanoseconds t) {
	std::chrono::microseconds mcs = std::chrono::duration_cast<std::chrono::microseconds>(t);
	std::chrono::milliseconds ms = std::chrono::duration_cast<std::chrono::milliseconds>(t);
	std::chrono::seconds s = std::chrono::duration_cast<std::chrono::seconds>(t);

	// lt 10mcs
	if (mcs.count() < 10) {
		return std::to_string(t.count()) + "ns";
	}

	// lt 10ms
	if (ms.count() < 10) {
		return std::to_string(mcs.count()) + "μs";
	}

	// lt 60s
	if (s.count() < 60) {
		return std::to_string(ms.count()) + "ms";
	}

	return std::to_string(s.count()) + "s";
}
