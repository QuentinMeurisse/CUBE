#ifndef CUBE_GRAPHUTILITY_H
#define CUBE_GRAPHUTILITY_H

#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <nlohmann/json.hpp>
#include <set>
#include <deque>
#include <random>
#include <memory>
#include "GraphGame.h"
#include "Invariants/ColorNodeInvariant.h"

class GraphUtility {

public:
    static void randomColor(const std::shared_ptr<GraphGame> &gg, const std::string &color, char key){
        std::shared_ptr<CoordGraph> g = gg->getG();
        std::map<char, std::shared_ptr<Invariant>> inv  = gg->getInvariants();
        std::shared_ptr<ColorNodeInvariant> cni = std::dynamic_pointer_cast<ColorNodeInvariant>(inv[key]);

        uint64_t node_id;
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<size_t> dis(0, (g->size())-1);
        do {
            size_t n = dis(gen);
            node_id = (*g)[n];
        }while (g->getColor(node_id) != "#ffffff");
        g->setColor(node_id, color);
        cni->addNode(node_id);
    }

    template<typename T> static void writeMatrix(const std::vector<std::vector<T>>& m, const
    std::string& path){
        std::ofstream writer;
        writer.open(path, std::ios::out);
        for (const std::vector<T>& row : m){
            for (int i = 0; i < row.size(); ++i) {
                if (i != 0)
                    writer << " ";
                writer << row[i] ;
            }
            writer << std::endl;
        }
        writer.close();
    }

    template <typename T> static std::vector<std::vector<T>> loadMatrix(std::string const& path){
        std::vector<std::vector<T>> to_return;
        std::ifstream reader(path);
        if (not reader.is_open()) {
            throw std::invalid_argument("Can not open the file " + path);
        }
        std::string line;
        while (getline(reader, line)){
            std::stringstream ss(line);
            std::string s;
            std::vector<T> row;
            while (getline(ss, s, ' ')){
                std::stringstream value(s);
                T x;
                value >> x;
                row.push_back(x);
            }
            to_return.push_back(row);
        }
        reader.close();
        return to_return;
    }

    /**
     *
     * @param w weight of the items to put in the knapsack
     * @param p profit of the items to put in the knapsack
     * @param c size of the knapsack
     * @param n number of the item
     * @return the objects to put in the knapsack to maximize the profit
     */
    template<typename P = int>static std::set<int> knapsack(const std::vector<int> &w,
                                  const std::vector<P> &p,
                                  int c,
                                  int n){
        std::vector<std::vector<P>> z(c+1, std::vector<P>(n+1));
        //Matrix containing the payoffs progressively computed

        for (int i = 1; i <= n; ++i) {
            // For each item we work with a progressively larger knapsack
            for (int d = 0; d <= w[i-1] - 1; ++d) {
                // If the knapsack is not enough large we cannot select the item i. The payoff doesn't change.
                z[d][i] = z[d][i - 1];
            }
            for (int d = w[i-1]; d <= c ; ++d){
                /* We are looking at the knapsack of size d - wi. We check if adding the item i to the selected item for
                 * the smaller knapsack increase the payoff
                */
                if (z[d - w[i-1]][i-1] + p[i-1] > z[d][i-1]) {
                    z[d][i] = z[d - w[i-1]][i-1] + p[i-1];
                }
                else {
                    z[d][i] = z[d][i - 1];
                }
            }
        }

        // Construction of the solution
        std::set<int> x;
        int d = c; // the current size of the knapsack
        for (int i = n; i >= 1; --i) {
            if (d >= w[i-1] and z[d][i] == z[d - w[i-1]][i-1] + p[i-1]){
                // If item i is selected, we just add it in the knapsack of size d - wi
                x.insert(i-1);
                d -= w[i-1];
            }
        }
        return x;
    };
    /**
     *
     * @param w matrix such as w[i][j] is the volume of the item j in the bin i
     * @param p matrix such as p[i][j] is the profit to put the item j in the bin i
     * @param binc maximal capcity of each bin
     * @param items number of items
     * @param bin number of bins
     * @return an assignment of items in the bin that optimize the gap problem
     */
    template<typename P = int> static std::deque<std::set<int>> gap (const std::vector<std::vector<int>> &w,
                                        const std::vector<std::vector<P>> &p,
                                        const std::vector<int> &binc, int items, int bin){
        return gap<P>(w, p, binc, items, bin, 0);
    }

    template<typename T> static void displayMatrix(std::vector<std::vector<T>> m){
        for (const std::vector<T>& row : m){
            for (int i = 0; i < row.size(); ++i) {
                if (i != 0)
                    std::cout << " ";
                std::cout << row[i];
            }
            std::cout << std::endl;
        }
    }

    template<typename T> static void writeVectorToJson(const std::string &path,
                                                const std::string &key,
                                                const std::vector<T> &v,
                                                bool overwrite = false){
        nlohmann::json j;
        if (not overwrite){
            std::ifstream reader(path);
            reader >> j;
            reader.close();
        }
        j[key] = v;
        std::ofstream writer;
        writer.open(path, std::ios::out);
        writer << j.dump(4);
        writer.close();
    }

    template<typename T> static std::vector<T>loadVectorFromJson(const std::string& path,
                                                          const std::string& key){
        std::ifstream reader(path);
        if (not reader.is_open()) {
            throw std::invalid_argument("Can not open the file " + path);
        }
        nlohmann::json j;
        reader >> j;
        std::vector<T> to_return = j[key];
        return to_return;
    }

    /**
    *
    * @param distribution distribution of the category (in decreasing order)
    * @param n number of elements to be associated with a category
    * @return a vector a n element respecting the distribution
    */
    static std::vector<int> generateCategoriesAttribution(const std::vector<double> &distribution, int const& n){
        std::vector<int> categories;
        size_t m = distribution.size() - 1;
        int total = 0;
        for (size_t i = m; i >= 1; i--) {
            int num = (int)(round(n * distribution[i]));
            total += num;
            for (int j = 0; j < num; ++j) {
                categories.push_back(i + 1);
            }
        }
        for (int j = 0; j < (n - total); ++j) {
            categories.push_back(1);
        }
        std::random_device rd;
        std::mt19937 gen(rd());
        shuffle(categories.begin(), categories.end(), gen);
        return categories;
    }

    /**
     *
     * @param distribution distribution of the categories (in increasing order).
     * We assume that the sum is less than 1.
     * @param n number of elements to be associated with a category
     * @return a vector a n element respecting the distribution. The value are from
     * distribution.size() to 1 for element cover the distribution. Else the value is 0
     */
    static std::vector<int> generateWeightBasedOnPartialDistribution(const std::vector<double> &distribution,
                                                                    const int &n){
        std::vector<int> categories;
        size_t m = distribution.size() - 1;
        int total = 0;
        for (size_t i = 0; i <=m ; i++) {
            int num = (int)(round(n * distribution[i]));
            total += num;
            for (int j = 0; j < num; ++j) {
                categories.push_back(i + 1);
            }
        }
        for (int j = 0; j < (n - total); ++j) {
            categories.push_back(0);
        }
        std::random_device rd;
        std::mt19937 gen(rd());
        shuffle(categories.begin(), categories.end(), gen);
        return categories;
    }

    /**
     *
     * @param maxDistance all the targets at less maxDistance from a source have the minimum profit
     * @param numCategories number of division of maxDistance to have categories of profit
     * (counting the category with distance bigger than maxDistance )
     * @param base_values matrix of distances
     * @return a matrix containing the profit of each source to go the each target
     */
    static std::vector<std::vector<int>> categorizePreferences(int maxDistance,
                                                               int numCategories,
                                                               const std::vector<std::vector<double>>
                                                               &base_values){
        int category_size  = maxDistance / (numCategories - 1);
        std::vector<std::vector<int>> to_return = std::vector<std::vector<int>>(base_values.size(),
                                                                                std::vector<int>(base_values[0].size()));
        for (int i = 0; i < base_values.size(); ++i) {
            for (int j = 0; j < base_values[0].size(); ++j) {
                double x = base_values[i][j];
                if (x < maxDistance) {
                    double y = fmod(x, category_size);
                    int z = (int) (x - y);
                    int value = numCategories - (z / category_size);
                    to_return[i][j] = value;
                }
                else
                    to_return[i][j] = 1;
            }
        }
        return to_return;
    }

    static std::deque<std::set<int>> loadGap(const std::string& path){
        std::deque<std::set<int>> to_return;

        std::ifstream reader(path);
        if (not reader.is_open()){
            throw std::invalid_argument("Cannot open the file " + path);
        }
        std::string line;
        while (getline(reader, line)){
            std::set<int> s;
            std::stringstream bin(line);
            std::string value;
            while (getline(bin, value, ' ')){
                s.insert(stoi(value));
            }
            to_return.push_back(s);
        }
        return to_return;
    }

    template<typename T = int>
    /**
     *
     * @param gap a gap solution
     * @param p matrix of costs
     * @param without a bin to not considerate during the total payoff computation
     * @return the total payoff of a gap solution
     */
    static T computeGapPayoff(const std::deque<std::set<int>>& gap,
                              const std::vector<std::vector<T>>& p,
                              const std::optional<int>& without = {}){
        T sum = 0;
        for (int i = 0; i < gap.size(); ++i) {
            if (not (without.has_value() and i == without.value())) {
                for (int x : gap[i]) {
                    sum += p[i][x];
                }
            }
        }
        return sum;
    }

    static void writeGap(const std::string& path, const std::deque<std::set<int>>& gap){
        std::ofstream writer;
        writer.open(path, std::ios::out);
        for (const auto& a : gap){
            int i(0);
            for(int x : a){
                if (i != 0)
                    writer << " ";
                writer << x;
                i++;
            }
            writer << std::endl;
        }
        writer.close();
    }

    /**
    *
    * @param numItem
    * @param numBin
    * @param maxValue the generate value are in [1, maxValue]
    * @return a matrix M such as M[i][j] is a random profit to put item j in bin i
    */
    static std::vector<std::vector<int>> generateRandomPreference(int const& numItem,
                                                                  int const& numBin,
                                                                  int const& maxValue){
        std::vector<std::vector<int>> toReturn = std::vector(numBin, std::vector(numItem, 0));
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> distrib(1, maxValue);
        for (int j = 0; j < numBin; ++j) {
            for (int i = 0; i < numItem; ++i) {
                toReturn[j][i] = distrib(gen);
            }
        }
        return toReturn;
    }

private:
    /**
     * Internal method call by the public method gap
     * @param w
     * @param p
     * @param binc
     * @param items
     * @param bin
     * @param j number of the bin  we are working with
     * @return
     */
    template<typename P>static std::deque<std::set<int>> gap (const std::vector<std::vector<int>> &w,
                                        const std::vector<std::vector<P>> &p,
                                        const std::vector<int> &binc,
                                        int items,
                                        int bin,
                                        int j) {
        std::set<int> s = knapsack<P>(w[j], p[j], binc[j], items);
        std::vector<std::vector<P>> p1(bin, std::vector<P>(items));
        std::vector<std::vector<P>> p2(bin, std::vector<P>(items));
        for (unsigned int i = 0; i < items; i++) {
            for (unsigned int k = 0; k < bin; k++) {
                p1[k][i] = s.find(i) != s.end() or k == j ? p[j][i] : 0;
                p2[k][i] = p[k][i] - p1[k][i];
            }
        }
        if (j < bin - 1) {
            std::deque<std::set<int>> s_rec = gap<P>(w, p2, binc, items, bin, j + 1);
            std::set<int> binj{};
            for (int t: s) {
                auto iterBin = s_rec.begin();
                bool found = false;
                while (iterBin != s_rec.end() and not found) {
                    if ((*iterBin).find(t) != (*iterBin).end()) {
                        found = true;
                    }
                    ++iterBin;
                }
                if (not found)
                    binj.insert(t);
            }
            s_rec.push_front(binj);
            return s_rec;
        } else {
            std::deque<std::set<int>> sj{};
            sj.push_front(s);
            return sj;
        }
    }
};


#endif //CUBE_GRAPHUTILITY_H
