#pragma once
#include <bits/stdc++.h>

constexpr int INF = 0x3f3f3f3f;
using namespace std;

class Graph
{
    public:
        int dimension;
        vector< pair<int, int> > points;
        vector< vector<int> > distance_matrix;
        vector<int> demands;
    
        int depot_index;
        int uniform_vehicle_capacity;
        string name;
    
        static int euclidean_distance( const pair<int, int>& A, const pair<int, int>& B);

        Graph() {}

        Graph(string _t_path_to_instance);
    
    protected:
        void initialize_distance_matrix();

};