#include <iostream>
#include <string>
#include <vector>

static constexpr int HASH_TABLE_DEFAULT_SIZE = 8;
static constexpr float HASH_TABLE_MAX_KEYS_FILL = 3 / 4;
static constexpr float HASH_TABLE_MAX_DELETE_FILL = 1 / 4;


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


// PROBING
// h(k, i)=h(k, i-1) + i (mod m). m - степень двойки
// size_t h(unsigned int ) {
// }


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
		/* if (keys_count >= keys.size() * HASH_TABLE_MAX_KEYS_FILL) {
			grow_if_keys_filled();
		} */

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
		/* if (delete_count >= keys.size() * HASH_TABLE_MAX_DELETE_FILL) {
			grow_if_delete_filled();
		} */

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
	// void grow_if_keys_filled();
	// void grow_if_delete_filled();

	// PROBING
	// size_t h(const int &key, size_t i) {
	// }
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





int main() {
	Hash_table<std::string, StringHasher> table;
	std::cout << table.add("AAAA");
	std::cout << table.has("AAAA");
	std::cout << table.remove("AAAA");
	std::cout << table.has("AAAA");
	std::cout << table.has("AA");

	return 0;
}


// Has
// Empty - return false;
// Deleted - идём дальше;
// Key - если искомый - return true, иначе идём дальше

// Add
// Empty - записываем Key ( в первый запомненный Deleted, если есть, либо сюда) и return true;
// Deleted - если первый Deleted - запоминаем, иначе идём дальше;
// Key - если добавляемый - return false, иначе идём дальше.

// Delete
// Empty - return false;
// Deleted - идём дальше;
// Key - если удаляемый - ставим Deleted и return true, иначе идём дальше;

// + два перехеширования
// При большом количестве ключей (3/4) с увеличением размера таблицы
// При большом количестве Deleted (1/4) с сохранением размера таблицы
