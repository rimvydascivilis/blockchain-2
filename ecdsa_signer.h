#ifndef ECDSA_SIGNER
#define ECDSA_SIGNER

#include <iostream>
#include <vector>
#include <cstring>
#include <secp256k1.h>
#include "util.h"
#include "hash.h"

using namespace std;

class ECDSASigner {
private:
    secp256k1_context* ctx;
    vector<unsigned char> seckey;
    secp256k1_pubkey pubkey;

public:
    ECDSASigner() {
        ctx = secp256k1_context_create(SECP256K1_CONTEXT_SIGN | SECP256K1_CONTEXT_VERIFY);

        while (true) {
            seckey = fillRandom(32);
            if (secp256k1_ec_seckey_verify(ctx, seckey.data())) {
                break;
            }
        }

        int return_val = secp256k1_ec_pubkey_create(ctx, &pubkey, seckey.data());
        if (!return_val) {
            cerr << "Failed to generate public key\n";
            terminate();
        }
    }

    ECDSASigner(const string& private_key_hex) {
        if (private_key_hex.size() != 64) {
            cerr << "Invalid private key length\n";
            terminate();
        }

        ctx = secp256k1_context_create(SECP256K1_CONTEXT_SIGN | SECP256K1_CONTEXT_VERIFY);

        seckey = hexStringToBytes(private_key_hex);

        if (!secp256k1_ec_seckey_verify(ctx, seckey.data())) {
            cerr << "Invalid private key\n";
            terminate();
        }

        int return_val = secp256k1_ec_pubkey_create(ctx, &pubkey, seckey.data());
        if (!return_val) {
            cerr << "Failed to generate public key\n";
            terminate();
        }
    }

    ~ECDSASigner() {
        secp256k1_context_destroy(ctx);
        secureErase(seckey.data(), seckey.size());
    }

    string sign(const string& message) const {
        vector<unsigned char> msg_hash = hexStringToBytes(convert_to_hex(hash_f(message)));

        secp256k1_ecdsa_signature sig;
        int return_val = secp256k1_ecdsa_sign(ctx, &sig, msg_hash.data(), seckey.data(), nullptr, nullptr);
        if (!return_val) {
            cerr << "Failed to sign message\n";
            terminate();
        }

        vector<unsigned char> serialized_signature(64);
        size_t len = serialized_signature.size();
        return_val = secp256k1_ecdsa_signature_serialize_compact(ctx, serialized_signature.data(), &sig);
        if (!return_val) {
            cerr << "Failed to serialize signature\n";
            terminate();
        }

        return bytesToHexString(serialized_signature.data(), len);
    }

    bool verify(const string& message, const string& signature_hex, const string& public_key_hex) const {
        if (signature_hex.size() != 128 || public_key_hex.size() != 66) {
            cerr << "Invalid input length\n";
            terminate();
        }

        vector<unsigned char> msg_hash = hexStringToBytes(convert_to_hex(hash_f(message)));
        vector<unsigned char> serialized_signature = hexStringToBytes(signature_hex);
        vector<unsigned char> compressed_pubkey = hexStringToBytes(public_key_hex);

        secp256k1_ecdsa_signature sig;
        if (!secp256k1_ecdsa_signature_parse_compact(ctx, &sig, serialized_signature.data())) {
            cerr << "Failed to parse signature\n";
            terminate();
        }

        secp256k1_pubkey pubkey;
        if (!secp256k1_ec_pubkey_parse(ctx, &pubkey, compressed_pubkey.data(), compressed_pubkey.size())) {
            cerr << "Failed to parse public key\n";
            terminate();
        }

        return secp256k1_ecdsa_verify(ctx, &sig, msg_hash.data(), &pubkey);
    }

    string getPublicKey() const {
        vector<unsigned char> compressed_pubkey(33);
        size_t len = compressed_pubkey.size();
        int return_val = secp256k1_ec_pubkey_serialize(ctx, compressed_pubkey.data(), &len, &pubkey, SECP256K1_EC_COMPRESSED);
        if (!return_val) {
            cerr << "Failed to serialize public key\n";
            terminate();
        }

        return bytesToHexString(compressed_pubkey.data(), len);
    }

    string getPrivateKey() const {
        return bytesToHexString(seckey.data(), seckey.size());
    }
};

#endif // ECDSA_SIGNER
