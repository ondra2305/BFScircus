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
#include <utility>
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
#endif /* __PROGTEST__ */

struct Link {
    Link(string name, unsigned fromTime, unsigned toTime) :
            m_Name(std::move(name)), m_FromTime(fromTime), m_ToTime(toTime) {}

    bool operator<(const Link &other) const {
        return tie(m_Name, m_FromTime, m_ToTime) < tie(other.m_Name, other.m_FromTime, other.m_ToTime);
    }

    string m_Name;
    unsigned m_FromTime;
    unsigned m_ToTime;
};

class CTeleport {
public:
    CTeleport() = default;

    ~CTeleport() = default;

    CTeleport &Add(const string &from,
                   const string &to,
                   unsigned fromTime,
                   unsigned toTime) {
        graph[from].insert({to, fromTime, toTime});
        return *this;
    }

    CTeleport &Optimize(void) {
        return *this;
    }

    unsigned FindWay(const string &from,
                     const string &to,
                     unsigned time) {
        cout << "****************************" << endl;
        queue<pair<string, unsigned>> toVisit;
        map<string, unsigned> visited;
        toVisit.emplace(from, time);
        visited.insert({from, time});

        while (!toVisit.empty()) {
            auto current = toVisit.front();
            cout << "Now at:" << current.first << "," << current.second << endl;
            toVisit.pop();

            if (current.first == to) {
                cout << "Found" << endl;
                cout << visited[current.first] << endl;
            }

            for (auto &neighbor: graph[current.first]) {
                if (neighbor.m_FromTime >= current.second &&
                    (visited.count(neighbor.m_Name) == 0 || visited[neighbor.m_Name] > neighbor.m_ToTime)) {
                    toVisit.emplace(neighbor.m_Name, neighbor.m_ToTime);
                    visited[neighbor.m_Name] = neighbor.m_ToTime;
                }
            }
        }
        if (visited.count(to) == 0) {
            throw invalid_argument("Path not found!!!");
        }
        return visited[to];
    }

private:
    map<string, set<Link>> graph;
};

#ifndef __PROGTEST__

int main(void) {
    CTeleport t;
    t.Add("Prague", "Vienna", 0, 7)
            .Add("Vienna", "Berlin", 9, 260)
            .Add("Vienna", "London", 8, 120)
            .Add("Vienna", "Chicago", 4, 3)
            .Add("Prague", "Vienna", 10, 10).Optimize();

    assert (t.FindWay("Prague", "Vienna", 0) == 7);
    assert (t.FindWay("Prague", "Vienna", 1) == 10);
    assert (t.FindWay("Prague", "London", 0) == 120);
    assert (t.FindWay("Vienna", "Chicago", 4) == 3);

    try {
        t.FindWay("Prague", "London", 2);
        assert ("Missing exception" == nullptr);
    }
    catch (const invalid_argument &e) {}
    catch (...) { assert ("Invalid exception" == nullptr); }
    try {
        t.FindWay("Prague", "Chicago", 0);
        assert ("Missing exception" == nullptr);
    }
    catch (const invalid_argument &e) {}
    catch (...) { assert ("Invalid exception" == nullptr); }


    t.Add("Dallas", "Atlanta", 150, 30)
            .Add("Berlin", "Helsinki", 1080, 2560)
            .Add("Chicago", "Frankfurt", 50, 0)
            .Add("Helsinki", "Vienna", 3200, 3)
            .Add("Chicago", "London", 10, 12)
            .Add("London", "Atlanta", 20, 40)
            .Add("Vienna", "Atlanta", 10, 50)
            .Add("Prague", "Vienna", 1, 6)
            .Add("Berlin", "Helsinki", 265, 265)
            .Add("Berlin", "London", 259, 0).Optimize();

    assert (t.FindWay("Prague", "Frankfurt", 0) == 0);
    assert (t.FindWay("Prague", "Atlanta", 0) == 40);
    assert (t.FindWay("Prague", "Atlanta", 10) == 50);
    return EXIT_SUCCESS;
}

#endif /* __PROGTEST__ */

