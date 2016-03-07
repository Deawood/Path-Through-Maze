//This Program was done by David Bacon
//The goal was to get through a maze made out of 0,1, and an e marking the entrance

#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
#include <stack>
#include <cctype>
#include <sstream>
#include <cassert>
#include <queue>
#include <utility>


using namespace std;

class Point {
public:
	int x = 0;
	int y = 0;
	Point(int newX, int newY){
		x = newX; 
		y = newY;
	}
};

class Maze {
	vector<vector<unsigned int>> matrix;
	unsigned int entranceRow, entranceCol;
	bool exitMaze = false;
	int rowLength;
	string mazePath;
	public:
	 Maze(istream &is) {
		vector<unsigned int> row;
		string buffer;
		while (getline(is, buffer)) {
			for (auto c : buffer) {
				switch (c) {
				case '0':
					row.push_back(0);
				break;

				case '1':
					row.push_back(1);
				break;

				case 'e':
				case 'E':
					entranceRow = matrix.size();
					entranceCol = row.size();
					row.push_back(2);
				break;
				}
			}
			matrix.push_back(row);
			
			row.erase(row.begin(), row.end());
		}
		rowLength = matrix[0].size();
	}
	vector<Point> checkArea(Point currentPoint){
		vector<Point> vecP;
		//check cell to left
		if (currentPoint.y <= rowLength-1 && currentPoint.y > 0 && matrix.at(currentPoint.x).at(currentPoint.y-1) == 0) { 
			Point newPoint = Point(currentPoint.x, currentPoint.y-1);
			vecP.push_back(newPoint); 
			matrix[currentPoint.x][currentPoint.y-1] = matrix[currentPoint.x][currentPoint.y]+1;
		}
		//check cell to right
		if (currentPoint.y < rowLength-1 && currentPoint.y >= 0 && matrix.at(currentPoint.x).at(currentPoint.y+1) == 0) { 
			Point newPoint1 = Point(currentPoint.x, currentPoint.y+1);
			vecP.push_back(newPoint1); 
			matrix[currentPoint.x][currentPoint.y+1] = matrix[currentPoint.x][currentPoint.y]+1;
		}
		
		// //check cell above
		if (currentPoint.x < matrix.size()-1 && currentPoint.x > 0 && matrix.at(currentPoint.x-1).at(currentPoint.y) == 0) {
			Point newPoint2 = Point(currentPoint.x-1, currentPoint.y);
			vecP.push_back(newPoint2); 
			matrix[currentPoint.x-1][currentPoint.y] = matrix[currentPoint.x][currentPoint.y]+1;
		}
		
		// check below
		if (currentPoint.x <= matrix.size()-1 && currentPoint.x >= 0 && matrix.at(currentPoint.x+1).at(currentPoint.y) == 0) {
			Point NextnewPoint = Point(currentPoint.x+1, currentPoint.y);
			vecP.push_back(NextnewPoint); 
			matrix[currentPoint.x+1][currentPoint.y] = matrix[currentPoint.x][currentPoint.y]+1;

		}
		//move new location off stack and repeat
		
		
		return vecP;
	}
	void findPath() {
		queue<Point> myQueue;
		int k = entranceRow;
		int l = entranceCol;
		int fun = 0;
		//start at entrance then look in each direction.
		Point fistPoint = Point(k,l);
		myQueue.push(fistPoint);
		auto vecP = checkArea(fistPoint);
		for (int x = 0; x < vecP.size(); x++){
			myQueue.push(vecP.at(x));
		}
		myQueue.pop();
		while (myQueue.size() > 0){ 
			vecP = checkArea(myQueue.front());
			myQueue.pop();
			for (int x = 0; x < vecP.size(); x++){
				myQueue.push(vecP.at(x));
			}
			exitMaze = mazeExitfound(myQueue.front());
			if (exitMaze == true) { 
				string mazePath = retraceShortestPath(myQueue.front(), fistPoint);
				cout << mazePath << endl;
				break;
			}
		}
	}

	string retraceShortestPath(Point currentPoint, Point entrancePoint) {
		int killingIt = 0;
		string exitString = "";
		string finalExitString = "";
		stack<string> thatExitStack;
		exitString = exitString + "(" + to_string(currentPoint.x) + "," + to_string(currentPoint.y) + ") ";
		thatExitStack.push(exitString);
		Point newPoint = Point(currentPoint.x, currentPoint.y);;
		while (matrix.at(currentPoint.x).at(currentPoint.y) > matrix.at(entrancePoint.x).at(entrancePoint.y)) {
			
			//check cell to left
			if (currentPoint.y <= rowLength-1 && currentPoint.y > 0 && matrix.at(currentPoint.x).at(currentPoint.y-1) 
			== matrix[currentPoint.x][currentPoint.y]-1) { 
				newPoint = Point(currentPoint.x, currentPoint.y-1);
				
			}

			// //check cell to right
			if (currentPoint.y < rowLength-1 && currentPoint.y >= 0 && matrix.at(currentPoint.x).at(currentPoint.y+1) 
			== matrix[currentPoint.x][currentPoint.y]-1) { 
				newPoint = Point(currentPoint.x, currentPoint.y+1);
			}
			
			 //check cell above
			if (currentPoint.x <= matrix.size()-1 && currentPoint.x > 0 && matrix.at(currentPoint.x-1).at(currentPoint.y) 
			== matrix[currentPoint.x][currentPoint.y]-1) {
				newPoint = Point(currentPoint.x-1, currentPoint.y);
			}
			
			// check below
			if (currentPoint.x < matrix.size()-1 && currentPoint.x >= 0 && matrix.at(currentPoint.x+1).at(currentPoint.y) 
			== matrix[currentPoint.x][currentPoint.y]-1) {
				newPoint = Point(currentPoint.x+1, currentPoint.y);
			}

			currentPoint = newPoint;
			exitString = "(" + to_string(currentPoint.x) + "," + to_string(currentPoint.y) + ") ";
			thatExitStack.push(exitString);
		}

		
		while(!thatExitStack.empty()) {
			finalExitString = finalExitString + thatExitStack.top();
			thatExitStack.pop();
			killingIt++;
			if (killingIt == 5){  finalExitString = finalExitString + "\n"; killingIt = 0; }

		}
		return finalExitString;
	}

	bool mazeExitfound(Point currentPoint) {
		vector<Point> vecP;
		if (currentPoint.x == 0 || currentPoint.x == matrix.size()-1 
			|| currentPoint.y == 0 || currentPoint.y == rowLength-1) { return true;}
		return false;
	}

 	void printMaze() {
 		for (int i = 0; i < matrix.size(); i++) {
 			for (int j = 0; j < rowLength; j++) {
				cout << matrix[i][j];
			}
			cout << "\n";
		}
		cout << "\n";
	}
	int errorMazes() {
		string badMaze;
		if (matrix.size() <= 1) {
			badMaze = "This maze is a single char maze.";
			return 1;
		}
		if (matrix.size() <= 2) {
			badMaze = "This is a two by two maze."; 
			return 2;
		}
		return 0;
	}
};



int main() {
	Maze myMaze = Maze(std::cin);
	int re = myMaze.errorMazes();
	if (re == 1) {
		cout << "This maze is a single char maze." << "\n";
	} 
	if (re == 2) {
		cout << "This is a two by two maze." << "\n";
 	} else {
		myMaze.printMaze();
		myMaze.findPath();
		myMaze.printMaze();
	}
	return 0;
}



 