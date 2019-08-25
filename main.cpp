#include <SFML/Graphics.hpp>
#include <iostream>
#include "quadtree.h"

int main() {
	sf::RenderWindow window(sf::VideoMode(1000, 1000), "QuadTree");

	std::vector<sf::RectangleShape> Draw;

	QuadTree::CheckerFunc checker = QuadTree::CoordChecker([](const QuadTree::CoordedNode& node) {
		auto checkP = [](float x, float y) {
			return int((x - 0.5) * (x - 0.5) + (y - 0.5) * (y - 0.5) < 0.25 * 0.25);
		};
		bool good = (
			checkP(node.CenterX - node.Size / 2, node.CenterY - node.Size / 2) +
			checkP(node.CenterX - node.Size / 2, node.CenterY + node.Size / 2) +
			checkP(node.CenterX + node.Size / 2, node.CenterY - node.Size / 2) +
			checkP(node.CenterX + node.Size / 2, node.CenterY + node.Size / 2) +
			checkP(node.CenterX, node.CenterY)
		) % 5 != 0;
		return good;
	});

	QuadTree::CallableFunc callable = QuadTree::CoordCallable([&Draw](const QuadTree::CoordedNode& node, bool isLeaf) {
		auto checkP = [](float x, float y) {
			return int((x - 0.5) * (x - 0.5) + (y - 0.5) * (y - 0.5) <= 0.25 * 0.25);
		};
		if (isLeaf) {
			QuadTree::CoordedNode shifted = node;
			shifted.CenterX -= shifted.Size / 2;
			shifted.CenterY -= shifted.Size / 2;

			sf::RectangleShape rectangle;
			rectangle.setPosition(sf::Vector2f(shifted.CenterX * 3000 - 500, shifted.CenterY * 3000 - 500));
			rectangle.setSize(sf::Vector2f(shifted.Size * 3000, shifted.Size * 3000));
			if (checkP(node.CenterX, node.CenterY)) {
				rectangle.setFillColor(sf::Color(0, 0, 0, 255));
				rectangle.setFillColor(sf::Color(255, 255, 255, 0));
			}
			else {
				rectangle.setFillColor(sf::Color(255, 255, 255, 0));
			}
			rectangle.setOutlineColor(sf::Color(0, 0, 0, 255));
			rectangle.setOutlineThickness(1);
			Draw.push_back(rectangle);
		}
	});

	QuadTree tree(checker);
	tree.Run();
	tree.TraversalCall(callable);

	while (window.isOpen()) {
		sf::Event event;
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed)
				window.close();
		}

		window.clear(sf::Color::White);
		for (const auto& i : Draw) {
			window.draw(i);
		}
		window.display();
	}

	return 0;
}