#include <string>
#include <cstring>
#include <algorithm>
#include <vector>
#include <chrono>
#include <future>
#include "benchmark.h"
#include "utils.cpp"


/*
 * BenchmarkTest:
 */

BenchmarkTest::BenchmarkTest(std::string alias, std::vector<int> iterationsTime) {
	this->alias = alias;
	this->iterationsTime = iterationsTime;	
}

/*
 * There're all sorting algorithms below. (compareBy)
 */

std::vector<BenchmarkTest> BenchmarkTest::compareByMedian(std::vector<BenchmarkTest> tests) {
	std::sort(tests.begin(), tests.end(),
		[](const BenchmarkTest& a, const BenchmarkTest& b) -> bool {
			return a.median() < b.median();
		}
	);

	return tests;
}

std::vector<BenchmarkTest> BenchmarkTest::compareByAverage(std::vector<BenchmarkTest> tests) {
	std::sort(tests.begin(), tests.end(),
		[](const BenchmarkTest& a, const BenchmarkTest& b) -> bool {
			return a.average() < b.average();
		}
	);

	return tests;
}

std::vector<BenchmarkTest> BenchmarkTest::compareByFastest(std::vector<BenchmarkTest> tests) {
	std::sort(tests.begin(), tests.end(),
		[](const BenchmarkTest& a, const BenchmarkTest& b) -> bool {
			return a.fastest() < b.fastest();
		}
	);

	return tests;
}

std::vector<BenchmarkTest> BenchmarkTest::compareBySlowest(std::vector<BenchmarkTest> tests) {
	std::sort(tests.begin(), tests.end(),
		[](const BenchmarkTest& a, const BenchmarkTest& b) -> bool {
			return a.slowest() < b.slowest();
		}
	);

	return tests;
}

int BenchmarkTest::first() {
	return this->iterationsTime[0];
}

int BenchmarkTest::last() {
	return this->iterationsTime[this->iterationsTime.size() - 1];
}

float BenchmarkTest::median() const {
	return utils::median<int>(this->iterationsTime);
}

float BenchmarkTest::average() const {
	return utils::average<int>(this->iterationsTime);
}

int BenchmarkTest::fastest() const {
	return utils::min<int>(this->iterationsTime);
}

int BenchmarkTest::slowest() const {
	return utils::max<int>(this->iterationsTime);
}


/*
 * Benchmark:
 */

/*
 * Structures all runtime test iterations.
 */
BenchmarkTest Benchmark::test(std::string command, int iterationsCount) {
	std::vector<int> iterationsTime(iterationsCount);

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
int Benchmark::runtime(std::string command) {
	std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();

	strcat((char*)command.c_str(), " > nul");
	system(command.c_str());
	
	return (int)std::chrono::duration_cast<std::chrono::microseconds>(
		std::chrono::steady_clock::now() - begin
	).count();
}

/*
 * Async wrapper for runtime().
 */
std::future<int> Benchmark::runtimeAsync(std::string command) {
	return std::async(std::launch::async, &Benchmark::runtime, command);
}

