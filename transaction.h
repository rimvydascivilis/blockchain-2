#ifndef TRANSACTION_H
#define TRANSACTION_H

#include <string>
#include <vector>

#include "hash.h"
#include "util.h"
#include "types.h"

using namespace std;

class Transaction {
private:
    address_t sender_address;
    address_t recipient_address;
    signature_t signature;
    vector<input_t> inputs;
    vector<output_t> outputs;

public:
    Transaction() {}

    Transaction(const address_t& sender, const address_t& recipient, vector<input_t> inputs, vector<output_t> outputs) {
        this->sender_address = sender;
        this->recipient_address = recipient;
        this->inputs = inputs;
        this->outputs = outputs;
    }

    transaction_hash_t calculateHash() const {
        return convert_to_hex(hash_f(
            this->sender_address + ";" +
            this->recipient_address + ";" +
            join(inputs, ",") + ";" +
            join(outputs, ",")
        ));
    }

    transaction_hash_t getHash() {
        return calculateHash();
    }

    const address_t& getSenderAddress() {
        return sender_address;
    }

    const address_t& getRecipientAddress() {
        return recipient_address;
    }

    const signature_t& getSignature() {
        return signature;
    }

    void setSignature(const signature_t& signature) {
        this->signature = signature;
    }

    const vector<input_t>& getInputs() {
        return inputs;
    }

    const vector<output_t>& getOutputs() {
        return outputs;
    }

    string toString() const {
        string str = "";
        str += "Hash: " + calculateHash() + "\n";
        str += "Sender address: " + sender_address + "\n";
        str += "Recipient address: " + recipient_address + "\n";
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
