//
// Created by Sheldon Lobo on 3/25/24.
//

#include <cstdlib>
#include <cmath>

#include "Edge.h"

Edge::Edge() :
    src(nullptr), dest(nullptr), distance(-1.0) {}

Edge::Edge(Node *s, Node *d) :
    src(s), dest(d) {
    distance = std::sqrt(std::pow(std::abs(src->x - dest->x), 2) +
            std::pow(std::abs(src->y - dest->y), 2));
}

std::ostream &operator<<(std::ostream &strm, const Edge &e) {
    strm << e.src << " -> " << e.dest << " [" << e.distance << "]";
    return strm;
}
