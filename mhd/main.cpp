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

class CPublicTransport {
public:
	CPublicTransport() {}

	CPublicTransport &addLine(unsigned cost, const vector<string> &stops) {
		for (auto &stop: stops) {
			for (auto &stop1: stops) {
				if (stop == stop1) continue;
				m_Graph[stop].insert({stop1, cost});
			}
		}
		return *this;
	}

	CPublicTransport &optimize() {
		// NOIDONTTHINKSO
		return *this;
	}

	unsigned findCheapest(const string &from, const string &to) {
		map<string, unsigned> visited;
		queue<pair<string, unsigned>> toVisit;
		toVisit.emplace(from, 0);
		visited.insert({from, 0});

		cout << "**********" << endl;
		while (!toVisit.empty()) {
			auto current = toVisit.front();
			toVisit.pop();

			for (auto &neighbor: m_Graph.at(current.first)) {
				if (visited.count(neighbor.first) == 0 || visited[neighbor.first] > current.second + neighbor.second) {
					if(visited.count(neighbor.first) == 1) {
						cout << "Looped at least twice" << endl;
					}
					if (neighbor.second != current.second) {
						toVisit.emplace(neighbor.first, current.second + neighbor.second);
						visited[neighbor.first] = current.second + neighbor.second;
					} else {
						toVisit.emplace(neighbor.first, current.second);
						visited[neighbor.first] = current.second;
					}
				}
			}
		}
		if (visited.count(to) == 0) {
			throw logic_error("Connection not found!");
		}
		return visited[to];
	}


private:
	map<string, set<pair<string, unsigned>>> m_Graph;
};

int main() {
	CPublicTransport t0;

	t0.addLine(1, {"Staromestska", "Muzeum", "Mustek", "Namesti Miru", "Jiriho z Podebrad"}).
			addLine(3, {"Hlavni Nadrazi", "Orionka", "Baterie", "Petriny", "Jiriho z Podebrad"}).
			addLine(2, {"Kamenicka", "Letenske namesti", "Baterie", "Petriny", "Namesti Miru", "Malovanka"}).optimize();


	assert(t0.findCheapest("Staromestska", "Baterie") == 3);
	assert(t0.findCheapest("Staromestska", "Staromestska") == 0);
	assert(t0.findCheapest("Staromestska", "Namesti Miru") == 1);
	assert(t0.findCheapest("Staromestska", "Hlavni Nadrazi") == 4);
	assert(t0.findCheapest("Orionka", "Kamenicka") == 5);

	t0.addLine(6, {"Vitezne namesti", "I P Pavlova", "Andel"});

	try {t0.findCheapest("Orionka", "Andel"); }
	catch ( const logic_error & e ) { assert(e.what() != "");}

	CPublicTransport t;

	t.addLine(3, {"Hlavni Nadrazi", "Orionka", "Baterie", "Petriny", "Jiriho z Podebrad"})
	.addLine(2, {"Kamenicka","Letenske namesti","Baterie","Petriny","Namesti Miru","Malovanka"})
	.addLine(5, {"Dejvicka", "Muzeum"})
	.addLine(1, {"Staromestska", "Muzeum", "Mustek", "Namesti Miru","Jiriho z Podebrad"}).optimize();

	assert(t.findCheapest("Staromestska", "Baterie") == 3);
	assert(t.findCheapest("Staromestska", "Staromestska") == 0);
	assert(t.findCheapest("Staromestska", "Namesti Miru") == 1);
	assert(t.findCheapest("Staromestska", "Hlavni Nadrazi") == 4);
	assert(t.findCheapest("Orionka", "Kamenicka") == 5);
	assert(t.findCheapest("Jiriho z Podebrad", "Namesti Miru") == 1);
	assert(t.findCheapest("Dejvicka", "Letenske namesti") == 8);


	t.addLine(6, {"Vitezne namesti", "I P Pavlova", "Andel"});
	t.addLine(9, {"Letenske namesti", "Vitezne namesti"});

	assert(t.findCheapest("Dejvicka", "Andel") == 23);
	assert(t.findCheapest("Andel", "Dejvicka") == 23);
	assert(t.findCheapest("Vitezne namesti", "Letenske namesti") == 9);
	assert(t.findCheapest("Orionka", "Andel") == 20);

	t.addLine(9, {"Terminal 1", "Terminal 2", "Terminal 3", "Nadrazi Veleslavin"});

	try {
		t.findCheapest("Namesti Miru", "Terminal 2");
	} catch (const logic_error &e) {
		//assert(e.what() != "");
		cout << "chyceno" << endl;
	}
}