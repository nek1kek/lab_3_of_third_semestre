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