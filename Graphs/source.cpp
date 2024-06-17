#include <iostream>
#include <vector>
#include <algorithm>
#include <set>
#include "coordinates.h"
#include "readgraph.h"
#include "RouteToCsv.h"

using namespace std;

typedef long long ll;

vector<wstring> naming;
vector<Coordinates> c;
double maxrast;
vector<int> p(1e5); // vector p[i] = x, i is number of element , which located in set with number x

int dsu_get (int v) { // get number of set for element v
	return (v == p[v]) ? v : (p[v] = dsu_get (p[v])); 
}

void dsu_unite (int a, int b) {
	a = dsu_get (a); // get number of set for element a
	b = dsu_get (b); // get number of set for element b
	if (rand() & 1) // chosing main set
		swap (a, b);
	if (a != b)
		p[a] = b; // set of a is b
}


struct vertex {
    ll to ;
    double cost;
};

vector<vector<vertex>> build_graph() {
    vector<wstring> s;
    readCSV("kg.csv" , s ,c); 
    vector<vector<vertex>> g(s.size());
    naming = s;
    for(int i = 0;i < s.size();i ++) {
        for(int j = 0;j < s.size();j ++) {
            if(i != j) {
                ll to = j ;
                double cost = distanceBetweenCoordinates(c[i],c[j]);
                g[i].push_back({to , cost});
            }
        }
    }
    return g;
}

//A* Algorithm
/*
double heuristic(ll v, ll goal) {
    return distanceBetweenCoordinates(c[v],c[goal]);
}

vector<double> find_route(vector<vector<vertex>>& g, ll v, vector<ll>& p, ll goal) {
    ll n = g.size();
    vector<double> d(n, numeric_limits<double>::infinity());
    vector<bool> used(n, false);
    d[v] = 0;
    set<pair<double, ll>> s;
    s.insert({0, v});
    while (!s.empty()) {
        auto itt = s.begin();
        ll u = itt->second;
        s.erase(itt);
        if (u == goal) {
            break;
        }
        used[u] = true;
        for (int i = 0; i < g[u].size(); i++) {
            ll to = g[u][i].to;
            double cost = g[u][i].cost;
            if (cost > maxrast || used[to]) {
                continue;
            }
            double new_cost = d[u] + cost;
            if (new_cost < d[to]) {
                d[to] = new_cost;
                p[to] = u;
                double priority = new_cost + heuristic(to, goal);
                s.insert({priority, to});
            }
        }
    }

    return d;
}*/

//Djkstra Algorithm 
vector<double> find_route(vector<vector<vertex>>& g , ll v , vector<ll>& p, ll goal) {
    ll n = g.size();
    vector<double> d(n , 1e9); // vector of distance from v
    set<pair<ll,ll>> s; // set for geting minimum path log(n)
    vector<bool> used(n , false);
    d[v] = 0;
    used[v] = true;
    s.insert({0 , v});
    while(!s.empty()) {
        auto itt = s.begin(); //get min
        ll u = itt->second;
        s.erase(itt);
        for(int i = 0;i < g[u].size();i ++) { // check all edges
            ll to = g[u][i].to ;
            double cost = g[u][i].cost;
            if(cost > maxrast) {
                continue;
            }
            if(!used[to] && d[to] > d[u] + cost) { // chose min distance
                d[to] = d[u] + cost;
                p[to] = u;
                s.insert({d[to] , to});
            }
        }
    }
    return d;
}

vector<pair<ll,ll>> find_mst(vector<vector<vertex>> & g) {
    ll n = g.size();
    vector<pair<double , pair<ll,ll>>> gmst; // graph as a list of edges for MST
    for(int i = 0;i < n;i ++) {
        for(int j = 0;j < g[i].size();j ++) {
            ll from = i , to = g[i][j].to ;
            double cost = g[i][j].cost;
            gmst.push_back({cost , {from , to}});
        }
    }
    vector<pair<ll,ll>> res; //MST as a list of edges
    ll cost = 0; // cost of MST
    sort(gmst.begin(), gmst.end()); // sort all edges for cost (from min to max)
    p.resize(n);
    for (int i=0; i<n; ++i) // init DSU ,all vertex in different sets
        p[i] = i;
    for (int i=0; i<gmst.size(); ++i) {
        ll a = gmst[i].second.first,  b = gmst[i].second.second; // edge from a to b
        double l = gmst[i].first; // cost of edge
        if (dsu_get(a) != dsu_get(b)) { // check if edges lie in different sets
            cost += l;
            res.push_back (gmst[i].second);
            dsu_unite (a, b);
        }
    }
    return res;
}

// just standart DFS
void check_conectivity(vector<vector<vertex>> & g , ll v, vector<bool> & used) {
    used[v] = true;
    for(int i = 0;i < g[v].size();i ++) {
        ll to = g[v][i].to ;
        double cost = g[v][i].cost;
        if(cost > maxrast) {
            continue;
        }
        if(!used[to]) {
            check_conectivity(g , to , used);
        }
    }
}

void graph_browser(vector<vector<vertex>> & g) {
    std::locale german_locale("de_DE.UTF-8");
    std::locale::global(german_locale);
    std::locale russian_locale("ru_RU.UTF-8");
    std::locale::global(russian_locale);
    ll n = g.size();
    vector<vector<ll>> d(n,vector<ll> (n , 1e18));
    for(int i = 0;i < g.size();i ++) {
        for(int j = 0;j < g[i].size();j ++) {
            ll to = g[i][j].to;
            double cost = g[i][j].cost;
            d[i][to] = cost;
        }
    }
    for (int k=0; k<n; ++k)
        for (int i=0; i<n; ++i)
            for (int j=0; j<n; ++j)
                d[i][j] = min (d[i][j], d[i][k] + d[k][j]);
    for(int i = 0;i < n;i ++) {
        for(int j = 0;j < n;j ++) {
            if(d[i][j] < maxrast) {
                wcout << naming[i] << "->" << naming[j] << endl;
            }
        }
    }
}

void startup() {
    std::locale german_locale("de_DE.UTF-8");
    std::locale::global(german_locale);
    std::locale russian_locale("ru_RU.UTF-8");
    std::locale::global(russian_locale);
    cout << "Please, enter the maximum positive distance between a plane can fly (km):";
    cin >> maxrast;
    vector<vector<vertex>> g = build_graph();
    while(true) {
        cout << "1. Check if a route between two cities exists." << endl;
        cout << "2. Create a route between two cities." << endl;
        cout << "3. Create MST." << endl;
        cout << "4. Browse the flight network." << endl;
        cout << "5. Play a game." << endl;
        cout << "6. Exit." << endl;
        ll m;
        cin >> m;
        if(m == 1) {
            wstring s1, s2;
            cout << "Please, enter the origin city." << endl;
            wcin >> s1;
            cout << "Please, enter the destination city" << endl;
            wcin >> s2;
            ll s , t;
            for(int i = 0;i < naming.size();i ++) {
                if(s1 == naming[i]) {
                    s = i;
                }
                if(s2 == naming[i]) {
                    t = i;
                }
            }
            vector<bool> used(g.size() , false);
            check_conectivity(g , s , used);
            cout << "Checking if route between two cities exists." << endl;
            if(used[t]) {
                wcout << "There is a route between " << s1 << " " << s2 << endl;
            }
            else {
                wcout << "There is no a route between " << s1 << " " << s2 << endl;
            }
        }
        else if(m == 2) {
            wstring s1, s2;
            cout << "Please, enter the origin city." << endl;
            wcin >> s1;
            cout << "Please, enter the destination city" << endl;
            wcin >> s2;
            ll s , t;
            for(int i = 0;i < naming.size();i ++) {
                if(s1 == naming[i]) {
                    s = i;
                }
                if(s2 == naming[i]) {
                    t = i;
                }
            }
            vector<ll> p(g.size() , -1);
            vector<double> d = find_route(g , s , p , t);
            vector<ll> path;
            ll x = t;
            path.push_back(x);
            while(p[x] != -1) {
                x = p[x];
                path.push_back(x);
            }
            reverse(path.begin(),path.end());
            vector<Coordinates> route;
            vector<wstring> title;
            for(int i = 0;i < path.size() - 1;i ++) {
                wcout << naming[path[i]] << "->";
                route.push_back(c[path[i]]);
                title.push_back(naming[path[i]]);
            }
            route.push_back(c[path[path.size() - 1]]);
            title.push_back(naming[path[path.size() - 1]]);
            writeRouteToCSV(route , title, "route_data.csv");
            wcout << naming[path[path.size() - 1]] << endl;
        }
        else if(m == 3) {
            cout << "Name:" << endl;
            vector<pair<ll,ll>> mst = find_mst(g);
            for(int i = 0;i < mst.size();i ++) {
                wcout << naming[mst[i].first] << "->" << naming[mst[i].second] << endl;
            }
        }
        else if(m == 4) {
            graph_browser(g);
        }
        else if(m == 5) {
            cout << "Please, chose start city:" << endl;
            wstring st;
            wcin >> st;
            cout << "Please, chose end city:" << endl;
            wstring ed;
            wcin >> ed;
            ll s , t;
            for(int i = 0;i < g.size();i ++) {
                if(st == naming[i]) {
                    s = i;
                }
                if(ed == naming[i]) {
                    t = i;
                }
            }
            vector<ll> p(g.size());
            vector<double> d = find_route(g , s , p , t);
            double cost = 0;
            while(s != t) {
                vector<ll> k;
                for(int i = 0;i < g[s].size();i ++) {
                    if(g[s][i].cost > maxrast) {
                        continue;
                    }
                    k.push_back(i);
                    wcout << k.size() << ". "<< naming[g[s][i].to] << " (" << g[s][i].cost << " km)" << endl;
                }
                cout << "Chose where you want to go:";
                ll x;
                cin >> x;
                cost += g[s][k[x - 1]].cost;
                s = g[s][k[x - 1]].to;
            }
            cout << "The shortest path:" << d[t]  << " km" << endl;
            cout << "You found the path:" << cost << " km" << endl;
            if(cost > d[t]) {
                cout << "Your path longer than shortest path on " << cost - d[t] << "km. Try again" << endl; 
            }
            else {
                cout << "You found shortest path!" << endl;
            }
        }
        else if(m == 6) {
            return;
        }
    }
    
}

int main() {
    std::locale german_locale("de_DE.UTF-8");
    std::locale::global(german_locale);
    std::locale russian_locale("ru_RU.UTF-8");
    std::locale::global(russian_locale);
    //setlocale(LC_ALL,"Russian");
    startup();
    return 0;
}
