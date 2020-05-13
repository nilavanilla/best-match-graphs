//Nicola Doering
//28/01/2020
//BA Best Match Graphs

#include "functions.h"

using namespace std;

//preparing the vertex sets for the two colored BMGs and connecting the least resolved trees of the components and computes the least resolved tree of the whole BMG "graph". Using the algorithm with number "alg" to check the properties of a 2-cBMG and adding a violated charateristic to "fails" in case of generating a dataset
bool build_lrTree(CDigraph &graph, int alg, vector<int> &fails)	{
	Tree t;
	vector<int> indVertSet;
	vector<vector<int> > indCompSet, indTrip;
	set<int> spec, vertSet;
	vector<set<int> > leafs;
	vector<int> v, subgraphs;
	vector<Vertex> vert;
	for (int i = 0; i < graph.connectedComponents.size(); i++)	{	//for every component of the n colored BMG do:
		for (int j = 0; j < graph.species.size()-1; j++)	{
			for (int h = j+1; h < graph.species.size(); h++)	{	//for every pair of species/colors do:
				graph.induced_vertexSet(j, h, indVertSet, i);	//collecting the vertices of the BMG with color "j" or "h" which have to be part of connectedComponents[i]
				spec.insert(j);
				spec.insert(h);
				if (!graph.find_conComp(indVertSet, indCompSet, spec, !fails.empty()))	{	//finding connected components in the induced vertex set "indVertSet"
					if (!fails.empty())	{
						fails[4]++;
					}
					return false;
				}
				for (int h = 0; h < indCompSet.size(); h++)	{	//checking every connected component of the two colored partitions of "graph" and constructing its LRT
					if (indCompSet[h].size() > 2)	{
						if (alg == 1)	{
							if (!alg_1(indCompSet[h], graph.edges, graph.vertices, indTrip))	{
								return false;								
							}
						}
						if (alg == 2)	{
							if(!alg_2(indCompSet[h], graph.edges, graph.vertices, indTrip, fails))	{
								return false;
							}
						}
					}
				}
				if (indCompSet.size() > 1)	{	//adding the triples that describe the relationship between the vertices of diferent connected components to "indTrip"
					find_triples(indCompSet, indTrip);
				}
				indVertSet.clear();
				indCompSet.clear();
				spec.clear();
			}
		}

		vertSet.insert(graph.connectedComponents[i].begin(), graph.connectedComponents[i].end());
		if (!t.build_aho(vertSet, indTrip, 0, graph.vertices, !fails.empty()))	{	//constructing the super tree of the LRTs of the two colored partitions of the considered connected component
			fails[7]++;
			return false;
		}

		if (graph.connectedComponents.size() > 1)	{	//adding the supertree of graph.connectedComponents[i] to "lrt"
			graph.lrt.connect_trees(t);	
		}
		else	{
			graph.lrt = t;
		}
		indTrip.clear();
		vert.clear();
		vertSet.clear();
		t.parent.clear();
		t.children.clear();
		t.vertex.clear();
		t.parent.push_back(-1);
		t.children.push_back({});
		t.vertex.push_back(-1);
	}

	subgraphs.push_back(0);
	leafs.push_back({});
	for (auto elem : graph.lrt.children[0])	{	//inserting the children of the root
		subgraphs.push_back(elem);
		leafs.push_back({});
	}

	for (int i = 0; i < graph.lrt.children[0].size(); i++)	{	//first part of constructing the BMG of "lrt", running through the children of the root to find the leaf sets of the subtrees rooted by the children
		graph.lrt.find_leafSets(i+1, subgraphs, leafs, true);
		leafs[0].insert(leafs[i+1].begin(), leafs[i+1].end());
	}
	graph.build_GofT(subgraphs, leafs, t);

	if (!graph.check_GofT(leafs[0], !fails.empty()))	{	//checking whether the constructed BMG is the same as the given BMG
		if (fails.empty())	{
			cout<<"the constructed BMG of the least resolved tree differs from the given BMG!\n";
		}
		else {
			fails[8]++;
		}
		return false;
	}
	return true;
}



//constructing the LRT of the two colored BMG "twocBMG" (with the colors of "spec" and vertices of "vert") by building the Hasse diagram of the hierarchy of leaf sets from the BMG, this hierarchical structure is formed by the reachable sets, also checking the two colored BMG with Theorem 3 from the paper because that is the requirement for the hierarchy and the Hasse diagram whether it is a tree and every pair of sibling R′(α) and R′(β) satisfy the requirement that its intersection is empty. The 2-cBMG is created with the vertex set "vertices" and therefore the adjecence matrix "adj" and vertex set "allVert" of the initial cBMG are needed. For later construction of the super tree the informative triples of the 2-cBMG are needed and are stored in "infTrip".
bool alg_1(vector<int> vertices, vector<vector<int> > adj, vector<Vertex> allVert, vector<vector<int> > &infTrip)	{
	CDigraph twocBMG(vertices, adj, allVert);
	twocBMG.find_classes(vertices);
	twocBMG.find_infTriples(vertices);
	infTrip.insert(infTrip.end(), twocBMG.infTriples.begin(), twocBMG.infTriples.end());
	if (!twocBMG.check_N2())	{	//checking whether Theorem 3 is satisfied
		cout << "in the two colored BMG with vertices { ";
		for (int i = 0; i < twocBMG.vertices.size(); i++)	{
			cout << twocBMG.vertices[i].mark << " ";
		}
		cout << "} is property N2 of Theorem 3 violated\n";
		return false;
	}
	if (!twocBMG.check_N3())	{
		cout << "in the two colored BMG with vertices { ";
		for (int i = 0; i < twocBMG.vertices.size(); i++)	{
			cout << twocBMG.vertices[i].mark << " ";
		}
		cout << "} is property N3 of Theorem 3 violated\n";
		return false;
	}
	if (!twocBMG.check_N1())	{
		cout << "in the two colored BMG with vertices { ";
		for (int i = 0; i < twocBMG.vertices.size(); i++)	{
			cout << twocBMG.vertices[i].mark << " ";
		}
		cout << "} is property N1 of Theorem 3 violated\n";
		return false;
	}

	vector<set<int> > r;	//computing R' for every class of the "twocBMG"
	for (int i = 0; i < twocBMG.classes.size(); i++)	{
		r.push_back({});
		for (int j = 0; j < twocBMG.classes.size(); j++)	{
			if (twocBMG.classes[i].neighbors[3] == twocBMG.classes[j].neighbors[3] && twocBMG.classes[j].neighbors[0] <= twocBMG.classes[i].neighbors[0])	{
				r[i].insert(twocBMG.classes[j].members.begin(), twocBMG.classes[j].members.end());
			}
		}
		r[i].insert(twocBMG.classes[i].neighbors[4].begin(), twocBMG.classes[i].neighbors[4].end());
	}

	vector<vector<int> > p(twocBMG.classes.size(),vector<int>(twocBMG.classes.size(), 0));	//tabulating P(a,b) = 1 if R'(a) <= R'(b) of the classes "a" and "b"
	for (int i = 0; i < twocBMG.classes.size(); i++)	{
		for (int j = i; j < twocBMG.classes.size(); j++)	{
			if (includes(r[j].begin(), r[j].end(), r[i].begin(), r[i].end()))	{
				p[i][j] = 1;
			}
			if (includes(r[i].begin(), r[i].end(), r[j].begin(), r[j].end()))	{	
				p[j][i] = 1;
			}
		}
	}

	twocBMG.lrt.build_hasse(p, twocBMG.classes, 0, false);	//constructing the Hasse diagram by using the hierarchy of classes tabulated in "p"
	if (!twocBMG.lrt.check_tree(r, twocBMG.vertices))	{	//checking whether the constructed Hasse diegram holds the characterizations of a tree whehter there are siblings R'(a) & R'(b) with non-empty intersection
		return false;
	}	
	return true;
}



//computing the LRT of the two colored BMG "twocBMG" (with the colors of "spec" and vertices of "vert") by constructing the Aho tree of the informative triples of the 2-cBMG. Checking whether the set of informative triples is consistent and whether the cBMG that is displayed by the constructed Aho tree is similar to "twocBMG". The 2-cBMG is created with the vertex set "vertices" and therefore the adjecence matrix "adj" and vertex set "allVert" of the initial cBMG are needed. For later construction of the super tree the informative triples of the 2-cBMG are needed and are stored in "infTrip". The failed properties of an 2-cBMG are collected in "fails" in case of generating a data set. 
bool alg_2(vector<int> vertices, vector<vector<int> > adj, vector<Vertex> allVert, vector<vector<int> > &infTrip, vector<int> &fails)	{
	CDigraph twocBMG(vertices, adj, allVert);
	twocBMG.find_infTriples(vertices);
	set<int> vertSet;
	vector<set<int> > leafs;
	vector<int> subgraphs;
	Tree t;
	copy(vertices.begin(), vertices.end(), inserter(vertSet, vertSet.begin()));
	infTrip.insert(infTrip.end(), twocBMG.infTriples.begin(), twocBMG.infTriples.end());
	adj.clear();
	if (twocBMG.lrt.build_aho(vertSet, twocBMG.infTriples, 0, twocBMG.vertices, !fails.empty()))	{	//constructing the aho tree of the informative triples
		subgraphs.push_back(0);
		leafs.push_back({});
		for (auto elem : twocBMG.lrt.children[0])	{	//constructing the cBMG of the aho tree
			subgraphs.push_back(elem);
			leafs.push_back({});
		}
		for (int i = 0; i < twocBMG.lrt.children[0].size(); i++)	{
			twocBMG.lrt.find_leafSets(i+1, subgraphs, leafs, true);
			leafs[0].insert(leafs[i+1].begin(), leafs[i+1].end());
		}
		twocBMG.build_GofT(subgraphs, leafs, t);

		if (!twocBMG.check_GofT(vertSet, !fails.empty()))	{	//checking whether the cBMG displayed of the aho tree is similar to ""twocBMG"
			if (!fails.empty())	{
				fails[6]++;
			}
			return false;
		}
	}
	else	{
		if (!fails.empty())	{
			fails[5]++;
		}
		return false;
	}
	return true;
}



//collecting the triples that descibe the realtionship between the vertices of the different components in "indComp" and storing them in "indTrip"
void find_triples(vector<vector<int> > indComp, vector<vector<int> > &indTrip)	{
	for (int i = 0; i < indComp.size(); i++)	{
		for (int j = 0; j < indComp.size(); j++)	{
			if (j != i)	{
				for (int x = 0; x < indComp[i].size(); x++)	{
					for (int y = x+1; y < indComp[i].size(); y++)	{
						for (int z = 0; z < indComp[j].size(); z++)	{
							indTrip.push_back({indComp[i][x], indComp[i][y], indComp[j][z]});
						}
					}
				}
			}
		}
	}
}

