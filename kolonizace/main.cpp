#ifndef __PROGTEST__
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cctype>
#include <cassert>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <stdexcept>
#include <string>
#include <list>
#include <vector>
#include <stack>
#include <queue>
#include <deque>
#include <map>
#include <unordered_map>
#include <set>
#include <unordered_set>
#include <algorithm>
#include <memory>
using namespace std;

class Cbase
{
public:
	Cbase(string sector, string species, int when) : m_Sector(std::move(sector)), m_Species(std::move(species)), m_When(when) {}
	~Cbase() = default;

	string m_Sector;
	string m_Species;
	int m_When;
};

class CPlanet
{
public:
	CPlanet(string species, int time, bool isValid, int delay)
			: m_species(species),m_time(time), m_valid(isValid), m_delay(delay) {}
	~CPlanet() = default;

	string m_species;
	int m_time;
	bool m_valid;
	int m_delay;
};

#endif /* __PROGTEST__ */

// Netestováno na PT, takže nemůžu zaručit 100% správnost, 10b by to ale snad mělo dát
class CUniverse
{
public:
	CUniverse &addConnection(const string &from, const string &to)
	{
		// Červí díry jsou obousměrné, jinak je graf klasický
		m_Graph[from].insert(to);
		m_Graph[to].insert(from);
		return *this;
	}

	CUniverse &optimize()
	{
		// LMAO, jsem rád že jsem to zprovoznil bez toho
		return *this;
	}

	map<string, string> colonise(const vector<Cbase> &bases) const
	{
		map<string, string> res = {};
		queue<pair<string, CPlanet>> toColonise;
		unordered_map<string, CPlanet> visited;

		// Přidám všechny druhy na jejich počáteční planety v čase 0 do fronty
		for (auto &base : bases)
		{
			// Pokud v grafu nemám žádné červí díry od počáteční planety civilzace se nemá kam rozšířit => přidám do výsledku
			if (m_Graph.count(base.m_Sector) == 0)
			{
				res.insert({base.m_Sector, base.m_Species});
				continue;
			}
			toColonise.push({base.m_Sector, {base.m_Species, 0, true, base.m_When}});
			visited.insert({base.m_Sector, {base.m_Species, 0, true, base.m_When}});
		}

		while (!toColonise.empty())
		{
			auto current = toColonise.front();
			toColonise.pop();
			/*
			cout << "Current:" << current.first << "," << current.second.m_species << "," << current.second.m_time << endl;
			cout << "Time: " << current.second.m_time << endl;

			for(auto &planet : visited) {
				if(planet.second.m_valid) {
					cout << "VALID:" << planet.first << ":" << planet.second.m_species << ":" << planet.second.m_time << endl;
				} else {
					cout << "INVALID:" << planet.first << ":" << planet.second.m_species << ":" << planet.second.m_time << endl;
				}
			}
			cout << "*******************" << endl;
			*/

			// Pokud je okupace aktuální planety neplatná, přeskočit "celou větev"
			if (visited.count(current.first) != 0 && !visited.at(current.first).m_valid)
			{
				continue;
			}
				// Pokud je civilizace ještě nevyvinutá, přidat do queue s časem + 1
			else if (current.second.m_delay > current.second.m_time)
			{
				// cout << "DELAYED:" << current.second.m_species << ";" << current.second.m_delay << ">" << current.second.m_time << endl;
				toColonise.push({current.first, {current.second.m_species, current.second.m_time + 1, true, current.second.m_delay}});
				continue;
			}

			// Procházíme všechny okolní planety aktuální
			for (auto &neighbor : m_Graph.at(current.first))
			{
				// Pokud je planeta nenavštívená => první kolonizace, přidám normálně do queue a do visited jako validní kolonizaci
				if (visited.count(neighbor) == 0)
				{
					toColonise.push({neighbor, {current.second.m_species, current.second.m_time + 1, true, 0}});
					visited.insert({neighbor, {current.second.m_species, current.second.m_time + 1, true, 0}});
				}
					// Pokud už byla planeta kolonizována v nižším čase jiným druhem přeskočím tuhle planetu
				else if (visited.at(neighbor).m_time < current.second.m_time + 1)
				{
					continue;

				}
					// Pokud je aktuální čas stejný jako nějaké předchozí kolonizace => kolonizace je neplatná, planeta zůstane prázdná
				else if (visited.at(neighbor).m_time == current.second.m_time + 1)
				{
					// cout << "CONFLICT!!! At:" << neighbor << endl;
					visited.at(neighbor).m_valid = false;
				}
			}
		}
		// Projdu všechny kolonizované (navštívené) planety, pokud má nějaké neplatnou kolonizaci, nepřidám jí do výsledku
		for (auto &planet : visited)
		{
			if (planet.second.m_valid)
			{
				res.insert({planet.first, planet.second.m_species});
			}
			else
			{
				//cout << "INVALID:" << planet.first << ":" << planet.second.m_species << ":" << planet.second.m_time << endl;
			}
		}
		cout << "*******************" << endl;
		cout << "Results:" << endl;
		for (auto &result : res)
		{
			cout << result.first << ":" << result.second << endl;
		}
		cout << "~~~~~~~~~~~~~~~~~~~" << endl;
		return res;
	}

private:
	// Unordered containery mají "konstatní" složitost místo logaritmické
	unordered_map<string, unordered_set<string>> m_Graph;
};

#ifndef __PROGTEST__

int main(void)
{
	CUniverse u1;

	u1.addConnection("Earth", "X1")
			.addConnection("Earth", "Y1")
			.addConnection("Vulcan", "X1")
			.addConnection("Vulcan", "Y2")
			.addConnection("Y2", "Y1")
			.addConnection("Kronos", "X1")
			.addConnection("X1", "X2")
			.addConnection("X2", "X3")
			.optimize();

	auto r1 = u1.colonise({{"Earth", "Humans", 0}, {"Vulcan", "Vulcans", 0}, {"Kronos", "Clingons", 0}});
	assert(r1 == (map<string, string>({{"Earth", "Humans"}, {"Y1", "Humans"}, {"Vulcan", "Vulcans"}, {"Y2", "Vulcans"}, {"Kronos", "Clingons"}})));
	auto r2 = u1.colonise({{"Earth", "Humans", 0}, {"Vulcan", "Vulcans", 0}, {"Kronos", "Humans", 0}});
	assert(r2 == (map<string, string>({{"Earth", "Humans"}, {"Y1", "Humans"}, {"Vulcan", "Vulcans"}, {"Y2", "Vulcans"}, {"Kronos", "Humans"}})));
	auto r3 = u1.colonise({{"Unknown", "Unknown", 0}});
	assert(r3 == (map<string, string>({{"Unknown", "Unknown"}})));
	auto r4 = u1.colonise({});
	assert(r4 == (map<string, string>({})));

	CUniverse u2;

	u2.addConnection("Earth", "Z1")
			.addConnection("Earth", "Y1")
			.addConnection("Earth", "Kronos")
			.addConnection("Earth", "Vulcan")
			.addConnection("Vulcan", "Y3")
			.addConnection("Vulcan", "X1")
			.addConnection("Kronos", "Z2")
			.addConnection("Kronos", "X4")
			.addConnection("Kronos", "Vulcan")
			.addConnection("Y1", "Y2")
			.addConnection("Y2", "Y3")
			.addConnection("Z1", "Z2")
			.addConnection("X1", "X2")
			.addConnection("X2", "X3")
			.addConnection("X1", "X3")
			.addConnection("X3", "X4")
			.addConnection("Bajor", "Cassiopea Prime")
			.optimize();

	auto r5 = u2.colonise({{"Earth", "Humans", 0}, {"Vulcan", "Vulcans", 0}, {"Kronos", "Clingons", 0}, {"Cassiopea Prime", "Cassiopeans", 0}});
	assert(r5 == (map<string, string>({{"Earth", "Humans"}, {"Kronos", "Clingons"}, {"Vulcan", "Vulcans"}, {"Cassiopea Prime", "Cassiopeans"}, {"Bajor", "Cassiopeans"}, {"Z1", "Humans"}, {"Z2", "Clingons"}, {"Y1", "Humans"}, {"Y3", "Vulcans"}, {"X1", "Vulcans"}, {"X2", "Vulcans"}, {"X4", "Clingons"}})));
	cout << "BASIC ASSERTS PASSED!!!" << endl;

	// Harder tests when != 0 for all colonies
	CUniverse u3;

	u3.addConnection("Earth", "Z1")
			.addConnection("Earth", "Y1")
			.addConnection("Earth", "Kronos")
			.addConnection("Earth", "Vulcan")
			.addConnection("Vulcan", "Y3")
			.addConnection("Vulcan", "X1")
			.addConnection("Kronos", "Z2")
			.addConnection("Kronos", "X4")
			.addConnection("Y1", "Y2")
			.addConnection("Y2", "Y3")
			.addConnection("Z1", "Z2")
			.addConnection("X1", "X2")
			.addConnection("X1", "X3")
			.addConnection("X2", "X3")
			.addConnection("X3", "X4")
			.addConnection("Bajor", "Cassiopea Prime")
			.optimize();

	auto r8 = u3.colonise({{"Earth", "Humans", 1}, {"Vulcan", "Vulcans", 0}, {"Kronos", "Clingons", 2}, {"Cassiopea Prime", "Cassiopeans", 100}});
	assert(r8 == (map<string, string>({{"Earth", "Humans"}, {"Kronos", "Clingons"}, {"Vulcan", "Vulcans"}, {"Y1", "Humans"}, {"Z1", "Humans"}, {"Y3", "Vulcans"}, {"Y2", "Vulcans"}, {"X1", "Vulcans"}, {"X2", "Vulcans"}, {"X3", "Vulcans"}, {"Cassiopea Prime", "Cassiopeans"}, {"Bajor", "Cassiopeans"}})));

	cout << "ALL ASSERTS PASSED!!!" << endl;
	return EXIT_SUCCESS;
}
#endif /* __PROGTEST__ */