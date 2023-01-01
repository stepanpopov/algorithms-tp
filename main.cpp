#include <iostream>
#include <array>
#include <cassert>
#include <string>
#include <vector>
#include <queue>
#include <unordered_map>
#include <algorithm>
#include <set>

const int SideSize = 4;
const int FieldSize = SideSize * SideSize;
const std::array<char, FieldSize> FinishField({1,  2,  3,  4, 
										       5,  6,  7,  8, 
										       9,  10, 11, 12,
										       13, 14, 15, 0});

struct NextState;

class GameState {
public:
	GameState(const std::array<char, FieldSize>& field_);
	
	bool CanMoveLeft() const;
	bool CanMoveUp() const;
	bool CanMoveRight() const;
	bool CanMoveDown() const;

	GameState MoveLeft() const;
	GameState MoveUp() const;
	GameState MoveRight() const;
	GameState MoveDown() const;

	bool isFinish() const;

	std::vector<NextState> getNextStates() const;

    bool hasSolution() const;

	int Heuristic() const;
    int findNodeIndex(int value) const;

	bool operator == (const GameState& state) const { return this->field == state.field; }
	bool operator != (const GameState& state) const { return !(*this == state); }
	bool operator < (const GameState& state) const { return this->Heuristic() < state.Heuristic(); }

	friend struct StateHasher;
private:
	std::array<char, FieldSize> field;
	char zeroPos;
};

struct NextState {
	GameState state;
	char move;
};

GameState::GameState(const std::array<char, FieldSize>& field_) : 	field(field_), 
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

bool GameState::hasSolution() const {
    int invCount = 0;
    for (int i = 0; i < FieldSize; ++i) {
        for (int j = i + 1; j < FieldSize; ++j) {
            if (field[j] == 0) {
                continue;
            } else if (field[i] > field[j]) {
                invCount++;
            }
        }
    }
    int zeroRow = (zeroPos / SideSize) + 1;

    if ((invCount + zeroRow) % 2 == 0) {
        return true;
    } else {
        return false;
    }
}

int GameState::Heuristic() const {
    int manhDistanceSum = 0;
    for (int i = 0; i < FieldSize; ++i) {
        int truePos = -1;
        if (field[i] == 0) {
            truePos = FieldSize - 1;
        } else {
            truePos = field[i] - 1;
        }

        manhDistanceSum += std::abs(truePos / SideSize - i / SideSize) +
                           std::abs(truePos % SideSize - i % SideSize);
    }
    return manhDistanceSum;
}

std::vector<NextState> GameState::getNextStates() const {
	std::vector<NextState> nextStates;
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


// BEST FIRST SEARCH
class StateHasher {
public:
	unsigned int operator() (const GameState& state) const {
		unsigned int hash = 0;
		for (char i = 0; i < FieldSize; ++i) {
			hash = hash * 127 + state.field[i];
		}
		return hash;
	}
};

std::string getSolution(const GameState& state) {
    if (!state.hasSolution()) {
        return "-1";
    }

	std::set<GameState> queue;
	queue.insert(state);
	std::unordered_map<GameState, char, StateHasher> visited;
	visited[state] = 'S';
	bool hasSolution = false;
	while (!queue.empty()) {
		GameState current = *queue.begin();
		queue.erase(queue.begin());
		if (current.isFinish()) {
			hasSolution = true;
			break;
		}

		std::vector<NextState> nextStates = current.getNextStates();
		for (const NextState &nextState : nextStates) {
			if (visited.find(nextState.state) == visited.end()) {
				visited[nextState.state] = nextState.move;
				queue.insert(nextState.state);
			}
		}
	}

	assert(hasSolution);
	std::string result;
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

void test(const std::array<char, FieldSize> &startField, const std::string &way) {
    GameState current(startField);
    for (char move : way) {
        switch (move) {
			case 'L':
				current = current.MoveLeft();
				break;
			case 'U':
				current = current.MoveUp();
				break;
			case 'R':
				current = current.MoveRight();
				break;
			case 'D':
				current = current.MoveDown();
				break;
			default:
                assert(false);
				break;
		}
    }
    assert(current.isFinish());
}

int main() {
    std::array<char, 16> startField;
    for (char i = 0; i < 16; ++i) {
        int x;
        std::cin >> x;
        startField[i] = x;
    }

    GameState state(startField);
    std::string result = getSolution(state);
    if (result != "-1") {
        std::cout << result.size() << std::endl;
    } 
	std::cout << result << std::endl;


    if (result != "-1") {
        test(startField, result);
    }

	return 0;
}
