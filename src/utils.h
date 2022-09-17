#pragma once

#ifndef UTILS_H
#define UTILS_H

#include <vector>
#include <string>
#include <chrono>


namespace utils {
	class LoadingIndicator {
		public:
			static std::vector<std::string> braileSymbols;

			int index;

			LoadingIndicator();

			std::string render();
			std::string next();
			std::string purge();
	};

	template <typename T>
	T average(std::vector<T> v);

	unsigned long long median(std::vector<unsigned long long> v);

	template <typename T>
	T max(std::vector<T> v);

	template <typename T>
	T min(std::vector<T> v);

	std::string formatTime(std::chrono::nanoseconds t);
}

#include "utils.tcc"

#endif
