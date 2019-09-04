#include "quadtree.h"

void QuadTree::Run(size_t index, std::vector<size_t>& path, CheckerFunc Checker) {
	if (path.size() < MaxRecursion && Tree.size() < MaxNodeCount && Checker(path)) {
		Push(index);
		for (int i = 0; i < 4; i++) {
			path.push_back(i);
			Run(Tree[index].Childs[i], path, Checker);
			path.pop_back();
		}
	}
}

void QuadTree::TraversalCall(size_t index, std::vector<size_t>& path, CallableFunc Callable) const {
	if (Tree[index].Childs[0] == 0) {
		Callable(path, true);
		return;
	}
	Callable(path, false);
	for (size_t i = 0; i < 4; i++) {
		path.push_back(i);
		TraversalCall(Tree[index].Childs[i], path, Callable);
		path.pop_back();
	}
}

void QuadTree::Push(size_t index) {
	for (int i = 0; i < 4; i++) {
		size_t newIndex = Tree.size();

		Tree[index].Childs[i] = newIndex;
		Tree.emplace_back();
	}
}


size_t QuadTree::GetNodeIndex(const std::vector<size_t>& path) const {
	size_t index = 0;
	for (auto i : path) {
		index = Tree[index].Childs[i];
	}
	return index;
}

QuadTree::Node& QuadTree::GetNode(const std::vector<size_t>& path) {
	return Tree[GetNodeIndex(path)];
}

QuadTree::Node QuadTree::GetNode(const std::vector<size_t>& path) const {
	return Tree[GetNodeIndex(path)];
}

void QuadTree::Run(CheckerFunc Checker) {
	std::vector<size_t> path;
	Run(0, path, Checker);
}

void QuadTree::RunBF(CheckerFunc Checker) {
	std::queue<std::pair<size_t, std::vector<size_t>>> q;
	q.emplace(0, std::vector<size_t>());
	while (q.size() && (MaxNodeCount == 0 || Tree.size() < MaxNodeCount)) {
		size_t index = q.front().first;
		std::vector<size_t> path = q.front().second;
		// I wish mingw supported 
		// auto [index, path] = q.front();
		// :(
		q.pop();
		if (path.size() < MaxRecursion && Checker(path)) {
			Push(index);
			for (int i = 0; i < 4; i++) {
				path.push_back(i);
				q.emplace(Tree[index].Childs[i], path);
				path.pop_back();
			}
		}
	}
}

void QuadTree::TraversalCall(CallableFunc Callable) const {
	std::vector<size_t> path;
	TraversalCall(0, path, Callable);		
}

void QuadTree::Split(const std::vector<size_t>& path) {
	size_t index = GetNodeIndex(path);
	Push(index);
}

// Builds tree, but starts only in leafs
void QuadTree::ContinueRun(CheckerFunc Checker) {

}

// Builds tree, but starts only in leafs (breadth-first)
void QuadTree::ContinueRunBF(CheckerFunc Checker) {
	
}

std::vector<QuadTree::Node>& QuadTree::GetRawTree() {
	return Tree;
}

std::vector<QuadTree::Node> QuadTree::GetRawTree() const {
	return Tree;
}

void QuadTree::Clear() {
	Tree.clear();
}

namespace QuadTreeUtility {

CoordedNode GetCoordsFromPath(const std::vector<size_t>& path) {
	const std::array<std::pair<int, int>, 4> Alignment = {
		std::pair<int, int>(-1, -1),
		std::pair<int, int>(1, -1),
		std::pair<int, int>(1, 1),
		std::pair<int, int>(-1, 1)
	};
	CoordedNode node{Geometry::Vector(0.5, 0.5), 0.5};
	for (size_t div : path) {
		node.Size /= 2;
		node.Pos.x += Alignment[div].first * node.Size;
		node.Pos.y += Alignment[div].second * node.Size;
	}
	return node;
}

QuadTree::CheckerFunc CoordChecker(CoordCheckerFunc Checker) {
	return [Checker](const std::vector<size_t>& path) -> bool {
		return Checker(GetCoordsFromPath(path));
	};
}

QuadTree::CallableFunc CoordCallable(CoordCallableFunc Callable) {
	return [Callable](const std::vector<size_t>& path, bool isLeaf) {
		Callable(GetCoordsFromPath(path), isLeaf);
	};
}

}
