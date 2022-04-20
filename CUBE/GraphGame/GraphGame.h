#ifndef CUBE_GRAPHGAME_H
#define CUBE_GRAPHGAME_H

#include "Game.h"
#include "Invariant.h"
#include "graph.h"

#include <vector>

class GraphGame : public Game{

public:
    explicit GraphGame(std::shared_ptr<CoordGraph> g);
    /**
     *
     * @param g
     * @param load Can we load the distances matrix from a file or not ?
     * @param path If load is true, path is the path to a file that contains distances matrix.
     *             If load is false, path is the path to a file where to write the matrix
     */
    GraphGame(std::shared_ptr<CoordGraph> g, bool load, std::string const &path);

    double distance(unsigned int n1, unsigned int n2);

    void setNodeColor(uint64_t n, const std::string& color);

    std::string getNodeColor(uint64_t n);

    void updateAll(const std::shared_ptr<Strategy> &s);

    void undoAll(const std::shared_ptr<Strategy> &s);

    void addInvariant(std::shared_ptr<Invariant> inv, char const& key);

    void removeInvariant(char const& key);

    [[nodiscard]] const std::map<char, std::shared_ptr<Invariant>> &getInvariants() const;

    size_t numNodes();

    [[nodiscard]] const std::shared_ptr<CoordGraph> & getG() const;


private:
    std::shared_ptr<CoordGraph> g;
    std::vector<std::vector<double>> sp;
    std::map<char, std::shared_ptr<Invariant>> invariants;

};


#endif //CUBE_GRAPHGAME_H
