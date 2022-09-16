#include <string>
#include <vector>
#include <future>


class BenchmarkTest {
	public:
		std::string alias;
		std::vector<int> iterationsTime;

		BenchmarkTest(std::string alias, std::vector<int> iterationsTime);

		static std::vector<BenchmarkTest> compareByMedian(std::vector<BenchmarkTest> tests);
		static std::vector<BenchmarkTest> compareByAverage(std::vector<BenchmarkTest> tests);
		static std::vector<BenchmarkTest> compareByFastest(std::vector<BenchmarkTest> tests);
		static std::vector<BenchmarkTest> compareBySlowest(std::vector<BenchmarkTest> tests);

		float median();
		float average();
		float fastest();
		float slowest();
};

class Benchmark {
	public:
		static BenchmarkTest test(std::string command, int iterationsCount);
		static BenchmarkTest test(std::string command);
		static std::future<BenchmarkTest> testAsync(std::string command, int iterationsCount);
		static std::future<BenchmarkTest> testAsync(std::string command);
	private:
		static int runtime(std::string command);
		static std::future<int> runtimeAsync(std::string command);
};

