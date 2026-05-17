// Graph Coloring using Backtracking and Branch & Bound
#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

class Graph {
    int V;
    vector<vector<int>> adj;

public:
    Graph(int V) : V(V) {
        adj.assign(V, vector<int>(V, 0));
    }

    void addEdge(int u, int v) {
        if (u >= 0 && u < V && v >= 0 && v < V) {
            adj[u][v] = 1;
            adj[v][u] = 1;
        } else {
            cout << "Invalid edge: " << u << " " << v << "\n";
        }
    }

    bool isSafe(int v, const vector<int>& color, int c) {
        for (int i = 0; i < V; i++) {
            if (adj[v][i] && color[i] == c) {
                return false;
            }
        }
        return true;
    }

    // --- Backtracking ---
    // Decision problem: Can we color the graph with exactly 'm' colors?
    bool backtracking(int m, int v, vector<int>& color) {
        if (v == V) return true;

        for (int c = 1; c <= m; c++) {
            if (isSafe(v, color, c)) {
                color[v] = c;
                if (backtracking(m, v + 1, color)) return true;
                color[v] = 0; // Backtrack
            }
        }
        return false;
    }

    void solveBacktracking() {
        int m;
        cout << "\nEnter the number of colors (m): ";
        cin >> m;

        vector<int> color(V, 0);
        if (backtracking(m, 0, color)) {
            cout << "\n[SUCCESS] Graph can be colored using " << m << " colors.\n";
            printColors(color);
        } else {
            cout << "\n[FAILURE] Graph CANNOT be colored using " << m << " colors.\n";
        }
    }

    // --- Branch and Bound ---
    // Optimization problem: Find the minimum number of colors needed (Chromatic Number)
    void branchAndBound(int v, vector<int>& color, int current_max_color, int& min_colors, vector<int>& best_color) {
        if (v == V) {
            if (current_max_color < min_colors) {
                min_colors = current_max_color;
                best_color = color;
            }
            return;
        }

        // Optimization (Symmetry Breaking): Colors are symmetric, so we only need to try up to (current_max_color + 1)
        // Trying color 3 when we've only used color 1 is redundant (same as trying color 2).
        for (int c = 1; c <= current_max_color + 1; c++) {
            // Bounding condition: if using this color equals or exceeds the minimum colors found so far, prune!
            int new_max_color = max(current_max_color, c);
            if (new_max_color >= min_colors) {
                continue; // Prune this branch
            }

            if (isSafe(v, color, c)) {
                color[v] = c;
                branchAndBound(v + 1, color, new_max_color, min_colors, best_color);
                color[v] = 0; // Backtrack
            }
        }
    }

    void solveBranchAndBound() {
        vector<int> color(V, 0);
        vector<int> best_color(V, 0);
        int min_colors = V + 1; // Start with an upper bound worse than worst-case (V colors)

        branchAndBound(0, color, 0, min_colors, best_color);

        cout << "\n[OPTIMAL] Minimum colors needed (Chromatic Number): " << min_colors << "\n";
        printColors(best_color);
    }

    void printColors(const vector<int>& color) {
        cout << "Vertex Colors:\n";
        for (int i = 0; i < V; i++) {
            cout << "  Vertex " << i << " -> Color " << color[i] << "\n";
        }
    }
};

int main() {
    int V, E;
    cout << "===========================\n";
    cout << "      GRAPH COLORING\n";
    cout << "===========================\n";
    cout << "Enter number of vertices: ";
    cin >> V;
    
    if (V <= 0) {
        cout << "Number of vertices must be greater than 0.\n";
        return 0;
    }

    Graph g(V);

    cout << "Enter number of edges: ";
    cin >> E;

    if (E > 0) {
        cout << "Enter " << E << " edges as pairs (u v) [0 to " << V - 1 << "]:\n";
        for (int i = 0; i < E; i++) {
            int u, v;
            cin >> u >> v;
            g.addEdge(u, v);
        }
    }

    int choice;
    do {
        cout << "\n----------- MENU -----------\n";
        cout << "1. Backtracking (Check if m colors are enough)\n";
        cout << "2. Branch and Bound (Find Chromatic Number - min colors)\n";
        cout << "3. Exit\n";
        cout << "----------------------------\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1:
                g.solveBacktracking();
                break;
            case 2:
                g.solveBranchAndBound();
                break;
            case 3:
                cout << "Exiting...\n";
                break;
            default:
                cout << "Invalid choice. Please try again.\n";
        }
    } while (choice != 3);

    return 0;
}