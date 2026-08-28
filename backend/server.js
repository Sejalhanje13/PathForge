const express = require("express");
const cors = require("cors");
const { spawn } = require("child_process");
const path = require("path");

const app = express();

const PORT = 3000;

app.use(cors());
app.use(express.json());


// =========================================
// C++ EXECUTABLE
// =========================================

const cppExecutable = path.join(
    __dirname,
    "..",
    "pathfinder.exe"
);


// =========================================
// TEST ROUTE
// =========================================

app.get("/", (req, res) => {

    res.json({
        status: "PathForge C++ Engine Online"
    });

});


// =========================================
// PARSE PATH COORDINATES
// =========================================

function parsePath(output, algorithm) {

    const marker =
        algorithm === "BFS"
            ? "BFS Result:"
            : "A* Result:";

    const startIndex = output.indexOf(marker);

    if (startIndex === -1) {
        return [];
    }

    const section = output.substring(startIndex);

    const pathIndex = section.indexOf("Path Coordinates:");

    if (pathIndex === -1) {
        return [];
    }

    const pathSection = section.substring(pathIndex);

    const lines = pathSection.split("\n");

    const path = [];

    for (let i = 1; i < lines.length; i++) {

        const line = lines[i].trim();

        // Stop when another section begins
        if (
            line.startsWith("========================================") ||
            line.startsWith("A* Result:") ||
            line.startsWith("BFS vs A* Comparison")
        ) {
            break;
        }

        const match = line.match(/^(-?\d+),(-?\d+)$/);

        if (match) {

            path.push({
                row: parseInt(match[1]),
                col: parseInt(match[2])
            });

        }
    }

    return path;
}


// =========================================
// RUN C++ ENGINE
// =========================================

app.post("/run", (req, res) => {

    const {
        rows,
        cols,
        startRow,
        startCol,
        goalRow,
        goalCol,
        obstacles
    } = req.body;

    console.log("Received request from dashboard.");

    let input = "";

    input += `${rows}\n`;
    input += `${cols}\n`;

    input += `${startRow}\n`;
    input += `${startCol}\n`;

    input += `${goalRow}\n`;
    input += `${goalCol}\n`;

    input += `${obstacles.length}\n`;

    for (const obstacle of obstacles) {

        input += `${obstacle.row}\n`;
        input += `${obstacle.col}\n`;

    }


    const cppProcess = spawn(cppExecutable);

    let output = "";
    let errorOutput = "";


    cppProcess.stdout.on("data", (data) => {

        output += data.toString();

    });


    cppProcess.stderr.on("data", (data) => {

        errorOutput += data.toString();

    });


    cppProcess.stdin.write(input);
    cppProcess.stdin.end();


    cppProcess.on("close", (code) => {

        console.log("C++ process finished.");
        console.log("Exit code:", code);

        console.log("========== C++ OUTPUT ==========");
        console.log(output);
        console.log("================================");


        if (code !== 0) {

            return res.status(500).json({

                success: false,
                error: errorOutput || "C++ engine failed.",
                output: output

            });

        }


        // =========================================
        // EXTRACT C++ PATHS
        // =========================================

        const bfsPath = parsePath(output, "BFS");
        const aStarPath = parsePath(output, "A*");


        console.log("BFS Path:", bfsPath);
        console.log("A* Path:", aStarPath);


        // =========================================
        // SEND RESULT TO FRONTEND
        // =========================================

        res.json({

            success: true,

            output: output,

            bfsPath: bfsPath,

            aStarPath: aStarPath

        });

    });


    cppProcess.on("error", (error) => {

        console.error(error);

        res.status(500).json({

            success: false,
            error: error.message

        });

    });

});


// =========================================
// START SERVER
// =========================================

app.listen(PORT, () => {

    console.log(
        `PathForge backend running at http://localhost:${PORT}`
    );

});