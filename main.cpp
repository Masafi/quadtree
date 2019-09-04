#include <iostream>
#include <thread>
#include <chrono>
#include "quadtree.h"
#include "render.h"

bool Checker(const QuadTreeUtility::CoordedNode& node, Geometry::Scene& scene) {
	Geometry::Vector a(node.Pos.x - node.Size, node.Pos.y - node.Size);
	Geometry::Vector b(node.Pos.x + node.Size, node.Pos.y + node.Size);
	return scene.IntersectAABB(a, b);
}

void RunTree(QuadTree& tree, Render::Window& renderScene, Geometry::Scene& scene) {
	QuadTree::CheckerFunc checker = QuadTreeUtility::CoordChecker([&renderScene, &scene](const QuadTreeUtility::CoordedNode& node) {
		std::this_thread::sleep_for(std::chrono::duration<int, std::milli>(int(std::max((std::log2(node.Size) + 100.0) * 0.1, 0.0))));
		renderScene.Objects.push_back(Render::WindowedObject(Render::CreateShape(node), node.Pos - Geometry::Vector(node.Size, node.Size)));
		if (!Checker(node, scene)) {
			return false;
		}
		return true;
	});

	tree.RunBF(checker);
	std::cout << "Ended tree building" << std::endl;
}

Geometry::Scene CreateScene() {
	Geometry::Scene result;
	Geometry::Polygon a;
	a.Vertexes = { Geometry::Vector(0.3, 0.7), Geometry::Vector(0.6, 0.5), Geometry::Vector(0.4, 0.3) };
	result.Objects.push_back(a);
	a.Vertexes.clear();
	a.Vertexes = { Geometry::Vector(0.1, 0.1), Geometry::Vector(0.2, 0.1), Geometry::Vector(0.3, 0.2), Geometry::Vector(0.3, 0.5), Geometry::Vector(0.05, 0.6) };
	result.Objects.push_back(a);
	a.Vertexes.clear();
	return result;
}

int main() {
	sf::RenderWindow Window(sf::VideoMode(1000, 1000), "QuadTree");
	Render::Window RenderScene;
	RenderScene.Objects.reserve(1e5);
	Geometry::Scene Scene = CreateScene();
	QuadTree Tree(10, 2e5);
	std::thread TreeThread(RunTree, std::ref(Tree), std::ref(RenderScene), std::ref(Scene));
	MantainWindow(Window, RenderScene);
	TreeThread.join();
	return 0;
}