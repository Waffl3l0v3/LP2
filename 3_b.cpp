// Implement a solution for a Constraint Satisfaction Problem using Branch and Bound and
// Backtracking for a graph coloring problem

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
            cout << "Invalid edge: " << u + 1 << " " << v + 1 << "\n";
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
    void backtracking(int m, int v, vector<int>& color, bool& foundAny) {
        if (v == V) {
            cout << "    [+] Outcome (Success): [ ";
            for (int c : color) cout << c << " ";
            cout << "]\n";
            foundAny = true;
            return;
        }

        bool deadEnd = true;
        for (int c = 1; c <= m; c++) {
            if (isSafe(v, color, c)) {
                deadEnd = false;
                color[v] = c;
                backtracking(m, v + 1, color, foundAny);
                color[v] = -1; // Backtrack
            }
        }

        if (deadEnd) {
            cout << "    [-] Outcome (Failure): [ ";
            for (int c : color) cout << c << " ";
            cout << "]\n";
        }
    }

    void solveBacktracking() {
        int m;
        cout << "Enter number of colors to check (M): ";
        cin >> m;

        cout << "\n--- Exploring Backtracking Space ---\n";
        vector<int> color(V, -1);
        bool foundAny = false;
        
        backtracking(m, 0, color, foundAny);

        if (foundAny) {
            cout << "\n[+] At least one solution exists with " << m << " colors.\n";
        } else {
            cout << "\n[-] No solution exists with " << m << " colors.\n";
        }
    }

    // --- Branch and Bound ---
    void branchAndBound(int v, vector<int>& color, int current_max_color, int& min_colors, vector<int>& best_color) {
        if (v == V) {
            if (current_max_color < min_colors) {
                min_colors = current_max_color;
                best_color = color;
            }
            cout << "    [+] Outcome (Valid Coloring): [ ";
            for (int c : color) cout << c << " ";
            cout << "] -> Colors used: " << current_max_color << "\n";
            return;
        }

        bool deadEnd = true;
        // Optimization: Try colors up to current_max_color + 1 to break symmetry
        for (int c = 1; c <= current_max_color + 1; c++) {
            int new_max_color = max(current_max_color, c);
            
            // Prune branches that exceed or equal the minimum colors found so far
            if (new_max_color >= min_colors) {
                // We can print pruned path if desired, or just skip
                continue; 
            }

            if (isSafe(v, color, c)) {
                deadEnd = false;
                color[v] = c;
                branchAndBound(v + 1, color, new_max_color, min_colors, best_color);
                color[v] = -1; // Backtrack
            }
        }

        if (deadEnd) {
            cout << "    [-] Outcome (Pruned/Failure): [ ";
            for (int c : color) cout << c << " ";
            cout << "]\n";
        }
    }

    void solveBranchAndBound() {
        cout << "\n--- Exploring Branch & Bound Space ---\n";
        vector<int> color(V, -1);
        vector<int> best_color(V, -1);
        int min_colors = V + 1; // Start with worst-case upper bound

        branchAndBound(0, color, 0, min_colors, best_color);

        if (min_colors <= V) {
            cout << "\n[+] Optimal Chromatic Number is " << min_colors << " colors.\n";
            cout << "Best Color Assignment: [ ";
            for (int c : best_color) cout << c << " ";
            cout << "]\n";
        } else {
            cout << "\n[-] No solution exists.\n";
        }
    }
};

int main() {
    int V, E;
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
        cout << "Enter edges (u v) [1-based indexing, e.g., 1 2]:\n";
        for (int i = 0; i < E; i++) {
            int u, v;
            cin >> u >> v;
            // Convert to 0-based internally
            g.addEdge(u - 1, v - 1);
        }
    }

    int choice;
    do {
        cout << "\n------------- MENU -------------\n";
        cout << "1. Check M-Coloring (Backtracking)\n";
        cout << "2. Find Minimum Colors (Branch & Bound)\n";
        cout << "3. Exit\n";
        cout << "--------------------------------\n";
        cout << "Choose an option: ";
        cin >> choice;

        switch (choice) {
            case 1:
                g.solveBacktracking();
                break;
            case 2:
                g.solveBranchAndBound();
                break;
            case 3:
                break;
            default:
                cout << "Invalid choice. Please try again.\n";
        }
    } while (choice != 3);

    return 0;
}