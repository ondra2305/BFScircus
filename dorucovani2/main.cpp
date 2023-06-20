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
	CDelivery() = default;

	~CDelivery() = default;

	CDelivery &addConn(const string &a, const string &b);

	vector<int> findCoverage(const set<string> &depots) const;

private:
	map<string, set<string>> m_Graph;
	set<string> m_Cities;
};

CDelivery &CDelivery::addConn(const string &a, const string &b) {
	m_Graph[a].insert(b);
	m_Graph[b].insert(a);
	m_Cities.insert(a);
	m_Cities.insert(b);
	return *this;
}

vector<int> CDelivery::findCoverage(const set<string> &depots) const {
	queue<pair<string, int>> toVisit;
	map<string, int> distances;
	set<string> visited;

	for (auto &depot: depots) {
		if (m_Cities.find(depot) == m_Cities.end()) {
			throw invalid_argument("Invalid Depot!!!");
		}
		toVisit.emplace(depot, 0);
		visited.insert(depot);
	}

	unsigned citiesCnt = m_Cities.size();
	unsigned prevRemaining;
	int largestDist = 0;

	while (!toVisit.empty()) {
		auto current = toVisit.front();
		toVisit.pop();

		prevRemaining = citiesCnt;
		largestDist = max(largestDist, current.second);

		if (distances.count(current.first) == 0 || distances[current.first] > current.second) {
			citiesCnt--;
			distances[current.first] = current.second;
		}

		if (prevRemaining == citiesCnt) {
			break;
		}

		for (auto &neighbor: m_Graph.at(current.first)) {
			if (visited.count(neighbor) == 0) {
				toVisit.emplace(neighbor, current.second + 1);
				visited.insert(neighbor);
			}
		}
	}

	vector<int> res(largestDist + 1, 0);
	for (auto &city: distances) {
		for (int i = city.second; i <= largestDist; i++) {
			res[i]++;
		}
	}

	cout << "Distances:" << endl;
	for(auto &city: distances) {
		cout << city.first << " " << city.second << endl;
	}
	return res;
}

int main() {
	CDelivery t0;
	vector<int> r;
	t0.addConn("Austin", "Berlin").addConn("Chicago", "Berlin").addConn("Chicago", "Dallas").addConn("Dallas",
																									 "Essen").addConn(
			"Essen", "Austin").addConn("Frankfurt", "Essen").addConn("Gyor", "Frankfurt").addConn("Helsinki",
																								  "Istanbul").addConn(
			"Istanbul", "Jakarta");

	r = t0.findCoverage(set<string>{"Berlin"});
	assert (r == (vector<int>{1, 3, 5, 6, 7}));
	/* result:
	 * [0]: Berlin = 1
	 * [1]: Austin, Berlin, Chicago = 3
	 * [2]: Austin, Berlin, Chicago, Dallas, Essen = 5
	 * [3]: Austin, Berlin, Chicago, Dallas, Essen, Frankfurt = 6
	 * [4]: Austin, Berlin, Chicago, Dallas, Essen, Frankfurt, Gyor = 7
	 */
	r = t0.findCoverage(set<string>{"Berlin", "Essen"});
	assert (r == (vector<int>{2, 6, 7}));

	r = t0.findCoverage(set<string>{"Helsinki"});
	assert (r == (vector<int>{1, 2, 3}));

	r = t0.findCoverage(set<string>{"Istanbul"});
	assert (r == (vector<int>{1, 3}));

	r = t0.findCoverage(set<string>{"Austin", "Jakarta"});
	assert (r == (vector<int>{2, 5, 9, 10}));

	r = t0.findCoverage(set<string>{"Chicago", "Gyor", "Helsinki", "Jakarta"});
	assert (r == (vector<int>{4, 8, 10}));

	try {
		r = t0.findCoverage(set<string>{"Incorrect city"});
		assert ("No invalid_argument exception caught!" == nullptr);
	} catch (const invalid_argument &e) {}
	return EXIT_SUCCESS;
}