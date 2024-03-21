//
// Created by Sheldon Lobo on 3/17/24.
//

#ifndef MAP_ROUTING_GRAPH_H
#define MAP_ROUTING_GRAPH_H

#include <vector>
#include <forward_list>
#include <string>
#include <fstream>

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include "Node.h"

class Graph {
private:
    std::vector<Node> vertices;
    int nEdges;
    std::vector<std::forward_list<int>> adj;
    int source;
    int destination;

    // For drawing
    int maxXCoord;
    int maxYCoord;
    static const int offset = 50;
    float xScale;
    float yScale;

public:
    explicit Graph(std::ifstream& file);
    void addEdge(int v1, int v2);
    std::forward_list<int>& getAdjacents(int v);
    void draw(SDL_Renderer *renderer, int windowSize);
    int xScaled(int n) const;
    int yScaled(int n) const;
    friend std::ostream& operator<<(std::ostream&, const Graph&);
};

#endif //MAP_ROUTING_GRAPH_H
