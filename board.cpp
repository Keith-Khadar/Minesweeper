#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "board.h"
#include <iostream>


board::board(int columns, int rows, int mines)
{
	_columns = columns;
	_rows = rows;
	_mines = mines;
	
	currentBoard.clear();
	currentBoard.resize(columns * rows,tile_hidden);


	mineBoard.clear();
	mineBoard.resize(columns * rows, 0);

}

void board::randomMines()
{
	srand(time(NULL));

	int minesPlaced = 0;
	// If there are more mines in the config than spaces available
	if(_mines > (_columns * _rows))
	{
		_mines = (_columns * _rows);
	}

	while (minesPlaced < _mines)
	{
		int randomNum = rand() % (_columns * _rows);
		if (mineBoard.at(randomNum) == 0)
		{
			mineBoard.at(randomNum) = mine;
			minesPlaced++;
		}
	}

	for (int i = 0; i < mineBoard.size(); i++)
	{
		if (mineBoard.at(i) == 1)
		{
			mineBoard.at(i) = mine;

		}
		tile t(i, mineBoard.at(i));
		tileBoard.push_back(t);
	}

	setTileNeighbors();
	SetTileValues();
}
void board::placeMines(std::vector<int> mineMap)
{
	mineBoard = mineMap;
	_mines = 0;

	for (int i = 0; i < mineBoard.size(); i++)
	{
		if (mineBoard.at(i) == 1)
		{
			mineBoard.at(i) = mine;
			_mines++;

		}
		tile t(i, mineBoard.at(i));
		tileBoard.push_back(t);
	}

	setTileNeighbors();
	SetTileValues();
}

void board::revealBoard(int index)
{
	tileBoard.at(index).revealTile(*this);
}

board::tile::tile(int index, int tileType)
{
	_tileType = tileType;
	_index = index;
}

void board::setTileNeighbors()
{
	for(int index = 0; index < tileBoard.size(); index++)
	{
		// Three Above
		if ((index - _columns) - 1 >= 0 && index % _columns > 0) // top left
		{
			tileBoard.at(index).topLeft = &(tileBoard.at((index - _columns) - 1));
		}

		if (index - _columns >= 0) // top
		{
			tileBoard.at(index).top = &(tileBoard.at((index - _columns)));
		}

		if ((index - _columns) + 1 >= 0 && index % _columns < (_columns - 1)) // top right
		{
			tileBoard.at(index).topRight = &(tileBoard.at((index - _columns) + 1));
		}

		// Same Row
		if (index + 1 < (_columns * _rows) && index % _columns < (_columns - 1)) // right
		{
			tileBoard.at(index).right = &(tileBoard.at(index + 1));
		}

		if (index - 1 >= 0 && index % _columns > 0) // left
		{
			tileBoard.at(index).left = &(tileBoard.at(index - 1));
		}

		// Three Below
		if ((index + _columns) - 1 < (_columns * _rows) && index % _columns > 0) // bottom left
		{
			tileBoard.at(index).botLeft = &(tileBoard.at((index + _columns) - 1));
		}

		if ((index + _columns) < (_columns * _rows)) // bottom
		{
			tileBoard.at(index).bot = &(tileBoard.at((index + _columns)));
		}
		if ((index + _columns) + 1 < (_columns * _rows) && index % _columns < (_columns - 1)) // bottom right
		{
			tileBoard.at(index).botRight = &(tileBoard.at((index + _columns) + 1));
		}
	}

}

void board::SetTileValues()
{
	int index = 0;
	for (int j = 0; j < _rows; j++)
	{
		for (int i = 0; i < _columns; i++)
		{
			int minesNear = 0;



			// Three Above
			if(tileBoard.at(index).topLeft != nullptr)
			{
				if (tileBoard.at(index).topLeft->_tileType == mine) // top left
				{
					minesNear++;

				}
			}
			if (tileBoard.at(index).top != nullptr)
			{
				if (tileBoard.at(index).top->_tileType == mine) // top 
				{
					minesNear++;

				}
			}
			if (tileBoard.at(index).topRight != nullptr)
			{
				if (tileBoard.at(index).topRight->_tileType == mine) // top right
				{
					minesNear++;
				}
			}

			// Same Row
			if (tileBoard.at(index).left != nullptr)
			{
				if (tileBoard.at(index).left->_tileType == mine) // left
				{
					minesNear++;

				}
			}
			if (tileBoard.at(index).right != nullptr)
			{
				if (tileBoard.at(index).right->_tileType == mine) // right 
				{
					minesNear++;

				}
			}

			// Three Below
			if (tileBoard.at(index).botLeft != nullptr)
			{
				if (tileBoard.at(index).botLeft->_tileType == mine) // bot left
				{
					minesNear++;

				}
			}
			if (tileBoard.at(index).bot != nullptr)
			{
				if (tileBoard.at(index).bot->_tileType == mine) // bot 
				{
					minesNear++;

				}
			}
			if (tileBoard.at(index).botRight != nullptr)
			{
				if (tileBoard.at(index).botRight->_tileType == mine) // bot right
				{
					minesNear++;
				}
			}

			// Check current tile
			if (tileBoard.at(index)._tileType == mine)
			{
				minesNear = mine;
			}

			tileBoard.at(index)._tileType = minesNear;

			index++;
		}
	}
}

void board::tile::revealTile(board &game)
{
	game.currentBoard.at(_index) = _tileType;

	if (_tileType == 0) {
		// Three Above
		if (topLeft != nullptr)
		{
			if (topLeft->_tileType != mine && game.currentBoard.at(topLeft->_index) == tile_hidden)
			{
				game.currentBoard.at(topLeft->_index) = topLeft->_tileType;
				if (topLeft->_tileType == tile_revealed)
				{
					topLeft->revealTile(game);
				}
			}
		}
		if (top != nullptr)
		{
			if (top->_tileType != mine && game.currentBoard.at(top->_index) == tile_hidden)
			{
				game.currentBoard.at(top->_index) = top->_tileType;
				if (top->_tileType == tile_revealed)
				{
					top->revealTile(game);
				}
			}
		}
		if (topRight != nullptr)
		{
			if (topRight->_tileType != mine && game.currentBoard.at(topRight->_index) == tile_hidden)
			{
				game.currentBoard.at(topRight->_index) = topRight->_tileType;
				if (topRight->_tileType == tile_revealed)
				{
					topRight->revealTile(game);
				}
			}
		}

		// Same Row
		if (left != nullptr)
		{
			if (left->_tileType != mine && game.currentBoard.at(left->_index) == tile_hidden)
			{
				game.currentBoard.at(left->_index) = left->_tileType;
				if (left->_tileType == tile_revealed)
				{
					left->revealTile(game);
				}
			}
		}
		if (right != nullptr)
		{
			if (right->_tileType != mine && game.currentBoard.at(right->_index) == tile_hidden)
			{
				game.currentBoard.at(right->_index) = right->_tileType;
				if (right->_tileType == tile_revealed)
				{
					right->revealTile(game);
				}
			}
		}

		// Three Below
		if (botLeft != nullptr)
		{
			if (botLeft->_tileType != mine && game.currentBoard.at(botLeft->_index) == tile_hidden)
			{
				game.currentBoard.at(botLeft->_index) = botLeft->_tileType;
				if (botLeft->_tileType == tile_revealed)
				{
					botLeft->revealTile(game);
				}
			}
		}
		if (bot != nullptr)
		{
			if (bot->_tileType != mine && game.currentBoard.at(bot->_index) == tile_hidden)
			{
				game.currentBoard.at(bot->_index) = bot->_tileType;
				if (bot->_tileType == tile_revealed)
				{
					bot->revealTile(game);
				}
			}
		}
		if (botRight != nullptr)
		{
			if (botRight->_tileType != mine && game.currentBoard.at(botRight->_index) == tile_hidden)
			{
				game.currentBoard.at(botRight->_index) = botRight->_tileType;
				if (botRight->_tileType == tile_revealed)
				{
					botRight->revealTile(game);
				}
			}
		}
	}
	
}

