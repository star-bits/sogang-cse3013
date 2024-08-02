#include "ofApp.h"
#include <string>
#include <vector>
#include <map>
#include <fstream>
#include <iostream>
#include <sstream>
#include <stack>
#include <algorithm>

using namespace std;

//--------------------------------------------------------------
void ofApp::setup() {
    ofSetWindowTitle("Maze Example");
    ofSetFrameRate(15);
    ofSetBackgroundColor(255, 255, 255);
    ofSetEscapeQuitsApp(false);
    this->draw_flag = false;
    this->showinfo_flag = false;
    this->fullscreen_flag = false;
    myFont.load("verdana.ttf", 12, true, true);
}

//--------------------------------------------------------------
void ofApp::update(){

}

//--------------------------------------------------------------
void ofApp::draw() {
    if (this->draw_flag) {
        ofSetLineWidth(5);
        ofSetColor(100);
        for (size_t i = 0; i < this->lines.size(); ++i) {
            MazeLine(get<0>(this->lines[i]), get<1>(this->lines[i]), get<2>(this->lines[i]), get<3>(this->lines[i]));
        }

        ofSetColor(255, 0, 0); // Red
        for (size_t i = 1; i < dfs_visited.size(); i += 2) {
            int start = dfs_visited[i - 1];
            int end = dfs_visited[i];
            auto [startX, startY] = graphCoordinates[start / mazeWidth][start % mazeWidth];
            auto [endX, endY] = graphCoordinates[end / mazeWidth][end % mazeWidth];
            MazeLine(startX, startY, endX, endY);
        }

        ofSetColor(0, 255, 0); // Green
        for (size_t i = 1; i < dfs_shortest.size(); ++i) {
            int start = dfs_shortest[i - 1];
            int end = dfs_shortest[i];
            auto [startX, startY] = graphCoordinates[start / mazeWidth][start % mazeWidth];
            auto [endX, endY] = graphCoordinates[end / mazeWidth][end % mazeWidth];
            MazeLine(startX, startY, endX, endY);
        }
    }

    if (myFont.isLoaded()) {
        string str = "comsil project";
        myFont.drawString(str, 15, ofGetHeight() - 20);
    }

    if (showinfo_flag) {
        myFont.drawString("Info: Press 'r' to read file, 'd' for DFS, 'b' for BFS, 'f' for fullscreen, 'q' to quit", 15, 30);
    }
}



//--------------------------------------------------------------
void ofApp::exit(){

}

//--------------------------------------------------------------
void ofApp::keyPressed(int key) {
    switch (key) {
        case 'r':
            cout << "r key pressed" << endl;
            if (readFile()) {
                this->draw_flag = 1;
            }
            break;
        case 'b':
            cout << "b key pressed" << endl;
            this->bfs_flag = 1;
            bfs();
            break;
        case 'd':
            cout << "d key pressed" << endl;
            this->dfs_flag = 1;
            dfs(0, graph.size() - 1); // Assuming start is 0 and end is the last cell
            break;
        case 'q':
            cout << "q key pressed" << endl;
            freeMemory();
            OF_EXIT_APP(0);
            break;
        case ' ':
            cout << "space key pressed" << endl;
            showinfo_flag = !showinfo_flag;
            break;
        case 'f':
            cout << "f key pressed" << endl;
            fullscreen_flag = !fullscreen_flag;
            doFullScreen(fullscreen_flag);
            break;
        case OF_KEY_ESC:
            cout << "esc key pressed" << endl;
            if (fullscreen_flag) {
                fullscreen_flag = false;
                doFullScreen(false);
            } else {
                OF_EXIT_APP(0);
            }
            break;
    }
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseScrolled(int x, int y, float scrollX, float scrollY){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){

}

bool ofApp::readFile() {
    freeMemory();
    ofFileDialogResult openFileResult = ofSystemLoadDialog("Select .maz file");

    if (!openFileResult.bSuccess) {
        cout << "File dialog was not successful" << endl;
        return false;
    }

    string file_location = openFileResult.getPath();
    ifstream input_file(file_location);

    if (!input_file.is_open()) {
        cout << "Failed to open file" << endl;
        return false;
    }

    string line;
    int realWIDTH = 0, realHEIGHT = 0;

    // Read file to determine dimensions
    while (getline(input_file, line)) {
        realWIDTH = line.length();
        realHEIGHT++;
    }

    // Reset file reading to beginning
    input_file.clear();
    input_file.seekg(0, ios::beg);

    // Calculate height and width of the maze
    int height = (realHEIGHT - 1) / 2;
    int width = (realWIDTH - 1) / 2;

    // Print height and width
    cout << "Height: " << height << endl;
    cout << "Width: " << width << endl;

    // Calculate and store graph coordinates
    graphCoordinates.resize(height, vector<pair<float, float>>(width));
    mazeWidth = width;
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            graphCoordinates[i][j] = make_pair(1.5 + j * 2, 1.5 + i * 2);
        }
    }

    // Print graph coordinates from cell 0 to n-1
    cout << "Coordinates:" << endl;
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            int cellIndex = i * width + j;
            cout << "Cell " << cellIndex << " coordinates: (" << graphCoordinates[i][j].first << ", " << graphCoordinates[i][j].second << ")" << endl;
        }
    }

    stringstream buffer;
    buffer << input_file.rdbuf();
    string mazeStr = buffer.str();
    graph = parseMaze(mazeStr);

    maze.resize(realHEIGHT, vector<char>(realWIDTH));
    istringstream iss(mazeStr);
    for (int i = 0; i < realHEIGHT; i++) {
        getline(iss, line);
        for (int j = 0; j < realWIDTH; j++) {
            maze[i][j] = line[j];
        }
    }

    // Print the maze
    cout << "Maze:" << endl;
    for (const auto& row : maze) {
        for (char cell : row) {
            cout << cell;
        }
        cout << endl;
    }

    for (int i = 0; i < realHEIGHT; i++) {
        for (int j = 0; j < realWIDTH; j++) {
            if (maze[i][j] == '-') {
                lines.emplace_back(j - 0.5, i + 0.5, j + 1.5, i + 0.5);
            } else if (maze[i][j] == '|') {
                lines.emplace_back(j + 0.5, i - 0.5, j + 0.5, i + 1.5);
            }
        }
    }

    // Print the coordinates of lines
    cout << "Lines:" << endl;
    for (const auto& line : lines) {
        cout << "From (" << std::get<0>(line) << ", " << std::get<1>(line) << ") to ("
             << std::get<2>(line) << ", " << std::get<3>(line) << ")" << endl;
    }

    // Print the graph in ascending order
    for (const auto& [key, value] : graph) {
        cout << key << ": ";
        for (int v : value) {
            cout << v << " ";
        }
        cout << endl;
    }

    return true;
}

std::map<int, std::vector<int>> ofApp::parseMaze(const string& mazeStr) {
    istringstream iss(mazeStr);
    vector<string> lines;
    string line;

    while (getline(iss, line)) {
        lines.push_back(line);
    }

    int height = (lines.size() - 1) / 2;
    int width = (lines[0].size() - 1) / 2;
    map<int, vector<int>> graph;

    for (int i = 0; i < height * width; ++i) {
        graph[i] = vector<int>();
    }

    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            int cellIndex = i * width + j;
            // Check right neighbor
            if (j < width - 1 && lines[2 * i + 1][2 * j + 2] == ' ') {
                graph[cellIndex].push_back(cellIndex + 1);
                graph[cellIndex + 1].push_back(cellIndex);
            }
            // Check bottom neighbor
            if (i < height - 1 && lines[2 * i + 2][2 * j + 1] == ' ') {
                graph[cellIndex].push_back(cellIndex + width);
                graph[cellIndex + width].push_back(cellIndex);
            }
        }
    }

    return graph;
}

void ofApp::MazeLine(float startX, float startY, float endX, float endY) {
    ofDrawLine(20 * startX, 20 * startY, 20 * endX, 20 * endY);
}

void ofApp::freeMemory() {
    maze.clear();
    lines.clear();
    graph.clear();
    dfs_visited.clear();
    dfs_shortest.clear();
    graphCoordinates.clear();
}

void ofApp::dfs(int start, int goal) {
    stack<pair<int, vector<int>>> stack;
    stack.push({start, {start}});
    dfs_visited.clear();
    dfs_shortest.clear();
    set<int> visited;

    while (!stack.empty()) {
        auto [vertex, path] = stack.top();
        stack.pop();

        if (path.size() > 1) {
            dfs_visited.push_back(path[path.size() - 2]);
            dfs_visited.push_back(path.back());
        }

        if (vertex == goal) {
            dfs_shortest = path;
            return;
        }

        if (visited.find(vertex) == visited.end()) {
            visited.insert(vertex);
            for (auto it = graph[vertex].rbegin(); it != graph[vertex].rend(); ++it) {
                int neighbor = *it;
                if (visited.find(neighbor) == visited.end()) {
                    stack.push({neighbor, path});
                    stack.top().second.push_back(neighbor);
                }
            }
        }
    }
}

int ofApp::bfs() {
    // BFS implementation placeholder
    return 0;
}

void ofApp::doFullScreen(bool fullscreen) {
    if (fullscreen) {
        ofSetFullscreen(true);
    } else {
        ofSetFullscreen(false);
    }
}
