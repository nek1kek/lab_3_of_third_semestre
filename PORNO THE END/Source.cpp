//Кайф, изучил стандартные библиотеки, скорее вспомнил
#include <vector>
#include <list>
#include <string>
#include <iostream>

//это гениально все в алогритмы запихнуть
#include <algorithm>

//если у нас map основано на rb-tree, потому 2 лабу на ней и делал, тогда unordered сделано на hash таблице
//map - отсортированный список ключей и потому итераторы там кайфовые
//unordered_map - случайно расположены(так как hash, то скорость O(1) вместо О(log N))
#include <unordered_map>
#include <map>

/*
Какая прикольная штука, она нужна для выявляения есть ли объект или нет
Раньше люди создавали std::pair<T,bool>), если бул был фалс, тогда типо нет объект, иначе есть
Однако была проблема, что в случае отсутствия создавался T(), и на него динамически могла вызываться дин.память
И потому в с++17 добавили такую штуку, чтобы если не имеет значения, то не создавался нулевой конструктор
*/
#include <optional>

#include "Graph.h"
#include <Windows.h>
#include "menu.h"
#include "tests.h"

using namespace std;
using namespace graph_ns;


int main() {
	setlocale(LC_ALL, "Russian");
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);
	base_test_dir();
	base_test_undir();
	algo_test_dijkstra_dir();
	algo_test_dijkstra_undir();
	algo_test_connectivity();
	algo_test_strong_connectivity();
	main_listener();

	return 0;
}