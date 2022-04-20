#ifndef CUBE_UTIL_H
#define CUBE_UTIL_H

#include <utility>
#include <vector>
#include <functional>
#include <limits>



template <typename T>

std::pair<T, double> maximum(std::vector<T> const& t, std::function<double (T)> const& f){
    T x(t[0]);
    double y = -1 * std::numeric_limits<double >::infinity();
    for(auto & z : t){
        double fz = f(z);
        if(fz > y){
            x = z;
            y = fz;
        }

    }
    return std::pair<T, double >(x, y);

}

template <typename T>

std::pair<T, double> minimum(std::vector<T> const& t, std::function<double (T)> const& f){
    T x(t[0]);
    double y = std::numeric_limits<double >::infinity();
    for(auto & z : t){
        double fz = f(z);
        if(fz < y){
            x = z;
            y = fz;
        }

    }
    return std::pair<T, double >(x, y);

}

template <typename T>

std::pair<T, double > optimum(std::vector<T> const& t, std::function<double (T)> const& f, bool isMaximizing = true){
    if(isMaximizing){
        return maximum(t, f);
    }
    else{
        return minimum(t, f);
    }

}

template <typename T>
bool isBetter(T const& x, T const& y, bool isMaximizing = true){
    if(isMaximizing){
        return x > y;
    }
    else{
        return x < y;
    }
}

#endif //CUBE_UTIL_H
