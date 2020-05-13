//Nicola Doering
//28/01/2020
//BA Best Match Graphs
//main function

#include "functions.h"

using namespace std;


int main(int argc, char* argv[])	{
	int n = 0;
	int opt = 0; 
	int	nr = 0;
	string data, path, g;
	int alg, nrS, x;
	bool t0, o;
	while((opt = getopt(argc, argv, "hdalc:")) != -1)  {	//input from the commend line
		o = true;
		switch(opt)   { 
			case 'h':
				cout << "> usage if the LRT of a colored digraph should be found:\n" << argv[0] << " -d [path of graph data file] dg -a [1 to use Hasse diagram or 2 to use imformative triples]\n";
				cout << "> usage if the cBMG of a rooted tree should be found:\n" << argv[0] << " -d [path of graph data file] t\n";
				cout << "> if the graph data file also contains the labels of the vertices you can add option \"-l\"\n";
				cout << "> usage if a dataset of fails of modified random cBMGs shall be created:\n" << argv[0] << " -c [number of ramdomly created cBMGs] [number of vertices] [number of species] [maximal number of manipulated edges] [number of testing runs per cBMG] (to learn more about randomly cBMG creation read \"README.txt\")\n";
				return 0;
			case 'd': 
				data = argv[2];
				g = argv[3];
				if (g == "t")	{
					t0 = true;
				}
				else if (g != "dg")	{
					cerr << argv[3] << " is not an available option of -d!\nChoose between \"t\" or \"dg\"!\n";
					return 0;
				}
				break;
			case 'a':
				alg = atoi(argv[5]);
				break;
			case 'l':
				n = 1;
				break;
			case 'c':
				nr = atoi(argv[2]);
				nrS = atoi(argv[3]);
				alg = atoi(argv[4]);
				x = atoi(argv[5]);
				n = atoi(argv[6]);
				break;
			default:
				cerr << "to call the help function choose option -h or read README.txt\n";
				return 0;
		}
	}
	if (!o)	{
		cerr << "to call the help function choose option -h or read README.txt\n";
		return 0;
	}

	Tree t;
	int nrV, vertPerS;
	char b;
	char c[] = {'0','1'};
	ifstream graphData;
	int vertnr = 0;
	vector<int> a;
	vector<Vertex> vert;
	set<int> spec;
	vector<set<int> > leafs;
	vector<int> v, subgraphs;
	vector<vector<int> > w, z;
	string name;
	vector<string> names;
	vector<string> species = {"a", "b", "c", "d", "e", "f", "g", "h", "i", "j", "k", "l", "m", "n", "o", "p", "q", "r", "s", "t", "u", "v", "w", "x", "y", "z"};	//every species has an unique letter to name the nodes in the graphical output
	if (nr != 0)	{	//creating a data set
		if (nrS < alg)	{
			cerr << "The number of vertices " << nrS << " is smaller than the nnumber of species " << alg << "! There have to be at least as many vertices as species!\n";
			return 0;
		}
		long seed = collect_data(nr, nrS, alg, x, n);
		name = "gedit ./output/output_"+to_string(seed)+".txt &";
		char c [name.size()+1];
		strcpy(c, name.c_str());
		system(c);
	}
	else	{
		if (!t0)	{	//checking whether the given digraph is a cBMG and constructing its LRT
			if (!(alg == 1 || alg == 2))	{
				cerr << alg << " is not a defined option of -a!\nChoose between 1 and 2, please!\n";
				return 0;
			}
			graphData.open(data);	
			if (!graphData)  {
				cerr << data << " cannot be open!\n";
				return 0;
			}

			graphData >> nrV;	//number of vertices in the BMG
			if (nrV == 0)	{
				cerr << "The input of the number of vertices is improper!\n";
				return 0;
			}
			graphData >> nrS;	//number of species in the BMG
			if (nrS == 0)	{
				cerr << "The input of the number of species is improper!\n";
				return 0;
			}
			if (n == 1)	{	//optionally reading in the veertex labels
				for (int i = 0; i < nrV; i++)	{
					graphData >> name;
					names.push_back(name);
				}
			}
			
			for (int i = 0; i < nrS; i++)	{	//creating the vertices
				graphData >> vertPerS;	//number of vertices of the i'th species
				if (vertPerS == 0)	{
					cerr << "The input of the number of the " << i+1 << ". species is improper!\n";
					return 0;
				}
				spec.insert(i);
				for (int j = 0; j < vertPerS; j++)	{
					if (n == 1)	{
						Vertex ve(vertnr, i, names[vertnr]);
						vert.push_back(ve);	
					}
					else	{
						Vertex ve(vertnr, i, species[i]+to_string(j+1));		//creating the vertex with number "vertnr" of the i'th species with the mark that results from the species and the number of vertices of that species
						vert.push_back(ve);	
					}
					vertnr++;
				}
			}
			CDigraph ncBMG(vert, spec);	//creating the BMG with the read in vertex set "vert" and the species set "spec"

			for (int i = 0; i < nrV; i++)	{	//reading in the adjacence matrix of the digraph
				for (int j = 0; j < nrV; j++)	{
					graphData >> b;
					if (b != c[0] && b != c[1])	{
						cerr << "The entry [" << i << "][" << j << "] in the adjacence matrix is improper!\n";
						return 0;
					}
					if (b == c[1] && (ncBMG.vertices[i].species == ncBMG.vertices[j].species))	{
						cout << "there is an edge between the nodes " << ncBMG.vertices[i].mark << " & " << ncBMG.vertices[j].mark << " of the same species!\n-> not a BMG\n";
						graphData.close();
						return 0;
					}
					ncBMG.edges[i][j] = b - '0';
				}
			}	
			graphData.close();

			if (!ncBMG.find_conComp(v, w, {}, false))	{	//finding the connected components of the BMG
				cout << "-> not a BMG\n";
				return 0;
			}

			if(!build_lrTree(ncBMG, alg, v))	{	//building the LRT based on the n speciesed BMG with the algorithm with number "alg"
				cout << "-> not a BMG\n";
				return 0;
			}			
			draw_graph(ncBMG);	//constructing the visualisation of the cBMG and its LRT
			cout << "The given colored digraph described by the input file " << data << " is a cBMG!\n";
		}

		else	{	//constructing the cBMG of a given tree
			graphData.open(data);	
			if (!graphData)  {
				cerr << data << " cannot be open!\n";
				return 0;
			}
			graphData >> nrV;	//number of vertices
			graphData >> nrS;	//number of species
			graphData >> vertnr;	//number of inner nodes
			if (n == 1)	{	//optionally reading in the veertex labels
				for (int i = 0; i < nrV-vertnr; i++)	{
					graphData >> name;
					names.push_back(name);
				}
			}
			for (int i = 0; i < nrS; i++)	{	//saving the species of the leafs
				graphData >> vertPerS;
				for (int j = 0; j < vertPerS; j++)	{
					v.push_back(i);
					subgraphs.push_back(j);
				}
				spec.insert(i);
			}
			for (int i = 0; i < vertnr; i++)	{	//reading in the adjacence matrix of the tree
				for (int j = 0; j < nrV; j++)	{
					graphData >> x;
					a.push_back(x);
				}
				w.push_back(a);
				a.clear();
			}
			t.matrix_to_vector(w, v, subgraphs, names, vert, vertnr);	//export the adjacence matrix to the vectors "parent", "children" and "vertex"

			subgraphs.clear();
			CDigraph cBMG(vert, spec);
			subgraphs.push_back(0);
			leafs.push_back({});
			for (auto elem : t.children[0])	{	//inserting the children of the root
				subgraphs.push_back(elem);
				leafs.push_back({});
			}
			for (int j = 0; j < t.children[0].size(); j++)	{ //finding the leaf sets of every subtree 
				t.find_leafSets(j+1, subgraphs, leafs, true);
				leafs[0].insert(leafs[j+1].begin(), leafs[j+1].end());
			}
			cBMG.build_GofT(subgraphs, leafs, t);	//constructing the cBMG

			v.clear();
			w.clear();
			t.parent.clear();
			t.children.clear();
			t.vertex.clear();
			t.parent.push_back(-1);
			t.children.push_back({});
			t.vertex.push_back(-1);
	
			n = cBMG.find_conComp(v, w, {}, false);
			for (auto elem : cBMG.connectedComponents)	{	//constructing the LRT of the cBMG, which is a refinement of the given tree
				cBMG.find_infTriples(elem);
				spec.clear();
				copy(elem.begin(), elem.end(), inserter(spec, spec.end()));
				t.build_aho(spec, cBMG.infTriples, 0, cBMG.vertices, false);
				cBMG.infTriples.clear();
				if (cBMG.connectedComponents.size() == 1)	{
					cBMG.lrt = t;
				}
				else	{
					cBMG.lrt.connect_trees(t);
					t.parent.clear();
					t.children.clear();
					t.vertex.clear();
					t.parent.push_back(-1);
					t.children.push_back({});
					t.vertex.push_back(-1);
				}
			}
			
			draw_graph(cBMG);	//constructing the visualisation of the cBMG and its LRT
		}
	}
	return 0;
}


				
















