#ifndef USER_H
#define USER_H

#include <string>

#include "transaction.h"
#include "blockchain.h"
#include "hash.h"
#include "ecdsa_signer.h"
#include "types.h"

using namespace std;

class User {
private:
    string name;
    private_key_t private_key;
    public_key_t public_key;
    Blockchain *blockchain;

public:
    User(string name, Blockchain* blockchain) {
        this->name = name;
        this->blockchain = blockchain;
        ECDSASigner signer;
        this->private_key = signer.getPrivateKey();
        this->public_key = signer.getPublicKey();
    }

    User(string name, private_key_t private_key, public_key_t public_key, token_t balance, Blockchain* blockchain) {
        this->name = name;
        this->private_key = private_key;
        this->public_key = public_key;
        this->blockchain = blockchain;

        // TODO: implement input UTXOs
        Transaction tx = Transaction("0", this->public_key, balance, {}, {{this->public_key, balance}});
        this->signTransaction(tx);
        this->blockchain->addTransaction(tx);
    }

    const public_key_t& getPublicKey() {
        return public_key;
    }

    void signTransaction(Transaction& tx) {
        ECDSASigner signer(this->private_key);
        tx.setSignature(signer.sign(tx.calculateHash()));
    }

    void sendTokens(const address_t& recipient_address, token_t amount) {
        // TODO: implement input UTXOs
        Transaction tx = Transaction(this->public_key, recipient_address, amount, {}, {{recipient_address, amount}});
        this->signTransaction(tx);
        this->blockchain->addTransaction(tx);
    }

    string toString() const {
        string result = "";
        result += "Name: " + this->name + "\n";
        result += "Private key: " + this->private_key + "\n";
        result += "Public key: " + this->public_key + "\n";
        result += "Balance: 0\n";
        return result;
    }

    friend ostream& operator<<(ostream& os, const User& user) {
        os << user.toString();
        return os;
    }
};

#endif // USER_H
