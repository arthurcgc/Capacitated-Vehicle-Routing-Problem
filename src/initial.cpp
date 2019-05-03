#include "initial.h"
using namespace std;

int get_shortest_dist(vector<int> &distance_vector, int node_index,  vector<int> &visited)
{
    int n = distance_vector.size();
    int lesser = 0x3f3f3f3f, lesser_index = node_index;


    std::vector<int>::iterator node_index_to_search;
    for(int i = 0; i < n; i++)
    {
        node_index_to_search = find(visited.begin(), visited.end(), i);
        // if iterator is *not* pointing to the end of the vector, it means we found it in the visited list
        if(distance_vector[i] == 0 || node_index_to_search != visited.end())
            continue;
        // this means we haven't visited node[i] yet, we can check the distance_matrix of node_index to see if node[i] has the lesser distance
        if(distance_vector[i] < lesser)
        {
            lesser = distance_vector[i];
            lesser_index = i;
        }
    }

    sort(visited.begin(), visited.end());
    return lesser_index;
}

vector< vector<int> > initial_solution(Graph *g)
{
    int capacity = g->uniform_vehicle_capacity;
    vector< vector<int> > routes;
    int count = 0;
    vector<int> visited_nodes;
    bool visited_all_nodes = false;

    visited_nodes.push_back(0);

    for(int i = 0; i < g->dimension && visited_all_nodes == false; i++)
    {
        routes.push_back(vector<int>());
        routes[i].push_back(0);
        int lesser_index = 0;
        while(true)
        {
            int last = lesser_index;
            lesser_index = get_shortest_dist( g->distance_matrix[lesser_index], lesser_index, visited_nodes );
            int temp_sum = g->demands[lesser_index] + count;
            if( temp_sum <= capacity )
            {
                count += g->demands[lesser_index];
                routes[i].push_back(lesser_index);
                visited_nodes.push_back(lesser_index);
                if(visited_nodes.size() == g->dimension)
                {
                    visited_all_nodes = true;
                    routes[i].push_back(0);
                    break;
                }
            }
            else
            {
                routes[i].push_back(0);
                count = 0;
                break;
            }         
        }
    }

    return routes;
}