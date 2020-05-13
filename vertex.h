//Header-Data of the Vertex

#ifndef _VERTEX_H_
#define _VERTEX_H_

#include <vector>
#include <set>
#include <algorithm>
#include <string>
#include <cstring>
#include <numeric>
#include <iostream>
#include <fstream>
#include <unistd.h>
#include <time.h>	


class Vertex	{
	public:
		int number;			//number of the vertex (every vertex number is unique in a graph)
		int species;		//color/species of the vertex
		std::string mark;	//label of the vertex that is used in the visualisation of the graphs
		
		Vertex() {};
		Vertex(int nr, int col, std::string na);		//line 6
};

#endif //_VERTEX_H_
