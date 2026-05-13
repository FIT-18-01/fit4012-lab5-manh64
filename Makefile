CXX := g++
CXXFLAGS := -std=c++17 -Wall -Wextra -pedantic

ENCRYPT_TARGET := encrypt
DECRYPT_TARGET := decrypt
CBC_ENCRYPT_TARGET := cbc_encrypt
CBC_DECRYPT_TARGET := cbc_decrypt

.PHONY: all clean run encrypt-sample decrypt-sample cbc-sample test

all: $(ENCRYPT_TARGET) $(DECRYPT_TARGET) $(CBC_ENCRYPT_TARGET) $(CBC_DECRYPT_TARGET)

$(ENCRYPT_TARGET): encrypt.cpp structures.h
	$(CXX) $(CXXFLAGS) encrypt.cpp -o $(ENCRYPT_TARGET)

$(DECRYPT_TARGET): decrypt.cpp structures.h
	$(CXX) $(CXXFLAGS) decrypt.cpp -o $(DECRYPT_TARGET)

$(CBC_ENCRYPT_TARGET): cbc_encrypt.cpp structures.h
	$(CXX) $(CXXFLAGS) cbc_encrypt.cpp -o $(CBC_ENCRYPT_TARGET)

$(CBC_DECRYPT_TARGET): cbc_decrypt.cpp structures.h
	$(CXX) $(CXXFLAGS) cbc_decrypt.cpp -o $(CBC_DECRYPT_TARGET)

run: all
	bash scripts/run_sample.sh

encrypt-sample: $(ENCRYPT_TARGET)
	printf "hello FIT4012 AES\n" | ./$(ENCRYPT_TARGET)

decrypt-sample: $(DECRYPT_TARGET)
	./$(DECRYPT_TARGET)

cbc-sample: $(CBC_ENCRYPT_TARGET) $(CBC_DECRYPT_TARGET)
	printf "hello FIT4012 AES CBC mode\n" | ./$(CBC_ENCRYPT_TARGET)
	./$(CBC_DECRYPT_TARGET)

test: all
	bash tests/test_aes_compile.sh
	bash tests/test_encrypt_decrypt_roundtrip.sh
	bash tests/test_multiblock_padding.sh
	bash tests/test_cbc_roundtrip.sh
	bash tests/test_tamper_negative.sh
	bash tests/test_wrong_key_negative.sh

clean:
	rm -f $(ENCRYPT_TARGET) $(DECRYPT_TARGET) $(CBC_ENCRYPT_TARGET) $(CBC_DECRYPT_TARGET) message.aes message_cbc.aes
	rm -rf build