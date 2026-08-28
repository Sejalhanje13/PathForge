#ifndef ASTAR_H
#define ASTAR_H

#include "Grid.h"
#include <vector>

using namespace std;

class AStar {
public:

    // Finds the shortest path from start to goal using A*
    bool findPath(const Grid& grid);

    // Returns the path found by A*
    const vector<Coordinate>& getPath() const;

    // Returns how many nodes A* explored
    int getNodesExplored() const;

private:

    vector<Coordinate> path_;
    int nodesExplored_ = 0;

    // Manhattan distance heuristic
    int calculateHeuristic(
        Coordinate current,
        Coordinate goal
    ) const;

    // Reconstructs the final path
    void reconstructPath(
        const vector<vector<Coordinate>>& parent,
        const Grid& grid
    );
};

#endif