#include <cassert>
#include <iostream>
#include <list>
#include <map>
#include <queue>
#include <set>
#include <stack>
#include <vector>

using namespace std;
using point = pair<int, int>;
using mazeMap = vector<vector<int>>;

class Maze {
public:
    void loadMaze(const vector<vector<int>> &maze);

    list<point> findMaze(const pair<int, int> &start,
                         const pair<int, int> &end) const;

private:
    bool isValid(int x, int y) const;

    map<point, set<point>> m_Graph;
    set<point> m_Blocked;
    size_t m_MapHeight;
    size_t m_MapWidth;
};

bool Maze::isValid(int x, int y) const {
    if (y > m_MapHeight || y < 0 || x > m_MapWidth || x < 0)
        return false;
    return true;
}

void Maze::loadMaze(const vector<vector<int>> &maze) {
    m_MapHeight = maze[0].size() - 1;
    m_MapWidth = maze.size() - 1;

    for (int x = 0; x < maze.size(); ++x) {
        for (int y = 0; y < maze[0].size(); ++y) {
            if (maze[x][y] == 0)
                m_Blocked.insert({x, y});

            if (isValid(x, y + 1) && maze[x][y + 1] != 0) { // RIGHT
                m_Graph[{x, y}].insert({x, y + 1});
            }
            if (isValid(x, y - 1) && maze[x][y - 1] != 0) { // LEFT
                m_Graph[{x, y}].insert({x, y - 1});
            }
            if (isValid(x + 1, y) && maze[x + 1][y] != 0) { // DOWN
                if (maze[x + 1][y] != 0)
                    m_Graph[{x, y}].insert({x + 1, y});
            }
            if (isValid(x - 1, y) && maze[x - 1][y] != 0) { // UP
                m_Graph[{x, y}].insert({x - 1, y});
            }
        }
    }
}

list<point> Maze::findMaze(const pair<int, int> &start,
                           const pair<int, int> &end) const {
    if (!isValid(start.first, start.second) ||
        m_Blocked.find(start) != m_Blocked.end())
        throw out_of_range("Start coords out of range/blocked!");
    if (!isValid(end.first, end.second) || m_Blocked.find(end) != m_Blocked.end())
        throw out_of_range("End coords out of range/blocked!");

    queue<point> toVisit;
    map<point, point> visitedFrom;
    list<point> res;

    toVisit.push(start);
    visitedFrom[start] = start;

    while (!toVisit.empty()) {
        auto current = toVisit.front();
        toVisit.pop();

        if (current == end) {
            auto pos = current;
            // cout << "FOUND END!" << endl;
            while (pos != start) {
                // cout << pos.first <<"," << pos.second << "->";
                res.push_front(pos);
                pos = visitedFrom[pos];
            }
            // cout << pos.first <<"," << pos.second << endl;
            res.push_front(pos);
        }

        for (auto &neighbor : m_Graph.at(current)) {
            if (visitedFrom.count(neighbor) == 0) {
                toVisit.push(neighbor);
                visitedFrom[neighbor] = current;
            }
        }
    }
    if (res.empty()) {
        throw logic_error("Path not found!");
    }
    return res;
}

int main() {

    Maze m;

    mazeMap m1 = {{1, 1, 1, 1}, {1, 0, 1, 0}, {1, 1, 1, 1}};
    point start = {0, 0};
    point end = {0, 3};
    m.loadMaze(m1);
    auto res = m.findMaze(start, end);
    assert(res == list<point>({{0, 0}, {0, 1}, {0, 2}, {0, 3}}));

    Maze m2;
    mazeMap m2m = {{1, 1, 0, 0}, {0, 1, 1, 1}, {0, 1, 0, 1}};
    start = {0, 0};
    end = {2, 3};
    m2.loadMaze(m2m);
    res = m2.findMaze(start, end);
    assert(res == list<point>({{0, 0}, {0, 1}, {1, 1}, {1, 2}, {1, 3}, {2, 3}}));

    Maze m3;

    mazeMap m3m = {{1, 1, 1, 0, 1, 1, 0, 1, 1, 1, 1, 1, 1, 0},
                   {1, 0, 1, 0, 1, 0, 0, 0, 0, 1, 0, 1, 0, 1},
                   {0, 0, 1, 0, 1, 0, 1, 1, 1, 1, 1, 1, 0, 1},
                   {1, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                   {1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1},
                   {1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
                   {0, 0, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 0, 1},
                   {1, 0, 0, 0, 1, 0, 1, 0, 1, 0, 0, 0, 0, 1},
                   {1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1}};

    start = {0, 1};
    end = {6, 13};

    m3.loadMaze(m3m);
    res = m3.findMaze(start, end);
    assert(
            res ==
            list<point>({{0, 1},  {0, 2},  {1, 2},  {2, 2},  {3, 2},  {4, 2}, {5, 2},
                         {6, 2},  {6, 3},  {6, 4},  {7, 4},  {8, 4},  {8, 5}, {8, 6},
                         {7, 6},  {6, 6},  {6, 7},  {6, 8},  {7, 8},  {8, 8}, {8, 9},
                         {8, 10}, {8, 11}, {8, 12}, {8, 13}, {7, 13}, {6, 13}}));

    start = {20, 20};
    end = {0, 0};
    try {
        res = m3.findMaze(start, end);
        assert(false);
    } catch (std::out_of_range &e) {
        assert(true);
    }

    start = {0, 0};
    end = {0, 12};
    try {
        res = m3.findMaze(start, end);
        assert(false);
    } catch (std::logic_error &er) {
        assert(true);
    }

    start = {0, 0};
    end = {0, 13};

    try {
        res = m3.findMaze(start, end);
        assert(false);
    } catch (std::out_of_range &er) {
        assert(true);
    }

    cout << "POG" << endl;
    return 0;
}