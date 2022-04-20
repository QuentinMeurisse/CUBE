#ifndef CUBE_GRAPH_H
#define CUBE_GRAPH_H

#include <algorithm>
#include <boost/heap/fibonacci_heap.hpp>
#include <fstream>
#include <memory>
#include <nlohmann/json.hpp>
#include <numeric>
#include <random>
#include <sstream>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>
#include <exception>
#include <optional>
#include <future>
#include <iostream>

// This graph implementation solves two kind of problems on dynamic graph:
//     - Fastest paths problem: where only the time to go
//       between two nodes is important for the solution
//     - Minimum cost paths problem: where the path cost
//       between two nodes is important for the solution
// ProblemType enum provides the choice between these two problems.
// An instance of this enum can be passed to shortest paths algorithms
// (dijkstra or dot) to choose between the two kind of problems mentioned above.
// To use Dijktra on a static graphe, use shortest_paths type
// Example:
//     Graph g;
//     /* graph initialization omitted */
//     auto costs = g.dijkstra(src_id, ProblemType::fastest_paths);
enum ProblemType {
    fastest_paths,
    min_cost_paths,
    shortest_path,
    num_edges,
};

// Represents a dynamic directed graph with nodes and edges.
// The particularity of a dynamic graph is that the cost of edges are time dependant.
// This graph representation also implements an all-to-one fastest/minimum-cost paths
// algorithm based on the paper from Ismail Chabini (https://doi.org/10.3141%2F1645-21)
template<class NodeData = int, class EdgeData = int>
class Graph {

protected:
    class Node;

    class Edge;

public:
    virtual ~Graph() {};

    // Adds a new node in the graph.
    // Returns the unique id the node newly created
    auto addNode() -> uint64_t {
        const std::size_t id = p_nodes.size();
        addNode(id);
        return id;
    }

    // Adds a new node in the graph with the `id` given as an argument
    void addNode(uint64_t id) {
        p_id_to_index[id] = p_nodes.size();
        p_nodes.push_back(std::make_shared<Node>(id));
    }

    // Adds a new node in the graph with the `id` given in the arguments
    // and initialize the data of the newly created node to `data`.
    void addNode(uint64_t id, NodeData data) {
        p_id_to_index[id] = p_nodes.size();
        p_nodes.push_back(std::make_shared<Node>(id, data));
    }

    void removeNode(uint64_t id){
        size_t node_index = findNodeIndex(id);
        const auto &node = p_nodes.at(node_index);
        for (const auto&[v, edge] : node->m_successors){
            v->removePredecessor(node.get());
        }
        for (const auto&[v, edge] : node->m_predecessors){
            v->removeSuccessor(node.get());
        }
        p_nodes.erase(p_nodes.begin() + node_index);

        for (auto const&[n, i] : p_id_to_index){
            if (i > node_index)
                p_id_to_index[n] = i - 1;
        }
        p_id_to_index.erase(id);

    }

    // Adds a new directed edge (a successor) from the source node `src_id`
    // to the destination node `dst_id`
    void addSuccessor(uint64_t src_id, uint64_t dst_id) {
        const auto &src = p_nodes.at(findNodeIndex(src_id));
        const auto &dst = p_nodes.at(findNodeIndex(dst_id));
        // Retrieves raw pointers
        // These pointer are never shared with the user and automatically
        // destroyed when the Graph object goes out of scope.
        Node *src_ptr = src.get();
        Node *dst_ptr = dst.get();
        auto e = std::make_shared<Edge>();
        src->m_successors[dst_ptr] = e;
        dst->m_predecessors[src_ptr] = e;
    }

    void addSuccessor(uint64_t src_id, uint64_t dst_id, EdgeData data){
        const auto &src = p_nodes.at(findNodeIndex(src_id));
        const auto &dst = p_nodes.at(findNodeIndex(dst_id));
        // Retrieves raw pointers
        // These pointer are never shared with the user and automatically
        // destroyed when the Graph object goes out of scope.
        Node *src_ptr = src.get();
        Node *dst_ptr = dst.get();
        auto e = std::make_shared<Edge>();
        src->m_successors[dst_ptr] = e;
        dst->m_predecessors[src_ptr] = e;
        e->m_data = data;
    }

    // Adds travel times to the edge between the `src_id` and the `dst_id`
    // The argument `time_interval` must correspond to the time value between two time points.
    void addTimes(uint64_t src_id, uint64_t dst_id, std::vector<unsigned> times,
                  unsigned time_interval) {
        const auto edge = getEdge(src_id, dst_id);
        edge->set_times(times);
        p_time_interval = time_interval;
    }

    // Adds travel costs to the edge between the `src_id` and the `dst_id`
    // The argument `time_interval` must correspond to the time value between two costs points.
    void
    addCosts(uint64_t src_id, uint64_t dst_id, std::vector<double> costs, unsigned time_interval) {
        const auto edge = getEdge(src_id, dst_id);
        edge->set_costs(costs);
        p_time_interval = time_interval;
    }

    // Returns the position of the node designed by the `id`.
    // Example:
    //     Graph g;
    //     /* graph initialization omitted */
    //     auto costs = g.dijkstra(src_id, ProblemType::fastest_paths);
    //     // Fastest time to go from `src_id` to `dst_id`
    //     std::cout << costs[g.findNodeIndex(dst_id)] << '\n';
    size_t findNodeIndex(uint64_t id) const {
        auto it = p_id_to_index.find(id);
        if (it == p_id_to_index.cend()) {
            std::stringstream ss;
            ss << "The requested id " << id << ",does not correspond to a node in the graph";
            throw std::out_of_range(ss.str());
        }
        return it->second;
    }

    NodeData& getNodeData(uint64_t node_id) const {
        return p_nodes[findNodeIndex(node_id)]->m_data;
    }

    EdgeData& getEdgeData(uint64_t src_id, uint64_t dst_id) const {
        return getEdge(src_id, dst_id)->m_data;
    }

    void setNodeData(uint64_t node_id, NodeData data) {
        const auto &node = p_nodes[findNodeIndex(node_id)];
        node->m_data = data;
    }

    void setEdgeData(uint64_t src_id, uint64_t dst_id, EdgeData data) {
        const auto &edge = getEdge(src_id, dst_id);
        edge->m_data = data;
    }

    std::vector<uint64_t> getSuccessors(uint64_t src_id) const{
        std::vector<uint64_t> succ;
        size_t index = findNodeIndex(src_id);
        auto const& node = p_nodes[index];
        for (auto const& [v, e] : node->m_successors){
            succ.push_back(v->get_id());
        }
        return succ;
    }

    double getEdgeCost(uint64_t src_id, uint64_t dst_id, ProblemType problem_type, double t) const{
        auto edge = getEdge(src_id, dst_id);
        uint t_unsigned = static_cast<uint>(t);
        return edge->get_cost(problem_type, t_unsigned, p_time_interval);
    }



    // Returns a vector containing the minimum costs to go from the `src_id`
    // to all the other nodes. The minimum costs depends on the type of problem
    // asked using the `problem_type` argument (see comment of the `ProblemType`
    // enum for more information).
    // Note: A value over 2e9 is considered as infinity and means that there are no paths
    // Example:
    //     Graph g;
    //     /* graph initialization omitted */
    //     auto costs = g.dijkstra(src_id, ProblemType::fastest_paths);
    //     // Fastest time to go from `src_id` to `dst_id`
    //     std::cout << costs[g.findNodeIndex(dst_id)] << '\n';
    std::vector<double> dijkstra(uint64_t src_id, ProblemType problem_type){
        return dijkstraGeneral(src_id, problem_type, false);
    }

    // Returns a vector containing the minimum costs to go from all nodes
    // to the `dst_id` node. The minimum costs depends on the type of problem
    // asked using the `problem_type` argument (see comment of the `ProblemType`
    // enum for more information).
    // In term of implementation this version of dijkstra use the predecessors
    // of a node instead of the successors used in the original function.
    // Note: A value over 2e9 is considered as infinity and means that there are no paths
    // Example:
    //     Graph g;
    //     /* graph initialization omitted */
    //     auto costs = g.dijkstraReverse(dst_id, ProblemType::fastest_paths);
    //     // Fastest time to go from `src_id` to `dst_id`
    //     std::cout << costs[g.findNodeIndex(src_id)] << '\n';
    std::vector<double> dijkstraReverse(uint64_t dst_id, ProblemType problem_type){
        return dijkstraGeneral(dst_id, problem_type, true);
    }

    std::vector<std::vector<double>> multicoreDijkstra(ProblemType problem_type, uint num_cores){
        auto f = [&] (unsigned int left, unsigned int right){
            std::vector<std::vector<double >> v;
            for (uint64_t i = left; i < right; ++i) {
                v.push_back(dijkstra((*this)[i], problem_type));
            }
            return v;
        };

        uint delim = size() / num_cores;
        std::vector<std::future<std::vector<std::vector<double>>>> futures;
        futures.reserve(num_cores);
        for (uint i = 0; i < (num_cores - 1) * delim; i += delim){
            futures.push_back(std::async(f, i, i+delim));
        }
        futures.push_back(std::async(f, (num_cores -1 ) * delim, size()));

        std::vector<std::vector<double>> to_return;
        for (auto & a : futures){
            std::vector<std::vector<double>> m = a.get();
            to_return.insert(to_return.end(), m.begin(), m.end());
        }
        return to_return;
    }

    // Returns a 2D vector containing the minimum cost paths from the all nodes
    // to the `dst_id` node for each departure time `t` between 0 and `staticTime` included.
    // Where the `staticTime` is defined as the time `M` after which the cost of each link
    // is static and therefore does not depend on the time.
    // This function implements the Decreasing Order of Time (DOT) algorithm invented by Ismail Chabini.
    // Example:
    //     Graph g;
    //     /* graph initialization omitted */
    //     auto costs = g.dot(dst_id, ProblemType::fastest_paths);
    //     // Fastest time to go from `src_id` to `dst_id` with a departure time at time `10`
    //     std::cout << costs[10][g.findNodeIndex(src_id)] << '\n';
    std::vector<std::vector<double>> dot(uint64_t dst_id, ProblemType problem_type){
        return dotInternal(dst_id, problem_type, 0);
    }

    std::vector<double> dot(uint64_t dst_id, ProblemType problem_type, int start_time){
        auto all_time_from_start = dotInternal(dst_id, problem_type, start_time);
        return all_time_from_start[start_time];
    }

    std::vector<std::vector<double>> multiCoreDot(const std::vector<uint64_t>& dst_ids,
                                                  ProblemType problem_type,
                                                  int start_time,
                                                  uint num_cores){

        auto f = [&](int left, int right){
            std::vector<std::vector<double>> v;
            for (uint i = left; i < right; ++i) {
                v.push_back(dot(dst_ids[i], problem_type, start_time));
            }
            return v;
        };

        uint delim = dst_ids.size() / num_cores;
        std::vector<std::future<std::vector<std::vector<double>>>> futures;
        futures.reserve(num_cores);
        for (uint i = 0; i < (num_cores - 1) * delim; i += delim){
            futures.push_back(std::async(f, i, i+delim));
        }
        futures.push_back(std::async(f, (num_cores -1 ) * delim, dst_ids.size()));

        std::vector<std::vector<double>> to_return;
        for (auto & a : futures){
            std::vector<std::vector<double>> m = a.get();
            to_return.insert(to_return.end(), m.begin(), m.end());
        }
        return to_return;
    }

    // Returns the size of the graph (ie. the number of node it contains).
    size_t size() const{
        return p_nodes.size();
    }

    // Returns the number of edges in the graph.
    size_t numberEdges() const{
        return std::accumulate(p_nodes.cbegin(), p_nodes.cend(), static_cast<size_t>(0),
                               [](size_t acc, const auto &node) {
                                   return acc + node->m_successors.size();
                               });
    }

    /**
     *
     * @param k
     * @return the id of the node at position k
     */
    uint64_t operator[](size_t k){
        return p_nodes[k]->get_id();
    }

    /**
     *
     * @param src_id
     * @param n
     * @return the nodes at n edges from src_id
     */
    std::vector<uint64_t> nSuccessors(uint64_t src_id, int n){
        std::vector<double> dist = dijkstra(src_id, ProblemType::num_edges);
        std::vector<uint64_t> to_return;
        for (int i = 0; i < dist.size(); ++i) {
            if (dist[i] == n)
                to_return.push_back((*this)[i]);
        }
        return to_return;
    }

    virtual Graph<NodeData, EdgeData> operator+(Graph<NodeData, EdgeData> const& other) const{
        Graph<NodeData, EdgeData> g;
        uint64_t k = 1;
        std::map<uint64_t, uint64_t> old_to_new;
        for (int i = 0; i < this->size(); ++i) {
            uint64_t node = this->p_nodes[i]->get_id();
            g.addNode(k, this->getNodeData(node));
            old_to_new[node] = k;
            k++;
        }
        for (int i = 0; i < this->size(); ++i) {
            uint64_t node = this->p_nodes[i]->get_id();
            for (uint64_t const& succ : this->getSuccessors(node)){
                uint64_t g_src = old_to_new[node];
                uint64_t g_dst = old_to_new[succ];
                g.addSuccessor(g_src, g_dst, this->getEdgeData(node, succ));
            }
        }

        old_to_new.clear();

        for (int i = 0; i < other.size(); ++i) {
            uint64_t node = other.p_nodes[i]->get_id();
            g.addNode(k, other.getNodeData(node));
            old_to_new[node] = k;
            k++;
        }
        for (int i = 0; i < other.size(); ++i) {
            uint64_t node = other.p_nodes[i]->get_id();
            for (uint64_t const& succ : other.getSuccessors(node)){
                uint64_t g_src = old_to_new[node];
                uint64_t g_dst = old_to_new[succ];
                g.addSuccessor(g_src, g_dst, other.getEdgeData(node, succ));
            }
        }
        return g;
    }

    // Loads the content of a DOT (Graph Description Language) file.
    // Example of DOT file:
    //
    //   strict digraph {
    //   splines=true
    //   node [shape=circle, style="filled", pin=true]
    //   0
    //   1
    //   2
    //
    //   0 -> 1
    //   0 -> 2
    //   2 -> 3
    //   }
    void loadDot(const std::string &path) {
        std::ifstream reader(path);
        if (not reader.is_open()) {
            throw std::invalid_argument("Can not open the file " + path);
        }
        loadDot(reader);
        reader.close();
    }

    void loadDot(std::istream &input) {
        std::string line;
        for (int i = 0; i < 3; ++i) {
            getline(input, line);
        }
        // Pass these lines (useless to build a graph) :
        /*  strict graph {
            splines=true
            node [shape=circle, style="filled", pin=true]
        */
        while (getline(input, line) and !line.empty()) {
            std::stringstream ss(line);
            uint64_t idnum;
            ss >> idnum;
            addNode(idnum);

            std::string s;
            getline(ss, s);
            while (s[0] == ' ')
                s.erase(0, 1);                //remove the white spaces
            s.erase(0, 1);                    //remove [
            s.erase(s.size() - 1, s.size());  //remove ]
            ss = std::stringstream(s);
            std::string word;
            while (getline(ss, word, ' ')) {
                std::stringstream ws(word);
                std::string key;
                getline(ws, key, '=');
                std::string value;
                getline(ws, value, '=');
                // TODO: add data in node
                //key == "fillcolor" ? node->set_color(std::stoul(value)) : node->add_data(key, std::stoul(value));
                if (key == "fillcolor") {
                    // Remove quotes around "value" and #
                    value.erase(0, 2);
                    value.erase(value.size() - 1, value.size());
                    //const unsigned value_u = std::strtoul(value.c_str(), NULL, 16);
                    // TODO: Actually use these values
                }
            }
        }

        while (getline(input, line) and line != "}") {
            std::stringstream ss(line);
            unsigned int n1;
            std::string word;
            ss >> n1;    // first node
            ss >> word;  // ->
            unsigned int n2;
            ss >> n2;  // second node
            addSuccessor(n1, n2);
        }
    }

    // Loads the content of a CSV file containing the travel times of each edge.
    // Example of CSV file:
    //
    //  Time, 0, 5, 10
    //  0 -> 1, 4, 3, 3
    //  0 -> 2, 5, 3, 4
    //  1 -> 3, 2, 4, 3
    //  2 -> 4, 2, 4, 4
    //  3 -> 5, 1, 2, 3
    //  4 -> 5, 2, 2, 2
    // The first line indicates the time where each value is the start time
    // of a new interval.
    // The rest of the lines contains in the first column the edge src_id -> dst_id.
    // Each value after the first column corresponds to the travel time between
    // the `src_id` and `dst_id` nodes at the time `t` in the first line.
    void loadEdgesTimes(const std::string &path) {
        std::ifstream reader(path);
        if (not reader.is_open()) {
            std::stringstream ss;
            ss << "Cannot open the file " << path;
            throw std::invalid_argument(ss.str());
        }
        loadEdgesTimes(reader);
        reader.close();
    }

    void loadEdgesTimes(std::istream &input) {
        const auto &edges_times = loadEdgesCosts<unsigned>(input);
        for (const auto &edge_times : edges_times) {
            addTimes(edge_times.src_id, edge_times.dst_id, edge_times.costs, p_time_interval);
        }
    }

    void loadEdgesCosts(const std::string &times_path, const std::string &costs_path) {
        std::ifstream times_reader(times_path);
        if (not times_reader.is_open()) {
            std::stringstream ss;
            ss << "Cannot open the file " << times_path;
            throw std::invalid_argument(ss.str());
        }
        loadEdgesTimes(times_reader);
        std::ifstream costs_reader(times_path);
        if (not costs_reader.is_open()) {
            std::stringstream ss;
            ss << "Cannot open the file " << costs_path;
            throw std::invalid_argument(ss.str());
        }
        loadEdgesCosts(times_reader, costs_reader);
        times_reader.close();
        costs_reader.close();
    }

    void loadEdgesCosts(std::istream &times_input, std::istream &costs_input) {
        loadEdgesTimes(times_input);
        const auto &edges_costs = loadEdgesCosts < double > (costs_input);
        for (const auto &edge_costs : edges_costs) {
            addCosts(edge_costs.src_id, edge_costs.dst_id, edge_costs.costs, p_time_interval);
        }
    }

    // Write the contents of the costs by departure time calculated from the `dot` function
    // to a output stream `os`.
    std::ostream &
    writeCsvCosts(std::ostream &os, std::vector<std::vector<double>> &costs){
        const char delimiter = ',';
        os << "Departure time / Node id" << delimiter;
        for (size_t i = 0; i < p_nodes.size() - 1; i++) {
            os << p_nodes[i]->get_id() << delimiter;
        }
        os << p_nodes[p_nodes.size() - 1]->get_id() << '\n';
        for (size_t i = 0; i < costs.size(); i++) {
            const size_t row_size = costs[i].size();
            os << i * p_time_interval << delimiter;
            for (size_t j = 0; j < row_size - 1; j++) {
                os << costs[i][j] << delimiter;
            }
            if (i != costs.size()) {
                os << costs[i][row_size - 1] << '\n';
            }
        }
        return os;
    }

    void loadJson(const std::string &path,
                  const std::optional<std::string>& edge_data_key = {},
                  const std::optional<std::string>& node_data_key = {},
                  const std::optional<std::string>& edge_times_key = {}) {
        std::ifstream reader(path);
        if (not reader.is_open()) {
            throw std::invalid_argument("Cannot open the file " + path);
        }
        nlohmann::json j;
        reader >> j;
        for (const auto &node : j["nodes"]) {
            if (node_data_key.has_value()) {
                NodeData data = node[node_data_key.value()];
                addNode(node["id"], data);
            }
            else{
                addNode(node["id"]);
            }
        }
        for (const auto &edge : j["links"]) {
            const uint64_t src_id = edge["source"];
            const uint64_t tgt_id = edge["target"];
            addSuccessor(src_id, tgt_id);

            const auto &e = getEdge(src_id, tgt_id);
            if (edge_data_key.has_value()){
                EdgeData data = edge[edge_data_key.value()];
                e->m_data = data;
            }
            else {
                e->m_data = 1;
            }
            if (edge_times_key.has_value()){
                std::vector<double> times = edge[edge_times_key.value()];
                std::vector<uint> rounded_times;
                rounded_times.reserve(times.size());
                std::transform(times.begin(), times.end(), std::back_inserter(rounded_times),
                               [](double d){return uint(std::round(d));});
                e->setTimes(rounded_times);
            }
        }
        if (edge_times_key.has_value()){
            p_time_interval = j["graph"]["time_interval"];
        }
    }

    void writeJson(std::string const& path){
        nlohmann::json json;
        std::vector<nlohmann::json> nodes_json;
        std::vector<nlohmann::json> edges_json;
        for(auto const& node : p_nodes){
            nlohmann::json node_json;
            uint64_t node_id = node->get_id();
            node_json["id"] = node_id;
            node_json["data"] = node->m_data;
            nodes_json.push_back(node_json);
            for (uint64_t const& succ : getSuccessors(node_id)){
                nlohmann::json edge_json;
                edge_json["source"] = node_id;
                edge_json["target"] = succ;
                edge_json["weight"] = getEdgeData(node_id, succ);
                edges_json.push_back(edge_json);
            }
        }
        json["nodes"] = nodes_json;
        json["links"] = edges_json;

        std::ofstream writer;
        writer.open(path, std::ios::out);
        writer << json.dump(4);
        writer.close();
    }



protected:
    std::shared_ptr<Edge> getEdge(uint64_t src_id, uint64_t dst_id) const {
        const auto &src = p_nodes[findNodeIndex(src_id)];
        const auto &dst = p_nodes[findNodeIndex(dst_id)];
        const auto &edge = src->getEdge(dst.get());
        if (edge == nullptr) {
            std::stringstream ss;
            ss <<"Error while searching edge, " << src->get_id() << " -> " << dst->get_id() << " is not an edge the graph";
            throw std::runtime_error(ss.str());
        }
        return edge;
    }

    std::vector<double>  dijkstraGeneral(uint64_t src_id, ProblemType problem_type,
                                         bool reverse_graph){
        using FiboHeap = boost::heap::fibonacci_heap<std::pair<double, size_t>, boost::heap::compare<std::greater<>>>;
        using FiboHandle = FiboHeap::handle_type;

        const size_t src_index = findNodeIndex(src_id);
        const double infty = std::numeric_limits<double>::infinity();
        std::vector<double> distances;
        std::vector<FiboHandle> fh_handles;  // List of Fibo heap's nodes. Use to increase the priority
        FiboHeap fh;
        for (size_t i = 0; i < p_nodes.size(); ++i) {
            double d = i == src_index ? 0. : infty;
            distances.push_back(d);
            fh_handles.push_back(fh.push(std::pair<double, size_t>(d, i)));
        }
        while (not fh.empty()) {
            const auto[curr_dist, u_index] = fh.top();
            fh.pop();
            const auto &current = p_nodes[u_index];
            const auto &adjacents = reverse_graph ? current->m_predecessors : current->m_successors;
            // for each edge `u` -> `v`
            for (const auto&[v, edge] : adjacents) {
                const uint64_t v_index = findNodeIndex(v->get_id());
                const double alt = curr_dist + edge->getCost(problem_type);
                if (alt < distances[v_index]) {
                    // A shorter path to go to `v` was found
                    distances[v_index] = alt;
                    fh.increase(fh_handles[v_index], std::pair<double, size_t>(alt, v_index));
                }
            }
        }
        return distances;
    }

    std::vector<std::vector<double>> dotInternal(uint64_t dst_id, ProblemType problem_type, int const& start_time){
        if (problem_type == shortest_path)
            throw std::invalid_argument(
                    "DOT algorithm must be used on a dynamic graph. ProblemType::shortest_path value "
                    "must be used only with a static graph");
        const size_t dst_index = findNodeIndex(dst_id);
        const double infty = std::numeric_limits<double>::infinity();
        // Travel times costs initialization (ttc)
        const unsigned static_time = findMaxStaticTime();
        std::vector<std::vector<double>> ttc(static_time + 1,
                                             std::vector<double>(p_nodes.size(), infty));
        for (size_t t = 0; t < static_time; t++) {
            ttc[t][dst_index] = 0;
        }
        // Satic shortest paths from every node to the `dst` node
        ttc[static_cast<size_t>(static_time)] = dijkstraReverse(dst_id, problem_type);
        for (int t = static_cast<int>(static_time) - 1; t >= 0; t--) {
            for (size_t i = 0; i < p_nodes.size(); i++) {
                const auto &n1 = p_nodes[i];
                for (const auto&[n2, edge] : n1->m_successors) {
                    // Edge i -> j (where i and j are the indices in the underlying array
                    const unsigned t_unsigned = static_cast<unsigned>(t);
                    const size_t j = findNodeIndex(n2->get_id());
                    const double c = edge->getCost(problem_type, t_unsigned, p_time_interval);
                    const unsigned d = static_cast<unsigned>(edge->getCost(
                            ProblemType::fastest_paths, t_unsigned, p_time_interval));
                    const unsigned td_index =
                            t_unsigned + d <= static_time ? t_unsigned + d : static_time;
                    ttc[t_unsigned][i] = std::min(ttc[t_unsigned][i], c + ttc[td_index][j]);
                }
            }
        }
        return ttc;
    }

    // Returns the time from which the time/cost of all edges is constant.
    unsigned findMaxStaticTime() const{
        unsigned max_static_time = 0;
        for (const auto &n : p_nodes) {
            for (const auto&[_, edge] : n->m_successors) {
                const unsigned max_iter = edge->staticTime(p_time_interval);
                if (max_iter > max_static_time) {
                    max_static_time = max_iter;
                }
            }
        }
        return max_static_time;
    }

    template<class T>
    std::pair<std::string, std::vector<T>> parse_line(std::string &line, char delimiter) {
        std::stringstream ss_line(line);
        std::string col_name;
        getline(ss_line, col_name, delimiter);
        std::vector<T> costs;
        std::string col_raw;
        while (getline(ss_line, col_raw, delimiter)) {
            costs.push_back(static_cast<T>(std::stoul(col_raw)));
        }
        return make_pair(col_name, costs);
    }

    std::pair<uint64_t, uint64_t> parse_edge(std::string &edge) {
        std::stringstream ss_edge(edge);
        uint64_t first, second;
        std::string delimiter;  // can be any string between the two node ids.
        ss_edge >> first >> delimiter >> second;
        return std::make_pair(first, second);
    }

    template<class T>
    struct EdgeCost {
        uint64_t src_id;
        uint64_t dst_id;
        std::vector<T> costs;
    };

    // Returns a vector that contains all the EdgeCost from the input stream
    // The input stream must contains a CSV content.
    template<class T>
    std::vector<EdgeCost<T>> loadEdgesCosts(std::istream &input){
        const char delimiter = ',';
        std::string line;
        getline(input, line);
        auto times = parse_line<unsigned>(line, delimiter).second;
        p_time_interval = times.size() > 1 ? times[1] - times[0] : 1;

        std::vector<Graph::EdgeCost<T>> edge_costs;
        while (getline(input, line)) {
            auto[raw_edge, costs] = parse_line<T>(line, delimiter);
            auto[n1_id, n2_id] = parse_edge(raw_edge);
            edge_costs.push_back(EdgeCost<T>{n1_id, n2_id, costs});
        }
        return edge_costs;
    }

    // Maps the node `id` to the actual position in the `p_nodes` vector.
    std::unordered_map<uint64_t, size_t> p_id_to_index;
    std::vector<std::shared_ptr<Node>> p_nodes;
    // Time interval between two times data
    // For example, if two observations were made at time 5 and 10 respectively
    // the time interval between these two observations is 10 - 5 = 5.
    unsigned p_time_interval{1};

    // Represents a Node in the dynamic directed graph
    // A Node contains an id, pointers to his successors, predecessors
    // and a coordinate struct.
    // In the future, this class should be enhanced by using templates
    // to allow more flexibility in the data it contains.
    class Node {

    public:
        Node() = delete;

        explicit Node(std::uint64_t id) : p_id{id} {}

        Node(std::uint64_t id, NodeData data) : p_id{id}, m_data{std::move(data)} {}

        Node(const Node &temp_obj) = delete;

        Node &operator=(const Node &temp_obj) = delete;

        // Returns true if the node has as succesor the `dst` node.
        bool hasSuccessor(Node *dst) const {
            return m_successors.find(dst) != m_successors.cend();
        }

        // Returns the edge between this node and the `dst` node
        // If the edge does not exist a `nullptr` is returned.
        std::shared_ptr<Edge> getEdge(Node *dst) const {
            const auto &it = m_successors.find(dst);
            return it != m_successors.cend() ? it->second : nullptr;
        }

        void removeSuccessor(Node* dst){
            m_successors.erase(dst);
        }

        void removePredecessor(Node* src){
            m_predecessors.erase(src);
        }

        // Note: These two maps use raw pointers because the usage
        // of a smart pointer produce memory leaks.
        // Indeed, since we are storing a directed graph that can
        // contains cycle, `shared_ptr` can not be used because
        // his internal counter will never drop to 0.
        // However, using raw pointers does not cause any problem
        // in this case because the graph does not remove any node
        // after its creation. Consequently, when the graph goes out
        // of scope, all the nodes are correctly freed because the
        // graph class store the nodes using `shared_ptr`.
        std::unordered_map<Node *, std::shared_ptr<Edge>> m_successors;
        std::unordered_map<Node *, std::shared_ptr<Edge>> m_predecessors;

        NodeData m_data;

        [[nodiscard]] uint64_t get_id() const {
            return p_id;
        }

    private:
        uint64_t p_id;
    };

    // Represents a dynamic Edge in the directed graph.
    // Because, this edge is dynamic it contains functions
    // to retrieve the cost of the edge time dependently.
    class Edge {
    public:
        Edge() = default;

        explicit Edge(std::vector<unsigned> times) {
            p_times = std::move(times);
        }

        Edge(std::vector<unsigned> times, std::vector<double> costs) {
            p_times = std::move(times);
            p_costs = std::move(costs);
        }

        // Returns the static time.
        //(ie. the time when the cost of the edge does not change afterward)
        [[nodiscard]] unsigned int staticTime(unsigned time_interval) const {
            if (not p_times.empty()) {
                return static_cast<unsigned>(time_interval * (p_times.size() - 1));
            }
            return 0;
        }

        // Returns the cost depending on the `ProblemType` at static time.
        [[nodiscard]] double getCost(ProblemType problem_type) const {
            switch (problem_type) {
                case fastest_paths:
                    return static_cast<double>(p_times.back());
                case min_cost_paths:
                    return p_costs.back();
                case shortest_path:
                    return static_cast<double>(m_data);
                case num_edges:
                    return 1.;
                default:
                    return 0;
            }
        }

        // Returns the cost depending on the `ProblemType` at time `t`.
        [[nodiscard]] double
        getCost(ProblemType problem_type, unsigned t, unsigned time_interval) const{
            unsigned index = t / time_interval;
            switch (problem_type) {
                case fastest_paths: {
                    unsigned cost = index < p_times.size() ? p_times[index] : p_times.back();
                    return static_cast<double>(cost);
                }
                case min_cost_paths:
                    return index < p_costs.size() ? p_costs[index] : p_costs.back();
                case shortest_path:
                    return static_cast<double>(m_data);
                case num_edges:
                    return 1.;
                default:
                    return 0;
            }
        }

        void setTimes(std::vector<unsigned> times) {
            p_times = std::move(times);
        }

        void setCosts(std::vector<double> costs) {
            p_costs = std::move(costs);
        }

        EdgeData m_data;

    private:
        std::vector<unsigned> p_times;
        std::vector<double> p_costs;
    };
};

// Class representing a dynamic directed graph with travel times/costs
// between nodes randomly generated.
class RandomCostsGraph : public Graph<nlohmann::json, double> {
public:
    using typename Graph<nlohmann::json, double>::Node;
    using typename Graph<nlohmann::json, double>::Edge;
    using Graph<nlohmann::json, double>::p_nodes;

    // Options for the random travel times/costs
    struct Options {
        unsigned time_interval{1};    // interval of time between two measures
        unsigned time_serie_size{1};  // size of the travel times/costs vector
        unsigned average_speed{1};    // average speed in km/h
        double min_cost{0};
        double max_cost{100};
    };

    RandomCostsGraph() = delete;

    explicit RandomCostsGraph(Options options) : p_options{options} {
        std::random_device rd;
        std::default_random_engine generator(rd());
        p_generator = generator;
    }

    // Loads the content of a json file that contains a graph made
    // by the `qgis_script.py`
    void loadJson(const std::string &path){
        std::ifstream reader(path);
        if (not reader.is_open()) {
            throw std::invalid_argument("Can not open the file " + path);
        }
        nlohmann::json j;
        reader >> j;
        std::map<std::vector<double>, uint64_t> coords_to_id;
        // Loads all the nodes into the graph
        for (const auto &node : j["nodes"]) {
            const auto it = node.find("osm_id");
            if (it != node.end()) {
                coords_to_id[node["id"]] = *it;
                addNode(*it);
            } else {
                coords_to_id[node["id"]] = addNode();
            }
        }
        // Loads edges into the graph
        for (const auto &edge_json : j["links"]) {
            const uint64_t src_id = coords_to_id[edge_json["source"]];
            const uint64_t dst_id = coords_to_id[edge_json["target"]];
            // Add a undirected edge between `src_id` and `dst_id`
            // This can be enhanced if there is some data attribute
            // in the JSON file indicating the direction of the edge
            addSuccessor(src_id, dst_id);
            addSuccessor(dst_id, src_id);
            const auto &edge1 = getEdge(src_id, dst_id);
            const auto &edge2 = getEdge(dst_id, src_id);
            edge1->m_data = edge_json["length"];
            edge2->m_data = edge_json["length"];
        }
        // Generate random time series on the edges
        randomCosts();
        reader.close();
    }

private:
    // Generates random travel times/costs for each edge in the graph
    // based on the Options given through the constructor of the class.
    void randomCosts() {
        for (const auto &node : p_nodes) {
            for (const auto&[successor, edge] : node->m_successors) {
                // Generate a random time serie that respects the FIFO condition
                const double dist = edge->m_data;
                // base_time in secondes
                const auto base_time = static_cast<unsigned>(round(
                        dist / (p_options.average_speed / 3.6)));
                const unsigned sigma = [&]() {
                    // Ensures the respect of the FIFO condition
                    if (unsigned sigma = base_time / 4; sigma <= p_options.time_interval) {
                        return sigma;
                    }
                    return p_options.time_interval;
                }();

                // TODO: Make something better that really simulate a time serie
                const unsigned min_time = std::max(0u, base_time - 2 * sigma);
                const unsigned max_time = base_time + 2 * sigma;
                std::vector<unsigned> time_serie(p_options.time_serie_size);
                time_serie[0] = static_cast<unsigned>(base_time);
                for (size_t i = 1; i < time_serie.size(); i++) {
                    const unsigned last = time_serie[i - 1];
                    std::uniform_int_distribution<unsigned> time_distribution(
                            std::max(min_time, last - sigma),
                            std::min(max_time, last + sigma));
                    time_serie[i] = time_distribution(p_generator);
                }
                edge->setTimes(time_serie);

                // Generation of random costs for the edge
                std::uniform_real_distribution<double> distribution(p_options.min_cost,
                                                                    p_options.max_cost);
                std::vector<double> costs(p_options.time_serie_size);
                std::generate(costs.begin(), costs.end(),
                              [&]() { return distribution(p_generator); });
                edge->setCosts(costs);
            }
        }
    }

    Options p_options;
    std::default_random_engine p_generator;
};

class CoordGraph : public Graph<std::tuple<double, double, std::string>, double>{
public:

    std::string getColor(uint64_t node_id) const{
        auto[x, y, c] = getNodeData(node_id);
        return c;
    }

    void setColor(uint64_t node_id, std::string const& color){
        auto[x, y, _] = getNodeData(node_id);
        auto new_data = std::make_tuple(x, y, color);
        setNodeData(node_id, new_data);
    }

    std::pair<double, double> getCoordinate(uint64_t node_id) const{
        auto [x, y, _] = getNodeData(node_id);
        return std::pair<double, double>(x, y);
    }

    CoordGraph operator+(CoordGraph const& other) const{
        CoordGraph g;
        uint64_t k = 1;
        std::map<uint64_t, uint64_t> old_to_new;
        for (int i = 0; i < this->size(); ++i) {
            uint64_t node = this->p_nodes[i]->get_id();
            g.addNode(k, this->getNodeData(node));
            old_to_new[node] = k;
            k++;
        }
        for (int i = 0; i < this->size(); ++i) {
            uint64_t node = this->p_nodes[i]->get_id();
            for (uint64_t const& succ : this->getSuccessors(node)){
                uint64_t g_src = old_to_new[node];
                uint64_t g_dst = old_to_new[succ];
                g.addSuccessor(g_src, g_dst, this->getEdgeData(node, succ));
            }
        }

        old_to_new.clear();

        for (int i = 0; i < other.size(); ++i) {
            uint64_t node = other.p_nodes[i]->get_id();
            auto xy = g.contain_coord(other.getCoordinate(node));
            if (xy.has_value()){
                old_to_new[node] = xy.value();
            }
            else {
                g.addNode(k, other.getNodeData(node));
                old_to_new[node] = k;
                k++;
            }
        }
        for (int i = 0; i < other.size(); ++i) {
            uint64_t node = other.p_nodes[i]->get_id();
            for (uint64_t const& succ : other.getSuccessors(node)){
                uint64_t g_src = old_to_new[node];
                uint64_t g_dst = old_to_new[succ];
                g.addSuccessor(g_src, g_dst, other.getEdgeData(node, succ));
            }
        }
        return g;
    }

private:
    std::optional<uint64_t> contain_coord(std::pair<double, double> const& xy){
        for (int i = 0; i < size(); ++i) {
            uint64_t n = this->p_nodes[i]->get_id();
            if (getCoordinate(n) == xy)
                return n;
        }
        return {};
    }


};


#endif  //CUBE_GRAPH_H
