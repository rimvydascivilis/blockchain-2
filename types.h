#ifndef TYPES_H
#define TYPES_H

#include <string>
#include <bitset>

using namespace std;

typedef bitset<256> hash_t;
typedef string address_t;
typedef string signature_t;
typedef string transaction_hash_t;
typedef string block_hash_t;
typedef string merkle_root_hash_t;
typedef string public_key_t;
typedef string private_key_t;
typedef uint token_t;
typedef uint output_index_t;
typedef struct {
    transaction_hash_t transaction_hash;
    output_index_t output_index;
} input_t;
typedef struct {
    address_t address;
    token_t amount;
} output_t;


#endif // TYPES_H
