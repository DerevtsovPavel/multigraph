#pragma once
#include <iostream>
#include "graph.h"
#include "Tests.h"


using namespace std;

int main() {
	TEST();
	setlocale(0, "Russian");
	graph<char> first;
	first.add(*"a", *"b", 13);
	first.add(*"a", *"b", -2);
	first.add(*"b", *"a", 11);
	first.add(*"a", *"a", 1);
	first.add(*"b", *"c", 9);
	first.add(*"d", *"e", 1);

	cout << first.size() << endl;
	first.print();

	cout << first.find(*"a")->inf << endl;

	first.del(*"d");
	first.print();


	node<char>* a = first.min_way(*"a");
	for (int i = 0; i < a->edges.size(); i++)
		cout << a->edges[i]->inf << " " << a->weight[i] << endl;
	cout << "-------To Vector-------\n";
	vector<char> vec = first.to_vector();
	for (auto i : vec) {
		cout << i << endl;
	}
	first.save("first_graph.txt");
	cout << "-------------------------------\n";

	graph<char> second;
	second.add(*"a", *"b", 1);
	second.add(*"b", *"a", -2);
	second.add(*"b", *"c", 2);
	second.add(*"b", *"c", 1);
	second.add(*"b", *"c", 3);
	second.add(*"c", *"b", -2);
	second.add(*"a", *"d", 1);
	second.add(*"a", *"d", 2);
	second.add(*"e", *"f", 1);
	second.add(*"c", *"g", 5);
	second.add(*"a", *"g", 9);
	second.add(*"g", *"a", -6);

	second.print();
	node<char>* b = second.min_way(*"a");
	for (int i = 0; i < b->edges.size(); i++)
		cout << b->edges[i]->inf << " " << b->weight[i] << endl;

	cout << "-----------------------\n";
	b = second.min_way(*"e");
	for (int i = 0; i < b->edges.size(); i++)
		cout << b->edges[i]->inf << " " << b->weight[i] << endl;
	cout << "-------To Vector-------\n";
	vec = second.to_vector();
	for (auto i : vec) {
		cout << i << endl;
	}
	cout << "-----------------------\n";

	graph<char> third;
	third.add(*"a", *"b", 2);
	third.add(*"b", *"a", -3);
	third.add(*"b", *"c", 1);
	third.add(*"c", *"d", 3);
	third.add(*"d", *"c", 4);
	third.add(*"a", *"d", 6);
	third.add(*"d", *"a", 5);
	node<char>* c = third.min_way(*"a");
	for (int i = 0; i < c->edges.size(); i++)
		cout << c->edges[i]->inf << " " << c->weight[i] << endl;


	graph<char> fourth;
	fourth.load("first_graph.txt");
	fourth.print();
}