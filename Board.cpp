#include "Board.h"
#include "Random.h"
#include <fstream>
#include <iostream>
#include <iomanip>
#include <sstream>
#include "TextureManager.h"
#include "MouseClickHandler.h"

enum states{playing, won, lost};

void Board::InitMap()
{
	buttons["face"] = (width - 12) * 32.0f;
	buttons["debug"] = (width - 8) * 32.0f;
	buttons["test_1"] = (width - 6) * 32.0f;
	buttons["test_2"] = (width - 4) * 32.0f;
	buttons["test_3"] = (width - 2) * 32.0f;
}

void Board::toggledebug()
{
	if (debugmode)
	{
		debugmode = false;
	}
	else
	{
		debugmode = true;
	}
}

Cell* Board::getCell(unsigned int i,unsigned int j)
{
	if (j >= 0 && j < width && i >= 0 && i < height)
	{
		unsigned int index = i * width + j;
		return &cells[index];
	}
	else
	{
		return nullptr;
	}
		
}

void Board::RecursiveShow(Cell* cell)
{
	cell->show();
	if(cellsleft!=0)
		cellsleft--;
	if (cell->countAdjacentMines() == 0)
	{
		for (unsigned int i = 0; i < 8; i++)
		{
			if(cell->adjacent[i]!=nullptr && !cell->adjacent[i]->isFlagged() && !cell->adjacent[i]->isShown())
				RecursiveShow(cell->adjacent[i]);
		}
	}
}

void Board::ShowAllMines()
{
	for (unsigned int i = 0; i < cells.size(); i++)
	{
		if (cells[i].isMine())
		{
			cells[i].show();
		}
	}
}

void Board::FlagAllMines()
{
	for (unsigned int i = 0; i < cells.size(); i++)
	{
		if (cells[i].isMine())
		{
			cells[i].toggleflag();
		}
	}
}



void Board::UpdateAdjacency(unsigned int i, unsigned int j)
{
	getCell(i,j)->setAdjacent(getCell((i - 1),(j)), getCell((i - 1),(j + 1)), getCell((i),(j + 1)), getCell((i + 1),(j + 1)), getCell((i + 1),(j)), getCell((i + 1),(j - 1)), getCell((i),(j - 1)), getCell((i - 1),(j - 1)));
}

void Board::setMines(unsigned int board)
{
	ifstream file;
	if (board !=0)
	{
		int i = 0;
		file.open("boards/testboard" + to_string(board) + ".brd");
		char temp;
		unsigned int newnmines = 0;
		while (file >> temp)
		{
			if (temp == '\n')
				continue;
			else if (temp == '1')
			{
				cells[i].setMine();
				newnmines++;
			}
			i++;
		}
		nmines = newnmines;

	}
	else
	{
		nmines = confignmines;
		unsigned int i = nmines;
		while (i > 0)
		{
			int index = Random::Int(0, width * height - 1);
			if (cells[index].isMine())
			{
				continue;
			}
			else
			{
				cells[index].setMine();
				i--;
			}
		}
	}
}

void Board::DrawDigits(sf::RenderWindow& window)
{
	auto texture=TextureManager::GetTexture("digits");
	std::ostringstream ss;
	if(displaynum>=0)
		ss << std::setw(3) << std::setfill('0') << displaynum;
	else
		ss << '-' << std::setw(3) << std::setfill('0') << -1*displaynum;
	string s = ss.str();
	for (size_t i = 0; i < s.size(); i++)
	{
		int temp;
		if (s[i] == '-')
			temp = 10;
		else
			temp = s[i] - '0';
		sf::Sprite sprite(texture, sf::IntRect(21*temp, 0, 21, 32));
		sprite.setPosition(15 + 21 * i, uiheight);
		window.draw(sprite);
	}
}

Board::Board(unsigned int width_, unsigned int height_, unsigned int nmines_)
{
	width = width_;
	height = height_;
	confignmines = nmines_;
	Initialize();
	InitMap();
}

Board::Board(string configfilename)
{
	ifstream file;
	file.open("boards/" + configfilename + ".cfg");
	file >> width;
	file >> height;
	file >> confignmines;
	Initialize();
	InitMap();
}

void Board::CreateWindow(sf::RenderWindow& window)
{
	window.create(sf::VideoMode(width * 32, height * 32 + 88), "Minesweeper");
}

void Board::Initialize(int board)
{
	uiheight = height * 32.0f;
	gamestate = 0;
	debugmode = false;
	vector<Cell> temp(width * height);
	for (unsigned int i = 0; i < width * height; i++)
	{
		Cell tempcell;
		temp[i] = tempcell;
	}
	cells = temp;
	setMines(board);
	cellsleft = cells.size() - nmines;
	displaynum = nmines;
	for (unsigned int i = 0; i < height; i++)
	{
		for (unsigned int j = 0; j < width; j++)
		{
			UpdateAdjacency(i, j);
			getCell(i, j)->countAdjacentMines(true);
		}
	}


}



void Board::Draw(sf::RenderWindow& window)
{

	
	for (unsigned int i = 0; i < height; i++)
	{
		for (unsigned int j = 0; j < width; j++)
		{
			getCell(i,j)->Draw(window, (float)j * 32.0f, (float)i * 32.0f,debugmode);
		}
	}	
	sf::Sprite face;
	if (gamestate == playing)
	{
		face.setTexture(TextureManager::GetTexture("face_happy"));
	}
	else if(gamestate == won)
	{
		face.setTexture(TextureManager::GetTexture("face_win"));
	}
	else
	{
		face.setTexture(TextureManager::GetTexture("face_lose"));
	}

	face.setPosition(buttons["face"], uiheight);
	//TODO make this the right locatino
	window.draw(face);

	sf::Sprite test1(TextureManager::GetTexture("test_1"));
	sf::Sprite test2(TextureManager::GetTexture("test_2"));
	sf::Sprite test3(TextureManager::GetTexture("test_3"));

	test1.setPosition(buttons["test_1"], uiheight);
	test2.setPosition(buttons["test_2"], uiheight);
	test3.setPosition(buttons["test_3"], uiheight);

	window.draw(test1);
	window.draw(test2);
	window.draw(test3);

	sf::Sprite debug(TextureManager::GetTexture("debug"));
	debug.setPosition(buttons["debug"], uiheight);
	window.draw(debug);

	DrawDigits(window);
}


void Board::Update(sf::Event event, sf::RenderWindow& window)
{
	if (MouseClickHandler::isMouseClick(event))
	{
		auto mousePosition = sf::Mouse::getPosition(window);
		if (mousePosition.y > height * 32.0f)
		{
			if (MouseClickHandler::withinBounds(mousePosition,sf::Vector2i(buttons["face"],uiheight),64))
			{
				Initialize();
			}
			else if (MouseClickHandler::withinBounds(mousePosition, sf::Vector2i(buttons["test_1"], uiheight), 64))
			{
				Initialize(1);
			}
			else if (MouseClickHandler::withinBounds(mousePosition, sf::Vector2i(buttons["test_2"], uiheight), 64))
			{
				Initialize(2);
			}
			else if (MouseClickHandler::withinBounds(mousePosition, sf::Vector2i(buttons["test_3"], uiheight), 64))
			{
				Initialize(3);
			}
			else if (gamestate==playing && MouseClickHandler::withinBounds(mousePosition, sf::Vector2i(buttons["debug"], uiheight), 64))
			{
				toggledebug();
			}
		}
		else if(gamestate == playing)
		{
			unsigned int j = floor(mousePosition.x / 32.0f);
			unsigned int i = floor(mousePosition.y / 32.0f);
			if (!getCell(i, j)->isFlagged() && !getCell(i,j)->isShown() && event.mouseButton.button == sf::Mouse::Left)
			{
				if (getCell(i, j)->isMine())
				{
					gamestate = lost;
					ShowAllMines();
				}
				else 
				{
					RecursiveShow(getCell(i, j));
				}
				if (cellsleft == 0)
				{
					gamestate = won;
					FlagAllMines();
					if(debugmode)
						toggledebug();
				}
					
				//TODO fix a problem with the thing not properly counting mines left and making you win prematurely
			}
			else if (event.mouseButton.button == sf::Mouse::Right && !getCell(i,j)->isShown())
			{
				getCell(i, j)->toggleflag();
				if (getCell(i, j)->isFlagged())
					displaynum--;
				else
					displaynum++;
			}
		}
		
	}
}


