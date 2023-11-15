#include <iostream>
#include <string>
#include <vector>
#include <fstream>

#include "blockchain.h"
#include "user.h"
#include "print.h"
#include "miner.h"

using namespace std;

int main() {
    srand(time(0));
    Blockchain blockchain;

    vector<User> users;
    for (int i=0; i<1000; i++) {
        string id = to_string(i + 1);
        users.push_back(User("user" + id, rand() % 900001 + 100000, &blockchain));
    }
    Miner miner = Miner("miner", &blockchain);
    miner.mineBlock();
    miner.mineBlock();
    miner.mineBlock();
    miner.mineBlock();
    miner.mineBlock();
    miner.mineBlock();
    miner.mineBlock();
    miner.mineBlock();
    miner.mineBlock();

    for (int i=0; i<10000; i++) {
        User randomUser = users[rand() % users.size()];
        User randomRecipient = users[rand() % users.size()];
        randomUser.sendTokens(randomRecipient.getPublicKey(), rand() % 10);
    }

    miner.mineBlock();
    writeToFile(blockchain, users, "results/");
}
