#include "AStar.h"

#include <queue>
#include <algorithm>
#include <cmath>

using namespace std;


// Represents a cell being considered by A*
struct AStarNode {

    Coordinate coordinate;

    int gCost;
    int hCost;
    int fCost;

    bool operator>(const AStarNode& other) const {
        return fCost > other.fCost;
    }
};


// Manhattan distance
int AStar::calculateHeuristic(
    Coordinate current,
    Coordinate goal
) const {

    return abs(current.row - goal.row)
         + abs(current.col - goal.col);
}


bool AStar::findPath(const Grid& grid) {

    // Clear previous results
    path_.clear();
    nodesExplored_ = 0;


    int rows = grid.getRows();
    int cols = grid.getCols();


    Coordinate start = grid.getStart();
    Coordinate goal = grid.getGoal();


    // Stores the cost from Start to each cell
    const int INF = 1000000000;

    vector<vector<int>> gCost(
        rows,
        vector<int>(cols, INF)
    );


    // Stores the previous cell for path reconstruction
    vector<vector<Coordinate>> parent(
        rows,
        vector<Coordinate>(cols, {-1, -1})
    );


    // Priority queue selects the cell with the lowest fCost
    priority_queue<
        AStarNode,
        vector<AStarNode>,
        greater<AStarNode>
    > openSet;


    // Start node
    int startHeuristic =
        calculateHeuristic(start, goal);


    gCost[start.row][start.col] = 0;


    openSet.push({
        start,
        0,
        startHeuristic,
        startHeuristic
    });


    // Four movement directions
    const int rowDirection[] = {-1, 1, 0, 0};
    const int colDirection[] = {0, 0, -1, 1};


    while (!openSet.empty()) {

        AStarNode currentNode = openSet.top();
        openSet.pop();


        Coordinate current =
            currentNode.coordinate;


        // Ignore outdated queue entries
        if (currentNode.gCost !=
            gCost[current.row][current.col]) {

            continue;
        }


        nodesExplored_++;


        // Goal reached
        if (current == goal) {

            reconstructPath(parent, grid);

            return true;
        }


        // Explore four neighbors
        for (int direction = 0;
             direction < 4;
             direction++) {


            int newRow =
                current.row +
                rowDirection[direction];


            int newCol =
                current.col +
                colDirection[direction];


            // Outside grid
            if (!grid.isInsideGrid(newRow, newCol)) {
                continue;
            }


            Coordinate neighbor{
                newRow,
                newCol
            };


            // Obstacle
            if (grid.isObstacle(neighbor)) {
                continue;
            }


            // Every movement costs 1
            int newGCost =
                currentNode.gCost + 1;


            // Found a better route to this cell
            if (newGCost < gCost[newRow][newCol]) {

                gCost[newRow][newCol] = newGCost;

                parent[newRow][newCol] =
                    current;


                int hCost =
                    calculateHeuristic(
                        neighbor,
                        goal
                    );


                int fCost =
                    newGCost + hCost;


                openSet.push({
                    neighbor,
                    newGCost,
                    hCost,
                    fCost
                });
            }
        }
    }


    // No path exists
    return false;
}


void AStar::reconstructPath(
    const vector<vector<Coordinate>>& parent,
    const Grid& grid
) {

    Coordinate current =
        grid.getGoal();

    Coordinate start =
        grid.getStart();


    while (!(current == start)) {

        path_.push_back(current);


        Coordinate previous =
            parent[current.row][current.col];


        // Safety check
        if (previous.row == -1 &&
            previous.col == -1) {

            path_.clear();

            return;
        }


        current = previous;
    }


    // Add start
    path_.push_back(start);


    // Reverse to get Start → Goal
    reverse(
        path_.begin(),
        path_.end()
    );
}


const vector<Coordinate>&
AStar::getPath() const {

    return path_;
}


int AStar::getNodesExplored() const {

    return nodesExplored_;
}