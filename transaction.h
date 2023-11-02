#ifndef TRANSACTION_H
#define TRANSACTION_H

#include <string>
#include <vector>

#include "hash.h"
#include "util.h"

using namespace std;

class Transaction {
private:
    string sender_address;
    string recipient_address;
    int amount;
    string signature;
    vector<pair<string, int>> inputs; // <transaction hash, output index>
    vector<pair<string, int>> outputs; // <address, amount>

public:
    Transaction(const string& sender_address, const string& recipient_address, int amount, vector<pair<string, int>> inputs, vector<pair<string, int>> outputs) {
        this->sender_address = sender_address;
        this->recipient_address = recipient_address;
        this->amount = amount;
        this->inputs = inputs;
        this->outputs = outputs;
    }

    string calculateHash() const {
        return convert_to_hex(hash_f(
            this->sender_address + ";" +
            this->recipient_address + ";" +
            to_string(this->amount) + ";" +
            join(inputs, ",") + ";" +
            join(outputs, ",")
        ));
    }

    const string getHash() {
        return calculateHash();
    }

    const string& getSenderAddress() {
        return sender_address;
    }

    const string& getRecipientAddress() {
        return recipient_address;
    }

    int getAmount() {
        return amount;
    }

    const string& getSignature() {
        return signature;
    }

    void setSignature(const string& signature) {
        this->signature = signature;
    }

    const vector<pair<string, int>>& getInputs() {
        return inputs;
    }

    const vector<pair<string, int>>& getOutputs() {
        return outputs;
    }

    string toString() const {
        string str = "";
        str += "Hash: " + calculateHash() + "\n";
        str += "Sender address: " + sender_address + "\n";
        str += "Recipient address: " + recipient_address + "\n";
        str += "Amount: " + to_string(amount) + "\n";
        str += "Signature: " + signature + "\n";
        str += "Inputs: \n";
        str += join(inputs, ",") + "\n";
        str += "Outputs: \n";
        str += join(outputs, ",") + "\n";
        return str;
    }

    friend ostream& operator<<(ostream& os, const Transaction& t) {
        os << t.toString();
        return os;
    }
};

#endif // TRANSACTION_H
