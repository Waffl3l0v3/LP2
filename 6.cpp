#include<bits/stdc++.h>

using namespace std;

const int AIRPORTS = 5;

// Airport Names
string airportNames[AIRPORTS] = {
    "Delhi",
    "Mumbai",
    "Pune",
    "Bangalore",
    "Chennai"
};

// Graph using Adjacency Matrix
int graph[AIRPORTS][AIRPORTS] = {
    // D   M   P   B   C
    {0,  2,  4,  0,  0}, // Delhi
    {2,  0,  1,  7,  0}, // Mumbai
    {4,  1,  0,  3,  5}, // Pune
    {0,  7,  3,  0,  2}, // Bangalore
    {0,  0,  5,  2,  0}  // Chennai
};

// Flight Structure
struct Flight
{
    string flightNo;
    int start;
    int end;
    int shortestDistance;
    int priority;
};

// Store Flights
vector<Flight> flights;

// Dijkstra Shortest Path Algorithm
int dijkstra(int start, int end)
{
    vector<int> dist(AIRPORTS, INT_MAX);
    priority_queue<pair<int,int>, vector<pair<int,int>>, greater<>> pq;

    dist[start] = 0;
    pq.push({0, start});

    while(!pq.empty())
    {
        auto [d, u] = pq.top();
        pq.pop();

        if (d > dist[u]) continue;

        for(int v = 0; v < AIRPORTS; v++)
        {
            if(graph[u][v]) // If there is a direct flight
            {
                int weight = graph[u][v];
                if(dist[u] + weight < dist[v])
                {
                    dist[v] = dist[u] + weight;
                    pq.push({dist[v], v});
                }
            }
        }
    }

    return dist[end];
}

// Stylish Line
void line()
{
    cout << "=========================================================\n";
}

// Show Airports
void showAirports()
{
    cout << "\nAvailable Airports:\n\n";

    for(int i = 0; i < AIRPORTS; i++)
    {
        cout << i << ". " << airportNames[i] << endl;
    }

    line();
}

// Add Flight
void addFlight()
{
    Flight f;
    bool valid;

    do {
        valid = true;
        cout << "\nEnter Flight Number: ";
        cin >> f.flightNo;
        
        for (const auto& flight : flights) {
            if (flight.flightNo == f.flightNo) {
                cout << "Error: Flight number already exists. Please enter a unique flight number.\n";
                valid = false;
                break;
            }
        }
    } while (!valid);

    showAirports();

    do {
        cout << "\nEnter Start Airport Number: ";
        cin >> f.start;
        cout << "Enter Destination Airport Number: ";
        cin >> f.end;
        
        if (f.start == f.end) {
            cout << "Error: Start and Destination airports cannot be the same.\n";
        }
    } while (f.start == f.end);
    
    cout << "Enter Priority (1 = High, 2 = Medium, 3 = Low): ";
    cin >> f.priority;

    // Apply Dijkstra Algorithm
    f.shortestDistance = dijkstra(f.start, f.end);

    flights.push_back(f);

    cout << "\n Flight Added Successfully!\n";
    cout << "Shortest Distance Calculated Using Dijkstra Algorithm: "
         << f.shortestDistance << endl;

    line();
}

// View Flights
void viewFlights()
{
    if(flights.empty())
    {
        cout << "\nNo Flights Available!\n";
        line();
        return;
    }

    cout << "\nFlight Details:\n\n";

    for(int i = 0; i < flights.size(); i++)
    {
        cout << i + 1 << ". Flight: " << flights[i].flightNo
             << " | Route: "
             << airportNames[flights[i].start]
             << " -> "
             << airportNames[flights[i].end]
             << " | Distance: "
             << flights[i].shortestDistance
             << " | Priority: "
             << flights[i].priority
             << endl;
    }

    line();
}

// Schedule by Distance
void scheduleByDistance()
{
    if(flights.empty())
    {
        cout << "\nNo Flights Available For Scheduling!\n";
        line();
        return;
    }

    // Sort by Shortest Distance
    sort(flights.begin(), flights.end(),
         [](Flight a, Flight b)
         {
             return a.shortestDistance < b.shortestDistance;
         });

    cout << "\nApplying Greedy Scheduling by Distance...\n";

    cout << "\nScheduled Flights Order:\n\n";

    for(int i = 0; i < flights.size(); i++)
    {
        cout << i + 1 << ". "
             << flights[i].flightNo
             << " : "
             << airportNames[flights[i].start]
             << " -> "
             << airportNames[flights[i].end]
             << " | Distance = "
             << flights[i].shortestDistance
             << endl;
    }

    line();

    cout << "\nExpert Scheduling Recommendations:\n\n";

    for(int i = 0; i < flights.size(); i++)
    {
        cout << "Flight " << flights[i].flightNo << ": ";

        if(flights[i].shortestDistance <= 3)
        {
            cout << "High efficiency route. Schedule immediately.";
        }
        else if(flights[i].shortestDistance <= 5)
        {
            cout << "Moderate route. Normal scheduling.";
        }
        else
        {
            cout << "Long route. Allocate extra fuel and time.";
        }

        cout << endl;
    }

    line();
}

// Schedule by Priority
void scheduleByPriority()
{
    if(flights.empty())
    {
        cout << "\nNo Flights Available For Scheduling!\n";
        line();
        return;
    }

    // Sort primarily by Priority, secondarily by Distance
    sort(flights.begin(), flights.end(),
         [](Flight a, Flight b)
         {
             if (a.priority == b.priority)
                 return a.shortestDistance < b.shortestDistance;
             return a.priority < b.priority; // Lower number = higher priority
         });

    cout << "\nApplying Scheduling by Priority (Distance as tie-breaker)...\n";

    cout << "\nScheduled Flights Order:\n\n";

    for(int i = 0; i < flights.size(); i++)
    {
        cout << i + 1 << ". "
             << flights[i].flightNo
             << " (Priority " << flights[i].priority << ") : "
             << airportNames[flights[i].start]
             << " -> "
             << airportNames[flights[i].end]
             << " | Distance = "
             << flights[i].shortestDistance
             << endl;
    }

    line();
}

// --- Cargo Loading using Backtracking ---
void solveCargoBacktrack(vector<int>& weights, int capacity, int index, vector<int>& currentLoad, vector<int>& bestLoad, int& maxWeight) {
    int currentWeight = 0;
    for (int w : currentLoad) currentWeight += w;

    if (currentWeight > capacity) return;

    if (currentWeight > maxWeight) {
        maxWeight = currentWeight;
        bestLoad = currentLoad;
    }

    for (int i = index; i < weights.size(); i++) {
        currentLoad.push_back(weights[i]);
        solveCargoBacktrack(weights, capacity, i + 1, currentLoad, bestLoad, maxWeight);
        currentLoad.pop_back(); // Backtrack
    }
}

void cargoLoading()
{
    if (flights.empty()) {
        cout << "\nPlease add a flight first before loading cargo!\n";
        line();
        return;
    }

    cout << "\n--- Cargo Loading (Backtracking) ---\n";
    cout << "Available Flights:\n";
    for(int i = 0; i < flights.size(); i++) {
        cout << "- Flight No: " << flights[i].flightNo << " (" << airportNames[flights[i].start] << " -> " << airportNames[flights[i].end] << ")\n";
    }

    string selectedFlight;
    cout << "\nEnter the Flight Number to load cargo into: ";
    cin >> selectedFlight;

    bool found = false;
    for(const auto& f : flights) {
        if(f.flightNo == selectedFlight) {
            found = true;
            break;
        }
    }

    if (!found) {
        cout << "Error: Flight number '" << selectedFlight << "' not found.\n";
        line();
        return;
    }

    int capacity;
    cout << "Enter Cargo Capacity for Flight " << selectedFlight << ": ";
    cin >> capacity;

    int n;
    cout << "Enter number of cargo items available: ";
    cin >> n;

    if (n <= 0) {
        cout << "No items to load.\n";
        line();
        return;
    }

    vector<int> weights(n);
    cout << "Enter weights of the " << n << " items: ";
    for(int i = 0; i < n; i++) {
        cin >> weights[i];
    }

    vector<int> currentLoad, bestLoad;
    int maxWeight = 0;

    cout << "\nApplying Backtracking to find optimal load for Flight " << selectedFlight << "...\n";
    solveCargoBacktrack(weights, capacity, 0, currentLoad, bestLoad, maxWeight);

    cout << "Best Load Combination for Flight " << selectedFlight << ":\n";
    if (bestLoad.empty()) {
        cout << "None (all items are too heavy)\n";
    } else {
        for (int w : bestLoad) {
            cout << w << " ";
        }
    }
    cout << "\nTotal Loaded Weight: " << maxWeight << " / " << capacity << "\n";
    line();
}

// Main Menu
void menu()
{
    cout << "\n AIRLINE EXPERT SYSTEM MENU\n\n";

    cout << "1. Add Flight\n";
    cout << "2. View Flights\n";
    cout << "3. Schedule by Distance\n";
    cout << "4. Schedule by Priority\n";
    cout << "5. Cargo Loading (Backtracking)\n";
    cout << "6. Exit\n";

    line();
}

// Main Function
int main()
{
    int choice;

    while(true)
    {
        line();
        cout << "      AIRLINE SCHEDULING EXPERT SYSTEM\n";
        cout << " Dijkstra + Greedy + Backtracking Algorithms\n";
        line();

        menu();

        cout << "\nEnter your choice: ";
        cin >> choice;

        switch(choice)
        {
            case 1:
                addFlight();
                break;

            case 2:
                viewFlights();
                break;

            case 3:
                scheduleByDistance();
                break;

            case 4:
                scheduleByPriority();
                break;

            case 5:
                cargoLoading();
                break;

            case 6:
                line();
                cout << "System Closed Successfully\n";
                line();
                return 0;

            default:
                cout << "\nInvalid Choice!\n";
        }
    }

    return 0;
}