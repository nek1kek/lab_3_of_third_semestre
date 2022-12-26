//����, ������ ����������� ����������, ������ ��������
#include <vector>
#include <list>
#include <string>
#include <iostream>

//��� ��������� ��� � ��������� ���������
#include <algorithm>

//���� � ��� map �������� �� rb-tree, ������ 2 ���� �� ��� � �����, ����� unordered ������� �� hash �������
//map - ��������������� ������ ������ � ������ ��������� ��� ��������
//unordered_map - �������� �����������(��� ��� hash, �� �������� O(1) ������ �(log N))
#include <unordered_map>
#include <map>

/*
����� ���������� �����, ��� ����� ��� ���������� ���� �� ������ ��� ���
������ ���� ��������� std::pair<T,bool>), ���� ��� ��� ����, ����� ���� ��� ������, ����� ����
������ ���� ��������, ��� � ������ ���������� ���������� T(), � �� ���� ����������� ����� ���������� ���.������
� ������ � �++17 �������� ����� �����, ����� ���� �� ����� ��������, �� �� ���������� ������� �����������
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