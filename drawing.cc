//Nicola Doering
//28/01/2020
//BA Best Match Graphs

#include "functions.h"

using namespace std;


//creating the .gv files to visualize the cBMG and LRT, compiling the graphviz files and opening the images with Eye of GNOME image viewer
void draw_graph(CDigraph graph)	{
	vector<vector<double> > colors;	//vector that includes as many colors as number of species in the BMG
	vector<double> color;
	for (int i = 0; i < graph.species.size(); i++)	{	//colors are created radomly
		color.push_back((rand() % 10 + 1)*0.1);
		color.push_back((rand() % 10 + 1)*0.1);
		color.push_back((rand() % 10 + 1)*0.1);
		colors.push_back(color);
		color.clear();
	}	

	ofstream bmg;	//creating the GraphViz data "bmg.gv" that explains the BMG
	bmg.open("bmg.gv");
	bmg<<"digraph BMG	{\n	subgraph clusterHeader {\n		margin = 0;\n		style = \"invis\";\n		HEADER [shape=\"box\" label=\"Given Best Match Graph\"];\n	}\n\n	subgraph clusterMain {\n		margin = 0;\n		style = \"invis\";\n";
	for (int i = 0; i < graph.vertices.size(); i++)	{
		bmg << "		n" << i << " [style = filled, width = 0.5, height = 0.5, color = \"" << colors[graph.vertices[i].species][0] << ", " << colors[graph.vertices[i].species][1] << ",	" << colors[graph.vertices[i].species][2] << "\", label = \"" << graph.vertices[i].mark << "\"];\n";
	}
	for (int i = 0; i < graph.edges.size(); i++)	{
		for (int j = 0; j < graph.edges.size(); j++)	{
			if (graph.edges[i][j] == 1)	{
				bmg << "		n" << i << " -> n" << j << ";\n";
			}
		}
	}
	bmg<<"	}\n}";
	bmg.close();
			
	int pos;
	ofstream tree;	//creating the GraphViz data "lrt.gv" that explains the least resolved tree of the BMG
	tree.open("lrt.gv");
	tree << "digraph LRT	{\n	subgraph clusterHeader {\n		margin = 0;\n		style = \"invis\";\n		HEADER [shape=\"box\" label=\"Computed Least Resolved Tree of the BMG\"];\n	}\n\n	subgraph clusterMain {\n		margin = 0;\n		style = \"invis\";\n";
	tree << "		n" << 0 << " [style = filled, shape = point, color = \"darkgreen\", root = ctr];\n";
	for (int i = 1; i < graph.lrt.vertex.size(); i++)	{
		if (graph.lrt.vertex[i] == -1)	{
			tree << "		n" << i << " [style = filled, shape = point, color = \"darkgreen\"];\n";
		}
		else	{
			pos = graph.pos_of_Vert(graph.lrt.vertex[i]);
			tree << "		n" << i << " [style = filled, width = 0.5, height = 0.5, color = \"" << colors[graph.vertices[pos].species][0] << ", " << colors[graph.vertices[pos].species][1] << ", " << colors[graph.vertices[pos].species][2] << "\", label = \"" << graph.vertices[pos].mark<<"\"];\n";
		}
	}
	tree << "		{rank = same";
	for (int i = 0; i < graph.lrt.vertex.size(); i++)	{
		if (graph.lrt.vertex[i] != -1)	{
			tree << " n" << i;
		}
	}
	tree << "};\n";
	for (int i = 0; i < graph.lrt.children.size(); i++)	{
		for (auto j : graph.lrt.children[i])	{
			tree << "		n" << i << " -> n" << j << ";\n";
		}
	}
	tree << "	}\n}";
	tree.close();
	
	system("circo -Tpng bmg.gv -o bmg.png");	//compiling the GraphViz data	
	system("dot -Tpng lrt.gv -o lrt.png");
	system("eog bmg.png &");	//opening the pictures of the graphs with "feh"
	system("eog lrt.png &");
}
