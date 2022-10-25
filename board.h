#pragma once
#include <vector>
struct board
{
	struct tile
	{
		int _index;
		int _tileType;


		tile* topRight = nullptr;
		tile* top = nullptr;
		tile* topLeft = nullptr;

		tile* right = nullptr;
		tile* left = nullptr;

		tile* botRight = nullptr;
		tile* bot = nullptr;
		tile* botLeft = nullptr;

		tile(int index, int tileType);
		void revealTile(board &game);
	};


	int _columns;
	int _rows;
	int _mines;
	std::vector<int> currentBoard;
	std::vector<int> mineBoard;
	std::vector<tile> tileBoard;

	enum sprites { tile_hidden = -1, tile_revealed = 0, flag = 10, mine = 11 };

	board(int columns, int rows, int mines);
	void setTileNeighbors();
	void SetTileValues();
	void randomMines();
	void placeMines(std::vector<int> mineMap);
	void revealBoard(int index);
};




