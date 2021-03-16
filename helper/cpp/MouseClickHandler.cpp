#include "MouseClickHandler.h"
bool MouseClickHandler::consumed = false;
bool MouseClickHandler::isMouseClick(sf::Event event)
{
	if (event.type == sf::Event::MouseButtonPressed && !consumed)
	{
		consumed = true;
		return true;
	}
	else if(event.type == sf::Event::MouseButtonReleased)
	{
		consumed = false;
	}
	return false;
}

bool MouseClickHandler::withinBounds(sf::Vector2i position, sf::Vector2i blockpos, int size)
{
	return (position.x >= blockpos.x && position.x <= blockpos.x+size && position.y >= blockpos.y && position.y <= blockpos.y + size);
}
