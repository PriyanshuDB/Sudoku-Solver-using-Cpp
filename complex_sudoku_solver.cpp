#include <iostream>
#include <vector>
#include <chrono>
#include <tuple>
using namespace std;
#define N 9
//function to print Sudoku board
void printBoard(int board[N][N]) 
{
    for (int row=0;row<N;row++)
    {
        for (int col=0;col<N;col++) 
        {
            cout << board[row][col] << " ";
        }
        cout << endl;
    }
}
//function to check if it is safe to place a number in given cell
bool isSafe(int board[N][N],int row,int col,int num) 
{
    //check row
    for (int x=0;x<N;x++) 
    {
        if (board[row][x]==num) 
        {
            return false;
        }
    }
    //check column
    for (int x=0;x<N;x++) 
    {
        if (board[x][col]==num) 
        {
            return false;
        }
    }
    //check 3x3 sub grid
    int startRow=row-row%3,startCol=col-col%3;
    for (int i=0;i<3;i++)
    {
        for (int j=0;j<3;j++) 
        {
            if (board[i+startRow][j+startCol]==num) 
            {
                return false;
            }
        }
    }
    return true;
}
//function to find cell with minimum remaining values (MRV heuristic)
tuple<int,int> findMinRemainingValues(int board[N][N]) {
    int minCount=N+1;
    tuple<int,int> bestCell=make_tuple(-1,-1);
    for (int row=0;row<N;row++) 
    {
        for (int col=0;col<N;col++) 
        {
            if (board[row][col]==0) 
            {
                int count=0;
                for (int num=1;num<=9;num++) 
                {
                    if (isSafe(board,row,col,num)) 
                    {
                        count++;
                    }
                }
                if (count<minCount) 
                {
                    minCount=count;
                    bestCell=make_tuple(row,col);
                }
            }
        }
    }
    return bestCell;
}
//function to solve sudoku using backtracking
bool solveSudoku(int board[N][N]) 
{
    auto bestCell=findMinRemainingValues(board);
    int row=get<0>(bestCell);
    int col=get<1>(bestCell);
    //if theres no empty cell, we're done
    if (row==-1 && col==-1) {
        return true;
    }
    //try all possible numbers for this empty cell
    for (int num=1;num<=9;num++) 
    {
        if (isSafe(board,row,col,num)) 
        {
            board[row][col] = num;
            //recursively try to solve rest of the board
            if (solveSudoku(board)) 
            {
                return true;
            }
            //if guess doesn't lead to a solution, backtrack
            board[row][col]=0;
        }
    }
    return false;
}
int main() 
{
    //example sudoku puzzle (0 denotes empty cell) - extreme level sudoku taken from website : https://www.extremesudoku.info/
    int board[N][N]=
    {
        {0, 0, 7, 0, 0, 6, 0, 0, 3},
        {0, 4, 0, 0, 7, 0, 0, 9, 0},
        {9, 0, 0, 2, 0, 0, 4, 0, 0},
        {1, 0, 0, 7, 0, 0, 9, 0, 0},
        {0, 6, 0, 0, 8, 0, 0, 2, 0},
        {0, 0, 4, 0, 0, 1, 0, 0, 8},
        {0, 0, 6, 0, 0, 9, 0, 0, 1},
        {0, 7, 0, 0, 4, 0, 0, 5, 0},
        {2, 0, 0, 5, 0, 0, 3, 0, 0}
    };
    auto start=chrono::high_resolution_clock::now();
    if (solveSudoku(board)) 
    {
        printBoard(board);
    } 
    else 
    {
        cout << "No solution exists" << endl;
    }
    auto end=chrono::high_resolution_clock::now();
    chrono::duration<double> duration=end-start;
    cout << "Time taken: " << duration.count() << " seconds" << endl;
    return 0;
}
