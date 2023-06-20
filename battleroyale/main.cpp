#ifndef __PROGTEST__
#include <algorithm>
#include <cassert>
#include <cctype>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <deque>
#include <iomanip>
#include <iostream>
#include <list>
#include <map>
#include <memory>
#include <queue>
#include <set>
#include <sstream>
#include <stack>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

using namespace std;

class CPos {
public:
	CPos(int y, int x, int hour)
			: m_posY(y), m_posX(x), m_hour(hour)
	{
	}

	int m_posY;
	int m_posX;
	int m_hour;
};
#endif /* __PROGTEST__ */

/*
Pokud by map<pair<int,int>, set<pair<int,int>>> m_Graph bylo moc scuffed:

struct Coords {
	Coords(int y, int x) : m_Y(y), m_X(x) {}

	bool operator < (const Coords& other) const {
		return tie(m_Y, m_X) < tie(other.m_Y, other.m_X);
	}

	bool operator == (const Coords& other) const {
		return tie(m_Y, m_X) == tie(other.m_Y, other.m_X);
	}

	int m_Y;
	int m_X;
};
*/

class CBattleRoyale {
public:
	CBattleRoyale(unsigned height, unsigned width, const std::list<CPos> &zones)
	{
		m_Height = height;
		m_Width = width;

		for(int y = 0; y < height; y++) {
			for(int x = 0; x < width; x++) {
				if(isValidPos(y,x+1)) { //RIGHT
					m_Graph[{y, x}].insert({y,x+1});
				}
				if(isValidPos(y,x-1)) { //LEFT
					m_Graph[{y, x}].insert({y,x-1});
				}
				if(isValidPos(y+1,x)) { //DOWN
					m_Graph[{y, x}].insert({y+1,x});
				}
				if(isValidPos(y-1,x)) { //UP
					m_Graph[{y, x}].insert({y-1,x});
				}
			}
		}

		for(auto &zone: zones) {
			m_Forbidden[{zone.m_posY, zone.m_posX}] = zone.m_hour;
		}
	}

	~CBattleRoyale() = default;

	unsigned findRoute(int ySt, int xSt, int yEn, int xEn) const
	{
		queue<pair<pair<int,int>, int>> toVisit;
		set<pair<int,int>> visited;
		map<pair<int,int>, pair<int,int>> parents;

		toVisit.push({{ySt, xSt}, 0});
		visited.insert({ySt, xSt});

		if(m_Forbidden.count({ySt, xSt}) != 0 && m_Forbidden.at({ySt, xSt}) == 0)
			throw std::logic_error("No path found");

		if(ySt == yEn && xSt == xEn)
			return 0;

		while(!toVisit.empty()) {
			auto current = toVisit.front();
			toVisit.pop();

			if(current.first.first == yEn && current.first.second == xEn) {
				cout << "*********" << endl;
				cout << "Path:" << endl;
				while(current.first.first != ySt || current.first.second != xSt) {
					cout << current.first.first << "," << current.first.second << endl;
					current.first = parents[current.first];
				}
				cout << "FOUND. Distance:" << current.second << endl;
				return current.second;
			}

			for(auto &neighbor : m_Graph.at(current.first)) {
				if(visited.count(neighbor) == 0 && (m_Forbidden.count(neighbor) == 0
												   || m_Forbidden.at(neighbor) > current.second + 1)) {
					visited.insert(neighbor);
					toVisit.emplace(neighbor, current.second + 1);
					parents[neighbor] = current.first;
				}
			}
		}
		throw std::logic_error("No path found");
	}

private:
	unsigned m_Height;
	unsigned m_Width;
	bool isValidPos(int y, int x) const {
		if(y > m_Height-1 || y < 0)
			return false;
		if(x > m_Width-1 || x < 0)
			return false;
		return true;
	}

	map<pair<int,int>, set<pair<int,int>>> m_Graph;
	map<pair<int,int>, int> m_Forbidden;
};

#ifndef __PROGTEST__
int main() {
	CBattleRoyale r1(5, 5, {});
	assert(r1.findRoute(0, 0, 4, 0) == 4);
	assert(r1.findRoute(4, 4, 4, 4) == 0);

	CBattleRoyale r2(6, 7, {CPos(1, 0, 1), CPos(2, 1, 2), CPos(3, 2, 5)});
	assert(r2.findRoute(0, 0, 4, 0) ==  10);

	CBattleRoyale r3(8, 8, {CPos(0, 2, 1), CPos(3, 1, 2), CPos(2, 1, 0)});
	try {
		r3.findRoute(2, 1, 4, 7);
		assert("Exception missing!" == nullptr);
	} catch (const logic_error &e) {
	} catch (...) {
		assert("Invalid exception thrown!" == nullptr);
	}
	assert(r3.findRoute(0,2,3,0)==5);

	CBattleRoyale b(5,5,{CPos(0,1,1),CPos(1,1,0)});
	assert(b.findRoute(0,0,2,2)==4);
	assert(b.findRoute(0,0,0,2)==6);
	assert(b.findRoute(3,3,3,3)==0);
	try{
		assert(b.findRoute(1,1,2,1)==1);
		assert("Kde vyjimka?"==nullptr);
	} catch (logic_error & x){}
	try{
		assert(b.findRoute(1,1,1,1)==0);
		assert("Kde vyjimka? xd"==nullptr);
	} catch (logic_error & x){}

	CBattleRoyale b1(5,5,{CPos(2,0,2), CPos(2,1,1),CPos(2,2,1), CPos(2,3,3),CPos(2,4,4)});
	try{
		b1.findRoute(1,1,3,1);
		assert("Kde vyjimka?"==nullptr);
	} catch (logic_error & x){}

	return 0;
}
#endif /* __PROGTEST__ */