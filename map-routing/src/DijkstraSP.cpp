//
// Created by Sheldon Lobo on 4/19/24.
//

#include "DijkstraSP.h"

bool LowestDoubleComparator::operator()(const std::pair<int, double> &a, const std::pair<int, double> &b) {
    return a.second > b.second;
}

DijkstraSP::DijkstraSP(Graph &g) {
    source = g.getSource();

    edgeTo = std::vector<Edge>(g.numVertices());
    distTo = std::vector<double>(g.numVertices());

    std::priority_queue<std::pair<int, double>, std::vector<std::pair<int, double>>,
            LowestDoubleComparator> pq;

    for (int i = 0; i < g.numVertices(); i++) {
        distTo[i] = DBL_MAX;
    }
    distTo[g.getSource()] = 0.0;

    pq.emplace(g.getSource(), 0.0);
    while (!pq.empty()) {
        std::pair<int, double> closest = pq.top();
        pq.pop();

        for (Edge &e : g.adjacents(closest.first)) {
            relax(e, pq);
        }
    }
}

void DijkstraSP::relax(Edge &e, std::priority_queue<std::pair<int, double>,
        std::vector<std::pair<int, double>>, LowestDoubleComparator> &pq) {
    int s = e.src->id;
    int d = e.dest->id;

    if (distTo[d] > distTo[s] + e.distance) {
        distTo[d] = distTo[s] + e.distance;
        edgeTo[d] = e;
        pq.emplace(d, distTo[d]);
    }
}

void DijkstraSP::shortestPath(std::list<Edge> &sp, int d) {
    int v = d;
    while (v != source) {
        Edge e = edgeTo[v];
        sp.emplace_front(e);
        v = e.src->id;
    }
}
