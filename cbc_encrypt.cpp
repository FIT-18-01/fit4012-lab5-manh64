/* encrypt.cpp
 * Performs encryption using AES 128-bit
 */
#include <vector>
#include <iomanip>
#include <iostream>
#include <cstring>
#include <fstream>
#include <sstream>
#include "structures.h"

using namespace std;

/* Serves as the initial round during encryption
 * AddRoundKey is simply an XOR of a 128-bit block with the 128-bit key.
 */
void AddRoundKey(unsigned char * state, unsigned char * roundKey) {
	for (int i = 0; i < 16; i++) {
		state[i] ^= roundKey[i];
	}
}

/* Perform substitution to each of the 16 bytes
 * Uses S-box as lookup table 
 */
void SubBytes(unsigned char * state) {
	for (int i = 0; i < 16; i++) {
		state[i] = s[state[i]];
	}
}

// Shift left, adds diffusion
void ShiftRows(unsigned char * state) {
	unsigned char tmp[16];

	/* Column 1 */
	tmp[0] = state[0];
	tmp[1] = state[5];
	tmp[2] = state[10];
	tmp[3] = state[15];
	
	/* Column 2 */
	tmp[4] = state[4];
	tmp[5] = state[9];
	tmp[6] = state[14];
	tmp[7] = state[3];

	/* Column 3 */
	tmp[8] = state[8];
	tmp[9] = state[13];
	tmp[10] = state[2];
	tmp[11] = state[7];
	
	/* Column 4 */
	tmp[12] = state[12];
	tmp[13] = state[1];
	tmp[14] = state[6];
	tmp[15] = state[11];

	for (int i = 0; i < 16; i++) {
		state[i] = tmp[i];
	}
}

 /* MixColumns uses mul2, mul3 look-up tables
  * Source of diffusion
  */
void MixColumns(unsigned char * state) {
	unsigned char tmp[16];

	tmp[0] = (unsigned char) mul2[state[0]] ^ mul3[state[1]] ^ state[2] ^ state[3];
	tmp[1] = (unsigned char) state[0] ^ mul2[state[1]] ^ mul3[state[2]] ^ state[3];
	tmp[2] = (unsigned char) state[0] ^ state[1] ^ mul2[state[2]] ^ mul3[state[3]];
	tmp[3] = (unsigned char) mul3[state[0]] ^ state[1] ^ state[2] ^ mul2[state[3]];

	tmp[4] = (unsigned char)mul2[state[4]] ^ mul3[state[5]] ^ state[6] ^ state[7];
	tmp[5] = (unsigned char)state[4] ^ mul2[state[5]] ^ mul3[state[6]] ^ state[7];
	tmp[6] = (unsigned char)state[4] ^ state[5] ^ mul2[state[6]] ^ mul3[state[7]];
	tmp[7] = (unsigned char)mul3[state[4]] ^ state[5] ^ state[6] ^ mul2[state[7]];

	tmp[8] = (unsigned char)mul2[state[8]] ^ mul3[state[9]] ^ state[10] ^ state[11];
	tmp[9] = (unsigned char)state[8] ^ mul2[state[9]] ^ mul3[state[10]] ^ state[11];
	tmp[10] = (unsigned char)state[8] ^ state[9] ^ mul2[state[10]] ^ mul3[state[11]];
	tmp[11] = (unsigned char)mul3[state[8]] ^ state[9] ^ state[10] ^ mul2[state[11]];

	tmp[12] = (unsigned char)mul2[state[12]] ^ mul3[state[13]] ^ state[14] ^ state[15];
	tmp[13] = (unsigned char)state[12] ^ mul2[state[13]] ^ mul3[state[14]] ^ state[15];
	tmp[14] = (unsigned char)state[12] ^ state[13] ^ mul2[state[14]] ^ mul3[state[15]];
	tmp[15] = (unsigned char)mul3[state[12]] ^ state[13] ^ state[14] ^ mul2[state[15]];

	for (int i = 0; i < 16; i++) {
		state[i] = tmp[i];
	}
}

/* Each round operates on 128 bits at a time
 * The number of rounds is defined in AESEncrypt()
 */
void Round(unsigned char * state, unsigned char * key) {
	SubBytes(state);
	ShiftRows(state);
	MixColumns(state);
	AddRoundKey(state, key);
}

 // Same as Round() except it doesn't mix columns
void FinalRound(unsigned char * state, unsigned char * key) {
	SubBytes(state);
	ShiftRows(state);
	AddRoundKey(state, key);
}

/* The AES encryption function
 * Organizes the confusion and diffusion steps into one function
 */
void AESEncrypt(unsigned char * message, unsigned char * expandedKey, unsigned char * encryptedMessage) {
	unsigned char state[16]; // Stores the first 16 bytes of original message

	for (int i = 0; i < 16; i++) {
		state[i] = message[i];
	}

	int numberOfRounds = 9;

	AddRoundKey(state, expandedKey); // Initial round

	for (int i = 0; i < numberOfRounds; i++) {
		Round(state, expandedKey + (16 * (i+1)));
	}

	FinalRound(state, expandedKey + 160);

	// Copy encrypted state to buffer
	for (int i = 0; i < 16; i++) {
		encryptedMessage[i] = state[i];
	}
}

int main() {
	cout << "=============================" << endl;
	cout << " AES-128 CBC Encryption Tool " << endl;
	cout << "=============================" << endl;

	string input;
	cout << "Enter the message to encrypt: ";
	getline(cin, input);

	int originalLen = input.size();
	int paddedMessageLen = originalLen;

	if ((paddedMessageLen % 16) != 0) {
		paddedMessageLen = (paddedMessageLen / 16 + 1) * 16;
	}

	vector<unsigned char> paddedMessage(paddedMessageLen, 0);
	for (int i = 0; i < originalLen; i++) {
		paddedMessage[i] = static_cast<unsigned char>(input[i]);
	}

	string keystr;
	ifstream keyfile("keyfile", ios::in | ios::binary);
	if (keyfile.is_open()) {
		getline(keyfile, keystr);
		keyfile.close();
	} else {
		cout << "Unable to open keyfile" << endl;
		return 1;
	}

	istringstream hex_chars_stream(keystr);
	unsigned char key[16];
	int idx = 0;
	unsigned int c;

	while (hex_chars_stream >> hex >> c && idx < 16) {
		key[idx] = static_cast<unsigned char>(c);
		idx++;
	}

	unsigned char expandedKey[176];
	KeyExpansion(key, expandedKey);

	unsigned char iv[16] = {
		0x00, 0x01, 0x02, 0x03,
		0x04, 0x05, 0x06, 0x07,
		0x08, 0x09, 0x0a, 0x0b,
		0x0c, 0x0d, 0x0e, 0x0f
	};

	vector<unsigned char> encryptedMessage(paddedMessageLen);
	unsigned char previous[16];

	for (int i = 0; i < 16; i++) {
		previous[i] = iv[i];
	}

	for (int block = 0; block < paddedMessageLen; block += 16) {
		unsigned char xoredBlock[16];
		unsigned char encryptedBlock[16];

		for (int i = 0; i < 16; i++) {
			xoredBlock[i] = paddedMessage[block + i] ^ previous[i];
		}

		AESEncrypt(xoredBlock, expandedKey, encryptedBlock);

		for (int i = 0; i < 16; i++) {
			encryptedMessage[block + i] = encryptedBlock[i];
			previous[i] = encryptedBlock[i];
		}
	}

	cout << "IV in hex:" << endl;
	for (int i = 0; i < 16; i++) {
		cout << hex << setw(2) << setfill('0') << (int)iv[i] << " ";
	}
	cout << endl;

	cout << "CBC encrypted message in hex:" << endl;
	for (int i = 0; i < paddedMessageLen; i++) {
		cout << hex << setw(2) << setfill('0') << (int)encryptedMessage[i] << " ";
	}
	cout << endl;

	ofstream outfile("message_cbc.aes", ios::out | ios::binary);
	if (outfile.is_open()) {
		outfile.write(reinterpret_cast<char*>(iv), 16);
		outfile.write(reinterpret_cast<char*>(encryptedMessage.data()), encryptedMessage.size());
		outfile.close();
		cout << "Wrote IV and encrypted message to file message_cbc.aes" << endl;
	} else {
		cout << "Unable to open output file" << endl;
		return 1;
	}

	return 0;
}