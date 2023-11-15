#ifndef MINER_H
#define MINER_H

#include <vector>

#include "user.h"
#include "transaction.h"
#include "blockchain.h"
#include "ecdsa_signer.h"

class Miner : public User {
private:
    bool isValidTx(Transaction tx, vector<utxo_t>& utxos) {
        ECDSASigner signer;
        if (signer.verify(tx.getHash(), tx.getSignature(), tx.getSenderAddress()) == false) {
            cout << "Invalid signature" << endl;
            return false;
        }
        vector<utxo_t> available_utxos = utxos;
        for (input_t input : tx.getInputs()) {
            if (input.transaction_hash == "0") {
                continue; // tx hash is 0 when generating coins
            }
            bool found = false;
            for (uint i=0; i<available_utxos.size(); i++) {
                utxo_t utxo = available_utxos[i];
                if (utxo.transaction_hash == input.transaction_hash && utxo.output_index == input.output_index) {
                    found = true;
                    available_utxos.erase(available_utxos.begin() + i);
                    break;
                }
            }
            if (found == false) {
                cout << "Input not found" << endl;
                return false;
            }
        }
        utxos = available_utxos;
        return true;
    }

    vector<Transaction> getValidRandomTxs(vector<Transaction> txs) {
        vector<Transaction> valid_txs;
        vector<utxo_t> utxos = this->getBlockchain()->getUTXOs();
        while (valid_txs.size() < 100 && txs.size() > 0) {
            int index = rand() % txs.size();
            if (this->isValidTx(txs[index], utxos)) {
                valid_txs.push_back(txs[index]);
            }
            txs.erase(txs.begin() + index);
        }
        return valid_txs;
    }

    Block POW(vector<Transaction> transactions, uint64_t nonceStart) {
        Block block = Block(this->getBlockchain()->getLastBlockHash(), transactions);
        block.setNonce(nonceStart);
        while (block.calculateHash().substr(0, block.getDifficulty()) != string(block.getDifficulty(), '0')) {
            block.setNonce(block.getNonce() + 1);
        }
        return block;
    }

public:
    Miner(string name, Blockchain* blockchain) : User(name, blockchain) {}

    void mineBlock() {
        vector<Transaction> txs = this->getBlockchain()->getPendingTransactions();
        vector<Transaction> valid_txs = this->getValidRandomTxs(txs);
        cout << "Valid txs: " << valid_txs.size() << endl;
        uint64_t nonceStart = rand();
        Block block = this->POW(valid_txs, nonceStart);
        cout << "Mined block with nonce: " << block.getNonce() << endl;
        this->getBlockchain()->addBlock(block);
        cout << "Added block to blockchain" << endl;
    }
};

#endif // MINER_H
