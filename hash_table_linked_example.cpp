#include <vector>
#include <iostream>
#include <cassert>
#include <string>

using std::string;
using std::vector;

const int HashTableDefaultSize = 8;
const int MaxFill = 3;

struct StringHasher {
	unsigned int operator()(const string& str) const {
		unsigned int hash = 0;
		for (unsigned int i = 0; i < str.length(); ++i) {
			hash = hash * 127 + str[i];
		}

		return hash;
	}
};


template<class T, class Hash>
class HashTable {
public:
	HashTable(const Hash& _hasher = Hash());
	~HashTable();

	bool Has(const T& key) const;
	bool Add(const T& key);
	bool Delete(const T& key);
private:
	void grow();
	struct HashTableNode {
		T Data;
		HashTableNode* Next;

		HashTableNode(const T& key, HashTableNode* next = nullptr) : Data(key), Next(next) {}
	};

	Hash hasher;
	vector<HashTableNode*> table;
	unsigned int keysCount;
};

template<class T, class Hash>
HashTable<T, Hash>::HashTable(const Hash& _hasher) :
	hasher(_hasher),
	table(HashTableDefaultSize, nullptr),
	keysCount(0) { }

template<class T, class Hash>
HashTable<T, Hash>::~HashTable() {
	for (unsigned int i = 0; i < table.size(); ++i) {
		HashTableNode* node = table[i];
		while (node != nullptr) {
			HashTableNode* nextNode = node->Next;
			delete node;
			node = nextNode;
		}
	}
}

template<class T, class Hash>
bool HashTable<T, Hash>::Has(const T& key) const {
	int hash = hasher(key) % table.size();
	HashTableNode* node = table[hash];
	while (node != nullptr && node->Data != key)
		node = node->Next;

	return node != nullptr;
}

template<class T, class Hash>
bool HashTable<T, Hash>::Add(const T& key) {
	// Не хорошо так делать
	if (Has(key)) {
		return false;
	}
	//
	if (keysCount + 1 > table.size() * MaxFill) {
		grow();
	}

	int hash = hasher(key) % table.size();
	table[hash] = new HashTableNode(key, table[hash]);
	++keysCount;

	return true;
}

template<class T, class Hash>
bool HashTable<T, Hash>::Delete(const T& key)
{
	int hash = hasher(key) % table.size();
	HashTableNode* prevNode = nullptr;
	HashTableNode* node = table[hash];
	while (node != nullptr && node->Data != key) {
		prevNode = node;
		node = node->Next;
	}

	if (node == nullptr)
		return false;

	if (prevNode != nullptr) {
		prevNode->Next = node->Next;
	}
	else {
		table[hash] = node->Next;
	}

	delete node;
	return true;
}

template<class T, class Hash>
void HashTable<T, Hash>::grow()
{
	vector<HashTableNode*> newTable(table.size() * 2, nullptr);
	for (unsigned int i = 0; i < table.size(); ++i) {
		HashTableNode* node = table[i];
		while (node != nullptr) {
			HashTableNode* nextNode = node->Next;
			
			int newHash = hasher(node->Data) % newTable.size();
			node->Next = newTable[newHash];
			newTable[newHash] = node;

			node = nextNode;
		}
	}
	table = newTable;
}

int main() {
	HashTable<string, StringHasher> table;
	char operation = 0;
	string word;
	while (std::cin >> operation >> word) {
		if (operation == '+') {
			std::cout << (table.Add(word) ? "OK" : "FAIL") << std::endl;
		}
		if (operation == '-') {
			std::cout << (table.Delete(word) ? "OK" : "FAIL") << std::endl;
		}
		if (operation == '?') {
			std::cout << (table.Has(word) ? "OK" : "FAIL") << std::endl;
		}
	}

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
