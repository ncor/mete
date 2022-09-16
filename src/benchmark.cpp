#include "utils.cpp"
#include <string>
#include <algorithm>
#include <vector>
#include <chrono>
#include <future>


class BenchmarkTest {
	public:
		std::string alias;
		std::vector<int> iterationsTime;

		BenchmarkTest(std::string alias, std::vector<int> iterationsTime) {
			alias = alias;
			iterationsTime = iterationsTime;	
		}
		
		/*
		 * There're all sorting algorithms below. (compareBy)
		 */

		static std::vector<BenchmarkTest> compareByMedian(std::vector<BenchmarkTest> tests) {
			std::sort(tests.begin(), tests.end(),
				[](const BenchmarkTest& a, const BenchmarkTest& b) -> bool {
					return a.median() < b.median();
				}
			);

			return tests;
		}

		static std::vector<BenchmarkTest> compareByAverage(std::vector<BenchmarkTest> tests) {
			std::sort(tests.begin(), tests.end(),
				[](const BenchmarkTest& a, const BenchmarkTest& b) -> bool {
					return a.average() < b.average();
				}
			);

			return tests;
		}

		static std::vector<BenchmarkTest> compareByFastest(std::vector<BenchmarkTest> tests) {
			std::sort(tests.begin(), tests.end(),
				[](const BenchmarkTest& a, const BenchmarkTest& b) -> bool {
					return a.fastest() < b.fastest();
				}
			);

			return tests;
		}

		static std::vector<BenchmarkTest> compareBySlowest(std::vector<BenchmarkTest> tests) {
			std::sort(tests.begin(), tests.end(),
				[](const BenchmarkTest& a, const BenchmarkTest& b) -> bool {
					return a.slowest() < b.slowest();
				}
			);

			return tests;
		}

		int first() {
			return this->iterationsTime[0];
		}

		int last() {
			return this->iterationsTime[this->iterationsTime.size() - 1];
		}
		
		float median() const {
			return utils::median<int>(this->iterationsTime);
		}

		float average() const {
			return utils::average<int>(this->iterationsTime);
		}

		int fastest() const {
			return utils::max<int>(this->iterationsTime);
		}

		int slowest() const {
			return utils::min<int>(this->iterationsTime);
		}
};

class Benchmark {
	public:
		/*
		 * Structures all runtime test iterations.
		 */
		static BenchmarkTest test(std::string command, int iterationsCount) {
			std::vector<int> iterationsTime(iterationsCount);

			for (int i = 0; i < iterationsCount; i++) {
				iterationsTime[i] = Benchmark::runtime(command);
			}

			return *(new BenchmarkTest(command, iterationsTime));
		}

		static BenchmarkTest test(std::string command) {
			return Benchmark::test(command, 1);
		}

		/*
		 * Async wrapper for benchmark().
		 */
		static std::future<BenchmarkTest> testAsync(std::string command, int iterationsCount) {
			return std::async(std::launch::async, [ command, iterationsCount ]() {
				return Benchmark::test(command, iterationsCount);
			});
		}

		std::future<BenchmarkTest> testAsync(std::string command) {
			return Benchmark::testAsync(command, 1);
		}
	private:
		/*
		 * Runs shell command and measures execution time.
		 */
		static int runtime(std::string command) {
			std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
			system(command.c_str());
			
			return (int)std::chrono::duration_cast<std::chrono::microseconds>(
				std::chrono::steady_clock::now() - begin
			).count();
		}
	
		/*
		 * Async wrapper for runtime().
		 */
		static std::future<int> runtimeAsync(std::string command) {
			return std::async(std::launch::async, &Benchmark::runtime, command);
		}
};

