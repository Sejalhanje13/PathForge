#include "Grid.h"

#include <iostream>
#include <stdexcept>

using namespace std;

Grid::Grid(int rows, int cols, Coordinate start, Coordinate goal)
    : rows_(rows), cols_(cols), start_(start), goal_(goal) {

    // Validate grid dimensions
    if (rows_ <= 0 || cols_ <= 0) {
        throw invalid_argument("Grid dimensions must be positive.");
    }

    // Validate start position
    if (!isInsideGrid(start_.row, start_.col)) {
        throw invalid_argument("Start coordinate is outside the grid.");
    }

    // Validate goal position
    if (!isInsideGrid(goal_.row, goal_.col)) {
        throw invalid_argument("Goal coordinate is outside the grid.");
    }

    initializeCells();
}

void Grid::initializeCells() {
    // Create rows_ x cols_ grid filled with '.'
    cells_.assign(rows_, vector<char>(cols_, '.'));

    // Place start and goal
    cells_[start_.row][start_.col] = 'S';
    cells_[goal_.row][goal_.col] = 'G';
}

bool Grid::isInsideGrid(int row, int col) const {
    return row >= 0 &&
           row < rows_ &&
           col >= 0 &&
           col < cols_;
}

bool Grid::addObstacle(Coordinate obstacle) {

    // Check whether obstacle is inside the grid
    if (!isInsideGrid(obstacle.row, obstacle.col)) {
        return false;
    }

    // Start and goal cannot become obstacles
    if (obstacle == start_ || obstacle == goal_) {
        return false;
    }

    // Check whether the cell is already an obstacle
    if (cells_[obstacle.row][obstacle.col] == '#') {
        return false;
    }

    // Mark the cell as an obstacle
    cells_[obstacle.row][obstacle.col] = '#';

    return true;
}

bool Grid::isObstacle(Coordinate coordinate) const {

    if (!isInsideGrid(coordinate.row, coordinate.col)) {
        return false;
    }

    return cells_[coordinate.row][coordinate.col] == '#';
}

void Grid::markPath(const vector<Coordinate>& path) {

    for (const Coordinate& coordinate : path) {

        // Don't replace Start or Goal
        if (coordinate == start_ || coordinate == goal_) {
            continue;
        }

        cells_[coordinate.row][coordinate.col] = '*';
    }
}

void Grid::display() const {

    for (const auto& row : cells_) {

        for (char cell : row) {
            cout << cell << ' ';
        }

        cout << '\n';
    }
}

int Grid::getRows() const {
    return rows_;
}

int Grid::getCols() const {
    return cols_;
}

Coordinate Grid::getStart() const {
    return start_;
}

Coordinate Grid::getGoal() const {
    return goal_;
}
