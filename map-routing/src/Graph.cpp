//
// Created by Sheldon Lobo on 3/17/24.
//

#include <sstream>
#include <iostream>

#include "Graph.h"
#include "SDL-Helpers.h"

Graph::Graph(std::ifstream& file) {
    std::string line;
    std::string src, dest;

    maxXCoord = maxYCoord = 0;

    // # vertices & edges
    getline(file, line);
    std::istringstream ss(line);
    std::string verticesString, edgesString;
    ss >> verticesString >> edgesString;
    int nVertices = std::stoi(verticesString);
    nEdges = std::stoi(edgesString);

    // Create the vector
    vertices = std::vector<Node>();
    vertices.reserve(nVertices);

    // Read the vertices
    for (int i = 0; i < nVertices; i++) {
        getline(file, line);

        // Skip empty lines
        while (line.empty())
            getline(file, line);

        ss.clear();
        ss.str(line);
        std::string idString, xString, yString;
        ss >> idString >> xString >> yString;
        int x = std::stoi(xString);
        int y = std::stoi(yString);
        maxXCoord = std::max(maxXCoord, x);
        maxYCoord = std::max(maxYCoord, y);
        vertices.emplace_back(std::stoi(idString), x, y);
    }

    // Build the adjacency vector for each vertex
    adj = std::vector<std::forward_list<Edge>>(vertices.size(), std::forward_list<Edge>());

    // Read the edges
    for (int i = 0; i < nEdges; i++) {
        getline(file, line);

        // Skip empty lines
        while (line.empty())
            getline(file, line);

        ss.clear();
        ss.str(line);
        ss >> src >> dest;
        addEdge(stoi(src), stoi(dest));
    }

    // Read the source and destination
    getline(file, line);
    while (line.empty()) {
        getline(file, line);
        if (file.eof())
            break;
    }
    if (!file.eof()) {
        ss.clear();
        ss.str(line);
        ss >> src >> dest;
        source = stoi(src);
        destination = stoi(dest);
        std::cout << "Graph src/dest : " << source << ", " << destination << std::endl;
    } else {
        std::cout << "No graph src/dest ... ";
        std::cin >> src >> dest;
        source = stoi(src);
        destination = stoi(dest);
        std::cout << "Input src/dest : " << source << ", " << destination << std::endl;
    }
}

void Graph::addEdge(int v1, int v2) {
    Edge edge(&vertices[v1], &vertices[v2]);
    adj[v1].push_front(edge);
    adj[v2].push_front(edge);
}

std::forward_list<Edge> & Graph::getAdjacents(int v) {
    return adj[v];
}

std::ostream& operator<<(std::ostream& strm, const Graph& g) {
    strm << "Vertices: " << g.vertices.size() << "; Edges: " << g.nEdges << std::endl;

    for (Node node : g.vertices) {
        strm << node << std::endl;
    }

    for (int i = 0; i < g.adj.size(); i++) {
        strm << i << " [ ";
        for (Edge edge : g.adj[i]) {
            strm << edge << ", ";
        }
        strm << "]" << std::endl;
    }

    return strm;
}

void Graph::draw(SDL_Renderer *renderer, int windowSize) {
    xScale = static_cast<float>(windowSize - 2*offset)/maxXCoord;
    yScale = static_cast<float>(windowSize - 2*offset)/maxYCoord;

    // Font for vertex ids
    TTF_Init();
    TTF_Font* font = TTF_OpenFont("fonts/Roboto-Regular.ttf", 14);
    if (!font) {
        std::cout << "Couldn't find/init open ttf font." << std::endl;
        std::exit(1);
    }
    SDL_Color blue = {0, 0, 255};

    // Draw vertices
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, SDL_ALPHA_OPAQUE);
    for (Node node : vertices) {
        int x = xScaled(node.x);
        int y = yScaled(node.y);
        if (node.id == source || node.id == destination)
            DrawCircleFill(renderer, x, y, 14);
        else
            DrawCircle(renderer, x, y, 14);

        // Write id in circle
        std::string idString = std::to_string(node.id);
        SDL_Surface* surfaceMessage = TTF_RenderText_Solid(font, idString.c_str(), blue);
        SDL_Texture* Message = SDL_CreateTextureFromSurface(renderer, surfaceMessage);

        SDL_Rect Message_rect; //create a rect
        Message_rect.x = x - 7;  //controls the rect's x coordinate
        Message_rect.y = y - 7; // controls the rect's y coordinte
        Message_rect.w = 14; // controls the width of the rect
        Message_rect.h = 14; // controls the height of the rect

        SDL_RenderCopy(renderer, Message, NULL, &Message_rect);

        SDL_FreeSurface(surfaceMessage);
        SDL_DestroyTexture(Message);
    }

    // Draw edges
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
    for (int i = 0; i < vertices.size(); i++) {
        std::forward_list<Edge> adj = getAdjacents(i);
        for (Edge edge : adj) {
            // Need to end edge outside the vertex circle
            int srcX = xScaled(edge.src->x);
            int srcY = yScaled(edge.src->y);
            int destX = xScaled(edge.dest->x);
            int destY = yScaled(edge.dest->y);
            if (srcX < destX) {
                srcX += 7;
                destX -= 7;
            } else {
                srcX -= 7;
                destX += 7;
            }
            if (srcY < destY) {
                srcY += 7;
                destY -= 7;
            } else {
                srcY -= 7;
                destY += 7;
            }
            SDL_RenderDrawLine(renderer, srcX, srcY, destX, destY);
        }
    }
}

int Graph::xScaled(int n) const {
    return static_cast<int>(n * xScale) + offset;
}

int Graph::yScaled(int n) const {
    return static_cast<int>((maxYCoord - n) * yScale) + offset;
}

size_t Graph::numVertices() {
    return vertices.size();
}

int Graph::getSource() {
    return source;
}

int Graph::getDestination() {
    return destination;
}

std::forward_list<Edge> Graph::adjacents(int v) {
    return adj[v];
}
