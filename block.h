#ifndef BLOCK_H
#define BLOCK_H

#include <string>
#include <vector>
#include <ctime>
#include <cassert>

#include "transaction.h"
#include "hash.h"
#include "util.h"

using namespace std;

string join(vector<Transaction> v, string delimiter) {
    string result = "";
    for (unsigned int i=0; i<v.size(); i++) {
        result += v[i].getHash();
        if (i != v.size() - 1) {
            result += delimiter;
        }
    }
    return result;
}

class Block {
private:
    string previous_hash;
    string merkle_root_hash;
    time_t timestamp;
    unsigned int nonce;
    string version = "0.1";
    unsigned short int difficulty = 14;
    vector<Transaction> transactions;

public:
    Block(string previous_hash, vector<Transaction> transactions) {
        this->previous_hash = previous_hash;
        this->transactions = transactions;
        this->timestamp = time(0);
        this->nonce = 0;
        this->merkle_root_hash = calculateMerkleRootHash();
    }

    string calculateMerkleRootHash() {
        vector<string> merkle {};
        for (unsigned int i=0; i<this->transactions.size(); i++) {
            merkle.push_back(this->transactions[i].getHash());
        }

        if (merkle.empty()) {
            return convert_to_hex(hash_f(""));
        } else if (merkle.size() == 1) {
            return merkle[0];
        }

        while (merkle.size() > 1) {
            if (merkle.size() % 2 != 0) {
                merkle.push_back(merkle.back());
            }

            assert(merkle.size() % 2 == 0);

            vector<string> new_merkle;
            for (auto it = merkle.begin(); it != merkle.end(); it += 2) {
                string concat_data = *it + *(it + 1);
                string concat_hash = convert_to_hex(hash_f(concat_data));
                new_merkle.push_back(concat_hash);
            }

            merkle = new_merkle;
        }

        return merkle[0];
    }

    string getPreviousHash() {
        return this->previous_hash;
    }

    string getMerkleRootHash() {
        return this->merkle_root_hash;
    }

    time_t getTimestamp() {
        return this->timestamp;
    }

    unsigned int getNonce() {
        return this->nonce;
    }

    string getVersion() {
        return this->version;
    }

    unsigned short int getDifficulty() {
        return this->difficulty;
    }

    vector<Transaction> getTransactions() {
        return this->transactions;
    }

    string calculateHash() const {
        string hash = "";
        hash += this->previous_hash;
        hash += this->merkle_root_hash;
        hash += to_string(this->timestamp);
        hash += to_string(this->nonce);
        hash += this->version;
        hash += to_string(this->difficulty);
        hash += join(this->transactions, ",");
        return convert_to_hex(hash_f(hash));
    }

    string toString() const {
        string result = "";
        result += "Hash: " + this->calculateHash() + "\n";
        result += "Previous Hash: " + this->previous_hash + "\n";
        result += "Merkle Root Hash: " + this->merkle_root_hash + "\n";
        result += "Timestamp: " + to_string(this->timestamp) + "\n";
        result += "Nonce: " + to_string(this->nonce) + "\n";
        result += "Version: " + this->version + "\n";
        result += "Difficulty: " + to_string(this->difficulty) + "\n";
        result += "Transactions:\n";
        for (unsigned int i=0; i<this->transactions.size(); i++) {
            if (i != this->transactions.size() - 1) {
                result += "=============================================\n";
            }
            result += this->transactions[i].toString();
        }
        return result;
    }

    friend ostream& operator<<(ostream& os, const Block& block) {
        os << block.toString();
        return os;
    }
};

#endif // BLOCK_H