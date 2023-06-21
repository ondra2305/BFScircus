#include <iostream>
#include <string>
#include <set>
#include <unordered_set>
#include <map>
#include <unordered_map>
#include <queue>
#include <deque>
#include <stack>
#include <vector>
#include <list>
#include <cassert>

using namespace std;

class SalaryAnalyzer {
public:
	void defineRegion(const string &region, const set<string> &subregions);

	void addSalary(const string &region, int salary);

	double averageSalary(const set<string> &regions);

private:
	map<string, set<string>> graph;
	map<string, vector<int>> salaries;
};

void SalaryAnalyzer::defineRegion(const string &region, const set<string> &subregions) {
	graph[region] = subregions;
	for (auto &subregion: subregions) {
		graph[subregion] = {};
	}
	salaries[region] = {};
}

void SalaryAnalyzer::addSalary(const string &region, int salary) {
	salaries[region].push_back(salary);
}

double SalaryAnalyzer::averageSalary(const set<string> &regions) {
	queue<string> toVisit;
	set<string> visited;
	vector<int> toAnalyze;

	for (auto &region: regions) {
		if (graph.count(region) == 0) {
			throw invalid_argument("Neplatny region!!");
		}
		toVisit.push(region);
		visited.insert(region);
	}

	while (!toVisit.empty()) {
		auto current = toVisit.front();
		toVisit.pop();

		if (!salaries[current].empty()) {
			toAnalyze.insert(toAnalyze.begin(), salaries[current].begin(), salaries[current].end());
		}

		for (auto &neighbor: graph.at(current)) {
			if (visited.count(neighbor) == 0) {
				toVisit.push(neighbor);
				visited.insert(neighbor);
			}
		}
	}

	double sum = 0;
	int count = 0;
	//cout << "Salaries found:" << endl;
	for (auto &salary: toAnalyze) {
		//cout << salary << ", ";
		sum += salary;
		count++;
	}
	//cout << endl;
	if (count == 0) {
		return 0;
	}

	return sum / count;
}

int main() {
	SalaryAnalyzer sa;

	sa.defineRegion("CZ", {"Praha", "Brno", "Ostrava"});
	sa.defineRegion("Praha", {"Praha 1", "Praha 2", "Praha 3", "Dejvice"});
	sa.defineRegion("Dejvice", {"FIT", "FEL"});
	sa.addSalary("Praha", 50000);
	sa.addSalary("Praha", 100000);
	sa.addSalary("FIT", 80000);

	double averageSalary1 = sa.averageSalary({"CZ", "FIT", "Praha 2"});
	cout << "average " << averageSalary1 << endl;
	assert(averageSalary1 == (50000 + 100000 + 80000) / 3.0);
	double averageSalary2 = sa.averageSalary({"CZ"});
	cout << "average " << averageSalary2 << endl;
	assert(averageSalary2 == (50000 + 100000 + 80000) / 3.0);

	sa.defineRegion("Praha 1", {"Stare Mesto", "Josefov", "Hradcany", "Mala Strana"});
	sa.defineRegion("Praha 2", {"Nusle", "Vinohrady"});
	sa.addSalary("Nusle", 12000);
	sa.addSalary("Nusle", 13000);
	sa.addSalary("Praha 2", 15000);

	double averageSalary3 = sa.averageSalary({"Praha 2"});
	cout << averageSalary3 << endl;
	assert(averageSalary3 == (12000 + 13000 + 15000) / 3.0);
	double averageSalary4 = sa.averageSalary({"Nusle"});
	cout << averageSalary4 << endl;
	assert(averageSalary4 == (12000 + 13000) / 2.0);
	double averageSalary5 = sa.averageSalary({"CZ"});
	cout << averageSalary5 << endl;
	assert(averageSalary5 == (50000 + 100000 + 80000 + 12000 + 13000 + 15000) / 6.0);

	assert(sa.averageSalary({"CZ", "CZ", "FIT", "Praha 2"}) == sa.averageSalary({"CZ", "FIT", "Praha 2"}));

	sa.defineRegion("RegionNoSalary", {"Subregion1", "Subregion2"});
	sa.addSalary("Subregion1", 50000);
	sa.addSalary("Subregion2", 60000);
	assert(sa.averageSalary({"RegionNoSalary"}) == (50000 + 60000) / 2.0);

	sa.defineRegion("MultiLevelRegion", {"Level1"});
	sa.defineRegion("Level1", {"Level2"});
	sa.defineRegion("Level2", {"Level3"});
	sa.addSalary("Level3", 80000);
	assert(sa.averageSalary({"MultiLevelRegion"}) == 80000);

	sa.addSalary("LateDefinedRegion", 100000);
	sa.defineRegion("RegionWithLateDefinedSubregion", {"LateDefinedRegion"});
	assert(sa.averageSalary({"RegionWithLateDefinedSubregion"}) == 100000);

	sa.defineRegion("SK", {"Bratislava", "Kosice", "Trencin", "Zilina"});
	assert(sa.averageSalary({"Kosice"}) == 0);

	try {
		sa.averageSalary({"DE"});
		assert("Exception not thrown!" == nullptr);
	} catch (const invalid_argument &e) {
	} catch (...) {
		assert("Invalid exception thrown!" == nullptr);
	}

	cout << "ALL TESTS PASSED!" << endl;
	return 0;
}
