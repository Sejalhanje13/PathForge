#ifndef PATHFINDER_H
#define PATHFINDER_H

#include "Grid.h"
#include <vector>

using namespace std;

class PathFinder {
public:
    // Finds the shortest path from start to goal using BFS
    bool findPath(const Grid& grid);

    // Returns the path found by BFS
    const vector<Coordinate>& getPath() const;

    // Returns the number of cells explored by BFS
    int getNodesExplored() const;

private:
    vector<Coordinate> path_;
    int nodesExplored_ = 0;

    // Reconstructs the path after BFS reaches the goal
    void reconstructPath(
        const vector<vector<Coordinate>>& parent,
        const Grid& grid
    );
};

#endif