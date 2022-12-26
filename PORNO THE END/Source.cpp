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


using namespace std;
using namespace graph_ns;

struct test {
	int a;
	int b;
	int c;
	test() = default;
	test(int a, int b, int c) {
		this->a = a;
		this->b = b;
		this->c = c;
	}
};

ostream& operator<<(ostream& os, const test& obj) {
	os << obj.a << " " << obj.b << " " << obj.c;
	return os;
}

struct less_x {
	_NODISCARD constexpr bool operator()(const test& first, const test& second) const noexcept {
		if (first.c == second.c) {
			if (first.a == second.a) {
				return first.b < second.b;
			}
			return first.a < second.a;
		}
		return first.c < second.c;
	}
};

int main() {
	setlocale(LC_ALL, "Russian");
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);
	main_listener();
	return 0;
}