#pragma once
#include "TextureManager.h"

using namespace std;
class Cell {
	bool mine;
	bool shown;
	bool flagged;
	unsigned int adjacentmines;
	
public:
	Cell(bool mine_ = false);
	Cell* adjacent[8];
	void setMine();
	bool isMine();
	void show();
	bool isShown();
	void toggleflag();
	bool isFlagged();
	unsigned int countAdjacentMines(bool update_= false);
	void setAdjacent(Cell* n_, Cell* ne_, Cell* e_,Cell* se_, Cell* s_,Cell* sw_,Cell* w_,Cell* nw_);
	void Draw(sf::RenderWindow& window, float x, float y, bool debugmode = false);

};
