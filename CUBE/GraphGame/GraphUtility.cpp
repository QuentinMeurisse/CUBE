#include "GraphUtility.h"
#include "Invariants/ColorNodeInvariant.h"

#include <random>
#include <fstream>
#include <iostream>
#include <qdebug.h>


using namespace std;
using Matrix = std::vector<std::vector<int>>;

void GraphUtility::randomColor(const std::shared_ptr<GraphGame> &gg, const std::string &color, char key) {
    shared_ptr<CoordGraph> g = gg->getG();
    map<char, shared_ptr<Invariant>> inv  = gg->getInvariants();
    shared_ptr<ColorNodeInvariant> cni = dynamic_pointer_cast<ColorNodeInvariant>(inv[key]);

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


std::vector<std::vector<double >> GraphUtility::loadMatrix(std::string const &path) {
    vector<vector<double>> m;
    ifstream reader(path);
    string line;
    while (getline(reader, line)){
        stringstream ss(line);
        string s;
        vector<double> row;
        while (getline(ss, s, ' ')){
            double x = stod(s);
            row.push_back(x);
        }
        m.push_back(row);
    }
    reader.close();
    return m;
}

std::set<int> GraphUtility::knapsack(const vector<int> &w, const vector<int> &p, int c, int n) {


    Matrix z(c+1, vector<int>(n+1));

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
    set<int> x;
    int d = c; // the current size of the knapsack
    for (int i = n; i >= 1; --i) {
        if (d >= w[i-1] and z[d][i] == z[d - w[i-1]][i-1] + p[i-1]){
            /* If item is selected, we just add it in the knapsack of size d - wi
            */
            x.insert(i-1);
            d -= w[i-1];
        }
    }

    return x;
}

void GraphUtility::displayMatrix(const std::vector<std::vector<int>> &v) {
    for (int i = 0; i < v.size(); ++i) {
        for (int j = 0; j < v[0].size(); ++j) {
            cout << v[i][j] << " ";
        }
        cout << endl;
    }
}

std::deque<std::set<int>> GraphUtility::gap(const vector<std::vector<int>> &w, const vector<std::vector<int>> &p,
                                          const std::vector<int> &binc, int items, int bin) {
    return gap(w, p, binc, items, bin, 0);
}

std::deque<std::set<int>> GraphUtility::gap(const vector<std::vector<int>> &w, const vector<std::vector<int>> &p,
                                          const std::vector<int> &binc, int items, int bin, int j) {

    set<int> s = knapsack(w[j], p[j], binc[j], items);
    Matrix p1(bin, vector<int>(items));
    Matrix p2(bin, vector<int>(items));
    for (unsigned int i = 0; i < items; i++){
        for (unsigned int k = 0; k < bin; k++){
            p1[k][i] = s.find(i) != s.end() or k == j ? p[j][i] : 0;
            p2[k][i] = p[k][i] - p1[k][i];
        }
    }
    if (j < bin-1){
        deque<set<int>> s_rec = gap(w, p2, binc, items, bin, j+1);
        set<int> binj{};
        for(int t : s){
            auto iterBin = s_rec.begin();
            bool found = false;
            while (iterBin != s_rec.end() and not found){
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
    }
    else{
        deque<set<int>> sj{};
        sj.push_front(s);
        return sj;
    }
}

std::vector<std::vector<int>>GraphUtility::loadMatrixFromJson(const std::string& path, const std::string& key) {
    QFile file(QString::fromStdString(path));
    file.open(QIODevice::ReadOnly);
    QByteArray data = file.readAll();
    file.close();
    QJsonDocument loadDoc(QJsonDocument::fromJson(data));
    QJsonObject json = loadDoc.object();

    QJsonArray a = json[QString::fromStdString(key)].toArray();
    vector<vector<int>> v;
    for (int i = 0; i < a.size(); ++i) {
        vector<int> w;
        w.reserve(a[0].toArray().size());
        for (int j = 0; j < a[0].toArray().size(); ++j) {
            w.push_back(a[i].toArray()[j].toInt());
        }
        v.push_back(w);
    }
    return v;
}

void GraphUtility::writeMatrixToJson(const string &path, const string &key, const std::vector<std::vector<int>>& v) {
    QJsonArray array;
    for (const auto& row : v){
        QJsonArray a;
        for (int x : row){
            a.push_back(x);
        }
        array.push_back(a);
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

std::vector<int> GraphUtility::loadVectorFromJson(const string &path, const string &key) {
    QFile file(QString::fromStdString(path));
    file.open(QIODevice::ReadOnly);
    QByteArray data = file.readAll();
    file.close();
    QJsonDocument loadDoc(QJsonDocument::fromJson(data));
    QJsonObject json = loadDoc.object();

    QJsonArray a = json[QString::fromStdString(key)].toArray();
    vector<int> v;
    for (auto x : a) {
        v.push_back(x.toString().toInt());
    }
    return v;

}

std::vector<int> GraphUtility::generateCategoriesAttribution(const vector<double> &distribution, const int &n) {
    vector<int> categories;
    int m = distribution.size();
    int total = 0;
    for (int i = m; i >= 1; i--) {
        int num = (int)(round(n * distribution[i]));
        total += num;
        for (int j = 0; j < num; ++j) {
            categories.push_back(i + 1);
        }
    }
    for (int j = 0; j < (n - total); ++j) {
        categories.push_back(1);
    }
    random_device rd;
    mt19937 gen(rd());
    shuffle(categories.begin(), categories.end(), gen);
    return categories;
}

std::vector<std::vector<int>>
GraphUtility::getPreferencesBasedOnDistance(int maxDistance, int numCategories,
                                            const std::vector<std::vector<double>> &dist) {
    int totalCategories = numCategories + 1;
    int categorySize  = maxDistance / numCategories;
    Matrix toReturn = Matrix(dist.size(), vector<int>(dist[0].size()));
    for (int i = 0; i < dist.size(); ++i) {
        for (int j = 0; j < dist[0].size(); ++j) {
            double x = dist[i][j];
            if (x < maxDistance) {
                double y = fmod(x, categorySize);
                int z = (int) (x - y);
                int value = totalCategories - (z / categorySize);
                toReturn[i][j] = value;
            }
            else
                toReturn[i][j] = 1;
        }
    }
    return toReturn;
}

std::deque<std::set<int>> GraphUtility::loadGap(const std::string& path) {
    deque<set<int>> toReturn;

    ifstream reader(path);
    if (not reader.is_open()){
        throw invalid_argument("Cannot open the file " + path);
    }
    string line;
    while (getline(reader, line)){
        set<int> s;
        stringstream bin(line);
        string value;
        while (getline(bin, value, ' ')){
            s.insert(stoi(value));
        }
        toReturn.push_back(s);
    }
    return toReturn;
}

int GraphUtility::computeGapPayoff(const std::deque<std::set<int>>& gap, std::vector<std::vector<int>> p, const optional<int>& without) {
    int sum = 0;
    for (int i = 0; i < gap.size(); ++i) {
        if (not (without.has_value() and i == without.value())) {
            for (int x : gap[i]) {
                sum += p[i][x];
            }
        }
    }
    return sum;
}

void GraphUtility::writeGap(const std::string& path, const std::deque<std::set<int>>& gap) {
    ofstream writer;
    writer.open(path, ios::out);
    for (const auto& a : gap){
        for(int x : a){
            writer << x << " ";
        }
        writer << endl;
    }
    writer.close();

}



