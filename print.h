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

void writeUsersToFile(vector<User> users, string filename) {
    ofstream file;
    file.open(filename);
    for (size_t i=0; i<users.size(); i++) {
        file << users[i] << endl;
    }
}

void writeToFile(Blockchain blockchain, vector<User> users, string directory) {
    int res = system(("mkdir -p " + directory).c_str());
    if (res != 0) {
        cout << "Error creating directory " << directory << endl;
        return;
    }
    writeBlockchainToFile(blockchain, directory + "blockchain.txt");
    writePendingTransactionsToFile(blockchain.getPendingTransactions(), directory + "pending_transactions.txt");
    WriteBlocksToFile(blockchain.getChain(), directory + "block_");
    writeUsersToFile(users, directory + "users.txt");
}

#endif // PRINT_H