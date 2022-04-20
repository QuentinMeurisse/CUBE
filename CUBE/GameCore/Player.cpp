#include "Player.h"
#include "Util.h"

#include <utility>
#include <sstream>

using namespace std;

Player::Player(std::function<bool(double, double)> better) : better(move(better)) {
    random_device rd;
    seed = rd();
    gen = mt19937 (seed);
    memory = FiniteMemory<Strategy>();
}

Player::Player() {
    function<bool (double, double)> f = [&] (double x, double y){
        return isBetter<double>(x, y, isMaximizing());
    };
    better = move(f);

    random_device rd;
    seed = rd();
    gen = mt19937 (seed);
    memory = FiniteMemory<Strategy>();
}

Player::~Player() = default;


std::ostream &operator<<(std::ostream &os, const Player &player) {
    player.print(os);
    return os;
}

bool Player::applyBetter(double x, double y) {
    return better(x, y);
}

void Player::resetRNG() {
    gen = mt19937 (seed);
}

unsigned int Player::getSeed() const {
    return seed;
}

void Player::setSeed(unsigned int num) {
    Player::seed = num;
    gen = mt19937 (seed);
}

void Player::addToMemory(const std::shared_ptr<Strategy>& strat) {
    memory.insert(strat);
}

bool Player::memoryContainsTooMuch(const shared_ptr<Strategy> &strat) {
    return memory.contain_too_much(strat);
}

const FiniteMemory<Strategy> &Player::getMemory() const {
    return memory;
}

void Player::setMemory(size_t max_size, int max_duplications) {
    memory.setMaxSize(max_size);
    memory.setMaxDuplicate(max_duplications);
}


MultiStratPlayer::MultiStratPlayer(const function<bool(double, double)> &better, int maxIndex) : Player(better),
                                                                                                 max_index(maxIndex) {
    if (maxIndex < strat_index){
        stringstream ss;
        ss << "The maxIndex value (" << maxIndex <<") must be greater or equal than strat_index (" << strat_index << ")"
        << endl;
        throw invalid_argument(ss.str());
    }
}

MultiStratPlayer::MultiStratPlayer(int maxIndex) :Player(), max_index(maxIndex) {
    if (maxIndex < strat_index){
        stringstream ss;
        ss << "The maxIndex value (" << maxIndex <<") must be greater or equal than strat_index (" << strat_index << ")"
           << endl;
        throw invalid_argument(ss.str());
    }
}

int MultiStratPlayer::getStratIndex() const {
    return strat_index;
}

void MultiStratPlayer::setStratIndex(int stratIndex) {
    strat_index = stratIndex;
}

void MultiStratPlayer::increaseStratIndex() {
    strat_index++;
}

int MultiStratPlayer::getMaxIndex() const {
    return max_index;
}

