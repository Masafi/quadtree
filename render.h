#pragma once

#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>
#include <utility>
#include "quadtree.h"

namespace Render {

// world window, not gui window

class Window;

class WindowedObject {
public:
	std::unique_ptr<sf::Shape> Object;
	Geometry::Vector Pos;
	Geometry::Vector Size;

	WindowedObject(sf::Shape* object, Geometry::Vector pos = Geometry::Vector(), Geometry::Vector size = Geometry::Vector()) : Object(object), Pos(pos), Size(size) {}

	void Update(const Window& window);
};

class Window {
public:
	std::vector<WindowedObject> Objects;
	Geometry::Vector Center;
	Geometry::Vector Size;
	Geometry::Vector RenderSize; //size of render surface in pixels

	Window() : Center(0.5, 0.5), Size(1, 1), RenderSize(1000, 1000) {}

	// 1 - in, -1 - out
	void Zoom(int dir);
	// 0 top, 1 right, 2 bottom, 3 left
	void Move(int dir);

	void Update();
	void Render(sf::RenderWindow& window);
};

sf::RectangleShape* CreateShape(const QuadTreeUtility::CoordedNode& node);

void MantainWindow(sf::RenderWindow& window, Window& renderScene);

}