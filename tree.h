//Header-Data of Tree

#ifndef _TREE_H_
#define _TREE_H_

#include "class.h"

class Tree	{
	public:
		//the vectors contain all characteristics of the nodes of the tree, every entry of the vectors represents one tree node while its position in the vectors is the same in all of them 
		std::vector<int> parent;					//the root is at position 0 and has parent -1 because it has no parent, the other entries contain the position of the parent node in the vectors
		std::vector<std::set<int> > children;		//the entries contain the positions of all children nodes of the represented node
		std::vector<int> vertex;					//the entries contain the vertex number if the represented node is a leaf, in the other case there is a -1 at this position
		std::vector<int> classes;					//the entries contain the class number if the represented node is a leaf, in the other case there is a -1 at this position (is only needed if algorithm 1 is chosen)
		std::vector<std::vector<int> > edgesGofT;	//adjacency matrix of the cBMG G(T,sigma)

		Tree();		//line 6

		void connect_trees(Tree subtree);		//line 16

		void build_hasse(std::vector<std::vector<int> > p, std::vector<Class> classes, int a, bool iN);	//line 45
		int compute_par(std::vector<int> &x, std::vector<std::vector<int> > p);	//line 121
		bool check_tree(std::vector<std::set<int> > r, std::vector<Vertex> v);	//line 143

		void find_leafSets(int n, std::vector<int> &subgraphs, std::vector<std::set<int> > &leafs, bool v);	//line 223
		int find_leafs(std::vector<int> subgraph, int n);	//line 249
		int pos_of_Vert(int n, std::vector<Vertex> v);	//lien 261

		bool build_aho(std::set<int> leafs, std::vector<std::vector<int> > triples, int p, std::vector<Vertex> vert, bool e);	//line 273

		void matrix_to_vector(std::vector<std::vector<int> > adj, std::vector<int> spec, std::vector<int> nr, std::vector<std::string> names, std::vector<Vertex> &vert, int nrIN);	//line 349
};

#endif //_TREE_H_
