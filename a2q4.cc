#include <iostream>
#include <string>
#include <vector>
#include <sstream>

using namespace std;

vector< vector< vector< int > > > findPossibles(vector< vector< int > > grid);

static int searchCount = 0;

// Each Node stores the actual sudoku grid, and a list of domains
// of each cell
struct Node {
	vector< vector<int> > grid;
	vector< vector< vector< int > > > possible;
	Node(vector< vector<int> > g) {
		grid = g;
		possible = findPossibles(grid);
	}
	Node(vector< vector<int> > g, vector< vector< vector< int > > > p) {
		possible = p;
		bool setNext = false;
		grid = g;
		possible = p;
	}
};

// Generates a list of domains for each node
vector< vector< vector< int > > > findPossibles(vector< vector< int > > grid) {
	// {1...9}
	vector< int > temp1;
	for(int i = 1; i < 10; i++) {
		temp1.push_back(i);
	}
	// empty vector
	vector< int > temp2;
	// result

	//Set up the list: all 0s have empty vectors, otherwise domain = all
	vector< vector< vector< int > > > retval;
	for(int y = 0; y < 9; y++) {
		vector< vector< int > > buffer;
		for(int x = 0; x < 9; x++) {
			if(grid[y][x] == 0) 
				buffer.push_back(temp1);
			else
				buffer.push_back(temp2);
		}
		retval.push_back(buffer);
	}

	// Remove possibilties from col, row, square
	for(int y = 0; y < 9; y++) {
		for(int x = 0; x < 9; x++) {
			if(grid[y][x] != 0) {
				int removed = grid[y][x];
				// remove from all in row
				for(int tempx = 0; tempx < 9; tempx++) {
					for(int i = 0; i < retval[y][tempx].size(); i++) {
						if(retval[y][tempx][i] == removed) {
							retval[y][tempx].erase(retval[y][tempx].begin()+i);
							break;
						}
					}
				}
				// remove from all in col
				for(int tempy = 0; tempy < 9; tempy++) {
					for(int i = 0; i < retval[tempy][x].size(); i++) {
						if(retval[tempy][x][i] == removed) {
							retval[tempy][x].erase(retval[tempy][x].begin()+i);
							break;
						}
					}
				}
				// remove from sq
				// first set quadrant number
				int closestX,closestY = 0;
				if(y>=0 && y<=2) {
					closestY = 1;
				}
				else if(y>=3 && y<=5) {
					closestY = 4;
				}
				else if(y>=6 && y<=8) {
					closestY = 7;
				}
				if(x>=0 && x<=2) {
					closestX = 1;
				}
				else if(x>=3 && x<=5) {
					closestX = 4;
				}
				else if(x>=6 && x<=8) {
					closestX = 7;
				}
				for(int tempx = -1; tempx <= 1; tempx++) {
					for(int tempy = -1; tempy <= 1; tempy++) {
						for(int i = 0; i < retval[closestY+tempy][closestX+tempx].size(); i++) {
							if(retval[closestY+tempy][closestX+tempx][i] == removed) {
								retval[closestY+tempy][closestX+tempx].erase(retval[closestY+tempy][closestX+tempx].begin()+i);
								break;
							}
						}
					}
				}
			}
		}
	}
	return retval;
}

//performAction: called after determining a value; removes it from the domains
// of other cells
Node performAction(Node n, int x, int y, int value) {
	n.grid[y][x] = value;
	int removed = value;
	// remove from all in row
	for(int tempx = 0; tempx < 9; tempx++) {
		for(int i = 0; i < n.possible[y][tempx].size(); i++) {
			if(n.possible[y][tempx][i] == removed) {
				n.possible[y][tempx].erase(n.possible[y][tempx].begin()+i);
				break;
			}
		}
	}
	// remove from all in col
	for(int tempy = 0; tempy < 9; tempy++) {
		for(int i = 0; i < n.possible[tempy][x].size(); i++) {
			if(n.possible[tempy][x][i] == removed) {
				n.possible[tempy][x].erase(n.possible[tempy][x].begin()+i);
				break;
			}
		}
	}
	// remove from sq
	// first set quadrant number
	int closestX,closestY = 0;
	if(y>=0 && y<=2) {
		closestY = 1;
	}
	else if(y>=3 && y<=5) {
		closestY = 4;
	}
	else if(y>=6 && y<=8) {
		closestY = 7;
	}
	if(x>=0 && x<=2) {
		closestX = 1;
	}
	else if(x>=3 && x<=5) {
		closestX = 4;
	}
	else if(x>=6 && x<=8) {
		closestX = 7;
	}
	for(int tempx = -1; tempx <= 1; tempx++) {
		for(int tempy = -1; tempy <= 1; tempy++) {
			for(int i = 0; i < n.possible[closestY+tempy][closestX+tempx].size(); i++) {
				if(n.possible[closestY+tempy][closestX+tempx][i] == removed) {
					n.possible[closestY+tempy][closestX+tempx].erase(n.possible[closestY+tempy][closestX+tempx].begin()+i);
					break;
				}
			}
		}
	}
	return n;
}

//countCVariables: count the number of constraining variables of a cell
//if a row,col,sq finds an empty cell, increment count
int countCVariables(vector< vector< int > > grid, int x, int y) {
	int count = 0;
	// check row
	for(int tempx = 0; tempx < 9; tempx++) {
		if(grid[y][tempx] == 0) {
			count++;
		}
	}
	// check col
	for(int tempy = 0; tempy < 9; tempy++) {
		if(grid[tempy][x] == 0) {
			count++;
		}
	}
	// first set quadrant number
	int closestX,closestY = 0;
	if(y>=0 && y<=2) {
		closestY = 1;
	}
	else if(y>=3 && y<=5) {
		closestY = 4;
	}
	else if(y>=6 && y<=8) {
		closestY = 7;
	}
	if(x>=0 && x<=2) {
		closestX = 1;
	}
	else if(x>=3 && x<=5) {
		closestX = 4;
	}
	else if(x>=6 && x<=8) {
		closestX = 7;
	}
	// check square
	for(int tempx = -1; tempx <= 1; tempx++) {
		for(int tempy = -1; tempy <= 1; tempy++) {
			// don't double count 
			if(closestX-tempx == x || closestY-tempy == y) {
				continue;
			}
			if(grid[closestY-tempy][closestX-tempx] == 0) {
				count++;
			}
		}
	}
	return count;
}

// checkCValues: return the index of the least constraining value
// if a row,col,sq contains the value, count goes up
// if count is smaller, it's the new least constraining vlaue
int checkCValues(Node n, int x, int y) {
	int index, value = 1000000;
	for(int i = 0; i < n.possible[y][x].size(); i++) {
		int count = 0;
		// check row
		for(int tempx = 0; tempx < 9; tempx++) {
			if(n.grid[y][tempx] == 0 && 
				find(n.possible[y][tempx].begin(),
					 n.possible[y][tempx].end(),
					 n.possible[y][x][i]) != n.possible[y][tempx].end()
				) {
				count++;
			}
		}
		// check col
		for(int tempy = 0; tempy < 9; tempy++) {
			if(n.grid[tempy][x] == 0 && 
				find(n.possible[tempy][x].begin(),
					 n.possible[tempy][x].end(),
					 n.possible[y][x][i]) != n.possible[tempy][x].end()
				) {
				count++;
			}
		}
		// first set quadrant number
		int closestX,closestY = 0;
		if(y>=0 && y<=2) {
			closestY = 1;
		}
		else if(y>=3 && y<=5) {
			closestY = 4;
		}
		else if(y>=6 && y<=8) {
			closestY = 7;
		}
		if(x>=0 && x<=2) {
			closestX = 1;
		}
		else if(x>=3 && x<=5) {
			closestX = 4;
		}
		else if(x>=6 && x<=8) {
			closestX = 7;
		}
		// check square
		for(int tempx = -1; tempx <= 1; tempx++) {
			for(int tempy = -1; tempy <= 1; tempy++) {
				int sqX = closestX - tempx;
				int sqY = closestY - tempy;
				if(sqX == x || sqY == y)
					continue;
				if(n.grid[sqY][sqX] == 0 && 
					find(n.possible[sqY][sqX].begin(),
						 n.possible[sqY][sqX].end(),
						 n.possible[y][x][i]) != n.possible[sqY][sqX].end()
					) {
					count++;
				}
			}
		}
		if(count < value) {
			value = count;
			index = i;
		}
	}
	return index;
}

//finished: validate of Node n is complete
//Pushes row,col,sq into vectors and sorts and compares
bool finished(Node n) {
	vector<int> correct;
	for(int i = 1; i <= 9; i++) {
		correct.push_back(i);
	}
	// Check colummns
	for(int y = 0; y < 9; y++) {
		vector<int> colvals;
		for(int x = 0; x < 9; x++) {
			// Break as soon as it finds a empty space
			if(n.grid[y][x] == 0) 
				return false;
			colvals.push_back(n.grid[y][x]);
		}
		sort(colvals.begin(),colvals.end());
		if(colvals != correct) 
			return false;
 	}
	for(int x = 0; x < 9; x++) {
		vector<int> rowvals;
		for(int y = 0; y < 9; y++) {
			rowvals.push_back(n.grid[y][x]);
		}
		sort(rowvals.begin(),rowvals.end());
		if(rowvals != correct) 
			return false;
	}
	for(int closestY = 1; closestY <= 7; closestY+=3) {
		for(int closestX = 1; closestX <= 7; closestX+=3) {
			vector<int> squarevals;
			for(int tempx = -1; tempx <= 1; tempx++) {
				for(int tempy = -1; tempy <= 1; tempy++) {
					int sqX = closestX - tempx;
					int sqY = closestY - tempy;
					squarevals.push_back(n.grid[sqY][sqX]);
				}
			}
			sort(squarevals.begin(),squarevals.end());
			if(squarevals != correct)
				return false;
		}
	}
	return true;
}

//search: the backtracking algorithm
void search(Node start) {
	// initialize stack
	vector<Node> s;
	s.push_back(start);
	while(!s.empty() && searchCount < 10000) {
		searchCount++;
		Node n = s.back();
		s.pop_back();
		// Check if solved
		if(finished(n)) {
			//Print soln to cerr
			for(int i = 0; i < 9; i++) {
				for(int j = 0; j < 9; j++) {
					if(n.grid[j][i] != 0) 
					cerr << n.grid[j][i] << " ";
					else
						cerr << "  ";
				}
				cerr << " |  ";
				for(int j = 0; j < 9; j++) {
					cerr << n.grid[j][i] << " ";
				}
				cerr << endl;
			}
			cerr << endl;
			cout << searchCount <<",";
			return;
		}
		//Print soln to cerr
			for(int i = 0; i < 9; i++) {
				for(int j = 0; j < 9; j++) {
					if(n.grid[j][i] != 0) 
					cerr << n.grid[j][i] << " ";
					else
						cerr << "  ";
				}
				cerr << " |  ";
				for(int j = 0; j < 9; j++) {
					cerr << n.grid[j][i] << " ";
				}
				cerr << endl;
			}
			cerr << endl;
		// Variables to store the next 
		int actionx = 10;
		int actiony = 10;
		int actionr = 10;
		int actionc = 10;
		// Find the next action to take
		// Find where it is
		for(int y = 0; y < 9; y++) {
			for(int x = 0; x < 9; x++) {
				if(n.grid[y][x] != 0) {
					continue;
				}
				// If there's less restrictions, it's a better action
				else if(actionr > n.possible[y][x].size()) {
					actionr = n.possible[y][x].size();
					actionx = x;
					actiony = y;
					actionc = countCVariables(n.grid,x,y);
				}
				// If tie in restrictions and more contraints, it's a better action
				else if(actionr == n.possible[y][x].size() &&
					actionc < countCVariables(n.grid,x,y)) {
					actionx = x;
					actiony = y;
					actionc = countCVariables(n.grid,x,y);
				}
			}
		}
		// Find what it is (index)
		// If there's no valid actions, then skip the node
		if(n.possible[actiony][actionx].size() == 0) 
			continue;
		int actioni = checkCValues(n,actionx,actiony);
		// Remove action from node, add to stack
		// so that next iteration of it won't check the action
		int value = n.possible[actiony][actionx][actioni];
		n.possible[actiony][actionx].erase(actioni + n.possible[actiony][actionx].begin());
		s.push_back(n);
		// Take action on node & forward check;
		Node newNode = performAction(n,actionx,actiony,value);
		bool valid = true;
		// Forward Check
		for(int y = 0; y < 9; y++) {
			for(int x = 0; x < 9; x++) {
				if(newNode.grid[y][x] == 0 &&
					newNode.possible[y][x].size() == 0)
					valid = false;
			}
		}
		if(valid) {
			s.push_back(newNode);
		}
	}
	if(searchCount == 10000) {
		cout << "10000,";
	}
}

int main() {
	vector< vector<int> > grid;
	for(int y = 0; y < 9; y++) {
		vector<int> row;
		for(int x = 0; x < 9; x++) {
			int n; cin >> n;
			row.push_back(n);
		}
		grid.push_back(row);
	}
	// Print start to cerr
	for(int i = 0; i < 9; i++) {
		for(int j = 0; j < 9; j++) {
			if(grid[j][i] != 0) 
			cerr << grid[j][i] << " ";
			else
				cerr << "  ";
		}
		cerr << " |  ";
		for(int j = 0; j < 9; j++) {
			cerr << grid[j][i] << " ";
		}
		cerr << endl;
	}
	cerr << endl;
	Node start = Node(grid);
	search(start);
}