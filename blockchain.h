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
    vector<Transaction> pending_transactions;
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
        return this->pending_transactions;
    }

    unordered_map<string, utxo_t> getUTXOs() {
        return this->utxos;
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
        this->pending_transactions.push_back(transaction);
    }

    void updatePendingTransactions(Block newBlock) {
        for (Transaction tx : newBlock.getTransactions()) {
            for (uint i = 0; i < this->pending_transactions.size(); i++) {
                if (this->pending_transactions[i].getHash() == tx.getHash()) {
                    this->pending_transactions.erase(this->pending_transactions.begin() + i);
                    break;
                }
            }
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
        this->chain.push_back(block);
        cout << "Added block to chain" << endl;
        this->updateUTXOs(block);
        cout << "Updated UTXOs" << endl;
        this->updatePendingTransactions(block);
        cout << "Updated pending transactions" << endl;
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