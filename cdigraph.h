//Nicola Doering
//28/01/2020
//BA Best Match Graphs

//Header-Data of the directed graph 

#ifndef _CDIGRAPH_H_
#define _CDIGRAPH_H_

#include "tree.h"

class CDigraph	{
	public:
		std::vector<Vertex> vertices;
		std::set<int> species;
		std::vector<Class> classes;
		std::vector<std::vector<int> > connectedComponents;
		std::vector<std::vector<int> > infTriples;
		std::vector<std::vector<int> > edges;
		Tree lrt;
		
		CDigraph()	{};
		CDigraph(std::vector<Vertex> ver, std::set<int> spec);		//line 9
		CDigraph(std::vector<int> ver, std::vector<std::vector<int> > adj, std::vector<Vertex> allVert);	//line 16

		int pos_of_Vert(int nr);		//line 39
		bool find_conComp(std::vector<int> indVertSet, std::vector<std::vector<int> > &indCompSet, std::set<int> indSpec, bool e);		//line 47
		void induced_vertexSet(int s, int t, std::vector<int> &indVertSet, int c);		//line 116

		void find_classes(std::vector<int> vert);	//line 127
		bool check_N1();		//line 177
		bool check_N2();		//line 216
		bool check_N3();		//line 228

		void find_infTriples(std::vector<int> v);		//line 252
		void build_GofT(std::vector<int> subgraphs, std::vector<std::set<int> > leafs, Tree t0);
		bool check_GofT(std::set<int> v, bool e);		//line 400
};
#endif //_CDIGRAPH_H_
