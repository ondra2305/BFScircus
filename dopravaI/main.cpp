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

class CMHD{
public:

	void Add(istringstream & is);

	set<string> Dest (const string & from, int maxCost);

private:
	map<string, set<string>> m_Graph;
};

void CMHD::Add(istringstream &is) {
	string line;
	set<string> lineStops;
	while(getline(is, line)) {
		lineStops.insert(line);
	}
	for(auto &lineStop : lineStops) {
		for(auto &lineStop1 : lineStops) {
			m_Graph[lineStop1].insert(lineStop);
		}
	}
}

set<string> CMHD::Dest(const string &from, int maxCost) {
	set<string> res;
	if(m_Graph.count(from) == 0) {
		res.insert(from);
		return res;
	}

	queue<pair<string, int>> toVisit;
	map<string, string> visitedFrom;

	toVisit.emplace(from, 0);
	visitedFrom.emplace(from, from);

	while(!toVisit.empty()) {
		auto current = toVisit.front();
		toVisit.pop();

		//cout << "Current pos:" << current.first << endl;
		//cout << "Current cost:" << current.second << endl;

		if(current.second > maxCost)
			continue;
		res.insert(current.first);

		for(auto &neighbor : m_Graph.at(current.first)) {
			if(visitedFrom.count(neighbor) == 0) {
				int newCost = current.second;

				auto prevPos = visitedFrom[current.first];
				prevPos = visitedFrom[prevPos];
				//cout << "Visited from:" << prevPos << endl;
				if(m_Graph.at(neighbor).find(visitedFrom[prevPos])
				   == m_Graph.at(neighbor).end()) {
					newCost++;
				}

				toVisit.emplace(neighbor, newCost);
				visitedFrom[neighbor] = current.first;
			}
		}
	}
	return res;
}

int main ( void )
{
	CMHD city;
	istringstream iss;
	iss.clear();

	iss . str ( "A\nB\nC\nD\nE\n" );
	city . Add ( iss );
	iss . clear();

	iss . str ( "B\nC\nF\nH\n" );
	city . Add ( iss );
	iss . clear();

	iss . str ( "F\nG\nI\nJ\nK\nN\n" );
	city . Add ( iss );
	iss . clear();

	iss . str ( "H\nL\n" );
	city . Add ( iss );
	iss . clear();

	iss . str ( "L\nM\nN\nO\n" );
	city . Add ( iss );
	iss . clear();

	iss . str ( "P\nQ\nR\nN\nS" );
	city . Add ( iss );
	iss . clear();
	//cout << city.Dest("N",0) << endl;
	assert ( city . Dest ( "S", 0 ) == set < string > ( {"S", "N", "R", "Q", "P"} ) );



	assert ( city . Dest ( "N", 0 ) == set < string > ( { "S", "N", "R", "Q", "P",
														  "O", "M", "L",
														  "K", "J", "I", "G", "F" } ) );
	// cout<<city.Dest("S", 1)<<endl;
	assert ( city . Dest ( "S", 1 ) == set < string > ( { "S", "N", "R", "Q", "P",
														  "O", "M", "L",
														  "K", "J", "I", "G", "F" } ) );
	assert ( city . Dest ( "N", 1 ) == set < string > ( { "S", "N", "R", "Q", "P",
														  "O", "M", "L",
														  "K", "J", "I", "G", "F",
														  "H", "F", "C", "B" } ) );

	assert ( city . Dest ( "N", 2 ) == set < string > ( { "S", "N", "R", "Q", "P",
														  "O", "M", "L",
														  "K", "J", "I", "G", "F",
														  "H", "F", "C", "B",
														  "A", "D", "E" } ) );


	assert ( city . Dest ( "unknown", 0 ) == set < string > ( { "unknown" } ) );
	assert ( city . Dest ( "unknown", 1 ) == set < string > ( { "unknown" } ) );
	assert ( city . Dest ( "unknown", 2 ) == set < string > ( { "unknown" }) );

	// speed test
	CMHD circleCity;
	iss.clear();
	iss.str("A\nB\nC\n");
	circleCity.Add(iss);

	iss.clear();
	iss.str("C\nD\nA\n");
	circleCity.Add(iss);

	assert(circleCity.Dest("A", 1000) == set<string>({"A", "B", "C", "D"}));

	cout<<"All asserts passed."<<endl;
	return 0;
}