# Ghi chú cấu trúc code AES

## 1. File `encrypt.cpp`

Vai trò chính:

- đọc plaintext từ bàn phím
- pad plaintext về bội số của 16 byte
- đọc khóa từ `keyfile`
- gọi `KeyExpansion`
- mã hóa từng block 16 byte bằng AES-128
- ghi ciphertext ra `message.aes`

Các hàm chính:

- `AddRoundKey`
- `SubBytes`
- `ShiftRows`
- `MixColumns`
- `Round`
- `FinalRound`
- `AESEncrypt`

## 2. File `decrypt.cpp`

Vai trò chính:

- đọc ciphertext từ `message.aes`
- đọc khóa từ `keyfile`
- gọi `KeyExpansion`
- giải mã từng block 16 byte
- in plaintext khôi phục

Các hàm chính:

- `SubRoundKey`
- `InverseMixColumns`
- `ShiftRows` theo chiều ngược
- `SubBytes` dùng inverse S-box
- `InitialRound`
- `AESDecrypt`

## 3. File `structures.h`

Vai trò chính:

- chứa S-box và inverse S-box
- chứa bảng tra cứu dùng cho MixColumns và InverseMixColumns
- chứa `rcon`
- triển khai `KeyExpansionCore`
- triển khai `KeyExpansion`

## 4. Gợi ý cải tiến cho sinh viên

- Tách phần AES core ra `aes.h` và `aes.cpp`.
- Tách CLI ra `main.cpp`.
- Không dùng `strlen` với dữ liệu ciphertext binary.
- Không ghi ciphertext bằng toán tử `<<` với con trỏ `unsigned char*`.
- Dùng `std::vector<unsigned char>` và `ofstream::write` / `ifstream::read`.
- Dùng PKCS#7 padding thay cho zero padding.
