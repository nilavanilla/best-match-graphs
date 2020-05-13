//Header-Data of the equivalence class in a  cBMG

#ifndef _CLASS_H_
#define _CLASS_H_

#include "vertex.h"

class Class : public Vertex	{
	public:
		std::vector<int> members;				//contains the numbers of the vertices of this class
		std::vector<std::set<int> > neighbors; 	//neighbors[0] = N, neighbors[1] = NN, neighbors[2] = NNN, neighbor[3] = N- neighbors[4] = R

		Class(){};
		Class(int col, std::vector<int> memb, std::set<int> in, std::set<int> out, std::vector<std::vector<int> > adj, std::vector<int> vert, int nr);		//line 6
};

#endif //_CLASS_H_
