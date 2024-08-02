#include "ofApp.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <stack>
#include <queue>
#include <algorithm>

using namespace std;

//--------------------------------------------------------------
void ofApp::setup() {
    ofSetWindowTitle("The Maze");
    ofSetFrameRate(60);
    ofSetBackgroundColor(255, 255, 255);
    ofSetEscapeQuitsApp(false);

    draw_flag = false;
    showinfo_flag = true;
    fullscreen_flag = false;
    dfs_flag = false;
    bfs_flag = false;
    delayActive = false;

    currentExploredIndex = 0;
    animationStartTime = 0;
    isAnimating = false;
    showShortestPath = false;

    myFont.load("verdana.ttf", 12, true, true);
}

//--------------------------------------------------------------
void ofApp::update() {
    if (isAnimating || delayActive) {
        updateAnimation();
    }
}

//--------------------------------------------------------------
void ofApp::draw() {
    if (draw_flag) {
        ofSetLineWidth(5);
        ofSetColor(100);

        // for line in lines로 미로 그리기
        for (const auto& line : lines) {
            MazeLine(get<0>(line), get<1>(line), get<2>(line), get<3>(line));
        }

        const auto& explored = dfs_flag ? dfs_explored : bfs_explored;
        if (dfs_flag || bfs_flag) {
            ofSetColor(255, 0, 0);
            // currentExploredIndex는 updateAnimation에서 업데이트 됨
            for (int i = 1; i < currentExploredIndex && i < explored.size(); i += 2) {
                int start = explored[i - 1];
                int end = explored[i];

                // 선이 아님
                if (start == end) continue;

                auto [startX, startY] = graphCoordinates[start / mazeWidth][start % mazeWidth];
                auto [endX, endY] = graphCoordinates[end / mazeWidth][end % mazeWidth];

                // 중복 확인
                bool isDuplicate = false;
                for (int j = 1; j < i; j += 2) {
                    if (explored[j - 1] == start && explored[j] == end) {
                        isDuplicate = true;
                        break;
                    }
                }
                if (isDuplicate) continue;

                if (i == currentExploredIndex - 1 && isAnimating) {
                    // 0.1 is the line segment animation duration
                    float progress = (ofGetElapsedTimef() - animationStartTime) / 0.1;
                    progress = ofClamp(progress, 0, 1);
                    // ofLerp is the function for linear interpolation
                    float currentEndX = ofLerp(startX, endX, progress);
                    float currentEndY = ofLerp(startY, endY, progress);
                    MazeLine(startX, startY, currentEndX, currentEndY);
                } else {
                    MazeLine(startX, startY, endX, endY);
                }
            }

            // 최단거리는 애니메이션 없음
            if (showShortestPath) {
                const auto& shortest = dfs_flag ? dfs_shortest : bfs_shortest;
                ofSetColor(0, 255, 0);
                for (size_t i = 1; i < shortest.size(); ++i) {
                    int start = shortest[i - 1];
                    int end = shortest[i];
                    auto [startX, startY] = graphCoordinates[start / mazeWidth][start % mazeWidth];
                    auto [endX, endY] = graphCoordinates[end / mazeWidth][end % mazeWidth];
                    MazeLine(startX, startY, endX, endY);
                }
            }
        }
    }

    ofSetColor(0);
    if (myFont.isLoaded()) {
        string str = "jaeh8nkim 20161277";
        myFont.drawString(str, 15, ofGetHeight() - 20);
    }

    if (showinfo_flag) {
        string infoText = "Commands:\n r \tRead file\n d \trun DFS\n b \trun BFS\n h \thide/unhide Help\n f \tgo Fullscreen\n q \tQuit";
        if (myFont.isLoaded()) {
            myFont.drawString(infoText, 15, 30);
        } else {
            ofDrawBitmapString(infoText, 15, 30);
        }
    }
}

//--------------------------------------------------------------
void ofApp::exit() {}

//--------------------------------------------------------------
void ofApp::keyPressed(int key) {
    switch (key) {
        case 'r':
            cout << "r key pressed" << endl;
            if (readFile()) {
                draw_flag = true;
            }
            break;
        case 'b':
            cout << "b key pressed" << endl;
            bfs_flag = true;
            dfs_flag = false;
            bfs(0, graph.size() - 1);
            currentExploredIndex = 0;
            animationStartTime = ofGetElapsedTimef();
            isAnimating = true;
            showShortestPath = false;
            break;
        case 'd':
            cout << "d key pressed" << endl;
            dfs_flag = true;
            bfs_flag = false;
            dfs(0, graph.size() - 1);
            currentExploredIndex = 0;
            animationStartTime = ofGetElapsedTimef();
            isAnimating = true;
            showShortestPath = false;
            break;
        case 'q':
            cout << "q key pressed" << endl;
            freeMemory();
            OF_EXIT_APP(0);
            break;
        case 'h':
            cout << "h key pressed" << endl;
            showinfo_flag = !showinfo_flag;
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
void ofApp::keyReleased(int key) {}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y) {}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button) {}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button) {}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button) {}

//--------------------------------------------------------------
void ofApp::mouseScrolled(int x, int y, float scrollX, float scrollY) {}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y) {}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y) {}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h) {}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg) {}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo) {}

//--------------------------------------------------------------
bool ofApp::readFile() {
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

    // 이전 미로 메모리 삭제
    freeMemory();
    
    // realWIDTH, realHEIGHT가 파일의 가로, 세로
    string line;
    int realWIDTH = 0, realHEIGHT = 0;
    while (getline(input_file, line)) {
        realWIDTH = line.length();
        realHEIGHT++;
    }

    input_file.clear();
    input_file.seekg(0, ios::beg);

    // height, width가 미로의 관념적 세로, 가로
    int height = (realHEIGHT - 1) / 2;
    int width = (realWIDTH - 1) / 2;
    cout << "Height: " << height << endl;
    cout << "Width: " << width << endl;

    // 미로의 각 셀의 중심의 좌표
    graphCoordinates.resize(height, vector<pair<float, float>>(width));
    mazeWidth = width;
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            graphCoordinates[i][j] = make_pair(1.5 + j * 2, 1.5 + i * 2);
        }
    }

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
    // into an undirected graph in dict form
    graph = parseMaze(mazeStr);

    maze.resize(realHEIGHT, vector<char>(realWIDTH));
    istringstream iss(mazeStr);
    // 파일에서 미로 읽기
    for (int i = 0; i < realHEIGHT; i++) {
        getline(iss, line);
        for (int j = 0; j < realWIDTH; j++) {
            maze[i][j] = line[j];
        }
    }

    // 파일상의 미로 출력
    cout << "Maze:" << endl;
    for (const auto& row : maze) {
        for (char cell : row) {
            cout << cell;
        }
        cout << endl;
    }

    // 미로의 가로벽, 세로벽
    for (int i = 0; i < realHEIGHT; i++) {
        for (int j = 0; j < realWIDTH; j++) {
            if (maze[i][j] == '-') {
                lines.emplace_back(j - 0.5, i + 0.5, j + 1.5, i + 0.5);
            } else if (maze[i][j] == '|') {
                lines.emplace_back(j + 0.5, i - 0.5, j + 0.5, i + 1.5);
            }
        }
    }

    // 가로벽, 세로벽의 좌표 출력
    cout << "Lines:" << endl;
    for (const auto& line : lines) {
        cout << "From (" << std::get<0>(line) << ", " << std::get<1>(line) << ") to ("
             << std::get<2>(line) << ", " << std::get<3>(line) << ")" << endl;
    }

    // undirected graph in dict form 출력
    for (const auto& [key, value] : graph) {
        cout << key << ": ";
        for (int v : value) {
            cout << v << " ";
        }
        cout << endl;
    }

    showinfo_flag = false;
    return true;
}

//--------------------------------------------------------------
std::map<int, std::vector<int>> ofApp::parseMaze(const string& mazeStr) {
    istringstream iss(mazeStr);
    vector<string> lines;
    string line;

    // lines.append(line)
    while (getline(iss, line)) {
        lines.push_back(line);
    }

    int height = (lines.size() - 1) / 2;
    int width = (lines[0].size() - 1) / 2;

    // empty graph with each cell as a node
    map<int, vector<int>> graph;
    for (int i = 0; i < height * width; ++i) {
        graph[i] = vector<int>();
    }

    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            int cellIndex = i * width + j;
            // 오른쪽 셀 확인
            if (j < width - 1 && lines[2 * i + 1][2 * j + 2] == ' ') {
                graph[cellIndex].push_back(cellIndex + 1);
                graph[cellIndex + 1].push_back(cellIndex);
            }
            // 아래쪽 셀 확인
            if (i < height - 1 && lines[2 * i + 2][2 * j + 1] == ' ') {
                graph[cellIndex].push_back(cellIndex + width);
                graph[cellIndex + width].push_back(cellIndex);
            }
        }
    }
    return graph;
}

//--------------------------------------------------------------
void ofApp::MazeLine(float startX, float startY, float endX, float endY) {
    ofDrawLine(20 * startX, 20 * startY, 20 * endX, 20 * endY);
}

//--------------------------------------------------------------
void ofApp::freeMemory() {
    maze.clear();
    lines.clear();
    graph.clear();
    dfs_explored.clear();
    dfs_shortest.clear();
    bfs_explored.clear();
    bfs_shortest.clear();
    graphCoordinates.clear();
}

//--------------------------------------------------------------
void ofApp::dfs(int start, int goal) {
    // stack
    stack<pair<int, vector<int>>> stack;
    stack.push({start, {start}});
    
    dfs_explored.clear();
    dfs_shortest.clear();
    
    set<int> visited;

    while (!stack.empty()) {
        auto [vertex, path] = stack.top();
        stack.pop();

        // record path explored
        if (path.size() > 1) {
            dfs_explored.push_back(path[path.size() - 2]);
            dfs_explored.push_back(path.back());
        }

        // shortest path found
        if (vertex == goal) {
            dfs_shortest = path;
            return;
        }

        // bfs와 비교했을 때 neighbor를 explore하는 방향이 반대임
        
        // (auto neighbor : graph[vertex])는 standard iteration
        // i.e., graph[vertex]가 [1, 2, 3, 4]일 경우 1, 2, 3, 4 순으로 iterate
        
        // (auto it = graph[vertex].rbegin(); it != graph[vertex].rend(); ++it)는 reverse iteration
        // i.e., graph[vertex]가 [1, 2, 3, 4]일 경우 4, 3, 2, 1 순으로 iterate
        
        // 방향을 서로 바꾼 이유는 dfs를 standard iteration으로 할 경우 예시 미로들의 정답을 한번에 찾아버리기 때문
        
        // not yet visited
        if (visited.find(vertex) == visited.end()) {
            // mark as visited
            visited.insert(vertex);
            // explore neighbors
            for (auto it = graph[vertex].rbegin(); it != graph[vertex].rend(); ++it) {
                int neighbor = *it;
                // not yet visited
                if (visited.find(neighbor) == visited.end()) {
                    stack.push({neighbor, path});
                    stack.top().second.push_back(neighbor);
                }
            }
        }
    }
}

//--------------------------------------------------------------
void ofApp::bfs(int start, int goal) {
    // queue
    queue<pair<int, vector<int>>> queue;
    queue.push({start, {start}});
    
    bfs_explored.clear();
    bfs_shortest.clear();
    
    set<int> visited;

    while (!queue.empty()) {
        auto [vertex, path] = queue.front();
        queue.pop();

        // record path explored
        if (path.size() > 1) {
            bfs_explored.push_back(path[path.size() - 2]);
            bfs_explored.push_back(path.back());
        }

        // shortest path found
        if (vertex == goal) {
            bfs_shortest = path;
            return;
        }

        // not yet visited
        if (visited.find(vertex) == visited.end()) {
            // mark as visited
            visited.insert(vertex);
            // explore neighbors
            for (auto neighbor : graph[vertex]) {
                // not yet visited
                if (visited.find(neighbor) == visited.end()) {
                    queue.push({neighbor, path});
                    queue.back().second.push_back(neighbor);
                }
            }
        }
    }
}

//--------------------------------------------------------------
void ofApp::doFullScreen(bool fullscreen) {
    ofSetFullscreen(fullscreen);
}

//--------------------------------------------------------------
void ofApp::updateAnimation() {
    float currentTime = ofGetElapsedTimef();
    float animationDuration = 0.1;

    if (isAnimating) {
        if (currentTime - animationStartTime >= animationDuration) {
            // 여기서 currentExploredIndex 업데이트
            currentExploredIndex += 2;
            animationStartTime = currentTime;

            const auto& explored = dfs_flag ? dfs_explored : bfs_explored;
            // 끝났나?
            if (currentExploredIndex >= explored.size()) {
                isAnimating = false;
                // explored path를 다 그리고 shortest path를 그리기까지 0.2초 딜레이를 주기 위한 flag
                delayActive = true;
                delayStartTime = currentTime;
                currentExploredIndex = explored.size();
            }
        }
    } else if (delayActive) {
        if (currentTime - delayStartTime >= 0.2) {
            delayActive = false;
            showShortestPath = true;
        }
    }
}
