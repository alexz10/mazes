/* Your code here! */
#pragma once

#include "cs225/HSLAPixel.h"
#include "cs225/PNG.h"
#include "dsets.h"

#include <vector>
#include <map>
#include <stack>
#include <algorithm>
#include <cstdlib>

using namespace std;
using namespace cs225;

class SquareMaze {
   public:
    // constructor
    SquareMaze();
    // destructor
    ~SquareMaze();

    /* Makes a new SquareMaze of the given height and width.

    If this object already represents a maze it will clear all the existing data
    before doing so. You will start with a square grid (like graph paper) with the
    specified height and width. You will select random walls to delete without
    creating a cycle, until there are no more walls that could be deleted without
    creating a cycle. Do not delete walls on the perimeter of the grid.

    Hints: You only need to store 2 bits per square: the "down" and "right" walls.
    The finished maze is always a tree of corridors.) */
    void makeMaze(int width, int height);

    /* This uses your representation of the maze to determine whether it is
     * possible to travel in the given direction from the square at coordinates
     * (x,y). */
    bool canTravel(int x, int y, int dir) const;

    /* Sets whether or not the specified wall exists. */
    void setWall(int x, int y, int dir, bool exists);

    /* Solves this SquareMaze.

    For each square on the bottom row (maximum y coordinate), there is a
    distance from the origin (i.e. the top-left cell), which is defined as the
    length (measured as a number of steps) of the only path through the maze
    from the origin to that square.

    Select the square in the bottom row with the largest distance from the
    origin as the destination of the maze. solveMaze() returns the winning path
    from the origin to the destination as a vector of integers, where each
    integer represents the direction of a step, using the same encoding as in
    canTravel().

    If multiple paths of maximum length exist, use the one with the destination
    cell that has the smallest x value. */
    vector<int> solveMaze();

    /*Draws the maze without the solution. */
    PNG* drawMaze() const;

    /* This function calls drawMaze, then solveMaze; it modifies the PNG from
     * drawMaze to show the solution vector and the exit. */
    PNG* drawMazeWithSolution();

    //additional helper function(s)
    

   private:
    int width_;
    int height_;

    vector<bool> wallRight_;
    vector<bool> wallDown_;
    DisjointSets cells_;
};
