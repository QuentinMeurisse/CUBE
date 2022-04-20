#ifndef CUBE_GRAPHUTILITY_H
#define CUBE_GRAPHUTILITY_H

#include <memory>
#include <deque>
#include <ios>
#include <fstream>
#include <QJsonObject>
#include <QJsonArray>
#include <QFile>
#include <QJsonDocument>
#include <optional>
#include <set>
#include "GraphGame.h"

class GraphUtility {

public:
    static void randomColor(const std::shared_ptr<GraphGame> &gg, const std::string &color, char key);

    template<typename  T> static void writeMatrix(const std::vector<std::vector<T>>& m, const std::string& path);

    static std::vector<std::vector<double >> loadMatrix(std::string const& path);

    /**
     *
     * @param w weight of the items to put in the knapsack
     * @param p profit of the items to put in the knapsack
     * @param c size of the knapsack
     * @param n number of the item
     * @return the objects to put in the knapsack to maximize the profit
     */
    static std::set<int> knapsack(const std::vector<int> &w,
                                  const std::vector<int> &p,
                                  int c,
                                  int n);
    /**
     *
     * @param w matrix such as w[i][j] is the volume of the item j in the bin i
     * @param p matrix such as p[i][j] is the profit to put the item j in the bin i
     * @param binc maximal capcity of each bin
     * @param items number of items
     * @param bin number of bins
     * @return an assignment of items in the bin that optimize the gap problem
     */
    static std::deque<std::set<int>> gap (const std::vector<std::vector<int>> &w,
                                        const std::vector<std::vector<int>> &p,
                                        const std::vector<int> &binc,
                                        int items,
                                        int bin);

    static void displayMatrix(const std::vector<std::vector<int>> &v);

    static std::vector<std::vector<int>> loadMatrixFromJson(const std::string& path, const std::string& key);

    static void writeMatrixToJson(const std::string& path, const std::string& key, const std::vector<std::vector<int>>& v);

    template<typename T> static void writeVectorToJson(const std::string& path, const std::string& key, const std::vector<T>& v);

    static std::vector<int> loadVectorFromJson(const std::string& path, const std::string& key);

    /**
    *
    * @param distribution distribution of the category (in decreasing order)
    * @param n number of elements to be associated with a category
    * @return a vector a n element respecting the distribution
    */
    static std::vector<int> generateCategoriesAttribution(const std::vector<double> &distribution, int const& n);

    /**
     *
     * @param maxDistance all the targets at less maxDistance from a source have the minimum profit
     * @param numCategories number of division of maxDistance to have categories of profit (without counting the catgory
     * with distance bigger than maxDistance )
     * @param dist matrix of distances
     * @return a matrix containing the profit of each source to go the each target
     */
    static std::vector<std::vector<int>>
    getPreferencesBasedOnDistance(int maxDistance, int numCategories, const std::vector<std::vector<double>> &dist);

    static std::deque<std::set<int>> loadGap(const std::string& path);

    static int computeGapPayoff(const std::deque<std::set<int>>& gap, std::vector<std::vector<int>> p, const std::optional<int>& without = {});

    static void writeGap(const std::string& path, const std::deque<std::set<int>>& gap);

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
    static std::deque<std::set<int>> gap (const std::vector<std::vector<int>> &w,
                                        const std::vector<std::vector<int>> &p,
                                        const std::vector<int> &binc,
                                        int items,
                                        int bin,
                                        int j);
};

template<typename  T> void GraphUtility::writeMatrix(const std::vector<std::vector<T >>& m, const std::string& path) {
    std::ofstream writer;
    writer.open(path, std::ios::out);
    for (const auto& row : m){
        for(T x : row)
            writer << x << " ";
        writer << "\n";
    }
    writer.close();
}

template<typename  T> void GraphUtility::writeVectorToJson(const std::string &path, const std::string &key, const std::vector<T> &v) {
    QJsonArray array;
    for (T x : v){
        QString value = QString::number(x);
        array.push_back(value);
    }
    QJsonObject json;
    QFile file(QString::fromStdString(path));

    if (file.open(QIODevice::ReadOnly)){
        QByteArray data = file.readAll();
        file.close();
        QJsonDocument loadDoc(QJsonDocument::fromJson(data));
        json = loadDoc.object();
    }
    json[QString::fromStdString(key)] = array;

    file.open(QIODevice::WriteOnly);
    file.write(QJsonDocument(json).toJson());
    file.close();
}

#endif //CUBE_GRAPHUTILITY_H
