#ifndef __PROGTEST__

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <set>
#include <list>
#include <deque>
#include <queue>
#include <algorithm>
#include <utility>
#include <memory>

using namespace std;
#endif /* __PROGTEST__ */

class CArmyNotSpecified {
};

class CSamePact {
};

class CArmy {
public:
	CArmy() = default;

	void Update(const string &name, int cnt) {
		sizes[name] = cnt;
		graph[name] = {};
		armies.insert(name);
	}

	void Treat(const string &name1, const string &name2, bool status) {
		if (name1 == name2) {
			return;
		}
		graph[name1][name2] = status;
		graph[name2][name1] = status;
	}

	int Compare(const string &name1, const string &name2) const {
		if (!graph.count(name1) || !graph.count(name2))
			throw CArmyNotSpecified();
		if (!armies.count(name1) || !armies.count(name2))
			throw CArmyNotSpecified();

		if (graph.at(name1).count(name2) && graph.at(name2).count(name1)) {
			if (graph.at(name1).at(name2) && graph.at(name2).at(name1)) {
				throw CSamePact();
			}
		}

		int size1 = bfs(name1);
		int size2 = bfs(name2);
		cout << "Army1:" << size1 << endl;
		cout << "Army2:" << size2 << endl;

		return size1 - size2;
	}

private:
	int bfs(const string &name) const {
		queue<string> q;
		set<string> visited;
		q.push(name);
		visited.insert(name);

		int size = 0;

		while (!q.empty()) {
			auto current = q.front();
			q.pop();

			for (auto &neighbor: graph.at(current)) {
				if (visited.count(neighbor.first) == 0 && neighbor.second) {
					q.push(neighbor.first);
					visited.insert(neighbor.first);
					if (sizes.count(neighbor.first)) {
						size += sizes.at(neighbor.first);
					}
				}
			}
		}

		if (sizes.count(name)) {
			size += sizes.at(name);
		}
		return size;
	}

	map<string, map<string, bool>> graph;
	map<string, int> sizes;
	set<string> armies;
};

#ifndef __PROGTEST__

int main() {
	CArmy x;
	x.Update("A", 1500);
	x.Update("B", 1500);
	x.Update("C", 2000);
	x.Update("D", 1000);

	cout << x.Compare("A", "B") << endl; //0
	cout << x.Compare("C", "D") << endl; //1000


	x.Treat("A", "B", true);
	x.Treat("C", "D", true);

	cout << x.Compare("A", "C") << endl; //0

	try { x.Compare("A", "E"); } catch (const CArmyNotSpecified &err) { cout << "CArmyNotSpecified" << endl; }

	x.Update("E", 100);
	x.Treat("E", "B", true);

	try { x.Compare("A", "E"); } catch (const CSamePact &err) { cout << "CSamePact" << endl; }

	x.Update("F", 2500);
	x.Treat("A", "F", true);

	cout << x.Compare("A", "C") << endl; //2600


	x.Treat("A", "B", false);
	cout << x.Compare("A", "B") << endl; //2400

	x.Treat("A", "B", true);
	x.Treat("A", "B", true);

	try { x.Compare("A", "B"); } catch (const CSamePact &err) { cout << "CSamePact" << endl; }

	x.Treat("B", "F", true);
	x.Treat("D", "G", true);
	try { x.Compare("F", "C"); } catch (const CArmyNotSpecified &err) { cout << "CArmyNotSpecified" << endl; }

	x.Update("G", 3300);
	cout << x.Compare("F", "C") << endl; //-700

	x.Treat("A", "B", false);
	x.Treat("F", "A", false);
	x.Update("A", 340);
	cout << x.Compare("A", "B") << endl; // -3760
}

#endif /* __PROGTEST__ */