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
		CDigraph(std::vector<Vertex> ver, std::set<int> spec);		//line 5
		CDigraph(std::vector<int> ver, std::vector<std::vector<int> > adj, std::vector<Vertex> allVert);	//line 12

		int pos_of_Vert(int nr);		//line 35
		bool find_conComp(std::vector<int> indVertSet, std::vector<std::vector<int> > &indCompSet, std::set<int> indSpec, bool e);		//line 47
		void induced_vertexSet(int s, int t, std::vector<int> &indVertSet, int c);		//line 112

		void find_classes(std::vector<int> vert);	//line 123
		bool check_N1();		//line 173
		bool check_N2();		//line 212
		bool check_N3();		//line 224

		void find_infTriples(std::vector<int> v);		//line 248
		void build_GofT(std::vector<int> subgraphs, std::vector<std::set<int> > leafs, Tree t0);
		bool check_GofT(std::set<int> v, bool e);		//line 396
};
#endif //_CDIGRAPH_H_
