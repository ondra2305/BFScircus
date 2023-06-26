#include <iostream>
#include <map>
#include <set>
#include <cassert>
#include <queue>
#include <list>
#include <vector>
#include <sstream>

using namespace std;

class CTrain {
public:
	CTrain() = default;

	~CTrain() = default;

	void Add(istream &is) {
		string prevName, curName;
		getline(is, prevName);

		while (getline(is, curName)) {
			this->stationList[curName].insert(prevName);
			this->stationList[prevName].insert(curName);
			prevName = curName;
		}
	}

	set<string> Dest(const vector<string> &stations, int &cost) {
		map<string, map<string, int>> distance;
		map<string, map<string, bool>> visited;
		for (const auto &pivot : stations) {
			queue<string> queue;
			visited[pivot][pivot] = true;
			distance[pivot][pivot] = 0;
			queue.push(pivot);
			while(!queue.empty()) {
				auto station = queue.front();
				queue.pop();
				for (const auto &child : this->stationList[station]) {
					if (!visited[pivot][child]) {
						visited[pivot][child] = true;
						distance[pivot][child] = distance[pivot][station] + 1;
						queue.push(child);
					}
				}
			}
		}
		set<string> result;
		auto minDistance = INTMAX_MAX;
		for (const auto &stationPair : this->stationList) {
			auto station = stationPair.first;
			bool common = true;
			auto curDistance = 0;
			for (const auto &pivot : stations) {
				if (!visited[pivot][station]) {
					common = false;
					break;
				}
				curDistance += distance[pivot][station];
			}
			if (common) {
				if (result.empty() || curDistance < minDistance) {
					result.clear();
					result.insert(station);
					minDistance = curDistance;
				} else if (curDistance == minDistance) {
					result.insert(station);
				}
			}
		}

		cost = minDistance == INTMAX_MAX ? cost : minDistance;
		return result;
	}

private:
	map<string, set<string>> stationList;
};

int main ( void )
{
	CTrain t0;
	istringstream iss;

	iss . clear ();
	iss . str (
			"1\n"
			"2\n"
			"3\n"
			"4\n"
			"5\n"
			"6\n"
			"7\n"
			"8\n" );
	t0 . Add ( iss );
	iss . clear ();
	iss . str (
			"12\n"
			"11\n"
			"4\n"
			"9\n"
			"10\n" );
	t0 . Add ( iss );
	iss . clear ();
	iss . str (
			"15\n"
			"11\n"
			"13\n"
			"14\n" );
	t0 . Add ( iss );
	iss . clear ();
	iss . str (
			"7\n"
			"16\n"
			"17\n" );
	t0 . Add ( iss );
	iss . clear ();
	iss . str (
			"20\n"
			"19\n"
			"17\n"
			"18\n" );
	t0 . Add ( iss );
	iss . clear ();
	iss . str (
			"21\n"
			"22\n"
			"23\n"
			"25\n"
			"24\n"
			"21\n" );
	t0 . Add ( iss );
	iss . clear ();
	iss . str (
			"26\n"
			"27\n"
			"28\n" );
	t0 . Add ( iss );
	iss . clear ();
	iss . str (
			"27\n"
			"29\n"
			"30\n" );
	t0 . Add ( iss );

	int cost = 5381;
	assert ( t0 . Dest ({ "2", "5", "11" }, cost ) == set <string> ({ "4" })
			 && cost == 4 );
	assert ( t0 . Dest ({ "12", "10" }, cost ) == set <string> ({ "10", "9", "4", "11", "12" })
			 && cost == 4 );
	assert ( t0 . Dest ({ "2", "9", "19" }, cost ) == set <string> ({ "4" })
			 && cost == 9 );
	assert ( t0 . Dest ({ "15", "12", "13" }, cost ) == set <string> ({ "11" })
			 && cost == 3 );
	cost = 5381;
	assert ( t0 . Dest ({ "21", "26" }, cost ) == set <string> ()
			 && cost == 5381 );
	cost = 1256;
	assert ( t0 . Dest ({ "10", "28" }, cost ) == set <string> ()
			 && cost == 1256 );
	assert ( t0 . Dest ({ "21", "25" }, cost ) == set <string> ({ "21", "24", "25" })
			 && cost == 2 );
	assert ( t0 . Dest ({ "21", "21" }, cost ) == set <string> ({ "21" })
			 && cost == 0 );
	assert ( t0 . Dest ({ "23", "21" }, cost ) == set <string> ({ "21", "22", "23" })
			 && cost == 2 );
	assert ( t0 . Dest ({ "12", "20" }, cost ) == set <string> ({ "12", "11", "4", "5", "6", "7", "16", "17", "19", "20" })
			 && cost == 9 );
	assert ( t0 . Dest ({ "50" }, cost ) == set <string> ({ "50" })
			 && cost == 0 );
	assert ( t0 . Dest ({ "13", "10" }, cost ) == set<string> ({ "10", "11", "13", "4", "9" })
			 && cost == 4 );
	assert ( t0 . Dest ({ "17", "19" }, cost ) == set<string> ({ "17", "19" })
			 && cost == 1 );
	cost = 1999;
	assert ( t0 . Dest ({ "29", "25" }, cost ) == set<string> ()
			 && cost == 1999 );
	assert ( t0 . Dest ({ "8", "5" }, cost ) == set<string> ({ "5", "6", "7", "8" })
			 && cost == 3 );
	assert ( t0 . Dest ({ "4", "4" }, cost ) == set<string> ({ "4" })
			 && cost == 0 );

	return 0;
}