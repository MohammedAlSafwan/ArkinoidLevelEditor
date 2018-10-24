#pragma once
#include "MazMap.h"

MazMap::MazMap()
{

}


MazMap::~MazMap()
{
	for (auto row : tileList)
	{
		for (auto col : row)
		{
			col->~Tile();
		}
	}
	tileList.clear();
}

void MazMap::LoadMap(std::string fileName)//, std::vector<Tile *> &tileList)
{
	//clear the current brickList
	//for (auto A : tileList) for (auto B : A) delete B;
	//tileList.clear();

	//open file
	std::ifstream myfile(fileName);
	std::string line;

	if (myfile.is_open())
	{
		//read in # of bricks
		myfile >> mazWidth;
		myfile >> mazHeight;

		//init cols
		for (int y = 0; y < mazHeight; y++)
		{
			//init rows
			std::vector<Tile*> newRow;

			for (int x = 0; x < mazWidth; x++)
			{
				//make a brick
				Tile *B = new Tile();
				B->typeID = TileEnum::TRAVERSABLE;
				newRow.push_back(B);
			}
			tileList.push_back(newRow);
			/*tileList[y].reserve(mazWidth);*/
		}

		int t = 0;
		int posX = 0;
		int posY = 0;

		while (!myfile.eof())
		{
			//make a tile
			myfile >> t;

			myfile >> posX;
			myfile >> posY;



			//make a tile
			//myfile >> t;
			//if ((TileEnum)t == TileEnum::TRAVERSABLE)
			//{
			//	tileList[posY][posX]->typeID = TileEnum::UNTRAVERSABLE;
			//}
			//else
			//{
			//	tileList[posY][posX]->typeID = (TileEnum)t;
			//}
			tileList[posY][posX]->typeID = (TileEnum)t;

			if ((TileEnum)t == TileEnum::ROVER)
			{
				rover.x = posX;
				rover.y = posY;
			}
			if ((TileEnum)t == TileEnum::END)
			{
				exit.x = posX;
				exit.y = posY;
			}
		}

		myfile.close();
	}
}

void MazMap::SaveMap(std::string fileName)
{
	//open file
	std::ofstream myfile;
	myfile.open(fileName, std::ios::trunc); //truncate old file

	if (myfile.is_open())
	{
		//write out # of bricks
		myfile << tileList.size() << std::endl;
		myfile << tileList[0].size() << std::endl;
		//write out each brick
		for (int y = 0; y < tileList.size(); y++)
		{
			for (int x = 0; x < tileList[0].size(); x++)
			{
				Tile *saveTile = tileList[y][x];
				if (saveTile->typeID != TileEnum::TRAVERSABLE)
				{
					myfile << (int)saveTile->typeID << " " << x << " " << y << std::endl;
				}
			}
		}

		myfile.close();
	}
}

void MazMap::printRover()
{
	printf("rover.x = %d\n", rover.x);
	printf("rover.y = %d\n", rover.y);
	printf("N = %d\n", ((tileList[rover.y][rover.x]->dir >> 3) & 1));
	printf("E = %d\n", ((tileList[rover.y][rover.x]->dir >> 2) & 1));
	printf("S = %d\n", ((tileList[rover.y][rover.x]->dir >> 1) & 1));
	printf("W = %d\n", ((tileList[rover.y][rover.x]->dir >> 0) & 1));
}

void MazMap::fixPath()
{
	//North
	if (rover.y + 1 >= mazHeight
		|| tileList[rover.y + 1][rover.x]->typeID == TileEnum::UNTRAVERSABLE
		|| tileList[rover.y + 1][rover.x]->typeID == TileEnum::PASSED)
		tileList[rover.y][rover.x]->dir = tileList[rover.y][rover.x]->dir & 0b0111;

	//South
	if (rover.y - 1 < 0
		|| tileList[rover.y - 1][rover.x]->typeID == TileEnum::UNTRAVERSABLE
		|| tileList[rover.y - 1][rover.x]->typeID == TileEnum::PASSED)
		tileList[rover.y][rover.x]->dir = tileList[rover.y][rover.x]->dir & 0b1101;

	//East
	if (rover.x + 1 >= mazWidth
		|| tileList[rover.y][rover.x + 1]->typeID == TileEnum::UNTRAVERSABLE
		|| tileList[rover.y][rover.x + 1]->typeID == TileEnum::PASSED)
		tileList[rover.y][rover.x]->dir = tileList[rover.y][rover.x]->dir & 0b1011;

	//Wests
	if (rover.x - 1 < 0
		|| tileList[rover.y][rover.x - 1]->typeID == TileEnum::UNTRAVERSABLE
		|| tileList[rover.y][rover.x - 1]->typeID == TileEnum::PASSED)
		tileList[rover.y][rover.x]->dir = tileList[rover.y][rover.x]->dir & 0b1110;
	//printRover();
}

bool MazMap::solveMap()
{
	bool solved = false;
	int currentX = rover.x;
	int currentY = rover.y;
	fixPath();
	whereExit(solved, currentY, currentX);

	//North
	if (!solved && ((tileList[rover.y][rover.x]->dir >> 3) & 1))
	{
		//printf("checking north!\n");
		goNorth(solved, currentY, currentX);
	}
	//East
	if (!solved && ((tileList[rover.y][rover.x]->dir >> 2) & 1))
	{
		//printf("checking east!\n");
		goEast(solved, currentY, currentX);
	}
	//West
	if (!solved && ((tileList[rover.y][rover.x]->dir >> 0) & 1))
	{
		//printf("checking west!\n");
		goWest(solved, currentY, currentX);
	}
	//South
	if (!solved && ((tileList[rover.y][rover.x]->dir >> 1) & 1))
	{
		//printf("checking south!\n");
		goSouth(solved, currentY, currentX);
	}

	//set the current rover position as passed
	if (tileList[currentY][currentY]->typeID != TileEnum::UNTRAVERSABLE)
		tileList[currentY][currentY]->typeID = TileEnum::PASSED;
	//printf("=======\n");
	//printf("Done \nx=%d || \ny=%d ||\n", currentX, currentY);
	//printf("=======\n");
	//if (currentX == 11 && currentY == 9)
	//{
	//	solved;
	//}
	return solved;
}

//void MazMap::goNorth(bool &solved, int &currentY, int &currentX)
//{
//	if (!solved && ((tileList[rover.y][rover.x]->dir >> 3) & 1))
//	{
//		counter++;
//		tileList[rover.y][rover.x]->typeID = TileEnum::PASSED;
//
//		rover.y = (rover.y + 1 >= mazHeight) ? mazHeight - 1 : rover.y + 1;
//		if (tileList[rover.y][rover.x]->typeID != TileEnum::END)
//		{
//			solved = solveMap();
//			if (!solved)
//			{
//				tileList[currentY][currentX]->dir = tileList[currentY][currentX]->dir & 0b1011;
//				rover.y -= 1;
//			}
//		}
//		else
//		{
//			exit.x = rover.x;
//			exit.y = rover.y;
//			solved = true;
//		}
//	}
//}
//
//void MazMap::goEast(bool &solved, int &currentY, int &currentX)
//{
//	if (!solved && ((tileList[rover.y][rover.x]->dir >> 2) & 1))
//	{
//		counter++;
//		tileList[rover.y][rover.x]->typeID = TileEnum::PASSED;
//
//		rover.x = (rover.x + 1 >= mazWidth) ? mazWidth - 1 : rover.x + 1;
//		if (tileList[rover.y][rover.x]->typeID != TileEnum::END)
//		{
//			solved = solveMap();
//			if (!solved)
//			{
//				tileList[currentY][currentX]->dir = tileList[currentY][currentX]->dir & 0b1011;
//				rover.x -= 1;
//			}
//		}
//		else
//		{
//			exit.x = rover.x;
//			exit.y = rover.y;
//			solved = true;
//		}
//	}
//}
//
//void MazMap::goSouth(bool &solved, int &currentY, int &currentX)
//{
//	if (!solved && ((tileList[rover.y][rover.x]->dir >> 1) & 1))
//	{
//		counter++;
//		tileList[rover.y][rover.x]->typeID = TileEnum::PASSED;
//
//		rover.y = (rover.y - 1 < 0) ? 0 : rover.y - 1;
//		if (tileList[rover.y][rover.x]->typeID != TileEnum::END)
//		{
//			solved = solveMap();
//			if (!solved)
//			{
//				tileList[currentY][currentX]->dir = tileList[currentY][currentX]->dir & 0b1101;
//				rover.y += 1;
//			}
//		}
//		else
//		{
//			exit.x = rover.x;
//			exit.y = rover.y;
//			solved = true;
//		}
//	}
//}
//
//void MazMap::goWest(bool &solved, int &currentY, int &currentX)
//{
//	if (!solved && ((tileList[rover.y][rover.x]->dir >> 0) & 1))
//	{
//		counter++;
//		tileList[rover.y][rover.x]->typeID = TileEnum::PASSED;
//		rover.x = (rover.x - 1 < 0) ? 0 : rover.x - 1;
//		if (tileList[rover.y][rover.x]->typeID != TileEnum::END)
//		{
//			solved = solveMap();
//			if (!solved)
//			{
//				tileList[currentY][currentX]->dir = tileList[currentY][currentX]->dir & 0b1110;
//				rover.x += 1;
//			}
//		}
//		else
//		{
//			exit.x = rover.x;
//			exit.y = rover.y;
//			solved = true;
//		}
//	}
//}

void MazMap::goNorth(bool &solved, int &currentY, int &currentX)
{

	counter++;
	tileList[rover.y][rover.x]->typeID = TileEnum::PASSED;

	rover.y = (rover.y + 1 >= mazHeight) ? mazHeight - 1 : rover.y + 1;
	if (tileList[rover.y][rover.x]->typeID != TileEnum::END)
	{
		solved = solveMap();
		if (!solved)
		{
			tileList[currentY][currentX]->dir = tileList[currentY][currentX]->dir & 0b1011;
			rover.y -= 1;
			counter--;
		}
	}
	else
	{
		exit.x = rover.x;
		exit.y = rover.y;
		solved = true;
	}
}

void MazMap::goEast(bool &solved, int &currentY, int &currentX)
{
	counter++;
	tileList[rover.y][rover.x]->typeID = TileEnum::PASSED;

	rover.x = (rover.x + 1 >= mazWidth) ? mazWidth - 1 : rover.x + 1;
	if (tileList[rover.y][rover.x]->typeID != TileEnum::END)
	{
		solved = solveMap();
		if (!solved)
		{
			tileList[currentY][currentX]->dir = tileList[currentY][currentX]->dir & 0b1011;
			rover.x -= 1;
			counter--;
		}
	}
	else
	{
		exit.x = rover.x;
		exit.y = rover.y;
		solved = true;
	}
}

void MazMap::goSouth(bool &solved, int &currentY, int &currentX)
{
	counter++;
	tileList[rover.y][rover.x]->typeID = TileEnum::PASSED;

	rover.y = (rover.y - 1 < 0) ? 0 : rover.y - 1;
	if (tileList[rover.y][rover.x]->typeID != TileEnum::END)
	{
		solved = solveMap();
		if (!solved)
		{
			tileList[currentY][currentX]->dir = tileList[currentY][currentX]->dir & 0b1101;
			rover.y += 1;
			counter--;
		}
	}
	else
	{
		exit.x = rover.x;
		exit.y = rover.y;
		solved = true;
	}
}

void MazMap::goWest(bool &solved, int &currentY, int &currentX)
{
	counter++;
	tileList[rover.y][rover.x]->typeID = TileEnum::PASSED;
	rover.x = (rover.x - 1 < 0) ? 0 : rover.x - 1;
	if (tileList[rover.y][rover.x]->typeID != TileEnum::END)
	{
		solved = solveMap();
		if (!solved)
		{
			tileList[currentY][currentX]->dir = tileList[currentY][currentX]->dir & 0b1110;
			rover.x += 1;
			counter--;
		}
	}
	else
	{
		exit.x = rover.x;
		exit.y = rover.y;
		solved = true;
	}
}

void MazMap::whereExit(bool &solved, int &currentY, int &currentX)
{
	//North
	if (rover.y < exit.y)
	{
		//North
		if (!solved && ((tileList[rover.y][rover.x]->dir >> 3) & 1))
		{
			//printf("checking north!\n");
			goNorth(solved, currentY, currentX);
		}
		if (rover.x < exit.x)
		{
			//East
			if (!solved && ((tileList[rover.y][rover.x]->dir >> 2) & 1))
			{
				//printf("checking east!\n");
				goEast(solved, currentY, currentX);
			}
		}
		if (rover.x > exit.x)
		{
			//West
			if (!solved && ((tileList[rover.y][rover.x]->dir >> 0) & 1))
			{
				//printf("checking west!\n");
				goWest(solved, currentY, currentX);
			}
		}
	}
	//South
	else
	{
		//South
		if (!solved && ((tileList[rover.y][rover.x]->dir >> 1) & 1))
		{
			//printf("checking south!\n");
			goSouth(solved, currentY, currentX);
		}

		if (rover.x < exit.x)
		{
			//East
			if (!solved && ((tileList[rover.y][rover.x]->dir >> 2) & 1))
			{
				//printf("checking east!\n");
				goEast(solved, currentY, currentX);
			}
		}
		if (rover.x > exit.x)
		{
			//West
			if (!solved && ((tileList[rover.y][rover.x]->dir >> 0) & 1))
			{
				//printf("checking west!\n");
				goWest(solved, currentY, currentX);
			}
		}
	}
}

void MazMap::methodMaker()
{
	//North
	if (rover.y < exit.y)
	{
		functions[0] = &MazMap::goNorth;

		if (rover.x < exit.x)
		{
			//East
			functions[1] = &MazMap::goEast;
		}
		if (rover.x > exit.x)
		{
			//West
			functions[2] = &MazMap::goWest;
		}
		else
		{
			//rover.x == exit.x
			//It doesn't matter
			functions[1] = &MazMap::goEast;
			functions[2] = &MazMap::goWest;
		}
		functions[3] = &MazMap::goSouth;
	}
	//South
	else
	{
		functions[0] = &MazMap::goSouth;
		if (rover.x < exit.x)
		{
			//East
			functions[1] = &MazMap::goEast;
		}
		if (rover.x > exit.x)
		{
			//West
			functions[2] = &MazMap::goWest;
		}
		else
		{
			//rover.x == exit.x
			//It doesn't matter
			functions[1] = &MazMap::goEast;
			functions[2] = &MazMap::goWest;
		}
		functions[3] = &MazMap::goNorth;
	}
}

bool MazMap::findEnd()
{
	bool solved = false;
	//check for the first tile
	if (tileList[rover.y][rover.x]->typeID == TileEnum::END)
	{
		exit.x = rover.x;
		exit.y = rover.y;
		solved = true;
	}
	else
	{
		methodMaker();
		solved = solveMap();
	}
	return solved;
}