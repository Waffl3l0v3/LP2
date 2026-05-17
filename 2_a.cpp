// Implement A star Algorithm for any game search problem. 
// A* for 8-Puzzle Problem

#include <iostream>
#include <vector>
#include <queue>
#include <set>
#include <string>
#include <cmath>
using namespace std;

struct Node {
    vector<vector<int>> state;
    int g, h;
    int x, y; // blank position
    string moves;

    Node(vector<vector<int>> s, int g, int h, int x, int y, string m = "")
        : state(s), g(g), h(h), x(x), y(y), moves(m) {}

    int f() const {
        return g + h;
    }
};

// Comparator for priority queue
struct Compare {
    bool operator()(Node const& a, Node const& b) {
        return a.f() > b.f();
    }
};

// Goal state
vector<vector<int>> goal = {
    {1,2,3},
    {4,5,6},
    {7,8,0}
};

// Manhattan Distance heuristic
int calculateHeuristic(vector<vector<int>> &state) {
    int h = 0;
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (state[i][j] != 0) {
                int val = state[i][j];
                int goalX = (val - 1) / 3;
                int goalY = (val - 1) % 3;
                h += abs(i - goalX) + abs(j - goalY);
            }
        }
    }
    return h;
}

// Convert state to string for visited set
string stateToString(vector<vector<int>> &state) {
    string s = "";
    for (auto &row : state)
        for (int val : row)
            s += to_string(val);
    return s;
}

bool isValid(int x, int y) {
    return x >= 0 && y >= 0 && x < 3 && y < 3;
}

void solvePuzzle(vector<vector<int>> start, int blankX, int blankY) {
    priority_queue<Node, vector<Node>, Compare> pq;
    set<string> visited;

    int h = calculateHeuristic(start);
    pq.push(Node(start, 0, h, blankX, blankY));

    int dx[] = {1, -1, 0, 0};
    int dy[] = {0, 0, 1, -1};

    while (!pq.empty()) {
        Node current = pq.top();
        pq.pop();

        string key = stateToString(current.state);

        if (visited.count(key)) continue;
        visited.insert(key);

        if (current.state == goal) {
            cout << "\n--- Optimal Solution Path ---\n";
            vector<vector<int>> currState = start;
            int px = blankX, py = blankY;
            int step = 0;
            
            cout << "Step 0 (Start): g=0, h=" << calculateHeuristic(currState) << ", f=" << calculateHeuristic(currState) << "\n";
            for(int r=0; r<3; r++) { for(int c=0; c<3; c++) cout << currState[r][c] << " "; cout << "\n"; }
            cout << "------------\n";

            for (char m : current.moves) {
                int nx = px, ny = py;
                if (m == 'D') nx++;
                else if (m == 'U') nx--;
                else if (m == 'R') ny++;
                else if (m == 'L') ny--;
                
                swap(currState[px][py], currState[nx][ny]);
                px = nx; py = ny;
                step++;
                
                int currentH = calculateHeuristic(currState);
                cout << "Step " << step << " (Move " << m << "): g=" << step << ", h=" << currentH << ", f=" << step+currentH << "\n";
                for(int r=0; r<3; r++) { for(int c=0; c<3; c++) cout << currState[r][c] << " "; cout << "\n"; }
                cout << "------------\n";
            }

            cout << "Solved with total cost: " << current.g << endl;
            return;
        }

        for (int i = 0; i < 4; i++) {
            int nx = current.x + dx[i];
            int ny = current.y + dy[i];

            if (isValid(nx, ny)) {
                vector<vector<int>> newState = current.state;
                swap(newState[current.x][current.y], newState[nx][ny]);

                int newH = calculateHeuristic(newState);

                char moveDir;
                if (i == 0) moveDir = 'D'; // dx=1, dy=0
                else if (i == 1) moveDir = 'U'; // dx=-1, dy=0
                else if (i == 2) moveDir = 'R'; // dx=0, dy=1
                else moveDir = 'L'; // dx=0, dy=-1

                pq.push(Node(newState, current.g + 1, newH, nx, ny, current.moves + moveDir));
            }
        }
    }

    cout << "No solution found." << endl;
}

int main() {
    vector<vector<int>> start(3, vector<int>(3));
    int blankX = -1, blankY = -1;

    cout << "Enter 8-puzzle (0 for blank):\n";

    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            cin >> start[i][j];

            if (start[i][j] < 0 || start[i][j] > 8) {
                cout << "Invalid input. Use numbers 0-8.\n";
                return 0;
            }

            if (start[i][j] == 0) {
                blankX = i;
                blankY = j;
            }
        }
    }

    if (blankX == -1) {
        cout << "Error: No blank space (0) found.\n";
        return 0;
    }

    solvePuzzle(start, blankX, blankY);
    return 0;
}