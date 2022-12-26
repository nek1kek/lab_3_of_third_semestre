#pragma once
#include <cmath>
#include <string>
#include <list>
#include <optional>
#include <set>
#include <unordered_map>
#include "exception.h"
#include <fstream>
#include <queue>



namespace graph_ns {


	enum _color {
		Black, Red, Blue
	};




	template<typename NodeInfo>
	struct node {//вершина
	private:
		size_t id;//номер
		NodeInfo info;//любая доп информация которая может хранится в ребре
		short int color = 0;
	public:
		node() = default;
		node(size_t id) {
			this->id = id;
		}
		node(const node& other) {
			this->id = other.id;
			this->color = other.color;
			this->info = other.info;
		}
		node(node&& other) {
			this->id = other.id;
			this->color = other.color;
			this->info = other.info;
			other.id = 0;
			other.info = NodeInfo();
			other.color = Black;
		}
		node& operator=(const node& other) {
			this->id = other.id;
			this->color = other.color;
			this->info = other.info;
			return *this;
		}
		node& operator=(node&& other) {
			this->id = other.id;
			this->color = other.color;
			this->info = other.info;
			other.id = 0;
			other.info = NodeInfo();
			other.color = Black;
			return *this;
		}
		node(size_t id, NodeInfo info) : node(id) {
			this->info = info;
		}
		~node() = default;
		size_t get_id() const noexcept {
			return id;
		}
		NodeInfo get_info() const noexcept {
			return info;
		}
		short int get_color() const noexcept {
			return color;
		}
		void set_color(size_t color) noexcept {
			this->color = color;
		}
	};




	template<typename EdgeInfo, typename WType>//WType - что берем в качестве веса
	struct edge {//ребро
	private:
		size_t from = 0;
		size_t to = 0;

		WType weight = WType();//вес ребра
		EdgeInfo info = EdgeInfo(); //любая доп информация которая может хранится в ребре
		size_t color = 0;
	public:
		edge() = delete;
		edge(size_t id_from, size_t id_to, WType weight) {
			this->from = id_from;
			this->to = id_to;
			this->weight = weight;
		}
		edge(const edge& other) : info(other.info) {
			this->from = other.from;
			this->to = other.to;
			this->weight = other.weight;
			this->color = other.color;
		}
		edge(size_t id_from, size_t id_to, WType weight, EdgeInfo info, size_t color = 0) : edge(id_from, id_to, weight) {
			this->info = info;
			this->color = color;
		}
		~edge() = default;
		size_t get_from() const noexcept {
			return this->from;
		}
		size_t get_to() const noexcept {
			return this->to;
		}
		EdgeInfo get_info() const noexcept {
			return this->info;
		}
		WType get_weight() const noexcept {
			return this->weight;
		}
		size_t get_color() const noexcept {
			return color;
		}
		void set_color(size_t color) noexcept {
			this->color = color;
		}
		bool operator==(const edge& other) const noexcept {
			if (this->from == other.from && this->to == other.to && this->weight == other.weight && this->info == other.info) {
				return true;
			}
			return false;
		}
		bool operator!=(const edge& other) const noexcept {
			if (this->from == other.from && this->to == other.to && this->weight == other.to && this->info == other.info) {
				return false;
			}
			return true;
		}
	};


	template<typename EdgeInfo, typename WType>
	struct path {//путь, состоящий из ребер только
	private:
		WType len = WType();//вес ребра - длина
		size_t count = 0;//сколько элементов
		std::list<edge<EdgeInfo, WType>> chain; //список-цепочка ребер
	public:
		path() = default;
		WType get_len() const noexcept {
			return len;
		}
		size_t get_count() const noexcept {
			return count;
		}
		const std::list<edge<EdgeInfo, WType>>& get_chain() const noexcept {
			return chain;
		}
		void push_back(const edge<EdgeInfo, WType>& ed) {
			if (chain.size() == 0 || (*chain.end()).get_to() == ed.get_from()) {//если равен конец цепочки с началом нового ребра
				chain.push_back(ed);
				count++;
				len += ed.get_weight();
			}
			else throw SetException(IncorrectPath);
		}
		void push_tobegin(const edge<EdgeInfo, WType>& ed) {
			if (chain.size() == 0 || (*chain.begin()).get_from() == ed.get_to()) {
				chain.insert(chain.begin(), ed);
				count++;
				len += ed.get_weight();
			}
			else throw SetException(IncorrectPath);
		}
		std::ostream& print(std::ostream& os) const noexcept {
			for (typename std::list<edge<EdgeInfo, WType>>::const_iterator it = chain.cbegin(); it != chain.cend(); it++) {
				os << "{ " << (*it).get_from() << ", " << (*it).get_to() << ", " << (*it).get_weight() << " }\n";
			}
			os << "\nTotal: " << len << "\n";
			return os;
		}
		path& operator+=(const path& other) {
			if ((*other.chain.begin()).get_from() == (*chain.end()).get_to()) {
				for (typename std::list<edge<EdgeInfo, WType>>::const_iterator it = other.chain.cbegin(); it != other.chain.cend(); it++) {
					this->chain.push_back(*it);
				}
			} else throw SetException(IncorrectPath);
		}
	};




	template<typename WType = int, bool isDirected = false, class NodeInfo = std::string, class EdgeInfo = std::string>
	class graph {//граф отец, от него наследуются неориентированный и ориентированный, со своими функциями
	public:
		//сокращения, чтобы не прописывать все время
		using node = node<NodeInfo>;
		using edge = edge<EdgeInfo, WType>;
		using path = path<EdgeInfo, WType>;

	protected:
		size_t weight_edges = 0;//взвешанных вершин скок
		std::unordered_map<size_t, std::list<edge>> incidences_list;//список соедененных вершин графа, матрица инцидентности
		std::unordered_map<size_t, node> nodes;//все вершины пронумерованные
		struct less_x {
			constexpr bool operator()(const edge& first, const edge& second) const noexcept {
				if (first.get_weight() == second.get_weight()) {
					if (first.get_from() == second.get_from()) {
						return first.get_to() < second.get_to();
					}
					return first.get_from() < second.get_from();
				}
				return first.get_weight() < second.get_weight();
			}
		};

		std::set<edge, less_x> edges;//множество ребер

		void print_node(std::ofstream& ofs, const node& nd) const noexcept {
			ofs << nd.get_id();
			if (nd.get_info() != NodeInfo()) {
				ofs << " [label=\"" << nd.get_id() << " " << nd.get_info() << "\",";//label-информация человека о вершинке
			}
			else {
				ofs << " [";
			}
			if (nd.get_color() % 11 == 1) {
				ofs << "color=\"red\"";
			}
			else if (nd.get_color() == 2) {
				ofs << "color=\"green\"";
			}
			else if (nd.get_color() == 3) {
				ofs << "color=\"blue\"";
			}
			else if (nd.get_color() == 4) {
				ofs << "color=\"yellow\"";
			}
			else if (nd.get_color() == 5) {
				ofs << "color=\"webmaroon\"";
			}
			else if (nd.get_color() == 6) {
				ofs << "color=\"cyan\"";
			}
			else if (nd.get_color() == 7) {
				ofs << "color=\"crimson\"";
			}
			else if (nd.get_color() == 8) {
				ofs << "color=\"darkorchid1\"";
			}
			else if (nd.get_color() == 9) {
				ofs << "color=\"goldenrod2\"";
			}
			else if (nd.get_color() == 10) {
				ofs << "color=\"lightslateblue\"";
			}
			ofs << "];\n";
		}
		virtual void print_edge(std::ofstream& ofs, const edge& ed) const noexcept {
			const node& nd = (*nodes.find(ed.get_from())).second;
			ofs << nd.get_id();
			ofs << "->";
			const node& nd2 = (*nodes.find(ed.get_to())).second;
			ofs << nd2.get_id();
			ofs << " [";
			if (ed.get_color() % 11 == 1) {
				ofs << "color=\"red\",";
			}
			else if (ed.get_color() == 2) {
				ofs << "color=\"green\",";
			}
			else if (ed.get_color() == 3) {
				ofs << "color=\"blue\",";
			}
			else if (ed.get_color() == 4) {
				ofs << "color=\"yellow\",";
			}
			else if (ed.get_color() == 5) {
				ofs << "color=\"webmaroon\",";
			}
			else if (ed.get_color() == 6) {
				ofs << "color=\"cyan\",";
			}
			else if (ed.get_color() == 7) {
				ofs << "color=\"crimson\",";
			}
			else if (ed.get_color() == 8) {
				ofs << "color=\"darkorchid1\",";
			}
			else if (ed.get_color() == 9) {
				ofs << "color=\"goldenrod2\",";
			}
			else if (ed.get_color() == 10) {
				ofs << "color=\"lightslateblue\",";
			}
			ofs << "label=\"" << ed.get_info();
			if (weight_edges > 0) ofs << " {" << ed.get_weight() << "}";
			ofs << "\"];\n";
		}
		size_t n;//кол-во вершин
		size_t m;//кол-во ребер

	public:
		graph() = default;
		graph(const graph& other) : edges(other.edges), incidences_list(other.incidences_list), nodes(other.nodes) {
			this->n = other.n;
			this->m = other.m;
			this->weight_edges = other.weight_edges;
		}
		size_t n_count() const noexcept {
			return n;
		}
		size_t m_count() const noexcept {
			return m;
		}
		
		path find_min_dist(size_t id_f, size_t id_t) {
			if (isWeight()) {
				return find_min_dist_dijkstra(id_f, id_t);
			}
			else return find_min_dist_bfs(id_f, id_t);//если не взвешанный, то поиск в ширину хватит
		}
		bool isWeight() const noexcept {
			return weight_edges != 0;
		}

		//от сюда рассмотреть+меню понять

		void write_fstream(std::ofstream& ofs) const noexcept {
			if (!ofs.is_open()) throw SetException(CannotReadFile);
			ofs << n << " " << m << "\n";
			for (auto i : nodes) {
				ofs << i.second.get_id();
				if (i.second.get_info() != NodeInfo())
					ofs << " " << i.second.get_info();
				else ofs << " " << "NoneInfo";
				ofs << "\n";
			}
			for (auto i : edges) {
				ofs << i.get_from() << " " << i.get_to();
				if (isWeight()) ofs << " " << i.get_weight();
				if (i.get_info() != EdgeInfo())
					ofs << " " << i.get_info();
				else ofs << " " << "NoneInfo";
				ofs << "\n";
			}
		}
		void read_fstream(std::ifstream& ifs, bool isweight) {
			if (!ifs.is_open()) throw SetException(CannotReadFile);
			size_t n_f, m_f;
			ifs >> n_f >> m_f;
			if (!ifs.good()) throw SetException(IncorrectInputFormat);
			for (int i = 0; i < n_f; i++) {
				size_t id;
				NodeInfo inf;
				ifs >> id >> inf;
				if (!ifs.good()) throw SetException(IncorrectInputFormat);
				if (inf == "NoneInfo") inf = NodeInfo();
				add_node(id, inf);
			}
			for (int i = 0; i < m_f; i++) {
				size_t from;
				size_t to;
				WType weight = 1;
				EdgeInfo inf;
				ifs >> from >> to;
				if (isweight) ifs >> weight;
				ifs >> inf;
				if (!ifs.good()) throw SetException(IncorrectInputFormat);
				if (inf == "NoneInfo") inf = EdgeInfo();
				add_edge(from, to, weight, inf);
			}
		}
		void read_stream(std::istream& ifs, bool isweight) {
			size_t n_f, m_f;
			ifs >> n_f >> m_f;
			if (!ifs.good()) throw SetException(IncorrectInputFormat);
			for (int i = 0; i < n_f; i++) {
				size_t id;
				NodeInfo inf;
				ifs >> id >> inf;
				ifs.ignore();
				if (!ifs.good()) throw SetException(IncorrectInputFormat);
				if (inf == "-") inf = NodeInfo();
				add_node(id, inf);
			}
			for (int i = 0; i < m_f; i++) {
				size_t from;
				size_t to;
				WType weight = 1;
				EdgeInfo inf;
				ifs >> from >> to;
				if (isweight) ifs >> weight;
				ifs >> inf;
				ifs.ignore();
				if (!ifs.good()) throw SetException(IncorrectInputFormat);
				if (inf == "-") inf = EdgeInfo();
				add_edge(from, to, weight, inf);
			}
		}
		
		void add_node(size_t id, NodeInfo inf = NodeInfo()) {
			if (nodes.find(id) == nodes.end()) {
				nodes[id] = node(id, inf);
				n++;
				incidences_list.emplace(id, std::list<edge>());
			}
			else throw SetException(NodeExists);
		}
		void add_edge(size_t id_from, size_t id_to, WType weight = 1, EdgeInfo inf = EdgeInfo()) {
			if (nodes.find(id_from) == nodes.end() || nodes.find(id_to) == nodes.end()) throw SetException(NoSuchElement);
			if (isDirected || edges.find(edge(id_to, id_from, weight, inf)) == edges.end()) {
				edges.emplace(edge(id_from, id_to, weight, inf));
				if (weight != 1) weight_edges++;
			}
			if (std::find(incidences_list[id_from].begin(), incidences_list[id_from].end(), edge(id_from, id_to, weight, inf)) == incidences_list[id_from].end())
			{
				if (isDirected || (std::find(incidences_list[id_from].begin(), incidences_list[id_from].end(), edge(id_to, id_from, weight, inf)) == incidences_list[id_from].end()))
				{
					m++;
					incidences_list[id_from].push_back(edge(id_from, id_to, weight, inf));
					if (!isDirected) {
						incidences_list[id_to].push_back(edge(id_to, id_from, weight, inf));
					}
				}
			}
		}
		void remove_node(size_t id) {
			if (nodes.find(id) == nodes.end()) throw SetException(NoSuchElement);
			for (typename std::set<edge>::iterator it = edges.begin(); it != edges.end(); ) {
				if ((*it).get_to() == id) {
					typename std::set<edge>::iterator it_cur = it;
					it++;
					typename std::list<edge>::iterator itx = std::find(incidences_list[(*it_cur).get_from()].begin(), incidences_list[(*it_cur).get_from()].end(), *it_cur);
					if (itx != incidences_list[(*it_cur).get_from()].end())
						incidences_list[(*it_cur).get_from()].erase(itx);
					if (!isDirected) {
						itx = incidences_list[(*it_cur).get_to()].end();
						edge opposite_edge((*it_cur).get_to(), (*it_cur).get_from(), (*it_cur).get_weight(), (*it_cur).get_info());
						itx = std::find(incidences_list[(*it_cur).get_to()].begin(), incidences_list[(*it_cur).get_to()].end(), opposite_edge);
						if (itx != incidences_list[(*it_cur).get_to()].end())
							incidences_list[(*it_cur).get_to()].erase(itx);
					}
					edges.erase(it_cur);
				}
				else if ((*it).get_from() == id) {
					typename std::set<edge>::iterator it_cur = it;
					it++;
					typename std::list<edge>::iterator itx = std::find(incidences_list[(*it_cur).get_from()].begin(), incidences_list[(*it_cur).get_from()].end(), *it_cur);
					if (itx != incidences_list[(*it_cur).get_from()].end())
						incidences_list[id].erase(itx);
					if (!isDirected) {
						itx = incidences_list[(*it_cur).get_to()].end();
						edge opposite_edge((*it_cur).get_to(), (*it_cur).get_from(), (*it_cur).get_weight(), (*it_cur).get_info());
						itx = std::find(incidences_list[(*it_cur).get_to()].begin(), incidences_list[(*it_cur).get_to()].end(), opposite_edge);
						if (itx != incidences_list[(*it_cur).get_to()].end())
							incidences_list[(*it_cur).get_to()].erase(itx);
					}
					edges.erase(it_cur);
				}
				else it++;
			}
			incidences_list.erase(id);
			n--;
			nodes.erase(id);
		}
		void remove_edge(size_t id_from, size_t id_to, std::optional<WType> weight = std::optional<WType>()) {
			size_t prev_m = m;
			for (typename std::list<edge>::iterator it = incidences_list[id_from].begin(); it != incidences_list[id_from].end();) {
				if ((*it).get_from() == id_from && (*it).get_to() == id_to && (!weight.has_value() || (*it).get_weight() == weight.value())) {
					typename std::list<edge>::iterator it_cur = it;
					it++;
					typename std::set<edge>::iterator itx = std::find(edges.begin(), edges.end(), *it_cur);
					if (itx != edges.end()) {
						edges.erase(itx);
						if (weight != 1) weight_edges--;
						m--;
					}
					incidences_list[id_from].erase(it_cur);
				}
				else it++;
			}
			if (prev_m == m) throw SetException(NoSuchEdge);
			if (!isDirected) {
				for (typename std::list<edge>::iterator it = incidences_list[id_to].begin(); it != incidences_list[id_to].end();) {
					if ((*it).get_from() == id_to && (*it).get_to() == id_from && (!weight.has_value() || (*it).get_weight() == weight.value())) {
						typename std::list<edge>::iterator it_cur = it;
						it++;
						typename std::set<edge>::iterator itx = std::find(edges.begin(), edges.end(), *it_cur);
						if (itx != edges.end())
							edges.erase(itx);
						incidences_list[id_to].erase(it_cur);
					}
					else it++;
				}
			}
		}
		std::ostream& print(std::ostream& os) const noexcept {
			for (typename std::unordered_map<size_t, std::list<edge>>::const_iterator it = incidences_list.cbegin(); it != incidences_list.cend(); it++) {
				os << (*it).first << " - ";
				for (typename std::list<edge>::const_iterator itl = (*it).second.cbegin(); itl != (*it).second.cend(); itl++) {
					os << "< " << (*itl).get_from() << ", " << (*itl).get_to() << ", " << (*itl).get_weight() << " >";
					if (std::distance(itl, (*it).second.cend()) > 1) os << ", ";
				}
				if ((*it).second.size() == 0) {
					os << "<>";
				}
				os << "\n\n";
			}
			return os;
		}
		std::ostream& print_edges(std::ostream& os) const noexcept {
			os << "\n";
			for (typename std::set<edge>::const_iterator it = edges.cbegin(); it != edges.cend(); it++) {
				os << "< " << (*it).get_from() << " - " << (*it).get_to() << " [" << (*it).get_weight() << "] >";
				os << "\n";
			}
			return os;
		}
		~graph() = default;

private:
	struct greater_pair {//создано только для деикстры
		bool operator()(const std::pair<size_t, WType>& first, const std::pair<size_t, WType>& second) const noexcept {
			//Указывает на обязательность использования результата при возврате из функции.
			if (first.second != second.second) {
				return first.second > second.second;
			}
			return first.first > second.first;
		}
	};

	path find_min_dist_dijkstra(size_t id_f, size_t id_t) {//для крутых взвешанных
		std::unordered_map<size_t, std::optional<WType>> dist;
		std::unordered_map<size_t, const edge*> parents;
		parents.reserve(n);
		dist.reserve(n);
		dist[id_f] = WType();
		std::priority_queue<std::pair<size_t, WType>, std::vector<std::pair<size_t, WType>>, greater_pair> pq;
		pq.push(std::make_pair(id_f, 0));
		while (pq.size()) {
			int v = pq.top().first;
			pq.pop();
			if (nodes[v].get_color() != 0) continue;
			if (!dist[v].has_value()) break;
			nodes[v].set_color(1);
			for (typename std::list<edge>::iterator it = incidences_list[v].begin(); it != incidences_list[v].end(); it++) {
				WType len = (*it).get_weight();
				size_t id_to = (*it).get_to();
				if ((!dist[id_to].has_value()) || dist[id_to].value() > dist[v].value() + len) {
					dist[id_to] = dist[v].value() + len;
					parents[id_to] = &(*it);
					pq.push(std::make_pair(id_to, dist[id_to].value()));
				}
			}
		}
		for (typename std::unordered_map<size_t, node>::iterator it = nodes.begin(); it != nodes.end(); it++) {
			(*it).second.set_color(0);
		}
		if (!dist[id_t].has_value()) throw SetException(GraphNotConnective);
		path res;
		for (size_t cur_id = id_t; cur_id != id_f; cur_id = parents[cur_id]->get_from()) {
			res.push_tobegin(*parents[cur_id]);
		}
		return res;
	}
	path find_min_dist_bfs(size_t id_f, size_t id_t) {//обход в ширину, или волновой алгоритм, возвращает путь от нужного id_f до конечного id_t
		std::unordered_map<size_t, std::optional<WType>> dist;//для каждого числа, он либо будет содержать внутри себя расстояние до него или нет
		std::unordered_map<size_t, const edge*> parents;
		parents.reserve(n);//а вдруг все вершины можно будет вставить
		dist.reserve(n);//устанавливает хеш функцию на наиболее подходящее для содержания не менее n элементов.
		dist[id_f] = WType();//типо от него до него же самого длина 0
		std::queue<size_t> id_queue;//создаем очередь
		id_queue.push(id_f);//кладем туда первый
		nodes[id_f].set_color(1);//если мы прошли, то расскрасим красным
		for (; id_queue.size();) {
			int v = id_queue.front();//первый элемент в очереди вытаскиваем и сохраняем
			id_queue.pop();
			if (v == id_t) break;//а если дошли, то че дальше искать
			if (!dist[v].has_value()) break;//если расстояние до него не существует, тогда выкидывает, то есть первоначальный мы положили уже

			for (typename std::list<edge>::const_iterator it = incidences_list[v].cbegin(); it != incidences_list[v].cend(); it++) {
				size_t id_to = (*it).get_to();
				if (nodes[id_to].get_color() == 0) {
					id_queue.push(id_to);//если мы еще не посмотрели его, тогда положим в очередь
					nodes[id_to].set_color(1);//перекрасим
					if ((!dist[id_to].has_value()) || dist[id_to].value() > dist[v].value() + 1) {
						dist[id_to] = dist[v].value() + 1;
						parents[id_to] = &(*it);
					}
				}
			}
		}
		//все раскрасим в обычный черный обратно
		for (typename std::unordered_map<size_t, node>::iterator it = nodes.begin(); it != nodes.end(); it++) {
			(*it).second.set_color(0);
		}
		//если расстояния до нужного нам нет, тогда выдадим ошибку, что нельзя соеденить
		if (!dist[id_t].has_value()) throw SetException(GraphNotConnective);

		//создаем конечный путь 
		path res;
		for (size_t cur_id = id_t; cur_id != id_f; cur_id = parents[cur_id]->get_from()) {
			res.push_tobegin(*parents[cur_id]);
		}
		return res;
	}
	};

	template<typename WType = int, class NodeInfo = std::string, class EdgeInfo = std::string>
	class directed_graph : public graph<WType, true, NodeInfo, EdgeInfo>
	{
	public:
		using node = node<NodeInfo>;
		using edge = edge<EdgeInfo, WType>;
		using path = path<EdgeInfo, WType>;
	private:
		using base = graph<WType, true, NodeInfo, EdgeInfo>;

		void csc_order(size_t id, std::vector<size_t>& order) {
			base::nodes[id].set_color(1);
			for (typename std::list<edge>::iterator it = base::incidences_list[id].begin(); it != base::incidences_list[id].end(); it++) {
				if (base::nodes[(*it).get_to()].get_color() == 0) {
					csc_order((*it).get_to(), order);
				}
			}
			order.push_back(id);
		}
		void connect_components(size_t node_id, directed_graph<WType, NodeInfo, EdgeInfo>& trp, directed_graph<WType, NodeInfo, EdgeInfo>& res, size_t clr) {
			trp.nodes[node_id].set_color(clr);
			for (typename std::list<edge>::iterator it = trp.incidences_list[node_id].begin(); it != trp.incidences_list[node_id].end(); it++) {
				if (trp.nodes[(*it).get_to()].get_color() == 0)
				{
					res.add_node((*it).get_to(), trp.nodes[node_id].get_info());
					connect_components((*it).get_to(), trp, res, clr);
					res.add_edge((*it).get_to(), (*it).get_from(), (*it).get_weight(), (*it).get_info());
				}
				else if (trp.nodes[(*it).get_to()].get_color() == clr) {
					res.add_edge((*it).get_to(), (*it).get_from(), (*it).get_weight(), (*it).get_info());
				}
			}
		}
	public:
		std::vector<directed_graph<WType, NodeInfo, EdgeInfo>> connect_components() {
			std::vector<directed_graph<WType, NodeInfo, EdgeInfo>> res;
			std::vector<size_t> order;
			for (typename std::unordered_map<size_t, node>::iterator it = base::nodes.begin(); it != base::nodes.end(); it++) {
				if ((*it).second.get_color() == 0)
					csc_order((*it).first, order);
			}
			for (typename std::unordered_map<size_t, node>::iterator it = base::nodes.begin(); it != base::nodes.end(); it++) {
				(*it).second.set_color(0);
			}
			directed_graph<WType, NodeInfo, EdgeInfo> trp = get_transponated();
			for (int i = order.size() - 1; i >= 0; i--) {
				if (trp.nodes[order[i]].get_color() == 0) {
					directed_graph<WType, NodeInfo, EdgeInfo> cur_gr;
					cur_gr.add_node(base::nodes[order[i]].get_id(), base::nodes[order[i]].get_info());
					connect_components(order[i], trp, cur_gr, i);
					res.push_back(cur_gr);
				}
			}
			return res;
		}
		void visualize() noexcept {
			std::ofstream ofs("g.dot");
			ofs << "digraph T {\n";
			for (typename std::unordered_map<size_t, node>::const_iterator it = base::nodes.cbegin(); it != base::nodes.cend(); it++) {
				base::print_node(ofs, (*it).second);
			}
			for (typename std::set<edge>::const_iterator it = base::edges.cbegin(); it != base::edges.cend(); it++) {
				base::print_edge(ofs, (*it));
			}
			ofs << "}";
			ofs.close();
			system("cd C:\\Users\\Артём\\source\\repos\\lab_6\\lab_6");
			system("dot g.dot -T png -o g.png");
			system("g.png");
		}
		void visualize(const path& path, size_t color) noexcept {
			for (typename std::list<edge>::const_iterator it = path.get_chain().cbegin(); it != path.get_chain().cend(); it++) {
				if (base::edges.find(*it) != base::edges.end()) {
					base::edges.erase((*it));
					edge ed((*it));
					ed.set_color(color);
					base::edges.emplace(ed);
					base::nodes[ed.get_from()].set_color(color);
					base::nodes[ed.get_to()].set_color(color);
				}
				else throw SetException(NoSuchElement);
			}
			visualize();
			for (typename std::list<edge>::const_iterator it = path.get_chain().cbegin(); it != path.get_chain().cend(); it++) {
				if (base::edges.find(*it) != base::edges.end()) {
					base::edges.erase((*it));
					edge ed((*it));
					ed.set_color(0);
					base::edges.emplace(ed);
					base::nodes[ed.get_from()].set_color(0);
					base::nodes[ed.get_to()].set_color(0);
				}
			}

		}
		void visualize(const std::vector<directed_graph<WType>>& vec) noexcept {
			for (size_t color = 1; color <= vec.size(); color++) {
				for (typename std::unordered_map<size_t, std::list<edge>>::const_iterator it = vec[color-1].incidences_list.cbegin(); it != vec[color - 1].incidences_list.cend(); it++) {
					if (base::nodes.find((*it).first) != base::nodes.cend()) {
						base::nodes[(*it).first].set_color(color);
					}
					else throw SetException(NoSuchElement);
				}
				for (typename std::set<edge>::const_iterator it = vec[color - 1].edges.cbegin(); it != vec[color - 1].edges.cend(); it++) {
					edge ed((*it).get_from(), (*it).get_to(), (*it).get_weight(), (*it).get_info());
					edge r_ed((*it).get_to(), (*it).get_from(), (*it).get_weight(), (*it).get_info());
					if (base::edges.find(ed) != base::edges.cend()) {
						base::edges.erase(ed);
						ed.set_color(color);
						base::edges.emplace(ed);
					}
					else throw SetException(NoSuchElement);
				}
			}
			visualize();
			for (typename std::unordered_map<size_t, node>::iterator it = base::nodes.begin(); it != base::nodes.end(); it++) {
				(*it).second.set_color(0);
			}
			for (int i = 0; i < vec.size(); i++) {
				for (typename std::set<edge>::const_iterator it = vec[i].edges.cbegin(); it != vec[i].edges.cend(); it++) {
					edge ed((*it).get_from(), (*it).get_to(), (*it).get_weight(), (*it).get_info());
					if (base::edges.find(ed) != base::edges.cend()) {
						base::edges.erase(ed);
						ed.set_color(0);
						base::edges.emplace(ed);
					}
				}
			}

		}
		void visualize(const directed_graph<WType>& other, size_t color) noexcept {
			for (typename std::unordered_map<size_t, std::list<edge>>::const_iterator it = other.incidences_list.cbegin(); it != other.incidences_list.cend(); it++) {
				if (base::nodes.find((*it).first) != base::nodes.cend()) {
					base::nodes[(*it).first].set_color(color);
				}
				else throw SetException(NoSuchElement);
			}
			for (typename std::set<edge>::const_iterator it = other.edges.cbegin(); it != other.edges.cend(); it++) {
				edge ed((*it).get_from(), (*it).get_to(), (*it).get_weight(), (*it).get_info());
				edge r_ed((*it).get_to(), (*it).get_from(), (*it).get_weight(), (*it).get_info());
				if (base::edges.find(ed) != base::edges.cend()) {
					base::edges.erase(ed);
					ed.set_color(color);
					base::edges.emplace(ed);
				}
				else throw SetException(NoSuchElement);
			}
			visualize();
			for (typename std::unordered_map<size_t, std::list<edge>>::const_iterator it = other.incidences_list.cbegin(); it != other.incidences_list.cend(); it++) {
				if (base::nodes.find((*it).first) != base::nodes.cend()) {
					base::nodes[(*it).first].set_color(0);
				}
			}
			for (typename std::set<edge>::const_iterator it = other.edges.cbegin(); it != other.edges.cend(); it++) {
				edge ed((*it).get_from(), (*it).get_to(), (*it).get_weight(), (*it).get_info());
				if (base::edges.find(ed) != base::edges.cend()) {
					base::edges.erase(ed);
					ed.set_color(0);
					base::edges.emplace(ed);
				}
			}

		}
		void add_graph(const directed_graph<WType, NodeInfo, EdgeInfo>& other) {
			for (typename std::unordered_map<size_t, node>::const_iterator it = other.nodes.cbegin(); it != other.nodes.cend(); it++) {
				this->add_node((*it).first, (*it).second.get_info());
			}
			for (typename std::set<edge>::const_iterator it = other.edges.cbegin(); it != other.edges.cend(); it++) {
				this->add_edge((*it).get_from(), (*it).get_to(), (*it).get_weight(), (*it).get_info());
			}
		}

		directed_graph<WType, NodeInfo, EdgeInfo> get_transponated() const noexcept {
			directed_graph<WType, NodeInfo, EdgeInfo> res;
			for (auto i : base::nodes) {
				res.add_node(i.first);
			}
			for (auto i : base::edges) {
				res.add_edge(i.get_to(), i.get_from(), i.get_weight(), i.get_info());
			}
			return res;
		}
	};

	template<typename WType = int, class NodeInfo = std::string, class EdgeInfo = std::string>
	class undirected_graph : public graph<WType, false, NodeInfo, EdgeInfo>
	{
	public:
		using node = node<NodeInfo>;
		using edge = edge<EdgeInfo, WType>;
		using path = path<EdgeInfo, WType>;
	private:
		using base = graph<WType, false, NodeInfo, EdgeInfo>;
		void connect_components(size_t node_id, undirected_graph<WType, NodeInfo, EdgeInfo>& res) {
			for (typename std::list<edge>::iterator it = base::incidences_list[node_id].begin(); it != base::incidences_list[node_id].end(); it++) {
				if (res.nodes.find((*it).get_to()) == res.nodes.end())
				{
					base::nodes[(*it).get_to()].set_color(1);
					res.add_node((*it).get_to());
					connect_components((*it).get_to(), res);
				}
				res.add_edge((*it).get_from(), (*it).get_to(), (*it).get_weight(), (*it).get_info());
			}
			
		}
		void print_edge(std::ofstream& ofs, const edge& ed) const noexcept override {
			const node& nd = (*base::nodes.find(ed.get_from())).second;
			ofs << nd.get_id();
			ofs << "--";
			const node& nd2 = (*base::nodes.find(ed.get_to())).second;
			ofs << nd2.get_id();
			ofs << " [";
			if (ed.get_color() % 11 == 1) {
				ofs << "color=\"red\",";
			}
			else if (ed.get_color() == 2) {
				ofs << "color=\"green\",";
			}
			else if (ed.get_color() == 3) {
				ofs << "color=\"blue\",";
			}
			else if (ed.get_color() == 4) {
				ofs << "color=\"yellow\",";
			}
			else if (ed.get_color() == 5) {
				ofs << "color=\"webmaroon\",";
			}
			else if (ed.get_color() == 6) {
				ofs << "color=\"cyan\",";
			}
			else if (ed.get_color() == 7) {
				ofs << "color=\"crimson\",";
			}
			else if (ed.get_color() == 8) {
				ofs << "color=\"darkorchid1\",";
			}
			else if (ed.get_color() == 9) {
				ofs << "color=\"goldenrod2\",";
			}
			else if (ed.get_color() == 10) {
				ofs << "color=\"lightslateblue\",";
			}
			ofs << "label=\"" << ed.get_info();
			if (base::weight_edges > 0) ofs << " {" << ed.get_weight() << "}";
			ofs << "\"];\n";
		}
	public:
		std::vector<undirected_graph<WType, NodeInfo, EdgeInfo>> skeleton() {
			std::vector<undirected_graph<WType, NodeInfo, EdgeInfo>> ans;
			size_t cur_bouqet = 1;
			std::vector<size_t> bouqettes(1);
			std::vector<undirected_graph<WType, NodeInfo, EdgeInfo>> cc = connect_components();
			for (typename std::vector<undirected_graph<WType, NodeInfo, EdgeInfo>>::iterator it_cc = cc.begin(); it_cc != cc.end(); it_cc++) {
				undirected_graph<WType, NodeInfo, EdgeInfo> res;
				if ((*it_cc).edges.size() == 0) {
					for (typename std::unordered_map<size_t, node>::const_iterator it_cur_n = (*it_cc).nodes.cbegin(); it_cur_n != (*it_cc).nodes.cend(); it_cur_n++) {
						res.add_node((*it_cur_n).second.get_id(), (*it_cur_n).second.get_info());
					}
					ans.push_back(res);
					continue;
				}
				
				res.n = (*it_cc).n;
				res.nodes.emplace((*(*it_cc).edges.cbegin()).get_from(), (*base::nodes.find((*(*it_cc).edges.cbegin()).get_from())).second);
				for (typename std::set<edge, typename base::less_x>::const_iterator it = (*it_cc).edges.cbegin(); it != (*it_cc).edges.cend(); it++) {
					typename std::unordered_map<size_t, node>::iterator from = base::nodes.find((*it).get_from());
					typename std::unordered_map<size_t, node>::iterator to = base::nodes.find((*it).get_to());
					if ((*to).second.get_color() == 0 && (*from).second.get_color() == 0) {
						res.nodes[(*it).get_from()] = (*from).second;
						res.nodes[(*it).get_to()] = (*to).second;
						base::nodes[(*it).get_to()].set_color(cur_bouqet);
						base::nodes[(*it).get_from()].set_color(cur_bouqet);
						bouqettes.push_back(cur_bouqet);
						cur_bouqet++;
						res.add_edge((*it).get_from(), (*it).get_to(), (*it).get_weight(), (*it).get_info());
					}
					else if (bouqettes[(*to).second.get_color()] != bouqettes[(*from).second.get_color()]) {
						if ((*to).second.get_color() == 0) {
							res.nodes[(*it).get_to()] = (*to).second;
							base::nodes[(*it).get_to()].set_color((*from).second.get_color());
						}
						else if ((*from).second.get_color() == 0) {
							res.nodes[(*it).get_from()] = (*from).second;
							base::nodes[(*it).get_from()].set_color((*to).second.get_color());
						}
						else {
							bouqettes[(*from).second.get_color()] = (*to).second.get_color();
						}
						res.add_edge((*it).get_from(), (*it).get_to(), (*it).get_weight(), (*it).get_info());
					}
				}
				ans.push_back(res);
			}
			for (typename std::unordered_map<size_t, node>::iterator it = base::nodes.begin(); it != base::nodes.end(); it++) {
				(*it).second.set_color(0);
			}
			return ans;
		}
		std::vector<undirected_graph<WType, NodeInfo, EdgeInfo>> connect_components() {
			std::vector<undirected_graph<WType, NodeInfo, EdgeInfo>> res;
			for (typename std::unordered_map<size_t, node>::iterator it = base::nodes.begin(); it != base::nodes.end(); it++) {
				if ((*it).second.get_color() == 0) {
					(*it).second.set_color(1);
					undirected_graph<WType, NodeInfo, EdgeInfo> new_cc;
					new_cc.add_node((*it).second.get_id(), (*it).second.get_info());
					connect_components((*it).second.get_id(), new_cc);
					res.push_back(new_cc);
				}
			}
			for (typename std::unordered_map<size_t, node>::iterator it = base::nodes.begin(); it != base::nodes.end(); it++) {
				(*it).second.set_color(0);
			}
			return res;
		}
		void visualize() noexcept {
			std::ofstream ofs("g.dot");
			ofs << "graph T {\n";
			for (typename std::unordered_map<size_t, node>::const_iterator it = base::nodes.cbegin(); it != base::nodes.cend(); it++) {
				base::print_node(ofs, (*it).second);
			}
			for (typename std::set<edge>::const_iterator it = base::edges.cbegin(); it != base::edges.cend(); it++) {
				print_edge(ofs, (*it));
			}
			ofs << "}";
			ofs.close();
			system("cd C:\\Users\\Артём\\source\\repos\\lab_6\\lab_6");
			system("dot g.dot -T png -o g.png");
			system("g.png");
		}
		void visualize(const path& path, size_t color) noexcept {
			for (typename std::list<edge>::const_iterator it = path.get_chain().cbegin(); it != path.get_chain().cend(); it++) {
				edge r_ed((*it).get_to(), (*it).get_from(), (*it).get_weight(), (*it).get_info(), (*it).get_color());
				if (base::edges.find(*it) != base::edges.end()) {
					base::edges.erase((*it));
					edge ed((*it));
					ed.set_color(color);
					base::edges.emplace(ed);
					base::nodes[ed.get_from()].set_color(color);
					base::nodes[ed.get_to()].set_color(color);
				}
				else if (base::edges.find(r_ed) != base::edges.end()) {
					base::edges.erase(r_ed);
					r_ed.set_color(color);
					base::edges.emplace(r_ed);
					base::nodes[r_ed.get_from()].set_color(color);
					base::nodes[r_ed.get_to()].set_color(color);
				}
				else throw SetException(NoSuchElement);
			}
			visualize();
			for (typename std::list<edge>::const_iterator it = path.get_chain().cbegin(); it != path.get_chain().cend(); it++) {
				edge r_ed((*it).get_to(), (*it).get_from(), (*it).get_weight(), (*it).get_info(), (*it).get_color());
				if (base::edges.find(*it) != base::edges.end()) {
					base::edges.erase((*it));
					edge ed((*it));
					ed.set_color(0);
					base::edges.emplace(ed);
					base::nodes[ed.get_from()].set_color(0);
					base::nodes[ed.get_to()].set_color(0);
				}
				else if (base::edges.find(r_ed) != base::edges.end()) {
					base::edges.erase(r_ed);
					r_ed.set_color(0);
					base::edges.emplace(r_ed);
					base::nodes[r_ed.get_from()].set_color(0);
					base::nodes[r_ed.get_to()].set_color(0);
				}
			}
		}
		void visualize(const std::vector<undirected_graph<WType>>& vec) noexcept {
			for (int color = 1; color <= vec.size(); color++) {
				for (typename std::unordered_map<size_t, std::list<edge>>::const_iterator it = vec[color-1].incidences_list.cbegin(); it != vec[color - 1].incidences_list.cend(); it++) {
					if (base::nodes.find((*it).first) != base::nodes.end()) {
						base::nodes[(*it).first].set_color(color);
					}
					else throw SetException(NoSuchElement);
				}
				for (typename std::set<edge>::const_iterator it = vec[color - 1].edges.cbegin(); it != vec[color - 1].edges.cend(); it++) {
					edge ed((*it).get_from(), (*it).get_to(), (*it).get_weight(), (*it).get_info());
					edge r_ed((*it).get_to(), (*it).get_from(), (*it).get_weight(), (*it).get_info());
					if (base::edges.find(ed) != base::edges.end()) {
						base::edges.erase(ed);
						ed.set_color(color);
						base::edges.emplace(ed);
					}
					else if (base::edges.find(r_ed) != base::edges.end()) {
						base::edges.erase(r_ed);
						r_ed.set_color(color);
						base::edges.emplace(r_ed);
					}
					else throw SetException(NoSuchElement);
				}
			}
			visualize();
			for (typename std::unordered_map<size_t, node>::iterator it = base::nodes.begin(); it != base::nodes.end(); it++) {
				(*it).second.set_color(0);
			}
			for (int i = 0; i < vec.size(); i++) {
				for (typename std::set<edge>::const_iterator it = vec[i].edges.cbegin(); it != vec[i].edges.cend(); it++) {
					edge ed((*it).get_from(), (*it).get_to(), (*it).get_weight(), (*it).get_info());
					edge r_ed((*it).get_to(), (*it).get_from(), (*it).get_weight(), (*it).get_info());
					if (base::edges.find(ed) != base::edges.cend()) {
						base::edges.erase(ed);
						ed.set_color(0);
						base::edges.emplace(ed);
					}
					else if (base::edges.find(r_ed) != base::edges.cend()) {
						base::edges.erase(r_ed);
						ed.set_color(0);
						base::edges.emplace(r_ed);
					}
				}
			}
		}
		void visualize(const undirected_graph<WType>& other, size_t color) noexcept {
			for (typename std::unordered_map<size_t, std::list<edge>>::const_iterator it = other.incidences_list.cbegin(); it != other.incidences_list.cend(); it++) {
				if (base::nodes.find((*it).first) != base::nodes.end()) {
					base::nodes[(*it).first].set_color(color);
				}
				else throw SetException(NoSuchElement);
			}
			for (typename std::set<edge>::const_iterator it = other.edges.cbegin(); it != other.edges.cend(); it++) {
				edge ed((*it).get_from(), (*it).get_to(), (*it).get_weight(), (*it).get_info());
				edge r_ed((*it).get_to(), (*it).get_from(), (*it).get_weight(), (*it).get_info());
				if (base::edges.find(ed) != base::edges.end()) {
					base::edges.erase(ed);
					ed.set_color(color);
					base::edges.emplace(ed);
				}
				else if (base::edges.find(r_ed) != base::edges.end()) {
					base::edges.erase(r_ed);
					r_ed.set_color(color);
					base::edges.emplace(r_ed);
				}
				else throw SetException(NoSuchElement);
			}
			visualize();
			for (typename std::unordered_map<size_t, std::list<edge>>::const_iterator it = other.incidences_list.cbegin(); it != other.incidences_list.cend(); it++) {
				if (base::nodes.find((*it).first) != base::nodes.end()) {
					base::nodes[(*it).first].set_color(0);
				}
			}
			for (typename std::set<edge>::const_iterator it = other.edges.cbegin(); it != other.edges.cend(); it++) {
				edge ed((*it).get_from(), (*it).get_to(), (*it).get_weight(), (*it).get_info());
				edge r_ed((*it).get_to(), (*it).get_from(), (*it).get_weight(), (*it).get_info());
				if (base::edges.find(ed) != base::edges.cend()) {
					base::edges.erase(ed);
					ed.set_color(0);
					base::edges.emplace(ed);
				}
				else if (base::edges.find(r_ed) != base::edges.cend()) {
					base::edges.erase(r_ed);
					ed.set_color(0);
					base::edges.emplace(r_ed);
				}
			}
		}
		void add_graph(const undirected_graph<WType>& other) {
			for (typename std::unordered_map<size_t, node>::const_iterator it = other.nodes.cbegin(); it != other.nodes.cend(); it++) {
				this->add_node((*it).first, (*it).second.get_info());
			}
			for (typename std::set<edge>::const_iterator it = other.edges.cbegin(); it != other.edges.cend(); it++) {
				this->add_edge((*it).get_from(), (*it).get_to(), (*it).get_weight(), (*it).get_info());
			}
		}
	};
}

