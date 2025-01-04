#pragma once
#include<vector>

///класс узла связного, взвешенного графа
template <typename t>
class node {
public:
	t inf; //информационное поле
	std::vector<node<t>*> edges; //указатели на другие узлы, с которыми связан текущий
	std::vector<float> weight; //соответственный вес связи(может отрицательный)

	node(t a) {
		inf = a;
	}
	~node() {
		edges.clear();
		weight.clear();
	}
};