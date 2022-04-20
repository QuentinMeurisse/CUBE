#ifndef COMODGRAPH_GAP_CONFIG_H
#define COMODGRAPH_GAP_CONFIG_H

#include <string>

struct Config{
    std::string graph_path;
    std::string save_dir;
    std::string weight_key;
    int max_dist;
    int num_dist_categories;
    std::string times_key;
    int start_time;
    int max_time;
    int num_time_categories;
    int max_value_for_random;
    int num_criteria;

};

#endif //COMODGRAPH_GAP_CONFIG_H
