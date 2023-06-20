#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>
#include <algorithm>
#include <map>
#include <set>
#include <vector>
#include <list>
#include <deque>
#include <queue>
#include <stack>
#include <cassert>

using namespace std;

class CTrain {
public:
    void Add(istringstream &is);

    int Count(void) const;

private:
    map<string, set<string>> m_Graph;
    set<string> m_Stops;
};

void CTrain::Add(istringstream &is) {
    string line;
    set<string> lineStops;
    while (getline(is, line)) {
        if (line.empty())
            continue;
        m_Stops.insert(line);
        lineStops.insert(line);
    }

    for (auto &stop: lineStops) {
        if (m_Graph.count(stop) != 0) {
            for (auto &lineStop: lineStops) {
                m_Graph[stop].insert(lineStop);
            }
            continue;
        }
        m_Graph[stop] = lineStops;
    }
}

int CTrain::Count(void) const {
    int count = 0;
    set<string> allVisited;
    if (m_Stops.empty())
        return 0;

    for (auto &stop: m_Stops) {
        cout << "Iteration: " << count << endl;
        if (allVisited == m_Stops) {
            cout << "DONE" << endl;
            break;
        }

        if (allVisited.count(stop) == 1) {
            continue;
        }

        queue<string> q;
        set<string> visited;

        q.push(stop);
        visited.insert(stop);
        count++;

        while (!q.empty()) {
            auto current = q.front();
            q.pop();

            for (auto &neighbor: m_Graph.at(current)) {
                if (visited.count(neighbor) == 0) {
                    q.push(neighbor);
                    visited.insert(neighbor);
                }
            }
        }

        allVisited.insert(visited.begin(), visited.end());
        cout << "*****************" << endl;
    }
    cout << "Count:" << count << endl;
    return count;
}

int main() {
    CTrain t0;
    istringstream iss;
    assert(t0.Count() == 0);

    iss.clear();
    iss.str("Newton\n");
    t0.Add(iss);
    assert(t0.Count() == 1);

    iss.clear();
    iss.str("Newton\n");
    t0.Add(iss);
    assert(t0.Count() == 1);

    iss.clear();
    iss.str("\n");
    t0.Add(iss);
    assert(t0.Count() == 1);

    iss.clear();
    iss.str("\n\nNewton\n\nNewton\n\nNewton\n\n");
    t0.Add(iss);
    assert(t0.Count() == 1);

    iss.clear();
    iss.str("Black Hill\n");
    t0.Add(iss);
    assert(t0.Count() == 2);

    iss.clear();
    iss.str("Wood Side\n");
    t0.Add(iss);
    assert(t0.Count() == 3);

    iss.clear();
    iss.str("Green Hill\n");
    t0.Add(iss);
    assert(t0.Count() == 4);

    iss.clear();
    iss.str("Lakeside\n");
    t0.Add(iss);
    assert(t0.Count() == 5);

    iss.clear();
    iss.str("Newton\nBlack Hill\nWood Side\nGreen Hill\nLakeside\n");
    t0.Add(iss);
    assert(t0.Count() == 1);

    iss.clear();
    iss.str("Little Newton\nLittle Burnside\nCastle Hill\nNewton Crossroad\nLakeside Central\n");
    t0.Add(iss);
    assert(t0.Count() == 2);

    iss.clear();
    iss.str("Waterton West\nWaterton Central\nWaterton East\nWaterton Woods\nLittle Waterton\nWaterton West\n");
    t0.Add(iss);
    assert(t0.Count() == 3);

    iss.clear();
    iss.str("Little Waterton\nLakeside Central\n");
    t0.Add(iss);
    assert(t0.Count() == 2);

    iss.clear();
    iss.str("Great Newton\nLittle Burnside\nGreen Hill\nWood Side\n");
    t0.Add(iss);
    assert(t0.Count() == 1);

    iss.clear();
    iss.str("Sodor Ironworks\nSodor Steamworks\nKnapford\nMaron\n");
    t0.Add(iss);
    assert(t0.Count() == 2);

    iss.clear();
    iss.str("Tidmouth\nGordons Hill\nSuderry\nKnapford\nGreat Waterton\nBrendam Docks\n");
    t0.Add(iss);
    assert(t0.Count() == 2);

    iss.clear();
    iss.str("Newton\nNewton Crossroad\nTidmouth\nBrendam Docks\n");
    t0.Add(iss);
    assert(t0.Count() == 1);
    return 0;
}