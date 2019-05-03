#include "graph.h"

vector< string > split_line( string& line )
{
    stringstream sl(line);
    istream_iterator<string> begin(sl);
    istream_iterator<string> end;
    vector<string> words(begin, end);
    return words;
}

Graph::Graph(string _path_to_instance)
{
        // Criar uma struct instance que tenha um vetor de pontos ( vertices ), number_of_nodes e indice do deposito
    ifstream in(_path_to_instance);
    string line;
    vector< vector<string> > file_lines;

    while( getline(in, line) )
    {
        auto x = split_line(line);
        file_lines.emplace_back(x);
    }
    in.close();
    

    name = file_lines[0][2];
    dimension = stoi(file_lines[3][2]);
    uniform_vehicle_capacity = stoi( file_lines[5][2] );

    constexpr int graphdata_start = 7;

    for(int node = 0; node < dimension; ++node)
    {
        int x = stoi( file_lines[graphdata_start + node][1] );
        int y = stoi( file_lines[graphdata_start + node][2] );
        points.emplace_back(x, y);
    }

    const int demand_start = graphdata_start + dimension + 1;
    for(int node = 0; node < dimension; ++node)
    {
        int demand = stoi( file_lines[demand_start + node][1] );
        demands.emplace_back( demand );
    }
    
    depot_index = stoi(file_lines[demand_start + dimension + 1][0]) - 1;
    
    initialize_distance_matrix();
}


int Graph::euclidean_distance(const pair<int, int>& A, const pair<int, int>& B)
{
    int dx = (A.first - B.first) * (A.first - B.first);
    int dy = (A.second - B.second) * (A.second - B.second);
    return (int) ceil( sqrt( dx + dy ) );
}

void Graph::initialize_distance_matrix()
{
    if( points.size() == 0) return;
    distance_matrix.assign( dimension, vector<int>(dimension, INF) );
    for(int i = 0; i < dimension; ++i)
    {
        distance_matrix[i][i] = 0;

        for(int j = i + 1; j < dimension; ++j)
        {
            distance_matrix[i][j] = distance_matrix[j][i] = euclidean_distance( points[i], points[j] );
        }
    }
}