// Implement a solution for a Constraint Satisfaction Problem using Branch and Bound and
// Backtracking for n-queens problem

#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

class NQueens {
    int N;
    vector<vector<int>> board;
    int solutionsCount;

    // Branch & Bound arrays for O(1) safety checks
    vector<bool> col_arr, diag1, diag2;

public:
    NQueens(int n) {
        setN(n);
    }

    void setN(int n) {
        if (n <= 0) {
            cout << "[-] N must be greater than 0.\n";
            return;
        }
        N = n;
        board.assign(N, vector<int>(N, 0));
        col_arr.assign(N, false);
        diag1.assign(2 * N - 1, false);
        diag2.assign(2 * N - 1, false);
        solutionsCount = 0;
    }

    void resetBoard() {
        for (int i = 0; i < N; i++) {
            for (int j = 0; j < N; j++) {
                board[i][j] = 0;
            }
        }
        fill(col_arr.begin(), col_arr.end(), false);
        fill(diag1.begin(), diag1.end(), false);
        fill(diag2.begin(), diag2.end(), false);
        solutionsCount = 0;
    }

    void printBoard() {
        solutionsCount++;
        cout << "Solution " << solutionsCount << ":\n";
        for (const auto& row : board) {
            for (int val : row) {
                if (val == 1) cout << "Q ";
                else cout << ". ";
            }
            cout << "\n";
        }
        cout << "-----------------\n";
    }

    // ==========================================
    // --- Backtracking Implementation ---
    // ==========================================
    bool isSafeBT(int row, int col) {
        // Check column
        for (int i = 0; i < row; i++)
            if (board[i][col] == 1) return false;
        
        // Check upper left diagonal
        for (int i = row, j = col; i >= 0 && j >= 0; i--, j--)
            if (board[i][j] == 1) return false;
        
        // Check upper right diagonal
        for (int i = row, j = col; i >= 0 && j < N; i--, j++)
            if (board[i][j] == 1) return false;
            
        return true;
    }

    void solveBT(int row) {
        // Base case: All queens placed
        if (row == N) {
            printBoard();
            return; // Don't stop, find all solutions
        }

        for (int c = 0; c < N; c++) {
            if (isSafeBT(row, c)) {
                board[row][c] = 1;
                solveBT(row + 1);
                board[row][c] = 0; // Backtrack
            }
        }
    }

    void runBacktracking() {
        if (N <= 0) return;
        resetBoard();
        cout << "\n--- Exploring Space using Backtracking ---\n";
        solveBT(0);
        
        if (solutionsCount == 0) {
            cout << "[-] No solution exists for N = " << N << "\n";
        } else {
            cout << "[+] Total Solutions Found: " << solutionsCount << "\n";
        }
    }

    // ==========================================
    // --- Branch and Bound Implementation ---
    // ==========================================
    void solveBB(int row) {
        // Base case: All queens placed
        if (row == N) {
            printBoard();
            return; // Don't stop, find all solutions
        }

        for (int c = 0; c < N; c++) {
            // Check bounding conditions in O(1)
            if (col_arr[c] || diag1[row + c] || diag2[row - c + N - 1])
                continue; // Prune branch

            // Place queen
            board[row][c] = 1;
            col_arr[c] = diag1[row + c] = diag2[row - c + N - 1] = true;

            solveBB(row + 1);

            // Backtrack
            board[row][c] = 0;
            col_arr[c] = diag1[row + c] = diag2[row - c + N - 1] = false;
        }
    }

    void runBranchAndBound() {
        if (N <= 0) return;
        resetBoard();
        cout << "\n--- Exploring Space using Branch & Bound ---\n";
        solveBB(0);
        
        if (solutionsCount == 0) {
            cout << "[-] No solution exists for N = " << N << "\n";
        } else {
            cout << "[+] Total Solutions Found: " << solutionsCount << "\n";
        }
    }
};

int main() {
    int N;
    cout << "Enter value of N for N-Queens: ";
    cin >> N;

    if (N <= 0) {
        cout << "N must be greater than 0.\n";
        return 0;
    }

    NQueens nq(N);
    int choice;

    do {
        cout << "\n------------- MENU -------------\n";
        cout << "1. Solve using Backtracking\n";
        cout << "2. Solve using Branch & Bound\n";
        cout << "3. Change value of N (Current: " << N << ")\n";
        cout << "4. Exit\n";
        cout << "--------------------------------\n";
        cout << "Choose an option: ";
        cin >> choice;

        switch (choice) {
            case 1:
                nq.runBacktracking();
                break;
            case 2:
                nq.runBranchAndBound();
                break;
            case 3:
                cout << "Enter new value of N: ";
                cin >> N;
                nq.setN(N);
                break;
            case 4:
                break;
            default:
                cout << "Invalid choice. Please try again.\n";
        }
    } while (choice != 4);

    return 0;
}