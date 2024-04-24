//
// Created by Sheldon Lobo on 4/19/24.
//

#ifndef MAP_ROUTING_DIJKSTRASP_H
#define MAP_ROUTING_DIJKSTRASP_H

#include <vector>
#include <queue>
#include <list>
#include "Edge.h"
#include "Graph.h"

class LowestDoubleComparator {
public:
    bool operator()(const std::pair<int, double>&, const std::pair<int, double>&);
};

class DijkstraSP {
private:
    int source;
    std::vector<Edge> edgeTo;
    std::vector<double> distTo;

    void relax(Edge &e, std::priority_queue<std::pair<int, double>, std::vector<std::pair<int, double>>,
               LowestDoubleComparator> &pq);

public:
    explicit DijkstraSP(Graph &g);

    void shortestPath(std::list<Edge> &sp, int d);
};

#endif //MAP_ROUTING_DIJKSTRASP_H
