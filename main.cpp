#include <iostream>
#include <string>
#include <vector>
#include <algorithm>  // std::fill
#include <cassert>  // test()

static constexpr int HASH_TABLE_DEFAULT_SIZE = 8;
static constexpr float HASH_TABLE_MAX_KEYS_FILL = 0.75;
static constexpr float HASH_TABLE_MAX_DELETE_FILL = 0.25;


// HASHER
struct StringHasher {
	unsigned int operator()(const std::string& str) const {
		unsigned int hash = 0;
		for (unsigned int i = 0; i < str.length(); ++i) {
			hash = hash * 127 + str[i];
		}
		return hash;
	}
};


// HASH_TABLE
template <class T, class Hash>
class Hash_table {
public:
	Hash_table(const Hash& hasher = Hash()) : 	conditions(HASH_TABLE_DEFAULT_SIZE, EMPTY),
							keys(HASH_TABLE_DEFAULT_SIZE),
							hasher(hasher) {}

	Hash_table(const Hash_table& table) = default;

	Hash_table &operator=(const Hash_table& table) = default;

	~Hash_table() = default;

	bool has(const T& key) const {
		unsigned int hash = hasher(key) % keys.size();

		for (size_t i = 0;; ++i) {
			switch (conditions[hash]) {
				case EMPTY: {
					return false;
					break;
				}
				case DELETED: {
					break;
				}
				case KEY: {
					if (keys[hash] == key) {
						return true;
					}
					break;
				}
			}
			hash = (hash + i) % keys.size();  // Probing
		}
	}

	bool add(const T& key) {
		if (keys_count >= keys.size() * HASH_TABLE_MAX_KEYS_FILL) {
			// std::cout << "grow()" << "keys_count " << keys_count << std::endl;
			grow();
		}

		unsigned int hash = hasher(key) % keys.size();

		bool deleted_found = false;
		unsigned int deleted_hash = 0;

		for (size_t i = 0;; ++i) {
			switch (conditions[hash]) {
				case EMPTY: {
					if (deleted_found) {
						keys[deleted_hash] = key;
						conditions[deleted_hash] = KEY;
						keys_count++;
						delete_count--;
					} else {
						keys[hash] = key;
						conditions[hash] = KEY;
						keys_count++;
					}
					return true;
					break;
				}
				case DELETED: {
					if (!deleted_found) {
						deleted_found = true;
						deleted_hash = hash;
					}
					break;
				}
				case KEY: {
					if (keys[hash] == key) {
						return false;
					}
					break;
				}
			}
			hash = (hash + i) % keys.size();  // Probing
		}
	}

	bool remove(const T& key) {
		if (delete_count >= keys.size() * HASH_TABLE_MAX_DELETE_FILL) {
			// std::cout << "deleted_clear() " << "keys_count " << keys_count << " deleted_count " << delete_count << std::endl;
			deleted_clear();
		}

		unsigned int hash = hasher(key) % keys.size();

		for (size_t i = 0;; ++i) {
			switch (conditions[hash]) {
				case EMPTY: {
					return false;
					break;
				}
				case DELETED: {
					break;
				}
				case KEY: {
					if (keys[hash] == key) {
						conditions[hash] = DELETED;
						keys_count--;
						delete_count++;
						return true;
					}
					break;
				}
			}
			hash = (hash + i) % keys.size();  // Probing
		}
	}

private:
	void grow() {

		std::vector<Condition> old_conditions(std::move(conditions));
		std::vector<T> old_keys(std::move(keys));

		conditions.clear();
		conditions.resize(old_conditions.size() * 2, EMPTY);

		keys.clear();
		keys.resize(old_keys.size() * 2);
		keys_count = 0;

		for (size_t i = 0; i < old_keys.size(); ++i) {
			if (old_conditions[i] == KEY) {
				add(old_keys[i]);
			}
		}
		delete_count = 0;
	}

	void deleted_clear() {
		std::vector<Condition> old_conditions(conditions);
		std::vector<T> old_keys(keys);

		std::fill(conditions.begin(), conditions.end(), EMPTY);
		keys_count = 0;

		for (size_t i = 0; i < old_keys.size(); ++i) {
			if (old_conditions[i] == KEY) {
				add(old_keys[i]);
			}
		}
		delete_count = 0;
	}

	enum Condition {
		EMPTY = 0,
		DELETED,
		KEY
	};

	std::vector<Condition> conditions;
	std::vector<T> keys;

	Hash hasher;
	size_t keys_count = 0;
	size_t delete_count = 0;
};


void test() {
	std::string s = "abcdef";
	std::sort(s.begin(), s.end());
	std::vector<std::string> input;
    	do {
        	input.push_back(s);
    	} while(std::next_permutation(s.begin(), s.end()));

	Hash_table<std::string, StringHasher> table;
	for (int i = 0; i < 500; i++) {
		assert(table.add(input[i]));
	}
	for (int i = 0; i < 500; i++) {
		assert(!table.add(input[i]));
	}

	for (int i = 0; i < 500; i++) {
		assert(table.has(input[i]));
	}

	for (int i = 0; i < 500; i++) {
		assert(table.remove(input[i]));
	}
	for (int i = 0; i < 500; i++) {
		assert(!table.remove(input[i]));
	}

	assert(!table.has("abcdef"));
	assert(!table.has("AA"));

	for (int i = 0; i < 720; i++) {
		assert(table.add(input[i]));
	}

	for (int i = 0; i < 720; i++) {
		assert(table.has(input[i]));
	}

	for (int i = 0; i < 720; i++) {
		assert(table.remove(input[i]));
	}
}

struct request_t {
	char operation;
	std::string key;
};

void requests(const std::vector<request_t>& input, std::vector<std::string> &output) {
	Hash_table<std::string, StringHasher> table;
	for (const request_t &req : input) {
		bool res;
		switch (req.operation) {
			case '+' :
				res = table.add(req.key);
				break;
			case '-' :
				res = table.remove(req.key);
				break;
			case '?' :
				res = table.has(req.key);
				break;
		}

		if (res) {
			output.push_back("OK");
		} else {
			output.push_back("FAIL");
		}
	}
}

int main() {
	// test();

	char operation;
	std::string key;
	std::vector<request_t> input;
	while (std::cin >> operation >> key) {
		input.push_back({operation, key});
	}

	std::vector<std::string> output;
	requests(input, output);
	for (const std::string &str : output) {
		std::cout << str << std::endl;
	}

	return 0;
}
