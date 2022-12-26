#include "Graph.h"
#include <set>
#include <cassert>
#include "tests.h"

using namespace graph_ns;
using namespace std;

void base_test_dir() {
	directed_graph<int> d_gr;
	ifstream ifs("C:\\Users\\vipda\\Desktop\\3rd lab of third semestre\\PORNO THE END\\PORNO THE END\\preset2.txt");
	d_gr.read_fstream(ifs, true);
	d_gr.add_edge(1, 5, 1);
	d_gr.add_edge(1, 6, 1);
	d_gr.add_edge(1, 7, 1);
	d_gr.remove_edge(1, 3);
	d_gr.remove_node(4);
	set<graph<int, true>::edge, graph<int, true>::less_x> ans;
	ans.emplace(graph<int, true>::edge(2, 1, 5));
	ans.emplace(graph<int, true>::edge(3, 2, 2));
	ans.emplace(graph<int, true>::edge(2, 7, 2));
	ans.emplace(graph<int, true>::edge(1, 5, 1));
	ans.emplace(graph<int, true>::edge(1, 6, 1));
	ans.emplace(graph<int, true>::edge(1, 7, 1));
	ans.emplace(graph<int, true>::edge(3, 6, 3));
	ans.emplace(graph<int, true>::edge(6, 7, 1));
	set<graph<int, true>::edge, graph<int, true>::less_x> st;
	st = d_gr.get_edges();
	assert(ans == st);
	//Assertion failed: ans != st, file C:\Users\vipda\Desktop\3rd lab of third semestre\PORNO THE END\PORNO THE END\tests.cpp, line 29
	//выведет если ошибка
}

void base_test_undir() {
	undirected_graph<int> d_gr;
	ifstream ifs("C:\\Users\\vipda\\Desktop\\3rd lab of third semestre\\PORNO THE END\\PORNO THE END\\preset2.txt");
	d_gr.read_fstream(ifs, true);
	d_gr.add_edge(1, 5, 1);
	d_gr.add_edge(1, 6, 1);
	d_gr.add_edge(1, 7, 1);
	d_gr.remove_edge(1, 3);
	d_gr.remove_node(4);
	set<graph<int>::edge, graph<int>::less_x> ans;
	ans.emplace(graph<int>::edge(2, 1, 5));
	ans.emplace(graph<int>::edge(3, 2, 2));
	ans.emplace(graph<int>::edge(2, 7, 2));
	ans.emplace(graph<int>::edge(1, 5, 1));
	ans.emplace(graph<int>::edge(1, 6, 1));
	ans.emplace(graph<int>::edge(1, 7, 1));
	ans.emplace(graph<int>::edge(3, 6, 3));
	ans.emplace(graph<int>::edge(6, 7, 1));
	set<graph<int>::edge, graph<int>::less_x> st;
	st = d_gr.get_edges();
	assert(ans == st);
}

void algo_test_dijkstra_dir() {
	directed_graph<int> d_gr;
	ifstream ifs("C:\\Users\\vipda\\Desktop\\3rd lab of third semestre\\PORNO THE END\\PORNO THE END\\preset1.txt");
	d_gr.read_fstream(ifs, true);
	directed_graph<int>::path p1 = d_gr.find_min_dist(1, 7);
	directed_graph<int>::path p2 = d_gr.find_min_dist(2, 3);
	directed_graph<int>::path p3 = d_gr.find_min_dist(9, 6);
	directed_graph<int>::path p4 = d_gr.find_min_dist(1, 3);
	directed_graph<int>::path p1_ans;
	p1_ans.push_back(edge<string, int>(1, 8, 1));
	p1_ans.push_back(edge<string, int>(8, 7, 6));
	assert(p1_ans == p1);
	directed_graph<int>::path p2_ans;
	p2_ans.push_back(edge<string, int>(2, 9, 3));
	p2_ans.push_back(edge<string, int>(9, 3, 3));
	assert(p2 == p2_ans);
	directed_graph<int>::path p3_ans;
	p3_ans.push_back(edge<string, int>(9, 5, 10));
	p3_ans.push_back(edge<string, int>(5, 6, 6));
	assert(p3 == p3_ans);
	directed_graph<int>::path p4_ans;
	p4_ans.push_back(edge<string, int>(1, 8, 1));
	p4_ans.push_back(edge<string, int>(8, 9, 4));
	p4_ans.push_back(edge<string, int>(9, 3, 3));
	assert(p4 == p4_ans);
}
void algo_test_dijkstra_undir() {
	undirected_graph<int> d_gr;
	ifstream ifs("C:\\Users\\vipda\\Desktop\\3rd lab of third semestre\\PORNO THE END\\PORNO THE END\\preset1.txt");
	d_gr.read_fstream(ifs, true);
	undirected_graph<int>::path p1 = d_gr.find_min_dist(7, 10);
	undirected_graph<int>::path p2 = d_gr.find_min_dist(3, 10);
	undirected_graph<int>::path p3 = d_gr.find_min_dist(9, 6);
	undirected_graph<int>::path p4 = d_gr.find_min_dist(7, 1);
	undirected_graph<int>::path p1_ans;
	p1_ans.push_back(edge<string, int>(7, 8, 6));
	p1_ans.push_back(edge<string, int>(8, 10, 2));
	assert(p1_ans == p1);
	undirected_graph<int>::path p2_ans;
	p2_ans.push_back(edge<string, int>(3, 10, 1));
	assert(p2 == p2_ans);
	undirected_graph<int>::path p3_ans;
	p3_ans.push_back(edge<string, int>(9, 5, 9));
	p3_ans.push_back(edge<string, int>(5, 6, 6));
	assert(p3 == p3_ans);
	undirected_graph<int>::path p4_ans;
	p4_ans.push_back(edge<string, int>(7, 1, 7));
	assert(p4 == p4_ans);
}


void algo_test_connectivity() {
	undirected_graph<int> d_gr;
	ifstream ifs("C:\\Users\\vipda\\Desktop\\3rd lab of third semestre\\PORNO THE END\\PORNO THE END\\preset_unconnective.txt");
	d_gr.read_fstream(ifs, true);
	vector<undirected_graph<int>> res = d_gr.connect_components();
	assert(res.size() == 4);
	undirected_graph<int> first;
	first.add_node(1);
	first.add_node(2);
	first.add_node(3);
	first.add_edge(1,2,3);
	first.add_edge(2,3,4);
	assert(first == res[0]);
	undirected_graph<int> second;
	second.add_node(4);
	second.add_node(5);
	second.add_node(6);
	second.add_node(7);
	second.add_edge(4, 5, 5);
	second.add_edge(4, 6, 6);
	second.add_edge(5, 6, 2);
	second.add_edge(6, 7, 7);
	assert(second == res[1]);
	undirected_graph<int> third;
	third.add_node(8);
	third.add_node(10);
	third.add_edge(8, 10, 2);
	assert(third == res[2]);
	undirected_graph<int> fourth;
	fourth.add_node(9);
	assert(fourth == res[3]);
}

void algo_test_strong_connectivity() {
	directed_graph<int> d_gr;
	ifstream ifs("C:\\Users\\vipda\\Desktop\\3rd lab of third semestre\\PORNO THE END\\PORNO THE END\\preset_unconnective.txt");
	d_gr.read_fstream(ifs, true);

	vector<directed_graph<int>> res = d_gr.connect_components();
	assert(res.size() == 8);
	directed_graph<int> first;
	first.add_node(9);
	assert(first == res[0]);
	directed_graph<int> second;
	second.add_node(8);
	assert(second == res[1]);
	directed_graph<int> third;
	third.add_node(10);
	assert(third == res[2]);
	directed_graph<int> fourth;
	fourth.add_node(4);
	fourth.add_node(5);
	fourth.add_node(6);
	fourth.add_edge(6, 4, 6);
	fourth.add_edge(4, 5, 5);
	fourth.add_edge(5, 6, 2);
	assert(fourth == res[3]);
	directed_graph<int> fifth;
	fifth.add_node(7);
	assert(fifth == res[4]);
	directed_graph<int> sixth;
	sixth.add_node(3);
	assert(sixth == res[5]);
	directed_graph<int> seventh;
	seventh.add_node(1);
	assert(seventh == res[6]);
	directed_graph<int> eight;
	eight.add_node(2);
	assert(eight == res[7]);
}