#include "Genes.h"

#include <utility>
#include <stdexcept>
#include <sstream>
#include <random>
#include <algorithm>
#include <iostream>
#include <set>

using namespace std;
Genes::Genes(std::vector<bool> genes) : genes(std::move(genes)) {}

size_t Genes::size() const{
    return genes.size();
}

void Genes::flip(int i) {
    try {
        genes[i] = not genes[i];
    } catch (out_of_range const& oor) {
        stringstream ss;
        ss << "This solution has " << size() << " chromosomes. We cannot access to chromosome " << i << endl;
        throw out_of_range(ss.str());
    }
}

ostream &operator<<(ostream &os, const Genes &solution) {
    for (auto const& g : solution.genes)
        os << g;
    return os;
}

const vector<bool> &Genes::getGenes() const {
    return genes;
}

bool Genes::operator<(const Genes &rhs) const {
    return fitness < rhs.fitness;
}

bool Genes::operator>(const Genes &rhs) const {
    return rhs < *this;
}

bool Genes::operator<=(const Genes &rhs) const {
    return !(rhs < *this);
}

bool Genes::operator>=(const Genes &rhs) const {
    return !(*this < rhs);
}

double Genes::getFitness() const {
    return fitness;
}

void Genes::setFitness(double value) {
    Genes::fitness = value;
}

void Genes::mutate() {
    random_device rd;
    mt19937 gen(rd());
    int n = int(genes.size());
    uniform_int_distribution<> dist(0, n-1);
    int i;
    do{
        i = dist(gen);
    } while (forced0->find(i) != forced0->end() or forced1->find(i) != forced1->end());
    flip(i);
}

bool Genes::operator[](size_t k) {
    return genes[k];
}

pair<Genes, Genes>
Genes::one_point_crossover(const Genes &gs1, const Genes &gs2) {
    if (gs1.size() != gs2.size())
        throw invalid_argument("A crossover must be perform on two gens of same size.");

    random_device rd;
    mt19937 gen(rd());
    int n = int(gs1.size()) ;
    uniform_int_distribution<> dist(0, n);
    int i = dist(gen);
    if (i == 0 or i == n)
        return make_pair(gs1, gs2);
    else{
        vector<bool> child1;
        child1.insert(child1.end(), gs1.genes.begin(), gs1.genes.begin() + i);
        child1.insert(child1.end(), gs2.genes.begin() + i, gs2.genes.end());
        vector<bool> child2;
        child2.insert(child2.end(), gs2.genes.begin(), gs2.genes.begin() + i);
        child2.insert(child2.end(), gs1.genes.begin() + i, gs1.genes.end());
        Genes g1(child1);
        Genes g2(child2);
        g1.forced0 = gs1.forced0;
        g1.forced1 = gs1.forced1;
        g2.forced0 =gs1.forced0;
        g2.forced1 = gs1.forced1;
        return make_pair(g1, g2);
    }
}

long Genes::num0() {
    return count(genes.begin(),  genes.end(), false);
}

long Genes::num1() {
    return count(genes.begin(),  genes.end(), true);
}

void Genes::setForced0(const shared_ptr<std::set<int>> &f0) {
    Genes::forced0 = f0;
}

void Genes::setForced1(const shared_ptr<std::set<int>> &f1) {
    Genes::forced1 = f1;
}




