#ifndef __PROGTEST__

#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <cstring>
#include <cassert>
#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <list>
#include <map>
#include <set>
#include <unordered_map>
#include <unordered_set>
#include <algorithm>
#include <deque>
#include <memory>
#include <functional>
#include <numeric>
#include <queue>

using namespace std;

class NoDataException {
};

namespace STUDENT_NAMESPACE {
#endif /* __PROGTEST__ */

	class CAnalyzer {
	public:
		CAnalyzer &DefineCategory(const string &name, const list<string> &subCat);

		CAnalyzer &Add(const string &category, int salary);

		double Median(const list<string> &categories) const;

	private:
		map<string, set<string>> graph;
		map<string, vector<int>> salaries;
	};

	CAnalyzer &CAnalyzer::DefineCategory(const string &name, const list<string> &subCat) {
		for (auto &cat: subCat) {
			graph[name].insert(cat);
			graph[cat] = {};
		}
		salaries[name] = {};
		return *this;
	}

	CAnalyzer &CAnalyzer::Add(const string &category, int salary) {
		salaries[category].push_back(salary);
		return *this;
	}

	double CAnalyzer::Median(const list<string> &categories) const {
		queue<string> toVisit;
		set<string> visited;
		vector<int> toAnalyze;
		int count = 0;

		for (auto &category: categories) {
			if (graph.count(category) == 0) {
				throw NoDataException();
			}
			toVisit.push(category);
			visited.insert(category);
		}

		while (!toVisit.empty()) {
			auto current = toVisit.front();
			toVisit.pop();

			if (salaries.count(current) && !salaries.at(current).empty()) {
				toAnalyze.insert(toAnalyze.begin(), salaries.at(current).begin(), salaries.at(current).end());
				count += (int) salaries.at(current).size();
			}

			for (auto &neighbor: graph.at(current)) {
				if (visited.count(neighbor) == 0) {
					toVisit.push(neighbor);
					visited.insert(neighbor);
				}
			}
		}
		if (count == 0) {
			throw NoDataException();
		}

		double median;
		sort(toAnalyze.begin(), toAnalyze.end());
		cout << "Salaries found:" << endl;
		for (auto &salary: toAnalyze) {
			cout << salary << ", ";
		}
		cout << endl;

		if (count % 2 != 0) {
			median = toAnalyze[count / 2];
		} else {
			median = (double) (toAnalyze[(count / 2) - 1] + toAnalyze[count / 2]) / 2;
		}

		cout << "Median:" << median << endl;
		return median;
	}

#ifndef __PROGTEST__
} // namespace
int main(void) {
	using namespace STUDENT_NAMESPACE;
	CAnalyzer a;
	a.DefineCategory("EU", {"CZ", "Germany", "Slovakia"});
	a.DefineCategory("CZ", {"Praha", "Brno", "Ostrava"});
	a.DefineCategory("Praha 6", {"Dejvice", "Vokovice", "Ruzyne"});
	a.DefineCategory("Praha", {"Praha 1", "Praha 2", "Praha 3", "Praha 4", "Praha 5", "Praha 6"});
	a.DefineCategory("Germany", {"East Germany", "West Germany"});
	a.DefineCategory("East Germany", {"Berlin", "Dresden", "Chemnitz"});
	a.DefineCategory("West Germany", {"Hamburg", "Munich"});
	a.DefineCategory("Slovakia", {"Bratislava", "Kosice"});
	a.Add("Praha", 5000);
	a.Add("Praha", 10000);
	assert(a.Median({"Praha"}) == 7500);

	a.Add("Praha", 7000);
	assert(a.Median({"Praha"}) == 7000);
	a.Add("Praha 2", 1000);
	a.Add("Praha 3", 4000);
	assert(a.Median({"Praha 2", "Praha 3"}) == 2500);
	a.Add("Bratislava", 11);
	a.Add("Kosice", 2);
	assert(a.Median({"Slovakia"}) == 6.5);

	try {
		a.Median({"Berlin", "Dresden"});
		abort();
	}
	catch (const NoDataException &x) {}

	assert(a.Median({"EU"}) == 4000);
	a.DefineCategory("US", {"New York", "Ohio", "Texas"});
	a.Add("New York", 3000);
	a.Add("US", 2000);
	a.Add("US", 1500);
	assert(a.Median({"EU", "US"}) == 2500);
	return 0;
}

#endif /* __PROGTEST__ */