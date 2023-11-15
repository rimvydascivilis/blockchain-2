#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <random>

#include "blockchain.h"
#include "user.h"
#include "print.h"
#include "miner.h"

using namespace std;

const uint USER_COUNT = 1000;
const uint TX_COUNT = 10000;

int main() {
    random_device rd;
    default_random_engine generator(rd());
    uniform_int_distribution<uint64_t> balanceDistr(100000, 1000000);
    uniform_int_distribution<uint64_t> userDistr(0, USER_COUNT - 1);
    uniform_int_distribution<uint64_t> txDistr(1, 1000);

    Blockchain blockchain;
    Miner miner = Miner("miner", &blockchain);
    vector<User> users;
    cout << "=================== Generating users ===================" << endl;
    for (uint i = 0; i < USER_COUNT; i++) {
        string name = "user" + to_string(i);
        token_t balance = balanceDistr(generator);
        users.push_back(User(name, balance, &blockchain));
    }
    miner.mineAllValidTxs();

    cout << "=================== Generating transactions ===================" << endl;
    for (uint i = 0; i < TX_COUNT; i++) {
        uint64_t senderIndex = userDistr(generator);
        uint64_t receiverIndex = userDistr(generator);
        while (senderIndex == receiverIndex) {
            receiverIndex = userDistr(generator);
        }
        token_t amount = txDistr(generator);
        users[senderIndex].sendTokens(users[receiverIndex].getPublicKey(), amount);
        if (i % 100 == 0) {
            miner.mineAllValidTxs();
        }
    }
    miner.mineAllValidTxs();

    writeToFile(blockchain, users, miner.getThreadFound(), "results/");
}
