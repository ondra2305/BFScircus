#include <sstream>
#include <string>
#include <list>
#include <vector>
#include <stack>
#include <queue>
#include <deque>
#include <iomanip>
#include <iostream>
#include <map>
#include <set>
#include <algorithm>
#include <memory>
#include <cassert>

using namespace std;

class CDelivery {
public:
	// ctor
	// dtor

	CDelivery &addConn(const string &from, const string &to);

	map<string, list<string>> serveCustomers(const set<string> &customers, const set<string> &depots) const;

private:
	map<string, set<string>> m_Graph;
	set<string> m_Cities;
};

CDelivery &CDelivery::addConn(const string &from, const string &to) {
	m_Graph[from].insert(to);
	m_Cities.insert(from);
	m_Cities.insert(to);
	return *this;
}

map<string, list<string>> CDelivery::serveCustomers(const set<string> &customers, const set<string> &depots) const {
	queue<string> toVisit;
	map<string, string> visitedFrom;

	for (auto &depot: depots) {
		if(m_Cities.find(depot) == m_Cities.end()) {
			throw invalid_argument("Invalid city!!!");
		}
		toVisit.push(depot);
		visitedFrom[depot] = depot;
	}
	map<string, list<string>> res;

	while (!toVisit.empty()) {
		auto current = toVisit.front();
		toVisit.pop();

		auto it = customers.find(current);
		if (it != customers.end()) {
			auto pos = current;
			while (depots.find(pos) == depots.end()) {
				res[*it].push_front(pos);
				pos = visitedFrom[pos];
			}
			res[*it].push_front(pos);
		}

		for (auto &neighbor: m_Graph.at(current)) {
			if (visitedFrom.count(neighbor) == 0) {
				toVisit.push(neighbor);
				visitedFrom[neighbor] = current;
			}
		}
	}
	for (auto &customer: customers) {
		if (res.count(customer) == 0) {
			res[customer] = {};
		}
	}
	return res;
}

int main() {
	CDelivery t0;
	map<string, list<string>> r;
	t0.addConn("Austin", "Berlin");
	t0.addConn("Chicago", "Berlin");
	t0.addConn("Berlin", "Dallas");
	t0.addConn("Dallas", "Essen");
	t0.addConn("Essen", "Austin");
	t0.addConn("Frankfurt", "Gyor");
	t0.addConn("Gyor", "Helsinki");
	t0.addConn("Helsinki", "Frankfurt");

	r = t0.serveCustomers(set<string>{"Berlin", "Gyor"}, set<string>{"Essen", "Helsinki"});
	assert (r == (map<string, list<string>>{{"Berlin", {"Essen",    "Austin",    "Berlin"}},
											{"Gyor",   {"Helsinki", "Frankfurt", "Gyor"}}}));

	r = t0.serveCustomers(set<string>{"Austin", "Gyor", "Chicago"}, set<string>{"Austin", "Dallas"});
	assert (r == (map<string, list<string>>{{"Austin",  {"Austin"}},
											{"Chicago", {}},
											{"Gyor",    {}}}));
	t0.addConn("Chicago", "Helsinki");

	r = t0.serveCustomers(set<string>{"Austin", "Gyor", "Chicago"}, set<string>{"Austin", "Dallas"});
	assert (r == (map<string, list<string>>{{"Austin",  {"Austin"}},
											{"Chicago", {}},
											{"Gyor",    {}}}));
	t0.addConn("Berlin", "Chicago");

	r = t0.serveCustomers(set<string>{"Austin", "Gyor", "Chicago"}, set<string>{"Austin", "Dallas"});
	assert (r == (map<string, list<string>>{{"Austin",  {"Austin"}},
											{"Chicago", {"Austin", "Berlin", "Chicago"}},
											{"Gyor",    {"Austin", "Berlin", "Chicago", "Helsinki", "Frankfurt", "Gyor"}}}));
	t0.addConn("Essen", "Frankfurt");

	r = t0.serveCustomers(set<string>{"Austin", "Gyor", "Chicago"}, set<string>{"Austin", "Dallas"});
	assert (r == (map<string, list<string>>{{"Austin",  {"Austin"}},
											{"Chicago", {"Austin", "Berlin", "Chicago"}},
											{"Gyor",    {"Dallas", "Essen",  "Frankfurt", "Gyor"}}}));

	r = t0.serveCustomers(set<string>{"asdasdsad"}, set<string>{"Austin", "Dallas"});
	assert (r == (map<string, list<string>>{{"asdasdsad",  { }}}));

	try {
		r = t0.serveCustomers(set<string>{"Austin"}, set<string>{"qweqewq"});
		assert("No exception thrown!" == nullptr);
	} catch (const invalid_argument &e) {
	} catch (const exception &e) {
		assert("Invalid exception thrown!" == nullptr);
	}
	return EXIT_SUCCESS;
}