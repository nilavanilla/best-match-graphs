bmg: bmg.o cdigraph.o vertex.o class.o tree.o dataCollection.o drawing.o buildLRT.o
	g++ -std=c++11 bmg.o cdigraph.o vertex.o class.o tree.o dataCollection.o drawing.o buildLRT.o -g -o bmg

bmg.o: cdigraph.h functions.h bmg.cc
	g++ -std=c++11 -c bmg.cc

buildLRT.o: cdigraph.h functions.h buildLRT.cc
	g++ -std=c++11 -c buildLRT.cc

drawing.o: cdigraph.h functions.h drawing.cc buildLRT.cc
	g++ -std=c++11 -c drawing.cc

dataCollection.o: cdigraph.h functions.h dataCollection.cc
	g++ -std=c++11 -c dataCollection.cc

tree.o: tree.h tree.cc 
	g++ -std=c++11 -c tree.cc

cdigraph.o: vertex.h class.h cdigraph.h cdigraph.cc
	g++ -std=c++11 -c cdigraph.cc

class.o: vertex.h class.h class.cc
	g++ -std=c++11 -c class.cc

vertex.o: vertex.h vertex.cc
	g++ -std=c++11 -c vertex.cc

clean: bmg.o vertex.o cdigraph.o tree.o class.o dataCollection.o drawing.o buildLRT.o bmg.gv bmg.png lrt.gv lrt.png
	rm bmg.o vertex.o cdigraph.o tree.o class.o dataCollection.o drawing.o buildLRT.o bmg.gv bmg.png lrt.gv lrt.png
