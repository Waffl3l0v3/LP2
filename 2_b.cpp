// Implement A star Algorithm for any game search problem. 
// A* for Grid Pathfinding (Game Map)

#include <iostream>
#include <vector>
#include <queue>
#include <cmath>
using namespace std;

struct Cell {
    int x, y;
    int g, h;

    Cell(int x, int y, int g, int h)
        : x(x), y(y), g(g), h(h) {}

    int f() const {
        return g + h;
    }
};

struct Compare {
    bool operator()(Cell const& a, Cell const& b) {
        return a.f() > b.f();
    }
};

bool isValid(int x, int y, int n, int m) {
    return (x >= 0 && y >= 0 && x < n && y < m);
}

bool isUnBlocked(vector<vector<int>> &grid, int x, int y) {
    return grid[x][y] == 0;
}

int heuristic(int x, int y, int goalX, int goalY) {
    return abs(x - goalX) + abs(y - goalY);
}

void aStar(vector<vector<int>> &grid, pair<int,int> start, pair<int,int> goal) {
    int n = grid.size();
    int m = grid[0].size();

    if (!isValid(start.first, start.second, n, m) ||
        !isValid(goal.first, goal.second, n, m)) {
        cout << "Invalid start or goal position.\n";
        return;
    }

    if (!isUnBlocked(grid, start.first, start.second) ||
        !isUnBlocked(grid, goal.first, goal.second)) {
        cout << "Start or goal is blocked.\n";
        return;
    }

    priority_queue<Cell, vector<Cell>, Compare> pq;
    vector<vector<bool>> visited(n, vector<bool>(m, false));

    int start_h = heuristic(start.first, start.second, goal.first, goal.second);
    cout << "--- Starting A* Search ---\n";
    cout << "Start Node (" << start.first << ", " << start.second << "): g=0, h=" 
         << start_h << ", f=" << start_h << endl;
    
    pq.push(Cell(start.first, start.second, 0, start_h));

    int dx[] = {1, -1, 0, 0};
    int dy[] = {0, 0, 1, -1};

    while (!pq.empty()) {
        Cell curr = pq.top();
        pq.pop();

        if (visited[curr.x][curr.y]) continue;
        visited[curr.x][curr.y] = true;

        cout << "\nEvaluating Node (" << curr.x << ", " << curr.y << ") -> g=" << curr.g 
             << ", h=" << curr.h << ", f=" << curr.f() << endl;

        if (curr.x == goal.first && curr.y == goal.second) {
            cout << "Reached goal with cost: " << curr.g << endl;
            return;
        }

        for (int i = 0; i < 4; i++) {
            int nx = curr.x + dx[i];
            int ny = curr.y + dy[i];

            if (isValid(nx, ny, n, m) &&
                isUnBlocked(grid, nx, ny) &&
                !visited[nx][ny]) {

                int neighbor_g = curr.g + 1;
                int neighbor_h = heuristic(nx, ny, goal.first, goal.second);
                int neighbor_f = neighbor_g + neighbor_h;
                
                cout << "  -> Found neighbor (" << nx << ", " << ny << "): g=" << neighbor_g 
                     << ", h=" << neighbor_h << ", f=" << neighbor_f << endl;

                pq.push(Cell(nx, ny, neighbor_g, neighbor_h));
            }
        }
    }

    cout << "No path found.\n";
}

int main() {
    int n, m;
    cout << "Enter grid size (n m): ";
    cin >> n >> m;

    if (n <= 0 || m <= 0) {
        cout << "Invalid grid size.\n";
        return 0;
    }

    vector<vector<int>> grid(n, vector<int>(m));

    cout << "Enter grid (0=free, 1=blocked):\n";
    for (int i = 0; i < n; i++)
        for (int j = 0; j < m; j++)
            cin >> grid[i][j];

    pair<int,int> start, goal;

    cout << "Enter start (x y): ";
    cin >> start.first >> start.second;

    cout << "Enter goal (x y): ";
    cin >> goal.first >> goal.second;

    aStar(grid, start, goal);
    return 0;
}