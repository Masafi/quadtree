#include <SFML/Graphics.hpp>
#include <iostream>
#include <thread>
#include <chrono>
#include "quadtree.h"

sf::RectangleShape DrawCoordedNode(const QuadTreeUtility::CoordedNode& node) {
	sf::RectangleShape rectangle;
	rectangle.setPosition(sf::Vector2f((node.Pos.x - node.Size / 2) * 800 + 100, (node.Pos.y - node.Size / 2) * 800 + 100));
	rectangle.setSize(sf::Vector2f(node.Size * 800, node.Size * 800));
	rectangle.setFillColor(sf::Color(255, 255, 255, 0));
	rectangle.setOutlineColor(sf::Color(0, 0, 0, 255));
	rectangle.setOutlineThickness(1);
	return rectangle;
}

void MantainWindow(sf::RenderWindow& window, std::vector<sf::RectangleShape>& buffer) {
	while (window.isOpen()) {
		sf::Event event;
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed)
				window.close();
		}

		window.clear(sf::Color::White);
		for (int i = 0; i < buffer.size(); i++) {
			window.draw(buffer[i]);
		}
		window.display();
	}
}

bool Checker(const QuadTreeUtility::CoordedNode& node, Geometry::Scene& scene) {
	bool good = (
		scene.PointIn(Geometry::Vector(node.Pos.x - node.Size / 2, node.Pos.y - node.Size / 2)) +
		scene.PointIn(Geometry::Vector(node.Pos.x - node.Size / 2, node.Pos.y + node.Size / 2)) +
		scene.PointIn(Geometry::Vector(node.Pos.x + node.Size / 2, node.Pos.y - node.Size / 2)) +
		scene.PointIn(Geometry::Vector(node.Pos.x + node.Size / 2, node.Pos.y + node.Size / 2)) +
		scene.PointIn(Geometry::Vector(node.Pos.x, node.Pos.y))
	) % 5 != 0;
	return good;
}

void RunTree(QuadTree& tree, std::vector<sf::RectangleShape>& buffer, Geometry::Scene& scene) {
	QuadTree::CheckerFunc checker = QuadTreeUtility::CoordChecker([&buffer, &scene](const QuadTreeUtility::CoordedNode& node) {
		//std::this_thread::sleep_for(std::chrono::duration<int, std::milli>(int(std::max((std::log2(node.Size) + 100.0) * 0.1, 0.0))));
		if (!Checker(node, scene)) {
			buffer.push_back(DrawCoordedNode(node));
			return false;
		}
		return true;
	});

	tree.RunBF(checker);
}

Geometry::Scene CreateScene() {
	Geometry::Scene result;
	Geometry::Polygon a;
	a.Vertexes = { Geometry::Vector(0.4, 0.3), Geometry::Vector(0.6, 0.5), Geometry::Vector(0.3, 0.7) };
	if (!a.IsConvex()) {
		std::cout << "loh pidr" << std::endl;
	}
	result.Objects.push_back(a);
	return result;
}

int main() {
	sf::RenderWindow Window(sf::VideoMode(1000, 1000), "QuadTree");
	std::vector<sf::RectangleShape> SquareBuffer;
	SquareBuffer.reserve(1e5);
	Geometry::Scene Scene = CreateScene();
	QuadTree Tree(40, 1e5);
	std::thread TreeThread(RunTree, std::ref(Tree), std::ref(SquareBuffer), std::ref(Scene));
	MantainWindow(Window, SquareBuffer);
	TreeThread.detach();
	return 0;
}