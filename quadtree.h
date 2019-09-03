#pragma once

#include "geometry.h"

#include <vector>
#include <array>
#include <utility>
#include <functional>
#include <queue>

class QuadTree {
public:
	size_t MaxRecursion;
	size_t MaxNodeCount;

	typedef std::function<bool(const std::vector<size_t>& /* path */)> CheckerFunc;
	typedef std::function<void(const std::vector<size_t>& /* path */, bool /* isLeaf */)> CallableFunc;
	
	// Represents tree node (aka 1 square)
	// Childs = { NorthWest, NorthEast, SouthEast, SouthWest }
	struct Node {
		std::array<size_t, 4> Childs;
		Node() : Childs({0, 0, 0, 0}) {}
	};

	QuadTree(size_t maxRecursion = 10, size_t maxNodeCount = 0) : MaxRecursion(maxRecursion), MaxNodeCount(maxNodeCount), Tree(1) {}

	size_t GetNodeIndex(const std::vector<size_t>& path) const;
	Node& GetNode(const std::vector<size_t>& path);
	Node GetNode(const std::vector<size_t>& path) const;

	// Starts building the tree
	void Run(CheckerFunc Checker);

	// Building the tree, but going breadth-first
	void RunBF(CheckerFunc Checker);

	// Calls Callable for every node
	void TraversalCall(CallableFunc Callable) const;

	// Force split a node with such path
	void Split(const std::vector<size_t>& path);

	// Builds tree, but starts only in leafs
	void ContinueRun(CheckerFunc Checker);

	// Builds tree, but starts only in leafs (breadth-first)
	void ContinueRunBF(CheckerFunc Checker);

	// Just raw tree without any calculations
	std::vector<Node>& GetRawTree();
	std::vector<Node> GetRawTree() const;

	void Clear();

private:
	void Run(size_t index, std::vector<size_t>& path, CheckerFunc Checker);
	void TraversalCall(size_t index, std::vector<size_t>& path, CallableFunc Callable) const;

	void Push(size_t index);

	std::vector<Node> Tree;
};

namespace QuadTreeUtility {
	struct CoordedNode {
		Geometry::Vector Pos;
		double Size;
	};

	typedef std::function<bool(const CoordedNode& /* node */)> CoordCheckerFunc;
	typedef std::function<void(const CoordedNode& /* node */, bool /* isLeaf */)> CoordCallableFunc;

	QuadTree::CheckerFunc CoordChecker(CoordCheckerFunc Checker);
	QuadTree::CallableFunc CoordCallable(CoordCallableFunc Callable);
}
