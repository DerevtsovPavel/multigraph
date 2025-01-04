//Автор: Деревцов Павел ВМК-22
#pragma once
#include "Node.h"
#include <iostream>
#include <queue>
#include <set>
#include <stack>
#include <algorithm>
#include <fstream>
#include <string>
#include <sstream>

using namespace std;

///Класс ориентированного мультиграфа с петлями
template <typename t>
class graph {
private:
	vector<node<t>*> nodes; //мн-во вершин графа

	///вспомогательный метод для поиска кратчайшего пути. Возвращаемся в предыдущую вершину ind и пересчитываем расстояние до её соседей.
	void path(vector<float>& w, size_t ind, const vector<node<t>*>& n) const{
		for (size_t i = 0; i < n[ind]->edges.size(); i++) {
			size_t in = -1;
			for (size_t k = 0; k < n.size(); k++) {
				if (n[k]->inf == n[ind]->edges[i]->inf) {
					in = k; //найдём индекс в векторе вершин этой(i) соседней вершины
					break;
				}
			}
			if (w[in] != INFINITY && w[ind] != INFINITY && w[ind] + n[ind]->weight[i] < w[in]) { //если расстояние до соседней было известно и его можно уменьшить
				w[in] = n[ind]->weight[i] + w[ind];
			}
		}

	}

	///Метод приведения из строки к вашему типу, необходимо переопределить
	t string_to_type(const string& e) {
		return e[0];
	}

public:
	~graph() {
		for (auto i : nodes)
			delete i;
		nodes.clear();
	}

	///добавить связь в граф. src - начало дуги dst - конец дуги weight - вес дуги. Если вершин со значением src или dst не было в графе, то они создадутся.
	void add(t src, t dst, int weight) {
		node<t>* f_s = nullptr;
		node<t>* f_dst = nullptr;

		for (node<t>* i : nodes) {
			if (i->inf == src)
				f_s = i;
			if (i->inf == dst)
				f_dst = i;

		}

		if (f_s == nullptr) {
			f_s = new node<t>(src);
			nodes.push_back(f_s);
		}
		if (f_dst == nullptr) {
			f_dst = new node<t>(dst);
			nodes.push_back(f_dst);
		}

		f_s->edges.push_back(f_dst);
		f_s->weight.push_back(weight);
	}


	 ///кол-во вершин в графе
	size_t size() const{
		return nodes.size();
	}

	///вывод вершин графа с помощью обхода в ширину
	void print() const{
		std::queue<node<t>*> q; //очередь обхода
		std::set<node<t>*> checked; //мн-во прошедших вершин

		size_t j = 1;
		for (size_t i = 0; i < nodes.size(); i++) {
			if (q.empty() && !checked.count(nodes[i])) { //если очередь пуста и просматриваемый узел не посещали ранее
				q.push(nodes[i]);
				std::cout << "Компонент связности " << j << std::endl;
				j++;


				while (!q.empty()) {
					node<t>* n = q.front();
					q.pop();
					if (!checked.count(n)) {
						checked.insert(n);
						std::cout << n->inf << " ";

						for (node<t>* i : n->edges)
							q.push(i);
					}

				}
				std::cout << std::endl;
			}
		}

		

	}

	///поиск узла по информации, вернёт указатель на вершину
	node<t>* find(t inf) const{
		for (size_t i = 0; i < nodes.size(); i++)
			if (nodes[i]->inf == inf) { 
				return nodes[i];
			}
		return nullptr;

	}

	///удаление узла по информационному полю
	void del(t inf) {
		node<t>* d = nullptr;
		for (size_t i = 0; i < nodes.size(); i++) { //удалить вершину из мн-ва вершин в графе
			if (nodes[i]->inf == inf) {
				d = nodes[i];
				nodes.erase(nodes.begin() + i);
				break;
			}
		}

		//у оставшихся рёбер идём по связям и удаляем указатели на удалённый узел
		for (size_t i = 0; i < nodes.size(); i++)
			for (size_t j = 0; j < nodes[i]->edges.size(); j++) {
				if (nodes[i]->edges[j]->inf == inf) {
					nodes[i]->edges.erase(nodes[i]->edges.begin() + j);
					nodes[i]->weight.erase(nodes[i]->weight.begin() + j);
				}
			}

		delete d;
	}

	

	///Поиск минимального пути от узла со значением inf, до всех остальных. Если вершина не достижима, до будет значение inf(infinity). Вернётся указатель на узел, в векторе edges - вершины в графе, в векторе weight - длина пути до соответствующей вершины из edges
	node<t>* min_way(t inf) const{
		set<t> check;
		vector<float> w(nodes.size());
		vector<node<t>*> s;

		for (size_t i = 0; i < nodes.size(); i++)
			s.push_back(nodes[i]);

		node<t>* src;
		for (size_t i = 0; i < s.size(); i++)
			if (s[i]->inf == inf) {
				src = new node<t>(inf);
				src->edges = s[i]->edges;
				src->weight = s[i]->weight;
				s.erase(s.begin() + i);
				break;
			}

		s.insert(s.begin(), src);

		w[0] = 0;
		for (size_t i = 1; i < s.size(); i++)
			w[i] = INFINITY;

		for (size_t i = 0; i < s.size()-1; i++) { //идём по вершинам
			for (size_t j = 0; j < s[i]->edges.size(); j++) { //смотрим пути из вершины к соседям
				
				size_t ind = -1;
				for (size_t k = 0; k < s.size(); k++) {
					if (s[k]->inf == s[i]->edges[j]->inf) {
						ind = k; //найдём индекс в векторе вершин этой(j) соседней вершины
						break;
					}
				}

				

				if (w[ind] == INFINITY && w[i] != INFINITY) //если мы ранее не вычислили путь до соседней, а путь к текущей есть, то расстояние до j вершины, сумма предыдущего пути и веса
					w[ind] = s[i]->weight[j]+w[i];

				else if (w[ind] != INFINITY && w[i] + s[i]->weight[j] < w[ind]) { //если расстояние до соседней было известно и его можно уменьшить
					w[ind] = s[i]->weight[j] + w[i];
					if (check.count(s[i]->edges[j]->inf)) //если вершину проходили ранее, то вернёмся в неё и пересчитаем пути из неё
						path(w, ind, s);
				}



				if (w[i] == INFINITY && w[ind] == INFINITY) //если вершина была не достигнута, то до её соседей мы тоже не дойдём
					w[ind] = INFINITY;

				else if (w[ind] == INFINITY) //если мы ещё не прошли эту вершину
					w[ind] = s[i]->weight[j];

			}
			check.insert(s[i]->inf);
		}
		
		src->edges = s;
		src->weight = w;
		return src;
	}

	//получение вектора с помощью обхода в глубину
	vector<t> to_vector() const{
		vector<t> vec;
		set<node<t>*> checked; //мн-во прошедших вершин
		stack<node<t>*> st; //стек обхода

		for (size_t i = 0; i < nodes.size(); i++) {
			if (st.empty() && !checked.count(nodes[i])) { //если очередь пуста и просматриваемый узел не посещали ранее
				st.push(nodes[i]);

				while (!st.empty()) {
					node<t>* n = st.top();
					st.pop();
					if (!checked.count(n)) {
						checked.insert(n);
						vec.push_back(n->inf);

						for (node<t>* j : n->edges) //проходит nodes
							st.push(j);
					}

				}
			}
		}
		return vec;

	}

	///вернёт указатель на искомый узел, в edges - соседи, в weight - вес ребра до них
	node<t>* get_neighbors(t inf) const{
		node<t>* answ = nullptr;

		for (size_t i = 0; i < nodes.size(); i++) {
			if (nodes[i]->inf == inf) {
				answ = new node<t>(inf);
				answ->edges = nodes[i]->edges;
				answ->weight = nodes[i]->weight;
			}
		}
		return answ;
	}

	///сохранение графа в текстовый файл с именем filename. Первая строка - имя узла источника, вторая - имена всех соседних узлов, третья - соответствующие веса до соседей.
	void save(const string& filename) {
		ofstream f(filename);
		if (f.is_open()) {
			for (size_t i = 0; i < nodes.size(); i++) {
				f << nodes[i]->inf; //добавить to_string
				f << "\n";
				for (size_t j = 0; j < nodes[i]->edges.size(); j++) {
					f << nodes[i]->edges[j]->inf;
					f << " ";
				}
				f << "\n";
				for (size_t j = 0; j < nodes[i]->weight.size(); j++) {
					f << nodes[i]->weight[j];
					f << " ";
				}
				f << "\n";

			}
			f.close();
		}
	}

	///создание графа из текстового файла с именем filename. Первая строка - имя узла источника, вторая - имена всех соседних узлов, третья - соответствующие веса до соседей.
	void load(const string& filename) {
		ifstream f(filename);

		if (f.is_open()) {
			t inf;
			vector<t> neighbors;
			vector<float> weights;
			string curr;
			while (getline(f, curr)) {
				inf = string_to_type(curr); //метод необходимо переопределить!
				getline(f, curr);
				
				stringstream ss(curr);
				string neighbor;
				while (ss>>neighbor)
				{
					neighbors.push_back(string_to_type(neighbor));
				}

				getline(f, curr);
				stringstream ss1(curr);
				string weight;
				while (ss1 >> weight) {
					weights.push_back(stof(weight));
				}

				for (size_t i = 0; i < neighbors.size(); i++) {
					add(inf, neighbors[i], weights[i]);
				}
			}
			f.close();
		}
	}

};