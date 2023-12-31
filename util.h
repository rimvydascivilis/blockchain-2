#ifndef UTIL_H
#define UTIL_H

#include <string>
#include <bitset>
#include <vector>
#include <sys/random.h>
#include <sstream>
#include <iomanip>
#include <cstring>

using namespace std;

string join(vector<input_t> v, string delimiter) {
    string result = "";
    for (unsigned int i=0; i<v.size(); i++) {
        result += v[i].transaction_hash + ":" + to_string(v[i].output_index);
        if (i != v.size() - 1) {
            result += delimiter;
        }
    }
    return result;
}

string join (vector<output_t> v, string delimiter) {
    string result = "";
    for (unsigned int i=0; i<v.size(); i++) {
        result += v[i].address + ":" + to_string(v[i].amount);
        if (i != v.size() - 1) {
            result += delimiter;
        }
    }
    return result;
}

string convert_to_hex(bitset<256> hash) {
    string bits = hash.to_string(), hex, chr, tmp;
    for (unsigned int i=0; i<bits.length(); i+=4) {
        tmp = bits.substr(i,4);
        if (!tmp.compare("0000")) {
            hex = hex + "0";
        }
        else if (!tmp.compare("0001")) {
            hex = hex + "1";
        }
        else if (!tmp.compare("0010")) {
            hex = hex + "2";
        }
        else if (!tmp.compare("0011")) {
            hex = hex + "3";
        }
        else if (!tmp.compare("0100")) {
            hex = hex + "4";
        }
        else if (!tmp.compare("0101")) {
            hex = hex + "5";
        }
        else if (!tmp.compare("0110")) {
            hex = hex + "6";
        }
        else if (!tmp.compare("0111")) {
            hex = hex + "7";
        }
        else if (!tmp.compare("1000")) {
            hex = hex + "8";
        }
        else if (!tmp.compare("1001")) {
            hex = hex + "9";
        }
        else if (!tmp.compare("1010")) {
            hex = hex + "A";
        }
        else if (!tmp.compare("1011")) {
            hex = hex + "B";
        }
        else if (!tmp.compare("1100")) {
            hex = hex + "C";
        }
        else if (!tmp.compare("1101")) {
            hex = hex + "D";
        }
        else if (!tmp.compare("1110")) {
            hex = hex + "E";
        }
        else if (!tmp.compare("1111")) {
            hex = hex + "F";
        }
        else {
            continue;
        }
    }
    return hex;
}

vector<unsigned char> fillRandom(size_t size) {
    vector<unsigned char> data(size);
    ssize_t res = getrandom(data.data(), size, 0);
    if (res < 0 || static_cast<size_t>(res) != size) {
        cerr << "Failed to generate randomness\n";
        terminate();
    }
    return data;
}

vector<unsigned char> hexStringToBytes(const string& hex) {
    vector<unsigned char> bytes;
    for (size_t i = 0; i < hex.length(); i += 2) {
        string byteString = hex.substr(i, 2);
        bytes.push_back(static_cast<unsigned char>(stoi(byteString, nullptr, 16)));
    }
    return bytes;
}

string bytesToHexString(const unsigned char* bytes, size_t len) {
    stringstream ss;
    ss << hex << setfill('0');
    for (size_t i = 0; i < len; ++i) {
        ss << setw(2) << static_cast<int>(bytes[i]);
    }
    return ss.str();
}

void secureErase(void *ptr, size_t len) {
    memset(ptr, 0, len);
    __asm__ __volatile__("" : : "r"(ptr) : "memory");
}

#endif // UTIL_H
