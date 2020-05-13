//Nicola Doering
//28/01/2020
//BA Best Match Graphs

#include "cdigraph.h"

using namespace std;

CDigraph::CDigraph(vector<Vertex> v, set<int> spec)	{
	vertices = v;
	species = spec;
	vector<int> e(v.size(), 0);
	for (int i = 0; i < v.size(); i++)	{
		edges.push_back(e);
	}
}



CDigraph::CDigraph(vector<int> v, vector<vector<int> > adj, vector<Vertex> allVert)	{	
	for (int i = 0; i < v.size(); i++)	{
		vertices.push_back(allVert[v[i]]);
		species.insert(allVert[v[i]].species);
	}
	edges = adj;
}



//returns the position in "vert" of the vertex with number "nr"
int CDigraph::pos_of_Vert(int nr)	{
	for (int i = 0; i < vertices.size(); i++)	{
		if (vertices[i].number == nr)	{
			return i;
		}
	}
	return -1;
}



//finds connected components in the whole BMG or only in a part of it which is given by "indvertSet", the components are written in "connectedComponents" or "indCompSet" where sub vector c(i) contains the vertices of component i, also checks whether every component contains at least one node of every species/color
bool CDigraph::find_conComp(vector<int> indVertSet, vector<vector<int> > &indCompSet, set<int> indSpec, bool e)	{
	vector<int> adjacents, component;
	set<int> connected, vertSet, colorSet, spec;
	vector<Vertex> vert;
	int v;

	if (indVertSet.empty())	{	//to decide whether the connected components shall be found in the n colored BMG or in the two colored BMG
		vert = vertices;
		spec = species;
	} 
	else	{
		for (auto i : indVertSet)	{
			vert.push_back(vertices[pos_of_Vert(i)]);
			vertSet.insert(i);
		}
		spec = indSpec;
	}

	for (int i = 0; i < vert.size(); i++)	{	
		v = vert[i].number;
		if (connected.find(v) == connected.end())	{	//checks whether the vertex with number "v" is already part of a component what means that it was already considered
			connected.insert(v);
			colorSet.insert(vertices[pos_of_Vert(v)].species);
			component.push_back(v);	//if the vertex does not already contains to a component a new component is built
			for (int j = 0; j < edges.size(); j++)	{
				if ((edges[v][j] == 1 || edges[j][v] == 1) && (vertSet.find(j) != vertSet.end() || indVertSet.empty()) && connected.find(j) == connected.end())	{
					adjacents.push_back(j);		//inserts all in- and out-neighbors of vert[i] to "adjacents"
					connected.insert(j);
				}
			}
			while (!adjacents.empty())	{
				v = adjacents.back();	
				adjacents.pop_back();
				colorSet.insert(vertices[pos_of_Vert(v)].species);
				component.push_back(v);	//inserts the vertex to the component
				for (int j = 0; j < edges.size(); j++)	{
					if ((edges[v][j] == 1 || edges[j][v] == 1) && (vertSet.find(j) != vertSet.end() || indVertSet.empty()) && connected.find(j) == connected.end())	{
						adjacents.push_back(j);		//inserts all in- and out-neighbors of vert[i] to "adjacents"
						connected.insert(j);
					}
				}
			}

			if (colorSet != spec)	{	//checks whether every component has the same color set
				if (!e)	{
					cout << "component ( ";
					for (int j = 0; j < component.size(); j++)	{
						cout << vertices[pos_of_Vert(component[j])].mark << " ";
					}
					cout << ") does not contain all species!\n";
				}
				return false;
			}

			if (indVertSet.empty())	{	//if the connected components shall be found in the n colored BMG
				connectedComponents.push_back(component);
			}
			else	{	//if the connected components shall be found in the two colored BMG
				indCompSet.push_back(component);
			}
			colorSet.clear();
			component.clear();
		}
	}
	return true;
}


//collecting the nodes of the c'th connected component with color s or t in the vector "indvertSet"
void CDigraph::induced_vertexSet(int s, int t, vector<int> &indVertSet, int c)	{
	for (int i = 0; i < connectedComponents[c].size(); i++)	{
		if (vertices[pos_of_Vert(connectedComponents[c][i])].species == s || vertices[pos_of_Vert(connectedComponents[c][i])].species == t)	{
			indVertSet.push_back(connectedComponents[c][i]);
		}
	}
}



//finds the equivalence classes of the vertices of the BMG and stores them in "classes", vertices are part of the same class if the have the same in- and out-neighborhood
void CDigraph::find_classes(vector<int> v)	{
	set<int> inNeigh;
	set<int> outNeigh;
	bool inserted = false;

	for (int i = 0; i < edges.size(); i++)	{//getting the neighborhood of Vertex with number vert[0] in the two speciesed BMG and stores the in-neighbors in "inNeigh" and the out-neighbors in "outNeigh"
		if (edges[v[0]][i] == 1 && find(v.begin(), v.end(), i) != v.end())	{
			outNeigh.insert(i);
		}
		if (edges[i][v[0]] == 1 && find(v.begin(), v.end(), i) != v.end())	{
			inNeigh.insert(i);
		}
	}
	Class c(vertices[pos_of_Vert(v[0])].species, {v[0]}, inNeigh, outNeigh, edges, v, classes.size());	//constructs the first equivalence class with member vertices[0]
	classes.push_back(c);
	outNeigh.clear();
	inNeigh.clear();

	for (int j = 1; j < v.size(); j++)	{
		for (int i = 0; i < edges.size(); i++)	{
			if (edges[v[j]][i] == 1 && find(v.begin(), v.end(), i) != v.end())	{
				outNeigh.insert(i);
			}
			if (edges[i][v[j]] == 1 && find(v.begin(), v.end(), i) != v.end())	{
				inNeigh.insert(i);
			}
		}
		for (int i = 0; i < classes.size() && !inserted; i++)	{	//run through "classes" until the vertex vertices[i] is inserted in a matching class or till the end of "classes"
			if (inNeigh == classes[i].neighbors[3] && outNeigh == classes[i].neighbors[0] && vertices[pos_of_Vert(v[j])].species == classes[i].species)	{
				classes[i].members.push_back(v[j]);	//inserts the vertex to the members of class classes[j] if they have the same neighborhood and the same color
				inserted = true;
			}
		}
		if (inserted)	{
			inserted = false;
		}
		else	{	//if vertices[i] is not already inserted to an existing class a new equvalenc class is constructed with member vertices[i]
			Class c(vertices[pos_of_Vert(v[j])].species, {v[j]}, inNeigh, outNeigh, edges, v, classes.size());	
			classes.push_back(c);
		}
		outNeigh.clear();
		inNeigh.clear();
	}
}	



//checking whether (a intersect N(b)) = (b intersect N(a)) = empty -> (N(a) intersect N(n(b))) = (N(b) intersect N(N(a))) = empty holds for all equivalence classes a and b in "classes"
bool CDigraph::check_N1()	{
	set<int> intersect1;
	set<int> intersect2;
	vector<vector<int> > y(classes.size(),vector<int>(classes.size(), 0));	//table Y
	for (int i = 0; i < classes.size()-1; i++)	{
		for (int j = i+1; j < classes.size(); j++)	{
			set_intersection(classes[i].neighbors[0].begin(), classes[i].neighbors[0].end(), classes[j].neighbors[1].begin(), classes[j].neighbors[1].end(), inserter(intersect1, intersect1.begin()));	//computing the intersection of N(a) and N(N(b))
			if (!intersect1.empty())	{	//tabulating Y(a,b) = 1 if N(a) intersect N(N(b)) = empty
				y[i][j] = 1;
			}
			intersect1.clear();
			set_intersection(classes[j].neighbors[0].begin(), classes[j].neighbors[0].end(), classes[i].neighbors[1].begin(), classes[i].neighbors[1].end(), inserter(intersect1, intersect1.begin()));	//computing the intersection of N(b) and N(N(a))
			if (!intersect1.empty())	{	//tabulating Y(b,a) = 1 if N(b) intersect N(N(a)) = empty
				y[j][i] = 1;
			}
			intersect1.clear();
		}
	}

	for (int i = 0; i < classes.size()-1; i++)	{
		for (int j = i+1; j < classes.size(); j++)	{
			set_intersection(classes[i].members.begin(), classes[i].members.end(), classes[j].neighbors[0].begin(), classes[j].neighbors[0].end(), inserter(intersect1, intersect1.begin())); //computing the intersection of a and N(b)
			set_intersection(classes[j].members.begin(), classes[j].members.end(), classes[i].neighbors[0].begin(), classes[i].neighbors[0].end(), inserter(intersect2, intersect2.begin()));	//computing the intersection of b and N(a)
			if (intersect1.empty() && intersect2.empty())	{
				if (!(y[i][j] == 0 && y[j][i] == 0))	{
					return false;
				}
			}
			intersect1.clear();
			intersect2.clear();
		}
	}
	return true;
}



//checking whether N(N(N(a))) <= N(a) holds for all equivalence classes in "classes"
bool CDigraph::check_N2()	{
	int v;
	for (int i = 0; i < classes.size(); i++)	{
		if (!includes(classes[i].neighbors[0].begin(), classes[i].neighbors[0].end(), classes[i].neighbors[2].begin(), classes[i].neighbors[2].end()))	{
			return false;
		}
	}
	return true;
}



//checking whether (a intersect N(N(b))) = (b intersect N(N(a))) = empty & (N(a) intersect N(b)) != empty -> N-(a) = N-(b)  & N(a) <= N(b) or N(b) <= N(a)
bool CDigraph::check_N3()	{
	set<int> aIntersectNNb;
	set<int> bIntersectNNa;
	set<int> NaIntersectNb;
	for (int i = 0; i < classes.size()-1; i++)	{
		for (int j = i+1; j < classes.size(); j++)	{
			set_intersection(classes[i].members.begin(), classes[i].members.end(), classes[j].neighbors[1].begin(), classes[j].neighbors[1].end(), inserter(aIntersectNNb, aIntersectNNb.begin()));	//computing intersection of a and N(N(b))
			set_intersection(classes[j].members.begin(), classes[j].members.end(), classes[i].neighbors[1].begin(), classes[i].neighbors[1].end(), inserter(bIntersectNNa, bIntersectNNa.begin()));	//computing intersection of b and N(N(a))
			set_intersection(classes[i].neighbors[0].begin(), classes[i].neighbors[0].end(), classes[j].neighbors[0].begin(), classes[j].neighbors[0].end(), inserter(NaIntersectNb, NaIntersectNb.begin()));	//computing intersection of N(a) and N(b)
			if (aIntersectNNb.empty() && bIntersectNNa.empty() && !NaIntersectNb.empty())	{
				if (!(classes[i].neighbors[3] == classes[j].neighbors[3] && (includes(classes[j].neighbors[0].begin(), classes[j].neighbors[0].end(), classes[i].neighbors[0].begin(), classes[i].neighbors[0].end()) || includes(classes[i].neighbors[0].begin(), classes[i].neighbors[0].end(), classes[j].neighbors[0].begin(), classes[j].neighbors[0].end()))))	{
					return false;
				}
			}
			aIntersectNNb.clear();
			bIntersectNNa.clear();
			NaIntersectNb.clear();
		}
	}
	return true;
}



//finding the informative triples in the BMG and save them in "infTriples"
void CDigraph::find_infTriples(vector<int> v)	{
	int a, b, x, y, z;
	int c = 0;	//needs a value that is a position in vertices because of the last if condition, if c wasn't assigned an other value "firstchar" is false
	bool assigned = false;
	bool firstchar;	//checks whether the first characteristic (which all triples satisfy -> b is outneighbor of a) is satisfied
	for (int i = 0; i < v.size()-2; i++)	{
		for (int j = i+1; j < v.size()-1; j++)	{
			for (int h = j+1; h < v.size(); h++)	{
				x = pos_of_Vert(v[i]);
				y = pos_of_Vert(v[j]);
				z = pos_of_Vert(v[h]);

				//to mark the vertices like in fig 8 (page 16) and to shorten the if cases to check whether the subgraph looks like X1, X2, X3 or X4
				firstchar = true;
				if (vertices[x].species == vertices[y].species)	{
					a = z;
					if (edges[vertices[a].number][vertices[x].number] == 1)	{
						b = x;
						c = y;
					}
					else if (edges[vertices[a].number][vertices[y].number] == 1)	{
						b = y;
						c = x;
					}
					else {
						firstchar = false;
					}
				}
				else if (vertices[x].species == vertices[z].species){
					a = y;
					if (edges[vertices[a].number][vertices[x].number] == 1)	{
						b = x;
						c = z;
					}
					else if (edges[vertices[a].number][vertices[z].number] == 1)	{
						b = z;
						c = x;
					}
					else	{
						firstchar = false;
					}				
				}
				else if (vertices[y].species == vertices[z].species)	{
					a = x;
					if (edges[vertices[a].number][vertices[y].number] == 1)	{
						b = y;
						c = z;
					}
					else if (edges[vertices[a].number][vertices[z].number] == 1)	{
						b = z;
						c = y;
					}
					else	{
						firstchar = false;
					}
				}
				else	{
					a = 0;
					firstchar = false;
				}
				if (edges[vertices[a].number][vertices[c].number] == 0 && firstchar)	{	//exclude that a has no outneighbors or two outneighbors in the subgraph
																						//->the subgraph is now isomorph to X1, X2, X3 or X4
						infTriples.push_back({vertices[a].number, vertices[b].number, vertices[c].number});
				}
			}
		}
	}
}



//Running through "leafs" to add the vertex numbers of all leafs to the out-neighbors of the considered leaf if they have an other color and lay at the same level or under the considered leaf. If there
void CDigraph::build_GofT(vector<int> subgraphs, vector<set<int> > leafs, Tree t0)	{
	int pos1, pos2, par;
	set<int> insSpec;
	set<int> diff;
	bool ins = false;
	vector<int> a(edges.size(), 0);
	vector<vector<int> > adj(edges.size(), a);
	Tree t;
	if (t0.parent.size() == 1)	{
		t = lrt;
	}
	else	{
		t = t0;
	}
	for (int i = 0; i < leafs.size(); i++)	{
		if (leafs[i].size() == 1)	{
			par = t.parent[subgraphs[i]];
			pos1 = pos_of_Vert(*leafs[i].begin());
			diff = species;
			diff.erase(vertices[pos1].species);
			while (!diff.empty())	{
				pos2 = t.find_leafs(subgraphs, par);
				for (auto x : leafs[pos2])	{
					if (diff.find(vertices[pos_of_Vert(x)].species) != diff.end())	{
						adj[vertices[pos1].number][vertices[pos_of_Vert(x)].number] = 1;
						insSpec.insert(vertices[pos_of_Vert(x)].species);
					}
				}
				for (auto s : insSpec)	{
					diff.erase(s);
				}
				insSpec.clear();
				if (par != 0)	{
					par = t.parent[par];
				}
			}
		}
	}    
	if (t0.parent.size() == 1)	{
		lrt.edgesGofT = adj;
	}
	else	{
		edges = adj;
	}
}



//checking whether the constructed BMG of the least resolved tree with leaf set "vertSet" displays the part of the given BMG with vertex set "vertSet"
bool CDigraph::check_GofT(set<int> v, bool e)	{
	for (int i = 0; i < vertices.size(); i++)	{
		for (int j = 0; j < edges.size(); j++)	{
			if (edges[vertices[i].number][j] != lrt.edgesGofT[vertices[i].number][j] && v.find(j) != v.end())	{
				if (!e)	{
					cout << vertices[i].mark << " has not the same neighborhood in the BMG that displays the LRT with leaf set { ";
					for (int j = 0; j < vertices.size(); j++)	{
						cout << vertices[j].mark << " ";
					}
					cout << "} and the given BMG!\n";
				}
				return false;
			}
		}
	}
	return true;
}			
