#ifndef RSA_WRAPPER_H
#define RSA_WRAPPER_H

#include <bitset>
#include <string>
#include <openssl/rsa.h>
#include <openssl/pem.h>

#include "hash.h"
#include "util.h"

class RSAWrapper {
public:
    RSAWrapper(int sizeOfKeys = 2048) {
        rsa_key_ = RSA_new();
        BIGNUM* e = BN_new();
        BN_set_word(e, RSA_F4);
        RSA_generate_key_ex(rsa_key_, sizeOfKeys, e, nullptr);
        BN_free(e);
    }

    RSAWrapper(const string& publicKeyStr, const string& privateKeyStr) {
        rsa_key_ = RSA_new();

        BIO* public_bio = BIO_new_mem_buf(publicKeyStr.c_str(), -1);
        rsa_key_ = PEM_read_bio_RSAPublicKey(public_bio, &rsa_key_, nullptr, nullptr);
        BIO_free(public_bio);

        BIO* private_bio = BIO_new_mem_buf(privateKeyStr.c_str(), -1);
        rsa_key_ = PEM_read_bio_RSAPrivateKey(private_bio, &rsa_key_, nullptr, nullptr);
        BIO_free(private_bio);
    }

    ~RSAWrapper() {
        RSA_free(rsa_key_);
    }

    string getPublicKey() {
        BIO* bio = BIO_new(BIO_s_mem());
        PEM_write_bio_RSAPublicKey(bio, rsa_key_);
        char* public_key = nullptr;
        long public_key_len = BIO_get_mem_data(bio, &public_key);
        string publicKeyStr(public_key, public_key_len);
        BIO_free(bio);
        return publicKeyStr;
    }

    string getPrivateKey() {
        BIO* bio = BIO_new(BIO_s_mem());
        PEM_write_bio_RSAPrivateKey(bio, rsa_key_, nullptr, nullptr, 0, nullptr, nullptr);
        char* private_key = nullptr;
        long private_key_len = BIO_get_mem_data(bio, &private_key);
        string privateKeyStr(private_key, private_key_len);
        BIO_free(bio);
        return privateKeyStr;
    }

    string encrypt(const string& message) {
        int rsa_len = RSA_size(rsa_key_);
        unsigned char* encrypted = new unsigned char[rsa_len];
        int encrypted_length = RSA_private_encrypt(message.size(), reinterpret_cast<const unsigned char*>(message.c_str()), encrypted, rsa_key_, RSA_PKCS1_PADDING);

        if (encrypted_length == -1) {
            return "";
        }

        string encryptedStr(reinterpret_cast<char*>(encrypted), encrypted_length);
        delete[] encrypted;
        return encryptedStr;
    }

    string decrypt(const string& cipher) {
        int rsa_len = RSA_size(rsa_key_);
        unsigned char* decrypted = new unsigned char[rsa_len];
        int decrypted_length = RSA_public_decrypt(cipher.size(), reinterpret_cast<const unsigned char*>(cipher.c_str()), decrypted, rsa_key_, RSA_PKCS1_PADDING);

        if (decrypted_length == -1) {
            return "";
        }

        string decryptedStr(reinterpret_cast<char*>(decrypted), decrypted_length);
        delete[] decrypted;
        return decryptedStr;
    }

    string sign(const string& text) {
        bitset<256> hash = hash_f(text);
        string hash_hex = convert_to_hex(hash);

        string signature = encrypt(hash_hex);
        return signature;
    }

    bool verify(const string& message, const string& signature, const string& publicKeyStr) {
        RSA* publicKey = createPublicKeyFromPEM(publicKeyStr);

        if (publicKey == nullptr) {
            return false;
        }

        string decryptedSignature = decryptWithPublicKey(signature, publicKey);

        bitset<256> messageHash = hash_f(message);
        string messageHashHex = convert_to_hex(messageHash);

        return (messageHashHex == decryptedSignature);
    }

private:
    RSA* rsa_key_;

    RSA* createPublicKeyFromPEM(const string& publicKeyStr) {
        RSA* publicKey = RSA_new();

        BIO* public_bio = BIO_new_mem_buf(publicKeyStr.c_str(), -1);
        if (!PEM_read_bio_RSAPublicKey(public_bio, &publicKey, nullptr, nullptr)) {
            BIO_free(public_bio);
            return nullptr;
        }

        BIO_free(public_bio);
        return publicKey;
    }

    string decryptWithPublicKey(const string& cipher, RSA* publicKey) {
        int rsa_len = RSA_size(publicKey);
        unsigned char* decrypted = new unsigned char[rsa_len];
        int decrypted_length = RSA_public_decrypt(cipher.size(), reinterpret_cast<const unsigned char*>(cipher.c_str()), decrypted, publicKey, RSA_PKCS1_PADDING);

        if (decrypted_length == -1) {
            delete[] decrypted;
            return "";
        }

        string decryptedStr(reinterpret_cast<char*>(decrypted), decrypted_length);
        delete[] decrypted;
        return decryptedStr;
    }
};

#endif // RSA_WRAPPER_H
