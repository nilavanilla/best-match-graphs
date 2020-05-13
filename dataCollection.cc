#include "functions.h"

using namespace std;

//generating a given number "nr" of random trees with "nrL" leafs and "nrS" species. Constructing the cBMG of the trees and manipulating them by removing and adding maximal "nrM" edges. Testing the manipulated colored digraphs whether they still hold the properties of a cBMG (the characteristics which are tested in algorithm 2 (Aho-Tree)).Repeat the manipulation and testing "nrR" times per randomly generated graph. Saving the results in an output file which you can find in the direction "output"
long collect_data(int nr, int nrL, int nrS, int nrM, int nrR)	{
	int a, b, c, d, e;
	vector<int> nodes;
	bool cor;
	vector<Vertex> vert;
	set<int> spec;
	vector<set<int> > leafs;
	vector<int> v, subgraphs, data;
	vector<vector<int> > w, z;
	set<vector<int> > y;
	Tree t;
	

	int seed = time(NULL);
	srand(seed);
	ofstream out;
	out.open("./output/output_"+to_string(seed)+".txt");
	if (!out)  {
		cerr << "cannot open output \"./output/output_" << seed << ".txt\" because there is no directory ./output\n";
		return 0;
	}
	//here you have to choose the right output message if you changed the code to consider special parameters like the number of connected components
	out << "#seed: " << seed << " number of species: " << nrS << " number vertices: " << nrL << " maximal number of manipulated edges: " << nrM << " number of testing runs of every graph: "<< nrR <<"\n";
	//out << "#seed: " << seed << " number of species: " << nrS << " number vertices: " << nrL << " fixed number of added edges: " << nrM << " number of testing runs of every graph: "<< nrR <<"\n";
	//out << "#seed: " << seed << " number of species: " << nrS << " number vertices: " << nrL << " maximal number of deleted edges: " << nrM << " number of testing runs of every graph: "<< nrR <<" and only 3 connected component\n";

	int x = 0;
	while (x < nr)	{
		while (spec.size() != nrS)	{	//constructing randomly the vertices of the cBMG. Repeating it untill it reaches the given number of species
			spec.clear();
			vert.clear();
			for (int i = 1; i <= nrL; i++)	{
				a = rand()%nrS;
				spec.insert(a);
				Vertex v(vert.size(), a, "");
				vert.push_back(v);
			}
		}
		a = nrL/3;	//maximal number of children of the root
		b = 2 + rand()%(a - 1);	//random number between 2 and "a"
		//b = 4;  //to generate cBMGs with 2 or more connected components
		for (int i = 1; i <= b; i++)	{	//constructing "b" children of the root
			t.parent.push_back(0);
			t.children.push_back({});
			t.vertex.push_back(-1);
			t.children[0].insert(i);
			nodes.push_back(i);	//adding the vector position of the new tree nodes to "nodes"
		}

		c = 0;	//current number of leafs in the tree
		while (!nodes.empty())	{
			if (nodes.size() == nrL-c)	{
				for (int i = 0; i < nodes.size(); i++)	{
					t.vertex[nodes[i]] = c;
					c++;
				}
				nodes.clear();
			}
			else	{
				b = 1 + rand()%10;
				d = rand()%nodes.size();	//deciding which node will be worked with
				if (b < 6 && nodes.size() > 1)	{	//the current node becomes a leaf with a chance of 50% and if it is not the last one in "nodes"
					t.vertex[nodes[d]] = c;	//assigning the vertex number "c" to that leaf
					c++;
				}
				else	{
					if (a > nrL-c-nodes.size())	{	//updating the maximal number of children
						a = nrL-c-nodes.size();
					}
					b = 2 + rand()%(a - 1);
					if (b == nrL-c-nodes.size())	{
						b++;
					}
					for (int i = 0; i < b; i++)	{	//creating "b" children of the given node and adding them to "nodes"
						nodes.push_back(t.parent.size());
						t.children[nodes[d]].insert(t.parent.size());
						t.parent.push_back(nodes[d]);
						t.children.push_back({});
						t.vertex.push_back(-1);
					}
				}
				nodes.erase(nodes.begin() + d);	//erasing the node
			}
		}

		CDigraph bmg(vert, spec);	//constructing the cBMG of the generated tree
		spec.clear();
		subgraphs.push_back(0);
		leafs.push_back({});
		for (auto elem : t.children[0])	{
			subgraphs.push_back(elem);
			leafs.push_back({});
		}
		for (int i = 0; i < t.children[0].size(); i++)	{
			t.find_leafSets(i+1, subgraphs, leafs, true);
			leafs[0].insert(leafs[i+1].begin(), leafs[i+1].end());
		}
		bmg.build_GofT(subgraphs, leafs, t);
		for (int i = 0; i < bmg.edges.size(); i++)	{	//saving all edges of "bmg" in "z"
			for (int j = 0; j < bmg.edges.size(); j++)	{
				if (bmg.edges[i][j] == 1)	{
					z.push_back({i, j});
				}
			}
		}

		data.push_back(z.size());	//data[0] = number of edeges
		cor = bmg.find_conComp(v, w, {}, true);
		data.push_back(bmg.connectedComponents.size()); //data[1] = number of components
		//if (bmg.connectedComponents.size() == 4){	//to test only cBMGs with a special number of components
		bmg.connectedComponents.clear();
		for (int i = 0; i < 8; i++)	{
			data.push_back(0);
		}
	
		if (z.size() < nrM)	{	//if the graph does not contain as many edges as the user wants to remove, the maximal number of removable edges becomes the number of edges in the graph
			c = z.size();
		}
		else	{
			c = nrM;
		}
		for (int i = 0; i < nrR; i++)	{	//test runs
			a = 1 + rand() % c;
			//a = c;	//if the number of manipulations is fixed
			e = 0;
			while (e != a)	{	//manipulatingthe edges of the generated cBMG
				b = rand() % 2;
				//b = 1;	//if only edges shall be deleted (1) or addded (0)
				if (b == 1)	{	//deleting an edge
					d = rand() % z.size();
					bmg.edges[z[d][0]][z[d][1]] = 0;
					y.insert({z[d][0],z[d][1]});
					e++;
				}
				else	{
					d = rand() % bmg.edges.size();
					b = rand() % bmg.edges.size();
					if (y.find({d,b}) == y.end() && bmg.edges[d][b] == 0)	{	//adding an edge
						bmg.edges[d][b] = 1;
						y.insert({d,b});
						e++;
					}
				}
			}
			for (auto elem : y)	{	//testing whether the added edges do not connect two vertices of the same color
				if (bmg.edges[elem[0]][elem[1]] == 1 && cor)	{
					if (bmg.vertices[elem[0]].species == bmg.vertices[elem[1]].species)	{
						data[2]++;
						cor = false;
					}
				}
			}
			if (!bmg.find_conComp(v, w, {}, true))	{	//finding the connected components of the BMG
				data[3]++;
				cor = false;
			}
		
			if (cor)	{
				if(!build_lrTree(bmg, 2, data))	{	//building the least reaolved tree based on the n colored BMG with the algorithm with number "alg"
					cor = false;
				}
			}
			if (cor)	{
				data[9]++;
			}
			for (auto elem : y)	{
				if (bmg.edges[elem[0]][elem[1]] == 1)	{
					bmg.edges[elem[0]][elem[1]] = 0;
				}
				else	{
					bmg.edges[elem[0]][elem[1]] = 1;
				}
			}
			y.clear();
			cor = true;
			bmg.infTriples.clear();
			bmg.connectedComponents.clear();
			bmg.lrt.parent.clear();
			bmg.lrt.parent.push_back(-1);
			bmg.lrt.children.clear();
			bmg.lrt.children.push_back({});
			bmg.lrt.vertex.clear();
			bmg.lrt.vertex.push_back(-1);
		}
		for (int i = 0; i < data.size(); i++)	{
			out << data[i] << "	";
		}
		out << "\n";
		print_progress((float)(x+1)/(float)nr*100);
		x++;
		//}	//in case of special number of components
		bmg.connectedComponents.clear();
		t.parent.clear();
		t.parent.push_back(-1);
		t.children.clear();
		t.children.push_back({});
		t.vertex.clear();
		t.vertex.push_back(-1);
		subgraphs.clear();
		leafs.clear();
		z.clear();
		data.clear();
		vert.clear();
	}
	cout<<"\n";
	out.close();
	return seed;
}



//printing the progress of the graph generating and manipulation to the shell
void print_progress(const float& percentage)
{
        printf("\rProgress: %6.2f%%", percentage);
        fflush(stdout);
}
