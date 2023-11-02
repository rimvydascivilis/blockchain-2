#ifndef USER_H
#define USER_H

#include <string>

#include "transaction.h"
#include "blockchain.h"
#include "hash.h"

using namespace std;

class User {
private:
    string name;
    string private_key;
    string public_key;
    Blockchain *blockchain;

public:
    User(string name, Blockchain* blockchain) {
        this->name = name;
        this->blockchain = blockchain;
    }

    User(string name, string private_key, string public_key, uint balance, Blockchain* blockchain) {
        this->name = name;
        this->private_key = private_key;
        this->public_key = public_key;
        this->blockchain = blockchain;

        Transaction tx = Transaction("0", this->public_key, balance, {}, {{this->public_key, balance}});
        this->blockchain->addTransaction(tx);
    }

    const string& getPublicKey() {
        return public_key;
    }

    void sendTokens(const string& recipient_address, int amount) {
        Transaction tx = Transaction(this->public_key, recipient_address, amount, {}, {{recipient_address, amount}});
        this->blockchain->addTransaction(tx);
    }
};

#endif // USER_H
