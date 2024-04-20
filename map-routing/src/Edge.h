//
// Created by Sheldon Lobo on 3/25/24.
//

#ifndef MAP_ROUTING_EDGE_H
#define MAP_ROUTING_EDGE_H

#include <ostream>

#include "Node.h"

class Edge {
public:
    Node *src;
    Node *dest;
    double distance;
    friend class Node;

    Edge(Node *s, Node *d);

    friend std::ostream& operator<<(std::ostream&, const Edge&);
};
#endif //MAP_ROUTING_EDGE_H
