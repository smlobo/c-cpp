//
// Created by Sheldon Lobo on 3/17/24.
//

#ifndef MAP_ROUTING_NODE_H
#define MAP_ROUTING_NODE_H

class Node {
private:
    int id;
    int x;
    int y;
    friend class Graph;

public:
    Node(int id, int x, int y) :
        id(id), x(x), y(y) {};
    friend std::ostream& operator<<(std::ostream&, const Node&);
};

#endif //MAP_ROUTING_NODE_H
