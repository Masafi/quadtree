#include "render.h"
#include <iostream>

namespace Render {

void WindowedObject::Update(const Window& window) {
	Geometry::Vector t = (Pos - (window.Center - window.Size)) / 2;
	t = Geometry::Vector(t.x * window.RenderSize.x / window.Size.x, t.y * window.RenderSize.y / window.Size.y);
	Object->setPosition(t.x, t.y);
	Object->setScale(0.5 * window.RenderSize.x / window.Size.x, 0.5 * window.RenderSize.y / window.Size.y);
	Object->setOutlineThickness(-0.002 * window.Size.x);
}

void Window::Zoom(int dir) {
	if (dir > 0) {
		Size = Size / 1.1;
	}
	else {
		Size = Size * 1.1;
	}
}

void Window::Move(int dir) {
	Geometry::Vector sgn;
	if (dir == 0) {
		sgn = Geometry::Vector(0, -1);
	}
	else if (dir == 1) {
		sgn = Geometry::Vector(1, 0);
	}
	else if (dir == 2) {
		sgn = Geometry::Vector(0, 1);
	}
	else {
		sgn = Geometry::Vector(-1, 0);
	}
	Center = Center + Geometry::Vector(Size.x * sgn.x, Size.y * sgn.y) / 10;
}

void Window::Update() {
	for (size_t i = 0; i < Objects.size(); i++) {
		Objects[i].Update(*this);
	}
}

void Window::Render(sf::RenderWindow& window) {
	for (size_t i = 0; i < Objects.size(); i++) {
		window.draw(*Objects[i].Object);
	}
}


sf::RectangleShape* CreateShape(const QuadTreeUtility::CoordedNode& node) {
	sf::RectangleShape* rectangle = new sf::RectangleShape();
	rectangle->setSize(sf::Vector2f(node.Size * 2, node.Size * 2));
	rectangle->setFillColor(sf::Color(255, 255, 255, 0));
	rectangle->setOutlineColor(sf::Color(0, 0, 0, 255));
	return rectangle;
}

void MantainWindow(sf::RenderWindow& window, Window& renderScene) {
	while (window.isOpen()) {
		sf::Event event;
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed)
				window.close();
			if (event.type == sf::Event::KeyPressed) {
				if (event.key.code == sf::Keyboard::Up)
					renderScene.Move(0);
				else if (event.key.code == sf::Keyboard::Right)
					renderScene.Move(1);
				else if (event.key.code == sf::Keyboard::Down)
					renderScene.Move(2);
				else if (event.key.code == sf::Keyboard::Left)
					renderScene.Move(3);
				else if (event.key.code == sf::Keyboard::Z)
					renderScene.Zoom(1);
				else if (event.key.code == sf::Keyboard::X)
					renderScene.Zoom(-1);
			}
		}

		window.clear(sf::Color::White);
		renderScene.Update();
		renderScene.Render(window);
		window.display();
	}
}

}