#include <iostream>
#include <string>
#include <vector>
#include <fstream>

#include "blockchain.h"
#include "user.h"
#include "print.h"

using namespace std;

int main() {
    srand(time(0));
    Blockchain blockchain;

    vector<User> users;
    for (int i=0; i<1000; i++) {
        string id = to_string(i + 1);
        users.push_back(User("user" + id, id, id, rand() % 999901 + 100, &blockchain));
    }

    blockchain.mineAll();
    writeToFile(blockchain, "results/");
}
