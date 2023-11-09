#ifndef BLOCKCHAIN_H
#define BLOCKCHAIN_H

#include <string>
#include <vector>

#include "block.h"
#include "transaction.h"
#include "types.h"

using namespace std;

class Blockchain {
private:
    vector<Block> chain;
    vector<Transaction> pending_transactions;

public:
    Blockchain() {
        this->chain.push_back(Block("0", vector<Transaction>()));
    }

    const vector<Block>& getChain() {
        return this->chain;
    }

    const Block& getLastBlock() {
        return this->chain.back();
    }

    const block_hash_t getLastBlockHash() const {
        return this->chain.back().calculateHash();
    }

    void addTransaction(Transaction transaction) {
        this->pending_transactions.push_back(transaction);
    }

    vector<Transaction> getPendingTransactions() {
        return this->pending_transactions;
    }

    void addBlock(Block block) {
        this->chain.push_back(block);
    }

    void mineAll() {
        while (this->pending_transactions.size() > 0) {
            vector<Transaction> transactions;
            for (unsigned int i=0; i<(uint) this->pending_transactions.size(); i++) {
                if (this->pending_transactions.size() > 0) {
                    transactions.push_back(this->pending_transactions.back());
                    this->pending_transactions.pop_back();
                }
            }
            this->addBlock(Block(this->getLastBlockHash(), transactions));
        }
    }

    string toString() const {
        string result = "";
        result += "Last block hash: " + this->getLastBlockHash() + "\n";
        result += "Pending transactions size: " + to_string(this->pending_transactions.size()) + "\n";
        result += "Chain size: " + to_string(this->chain.size()) + "\n";
        return result;
    }

    friend ostream& operator<<(ostream& os, const Blockchain& blockchain) {
        os << blockchain.toString();
        return os;
    }
};

#endif // BLOCKCHAIN_H