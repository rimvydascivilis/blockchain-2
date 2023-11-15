#ifndef MINER_H
#define MINER_H

#include <vector>
#include <atomic>
#include <thread>
#include <random>

#include "user.h"
#include "transaction.h"
#include "blockchain.h"
#include "ecdsa_signer.h"

using namespace std;

const uint8_t threadCount = 5;

class Miner : public User {
private:
    atomic<bool> stopFlag;
    vector<uint> threadFound = vector<uint>(threadCount, 0);
    random_device rd;
    default_random_engine generator;
    uniform_int_distribution<uint64_t> distr;

    bool isValidTx(Transaction tx, vector<utxo_t>& utxos) {
        ECDSASigner signer;
        if (signer.verify(tx.getHash(), tx.getSignature(), tx.getSenderAddress()) == false) {
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

    void mineBlock(vector<Transaction> validTxs, uint64_t nonceStart, uint threadIdx) {
        Block block = Block(this->getBlockchain()->getLastBlockHash(), validTxs);
        block.setNonce(nonceStart);
        while (block.calculateHash().substr(0, block.getDifficulty()) != string(block.getDifficulty(), '0')
                && !stopFlag.load()) {
            block.setNonce(block.getNonce() + 1);
        }

        if (stopFlag.load() == false) {
            cout << "Thread #" << threadIdx << " found a block!" << endl;
            threadFound[threadIdx]++;
            stopFlag.store(true);
            this->getBlockchain()->addBlock(block);
        }
    }

public:
    Miner(string name, Blockchain* blockchain) : User(name, blockchain) {}

    void mineAllValidTxs() {
        while (1) {
            vector<Transaction> txs = this->getBlockchain()->getPendingTransactions();
            if (this->getValidRandomTxs(txs).size() == 0) {
                break;
            }

            vector<vector<Transaction>> validTxsPerThread;
            for (uint i = 0; i < threadCount; i++) {
                validTxsPerThread.push_back(this->getValidRandomTxs(txs));
            }

            stopFlag.store(false);
            vector<thread> threads;
            for (uint i = 0; i < threadCount; i++) {
                uint64_t nonceStart = distr(generator);
                threads.push_back(thread(&Miner::mineBlock, this, validTxsPerThread[i], nonceStart, i));
            }

            for (auto &thread : threads) {
                thread.join();
            }
        }
    }

    vector<uint> getThreadFound() {
        return threadFound;
    }
};

#endif // MINER_H
