#ifndef CUBE_SEGMENT_H
#define CUBE_SEGMENT_H

#include <ostream>
#include <vector>
#include <set>

class Segment {
public:
    /**
     *
     * @param x1 x-coordinate of the alpha extremity
     * @param y1 y-coordinate of the alpha extremity
     * @param x2 x-coordinate of the beta extremity
     * @param y2 y-coordinate of the beta extremity
     */
    Segment(double x1, double y1, double x2, double y2);

    Segment(const std::pair<double, double> &alpha, const std::pair<double, double> &beta);

    Segment();

    [[nodiscard]] bool is_horizontal() const;

    [[nodiscard]] bool is_vertical() const;

    [[nodiscard]] const std::pair<double, double> &getAlpha() const;

    [[nodiscard]] const std::pair<double, double> &getBeta() const;

    void setBeta(const std::pair<double, double> &b);

    /**
     * The first point of the segment is suppose to be fixed. So we can only move the second
     * @param dx
     * @param dy
     */
    void translate (double dx, double dy);

    [[nodiscard]] bool is_parallel(Segment const& s) const;

    friend std::ostream &operator<<(std::ostream &os, const Segment &segment);

    bool operator==(const Segment &rhs) const;

    bool operator!=(const Segment &rhs) const;

    bool operator<(const Segment &rhs) const;

    bool operator>(const Segment &rhs) const;

    bool operator<=(const Segment &rhs) const;

    bool operator>=(const Segment &rhs) const;

    /**
     *
     * @param s
     * @return if this is a subsegment of s
     */
    [[nodiscard]] bool is_subsegment_of (Segment const& s) const;

    [[nodiscard]]bool is_in_front_of (Segment const& s) const;

    [[nodiscard]] bool has_common_extremity(Segment const& s) const;

    [[nodiscard]] std::pair<double, double> common_extremity(Segment const& s) const;

    static std::vector<Segment> intersection_of_sets_of_segments(const std::set<Segment>& s1, const std::set<Segment>& s2);

    static double distance(Segment const& s1, Segment const& s2);

private:
    std::pair<double, double> alpha, beta;

    [[nodiscard]] bool is_in_xrange(std::pair<double, double> p) const;

    [[nodiscard]] bool is_in_yrange(std::pair<double, double> p) const;
};

struct SegmentYorder{
    static bool pair_strict_yorder(std::pair<double, double> p1, std::pair<double, double> p2);

    bool operator()(Segment const& s1, Segment const& s2);
};


#endif //CUBE_SEGMENT_H
