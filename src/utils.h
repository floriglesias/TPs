#ifndef UTILS_H
#define UTILS_H

#include <iostream>
#include <vector>
#include <list>



template<class T>
bool pertenece(T xs, std::vector<T> v) {
    for (int i = 0; i < v.size(); ++i) {
        if (xs == v[i]) {
            return true;
        }
    }
    return false;
}

template<class T>
bool subset(std::vector<T> v1, std::vector<T> v2) {
    for (int i = 0; i < v1.size(); ++i) {
        if (not pertenece(v1[i], v2)) {
            return false;
        }
    }
    return true;
}

template<class T>
bool seteq(std::vector<T> v1, std::vector<T> v2) {
    return subset(v1, v2) and subset(v2, v1);
}

template<class T>
std::ostream &operator<<(std::ostream &os, const std::list<T> &l) {
  os << "[";
  for (auto e : l) {
    os << e << ", ";
  }
  os << "]";
  return os;
}

template<class A, class B>
std::ostream &operator<<(std::ostream &os, const std::pair<A, B> &p) {
  os << "(" << p.first << ", " << p.second << ")";
  return os;
}

#endif //UTILS_H
