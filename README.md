# best-match-graphs
bmg is a program that processes different tasks that come along with the consideration of colored Best Match Graphs.
____________

INSTALLATION

This description of the installation process can be used with any Linux operating system.

Navigate the bash to the directory where the program files lay.<br>
Use the command:	make

To visualize the graphs you need the program Graphviz.<br>
Install it with: 	sudo apt-get install graphviz

The survey of datas of the robustness of random cBMGs needs a directory called "output". If there is not such an directory in the directory of the program files you need to create one.<br>
Create the directory with the command:	mkdir output
___________

USAGE

./bmg<br>
If you call the program without any other command line argument it will tell you how to open the help function.


./bmg -h<br>
It is the help function that explains briefly the usage of the program in the bash.


./bmg -d [path of the input data] dg -a [1/2] [-l]<br>
To check whether a given colored digraph is a cBMG you have to hand over the path of the input data. Then you can choose which algorithm you want to use to test the properties of a cBMG. Option 1 is the algorithm that uses the reachable sets of the equivalence classes to build the Hasse diagram of their hierarchy and option 2 uses the informative triples to construct the Aho tree.<br>
With the optional parameter -l you indicate that the input data also contains the labels of the vertices.


./bmg -d [path of the input data] t [-l]<br>
To construct the cBMG of a given tree you have to hand over the path of the input data.<br>
With the optional parameter -l you indicate that the input data also contains the labels of the vertices.


./bmg -c [number of ramdomly created cBMGs] [number of vertices] [number of species] [maximal number of manipulated edges] [number of testing runs per cBMG]<br>
To generate a data set of the robustness of randomly created cBMGs. The number of cBMGs, the number of testing runs per cBMG and the maximal number of edge manipuations. The random cBMGs are created with different parameters that you also have to enter to the command line when calling the program. Here you can decide how many vertices and how many species the graph shall contain.

___________

INPUT DATA FILES

###### Colored Digraph
You need a .txt file and if you want to take a look at an example open ./examples/example1.txt or ./examples/example2.txt. Note that it is not necessary to specify the vertex labels and if you want to do it you have to arrange them in the same order as they are read in while the species specification in the next lines. Also the adjacence matrix repesents the vertices in this order. Every line and row represent a vertex and you insert 1 at the position [i][j] if there is an directed edge from the i'th vertex to the j'th vertex or 0 if not. The input data file needs the following content and it has to be in that order:


[number of vertices v]<br>
[number of species n]<br>
[optionally vertex labels sperated by a blank]<br>
[number of vertices of the first species]<br>
.<br>
.<br>
.<br>
[number of vertices of the n'th species]<br>
[adjecence matrix with v lines and rows]<br>



###### Tree
You need a .txt file and if you want to take a look at an example open ./examples/example3.txt or ./examples/example4.txt. Note that it is not necessary to specify the leaf labels and if you want to do it you have to arrange them in the same order as they are read in while the species specification in the next lines. The rows of the adjacence matrix represent all tree nodes in which the first x rows represent the inner nodes and the others represent the leafs (same order as they are read in while species specification). The adjacence matrix only needs x lines because there canot be an outer edge of a leaf. So every line represents a inner node of the tree. You insert 1 at the position [i][j] if there is an directed edge from the i'th inner node to the j'th tree node or 0 if not. Note that the first row and line of the adjacence matrix represent the root of the tree. The input data file needs the following content and it has to be in that order:


[number of vertices v]<br>
[number of species n]<br>
[number of inner nodes x]<br>
[optionally leaf labels sperated by a blank]<br>
[number of vertices of the first species]<br>
.<br>
.<br>
.<br>
[number of vertices of the n'th species]<br>
[adjecence matrix with v rows and x lines]<br>

___________

OUTPUT

Improper inserts in the input data files or improper input parameter in the command line while calling the program produce an error message at the command line. That error message desccribes the type of problem that occured.


When you call the functionality of the program that checks whether a given digraph is a cBMG and constructs its LRT you can get divers outputs. If the algorithm finds a propertiy of cBMG that is not satisfied by the given digraph you get as output of the program a message at the command line that contains the violated property and which vertex or vertex set fault it is. If the given digraph is a cBMG two new windows are opened that contain the visualization of the input graph and the constructed LRT. These image files are called lrt.png and bmg.png.


When you call the functionality of the program that constructs the cBMG of a given tree and refines it so it becomes the LRT you get two windows that contain the visualization of the input constructed cBMG and the constructed LRT. These image files are called lrt.png and bmg.png.


When the program generates a data set regarding the robustness of random cBMGs you can see the progress of that task in the command line which is stated in percentage. After completing the task a new text file will be opened that contains the data. The input parameters and the timestamp (needed to genrate the random graphs) are written down in the first line. After that every line represents the data of one randomly created cBMG. In the first two rows you can read the number of:
- edges in the cBMG
- connected components of the cBMG
The next rows contain the number of failures of cBMG properties which occured during the testing runs of edge manipulations.The characteristic of cBMGs are ordered in the rows as following:
- no edge between two vertices of the same species
- every connected component in the cBMG contains all species
- every connected component in any two colored partition of the cBMG contains both species
- the set of informative triples of every two colored, connected partition of the cBMG has to be consistent
- the cBMG that is displayed by the Aho tree of the informative triples of the two colored, connected partition of the cBMG has to be conform with that partition
- the union of informative triples of every two colored, connected partition and the triples that explain the relationship between the vertices of different connected components of a 2-cBMG has to be consistent
- the cBMG that is displayed by the constructed LRT hass to be similar to the initial cBMG
The manipulated cBMGs that still satisfy these properties are counted in the last row.
Note that the testing run of a manipultaed cBMG interupts if the graph failed a characteristic so it is more possible to fail one of the properties that are checked first.

___________

OTHER IMPORTANT FACTS

You can remove the object files with the ending .o and the visualization files from the working directory if you want to archive the directory. <br>
Use the command:	make clean

The image files bmg.png and lrt.png that are generated to visualize the cBMG and LRT will be overwritten with the new output when you call the program again.

If you want to learn more about colored Best Match Graphs read the paper "Best match graphs" which you can find here:
https://arxiv.org/abs/1803.10989
