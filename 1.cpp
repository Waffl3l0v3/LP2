// Implement depth first search algorithm and Breadth First Search algorithm, Use an undirected graph
// and develop a recursive algorithm for searching all the vertices of a graph or tree data structure. 

#include <iostream>
#include <vector>
#include <queue>
using namespace std;

class Graph {
    int V; // number of vertices
    vector<vector<int>> adj; // adjacency list

public:
    // Constructor
    Graph(int vertices) {
        if (vertices <= 0) {
            throw invalid_argument("Number of vertices must be positive.");
        }
        V = vertices;
        adj.resize(V);
    }

    // Add edge (undirected)
    void addEdge(int u, int v) {
        if (u < 0 || v < 0 || u >= V || v >= V) {
            cout << "Error: Invalid edge (" << u << ", " << v << ")" << endl;
            return;
        }
        adj[u].push_back(v);
        adj[v].push_back(u);
    }

    // DFS Utility (recursive)
    void DFSUtil(int v, vector<bool> &visited) {
        visited[v] = true;
        cout << v << " ";

        for (int neighbor : adj[v]) {
            if (!visited[neighbor]) {
                DFSUtil(neighbor, visited);
            }
        }
    }

    // DFS traversal (handles disconnected graph)
    void DFS() {
        vector<bool> visited(V, false);

        cout << "DFS Traversal: ";
        for (int i = 0; i < V; i++) {
            if (!visited[i]) {
                DFSUtil(i, visited);
            }
        }
        cout << endl;
    }

    // BFS traversal
    void BFS(int start) {
        if (start < 0 || start >= V) {
            cout << "Error: Invalid starting vertex for BFS." << endl;
            return;
        }

        vector<bool> visited(V, false);
        queue<int> q;

        visited[start] = true;
        q.push(start);

        cout << "BFS Traversal: ";

        while (!q.empty()) {
            int curr = q.front();
            q.pop();
            cout << curr << " ";

            for (int neighbor : adj[curr]) {
                if (!visited[neighbor]) {
                    visited[neighbor] = true;
                    q.push(neighbor);
                }
            }
        }
        cout << endl;
    }
};

// Helper function for robust integer input
int getValidInt() {
    int input;
    while (!(cin >> input)) {
        cout << "Invalid input. Please enter a number: ";
        cin.clear();
        cin.ignore(10000, '\n');
    }
    return input;
}

// Driver code
int main() {
    try {
        int V;
        cout << "Enter total number of vertices for the graph: ";
        V = getValidInt();

        Graph g(V);
        int choice;

        do {
            cout << "\n--- Graph Traversal Menu ---\n";
            cout << "1. Add Edges\n";
            cout << "2. Perform Depth First Search (DFS)\n";
            cout << "3. Perform Breadth First Search (BFS)\n";
            cout << "4. Exit\n";
            cout << "Choose an option: ";
            
            choice = getValidInt();

            switch (choice) {
                case 1: {
                    int E;
                    cout << "\nHow many edges do you want to add? ";
                    E = getValidInt();

                    if (E > 0) {
                        cout << "Enter " << E << " edges (space-separated, e.g. '0 1') using 0 to " << (V - 1) << ":\n";
                        for (int i = 0; i < E; i++) {
                            int u = getValidInt();
                            int v = getValidInt();
                            g.addEdge(u, v);
                        }
                        cout << "Edges added successfully!\n";
                    } else {
                        cout << "No edges added.\n";
                    }
                    break;
                }
                case 2:
                    cout << "\nExecuting DFS...\n";
                    g.DFS();
                    break;
                case 3: {
                    int start;
                    cout << "\nEnter starting vertex for BFS (0 to " << (V - 1) << "): ";
                    start = getValidInt();
                    g.BFS(start);
                    break;
                }
                case 4:
                    cout << "\nExiting program. Goodbye!\n";
                    break;
                default:
                    cout << "\nInvalid choice! Please select an option between 1 and 4.\n";
            }
        } while (choice != 4);

    }
    catch (exception &e) {
        cout << "Exception: " << e.what() << endl;
    }

    return 0;
}