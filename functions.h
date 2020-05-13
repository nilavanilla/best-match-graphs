//Nicola Doering
//28/01/2020
//BA Best Match Graphs

#ifndef _FUNCTIONS_H_
#define _FUNCTIONS_H_

#include "cdigraph.h"

//defined in dataCollection.cc
long collect_data(int nr, int nrL, int nrS, int nrDel, int nrR);	//line 9
void print_progress(const float& percentage);	//line 218

//defined in drawing.cc
void draw_graph(CDigraph graph);

//defined in buildLRT.cc
bool build_lrTree(CDigraph &graph, int alg, std::vector<int> &fails);	//line 9
bool alg_1(std::vector<int> vert, std::vector<std::vector<int> > adj, std::vector<Vertex> allVert, std::vector<std::vector<int> > &infTriples);	//line 103
bool alg_2(std::vector<int> vert, std::vector<std::vector<int> > adj, std::vector<Vertex> allVert, std::vector<std::vector<int> > &infTriples, std::vector<int> &fails);	//line 166
void find_triples(std::vector<std::vector<int> > indComp, std::vector<std::vector<int> > &indTrip);	//line 208

#endif //_FUNCTIONS_H_
