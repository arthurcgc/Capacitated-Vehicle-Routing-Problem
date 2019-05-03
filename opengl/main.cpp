#include <iostream>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <bits/stdc++.h>
#include <boost/algorithm/string.hpp>

using namespace std;

void getPair(std::string line, vector<pair<int,int>> &v)
{
    vector<string> first_split;
    boost::split(first_split, line, boost::is_any_of(" "));
    vector<string> xy(2);
    for( string s : first_split)
    {
        if(s == "")
            break;
        boost::split(xy, s, boost::is_any_of(","));
        int x = stoi(xy[0]);
        int y = stoi(xy[1]);
        v.push_back(make_pair(x, y));
        xy.clear();
    }
}

string filename;

static void Redraw(void)
{
    std::ifstream file(filename, std::ifstream::in);
    std::string line;
    vector<vector<pair<int, int>>> xy;
    int i = 0;
    while(file.good())
    {
        std::getline(file, line);
        if(line == "")
            break;
        xy.push_back(vector<pair<int,int>>());
        getPair(line, xy[i]);
        i++;
    }
    int rootx = xy[0][0].first;
    int rooty = xy[0][0].second;

    glClear(GL_COLOR_BUFFER_BIT);
    glPointSize(15);
    glColor3f(1, 0, 0);
    glBegin(GL_POINTS);
    glVertex2d(rootx, rooty);
    glEnd();
    

    glPointSize(5.0);
    glColor3f(0.3,0.3,1);
    glBegin(GL_POINTS);    
    for(int i = 0; i < xy.size(); i++)
    {
        for(int j =0; j < xy[i].size(); j++)
        {
            int x = xy[i][j].first;
            int y = xy[i][j].second;
            if(x == rootx && y == rooty)
                continue;
            glVertex2d(x, y);
        }
    }
    glEnd();

    //drawing routes
    srand(time(NULL));
    bool line_type;
    for(int i = 0; i < xy.size(); i++)
    {   
        vector<pair<int, int>> route = xy[i];
        double r = ((double) rand() / (RAND_MAX));
        double g = ((double) rand() / (RAND_MAX));
        double b = ((double) rand() / (RAND_MAX));
        if(r < 0.3)
            r+=0.3;
        if(g < 0.3)
            g+=0.3;
        if(b < 0.3)
            b+=0.3;
        glColor3f(r,g,b);
        glLineWidth(2);
        glBegin(GL_LINE_STRIP);

        for(int j = 1; j < route.size()-1;j++)
            glVertex2d(route[j].first, route[j].second);
        glEnd();
    }
    glFlush();
}

int main(int argc,char *argv[])
{
    filename = argv[1];
    glutInit(&argc,argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(1080,720);
    glutInitWindowPosition(0,0);
    glutCreateWindow("Routes");
    gluOrtho2D(0,1000,0,1000);
    glutDisplayFunc(Redraw);
    glutMainLoop();


    return 0;
}