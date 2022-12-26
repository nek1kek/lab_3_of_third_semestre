#include <vector>
#include "Graph.h"
#include "Auxillary.h"


using namespace std;
using namespace graph_ns;

template<bool isDirected>
void common_cmd(const string& cmd, graph<double, isDirected>* gr) {
	string cmd_type = _GetCommand(cmd);
	if (cmd_type == "add_edge") {
		cout << "Введите ребро в формате: \n<id_from> <id_to> <weight (if graph is weight)> <Info (\"-\" if no info)>\n";
		size_t id_from;
		size_t id_to;
		double w;
		string inf;
		cin >> id_from >> id_to >> w >> inf;
		cin.ignore();
		if (!cin.good()) throw SetException(IncorrectInputFormat);
		gr->add_edge(id_from, id_to, w, inf == "-" ? "" : inf);
	}
	else if (cmd_type == "add_node") {
		cout << "Введите вершину в формате: \n<id> <Info (\"-\" if no info)>\n";
		size_t id;
		string inf;
		cin >> id >> inf;
		cin.ignore();
		if (!cin.good()) throw SetException(IncorrectInputFormat);
		gr->add_node(id, inf == "-" ? "" : inf);
	}
	else if (cmd_type == "remove_edge") {
		cout << "Введите ребро в формате: \n<id_from> <id_to> <weight (* если нужно удалить все ребра ведущие из id_from в id_to)>\n";
		size_t id_from;
		size_t id_to;
		double w;
		string w_s;
		cin >> id_from >> id_to;
		cin >> w_s;
		cin.ignore();
		if (!cin.good()) throw SetException(IncorrectInputFormat);
		if (w_s == "*") {
			gr->remove_edge(id_from, id_to);
		}
		else {
			try {
				w = stod(w_s);
			}
			catch (...) {
				throw SetException(IncorrectInputFormat);
			}
			gr->remove_edge(id_from, id_to, w);
		}
	}
	else if (cmd_type == "remove_node") {
		cout << "Введите вершину в формате: \n<id>\n";
		size_t id;
		cin >> id;
		cin.ignore();
		if (!cin.good()) throw SetException(IncorrectInputFormat);
		gr->remove_node(id);
	}
	else if (cmd_type == "writefile") {
		vector<Argument<string>>* argList = _MakeArgumentList<string>(cmd, false);
		string filepath = "";
		for (vector<Argument<string>>::iterator it = argList->begin(); it != argList->end(); it++) {
			if ((*it).GetFlag() == 'f') {
				filepath = (*it).Getvalue();
			}
			else throw SetException(UnknownFlag);
		}
		std::ofstream ofs(filepath);
		if (ofs.is_open()) {
			gr->write_fstream(ofs);
			ofs.close();
		}
		else {
			ofs.close();
			throw SetException(CannotWriteFile);
		}

	}
	else if (cmd_type == "min_dist") {
		vector<Argument<string>>* argList = _MakeArgumentList<string>(cmd, false);
		size_t id_from;
		size_t id_to;
		for (vector<Argument<string>>::iterator it = argList->begin(); it != argList->end(); it++) {
			if ((*it).GetFlag() == 'f') {
				id_from = _TypeCast<int>::cast((*it).Getvalue());
			}
			else if ((*it).GetFlag() == 't') {
				id_to = _TypeCast<int>::cast((*it).Getvalue());
			}
			else throw SetException(UnknownFlag);
		}
		graph<double>::path p = gr->find_min_dist(id_from, id_to);
		p.print(cout);
		if (isDirected) {
			directed_graph<double>* nw_gr = dynamic_cast<directed_graph<double>*>(gr);
			nw_gr->visualize(p, 1);
		}
		else {
			undirected_graph<double>* nw_gr = dynamic_cast<undirected_graph<double>*>(gr);
			nw_gr->visualize(p, 1);
		}
	}
	else throw SetException(UnknownCommand);
}

void help() {
	cout << "\n";
	cout << "\tСписок команд: \n\n\t[] - обязательные аргументы, {} - опциональные аргументы\n\n\t=======================================\n\n\tcreate {-d (орграф)} {-f <путь> (чтение из файла)} {-s (невзвешенный граф)} - создать граф\n\tprint - вывести граф\n\tmid_dist [-f <id from>] [-t <id to>] - кратчайший путь\n\tadd - объединить графы";
	cout << "\n\tvisualize - визуализировать граф";
	cout << "\n\tadd_node, add_edge - добавить элемент в граф";
	cout << "\n\tremove_node, remove_edge - удалить элемент из графа";
	cout << "\n\transponate - транспонировать (для орграфа)";
	cout << "\n\tconnect - компоненты (сильной для орграфа) связности";
	cout << "\n\t======================================= \n\n";
}

string undirected_graph_menu(undirected_graph<double>* gr) {
	string cmd;
	cout << "\n[System] Enter command:\n>> ";
	std::getline(cin, cmd);
	string cmd_type = _GetCommand(cmd);
	try {
		while (cmd_type != "exit" && cmd_type != "create") {
			if (cmd_type == "connect") {
				vector<undirected_graph<double>> scc = gr->connect_components();
				for (auto i : scc) {
					i.print(cout);
					cout << "\n===================\n";
				}
				gr->visualize(scc);
			}
			else if (cmd_type == "help") {
				help();
			}
			else if (cmd_type == "visualize") {
				gr->visualize();
			}
			else if (cmd_type == "add") {
				vector<Argument<string>>* argList = _MakeArgumentList<string>(cmd, false);
				bool isWeight = true;
				string filepath;
				for (vector<Argument<string>>::iterator it = argList->begin(); it != argList->end(); it++) {
					if ((*it).GetFlag() == 's') {
						isWeight = false;
					}
					else if ((*it).GetFlag() == 'f') {
						filepath = (*it).Getvalue();
					}
					else throw SetException(UnknownFlag);
				}
				undirected_graph<double> other;
				if (filepath == "") {
					cout << "\nВведите граф в формате: \n\n<Кол-во вершин> <Кол-во рёбер>\n<Описание вершины 1>\n...\n<Описание вершины n>\n<Описание ребра 1>\n...\n<Описание ребра m>\n\n";
					other.read_stream(cin, isWeight);
				}
				else {
					ifstream ifs(filepath);
					other.read_fstream(ifs, isWeight);
				}
				gr->add_graph(other);
			}
			else if (cmd_type == "print") {
				gr->print(std::cout);
			}
			else if (cmd_type == "print_edges") {
				gr->print_edges(std::cout);
			}
			else {
				common_cmd<false>(cmd, gr);
			}
			cout << "\n[System] Enter command:\n>> ";
			getline(std::cin, cmd);
			cmd_type = _GetCommand(cmd);
		}
	}
	catch (SetException e) {
		cout << e.message();
		return undirected_graph_menu(gr);
	}
	return cmd;
}

string directed_graph_menu(directed_graph<double>* gr) {
	string cmd;
	cout << "\n[System] Enter command:\n>> ";
	std::getline(cin, cmd);
	string cmd_type = _GetCommand(cmd);
	try {
		while (cmd_type != "exit" && cmd_type != "create") {
			if (cmd_type == "connect") {
				vector<directed_graph<double>> scc = gr->connect_components();
				gr->visualize(scc);
				for (auto i : scc) {
					i.print(cout);
					cout << "\n===================\n";
				}
			}
			else if (cmd_type == "transponate") {
				directed_graph<double>* _old = gr;
				gr = new directed_graph(gr->get_transponated());
				delete _old;
				cout << "\n[System] Succsessfully transponated\n";

			}
			else if (cmd_type == "help") {
				help();
			}
			else if (cmd_type == "add") {
				vector<Argument<string>>* argList = _MakeArgumentList<string>(cmd, false);
				bool isWeight = true;
				string filepath;
				for (vector<Argument<string>>::iterator it = argList->begin(); it != argList->end(); it++) {
					if ((*it).GetFlag() == 's') {
						isWeight = false;
					}
					else if ((*it).GetFlag() == 'f') {
						filepath = (*it).Getvalue();
					}
					else throw SetException(UnknownFlag);
				}
				directed_graph<double> other;
				if (filepath == "") {
					cout << "\nВведите граф в формате: \n\n<Кол-во вершин> <Кол-во рёбер>\n<Описание вершины 1>\n...\n<Описание вершины n>\n<Описание ребра 1>\n...\n<Описание ребра m>\n\n";
					other.read_stream(cin, isWeight);
				}
				else {
					ifstream ifs(filepath);
					other.read_fstream(ifs, isWeight);
				}
				gr->add_graph(other);
			}
			else if (cmd_type == "visualize") {
				gr->visualize();
			}
			else if (cmd_type == "print") {
				gr->print(std::cout);
			}
			else if (cmd_type == "print_edges") {
				gr->print_edges(std::cout);
			}

			else {
				common_cmd<true>(cmd, gr);
			}
			cout << "\n[System] Enter command:\n>> ";
			getline(std::cin, cmd);
			cmd_type = _GetCommand(cmd);
		}
	}
	catch (SetException e) {
		cout << e.message();
		return directed_graph_menu(gr);
	}
	return cmd;
}


void main_listener() {
	string cmd;
	cout << "\n[System] Enter command:\n>> ";
	getline(cin, cmd);
	string cmd_type = _GetCommand(cmd);
	try {
		while (cmd_type != "exit") {
			if (cmd_type == "create") {
				vector<Argument<string>>* argList = _MakeArgumentList<string>(cmd, false);
				bool isDirected = false;
				bool isWeight = true;
				string filepath = "";
				for (vector<Argument<string>>::iterator it = argList->begin(); it != argList->end(); it++) {
					if ((*it).GetFlag() == 'd') {
						isDirected = true;
					}
					else if ((*it).GetFlag() == 's') {
						isWeight = false;
					}
					else if ((*it).GetFlag() == 'f') {
						filepath = (*it).Getvalue();
					}
					else throw SetException(UnknownFlag);
				}
				if (isDirected) {
					directed_graph<double>* gr = new directed_graph<double>;
					if (filepath != "") {
						ifstream ifs(filepath);
						gr->read_fstream(ifs, isWeight);
					}
					else {
						cout << "Введите граф в формате: \n\n<Кол-во вершин> *пробел* <Кол-во рёбер>\n<Описание вершины 1(название-id в системе и описание ее или -)>\n...\n<Описание вершины n>\n<Описание ребра 1(откуда, куда, вес, описание или -)>\n...\n<Описание ребра m>\n\n";
						gr->read_stream(cin, isWeight);
					}
					cmd = directed_graph_menu(gr);
				}
				else {
					undirected_graph<double>* gr = new undirected_graph<double>;
					if (filepath != "") {
						ifstream ifs(filepath);
						gr->read_fstream(ifs, isWeight);
					}
					else {
						cout << "Введите граф в формате: \n\n<Кол-во вершин> *пробел* <Кол-во рёбер>\n<Описание вершины 1(название-id в системе и описание ее или -)>\n...\n<Описание вершины n>\n<Описание ребра 1(откуда, куда, вес, описание или -)>\n...\n<Описание ребра m>\n\n";
						gr->read_stream(cin, isWeight);
					}
					cmd = undirected_graph_menu(gr);
				}
				cmd_type = _GetCommand(cmd);
			}
			else if (cmd_type == "help") {
				help();
				main_listener();
				return;
			}
			else throw SetException(UnknownCommand);
		}
	}
	catch (SetException e) {
		cout << e.message();
		main_listener();
		return;
	}
	return;
}