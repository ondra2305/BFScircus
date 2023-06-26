#include <iostream>
#include <map>
#include <set>
#include <cassert>
#include <queue>
#include <sstream>

using namespace std;

class CTrain {
public:
	CTrain(void) {}

	~CTrain(void) {}

	void Add(istream &is) {
		string station;
		string prevSt;
		// Načteme první řádek, aby předchozí stanice nebyla prázdná v první iteraci
		getline(is, prevSt);

		// Sousední stanice jsou vždycky "vedle sebe" v grafu
		while (getline(is, station)) {
			m_Graph[station].insert(prevSt);
			m_Graph[prevSt].insert(station);
			prevSt = station;
		}
	}

	set<string> Dest(const vector<string> &stations, int &cost) {
		// Trochu nechutná struktura, nechtělo se mi dělat vlastní classu/struct
		// first = stanice, second.first = parent, second.second = time
		queue<pair<string, pair<string, int>>> toVisitFrom;
		map<string, map<string, int>> visitedFrom;
		set<string> startStations;
		set<string> res = {};

		// Pokud máme jen jednu stanici, rovnou vrátíme
		if (stations.size() == 1) {
			res.insert(stations[0]);
			cost = 0;
			return res;
		}

		// Vložím všechny startovní pozice do frony s časem 0
		for (auto &start: stations) {
			toVisitFrom.push({start, {start, 0}});
			visitedFrom.insert({start, {{start, 0}}});
			// Pomocný set startovních stanic, na vyřazení duplicitních startovních stanic
			startStations.insert(start);
		}

		while (!toVisitFrom.empty()) {
			auto current = toVisitFrom.front();
			toVisitFrom.pop();

			for (auto &neighbor: m_Graph.at(current.first)) {
				// Pokud je soused ještě nenavštívený, navštívím ho
				if (visitedFrom.count(neighbor) == 0) {
					// Vložím do fronty a visited, parent je aktuální stanice z které jedu, čas + 1
					toVisitFrom.push({neighbor, {current.second.first, current.second.second + 1}});
					visitedFrom.insert({neighbor, {{current.second.first, current.second.second + 1}}});

					// Pokud už je navštívená, zkontroluju, že to nebylo z mojí startovní stanice
				} else if (visitedFrom.at(neighbor).count(current.second.first) == 0) {
					// Vložím do fronty a visited, parent je aktuální stanice z které jedu, čas + 1
					toVisitFrom.push({neighbor, {current.second.first, current.second.second + 1}});
					visitedFrom.at(neighbor).insert({current.second.first, current.second.second + 1});
				}
			}
		}

		// Nastavíme minimální cenu na max hodnotu, meeting jako neorganizovatelný
		//cout << "Max cost is:" << cost << endl;
		int minCost = INT32_MAX;
		bool organizable = false;
		//cout << "Visited:" << endl;

		// Projedeme všechny navštívené stanice a spočítáme ceny meetu v každé
		for (auto &entry: visitedFrom) {
			int meetCost = 0;
			//cout << entry.first << ":" << endl;

			// Pokud počet nesedí, stanice není dostupná pro všechny => přeskočím
			if (entry.second.size() != startStations.size())
				continue;

			// Jinak přičítám cenu každého co celkové ceny
			for (auto &entry2: visitedFrom[entry.first]) {
				//cout << " " << entry2.first << ":" << entry2.second << endl;
				meetCost += entry2.second;
			}
			//cout << " - Meeting cost: " << meetCost << endl;
			//cout << "Min cost now: " << minCost << endl;

			// Pokud je celková cena meetingu menší než jsme viděl, je lepší, vyresetujeme a přidáme do výsledku
			if (meetCost < minCost) {
				res = {};
				res.insert(entry.first);
				minCost = meetCost;
				organizable = true;
				// Pokud je cena stejná jako předchozí nejlepší, máme dvě možné stanice kde ho uspořádat
			} else if (meetCost == minCost) {
				res.insert(entry.first);
			}
		}
		// Vrátíme nejměnší možnou cenu organizace meetingu, pokud je organizovatelný, jinak nezměníme
		// Flex ternární operátor :-)
		cost = organizable ? minCost : cost;
		return res;
	}

private:
	map<string, set<string>> m_Graph;
};

int main(void) {
	CTrain t0;
	istringstream iss;

	iss.clear();
	iss.str("1\n"
			"2\n"
			"3\n"
			"4\n"
			"5\n"
			"6\n"
			"7\n"
			"8\n");
	t0.Add(iss);
	iss.clear();
	iss.str("12\n"
			"11\n"
			"4\n"
			"9\n"
			"10\n");
	t0.Add(iss);
	iss.clear();
	iss.str("15\n"
			"11\n"
			"13\n"
			"14\n");
	t0.Add(iss);
	iss.clear();
	iss.str("7\n"
			"16\n"
			"17\n");
	t0.Add(iss);
	iss.clear();
	iss.str("20\n"
			"19\n"
			"17\n"
			"18\n");
	t0.Add(iss);
	iss.clear();
	iss.str("21\n"
			"22\n"
			"23\n"
			"25\n"
			"24\n"
			"21\n");
	t0.Add(iss);
	iss.clear();
	iss.str("26\n"
			"27\n"
			"28\n");
	t0.Add(iss);
	iss.clear();
	iss.str("27\n"
			"29\n"
			"30\n");
	t0.Add(iss);

	int cost = 5381;
	assert (t0.Dest({"2", "5", "11"}, cost) == set<string>({"4"}) && cost == 4);
	assert (t0.Dest({"12", "10"}, cost) == set<string>({"10", "9", "4", "11", "12"}) && cost == 4);
	assert (t0.Dest({"2", "9", "19"}, cost) == set<string>({"4"}) && cost == 9);
	assert (t0.Dest({"15", "12", "13"}, cost) == set<string>({"11"}) && cost == 3);
	cost = 5381;
	assert (t0.Dest({"21", "26"}, cost) == set<string>() && cost == 5381);
	cost = 1256;
	assert (t0.Dest({"10", "28"}, cost) == set<string>() && cost == 1256);
	assert (t0.Dest({"21", "25"}, cost) == set<string>({"21", "24", "25"}) && cost == 2);
	assert (t0.Dest({"21", "21"}, cost) == set<string>({"21"}) && cost == 0);
	assert (t0.Dest({"23", "21"}, cost) == set<string>({"21", "22", "23"}) && cost == 2);
	assert (t0.Dest({"12", "20"}, cost) == set<string>({"12", "11", "4", "5", "6", "7", "16", "17", "19", "20"}) &&
			cost == 9);

	assert (t0.Dest({"50"}, cost) == set<string>({"50"}) && cost == 0);
	assert (t0.Dest({"13", "10"}, cost) == set<string>({"10", "11", "13", "4", "9"}) && cost == 4);
	assert (t0.Dest({"17", "19"}, cost) == set<string>({"17", "19"}) && cost == 1);
	cost = 1999;
	assert (t0.Dest({"29", "25"}, cost) == set<string>() && cost == 1999);
	assert (t0.Dest({"8", "5"}, cost) == set<string>({"5", "6", "7", "8"}) && cost == 3);
	assert (t0.Dest({"4", "4"}, cost) == set<string>({"4"}) && cost == 0);

	cout << "ALL ASSERTS PASSED!" << endl;
	return 0;
}