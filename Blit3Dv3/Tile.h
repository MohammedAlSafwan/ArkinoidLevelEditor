#pragma once

#define TILE_WIDTH 20
#define TILE_HEIGHT 20

//traversable
enum class TileEnum { TRAVERSABLE = 0, UNTRAVERSABLE, END, PASSED, ROVER, UNKNOWN };
//enum class TileEnum { UNTRAVERSABLE = 0, TRAV1ERSABLE, END, PASSED, ROVER, UNKNOWN  };
//enum TILE_TYPE {TRAVERSABLE, NON_TRAVERSABLE, START, END, VISITED, ROVER};

class Tile
{
public:

	TileEnum typeID;
	int dir = 0b1111;

	Tile();
	~Tile();
};

