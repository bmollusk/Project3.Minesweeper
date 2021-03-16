#include "Cell.h"
#include "TextureManager.h"
#include <vector>
#include <iostream>

enum directions {n,ne,e,se,s,sw,w,nw};



Cell::Cell(bool mine_)
{
	mine = mine_;
	shown = false;
	flagged = false;
	adjacentmines = 0;
	for (unsigned int i = 0; i < 8; i++)
	{
		adjacent[i] = nullptr;
	}
}

void Cell::setMine()
{
	mine = true;
}

bool Cell::isMine()
{
	return mine;
}
void Cell::show()
{
	shown = true;
}

bool Cell::isShown()
{
	return shown;
}

void Cell::toggleflag()
{
	if (flagged)
	{
		flagged = false;
	}
	else
	{
		flagged = true;
	}
}

bool Cell::isFlagged()
{
	return flagged;
}


unsigned int Cell::countAdjacentMines(bool update_)
{
	if (update_)
	{
		unsigned int temp = 0;
		for (unsigned int i = 0; i < 8; i++)
		{
			if (adjacent[i]!=nullptr && adjacent[i]->isMine())
				temp++;
		}
		adjacentmines = temp;
	}
	return adjacentmines;	
}
void Cell::setAdjacent(Cell* n_, Cell* ne_, Cell* e_, Cell* se_, Cell* s_, Cell* sw_, Cell* w_, Cell* nw_)
{
	adjacent[n] = n_;
	adjacent[ne] = ne_;
	adjacent[e] = e_;
	adjacent[se] = se_;
	adjacent[s] = s_;
	adjacent[sw] = sw_;
	adjacent[w] = w_;
	adjacent[nw] = nw_;
}



void Cell::Draw(sf::RenderWindow& window,float x, float y, bool debugmode)
{
	
	vector<sf::Sprite> cellsprites;
	if (shown)
	{
		sf::Sprite cellsprite(TextureManager::GetTexture("tile_revealed"));
		cellsprites.push_back(cellsprite);
		if (isFlagged())
		{
			sf::Sprite overlay(TextureManager::GetTexture("flag"));
			cellsprites.push_back(overlay);
		}
		if (isMine())
		{
			sf::Sprite overlay(TextureManager::GetTexture("mine"));
			cellsprites.push_back(overlay);
		}
		else if (countAdjacentMines() > 0)
		{
			sf::Sprite overlay(TextureManager::GetTexture("number_" + to_string(countAdjacentMines())));
			cellsprites.push_back(overlay);
		}
		
	}
	else
	{
		sf::Sprite cellsprite(TextureManager::GetTexture("tile_hidden"));
		cellsprites.push_back(cellsprite);
		if (isFlagged())
		{
			sf::Sprite overlay(TextureManager::GetTexture("flag"));
			cellsprites.push_back(overlay);
		}
	}
	if (debugmode && isMine())
	{
		sf::Sprite overlay(TextureManager::GetTexture("mine"));
		cellsprites.push_back(overlay);
	}
	for (unsigned int i = 0; i < cellsprites.size(); i++)
	{
		cellsprites[i].setPosition(x, y);
		window.draw(cellsprites[i]);
	}
	
	
}
