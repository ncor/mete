#pragma once

#ifndef UTILS_H
#define UTILS_H

#include <vector>
#include <string>
#include <chrono>


namespace utils {
	template <typename T>
	T average(std::vector<T> v);

    template <typename T>
	T median(std::vector<T> v);

	template <typename T>
	T max(std::vector<T> v);

	template <typename T>
	T min(std::vector<T> v);
}

#include "utils.cpp"

#endif
