#include <iostream>
#include <nlohmann/json.hpp>
#include <fstream>
#include <chrono>


#include "graph.h"
#include "GeneticFLP.h"
#include "GeneticDisplay.h"

using namespace std;
typedef chrono::high_resolution_clock Clock;

int main(int argc, char *argv[]){
    CoordGraph g;
    g.loadJson("web50x50.json", "weight", "data");

    vector<double> facility_cost(g.size(), 250);
    /*vector<vector<double>> transport_cost;
    for (int i = 0; i < g.size(); ++i) {
        auto v = g.dijkstra(g[i], ProblemType::shortest_path);
        transport_cost.push_back(v);
    }

    nlohmann::json json;
    json["spp"] = transport_cost;
    ofstream writer;
    writer.open("web50x50_spp.json", ios::out);
    writer << json.dump(4);
    writer.close();*/

    nlohmann::json json;
    ifstream reader("web50x50_spp.json");
    reader >> json;
    reader.close();
    vector<vector<double>> transport_cost = json["spp"].get<vector<vector<double>>>();

    GeneticFLP gflp(2000, 1000, int(g.size()), 0.1, facility_cost, transport_cost);
    shared_ptr<GeneticDisplay> gd = make_shared<GeneticDisplay>();
    gflp.add_listener(gd);
    auto start_time = Clock::now();
    gflp.genetic_search();
    auto best = gflp.getBestGene();
    for (int i = 0; i < g.size(); ++i) {
        if ((*best)[i])
            g.setColor(g[i], "#ff0000");
        else
            g.setColor(g[i], "#ffffff");

    }
    auto end_time = Clock::now();
    chrono::duration<double> time = end_time - start_time;
    int seconds = int(round(time.count()));
    int minutes = seconds / 60;
    int hours = minutes / 60;

    cout << hours << " h " << (minutes % 60) << " min " << (seconds % 60) << " sec" << endl;
    g.writeJson("web50x50_solution.json");

    return 0.;
}