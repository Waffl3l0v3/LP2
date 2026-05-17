// “Smart Logistics Network”
// You are building a delivery network system for a company:
// 🧩 Problems mapped:
// Kruskal (MST) → Build cheapest network connecting all warehouses
// Dijkstra (Shortest Path) → Find shortest delivery route from a warehouse
// Job Scheduling → Schedule deliveries to maximize profit

#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
#include <limits>
using namespace std;

// Helper function for robust integer input
int getValidInt() {
    int input;
    while (!(cin >> input)) {
        cout << "Invalid input. Please enter a valid number: ";
        cin.clear();
        cin.ignore(10000, '\n');
    }
    return input;
}

/////////////////////// 1. KRUSKAL & DIJKSTRA (NETWORK) ///////////////////////
struct Edge {
    int u, v, w;
};

bool cmpEdge(Edge a, Edge b) {
    return a.w < b.w;
}

class DSU {
    vector<int> parent;
public:
    DSU(int n) {
        parent.resize(n);
        for (int i = 0; i < n; i++) parent[i] = i;
    }
    int find(int x) {
        if (parent[x] != x)
            parent[x] = find(parent[x]);
        return parent[x];
    }
    void unite(int a, int b) {
        parent[find(a)] = find(b);
    }
};

class LogisticsNetwork {
    int V, E;
    vector<Edge> edges;
    vector<vector<pair<int, int>>> adj;
    bool isSetup;

    bool validVertex(int v) {
        return v >= 0 && v < V;
    }

public:
    LogisticsNetwork() : V(0), E(0), isSetup(false) {}

    void setupNetwork() {
        cout << "\nEnter total number of warehouses (V): ";
        V = getValidInt();
        
        if (V <= 0) {
            cout << "[-] Invalid number of warehouses.\n";
            V = 0;
            return;
        }

        cout << "Enter total number of roads connecting them (E): ";
        E = getValidInt();

        if (E < 0) {
            cout << "[-] Invalid number of roads.\n";
            E = 0;
            V = 0;
            return;
        }

        edges.clear();
        adj.assign(V, vector<pair<int, int>>());

        if (E > 0) {
            cout << "\nEnter " << E << " roads in format (warehouse1 warehouse2 distance):\n";
            for (int i = 0; i < E; i++) {
                int u, v, w;
                cout << "Road " << i + 1 << ": ";
                u = getValidInt();
                v = getValidInt();
                w = getValidInt();

                if (!validVertex(u) || !validVertex(v) || w < 0) {
                    cout << "[-] Invalid input. Road discarded. Please start over.\n";
                    edges.clear();
                    adj.clear();
                    isSetup = false;
                    return;
                }

                edges.push_back({u, v, w});
                adj[u].push_back({v, w});
                adj[v].push_back({u, w}); // Undirected
            }
        }
        isSetup = true;
        cout << "[+] Logistics Network successfully configured!\n";
    }

    bool ready() {
        if (!isSetup) {
            cout << "[-] Error: Network is not configured. Please run 'Setup Network' first.\n";
        }
        return isSetup;
    }

    void kruskalMST() {
        if (!ready()) return;

        if (edges.empty()) {
            cout << "[-] Network has no roads. Cost is 0.\n";
            return;
        }

        vector<Edge> sortedEdges = edges;
        sort(sortedEdges.begin(), sortedEdges.end(), cmpEdge);

        DSU dsu(V);
        int total = 0;

        cout << "\n--- Minimum Cost Network (MST) ---\n";
        for (auto &e : sortedEdges) {
            if (dsu.find(e.u) != dsu.find(e.v)) {
                dsu.unite(e.u, e.v);
                cout << "Connect Warehouse " << e.u << " to " << e.v << " | Cost: " << e.w << endl;
                total += e.w;
            }
        }
        cout << "----------------------------------\n";
        cout << "Total Minimum Cost: " << total << "\n";
    }

    void dijkstraShortestPath() {
        if (!ready()) return;

        int src;
        cout << "\nEnter starting warehouse (0 to " << V - 1 << "): ";
        src = getValidInt();

        if (!validVertex(src)) {
            cout << "[-] Invalid source warehouse.\n";
            return;
        }

        vector<int> dist(V, 1e9);
        priority_queue<pair<int,int>, vector<pair<int,int>>, greater<>> pq;

        dist[src] = 0;
        pq.push({0, src});

        while (!pq.empty()) {
            auto [d, u] = pq.top(); pq.pop();

            if (d > dist[u]) continue;

            for (auto &p : adj[u]) {
                int v = p.first, w = p.second;
                if (dist[u] + w < dist[v]) {
                    dist[v] = dist[u] + w;
                    pq.push({dist[v], v});
                }
            }
        }

        cout << "\n--- Shortest Routes from Warehouse " << src << " ---\n";
        for (int i = 0; i < V; i++) {
            if (dist[i] == 1e9) cout << "To Warehouse " << i << ": UNREACHABLE\n";
            else cout << "To Warehouse " << i << ": " << dist[i] << " units\n";
        }
        cout << "----------------------------------------\n";
    }
};

/////////////////////// 2. JOB SCHEDULING ///////////////////////
struct Job {
    int id, deadline, profit;
};

bool cmpJob(Job a, Job b) {
    return a.profit > b.profit;
}

void jobScheduling() {
    int n;
    cout << "\nEnter number of delivery jobs: ";
    n = getValidInt();

    if (n <= 0) {
        cout << "[-] Invalid input\n"; return;
    }

    vector<Job> jobs(n);

    cout << "Enter details for " << n << " jobs (ID Deadline Profit):\n";
    for (int i = 0; i < n; i++) {
        cout << "Job " << i + 1 << ": ";
        jobs[i].id = getValidInt();
        jobs[i].deadline = getValidInt();
        jobs[i].profit = getValidInt();

        if (jobs[i].deadline <= 0 || jobs[i].profit < 0) {
            cout << "[-] Invalid job details.\n"; return;
        }
    }

    sort(jobs.begin(), jobs.end(), cmpJob);

    int maxD = 0;
    for (auto &j : jobs)
        maxD = max(maxD, j.deadline);

    vector<int> slot(maxD+1, -1);
    int profit = 0;
    int scheduledCount = 0;

    for (auto &job : jobs) {
        for (int t = job.deadline; t > 0; t--) {
            if (slot[t] == -1) {
                slot[t] = job.id;
                profit += job.profit;
                scheduledCount++;
                break;
            }
        }
    }

    cout << "\n--- Scheduled Deliveries ---\n";
    if (scheduledCount == 0) {
        cout << "No jobs could be scheduled.\n";
    } else {
        for (int i = 1; i <= maxD; i++) {
            if (slot[i] != -1) {
                cout << "Time Slot " << i << ": Deliver Job " << slot[i] << endl;
            }
        }
    }
    cout << "----------------------------\n";
    cout << "Total Profit Maximum: " << profit << "\n";
}

/////////////////////// MAIN ///////////////////////
int main() {
    LogisticsNetwork network;
    int choice;

    do {
        cout << "\n===== SMART LOGISTICS SYSTEM =====\n";
        cout << "1. Setup Network (Warehouses & Roads)\n";
        cout << "2. Build Minimum Cost Network (Kruskal's)\n";
        cout << "3. Find Shortest Routes (Dijkstra's)\n";
        cout << "4. Schedule Deliveries (Job Scheduling)\n";
        cout << "0. Exit\n";
        cout << "----------------------------------\n";
        cout << "Enter choice: ";
        
        choice = getValidInt();

        switch(choice) {
            case 1: 
                network.setupNetwork(); 
                break;
            case 2: 
                network.kruskalMST(); 
                break;
            case 3: 
                network.dijkstraShortestPath(); 
                break;
            case 4: 
                jobScheduling(); 
                break;
            case 0: 
                cout << "Exiting Smart Logistics System...\n"; 
                break;
            default: 
                cout << "[-] Invalid choice. Please try again.\n";
        }

    } while (choice != 0);

    return 0;
}