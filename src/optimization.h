#pragma once
#include <bits/stdc++.h>
#include "graph.h"

using namespace std;

static int route_distance(vector<int> route, vector< vector<int> > distance_matrix )
{
    int dist = 0;
    for(int i = 0; i < route.size()-1; i++)
    {
        int line = route[i];
        int column = route[i+1];
        dist += distance_matrix[line][column];
    }

    return dist;
}


void TwoOpt(vector<vector<int> > &routes, Graph *g);


void Exchange(vector<vector<int>> &routes, Graph *g);

void DeleteInsert(vector<vector<int>> &routes, Graph *g);