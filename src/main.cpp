#include <bits/stdc++.h>
#include "graph.h"
#include "initial.h"
#include "optimization.h"
#include <ctime>

using namespace std;

int TotalDistance(vector< vector<int> > &routes, Graph *g)
{
    int dist = 0;
    for(int i = 0; i < routes.size(); i++)
    {
        dist += route_distance(routes[i], g->distance_matrix);
    }

    return dist;
}


void printRoutePath(vector<int> &route, int route_num, vector<int> demands)
{
    cout << "--------------------------------------------------------------------------------------------------------------------------------------------------" << endl;
    cout << "#route" << route_num << ":\t";
    fflush(stdout);
    for(int i = 0; i < route.size(); i++)
    {
        cout << " -> " << route[i];
    }
    cout << endl;

    int demand_sum = 0;
    cout << "demands:\t";
    string str1,str2;
    int diff = 0;
    for(int i = 0;i < route.size(); i++)
    {
        demand_sum+= demands[route[i]];
        str1 = std::to_string(route[i]);
        str2 = std::to_string(demands[route[i]]);
        if(str1.length() > str2.length())
        {
            diff = str1.length() - str2.length();
            cout << " -> " << demands[route[i]];
            for(int j = 0; j < diff; j++)
                cout << ' ';
        }
        else
        {
            diff = str2.length() - str1.length();
            cout << " -> " << demands[route[i]];
        }
        
    }
    cout << "\tDemand sum: " << demand_sum;
    cout << endl;
}


void print2file(vector<vector<int>> routes, vector<pair<int,int>> points)
{
    std::ofstream file("initial_path.txt", std::ofstream::out);
    int node;
    for(int i = 0; i < routes.size(); i++)
    {
        for(int j = 0; j < routes[i].size(); j++)
        {
            node = routes[i][j];
            file << points[node].first << ',' << points[node].second << ' ';
        }
        file << endl;
    }
    file.close();
}

vector<int> parseBKS(string location)
{
    ifstream file;
    file.open(location);

    int count = 0;
    vector<int> bks;
    while(file.good() && count < 2)
    {
        string line; getline(file, line);
        int n = stoi(line);
        bks.push_back(n);
        count++;
    }
    return bks;
}


int main(int argc, char const *argv[])
{
    clock_t c_start_total = std::clock();
    // criando o grafo
    Graph *g = new Graph(argv[1]);
    //solucao inicial
    vector< vector<int> > routes = initial_solution(g);
    std::clock_t c_end = std::clock();
    long double time_elapsed_ms = 1000.0 * (c_end-c_start_total) / CLOCKS_PER_SEC;
    // obtendo distancia inicial
    int dist1 = TotalDistance(routes, g);
    /* print2file(routes, g->points); */

    vector<vector<int>> routes2opt = routes;
    clock_t start2opt = std::clock();
    // solucao inicial + 2-opt
    TwoOpt(routes2opt, g);
    std::clock_t end2opt = std::clock();
    long double time_elapsed_2opt = 1000.0 * (end2opt-start2opt) / CLOCKS_PER_SEC;
    int dist2 = TotalDistance(routes2opt, g);

    
    clock_t startExchange = std::clock();
    // Solucao inicial + Delete and Insert + Exchange
    DeleteInsert(routes,g);
    TwoOpt(routes, g);
    Exchange(routes, g);
    TwoOpt(routes, g);
    std::clock_t endExchange = std::clock();
    long double time_elapsed_exchange = 1000.0 * (endExchange-startExchange) / CLOCKS_PER_SEC;
    int dist3 = TotalDistance(routes, g);
    /* print2file(routes, g->points); */


    cout << "##################################################################################################################################################" << endl;
    cout << "Printing Delete/Insert + Exchange + 2opt:\n";
    for(int i = 0; i < routes.size(); i++)
        printRoutePath(routes[i], i, g->demands);
    cout << "##################################################################################################################################################" << endl;


    cout << "\n\nInitial solution distance: " << dist1 << endl;
    cout << "Distance with 2-Opt: " << dist2 << endl;
    cout << "Distance with Delete/Insert + Exchange + 2opt: " << dist3 << endl;
    cout << "Amount of vehicles used: " << routes.size() << endl;
    cout << endl;

    string BKS_location = argv[2];
    vector<int> bks = parseBKS(BKS_location);
    cout << "BKS distance: " << bks[0] << endl;
    cout << "BKS vehicles: " << bks[1] << endl;

    std::cout << "CPU time used, Initial Solution: " << time_elapsed_ms << " ms\n";
    std::cout << "CPU time used, 2-opt Solution: " << time_elapsed_2opt << " ms\n";
    std::cout << "CPU time used, Delete and Insert + Exchange Solution: " << time_elapsed_exchange << " ms\n";

    return 0;
}
