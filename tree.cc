//Nicola Doering
//28/01/2020
//BA Best Match Graphs

#include "tree.h"

using namespace std;


//constructing the first entry in the vectors which represents the root of the tree
Tree::Tree()	{
	parent.push_back(-1);
	children.push_back({});
	vertex.push_back(-1);
	classes.push_back(-1);
}



//connecting "subtree" to the Tree by adding the vectors "children", "parent" and "vertex" of "subtree" to those of the Tree and modifies them, the root of "subtree" will be a children of the tree
void Tree::connect_trees(Tree subtree)	{
	set<int> toInsert;
	int pos = parent.size();
	parent.insert(parent.end(), subtree.parent.begin(), subtree.parent.end());
	parent[pos] = 0;
	children.insert(children.end(), subtree.children.begin(), subtree.children.end());
	children[0].insert(pos);
	for (auto elem : children[pos])	{
		toInsert.insert(elem + pos);
	}
	children[pos].clear();
	children[pos] = toInsert;
	toInsert.clear();

	for (int j = pos + 1; j < parent.size(); j++)	{
		parent[j] = parent[j] + pos;
		for (auto elem : children[j])	{
			toInsert.insert(elem + pos);
		}
		children[j].clear();
		children[j] = toInsert;
		toInsert.clear();
	}
	vertex.insert(vertex.end(), subtree.vertex.begin(), subtree.vertex.end());
}



//constructing the hasse diagram by the hierarchy of the equivalence classes of the BMG, the hierarchy is based on the matrix "p", "classes" contains the classes of the BMG in the order like they are represented in "p", int "a" is the pos of the tree node that is the root of the subtree with the leaf set "classes", the function is used recursively so "innerNode" tells the next run through the function whether an inner node have to be constructed  
void Tree::build_hasse(vector<vector<int> > p, vector<Class> cl, int a, bool iN)	 {
	vector<int> x;
	vector<int> delP, delNP;	//contain the indices of the lines and rows of "p" if it shall be parted in two matrices
	vector<Class> newClasses = cl;
	vector<vector<int> > newP = p;
	int z = compute_par(x, p);

	if (z == p.size() && !iN)	{
		if (iN)	{
			parent.push_back(a);	//constructing a new inner node
			classes.push_back(-1);
			children.push_back({});
			children[a].insert(children.size()-1);
			a = parent.size()-1;
		}
		for (int i = x.size()-1; i >= 0; i--)	{	//construct new leafs that are inserted in the Hasse diagram
			parent.push_back(a);
			classes.push_back(cl[x[i]].number);
			children.push_back({});
			children[a].insert(children.size()-1);
			newClasses.erase(newClasses.begin()+x[i]);	//remove the inserted classes
			for (int j = 0; j < newP.size(); j++)	{
				newP[j].erase(newP[j].begin()+x[i]);
			}
			newP.erase(newP.begin()+x[i]);
		}
		if (!newP.empty())	{
			build_hasse(newP, newClasses, a, true);	//calling the function to construct the Hasse diagram for the residual classes, "innerNode" is false because R'(a) < R'(b) holds for all residual classes a and all inserted classes b
		}
	}

	else	{
		while (!p.empty())	{
			parent.push_back(a);	//constructing a new inner node that becomes the root of the subtree with leaf set "newClasses"
			classes.push_back(-1);
			children.push_back({});
			children[a].insert(children.size()-1);
			for (int i = 0; i < p.size(); i++)	{	//parting "p" in the matrix "newP" that represents the classes which are hierachically comparable with the class with index x[0] and the matrix "p" of all other classes
				if (p[i][x[0]] == 1)	{
					delP.push_back(i);
				}
				else	{
					delNP.push_back(i);
				}
			}
			for (int i = delP.size()-1; i >= 0; i--)	{
				cl.erase(cl.begin()+delP[i]);
				p.erase(p.begin()+delP[i]);
				for (int j = 0; j < p[0].size(); j++)	{
					p[j].erase(p[j].begin()+delP[i]);
				}
			}
			for (int i = delNP.size()-1; i >= 0; i--)	{
				newClasses.erase(newClasses.begin()+delNP[i]);
				newP.erase(newP.begin()+delNP[i]);
				for (int j = 0; j < newP[0].size(); j++)	{
					newP[j].erase(newP[j].begin()+delNP[i]);
				}
			}
			delP.clear();
			delNP.clear();
			build_hasse(newP, newClasses, parent.size()-1, false);

			if (!p.empty())	{
				x.clear();
				z = compute_par(x, p);
			}
			newP = p;
			newClasses = cl;
		}
	}
}	



//returning the max number of 1s in the columns of the matrix "p", stores the index of the columns with the most 1s in the vector "x"
int Tree::compute_par(vector<int> &x, vector<vector<int> > p)	{
	int y = 0;	//counts the 1s in the column
	int z = 0;	//maximal y
	for (int i = 0; i < p.size(); i++)	{
		for (int j = 0; j < p.size(); j++)	{
			y = p[j][i]+y;
		}
		if (y >= z)	{
			if ( y != z)	{
				x.clear();
				z = y;
			}
			x.push_back(i);
		}
		y = 0;
	}
	return z;
}



//checking whether the constructed Hasse diagram is a tree so it is acyclic (the recursive construction of the Hasse diagram guarantees that the tree is connected) and whether there are siblings R'(a) & R'(b) with non-empty intersection
bool Tree::check_tree(vector<set<int> > r, vector<Vertex> v)	{
	set<int> c;	//saving the children of the allready visited nodes
	for (int i = 0; i < children.size(); i++)	{	//checking whether the tree is Hasse diagram is acyclic)
		for (auto j : children[i])	{
			if (c.find(j) != c.end())	{
				cout << "the constructed Hasse-Diagram of the vertices { ";
				for (auto h : v)	{
					cout << h.mark << " ";
				}
				cout << "} contains a circle!\n";
				return false;
			}
			c.insert(j);
		}
	}
	c.clear();

	set<int> setunion;
	vector<set<int>> leafs, toVisit, toCompare, x;	//toVisit: position of that still have to be visited, toCompare: sets of numbers of classes which extended reachable sets are compared
	vector<int> subgraphs;
	bool found = false;
	subgraphs.push_back(0);
	leafs.push_back({});
	for (auto elem : children[0])	{
		subgraphs.push_back(elem);
		leafs.push_back({});
	}
	for (int i = 1; i <= children[0].size(); i++)	{
		find_leafSets(i, subgraphs, leafs, false);
		leafs[0].insert(leafs[i].begin(), leafs[i].end());
	}		
	toVisit.push_back(children[0]);
	subgraphs.erase(subgraphs.begin());
	leafs.erase(leafs.begin());
	while (!toVisit.empty())	{	//considering all siblings in the Hasse diagram
		for (auto i : toVisit[0])	{
			if (classes[i] == -1)	{
				for (int j = 0; j < subgraphs.size() && !found; j++)	{
					if (subgraphs[j] == i)	{
						toCompare.push_back(leafs[j]);
						subgraphs.erase(subgraphs.begin()+j);
						leafs.erase(leafs.begin()+j);
						found = true;
					}
				}
				x.push_back(children[i]);
				found = false;
			}				
		}
		toVisit.erase(toVisit.begin());
		toVisit.insert(toVisit.end(), x.begin(), x.end());
		x.clear();
		if (toCompare.size() > 1)	{
			for (int i = 0; i < toCompare.size(); i++)	{	//uniting the extended reachable sets of all leafs of the considered siblings
				for (auto j : toCompare[i])	{
					setunion.insert(r[j].begin(), r[j].end());
				}
				x.push_back(setunion);
				setunion.clear();
			}
			for (int i = 0; i < x.size()-1; i++)	{	//checking whether the intersections of these unions are empty which implies that the intersection of every single class of the siblings is empty too
				for (int j = i+1; j < x.size(); j++)	{
					set_intersection(x[i].begin(), x[i].end(), x[j].begin(), x[j].end(), inserter(c, c.begin()));
					if (!c.empty())	{
						return false;
					}
					c.clear();
				}
			}
			x.clear();
			toCompare.clear();
		}
	}
	return true;
}




//running through the treenodes, saving their tree vector index in "subgraphs" and saving the vertex or class numbers of the leaf set of the subtree of the considered tree node at the same index in "leafs" like the position in "subgraphs". "i" is the position of the node "subgraphs" that is considered in this run of the function after that the function is called recursively for the children of the i'th node. "v" is true if vertices are considered and false if classes are considered
void Tree::find_leafSets(int i, vector<int> &subgraphs, vector<set<int> > &leafs, bool v)	{
	if (children[subgraphs[i]].empty())	{	//adding the vertex or class number to leafs[i]
		if (v)	{
			leafs[i].insert(vertex[subgraphs[i]]);
		}
		else	{
			leafs[i].insert(classes[subgraphs[i]]);
		}
	}

	else	{	//adding more tree nodes to subgraphs and calling the function for them 
		int x = subgraphs.size();
		for (auto elem : children[subgraphs[i]])	{
			subgraphs.push_back(elem);
			leafs.push_back({});
		}
		for (int j = 0; j < children[subgraphs[i]].size(); j++)	{
			find_leafSets(x+j, subgraphs, leafs, v);
			leafs[i].insert(leafs[x+j].begin(), leafs[x+j].end());
		}
	}
}	



//returning the position of the entry "n" in the vector "subgraph". returning "-1" if "n" cannot be found
int Tree::find_leafs(vector<int> subgraph, int n)	{
	for (int i = 0; i < subgraph.size(); i++)	{
		if (subgraph[i] == n)	{
			return i;
		}
	}
	return -1;
}



//returning the position of the vertex with number "n" in the vector "v". returning "-1" if such a vertex cannot be found
int Tree::pos_of_Vert(int n, vector<Vertex> v)	{
	for (int i = 0; i < v.size(); i++)	{
		if (v[i].number == n)	{
			return i;
		}
	}
	return -1;
}



//BUILD algorithm to construct an Aho-graph. function is called recursively with the currently considered set of vertices "leafs" and the position "p" of the node in the tree which is the root of the subtree with the leaf set "leafs", the informative triples of the digraph are stored in "triples", "vert" contains the vertices of the digraph and "e" is true if the output does not shall be shown while data collection, otherwise "e" is false
bool Tree::build_aho(set<int> leafs, vector<vector<int> > triples, int p, vector<Vertex> vert, bool e)	{
	vector<set<int> > components;	//set of connected components in the Aho-graph of vertex set "leafs"
	int added = -1;
	bool connected = false;
	set<int> inComp;

	for (int i = 0; i < triples.size(); i++)	{	//constructing the aho graph of leaf set "leafs"
		if (leafs.find(triples[i][0]) != leafs.end() && leafs.find(triples[i][1]) != leafs.end() && leafs.find(triples[i][2]) != leafs.end())	{
			for (int j = 0; j < components.size(); j++)	{
				if (components[j].find(triples[i][0]) != components[j].end() || components[j].find(triples[i][1]) != components[j].end())	{
					if (added == -1)	{	
						components[j].insert(triples[i][0]);
						components[j].insert(triples[i][1]);
						inComp.insert(triples[i][0]);
						inComp.insert(triples[i][1]);
						added = j;	//saves the index of the component where the vertices are added
					}
					else	{	//if an other component contains the other part of the tripel the component component[added] is inserted to this one
						components[j].insert(components[added].begin(), components[added].end());
						connected = true;
					}
				}
			}
			if (added == -1)	{	//constructing a new connected component in the Aho-graph if there is not an existing component that contains the first or second part of the triple
				components.push_back({triples[i][0], triples[i][1]});
				inComp.insert(triples[i][0]);
				inComp.insert(triples[i][1]); 
			}
			else if (connected)	{
				components.erase(components.begin()+added);
			}
			added = -1;
			connected = false;
		}
	}
	for (auto elem : leafs)	{	//adding the remaining leafs to the Aho-graph
		if (inComp.find(elem) == inComp.end())	{
			components.push_back({elem});
		}
	}
  
	if (components.size() == 1)	{	//checking whether the triple set is consistent
		if (!e)	{
			cout << "the triple set is not consistent in leaf set { ";
			for (auto elem : leafs)	{
				cout << vert[pos_of_Vert(elem, vert)].mark << " ";   
			}
			cout<<"}!\n";
		}
		return false;
	}

	for (int i = 0; i < components.size(); i++)	{
		if (components[i].size() == 1)	{	//adding the vertex of the single vertex component to the tree as a child of the tree node of position "p"
			parent.push_back(p);
			vertex.push_back(*components[i].begin());
			children.push_back({});
			children[p].insert(children.size()-1);
		}
		
		else	{	//adding a new node to the tree that becomes the root of the subtree with the leaf set components[i]
			parent.push_back(p);
			vertex.push_back(-1);
			children.push_back({});
			children[p].insert(children.size()-1);
			if (!build_aho(components[i], triples, children.size()-1, vert, e))	{	//repeating the function with components[i]
				return false;
			}
		}
	}
	return true;
}



//converting the adjacence matirx "adj" of a given tree to the vectors "parent","children" and "vertex" that are used in this program. The properties of the leafs in the tree and later of the vertices in the cBMG are given in the vectors "spec", "nr" and optionally "names". The vertices of the cBMG will be added to "vert". The number of inner nodes in the tree is given by "nrIN".
void Tree::matrix_to_vector(vector<vector<int> > adj, vector<int> spec, vector<int> nr, vector<string> names, vector<Vertex> &vert, int nrIN)	{
	vector<vector<int> > visit;	
	int x;
	vector<string> species = {"a", "b", "c", "d", "e", "f", "g", "h", "i", "j", "k", "l", "m", "n", "o", "p", "q", "r", "s", "t", "u", "v", "w", "x", "y", "z"};	//to name the vertices of the different species, if there are not given names

	for (int i = 0; i < adj[0].size(); i++)	{	//adding the row index in the adjecence matrix of the children of the root to "visit" with the position of the root in the vectors (0)
		if (adj[0][i] == 1)	{
			visit.push_back({i,0});	
		}
	}
	while (!visit.empty())	{
		children[visit[0][1]].insert(children.size());	//adding the position of the new tree node to the children of its parent
		parent.push_back(visit[0][1]);	//adding a new tree node with the parent that is given in visit[0][1]
		children.push_back({});
		if (visit[0][0] < nrIN)	{	//checking whether the new tree node is an inner node
			vertex.push_back(-1);
			for (int i = 0; i < adj[0].size(); i++)	{	//checking the line in the adjacence matrix of the tree node
				if (adj[visit[0][0]][i] == 1)	{	//adding the row index in the adjecence matrix of its children to "visit" with its own vector index
					x = parent.size()-1;
					visit.push_back({i, x});
				}
			}
		}
		else	{
			vertex.push_back(vert.size());	//constructing a new vertex for the cBMG and adding it to "vert" and its number to the "vertex" vector of the tree
			if (!names.empty())	{
				Vertex v(vert.size(), spec[visit[0][0]-nrIN], names[visit[0][0]-nrIN]);
				vert.push_back(v);
			}
			else	{
				Vertex v(vert.size(), spec[visit[0][0]-nrIN], species[spec[visit[0][0]-nrIN]]+to_string(nr[visit[0][0]-nrIN]));
				vert.push_back(v);
			}
		}
		visit.erase(visit.begin());	//erasing the considered tree node from "visit"
	}
}
