#ifndef UTIL_H
#define UTIL_H

#include <string>
#include <bitset>
#include <vector>

using namespace std;

string join(vector<pair<string, int>> v, string delimiter) {
    string result = "";
    for (unsigned int i=0; i<v.size(); i++) {
        result += v[i].first + ":" + to_string(v[i].second);
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

#endif // UTIL_H
