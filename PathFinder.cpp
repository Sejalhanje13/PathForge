#include "PathFinder.h"

#include <queue>
#include <algorithm>

using namespace std;


bool PathFinder::findPath(const Grid& grid) {

    // Clear results from any previous search
    path_.clear();
    nodesExplored_ = 0;


    int rows = grid.getRows();
    int cols = grid.getCols();

    Coordinate start = grid.getStart();
    Coordinate goal = grid.getGoal();


    // Keeps track of cells that have already been visited
    vector<vector<bool>> visited(
        rows,
        vector<bool>(cols, false)
    );


    // Stores the previous cell used to reach each cell
    vector<vector<Coordinate>> parent(
        rows,
        vector<Coordinate>(cols, {-1, -1})
    );


    // Queue used by BFS
    queue<Coordinate> q;


    // Start BFS from the starting position
    q.push(start);
    visited[start.row][start.col] = true;


    // Four possible movement directions:
    // Up, Down, Left, Right
    const int rowDirection[] = {-1, 1, 0, 0};
    const int colDirection[] = {0, 0, -1, 1};


    while (!q.empty()) {

        Coordinate current = q.front();
        q.pop();

        nodesExplored_++;


        // Goal reached
        if (current == goal) {

            reconstructPath(parent, grid);

            return true;
        }


        // Check all four neighboring cells
        for (int direction = 0; direction < 4; direction++) {

            int newRow =
                current.row + rowDirection[direction];

            int newCol =
                current.col + colDirection[direction];


            // Check whether the new position is inside the grid
            if (!grid.isInsideGrid(newRow, newCol)) {
                continue;
            }


            // Skip already visited cells
            if (visited[newRow][newCol]) {
                continue;
            }
            // Skip obstacles
            if (grid.isObstacle({newRow, newCol})) {
                continue;
            }


            /*
                We cannot directly access Grid's private cells_.

                For Stage 3A, this is intentionally left as a
                connection point. In the next step we will add a
                small Grid method that tells PathFinder whether
                a cell is blocked.
            */

            visited[newRow][newCol] = true;

            parent[newRow][newCol] = current;

            q.push({newRow, newCol});
        }
    }


    // Queue became empty without reaching the goal
    return false;
}


void PathFinder::reconstructPath(
    const vector<vector<Coordinate>>& parent,
    const Grid& grid
) {

    Coordinate current = grid.getGoal();
    Coordinate start = grid.getStart();


    while (!(current == start)) {

        path_.push_back(current);

        Coordinate previous =
            parent[current.row][current.col];


        // Safety check
        if (previous.row == -1 && previous.col == -1) {
            path_.clear();
            return;
        }


        current = previous;
    }


    // Add start position
    path_.push_back(start);


    // Path was constructed backwards,
    // so reverse it.
    reverse(path_.begin(), path_.end());
}


const vector<Coordinate>& PathFinder::getPath() const {
    return path_;
}


int PathFinder::getNodesExplored() const {
    return nodesExplored_;
}