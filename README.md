# SudokuCSP

Sudoku as a Constraint Problem. Made for CS486 (Artificial Intelligence) at the University of Waterloo in Winter 2015. 

## Problem

Sudoku is a simple game of deduction, usually played on a 9x9 grid. In the goal state, each number from 1 to 9 appears exactly once in each row and column on the grid. Additionally, the grid is subdivided into 9 3x3 non-overlapping subgrids, and each number must appear exactly once in each subgrid. A Sudoku puzzle usually starts with some numbers filled in, so that there is a single unique solution.

## Algorithm

* Uses forward checking and backtracking 
  * Forward Checking: Check if a potential move results in an impossible board
  * Backtracking: Essentially DFS
* Heuristics: Most Restricted Variable (MRV) and Least Constraining Value (LCV), with Most Constraining Variable (MCV) heuristic as a tie breaker
  * MRV: Pick the cell with the least number of options to fill next
  * LCV: Pick the value that imposes the least number of constraints on the cell's neighbors
  * MCV: 

## Input

Each file contains 9 rows of exactly 9 numbers. Numbers within a line are separated by whitespace. The value 0 is used to indicate a blank space in the grid

## Possible Enhancements

* Create a front-end for better usability 
