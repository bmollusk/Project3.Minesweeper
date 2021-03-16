#pragma once
#include <SFML/Graphics.hpp>
class MouseClickHandler
{
	static bool consumed;
public:
	static bool isMouseClick(sf::Event event);
	static bool withinBounds(sf::Vector2i position, sf::Vector2i blockpos, int size);
};

