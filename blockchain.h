#ifndef BLOCKCHAIN_H
#define BLOCKCHAIN_H

#include <string>
#include <vector>
#include <unordered_map>

#include "block.h"
#include "transaction.h"
#include "types.h"

using namespace std;

class Blockchain {
private:
    vector<Block> chain;
    unordered_map<transaction_hash_t, Transaction> pending_transactions;
    unordered_map<string, utxo_t> utxos;

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

    vector<Transaction> getPendingTransactions() {
        vector<Transaction> pending_transactions;
        for (auto const& pending_transaction : this->pending_transactions) {
            pending_transactions.push_back(pending_transaction.second);
        }
        return pending_transactions;
    }

    vector<utxo_t> getUTXOs() {
        vector<utxo_t> utxos;
        for (auto const& utxo : this->utxos) {
            utxos.push_back({utxo.second.transaction_hash, utxo.second.output_index, utxo.second.address, utxo.second.amount});
        }
        return utxos;
    }

    vector<utxo_t> getUTXOsForAddress(address_t address) {
        vector<utxo_t> utxos;
        for (auto const& utxo : this->utxos) {
            if (utxo.second.address == address) {
                utxos.push_back({utxo.second.transaction_hash, utxo.second.output_index, utxo.second.address, utxo.second.amount});
            }
        }
        return utxos;
    }

    void addPendingTransaction(Transaction transaction) {
        this->pending_transactions[transaction.getHash()] = transaction;
    }

    void updatePendingTransactions(Block newBlock) {
        for (Transaction tx : newBlock.getTransactions()) {
            this->pending_transactions.erase(tx.getHash());
        }
    }

    void updateUTXOs(Block newBlock) {
        for (Transaction tx : newBlock.getTransactions()) {
            for (uint i = 0; i < tx.getOutputs().size(); i++) {
                output_t output = tx.getOutputs()[i];
                utxo_t utxo = {tx.getHash(), i, output.address, output.amount};
                this->utxos[tx.getHash() + ":" + to_string(i)] = utxo;
            }

            for (input_t input : tx.getInputs()) {
                this->utxos.erase(input.transaction_hash + ":" + to_string(input.output_index));
            }
        }
    }

    void addBlock(Block block) {
        cout << "Adding block " << block.calculateHash() << endl;
        cout << "Block tx count: " << block.getTransactions().size() << endl;
        this->chain.push_back(block);
        this->updateUTXOs(block);
        this->updatePendingTransactions(block);
        cout << this->toString() << endl;
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