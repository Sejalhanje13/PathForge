#include <iostream>
#include <limits>
#include <string>
#include <stdexcept>
#include <vector>

#include "Grid.h"
#include "PathFinder.h"
#include "AStar.h"

using namespace std;


// Reads an integer safely from the user
int readInt(const string& prompt) {
    int value;

    while (true) {
        cout << prompt;

        if (cin >> value) {
            return value;
        }

        cout << "Invalid input. Please enter a whole number.\n";

        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
}


// Reads a non-negative integer
int readNonNegativeInt(const string& prompt) {
    while (true) {
        int value = readInt(prompt);

        if (value >= 0) {
            return value;
        }

        cout << "Please enter a non-negative number.\n";
    }
}

void printPath(const vector<Coordinate>& path) {

    cout << "Path Coordinates:\n";

    for (const Coordinate& coordinate : path) {

        cout << coordinate.row
             << ","
             << coordinate.col
             << '\n';
    }
}

int main() {

    cout << "=== C++ Pathfinding & Obstacle Detection Simulator ===\n";
    cout << "--- Stage 5: BFS vs A* Pathfinding ---\n\n";


    // -----------------------------------------
    // 1. Get grid dimensions
    // -----------------------------------------

    int rows = readInt("Enter number of rows: ");
    int cols = readInt("Enter number of columns: ");


    // -----------------------------------------
    // 2. Get start position
    // -----------------------------------------

    int startRow = readInt("Enter start row: ");
    int startCol = readInt("Enter start column: ");


    // -----------------------------------------
    // 3. Get goal position
    // -----------------------------------------

    int goalRow = readInt("Enter goal row: ");
    int goalCol = readInt("Enter goal column: ");


    Coordinate start{startRow, startCol};
    Coordinate goal{goalRow, goalCol};


    try {

        // -----------------------------------------
        // 4. Create grid
        // -----------------------------------------

        Grid grid(rows, cols, start, goal);

        cout << "\nGrid created successfully.\n";


        // -----------------------------------------
        // 5. Add obstacles
        // -----------------------------------------

        int obstacleCount =
            readNonNegativeInt("\nEnter number of obstacles: ");


        int obstaclesAdded = 0;

        for (int i = 0; i < obstacleCount; i++) {

            cout << "\nObstacle " << i + 1 << ":\n";

            int obstacleRow =
                readInt("Enter row: ");

            int obstacleCol =
                readInt("Enter column: ");


            Coordinate obstacle{
                obstacleRow,
                obstacleCol
            };


            if (grid.addObstacle(obstacle)) {

                cout << "Obstacle added successfully.\n";

                obstaclesAdded++;

            }
            else {

                cout << "Invalid obstacle position.\n";
                cout << "Obstacle was NOT added.";
            }
        }


        // -----------------------------------------
        // 6. Display grid
        // -----------------------------------------

        cout << "\n========================================\n";
        cout << "Grid:\n";
        cout << "========================================\n\n";

        grid.display();


        cout << "\nObstacles added: "
             << obstaclesAdded
             << " / "
             << obstacleCount
             << '\n';


        // =========================================
        // 7. BFS
        // =========================================

        PathFinder bfs;

        cout << "\n========================================\n";
        cout << "Running BFS...\n";
        cout << "========================================\n";


        bool bfsFound =
            bfs.findPath(grid);


        if (bfsFound) {

            cout << "\nBFS Result:\n";
            cout << "Path Found: Yes\n";

            cout << "Path Length: "
                 << bfs.getPath().size() - 1
                 << '\n';

            cout << "Nodes Explored: "
                 << bfs.getNodesExplored()
                 << '\n';

            printPath(bfs.getPath());

        }
        else {

            cout << "\nBFS Result:\n";
            cout << "Path Found: No\n";
        }


        // =========================================
        // 8. A*
        // =========================================

        AStar aStar;

        cout << "\n========================================\n";
        cout << "Running A*...\n";
        cout << "========================================\n";


        bool aStarFound =
            aStar.findPath(grid);


        if (aStarFound) {

            cout << "\nA* Result:\n";
            cout << "Path Found: Yes\n";

            cout << "Path Length: "
                 << aStar.getPath().size() - 1
                 << '\n';

            cout << "Nodes Explored: "
                 << aStar.getNodesExplored()
                 << '\n';
            printPath(aStar.getPath());

        }
        else {

            cout << "\nA* Result:\n";
            cout << "Path Found: No\n";
        }


        // =========================================
        // 9. Comparison
        // =========================================

        cout << "\n========================================\n";
        cout << "BFS vs A* Comparison\n";
        cout << "========================================\n\n";


        if (bfsFound && aStarFound) {

            cout << "BFS:\n";
            cout << "  Path Length: "
                 << bfs.getPath().size() - 1
                 << '\n';

            cout << "  Nodes Explored: "
                 << bfs.getNodesExplored()
                 << "\n\n";


            cout << "A*:\n";
            cout << "  Path Length: "
                 << aStar.getPath().size() - 1
                 << '\n';

            cout << "  Nodes Explored: "
                 << aStar.getNodesExplored()
                 << "\n\n";


            if (aStar.getNodesExplored() <
                bfs.getNodesExplored()) {

                cout << "A* explored fewer nodes.\n";

            }
            else if (aStar.getNodesExplored() >
                     bfs.getNodesExplored()) {

                cout << "BFS explored fewer nodes.\n";

            }
            else {

                cout << "Both algorithms explored the same "
                     << "number of nodes.\n";
            }

        }
        else {

            cout << "A complete comparison is not possible "
                 << "because a path was not found.\n";
        }

    }

    catch (const invalid_argument& e) {

        cout << "\nError: "
             << e.what()
             << '\n';

        return 1;
    }


    return 0;
}