#include <string>
#include <vector>
#include <chrono>
#include "utils.h"


inline std::string format::style::wrap(std::string text, std::vector<std::string> styles) {
    std::string wrapped = "";

    for (std::string s: styles) {
        wrapped += s;
    };

    return wrapped + text + format::style::reset;
}


/*
 * ProgressBar:
 */

inline format::ProgressBar::ProgressBar(std::vector<std::string> indicators, std::string alias) {
    this->indicators = indicators;
    this->alias = alias;
    this->indicatorIndex = 0;
};

inline void format::ProgressBar::setAlias(std::string alias) {
    this->alias = alias;
}

inline std::string format::ProgressBar::render() {
    return this->indicators[this->indicatorIndex] + " " + this->alias + "\n";
};

inline std::string format::ProgressBar::update() {
    this->indicatorIndex++;

    if (this->indicatorIndex == this->indicators.size()) {
        this->indicatorIndex = 0;
    }

    return this->purge() + this->render();
};

inline std::string format::ProgressBar::purge() {
    return format::prevLine + format::clearLine;
};


/*
 * Table:
 */

inline format::Table::Table() {
    this->rows = tableRows{};
}

inline format::Table::Table(tableRows rows) {
    this->rows = rows;
}

inline void format::Table::addRow(tableRow row) {
    this->rows.push_back(row);
}

inline void format::Table::addRows(tableRows rows) {
    for (tableRow row: rows) {
        this->addRow(row);
    }
}

inline void format::Table::extendRow(int rowIndex, tableRow elements) {
    for (tableElement element: elements) {
        this->rows[rowIndex].push_back(element);
    }
}

inline std::string format::Table::render() {
    // TODO: Implement
    std::string render = "";

    std::vector<int> rowElementsCounts;
    int columnsCount;

    for (tableRow row: this->rows) {
        rowElementsCounts.push_back(row.size());
    }

    columnsCount = utils::max<int>(rowElementsCounts);

    std::vector<int> columnsWidth(columnsCount);

    for (int i = 0; i < columnsCount; i++) {
        std::vector<int> widths;

        for (tableRow row: this->rows) {
            if (i < row.size()) {
                widths.push_back(row[i].first.length());
            }
        }

        columnsWidth[i] = utils::max<int>(widths);
    }

    for (int i = 0; i < this->rows.size(); i++) {
        tableRow row = this->rows[i];
        std::vector<std::string> rowRenderLines(2);
        std::string terminationLine = "";
        bool lastRow = (i + 1) == this->rows.size();

        rowRenderLines[1] += "│";

        if (i) {
            rowRenderLines[0] += "├";
        } else {
            rowRenderLines[0] += "┌";
        }

        if (lastRow) {
            terminationLine += "└";
        }

        for (int j = 0; j < columnsCount; j++) {
            int columnWidth = columnsWidth[j];
            
            for (int l = 0; l < (columnWidth + 2); l++) {
                rowRenderLines[0] += "─";
                if (lastRow) {
                    terminationLine += "─";
                }
            }

            if ((j + 1) == columnsCount) {
                if (i) {
                    rowRenderLines[0] += "┤";
                } else {
                    rowRenderLines[0] += "┐";
                }

                if (lastRow) {
                    terminationLine += "┘";
                }
            } else {
                if (i) {
                    rowRenderLines[0] += "┼";
                } else {
                    rowRenderLines[0] += "┬";
                }

                if (lastRow) {
                    terminationLine += "┴";
                }
            }

            rowRenderLines[1] += " ";

            if (j < row.size()) {
                tableElement element = row[j];

                rowRenderLines[1] += element.second;

                for (int l = 0; l < (columnWidth - element.first.length()); l++) {
                    rowRenderLines[1] += " ";
                }
            } else {
                for (int l = 0; l < columnWidth; l++) {
                    rowRenderLines[1] += " ";
                }
            }

            rowRenderLines[1] += " │";
        }

        render += rowRenderLines[0] + '\n' + rowRenderLines[1] + '\n' + terminationLine;
    }

    return render;
}


inline std::string format::time(std::chrono::nanoseconds t) {
    std::chrono::microseconds mcs = std::chrono::duration_cast<std::chrono::microseconds>(t);
	std::chrono::milliseconds ms = std::chrono::duration_cast<std::chrono::milliseconds>(t);
	std::chrono::seconds s = std::chrono::duration_cast<std::chrono::seconds>(t);

	// lt 10mcs
	if (mcs.count() < 10) {
		return std::to_string(t.count()) + "ns";
	}

	// lt 10ms
	if (ms.count() < 10) {
		return std::to_string(mcs.count()) + "mcs";
	}

	// lt 60s
	if (s.count() < 10) {
		return std::to_string(ms.count()) + "ms";
	}

	return std::to_string(s.count()) + "s";
}
