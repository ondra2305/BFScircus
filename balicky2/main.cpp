#include<iostream>
#include<cassert>
#include <utility>
#include<vector>
#include<set>
#include<map>
#include<string>
#include<unordered_map>
#include<memory>
#include<unordered_set>
#include<queue>
#include<list>
#include<sstream>
#include<cstring>

using namespace std;

class CPkg{
public:
    explicit CPkg(string name) : m_PackageName(std::move(name)) {}

    CPkg& addDep(const string& depName) {
        m_Deps.insert(depName);
        return *this;
    }
    string m_PackageName;
    set<string> m_Deps;
private:
};
class CPkgSys{
public:
    CPkgSys() = default;
    ~CPkgSys() = default;

    CPkgSys& addPkg(const CPkg& package) {
        m_Graph[package.m_PackageName] = package.m_Deps;
        m_Available.insert(package.m_PackageName);

        return *this;
    }

    set<string> install(const list<string> packages) {
        queue<string> toVisit;
        set<string> visited;
        set<string> res;

        for(auto &package : packages) {
            if(m_Graph.count(package) == 0)
                throw invalid_argument("Package not found!");

            toVisit.push(package);
            visited.insert(package);
        }

        while(!toVisit.empty()) {
            auto currentPkg = toVisit.front();
            toVisit.pop();

            for(auto &neighbor : m_Graph[currentPkg]) {
                if(visited.count(neighbor) == 0) {
                    if(m_Available.count(neighbor) == 0)
                        throw invalid_argument("Package not found.");

                    visited.insert(neighbor);
                    toVisit.push(neighbor);
                }
            }
        }
        for(auto &entry : visited) {
            if(m_Installed.count(entry) == 0) {
                res.insert(entry);
                m_Installed.insert(entry);
            }
        }
        return res;
    }

    //TODO operator <<
    friend ostream & operator<<(ostream& os, const CPkgSys& pkgSys) {
        if(pkgSys.m_Installed.empty())
            return os;

        bool first = true;
        for(auto &package : pkgSys.m_Installed) {
            if(!first)
                os << ", ";

            first = false;
            os << package;
        }
        return os;
    }
private:
    set<string> m_Installed;
    map<string, set<string>> m_Graph;
    set<string> m_Available;
};

int main(void){
    CPkgSys s;
    stringstream ss;
    s.addPkg(CPkg("ssh").addDep("sudo").addDep("apt"))
            .addPkg(CPkg("sudo").addDep("git").addDep("c++"));
    s.addPkg(CPkg("apt"))
            .addPkg(CPkg("c++").addDep("c").addDep("asm").addDep("fortran"));
    s.addPkg(CPkg("git"))
            .addPkg(CPkg("c").addDep("kekw"))
            .addPkg(CPkg("kekw"))
            .addPkg(CPkg("asm"))
            .addPkg(CPkg("fortran"));
    s.addPkg(CPkg("python").addDep("bash").addDep("sadge"))
            .addPkg(CPkg("karel").addDep("python"))
            .addPkg(CPkg("bash").addDep("sadge"))
            .addPkg(CPkg("sadge"))
            .addPkg(CPkg("cython").addDep("dev"));
    s.addPkg(CPkg("perl"));

    ss << s;
    cout << ss.str() << endl;
    assert(ss.str() == "");
    ss.clear();
    ss.str("");

    set<string> t1 = s.install(list<string> {"sudo"});
    assert(t1 == (set<string> {"asm", "c", "c++", "fortran", "git", "kekw", "sudo"}));

    set<string> t2 = s.install(list<string> {"ssh", "c++"});
    assert(t2 == (set<string> {"apt", "ssh"}));

    ss << s;
    cout << ss.str() << endl;
    assert(ss.str() == "apt, asm, c, c++, fortran, git, kekw, ssh, sudo");
    ss.clear();
    ss.str("");

    try{
        set<string> e = s.install(list<string> {"karel", "cython"});
        assert("Sem ses nemel dostat debilku" == nullptr);
    }
    catch(const invalid_argument & e){
        assert(strcmp("Package not found.", e.what()) == 0);
    }
    set<string> t3 = s.install(list<string> {"karel", "fortran", "git"});
    assert(t3 == (set<string> {"bash", "karel", "python", "sadge"}));

    s.addPkg(CPkg("java").addDep("utils"))
            .addPkg(CPkg("utils").addDep("VB"))
            .addPkg(CPkg("VB").addDep("java"));

    set<string> t4 = s.install(list<string> {"java", "perl"});
    assert(t4 == (set<string> {"VB", "java", "perl", "utils"}));

    ss << s;
    cout << ss.str() << endl;
    assert(ss.str() == "VB, apt, asm, bash, c, c++, fortran, git, java, karel, kekw, perl, python, sadge, ssh, sudo, utils");
    ss.clear();
    ss.str("");

    CPkgSys k;
    k.addPkg(CPkg("ssh").addDep("sudo").addDep("apt"))
            .addPkg(CPkg("sudo").addDep("git"));
    k.addPkg(CPkg("apt"));
    k.addPkg(CPkg("git"))
            .addPkg(CPkg("c").addDep("kekw"))
            .addPkg(CPkg("kekw"))
            .addPkg(CPkg("asm"))
            .addPkg(CPkg("fortran"));
    k.addPkg(CPkg("python").addDep("bash").addDep("sadge"))
            .addPkg(CPkg("karel").addDep("python"))
            .addPkg(CPkg("bash").addDep("sadge"))
            .addPkg(CPkg("sadge"));
    k.addPkg(CPkg("perl").addDep("no"));

    set<string> t5 = k.install(list<string> {"asm"});
    assert(t5 == (set<string> {"asm"}));
    set<string> t6 = k.install(list<string> {"python", "ssh"});
    assert(t6 == (set<string> {"apt", "bash", "git", "python", "sadge", "ssh", "sudo"}));

    try{
        set<string> t7 = k.install(list<string> {"perl", "c"});
        assert("Sem ses nemel dostat debilku" == nullptr);
    }
    catch(const invalid_argument & e){
        assert(strcmp("Package not found.", e.what()) == 0);
    }
    set<string> t8 = k.install(list<string> {"c", "ssh", "karel"});

    assert(t8 == (set<string> {"c", "karel", "kekw"}));

    ss << k;
    cout << ss.str() << endl;
    assert(ss.str() == "apt, asm, bash, c, git, karel, kekw, python, sadge, ssh, sudo");
    ss.clear();
    ss.str("");
}