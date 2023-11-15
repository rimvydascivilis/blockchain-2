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

protected:
    Blockchain* getBlockchain() {
        return this->blockchain;
    }

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

        input_t input = {"0", 0}; // 0 as tx hash is used to generate coins
        Transaction tx = Transaction("0", this->public_key, {input}, {{this->public_key, balance}});
        this->signTransaction(tx);
        this->blockchain->addPendingTransaction(tx);
    }

    User(string name, token_t balance, Blockchain* blockchain) {
        this->name = name;
        this->blockchain = blockchain;
        ECDSASigner signer;
        this->private_key = signer.getPrivateKey();
        this->public_key = signer.getPublicKey();

        input_t input = {"0", 0}; // 0 as tx hash is used to generate coins
        Transaction tx = Transaction(this->public_key, this->public_key, {input}, {{this->public_key, balance}});
        this->signTransaction(tx);
        this->blockchain->addPendingTransaction(tx);
    }

    const public_key_t& getPublicKey() {
        return public_key;
    }

    const token_t getBalance() const {
        vector<utxo_t> utxos = this->blockchain->getUTXOsForAddress(this->public_key);
        token_t balance = 0;
        for (utxo_t utxo : utxos) {
            balance += utxo.amount;
        }
        return balance;
    }

    void signTransaction(Transaction& tx) {
        ECDSASigner signer(this->private_key);
        tx.setSignature(signer.sign(tx.calculateHash()));
    }

    void sendTokens(const address_t& recipient_address, token_t amount) {
        vector<utxo_t> utxos = this->blockchain->getUTXOsForAddress(this->public_key);
        vector<input_t> inputs;
        token_t balance = 0;
        for (utxo_t utxo : utxos) {
            inputs.push_back({utxo.transaction_hash, utxo.output_index});
            balance += utxo.amount;
            if (balance >= amount) {
                break;
            }
        }
        assert(balance >= amount);
        Transaction tx = Transaction(this->public_key, recipient_address, inputs, {
            {recipient_address, amount},
            {this->public_key, balance - amount}
        });
        this->signTransaction(tx);
        this->blockchain->addPendingTransaction(tx);
    }

    string toString() const {
        string result = "";
        result += "Name: " + this->name + "\n";
        result += "Private key: " + this->private_key + "\n";
        result += "Public key: " + this->public_key + "\n";
        result += "Balance: " + to_string(this->getBalance()) + "\n";
        return result;
    }

    friend ostream& operator<<(ostream& os, const User& user) {
        os << user.toString();
        return os;
    }
};

#endif // USER_H
