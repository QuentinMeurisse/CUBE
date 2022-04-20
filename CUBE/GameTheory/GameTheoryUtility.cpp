#include "GameTheoryUtility.h"
#include "Strategies/ChangeColorStrat.h"
#include "Strategies/ChangeBlockColorStrat.h"
#include "RadiusConstraint.h"
#include "TwoRadiusConstraint.h"

#include <cmath>
#include <map>
#include <deque>
#include <algorithm>
#include <random>
#include <chrono>
#include <string>
#include <unordered_set>
#include <iostream>

using Mesh = OpenMesh::PolyMesh_ArrayKernelT<>;
using namespace std;
using namespace std::chrono;

double GameTheoryUtility::norm(Mesh::Point const &p) {
    double sum = p[0] * p[0] + p[1] * p[1] + p[2] * p[2];
    return sqrt(sum);
}

deque<unsigned int>
GameTheoryUtility::faceBall(shared_ptr<Mesh> const& mesh, const unsigned int &fid, const unsigned int &radius) {
    deque<unsigned int > toReturn;
    map<int, int > dist;
    deque<unsigned int > q;
    q.push_back(fid);
    dist[fid] = 0;

    while (!q.empty()){
        int current = q.front();
        q.pop_front();
        toReturn.push_back(current);

        if (dist[current] < radius){
            for (Mesh::FaceVertexIter v = mesh->fv_iter(mesh->face_handle(current)); v.is_valid(); ++v){
                for (Mesh::VertexFaceIter f = mesh->vf_iter(*v); f.is_valid(); ++f){
                    if (dist.count(f->idx()) == 0){
                        dist[f->idx()] = dist[current] + 1;
                    }
                    if (find(toReturn.begin(), toReturn.end(), f->idx()) == toReturn.end() &&
                            find(q.begin(), q.end(), f->idx()) == q.end()){
                        q.push_back(f->idx());
                    }
                }
            }
        }
    }
    toReturn.pop_front(); // The first element in toReturn is fid and we don't want it
    return toReturn;
}

std::map<unsigned int, std::deque<unsigned int>>
GameTheoryUtility::faceDistances(shared_ptr<Mesh> const &mesh, const unsigned int &fid, const unsigned int &radius) {
    map<unsigned int, deque<unsigned int >> toReturn;
    map<unsigned int, unsigned int > dist;
    deque<unsigned int > q;
    vector<unsigned int> visited;
    q.push_back(fid);
    dist[fid] = 0;

    while (not q.empty()){
        unsigned int current = q.front();
        q.pop_front();
        toReturn[dist[current]].push_back(current);
        visited.push_back(current);

        if (dist[current] < radius){
            for (Mesh::FVIter v = mesh->fv_iter(mesh->face_handle(current)); v.is_valid(); ++v) {
                for (Mesh::VFIter f = mesh->vf_iter(*v); f.is_valid(); ++f) {
                    if (dist.count(f->idx()) == 0) {
                        dist[f->idx()] = dist[current] + 1;
                    }
                    if (find(visited.begin(), visited.end(), f->idx()) == visited.end() and
                            find(q.begin(), q.end(), f->idx()) == q.end()){
                        q.push_back(f->idx());
                    }
                }
            }
        }
    }
    toReturn.erase(0);
    return toReturn;

}

void GameTheoryUtility::generateRandomGame(const shared_ptr<UrbanBlockGame> &ubg, unsigned int const &radius) {
    shared_ptr<Tiling> t = ubg->getT();
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> distribution(0, t->numCells() - 1);
    RadiusConstraint constraint(radius);
    auto start = high_resolution_clock::now();
    while (duration_cast<milliseconds>(high_resolution_clock::now() - start).count() <= 500){
        int k = distribution(gen);
        if (t->getColor(k) != "#ffffff"){
            ChangeBlockColorStrat strat(k,  "#ffffff");
            strat.select(ubg);
        }
        else {
            shared_ptr<ChangeColorStrat> s = make_shared<ChangeColorStrat>(k, "#f00000");
            if (constraint.validChangeColor(ubg, s))
                s->select(ubg);
        }
    }
}

void GameTheoryUtility::generateRandomGameWithGreen(const std::shared_ptr<UrbanBlockGame> &ubg, const unsigned int &rb,
                                                    const unsigned int &rg) {

    shared_ptr<Tiling> t = ubg->getT();
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> distribution(0, t->numCells() - 1);
    uniform_int_distribution<> zeroOrOne(0, 1);
    TwoRadiusConstraint constraint(rb, rg);
    auto start = high_resolution_clock::now();
    while (duration_cast<milliseconds>(high_resolution_clock::now() - start).count() <= 500){
        int k = distribution(gen);
        if (t->getColor(k) != "#ffffff"){
            ChangeBlockColorStrat strat(k,  "#ffffff");
            strat.select(ubg);
        }
        else {
            int colorNum = zeroOrOne(gen);
            string color = colorNum == 0 ? "#f00000" : "#00f000";
            shared_ptr<ChangeColorStrat> s = make_shared<ChangeColorStrat>(k, color);
            if (constraint.validChangeColor(ubg, s))
                s->select(ubg);
        }
    }

}

std::deque<unsigned int> GameTheoryUtility::qIntersection(const deque<unsigned int> &q1, const deque<unsigned int> &q2) {
    if (q1.empty() or q2.empty())
        return deque<unsigned int>();

    unordered_set<unsigned int> s{q1.begin(), q1.end()};
    deque<unsigned int> intersection;
    for (unsigned int x : q2){
        if (s.erase(x) > 0)
            intersection.push_back(x);
    }
    return intersection;
}
