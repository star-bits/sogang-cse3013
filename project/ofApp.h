#pragma once

#include "ofMain.h"
#include <map>
#include <vector>
#include <string>

class ofApp : public ofBaseApp {
public:
    void setup();
    void update();
    void draw();
    void exit();
    
    void keyPressed(int key);
    void keyReleased(int key);
    void mouseMoved(int x, int y);
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void mouseScrolled(int x, int y, float scrollX, float scrollY);
    void mouseEntered(int x, int y);
    void mouseExited(int x, int y);
    void windowResized(int w, int h);
    void dragEvent(ofDragInfo dragInfo);
    void gotMessage(ofMessage msg);

private:
    bool readFile();
    std::map<int, std::vector<int>> parseMaze(const std::string& mazeStr);
    void freeMemory();

    void dfs(int start, int goal);
    void bfs(int start, int goal);

    void MazeLine(float startX, float startY, float endX, float endY);
    void updateAnimation();
    void doFullScreen(bool fullscreen);

    std::vector<std::vector<char>> maze;
    std::vector<std::tuple<float, float, float, float>> lines;
    std::vector<std::vector<std::pair<float, float>>> graphCoordinates;
    std::map<int, std::vector<int>> graph;
    std::vector<int> dfs_shortest;
    std::vector<int> dfs_explored;
    std::vector<int> bfs_shortest;
    std::vector<int> bfs_explored;

    int mazeWidth;
    int currentExploredIndex;
    float animationStartTime;
    bool isAnimating;
    bool showShortestPath;
    bool draw_flag;
    bool showinfo_flag;
    bool fullscreen_flag;
    bool dfs_flag;
    bool bfs_flag;
    bool delayActive;
    float delayStartTime;

    ofTrueTypeFont myFont;
};
