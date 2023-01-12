#include <iostream>
#include <array>
#include <cassert>
#include <string>
#include <vector>
#include <queue>
#include <unordered_map>
#include <algorithm>
#include <set>

using std::array;
using std::vector;
using std::string;
using std::queue;
using std::unordered_map;
using std::set;

const int SideSize = 4;
const int FieldSize = SideSize * SideSize;
const array<char, FieldSize> FinishField({1, 2, 3, 4, 
										  5, 6, 7, 8, 
										  9, 10, 11, 12,
										  13, 14, 15, 0});

struct NextState {
	GameState state;
	char move;
};

class GameState {
public:
	GameState(const array<char, FieldSize>& field_);
	
	bool CanMoveLeft() const;
	bool CanMoveUp() const;
	bool CanMoveRight() const;
	bool CanMoveDown() const;

	GameState MoveLeft() const;
	GameState MoveUp() const;
	GameState MoveRight() const;
	GameState MoveDown() const;

	bool isFinish() const;

	vector<NextState> getNextStates() const;

	int getHeuristic() const;
	int findNodeIndex(int value) const;

	bool operator == (const GameState& state) const { return this->field == state.field; }
	bool operator != (const GameState& state) const { return !(*this == state); }
	bool operator < (const GameState& state) const { return this->getHeuristic() < state.getHeuristic(); }

    size_t Evristic();

	friend struct StateHasher;
	friend std::ostream& operator << (std::ostream& out, const GameState& state) {
		for (int y = 0; y < SideSize; ++y) {
			for (int x = 0; x < SideSize; ++x) {
				out << static_cast<int>(state.field[y * SideSize + x])<< " ";
			}

			out << std::endl;
		}

		out << std::endl;
		return out;
	}

private:
	array<char, FieldSize> field;
	char zeroPos;
};

GameState::GameState(const array<char, FieldSize>& field_) : 	field(field_), 
																zeroPos(-1) {
	for (size_t i = 0; i < field.size(); ++i) {
		if (field[i] == 0) {
			zeroPos = i;
			break;
		}
	}

	assert(zeroPos != -1);
}

bool GameState::CanMoveLeft() const {
	return zeroPos % SideSize < SideSize - 1;
}

bool GameState::CanMoveUp() const {
	return (zeroPos + SideSize) < FieldSize;
}

bool GameState::CanMoveRight() const {
	return zeroPos % SideSize > 0;
}

bool GameState::CanMoveDown() const {
	return (zeroPos - SideSize) >= 0;
}

GameState GameState::MoveLeft() const {
	assert(CanMoveLeft());
	GameState newState(*this);
	std::swap(newState.field[newState.zeroPos + 1], newState.field[newState.zeroPos]);
	++newState.zeroPos;

	return newState;
}

GameState GameState::MoveUp() const {
	assert(CanMoveUp());
	GameState newState(*this);
	std::swap(newState.field[newState.zeroPos + SideSize], newState.field[newState.zeroPos]);
	newState.zeroPos += SideSize;

	return newState;
}

GameState GameState::MoveRight() const {
	assert(CanMoveRight());
	GameState newState(*this);
	std::swap(newState.field[newState.zeroPos - 1], newState.field[newState.zeroPos]);
	--newState.zeroPos;

	return newState;
}

GameState GameState::MoveDown() const {
	assert(CanMoveDown());
	GameState newState(*this);
	std::swap(newState.field[newState.zeroPos - SideSize], newState.field[newState.zeroPos]);
	newState.zeroPos -= SideSize;

	return newState;
}

bool GameState::isFinish() const {
	return field == FinishField;
}

int GameState::findNodeIndex(int value) const {
	if (value == 0)
		return FieldSize - 1;

	return value - 1;
}

vector<NextState> GameState::getNextStates() const {
	vector<NextState> nextStates;
	if (CanMoveLeft()) {
		nextStates.push_back({MoveLeft(), 'L'});
	}

	if (CanMoveUp()) {
		nextStates.push_back({MoveUp(), 'U'});
	}

	if (CanMoveRight()) {
		nextStates.push_back({MoveRight(), 'R'});
	}

	if (CanMoveDown()) {
		nextStates.push_back({MoveDown(), 'D'});
	}

	return nextStates;
}

int GameState::getHeuristic() const {
	int generalHeuristic = 0;
	for (int i = 0; i < FieldSize; ++i) {
		// Ситуация нахождения костяшки на чужом месте
		int finalPosition = findNodeIndex(field[i]);
		if (i != finalPosition) {
			generalHeuristic += (abs(i / SideSize - finalPosition / SideSize) + abs(i % SideSize - finalPosition % SideSize));
		}

		int linearConflicts = 0;

		for (int j = i + 1; j < SideSize; ++j) {
			int secondFinalPos = findNodeIndex(field[j]);
			if ((i / SideSize == finalPosition / SideSize) && (j / SideSize == secondFinalPos / SideSize)) {
				if (field[i] > field[j]) linearConflicts += 2;
			}
		}

		for (int j = i + SideSize; j < FieldSize; j += SideSize) {
			int secondFinalPos = findNodeIndex(field[j]);
			if ((i % SideSize == finalPosition % SideSize) && (j % SideSize == secondFinalPos % SideSize)) {
				if (field[i] > field[j]) linearConflicts += 2;
			}
		}

		generalHeuristic += linearConflicts;

		if (linearConflicts == 0) {
			if (((i == 1 && field[i] == 2) || (i == SideSize && field[i] == SideSize + 1)) && (findNodeIndex(1) != 0)) {
				generalHeuristic += 2;
			}
			else if (((i == SideSize - 2 && field[i] == SideSize) || (i == 2 * SideSize - 1 && field[i] == 2 * SideSize)) && (findNodeIndex(SideSize) != SideSize - 1)) {
				generalHeuristic += 2;
			}
			else if (((i == SideSize * (SideSize - 2) && field[i] == SideSize * (SideSize - 2) + 1) || (i == SideSize * (SideSize - 1) + 1 && field[i] == SideSize * (SideSize - 1) + 2)) && (findNodeIndex(SideSize * (SideSize - 1) + 1) != SideSize * (SideSize - 1))) {
				generalHeuristic += 2;
			}
		}
	}

	return generalHeuristic;
}


class StateHasher {
private:
	static const size_t factor = 37;
public:
	size_t operator() (const GameState& state) const {
		size_t hash = 0;
		for (size_t i = 0; i < FieldSize; ++i) {
			hash = hash * factor + size_t(state.field[i]);
		}
		return hash;
	}
};

string getSolution(const GameState& state) {
	set<GameState> queue;
	queue.insert(state);
	unordered_map<GameState, char, StateHasher> visited;
	visited[state] = 'S';
	bool hasSolution = false;
	while (!queue.empty()) {
		GameState current = *queue.begin();
		queue.erase(queue.begin());
		if (current.isFinish()) {
			hasSolution = true;
			break;
		}

		vector<NextState> nextStates = current.getNextStates();
		for (const NextState &nextState : nextStates) {
			if (visited.find(nextState.state) == visited.end()) {
				visited[nextState.state] = nextState.move;
				queue.insert(nextState.state);
			}
		}
	}

	assert(hasSolution);
	string result;
	GameState current(FinishField);
	char move = visited[current];
	while (move != 'S') {
		result += move;
		switch (move) {
			case 'L':
				current = current.MoveRight();
				break;
			case 'U':
				current = current.MoveDown();
				break;
			case 'R':
				current = current.MoveLeft();
				break;
			case 'D':
				current = current.MoveUp();
				break;
			default:
				break;
		}

		move = visited[current];
	}

	std::reverse(result.begin(), result.end());
	return result;
}

int main() {
	GameState state({3, 4, 6, 1, 8, 0, 2, 5, 7});
	string result = getSolution(state);
	std::cout << result.size() << std::endl;
	std::cout << result;
	
	/* for (char move : result) {
		switch (move) {
		case 'L':
			state = state.MoveLeft();
			break;
		case 'U':
			state = state.MoveUp();
			break;
		case 'R':
			state = state.MoveRight();
			break;
		case 'D':
			state = state.MoveDown();
			break;
		default:
			assert(false);
		}

		std::cout << state;
	} */

	return 0;
}
