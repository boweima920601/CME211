#include <iostream>
#include <fstream>
#include <string>

// maximum size of the static array
#define max_row 256
#define max_col 256

int main(int argc, char* argv[]) {
	// this may seems to be a dumb way to initialize the zero array. I was considering using memset().
	int maze[max_row][max_col] = {};
	int row = 0;
	int col = 0;

	// check if user has provided valid input stream
	// if not, print usage message and exit main()
	if (argc < 3) {
		std::cout << "Usage:" << std::endl;
		std::cout << " " << argv[0] << "<maze file> <solution file>" << std::endl;
		return 0;
	}

	std::ifstream fin;
	fin.open(argv[1]);
	if (fin.is_open()) {
		// read in number of row and col of the maze
		fin >> row >> col;
		// if the maze requires more storage than our static array, print error message and exit main()
		if (row > max_row || col > max_col) {
			std::cout << "Not enough storage to store the maze. Sorry" << std::endl;
			return 0;
		}
		// record the positions of the walls
		// denote maze[i][j] = 1 if it is a wall
		int i, j;
		while(fin >> i >> j) {
			maze[i][j] = 1;
		}
		fin.close();
	}

	// find the position of the entrance on the top of the maze
	int entrance;
	for(int i = 0; i < col; i++) {
        if (maze[0][i] == 0) {
			entrance = i;
        }
	}

	int x = 0;
	int y = entrance;
	// Denote directions as: 0 is going down, 1 is going left, 2 is going up, and 3 is going right(overall clockwise)
	int d = 0;

	// implement the right hand follower algorithm and write each move into the solution file.
	// the right hand follower algorithm can be described as follows:
	// while (current position is not an exit)
	// 		1. if right hand side is not a wall, turn right and step forward
	// 		2. else, step forward if not facing a wall
	//		3. else, turn left
	std::ofstream fout;
	fout.open(argv[2]);
	if (fout.is_open()) {
		// record the entrance of the maze
		fout << x << " " << y << std::endl;

		while (x < row - 1) {
			if (d == 0) {	// facing down
                if (maze[x][y - 1] == 0) {
                    y -= 1;
                    d = (d + 1) % 4;	// turn right
                }
				else if (maze[x + 1][y] == 0) {
					x += 1;		// step forward
				}
                else {
                    d = (d + 3) % 4;	// turn left and iterate again
                    continue;
                }
            }
			else if (d == 1) {	// facing left
                if (maze[x - 1][y] == 0) {
                    x -= 1;
                    d = (d + 1) % 4;	// turn right
                }
                else if (maze[x][y - 1] == 0) {
					y -= 1;		// step forward
				}
                else {
                    d = (d + 3) % 4;	// turn left and iterate again
                    continue;
                }

			}
			else if (d == 2) {	// facing up
                if (maze[x][y + 1] == 0) {
                    y += 1;
                    d = (d + 1) % 4;	// turn right
                }
                else if (maze[x - 1][y] == 0) {
					x -= 1;		// step forward
				}
                else {
                    d = (d + 3) % 4;	// turn left and iterate again
                    continue;	
                }
            }
			else {	// facing right
                if (maze[x + 1][y] == 0) {
                    x += 1;
                    d = (d + 1) % 4;	// turn right
                }
                else if (maze[x][y + 1] == 0) {
					y += 1;		// step forward
				}
                else {
                    d = (d + 3) % 4;	// turn left and iterate again
                    continue;
                }
            }
            // write a single move into the solution file
			fout << x << " " << y << std::endl;
		}
		fout.close();
	}
    return 0;
}