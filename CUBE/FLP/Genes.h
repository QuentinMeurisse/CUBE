#ifndef CUBE_GENES_H
#define CUBE_GENES_H


#include <vector>
#include <ostream>
#include <set>
#include <memory>


class Genes {
public:
    explicit Genes(std::vector<bool> genes);

    [[nodiscard]] size_t size() const;

    /**
     *
     * @param i change genes[i] into not genes[i]
     */
    void flip(int i);

    void mutate();

    friend std::ostream &operator<<(std::ostream &os, const Genes &solution);


    [[nodiscard]] const std::vector<bool> &getGenes() const;

    [[nodiscard]] double getFitness() const;

    void setFitness(double value);

    long num0();

    long num1();

    bool operator<(const Genes &rhs) const;

    bool operator>(const Genes &rhs) const;

    bool operator<=(const Genes &rhs) const;

    bool operator>=(const Genes &rhs) const;

    bool operator[] (size_t k);

    void setForced0(const std::shared_ptr<std::set<int>> &f0);

    void setForced1(const std::shared_ptr<std::set<int>> &f1);

    /**
     *
     * @param gs1
     * @param gs2
     * @return the two child solution from a one point crossover performed on gs1 and gs2.
     */
    static std::pair<Genes, Genes> one_point_crossover(Genes const& gs1, Genes const& gs2);

private:
    std::vector<bool> genes;
    double fitness = 0.;
    std::shared_ptr<std::set<int>> forced0 = std::make_shared<std::set<int>>();
    std::shared_ptr<std::set<int>> forced1 = std::make_shared<std::set<int>>();

};


#endif //CUBE_GENES_H
