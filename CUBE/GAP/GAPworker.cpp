#include "GAPworker.h"
#include "GraphUtility.h"
#include "Graph.h"
#include "Matrix.h"
#include <utility>
#include <vector>
#include <experimental/filesystem>
#include <sstream>
using namespace std;
namespace fs = std::experimental::filesystem;
GAPWorker::GAPWorker(GAPConfig c, QObject *parent) : config(std::move(c)), QObject(parent) {

}

void GAPWorker::process() {
    Graph<string, double> g;
    if (config.times_key.empty())
        g.loadJson(config.graph_path, config.weight_key, "color");
    else
        g.loadJson(config.graph_path, config.weight_key, "color", config.times_key);

    emit infoChanged(QString::fromStdString("Load graph from " + config.graph_path));
    cout << "Number of nodes: " << g.size() << endl;
    cout << "Number of edges: " << g.numberEdges() << endl;
    vector<uint64_t> red;
    vector<uint64_t> blue;
    //separate buildings from school
    for (int i = 0; i < g.size(); ++i) {
        uint64_t node_id = g[i];
        if (g.getNodeData(node_id) == "#f00000"){
            red.push_back(node_id);
        }
        if (g.getNodeData(node_id) == "#00ffff"){
            blue.push_back(node_id);
        }
    }

    vector<double> distrib = {0.105227396267440,0.047010327957963,0.014296068128284,0.004230838920094,0.001358941837289,0.000543576734916,0.000190251857221,0.000117774959232,0.000099655734735};
    vector<int> num_people = GraphUtility::generateWeightBasedOnPartialDistribution(distrib, red.size());

    // we remove the buildings we only one or two inhabitant
    vector<uint64_t> red_filtered;
    vector<int> num_people_filtered;
    emit infoChanged(QString::fromStdString("Filter red nodes"));
    for (int i = 0; i < num_people.size(); ++i) {
        if (num_people[i] != 0){
            red_filtered.push_back(red[i]);
            num_people_filtered.push_back(num_people[i]);
        }
        else{ //nodes with weight 0 have no children. So they are useless for the rest of the
            // computation
            g.removeNode(red[i]);
        }
    }
    cout << "Number of nodes: " << g.size() << endl;
    cout << "Number of edges: " << g.numberEdges() << endl;

    string save_json_path = config.save_dir + fs::path::preferred_separator + "gap.json";
    GraphUtility::writeVectorToJson<uint64_t>(save_json_path, "red", red_filtered, false);
    GraphUtility::writeVectorToJson<uint64_t>(save_json_path, "blue", blue, false);
    GraphUtility::writeVectorToJson<int>(save_json_path, "num_people", num_people_filtered, false);

    // we create the matrix of weight (the number of kid having to go to school
    vector<vector<int>> w = vector(blue.size(), num_people_filtered);
    GraphUtility::writeVectorToJson<vector<int>>(save_json_path, "w", w, false);

    vector<vector<double>> dist;
    emit infoChanged(QString::fromStdString("Run dijkstra"));
    for (auto school : blue){
        vector<double> dist_to_school = g.dijkstraReverse(school, ProblemType::shortest_path);;
        vector<double> dist_from_red;
        for (uint64_t x : red_filtered){
            auto i = g.findNodeIndex(x);
            dist_from_red.push_back(dist_to_school[i]);
        }
        dist.push_back(dist_from_red);
    }

    vector<vector<int>> p_dist = GraphUtility::categorizePreferences(config.max_dist,
                                                                     config.num_dist_categories,
                                                                     dist);

    vector<vector<int>> p_times;
    if (not config.times_key.empty()){
        vector<vector<double>> times;
        emit infoChanged(QString::fromStdString("Run DOT"));
        vector<vector<double>> times_to_school = g.multiCoreDot(blue,
                                                                  ProblemType::fastest_paths,
                                                                  config.start_time, 6);
        emit infoChanged(QString::fromStdString("Filter DOT results"));
        for (int school = 0; school < blue.size(); school++){
            vector<double> times_from_red;
            for(uint64_t x : red_filtered){
                auto i = g.findNodeIndex(x);
                times_from_red.push_back(times_to_school[school][i]);
            }
            times.push_back(times_from_red);
        }
        p_times = GraphUtility::categorizePreferences(config.max_time, config.num_time_categories,
                                                      times);
    }

    vector<vector<int>> p_rnd;
    if (config.max_value_for_random != 0){
        emit infoChanged(QString::fromStdString("Generate random preferences"));
        p_rnd = GraphUtility::generateRandomPreference(red_filtered.size(), blue.size(),
                                                       config.max_value_for_random);
    }

    Matrix<int> m_dist(p_dist);
    Matrix<double> p = m_dist * (1. / config.num_criteria);
    if (not p_times.empty()) {
        Matrix<int> m_times(p_times);
        p = p + m_times * (1. / config.num_criteria);
    }
    if (not p_rnd.empty()){
        Matrix<int> m_rnd(p_rnd);
        p = p + m_rnd * (1. / config.num_criteria);
    }
    GraphUtility::writeVectorToJson<vector<double>>(save_json_path, "p", p.getData(), false);


    vector<int> binc = vector(blue.size(), 400);
    emit infoChanged(QString::fromStdString("Run gap"));
    auto s = GraphUtility::gap<double>(w, p.getData(), binc, red_filtered.size(), blue.size());

    string save_txt_path = config.save_dir + fs::path::preferred_separator + "gap.txt";
    GraphUtility::writeGap(save_txt_path, s);

    int payoff = GraphUtility::computeGapPayoff(s, p_dist);
    stringstream ss;
    ss << "Payoff of the solution: " << payoff << endl;

    emit infoChanged(QString::fromStdString(ss.str()));
    emit finished();
}
