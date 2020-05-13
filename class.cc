//Nicola Doering
//28/01/2020
//BA Best Match Graphs

#include "class.h"

using namespace std;


//constructiong a new class of the species "col", with number "nr", the in and out neighbors "in" and "out" and the represented vertices "memb". computing the neighborhoods NN and NNN and the reachable set R of the class in the BMG with vertex set "vertOfGraph", for these computations it needs the adjacency matrix "adj" and "vert" that contains the vertex numbers of all vertices in the connected 2 colored Digraph
Class::Class(int col, vector<int> memb, set<int> in, set<int> out, vector<vector<int> > adj, vector<int> vert, int nr)	{
	members = memb;
	species = col;
	number = nr;
	neighbors.assign(5, {});
	neighbors[0] = out;
	neighbors[3] = in;
	int v;
	int w;
	for (auto elem1 : neighbors[0])	{
		for (int i = 0; i < adj.size(); i++)	{
			if (adj[elem1][i] == 1 && find(vert.begin(), vert.end(), i) != vert.end())	{
				neighbors[1].insert(i);
			}
		}
		for (auto elem2 : neighbors[1])	{
			for (int i = 0; i < adj.size(); i++)	{
				if (adj[elem2][i] == 1 && find(vert.begin(), vert.end(), i) != vert.end())	{
					neighbors[2].insert(i);
				}
			}
		}
	}
	neighbors[4] = neighbors[0];
	neighbors[4].insert(neighbors[1].begin(), neighbors[1].end());
}
