#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include "Cell.h"

class Board {
	unsigned int width;
	unsigned int height;
	unsigned int confignmines;
	unsigned int nmines;
	unsigned int cellsleft;
	unsigned int gamestate;
	int displaynum;
	bool debugmode;
	float uiheight;
	vector<Cell> cells;
	unordered_map<string, float> buttons;
	void InitMap();
	void toggledebug();
	Cell* getCell(unsigned int i,unsigned int j);
	void RecursiveShow(Cell* cell);
	void ShowAllMines();
	void FlagAllMines();
	void UpdateAdjacency(unsigned int i, unsigned int j);
	void setMines(unsigned int board = 0);
	void DrawDigits(sf::RenderWindow& window);
public:
	Board(unsigned int width_, unsigned int height_, unsigned int nmines_ = 0);
	Board(string configfilename);
	void Initialize(int board = 0);
	void CreateWindow(sf::RenderWindow& window);
	void Draw(sf::RenderWindow& window);
	void Update(sf::Event event, sf::RenderWindow& window);
};