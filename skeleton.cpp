/*
 * =====================================================================================
 *
 *       Filename:  skeleton.cpp      Created:  06/10/2015 10:09:09 AM
 *
 *    Description:  skeleton info
 *
 *         Author:  Wu Bo (Robert), wubo.gfkd@gmail.com
 *		Copyright:	Copyright (c) 2015, Wu Bo
 *   Organization:  National University of Defense Technology
 *
 * =====================================================================================
 */
#include <iostream>
#include <fstream>
#include <QDebug>
#include "skeleton.h"
#include "StarlabException.h"

void Skeleton::read_skel(const std::string fname)
{
    using namespace std;
    using namespace Eigen;
    string inputLine;
    ifstream file(fname.c_str());
    if(!file.is_open())
        throw StarlabException("Cannot open skeleton file");

    int degree, numberOfEdges, numberOfNodes;
    if(!file.eof() && getline(file, inputLine))
    {
        if(sscanf(inputLine.c_str(), "# D:%d NV:%d NE:%d", &degree, &numberOfNodes, &numberOfEdges) != 3)
        {
            qDebug("%d - %d - %d\n", degree, numberOfNodes, numberOfEdges);
            file.close();
            throw StarlabException("Error reading skeleton file");
        }
    }

    int nVertices = 0, nEdges = 0;
    float x, y, z;
    int n1, n2;
    while(!file.eof())
    {
        getline(file, inputLine);
        if(inputLine.length() < 1) 
            continue;
        switch(inputLine[0])
        {
            case 'v':
                if(sscanf(inputLine.c_str(), "v %f %f %f", &x, &y, &z) == 3)
                {
                    vertices.push_back(Vector3d(x, y, z));
                    ++nVertices;
                }
                break;
            case 'e':
                if(sscanf(inputLine.c_str(), "e %d %d", &n1, &n2) == 2)
                {
                    edges.push_back(Vector2i(n1-1, n2-1));
                    ++nEdges;
                }
                break;
        }
    }
    file.close();
    if(nVertices != numberOfNodes)
        cerr<<"vertex number error\n";
    if(nEdges != numberOfEdges)
        cerr<<"edge number error\n";
}

void Skeleton::clear_skel()
{
    vertices.clear();
    edges.clear();
}
