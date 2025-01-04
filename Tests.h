#pragma once
#include "graph.h"
#include <assert.h>

using namespace std;

void TEST() {
	graph<char> t1;

	t1.add('a', *"b", 12);
	t1.add(*"a", *"c", 1);
	t1.add(*"b", *"c", 11);

	assert(t1.find(*"a"));
	assert(!t1.find(*"z"));

	node<char>* ta = t1.get_neighbors(*"a");
	assert(ta->edges[0]->inf == *"b");
	assert(ta->weight[0] == 12);
	assert(ta->edges[1]->inf == *"c");
	assert(ta->weight[1] == 1);

	ta = t1.get_neighbors(*"c");
	assert(ta->edges.size() == 0);
	assert(ta->weight.size() == 0);

	assert(t1.size() == 3);

	t1.del(*"c");

	assert(t1.size() == 2);

	ta = t1.get_neighbors(*"a");
	assert(ta->edges.size() == 1);
	assert(ta->edges[0]->inf == *"b");


	t1.add(*"a", *"c", 0);
	t1.add(*"a", *"c", 9);
	t1.add(*"b", *"c", -20);
	t1.add(*"d", *"e", 1);
	t1.add(*"c", *"a", 1);

	ta = t1.min_way(*"a");
	for (size_t i = 0; i < ta->edges.size(); i++)
		cout << ta->edges[i]->inf << " " << ta->weight[i] << endl;

	cout << "------------------------\n";

	assert(ta->weight[0] == -7);
	assert(ta->weight[1] == 5);
	assert(ta->weight[2] == -8);
	assert(ta->weight[3] == INFINITY);
	assert(ta->weight[4] == INFINITY);

	ta = t1.min_way(*"c");
	for (size_t i = 0; i < ta->edges.size(); i++)
		cout << ta->edges[i]->inf << " " << ta->weight[i] << endl;

	assert(ta->weight[0] == -7);
	assert(ta->weight[1] == -6);
	assert(ta->weight[2] == 13);
	assert(ta->weight[3] == INFINITY);
	assert(ta->weight[4] == INFINITY);

	ta = t1.min_way(*"e");

	for (size_t i = 1; i < ta->weight.size(); i++)
		assert(ta->weight[i] == INFINITY);


	t1.save("Test_graph.txt");
	vector<char> t1_v = t1.to_vector();

	graph<char> t2;
	t2.load("Test_graph.txt");
	vector<char> t2_v = t2.to_vector();

	assert(t1_v == t2_v);

	cout << "______________-End Tests------------------------\n";
}
