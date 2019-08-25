#include <vector>
#include <array>
#include <utility>
#include <functional>

class QuadTree {
public:
	typedef std::function<bool(const std::vector<size_t>& /* path */)> CheckerFunc;
	typedef std::function<void(const std::vector<size_t>& /* path */, bool /* isLeaf */)> CallableFunc;
	
	// Represents tree node (aka 1 square)
	// Childs = { NorthWest, NorthEast, SouthEast, SouthWest }
	struct Node {
		std::array<size_t, 4> Childs;
		Node() : Childs({0, 0, 0, 0}) {}
	};

private:
	void Run(size_t index, std::vector<size_t>& path) {
		if (path.size() < MaxRecursion && Checker(path)) {
			for (int i = 0; i < 4; i++) {
				size_t newIndex = Tree.size();
				
				Tree[index].Childs[i] = newIndex;
				Tree.emplace_back();
				
				path.push_back(i);
				Run(newIndex, path);
				path.pop_back();
			}
		}
	}

	void TraversalCall(size_t index, std::vector<size_t>& path, CallableFunc Callable) {
		if (Tree[index].Childs[0] == 0) {
			Callable(path, true);
			return;
		}
		Callable(path, false);
		for (size_t i = 0; i < Tree[index].Childs.size(); i++) {
			path.push_back(i);
			TraversalCall(Tree[index].Childs[i], path, Callable);
			path.pop_back();
		}
	}

public:
	QuadTree(CheckerFunc checker, size_t maxRecursion = 10) :
		Tree(1),
		Checker(checker),
		MaxRecursion(maxRecursion)
		{}

	// Starts building the tree
	void Run() {
		std::vector<size_t> path;
		Run(0, path);
	}

	// Just raw tree without any calculations
	std::vector<Node> GetRawTree() {
		return Tree;
	}

	// Calls Callable for every node
	void TraversalCall(CallableFunc Callable) {
		std::vector<size_t> path;
		TraversalCall(0, path, Callable);		
	}

// Utility examples

	struct CoordedNode {
		float CenterX;
		float CenterY;
		float Size;
	};

	typedef std::function<bool(const CoordedNode& /* node */)> CoordCheckerFunc;
	typedef std::function<void(const CoordedNode& /* node */, bool /* isLeaf */)> CoordCallableFunc;

	static CoordedNode GetCoordsFromPath(const std::vector<size_t>& path) {
		const std::array<std::pair<int, int>, 4> Alignment = {
			std::pair<int, int>(-1, -1),
			std::pair<int, int>(1, -1),
			std::pair<int, int>(1, 1),
			std::pair<int, int>(-1, 1)
		};
		CoordedNode node{0.5, 0.5, 1};
		for (size_t div : path) {
			node.Size /= 2;
			node.CenterX += Alignment[div].first * node.Size / 2;
			node.CenterY += Alignment[div].second * node.Size / 2;
		}
		return node;
	}

	static CheckerFunc CoordChecker(CoordCheckerFunc Checker) {
		return [Checker](const std::vector<size_t>& path) -> bool {
			return Checker(GetCoordsFromPath(path));
		};
	}

	static CallableFunc CoordCallable(CoordCallableFunc Callable) {
		return [Callable](const std::vector<size_t>& path, bool isLeaf) {
			Callable(GetCoordsFromPath(path), isLeaf);
		};
	}

private:
	std::vector<Node> Tree;
	CheckerFunc Checker;
	size_t MaxRecursion;
};
