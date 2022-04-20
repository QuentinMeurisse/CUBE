#include "Segment.h"

#include <iostream>
#include <algorithm>
#include <limits>

using namespace std;
Segment::Segment(double x1, double y1, double x2, double y2){
    // The extremities are ordered following the lexicographical order
    pair<double, double> a(x1, y1);
    pair<double, double> b(x2, y2);
    this->alpha = a < b ? a : b;
    this->beta = a < b ? b : a;

}

Segment::Segment(const pair<double, double> &alpha, const pair<double, double> &beta){
    this->alpha = alpha < beta ? alpha : beta;
    this->beta = alpha < beta ? beta : alpha;
}

Segment::Segment() = default;


bool Segment::is_horizontal() const {
    return abs(alpha.second - beta.second) < 1e-10 and abs(alpha.first - beta.first) >= 1e-10;
}

bool Segment::is_vertical() const {
    return abs(alpha.second - beta.second) >= 1e-10 and abs(alpha.first - beta.first) < 1e-10;
}

const pair<double, double> &Segment::getAlpha() const {
    return alpha;
}

const pair<double, double> &Segment::getBeta() const {
    return beta;
}

void Segment::translate(double dx, double dy) {
    beta.first += dx;
    beta.second += dy;
}

ostream &operator<<(ostream &os, const Segment &segment) {
    os << "(" << segment.alpha.first << ", " << segment.alpha.second << ") ----- ("
    << segment.beta.first << ", " <<segment.beta.second << ")";
    return os;
}

bool Segment::operator==(const Segment &rhs) const {
    return alpha == rhs.alpha and beta == rhs.beta;
}

bool Segment::operator!=(const Segment &rhs) const {
    return not (rhs == *this);
}

bool Segment::operator<(const Segment &rhs) const {
    if (alpha == rhs.alpha)
        return beta < rhs.beta;
    else
        return alpha < rhs.alpha;
}

bool Segment::operator>(const Segment &rhs) const {
    return rhs < *this;
}

bool Segment::operator<=(const Segment &rhs) const {
    return not (rhs < *this);
}

bool Segment::operator>=(const Segment &rhs) const {
    return not (*this < rhs);
}

std::vector<Segment> Segment::intersection_of_sets_of_segments(const std::set<Segment>& s1, const std::set<Segment>& s2) {
    vector<Segment> intersection;
    set_intersection(s1.begin(), s1.end(), s2.begin(), s2.end(), back_inserter(intersection));
    return intersection;
}

bool Segment::is_subsegment_of(const Segment &s) const {
    if (s.is_horizontal()){
        bool x_condition = (s.alpha.first - 1e-10) <= this->alpha.first and this->beta.first <= (s.beta.first + 1e-10);
        bool y_condition = abs(this->alpha.second - s.alpha.second) < 1e-10;
        return x_condition and y_condition;
    } else if(s.is_vertical()){
        bool x_condition = abs(this->alpha.first - s.alpha.first) < 1e-10;
        bool y_condition = (s.alpha.second - 1e-10) <= this->alpha.second and this->beta.second <= (s.beta.second + 1e-10);
        return x_condition and y_condition;
    } else {
        double m = (s.beta.second - s.alpha.second) / (s.beta.first - s.alpha.first);
        double p = s.alpha.second / (m * s.alpha.first);

        auto d = [&](double const &x) {
            return m * x + p;
        };

        bool in_straight_line = abs(this->alpha.second - d(this->alpha.first)) <= 1e-10
                                and abs(this->beta.second - d(this->beta.first)) <= 1e-10;
        bool in_segment = (s.alpha.first - 1e-10) <= this->alpha.first and this->beta.first <= (s.beta.first + 1e-10);
        return in_straight_line and in_segment;
    }
}

void Segment::setBeta(const pair<double, double> &b) {
    Segment::beta = b;
}

bool Segment::is_parallel(const Segment &s) const {
    if(this->is_vertical() and s.is_vertical())
        return true;
    else if (this->is_horizontal() and s.is_horizontal())
        return true;
    else{
        double m1 = (beta.second - alpha.second) / (beta.first - alpha.first);
        double m2 = (s.beta.second - s.alpha.second) / (s.beta.first - s.alpha.first);
        return abs(m1 - m2) < 1e-10;
    }
}

bool Segment::is_in_front_of(const Segment &s) const {
    if (this->is_parallel(s)){
        if (this->is_vertical()){
            return this->is_in_yrange(s.alpha) or this->is_in_yrange(s.beta)
            or s.is_in_yrange(this->alpha) or s.is_in_yrange(this->beta);
        } else{
            return this->is_in_xrange(s.alpha) or this->is_in_xrange(s.beta)
                   or s.is_in_xrange(this->alpha) or s.is_in_xrange(this->beta);
        }
    } else
        return false;
}

double Segment::distance(const Segment &s1, const Segment &s2) {
    if (s1.is_in_front_of(s2)){
        if (s1.is_vertical())
            return abs(s1.alpha.first - s2.alpha.first);
        else if (s1.is_horizontal())
            return abs(s1.alpha.second - s2.alpha.second);
        else
            return numeric_limits<double>::infinity();
    } else
        return numeric_limits<double>::infinity();
}

bool Segment::is_in_xrange(std::pair<double, double> p) const {
    return alpha.first <= p.first and beta.first >= p.first;
}

bool Segment::is_in_yrange(std::pair<double, double> p) const {
    return alpha.second <=p.second and beta.second >= p.second;
}

bool Segment::has_common_extremity(const Segment &s) const {
    return this->alpha == s.alpha or this->alpha == s.beta or this->beta == s.alpha or this->beta == s.beta;
}

std::pair<double, double> Segment::common_extremity(const Segment &s) const {
    if (not this->has_common_extremity(s))
        throw invalid_argument("These two segment don't have a common intersection");
    else if (this->alpha == s.alpha or this->alpha == s.beta)
        return alpha;
    else
        return beta;
}


bool SegmentYorder::pair_strict_yorder(std::pair<double, double> p1, std::pair<double, double> p2) {
    if (p1.second == p2.second)
        return p1.first < p2.first;
    else
        return p1.second < p2.second;
}

bool SegmentYorder::operator()(const Segment &s1, const Segment &s2) {
    if (s1.getAlpha() == s2.getAlpha())
        return not pair_strict_yorder(s2.getBeta(), s1.getBeta());
    else
        return not pair_strict_yorder(s2.getAlpha(), s1.getAlpha());
}
