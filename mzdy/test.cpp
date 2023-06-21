#include <iostream>
#include <map>
#include <vector>
#include <set>
using namespace std;

class SalaryAnalyzer {

public:
	// prvni parametr nazev regionu, druhy set s potomky/subregiony
	void defineRegion(const string & region, const set<string> & subregions) {
		SRegion * newRegion;
		if (m_allRegions.count(region) > 0) {
			newRegion = m_allRegions[region];
		}
		else {
			newRegion = new SRegion();
		}
		for (auto regName : subregions) {
			SRegion * newSub = new SRegion();
			newRegion->m_subRegions.insert(pair<string, SRegion *>(regName, newSub));
			m_allRegions.insert(pair<string, SRegion *>(regName, newSub));
		}
		m_allRegions.insert(pair<string, SRegion*>(region, newRegion));
	};
	void print() {
		for (auto region : m_allRegions) {
			cout << region.first;
			region.second->print(1);
		}
	}
	// prida do evidence regionu jednoho cloveka s danym platem
	void addSalary(const string & region, int salary) {
		m_allRegions[region]->m_salaries.insert(salary);
	};

	// vrati prumernou mzdu ze zadanych regionu
	double averageSalary(const set<string> & regions) {
		multiset<double> * salaries = new multiset<double>;
		set<string> * visited = new set<string>;

		for (string r : regions) {
			SRegion * currReg = m_allRegions[r];
			visited->insert(r);
			for (double s : currReg->m_salaries) {
				salaries->insert(s);
			}
			currReg->salary(salaries, visited);
		}
		double sum = 0;
		for (double s : * salaries) {
			sum += s;
		}
		return sum / salaries->size();
	};
private:
	struct SRegion {
		map<string, SRegion *> m_subRegions;
		multiset<double> m_salaries;
		void print(int depth) {
			cout << " (";
			for (auto s : m_salaries) {
				cout << s << ", ";
			}
			cout << ")" << endl;
			for (auto x : m_subRegions) {
				for (int i = 0; i < depth; ++i) {
					cout << " ";
				}
				cout << x.first;
				x.second->print(depth + 1);
			}
		}
		void salary(multiset<double> * salaries, set<string> * visited) {
			for (auto subreg : m_subRegions) {
				if (visited->count(subreg.first) == 0) {
					visited->insert(subreg.first);
					for (double s : subreg.second->m_salaries) {
						salaries->insert(s);
					}
					subreg.second->salary(salaries, visited);
				}
			}

		}
	};
	map<string, SRegion *> m_allRegions;
};

int main() {
	SalaryAnalyzer sa;
	sa.defineRegion("CZ", {"Praha", "Brno", "Ostrava"});
	sa.defineRegion("Praha", {"Praha 1", "Praha 2", "Praha 3", "Dejvice"});
	sa.defineRegion("Dejvice", {"FIT", "FEL"});
	sa.addSalary("Praha", 50000);
	sa.addSalary("Praha", 100000);
	sa.addSalary("FIT", 80000);
	cout << "average " << sa.averageSalary({"CZ", "FIT", "Praha 2"}) << endl;
	cout << "average " << sa.averageSalary({"CZ"}) << endl;
	sa.defineRegion("Praha 1", {"Stare Mesto", "Josefov", "Hradcany", "Mala Strana"});
	sa.defineRegion("Praha 2", {"Nusle", "Vinohrady"});
	sa.addSalary("Nusle", 12000);
	sa.addSalary("Nusle", 13000);
	sa.addSalary("Praha 2", 15000);
	sa.print();
	cout << sa.averageSalary({"Praha 2"}) << endl;
	cout << sa.averageSalary({"Nusle"}) << endl;
	cout << sa.averageSalary({"CZ"}) << endl;
	return 0;
}