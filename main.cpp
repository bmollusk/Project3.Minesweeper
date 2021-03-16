#include <SFML/Graphics.hpp>
#include "Board.h"
#include "Cell.h"
#include <unordered_map>
#include <iostream>

int main()
{


	unsigned int width = 20;
	unsigned int height = 15;
	
	sf::RenderWindow window;
	Board board("config");
	board.CreateWindow(window);
	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}
		window.clear();
		
		board.Update(event,window);

		board.Draw(window);

		window.display();
	}
	TextureManager::Clear();

	return 0;
}