#pragma once

#include <string>
#include <vector>
#include <future>
#include <chrono>


class BenchmarkTest {
	public:
		std::string alias;
		std::vector<std::chrono::nanoseconds> iterationsTime;
		std::vector<unsigned long long> realtimeIterations;

		std::chrono::nanoseconds first;
		std::chrono::nanoseconds last;
		std::chrono::nanoseconds median;
		std::chrono::nanoseconds average;
		std::chrono::nanoseconds fastest;
		std::chrono::nanoseconds slowest;

		BenchmarkTest(std::string alias, std::vector<std::chrono::nanoseconds> iterationsTime);

		static std::vector<BenchmarkTest> compareByMedian(std::vector<BenchmarkTest> tests);
		static std::vector<BenchmarkTest> compareByAverage(std::vector<BenchmarkTest> tests);
		static std::vector<BenchmarkTest> compareByFastest(std::vector<BenchmarkTest> tests);
		static std::vector<BenchmarkTest> compareBySlowest(std::vector<BenchmarkTest> tests);
		
	private:
		std::vector<unsigned long long> computeRealtimeIterations();
		std::chrono::nanoseconds computeMedian();
		std::chrono::nanoseconds computeAverage();
		std::chrono::nanoseconds computeFastest();
		std::chrono::nanoseconds computeSlowest();
};

class Benchmark {
	public:
		static BenchmarkTest test(std::string command, int iterationsCount);
		static BenchmarkTest test(std::string command);
		static std::future<BenchmarkTest> testAsync(std::string command, int iterationsCount);
		static std::future<BenchmarkTest> testAsync(std::string command);
	private:
		static std::chrono::nanoseconds runtime(std::string command);
		static std::future<std::chrono::nanoseconds> runtimeAsync(std::string command);
};

