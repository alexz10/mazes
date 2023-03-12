/* Your code here! */
#include "maze.h"

using namespace std;
using namespace cs225;

SquareMaze::SquareMaze() {}

SquareMaze::~SquareMaze() {}

void SquareMaze::makeMaze(int width, int height) {
    width_ = width;
    height_ = height;

    int cells = width * height;

    cells_.addelements(cells);

    wallRight_.resize(cells, true);
    wallDown_.resize(cells, true);

    // random fill
    int counter = 1;
    while (counter < cells) {
        int randX = rand() % width;
        int randY = rand() % height;

        // calculate first cell
        int cell1 = (width * randY) + randX;

        // randomly choose wall, 0 is right, 1 is down
        int randWall = rand() % 2;

        if (randWall == 0) {          // break right
            if (randX < width - 1) {  // check that it is not on the right edge
                if (wallRight_[cell1] == true) {  // check that there is a wall
                    int cell2 = (width * randY) +
                                (randX + 1);  // calculate adjacent cell
                    if (cells_.find(cell1) !=
                        cells_.find(
                            cell2)) {  // check that they are not in same set
                        wallRight_[cell1] = false;  // break wall
                        cells_.setunion(cells_.find(cell1),
                                        cells_.find(cell2));  // join sets
                        counter++;
                    }
                }
            }
        } else {  // break down
            if (randY <
                height - 1) {  // check that it is not on the bottom edge
                if (wallDown_[cell1] == true) {  // check that there is a wall
                    int cell2 = (width * (randY + 1)) +
                                randX;  // calculate adjacent cell
                    if (cells_.find(cell1) !=
                        cells_.find(
                            cell2)) {  // check that they are not in same set
                        wallDown_[cell1] = false;  // break wall
                        cells_.setunion(cells_.find(cell1),
                                        cells_.find(cell2));  // join sets
                        counter++;
                    }
                }
            }
        }
    }
}

bool SquareMaze::canTravel(int x, int y, int dir) const {
    /* dir = 0 represents a rightward step (+1 to the x coordinate)
       dir = 1 represents a downward step (+1 to the y coordinate)
       dir = 2 represents a leftward step (-1 to the x coordinate)
       dir = 3 represents an upward step (-1 to the y coordinate) */
    // right
    if (dir == 0) {
        if (x + 1 >= width_)
            return false;
        else if (wallRight_[width_ * y + x] != true)
            return true;
    }
    // down
    else if (dir == 1) {
        if (y + 1 >= height_)
            return false;
        else if (wallDown_[width_ * y + x] != true)
            return true;
    }
    // left
    else if (dir == 2) {
        if (x - 1 < 0)
            return false;
        else if (wallRight_[width_ * y + (x - 1)] != true)
            return true;
    }
    // up
    else if (dir == 3) {
        if (y - 1 < 0)
            return false;
        else if (wallDown_[width_ * (y - 1) + x] != true)
            return true;
    }

    return false;
}

void SquareMaze::setWall(int x, int y, int dir, bool exists) {
    /* dir = 0 represents a rightward step
       dir = 1 represents a downward step */
    if (dir == 0)
        wallRight_[width_ * y + x] = exists;
    if (dir == 1)
        wallDown_[width_ * y + x] = exists;
}

vector<int> SquareMaze::solveMaze() {
    vector<int> solution;

    map<int, int> path;

    vector<bool> visited;
    visited.resize(width_ * height_, false);
    visited[0] = true;

    stack<int> s;
    s.push(0);

    while (!s.empty()) {
        int pos = s.top();
        s.pop();
        int curX = pos % width_;
        int curY = pos / width_;

        // right
        if (canTravel(curX, curY, 0) && !visited[pos + 1]) {
            visited[pos + 1] = true;
            path[pos + 1] = pos;
            s.push(pos + 1);
        }

        // down
        if (canTravel(curX, curY, 1) && !visited[pos + width_]) {
            visited[pos + width_] = true;
            path[pos + width_] = pos;
            s.push(pos + width_);
        }

        // left
        if (canTravel(curX, curY, 2) && !visited[pos - 1]) {
            visited[pos - 1] = true;
            path[pos - 1] = pos;
            s.push(pos - 1);
        }

        // up
        if (canTravel(curX, curY, 3) && !visited[pos - width_]) {
            visited[pos - width_] = true;
            path[pos - width_] = pos;
            s.push(pos - width_);
        }
    }

    int maxLength = 0, i = 0;
    int pathLength;
    int destination;
    vector<int> solved;
    while (i < width_) {
        solved.clear();
        pathLength = 0;
        destination = width_ * (height_ - 1) + i;

        int check;
        while (!(destination == 0)) {
            int temp = path[destination];
            check = destination - temp;

            if (check == 1)
                solved.push_back(0);
            else if (check == width_)
                solved.push_back(1);
            else if (check == -1)
                solved.push_back(2);
            else if (check == -width_)
                solved.push_back(3);

            destination = temp;
            pathLength++;
        }

        if (maxLength < pathLength) {
            maxLength = pathLength;
            solution = solved;
        }

        i++;
    }

    reverse(solution.begin(), solution.end());

    return solution;
}

PNG* SquareMaze::drawMaze() const {
    PNG* maze = new PNG((width_ * 10) + 1, (height_ * 10) + 1);

    // luminance 0 = black

    // set left and top edge walls by coloring it black
    for (int i = 10; i < (width_ * 10) + 1; i++) {
        HSLAPixel& square = maze->getPixel(i, 0);
        square.l = 0.0;
    }
    for (int i = 0; i < (height_ * 10) + 1; i++) {
        HSLAPixel& square = maze->getPixel(0, i);
        square.l = 0.0;
    }

    // set walls accordingly
    for (int x = 0; x < width_; x++) {
        for (int y = 0; y < height_; y++) {
            int cell = (width_ * y) + x;     // calculate current cell
            if (wallRight_[cell] == true) {  // check if right wall exists
                // cout << "right" << endl;
                for (int i = 0; i <= 10; i++) {  // color pixels black
                    HSLAPixel& square =
                        maze->getPixel((x + 1) * 10, (y * 10) + i);
                    square.l = 0.0;
                }
            }
            if (wallDown_[cell] == true) {  // check if bottom wall exists
                // cout << "down" << endl;
                for (int i = 0; i <= 10; i++) {  // color pixels black
                    HSLAPixel& square =
                        maze->getPixel((x * 10) + i, (y + 1) * 10);
                    square.l = 0.0;
                }
            }
        }
    }

    return maze;
}

PNG* SquareMaze::drawMazeWithSolution() {
    PNG* mazeSolved = drawMaze();
    vector<int> solved = solveMaze();

    int curX = 5;
    int curY = 5;
    for (auto direction : solved) {
        switch (direction) {
            case 0:  // right
                for (int i = 0; i < 10; i++) {
                    HSLAPixel& curPixel = mazeSolved->getPixel(curX, curY);
                    curPixel.h = 0;
                    curPixel.s = 1.0;
                    curPixel.l = 0.5;
                    curX++;
                }
                break;
            case 1:  // down
                for (int i = 0; i < 10; i++) {
                    HSLAPixel& curPixel = mazeSolved->getPixel(curX, curY);
                    curPixel.h = 0;
                    curPixel.s = 1.0;
                    curPixel.l = 0.5;
                    curY++;
                }
                break;
            case 2:  // left
                for (int i = 0; i < 10; i++) {
                    HSLAPixel& curPixel = mazeSolved->getPixel(curX, curY);
                    curPixel.h = 0;
                    curPixel.s = 1.0;
                    curPixel.l = 0.5;
                    curX--;
                }
                break;
            case 3:  // up
                for (int i = 0; i < 10; i++) {
                    HSLAPixel& curPixel = mazeSolved->getPixel(curX, curY);
                    curPixel.h = 0;
                    curPixel.s = 1.0;
                    curPixel.l = 0.5;
                    curY--;
                }
                break;
            default:
                break;
        }
    }

    HSLAPixel& curPixel = mazeSolved->getPixel(curX, curY);
    curPixel.h = 0;
    curPixel.s = 1.0;
    curPixel.l = 0.5;

    curX -= 4;
    curY += 5;

    for (int i = curX; i < curX + 9; i++) {
        HSLAPixel& curPixel = mazeSolved->getPixel(i, height_ * 10);
        curPixel.l = 1.0;
    }

    return mazeSolved;
}