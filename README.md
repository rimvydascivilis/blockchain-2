# Simplified Blockchain

## Description

This is a simplified blockchain implementation in C++. It creates 1000 users and generates 10000 transactions. Each block contains 100 transactions.

## Results

After running the program, you will see blocks, users and transactions information in the `results` directory.

## Prerequisites

### install secp256k1 library

See [secp256k1 building](https://github.com/bitcoin-core/secp256k1#building-on-posix-systems).

## Usage

### Build

```bash
make
```

### Run

```bash
./main
```

## Implementation Details

### Transaction

- Transaction contains:
  - sender's public key
  - receiver's public key
  - inputs
  - outputs
  - signature

- Transaction signing and verification is implemented using [secp256k1 library](https://github.com/bitcoin-core/secp256k1) and [my hashing algorith](https://github.com/rimvydascivilis/blockchain-1)

### Block

- Block contains:
  - previous block hash
  - merkle root hash
  - timestamp
  - nonce
  - version
  - difficulty
  - transactions

### Blockchain

- Blockchain contains:
  - chain of blocks
  - transaction pool
  - unspent transaction outputs

### User

- User contains:
  - name
  - private key
  - public key

### Mining

- Block is mined using [Proof of Work](https://en.wikipedia.org/wiki/Proof_of_work) algorithm.
- Mining is implemented using [my hashing algorith](https://github.com/rimvydascivilis/blockchain-1).
- Mining is multithreaded. Each thread tries to mine a block with a different nonce. The first thread that finds a valid block wins.

### UTXO Model

- Blockchain uses [UTXO model](https://en.wikipedia.org/wiki/Unspent_transaction_output).
- UTXO is implemented using [std::unordered_map](https://en.cppreference.com/w/cpp/container/unordered_map).
- UTXO is updated after each block is mined.
