
#ifndef GRID_H
#define GRID_H

#include <vector>

using namespace std;

// Represents a position in the grid
struct Coordinate {
    int row;
    int col;

    bool operator==(const Coordinate& other) const {
        return row == other.row && col == other.col;
    }
};

class Grid {
public:
    // Creates a grid with a start and goal position
    Grid(int rows, int cols, Coordinate start, Coordinate goal);

    // Displays the grid in the terminal
    void display() const;

    // Checks whether a coordinate is inside the grid
    bool isInsideGrid(int row, int col) const;

    // Adds an obstacle to the grid
    bool addObstacle(Coordinate obstacle);

    // Returns true if the specified cell is an obstacle
    bool isObstacle(Coordinate coordinate) const;

    // Marks a cell as part of the final path
void markPath(const vector<Coordinate>& path);

    int getRows() const;
    int getCols() const;

    Coordinate getStart() const;
    Coordinate getGoal() const;

private:
    int rows_;
    int cols_;

    Coordinate start_;
    Coordinate goal_;

    vector<vector<char>> cells_;

    // Initializes all cells as empty
    void initializeCells();
};

#endif // GRID_H

