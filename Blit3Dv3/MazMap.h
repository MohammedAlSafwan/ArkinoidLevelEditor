#pragma once
#include "Tile.h"
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>

enum class RoverDir { NORTH = 0, EAST, SOUTH, WEST };

class MazMap
{
private:
	//TArith::TArithActionFunc  TArith::m_actionFcns[TAction::count]
	typedef void (MazMap::*fn)(bool &solved, int &currentY, int &currentX);
	MazMap::fn functions[4] = { NULL,NULL,NULL,NULL };
	//typedef void(*func)(bool &solved, int &currentY, int &currentX);
	//void(*functions[4])(bool &solved, int &currentY, int &currentX) = { &goNorth,&goEast,&goSouth,&goWest };

	struct Rover
	{
		int x = -1;
		int y = -1;
	};
	struct Exit
	{
		int x = -1;
		int y = -1;
	};

	void fixPath();
	bool solveMap();
	void printRover();
	void goNorth(bool &solved, int &currentY, int &currentX);
	void goEast(bool &solved, int &currentY, int &currentX);
	void goSouth(bool &solved, int &currentY, int &currentX);
	void goWest(bool &solved, int &currentY, int &currentX);
	void whereExit(bool &solved, int &currentY, int &currentX);
	void methodMaker();
public:
	//Map height and width
	int mazWidth = 0;
	int mazHeight = 0;
	int counter = 0;
	Rover rover;
	Exit exit;
	//Map Tiles
	std::vector<std::vector<Tile *>> tileList;

	MazMap();
	~MazMap();
	void LoadMap(std::string fileName);
	void SaveMap(std::string fileName);
	bool findEnd();

};

