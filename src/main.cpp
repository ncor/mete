#include <iostream>
#include <string>
#include <chrono>
#include "benchmark.h"
#include "utils.h"
#include "format.h"


enum Comparison {
    Total,
    Average,
    Median,
    Fastest,
    Slowest
};

BenchmarkTest runBenchmark(std::string command, int id, int iterationsCount) {
    std::cout << format::style::wrap(
            "Benchmark #" + std::to_string(id) + ":",
            std::vector<std::string> {
                format::style::decoration::bold
            }
        )
        << " "
        << command
        << std::endl;
	
	std::future<BenchmarkTest> test = Benchmark::testAsync(id, command, iterationsCount);
	format::ProgressBar* progressBar = new format::ProgressBar(format::symbols::braileCycled, "Working");

    std::cout << progressBar->render();

	while (test.wait_for(std::chrono::milliseconds(100)) != std::future_status::ready)
		std::cout << progressBar->update();

	std::cout << progressBar->purge();
	test.wait();

	BenchmarkTest results = test.get();
	
    std::cout
        << "  Total time:\t\t± "
        << format::style::wrap(
            format::time(results.total),
            std::vector<std::string>{ format::style::decoration::bold, format::style::colors::text::intense::yellow }
        );

	if (iterationsCount > 1) {
        std::cout
            << format::style::wrap(
                " " + std::to_string(iterationsCount) + " run(s)",
                std::vector<std::string>{ format::style::colors::text::intense::black }
            )
            << std::endl;

        std::cout
            << "  Average:\t\t"
            << format::style::wrap(
                format::time(results.average),
                std::vector<std::string>{ format::style::decoration::bold, format::style::colors::text::intense::yellow }
            )
            << std::endl;

        std::cout
            << "  Range ("
            << format::style::wrap(
                "min",
                std::vector<std::string>{ format::style::decoration::bold, format::style::colors::text::intense::cyan }
            )
            << " … "
            << format::style::wrap(
                "max",
                std::vector<std::string>{ format::style::decoration::bold, format::style::colors::text::intense::red }
            )
            << "):\t"
            << format::style::wrap(
                format::time(results.fastest),
                std::vector<std::string>{ format::style::decoration::bold, format::style::colors::text::intense::cyan }
            )
            << " … "
            << format::style::wrap(
                format::time(results.slowest),
                std::vector<std::string>{ format::style::decoration::bold, format::style::colors::text::intense::red }
            )
            << std::endl;
	} else {
        std::cout << std::endl;
    }

	return results;
}

std::string compareTable(int compareType, std::vector<BenchmarkTest> results, std::string rowPrefix) {
    format::Table* table = new format::Table();

    for (int i = 0; i < results.size(); i++) {
        BenchmarkTest result = results[i];
        std::string timeColor = format::style::colors::text::intense::yellow;
        std::string displayTime;

        if (compareType == Comparison::Total) {
            displayTime = format::time(result.total);
        } else if (compareType == Comparison::Average) {
            displayTime = format::time(result.average);
        } else if (compareType == Comparison::Median) {
            displayTime = format::time(result.median);
        } else if (compareType == Comparison::Fastest) {
            displayTime = format::time(result.fastest);
        } else if (compareType == Comparison::Slowest) {
            displayTime = format::time(result.slowest);
        }

        if ((i + 1) == results.size())
            timeColor = format::style::colors::text::intense::red;
        if (!i)
            timeColor = format::style::colors::text::cyan;

        table->addRow(
            tableRow {
                tableElement {
                    std::to_string(i + 1),
                    std::to_string(i + 1)
                },
                tableElement {
                    result.alias,
                    result.alias
                },
                tableElement {
                    displayTime,
                    format::style::wrap(
                        displayTime,
                        std::vector<std::string> {
                            format::style::decoration::bold,
                            timeColor
                        }
                    )
                }
            }
        );
    }

    return table->render(rowPrefix) + "\n";
}

std::string compareTable(int compareType, std::vector<BenchmarkTest> results) {
    return compareTable(compareType, results, "");
}


int main(int argc, char* argv[]) {
    std::vector<BenchmarkTest> results;
    int iterationsCount = 1;
	int id = 1;

	for (int i = 1; i < argc; i++) {
		std::string arg = std::string(argv[i]);

		if (arg[0] == '-' && (i + 1) != argc) {
			std::string numberPart = arg.substr(1, arg.length());

			if (utils::isNumber(numberPart))
				iterationsCount = std::stoi(numberPart);

			continue;
		} else {
			results.push_back(runBenchmark(arg, id, iterationsCount));
            
            if ((i + 1) != argc) std::cout << std::endl;

			iterationsCount = 1;
			id++;
		}
	}

    if (results.size() > 1) {
        std::cout
            << format::style::wrap(
                "\nSummary\n",
                std::vector<std::string> {
                    format::style::decoration::bold
                }
            )
            << compareTable(Comparison::Average, BenchmarkTest::compareByAverage(results), "  ");
    }

	return 0;
}
