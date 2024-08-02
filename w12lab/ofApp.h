#pragma once

#include "ofMain.h"
#include <vector>
#include <tuple>
#include <map>

class ofApp : public ofBaseApp {
public:
    void setup();
    void update();
    void draw();
    void exit();

    void keyPressed(int key);
    void keyReleased(int key);
    void mouseMoved(int x, int y );
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void mouseScrolled(int x, int y, float scrollX, float scrollY);
    void mouseEntered(int x, int y);
    void mouseExited(int x, int y);
    void windowResized(int w, int h);
    void gotMessage(ofMessage msg);
    void dragEvent(ofDragInfo dragInfo);

    bool readFile();
    void MazeLine(float startX, float startY, float endX, float endY);
    void freeMemory();
    int dfs();
    int bfs();
    void doFullScreen(bool fullscreen);

private:
    bool draw_flag;
    bool showinfo_flag;
    bool fullscreen_flag;
    bool bfs_flag;
    bool dfs_flag;
    ofTrueTypeFont myFont;
    std::vector<std::vector<char>> maze;
    std::vector<std::tuple<float, float, float, float>> lines;

    std::map<int, std::vector<int>> parseMaze(const std::string& mazeStr);
    std::map<int, std::vector<int>> graph;
};
