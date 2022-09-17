#include <string>
#include <cstring>
#include <algorithm>
#include <vector>
#include <chrono>
#include <future>
#include "benchmark.h"
#include "utils.h"


/*
 * BenchmarkTest:
 */

BenchmarkTest::BenchmarkTest(std::string alias, std::vector<std::chrono::nanoseconds> iterationsTime) {
	this->alias = alias;
	this->iterationsTime = iterationsTime;
	this->realtimeIterations = this->computeRealtimeIterations();

	this->first = this->iterationsTime[0];
	this->last = this->iterationsTime[this->iterationsTime.size() - 1];
	this->median = this->computeMedian();
	this->average = this->computeAverage();
	this->fastest = this->computeFastest();
	this->slowest = this->computeSlowest();
}

/*
 * There're all sorting algorithms below. (compareBy)
 */

std::vector<BenchmarkTest> BenchmarkTest::compareByMedian(std::vector<BenchmarkTest> tests) {
	std::sort(tests.begin(), tests.end(),
		[](const BenchmarkTest& a, const BenchmarkTest& b) -> bool {
			return a.median.count() < b.median.count();
		}
	);

	return tests;
}

std::vector<BenchmarkTest> BenchmarkTest::compareByAverage(std::vector<BenchmarkTest> tests) {
	std::sort(tests.begin(), tests.end(),
		[](const BenchmarkTest& a, const BenchmarkTest& b) -> bool {
			return a.average.count() < b.average.count();
		}
	);

	return tests;
}

std::vector<BenchmarkTest> BenchmarkTest::compareByFastest(std::vector<BenchmarkTest> tests) {
	std::sort(tests.begin(), tests.end(),
		[](const BenchmarkTest& a, const BenchmarkTest& b) -> bool {
			return a.fastest.count() < b.fastest.count();
		}
	);

	return tests;
}

std::vector<BenchmarkTest> BenchmarkTest::compareBySlowest(std::vector<BenchmarkTest> tests) {
	std::sort(tests.begin(), tests.end(),
		[](const BenchmarkTest& a, const BenchmarkTest& b) -> bool {
			return a.slowest.count() < b.slowest.count();
		}
	);

	return tests;
}

std::vector<unsigned long long> BenchmarkTest::computeRealtimeIterations() {
	std::vector<unsigned long long> realtimeIterations;

	for (int i = 0; i < this->iterationsTime.size(); i++) {
		realtimeIterations.push_back(this->iterationsTime[i].count());
	}

	return realtimeIterations;
}

std::chrono::nanoseconds BenchmarkTest::computeMedian() {
	return std::chrono::nanoseconds(utils::median<unsigned long long>(this->realtimeIterations));
}

std::chrono::nanoseconds BenchmarkTest::computeAverage() {
	return std::chrono::nanoseconds(utils::average<unsigned long long>(this->realtimeIterations));
}

std::chrono::nanoseconds BenchmarkTest::computeFastest() {
	return std::chrono::nanoseconds(utils::min<unsigned long long>(this->realtimeIterations));
}

std::chrono::nanoseconds BenchmarkTest::computeSlowest() {
	return std::chrono::nanoseconds(utils::max<unsigned long long>(this->realtimeIterations));
}


/*
 * Benchmark:
 */

/*
 * Structures all runtime test iterations.
 */
BenchmarkTest Benchmark::test(std::string command, int iterationsCount) {
	std::vector<std::chrono::nanoseconds> iterationsTime(iterationsCount);

	for (int i = 0; i < iterationsCount; i++) {
		iterationsTime[i] = Benchmark::runtime(command);
	}

	return *(new BenchmarkTest(command, iterationsTime));
}

BenchmarkTest Benchmark::test(std::string command) {
	return Benchmark::test(command, 1);
}

/*
 * Async wrapper for benchmark().
 */
std::future<BenchmarkTest> Benchmark::testAsync(std::string command, int iterationsCount) {
	return std::async(std::launch::async, [ command, iterationsCount ]() {
		return Benchmark::test(command, iterationsCount);
	});
}

std::future<BenchmarkTest> Benchmark::testAsync(std::string command) {
	return Benchmark::testAsync(command, 1);
}

/*
 * Runs shell command and measures execution time.
 */
std::chrono::nanoseconds Benchmark::runtime(std::string command) {
	std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();

	strcat((char*)command.c_str(), " > nul");
	system(command.c_str());
	
	return std::chrono::duration_cast<std::chrono::nanoseconds>(
		std::chrono::steady_clock::now() - begin
	);
}

/*
 * Async wrapper for runtime().
 */
std::future<std::chrono::nanoseconds> Benchmark::runtimeAsync(std::string command) {
	return std::async(std::launch::async, &Benchmark::runtime, command);
}

