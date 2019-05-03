#include "optimization.h"


vector<int> TwoOptSwap(vector<int> &route, int i, int k)
{
    vector<int> new_route;

    //route[0] to route[i-1], add them to new_route
    for(int j = 0; j < i; j++)
        new_route.push_back(route[j]);
    vector<int> reverse;

    //route[i] to route[k] in reverse to new_route
    for(int j = i; j <= k; j++ )
        reverse.push_back(route[j]);
    std::reverse(reverse.begin(), reverse.end());
    new_route.insert(new_route.end(), reverse.begin(), reverse.end());
    
    // route[k+1] until the end, add to new_route
    for(int j = k+1; j < route.size(); j++)
        new_route.push_back(route[j]);

    return new_route;
}




void TwoOpt(vector<vector<int> > &routes, Graph *g)
{
    int dist_sum = 0;
    vector<int> new_route;
    int new_dist;
    // we're wrapping the 2-opt algorithm with all of the existing routes
    for(int count = 0; count < routes.size(); count++)
    {
        vector<int> route = routes[count];
        // this is where 2-opt begins
        bool improved = true;
        int best_dist = route_distance(routes[count], g->distance_matrix);
        while(improved)
        {
            improved = false;
            for (int i = 1; i < route.size()-2; i++)
            {
                for (int j = i+1; j < route.size()-1; j++)
                {
                    new_route = TwoOptSwap(route, i, j);
                    new_dist = route_distance(new_route, g->distance_matrix);

                    if(new_dist < best_dist)
                    {
                        routes[count] = new_route;
                        best_dist = new_dist;
                        improved = true;
                    }
                }   
            }
        }
    }
}


vector<int> getRoutesDemands(vector< vector<int> > routes, vector<int> demands)
{
    vector<int> res_vec;
    for(int i = 0; i < routes.size(); i++)
    {
        int demand = 0;
        for(int j = 0; j < routes[i].size(); j++)
        {
            demand += demands[routes[i][j]];
        }
        res_vec.push_back(demand);
    }

    return res_vec;
}


void swap(vector<int> &v1, vector<int> &v2, int i, int j)
{
    int temp = v1[i];
    v1[i] = v2[j];
    v2[j] = temp;
}

int calculate_route_demands(vector<int> route, vector<int> demands)
{
    int sum = 0;
    for(int i = 1; i < route.size()-1; i++)
        sum += demands[route[i]];
    return sum;
}

/*swap all the nodes from route1 through route2, always checking capacity,
 calculate the distance between the new routes everytime, if we find improvement, we accept the trade */
void swapNodes(vector<int> &route1, vector<int> &route2, Graph *g)
{
    int max_cap = g->uniform_vehicle_capacity;
    vector<int> route1_dc = route1;
    vector<int> route2_dc = route2;
    int new_dist1, new_dist2, total_dist, old_dist;
    int new_cap1, new_cap2;

    old_dist = route_distance(route1, g->distance_matrix) + route_distance(route2, g->distance_matrix);
    for(int i = 1; i < route1_dc.size()-1; i++)
    {
        for(int j = 1; j < route2_dc.size()-1; j++)
        {
            swap(route1_dc, route2_dc, i, j);
            new_cap1 = calculate_route_demands(route1_dc, g->demands);
            new_cap2 = calculate_route_demands(route2_dc, g->demands);
            if(new_cap1 > max_cap || new_cap2 > max_cap)
            {
                // unswap
                swap(route1_dc, route2_dc, i, j);
                continue;
            }
            else
            {
                new_dist1 = route_distance(route1_dc, g->distance_matrix);
                new_dist2 = route_distance(route2_dc, g->distance_matrix);
                total_dist = new_dist1 + new_dist2;
                if(total_dist < old_dist)
                {
                    route1 = route1_dc;
                    route2 = route2_dc;
                }
            }            
        }
    }
}

void Exchange(vector<vector<int>> &routes, Graph *g)
{
    for(int i = 0; i < routes.size(); i++)
    {
        for(int j = 0; j < routes.size(); j++)
        {
            if(i == j)
                continue;
            swapNodes(routes[i], routes[j], g);
        }
    }
}


tuple<int,int,int> Insertion(vector<int> &route1, vector<int> &route2, Graph *g, int route2_index)
{
    vector<int> cost_vec;
    int max_cap = g->uniform_vehicle_capacity;
    vector<int> route1_dc = route1;
    vector<int> route2_dc = route2;
    int new_dist1, new_dist2, total_dist, old_dist, best_dist;
    int new_cap1, new_cap2;
    std::tuple<int,int,int> dist_index1_index2 = make_tuple(-1, -1, -1);
    
    old_dist = route_distance(route1, g->distance_matrix) + route_distance(route2, g->distance_matrix);
    best_dist = old_dist;
    for(int i = 1; i < route1.size()-1; i++)
    {
        int cand = route1_dc[i];
        route1_dc.erase(route1_dc.begin()+i);
        new_dist1 = route_distance(route1_dc, g->distance_matrix);
        new_cap1 = calculate_route_demands(route1_dc, g->demands) - g->demands[cand];
        for(int j = 1; j < route2.size()-1; j++)
        {
            // agora achar melhor rota + melhor posição possivel
            new_cap2 = calculate_route_demands(route2_dc, g->demands) + g->demands[cand];
            if(new_cap2 > max_cap)
                break;
            else
            {
                route2_dc.insert(route2_dc.begin()+j, cand);
                new_dist2 = route_distance(route2_dc, g->distance_matrix);
                total_dist = new_dist1 + new_dist2;
                if(total_dist < best_dist)
                {
                    best_dist = total_dist;
                    dist_index1_index2 = make_tuple(best_dist, i ,j);
                }
                route2_dc.erase(route2_dc.begin()+j);
            }
        }
        // insert candidate back
        route1_dc.insert(route1_dc.begin()+i, cand);
    }
    return dist_index1_index2;
}

void Change(vector<vector<int>> &routes, vector<tuple<int,int,int>> v2sort, int route_corr)
{
    int best_dist = 99999999;
    int best_indexi, best_indexj, dist, route2insert;
    for(int i = 0; i < v2sort.size(); i++)
    {
        dist = std::get<0>(v2sort[i]);
        if(dist < best_dist && dist != -1)
        {
            best_dist = dist;
            best_indexi = std::get<1>(v2sort[i]);
            best_indexj = std::get<2>(v2sort[i]);
            route2insert = i;
        }
    }
    if(best_dist == 99999999)
        return;
    int node = *((routes[route_corr].begin()+best_indexi));
    routes[route_corr].erase(routes[route_corr].begin()+best_indexi);
    routes[route2insert].insert(routes[route2insert].begin()+best_indexj, node);
}

void DeleteInsert(vector<vector<int>> &routes, Graph *g)
{
    tuple<int, int, int> distance_indexi_indexj;
    for(int i = 0; i < routes.size(); i++)
    {
        vector<tuple<int,int,int>> ri_rj_distance_index2insert;
        for(int j = 0; j < routes.size(); j++)
        {
            if(i == j)
            {
                ri_rj_distance_index2insert.push_back(make_tuple(-1,-1,-1));
                continue;
            }
            distance_indexi_indexj = Insertion(routes[i], routes[j], g, j);    
            ri_rj_distance_index2insert.push_back(distance_indexi_indexj);
        }
        /* after the second loop we have populated the tuple vector containing the best route and index to delete and insert for route[i],
        *  now we have to sort the vector and change route[i] and route[j] accordingly */
        Change(routes, ri_rj_distance_index2insert, i);
    }
}