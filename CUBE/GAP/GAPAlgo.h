#ifndef CUBE_GAPALGO_H
#define CUBE_GAPALGO_H

#include <vector>
#include <memory>
#include <set>
#include <deque>

class GAPAlgo{
public:
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

#endif //CUBE_GAPALGO_H
