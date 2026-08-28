const gridElement = document.getElementById("grid");

const rowsInput = document.getElementById("rows");
const colsInput = document.getElementById("cols");

const generateBtn = document.getElementById("generateBtn");
const clearBtn = document.getElementById("clearBtn");

const bfsBtn = document.getElementById("bfsBtn");
const astarBtn = document.getElementById("astarBtn");
const compareBtn = document.getElementById("compareBtn");

const bfsStatus = document.getElementById("bfsStatus");
const bfsPath = document.getElementById("bfsPath");
const bfsNodes = document.getElementById("bfsNodes");

const astarStatus = document.getElementById("astarStatus");
const astarPath = document.getElementById("astarPath");
const astarNodes = document.getElementById("astarNodes");

const comparisonText = document.getElementById("comparisonText");


// ============================================
// GRID DATA
// ============================================

let rows = 10;
let cols = 10;

let start = {
    row: 0,
    col: 0
};

let goal = {
    row: 9,
    col: 9
};

let obstacles = [];

let currentResults = {
    bfs: null,
    astar: null
};


// ============================================
// GENERATE GRID
// ============================================

function generateGrid() {

    rows = parseInt(rowsInput.value);
    cols = parseInt(colsInput.value);

    if (rows < 2 || cols < 2) {
        alert("Rows and columns must be at least 2.");
        return;
    }

    start = {
        row: 0,
        col: 0
    };

    goal = {
        row: rows - 1,
        col: cols - 1
    };

    obstacles = [];

    currentResults.bfs = null;
    currentResults.astar = null;

    resetResults();

    gridElement.innerHTML = "";

    gridElement.style.gridTemplateColumns =
        `repeat(${cols}, 1fr)`;


    for (let row = 0; row < rows; row++) {

        for (let col = 0; col < cols; col++) {

            const cell = document.createElement("div");

            cell.classList.add("cell");

            cell.dataset.row = row;
            cell.dataset.col = col;


            // START

            if (row === start.row && col === start.col) {

                cell.classList.add("start");
                cell.textContent = "S";

            }


            // GOAL

            else if (row === goal.row && col === goal.col) {

                cell.classList.add("goal");
                cell.textContent = "G";

            }


            // CLICK CELL

            cell.addEventListener("click", () => {

                toggleObstacle(row, col);

            });


            gridElement.appendChild(cell);
        }
    }
}


// ============================================
// TOGGLE OBSTACLE
// ============================================

function toggleObstacle(row, col) {

    // Don't allow obstacle on start

    if (
        row === start.row &&
        col === start.col
    ) {
        return;
    }


    // Don't allow obstacle on goal

    if (
        row === goal.row &&
        col === goal.col
    ) {
        return;
    }


    const index = obstacles.findIndex(
        obstacle =>
            obstacle.row === row &&
            obstacle.col === col
    );


    const cell = document.querySelector(
        `.cell[data-row="${row}"][data-col="${col}"]`
    );


    if (index === -1) {

        obstacles.push({
            row: row,
            col: col
        });

        cell.classList.add("obstacle");

    } else {

        obstacles.splice(index, 1);

        cell.classList.remove("obstacle");

    }


    clearPaths();
}


// ============================================
// CLEAR GRID
// ============================================

function clearGrid() {

    obstacles = [];

    currentResults.bfs = null;
    currentResults.astar = null;

    resetResults();

    generateGrid();
}


// ============================================
// CLEAR PATH VISUALIZATION
// ============================================

function clearPaths() {

    document
        .querySelectorAll(".cell.path")
        .forEach(cell => {

            cell.classList.remove("path");

        });
}


// ============================================
// DRAW PATH ON GRID
// ============================================

function drawPath(path) {

    clearPaths();

    if (!path || path.length === 0) {
        return;
    }

    path.forEach(position => {

        const cell = document.querySelector(
            `.cell[data-row="${position.row}"][data-col="${position.col}"]`
        );

        if (!cell) {
            return;
        }

        // Don't replace Start or Goal styling
        if (
            (position.row === start.row &&
             position.col === start.col) ||
            (position.row === goal.row &&
             position.col === goal.col)
        ) {
            return;
        }

        cell.classList.add("path");
    });
}

// ============================================
// RESET RESULTS
// ============================================

function resetResults() {

    bfsStatus.textContent = "—";
    bfsPath.textContent = "—";
    bfsNodes.textContent = "—";

    astarStatus.textContent = "—";
    astarPath.textContent = "—";
    astarNodes.textContent = "—";

    comparisonText.textContent =
        "Run both algorithms to compare their performance.";
}


// ============================================
// SEND DATA TO C++ ENGINE
// ============================================

async function runCppEngine() {

    const requestData = {

        rows: rows,
        cols: cols,

        startRow: start.row,
        startCol: start.col,

        goalRow: goal.row,
        goalCol: goal.col,

        obstacles: obstacles
    };


    try {

        const response = await fetch(
            "http://localhost:3000/run",
            {
                method: "POST",

                headers: {
                    "Content-Type": "application/json"
                },

                body: JSON.stringify(requestData)
            }
        );


        const data = await response.json();


        if (!data.success) {

            console.error(data.error);

            alert(
                "C++ engine error:\n" +
                (data.error || "Unknown error")
            );

            return null;
        }


        console.log("C++ OUTPUT:");
        console.log(data.output);

        return data.output;

    } catch (error) {

        console.error(error);

        alert(
            "Could not connect to PathForge backend.\n\n" +
            "Make sure Node.js server is running."
        );

        return null;
    }
}


// ============================================
// PARSE BFS RESULT
// ============================================

function parseBFS(output) {

    const section = output.match(
        /BFS Result:([\s\S]*?)(?=Running A\*|A\* Result:|BFS vs A\* Comparison|$)/
    );

    if (!section) {
        return null;
    }

    const text = section[1];

    const pathFoundMatch =
        text.match(/Path Found:\s*(Yes|No)/i);

    const pathLengthMatch =
        text.match(/Path Length:\s*(\d+)/i);

    const nodesMatch =
        text.match(/Nodes Explored:\s*(\d+)/i);

    const pathCoordinatesMatch =
        text.match(
            /Path Coordinates:\s*([\s\S]*?)(?=\n\s*\n|$)/i
        );

    let path = [];

    if (pathCoordinatesMatch) {

        const coordinateLines =
            pathCoordinatesMatch[1]
                .trim()
                .split("\n");

        path = coordinateLines
            .map(line => {

                const match =
                    line.trim().match(/^(\d+),(\d+)$/);

                if (!match) {
                    return null;
                }

                return {
                    row: parseInt(match[1]),
                    col: parseInt(match[2])
                };

            })
            .filter(Boolean);
    }

    return {

        found:
            pathFoundMatch
                ? pathFoundMatch[1].toLowerCase() === "yes"
                : false,

        pathLength:
            pathLengthMatch
                ? parseInt(pathLengthMatch[1])
                : 0,

        nodes:
            nodesMatch
                ? parseInt(nodesMatch[1])
                : 0,

        path: path
    };
}


// ============================================
// PARSE A* RESULT
// ============================================
function parseAStar(output) {

    const section = output.match(
        /A\* Result:([\s\S]*?)(?=BFS vs A\* Comparison|$)/
    );

    if (!section) {
        return null;
    }

    const text = section[1];

    const pathFoundMatch =
        text.match(/Path Found:\s*(Yes|No)/i);

    const pathLengthMatch =
        text.match(/Path Length:\s*(\d+)/i);

    const nodesMatch =
        text.match(/Nodes Explored:\s*(\d+)/i);

    const pathCoordinatesMatch =
        text.match(
            /Path Coordinates:\s*([\s\S]*?)(?=\n\s*\n|$)/i
        );

    let path = [];

    if (pathCoordinatesMatch) {

        const coordinateLines =
            pathCoordinatesMatch[1]
                .trim()
                .split("\n");

        path = coordinateLines
            .map(line => {

                const match =
                    line.trim().match(/^(\d+),(\d+)$/);

                if (!match) {
                    return null;
                }

                return {
                    row: parseInt(match[1]),
                    col: parseInt(match[2])
                };

            })
            .filter(Boolean);
    }

    return {

        found:
            pathFoundMatch
                ? pathFoundMatch[1].toLowerCase() === "yes"
                : false,

        pathLength:
            pathLengthMatch
                ? parseInt(pathLengthMatch[1])
                : 0,

        nodes:
            nodesMatch
                ? parseInt(nodesMatch[1])
                : 0,

        path: path
    };
}

// ==============================
// UPDATE BFS CARD
// ============================================

function displayBFS(result) {

    if (!result) {
        return;
    }


    bfsStatus.textContent =
        result.found
            ? "Path Found"
            : "No Path";


    bfsPath.textContent =
        result.found
            ? result.pathLength
            : "—";


    bfsNodes.textContent =
        result.nodes;
}


// ============================================
// UPDATE A* CARD
// ============================================

function displayAStar(result) {

    if (!result) {
        return;
    }


    astarStatus.textContent =
        result.found
            ? "Path Found"
            : "No Path";


    astarPath.textContent =
        result.found
            ? result.pathLength
            : "—";


    astarNodes.textContent =
        result.nodes;
}


// ============================================
// RUN BFS
// ============================================

async function runBFS() {

    clearPaths();

    comparisonText.textContent =
        "Running BFS...";


    const output = await runCppEngine();


    if (!output) {
        return;
    }


    const result = parseBFS(output);


    if (!result) {

        alert(
            "Could not read BFS result from C++."
        );

        return;
    }


    currentResults.bfs = result;

    displayBFS(result);

    if (result.found) {
        drawPath(result.path);
    }


    if (result.found) {

        comparisonText.textContent =
            `BFS found a shortest path of ` +
            `${result.pathLength} steps and explored ` +
            `${result.nodes} nodes.`;

    } else {

        comparisonText.textContent =
            "BFS could not find a path.";
    }
}


// ============================================
// RUN A*
// ============================================

async function runAStar() {

    clearPaths();

    comparisonText.textContent =
        "Running A*...";


    const output = await runCppEngine();


    if (!output) {
        return;
    }


    const result = parseAStar(output);


    if (!result) {

        alert(
            "Could not read A* result from C++."
        );

        return;
    }


    currentResults.astar = result;

    displayAStar(result);

    if (result.found) {
        drawPath(result.path);
    }


    if (result.found) {

        comparisonText.textContent =
            `A* found a shortest path of ` +
            `${result.pathLength} steps and explored ` +
            `${result.nodes} nodes.`;

    } else {

        comparisonText.textContent =
            "A* could not find a path.";
    }
}


// ============================================
// COMPARE BFS vs A*
// ============================================

async function compareAlgorithms() {

    clearPaths();

    comparisonText.textContent =
        "Running BFS and A*...";


    const output = await runCppEngine();


    if (!output) {
        return;
    }


    const bfsResult = parseBFS(output);
    const astarResult = parseAStar(output);


    if (!bfsResult || !astarResult) {

        alert(
            "Could not read algorithm results."
        );

        return;
    }


    currentResults.bfs = bfsResult;
    currentResults.astar = astarResult;


    displayBFS(bfsResult);
    displayAStar(astarResult);

    if (astarResult.found) {
        drawPath(astarResult.path);
    }
    else if (bfsResult.found) {
        drawPath(bfsResult.path);
    }


    // BOTH FOUND PATH

    if (
        bfsResult.found &&
        astarResult.found
    ) {

        if (
            bfsResult.nodes >
            astarResult.nodes
        ) {

            const improvement =
                (
                    (
                        bfsResult.nodes -
                        astarResult.nodes
                    )
                    /
                    bfsResult.nodes
                ) * 100;


            comparisonText.textContent =
                `A* explored ${improvement.toFixed(1)}% ` +
                `fewer nodes than BFS while finding ` +
                `the same shortest path.`;

        }


        else if (
            astarResult.nodes >
            bfsResult.nodes
        ) {

            const improvement =
                (
                    (
                        astarResult.nodes -
                        bfsResult.nodes
                    )
                    /
                    astarResult.nodes
                ) * 100;


            comparisonText.textContent =
                `BFS explored ${improvement.toFixed(1)}% ` +
                `fewer nodes than A* in this case.`;

        }


        else {

            comparisonText.textContent =
                "BFS and A* explored the same number of nodes.";
        }

    }


    else {

        comparisonText.textContent =
            "One or both algorithms could not find a path.";
    }
}


// ============================================
// BUTTON EVENTS
// ============================================

generateBtn.addEventListener(
    "click",
    generateGrid
);

clearBtn.addEventListener(
    "click",
    clearGrid
);

bfsBtn.addEventListener(
    "click",
    runBFS
);

astarBtn.addEventListener(
    "click",
    runAStar
);

compareBtn.addEventListener(
    "click",
    compareAlgorithms
);


// ============================================
// INITIAL GRID
// ============================================

generateGrid();