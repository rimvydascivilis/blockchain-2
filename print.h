#ifndef PRINT_H
#define PRINT_H

#include <string>
#include <vector>
#include <fstream>

#include "blockchain.h"
#include "block.h"
#include "transaction.h"
#include "user.h"

using namespace std;

void writePendingTransactionsToFile(vector<Transaction> pending_transactions, string filename) {
    ofstream file;
    file.open(filename);
    for (Transaction transaction : pending_transactions) {
        file << transaction << endl;
    }
}

void WriteBlocksToFile(vector<Block> chain, string filename) {
    for (uint i=0; i<chain.size(); i++) {
        ofstream file;
        file.open(filename + to_string(i) + ".txt");
        file << chain[i] << endl;
    }
}

void writeBlockchainToFile(Blockchain blockchain, string filename) {
    ofstream file;
    file.open(filename);
    file << blockchain;
}

void writeToFile(Blockchain blockchain, string directory) {
    system(("mkdir -p " + directory).c_str());
    writeBlockchainToFile(blockchain, directory + "blockchain.txt");
    writePendingTransactionsToFile(blockchain.getPendingTransactions(), directory + "pending_transactions.txt");
    WriteBlocksToFile(blockchain.getChain(), directory + "block_");
}

#endif // PRINT_H