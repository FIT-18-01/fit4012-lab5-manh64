[![Review Assignment Due Date](https://classroom.github.com/assets/deadline-readme-button-22041afd0340ce965d47ae6ef1cefeee28c7c493a6346c4f15d667ab976d596c.svg)](https://classroom.github.com/a/hzL_lEw5)
# FIT4012 - Lab 5: AES-128 Starter Repository

## 1. Cấu trúc repo

```text
.
├── .github/
│   ├── scripts/
│   │   └── check_submission.sh
│   └── workflows/
│       └── ci.yml
├── docs/
│   └── aes-code-notes.md
├── logs/
│   ├── .gitkeep
│   ├── README.md
│   └── sample-run.log
├── scripts/
│   └── run_sample.sh
├── tests/
│   ├── test_aes_compile.sh
│   ├── test_encrypt_decrypt_roundtrip.sh
│   ├── test_multiblock_padding.sh
│   ├── test_tamper_negative.sh
│   └── test_wrong_key_negative.sh
├── .gitignore
├── CMakeLists.txt
├── Makefile
├── README.md
├── decrypt.cpp
├── encrypt.cpp
├── keyfile
├── report-1page.md
└── structures.h
```

## 2. Cách chạy chương trình

### Cách 1: Dùng Makefile

```bash
make
printf "hello FIT4012 AES\n" | ./encrypt
./decrypt
```

### Cách 2: Chạy script mẫu

```bash
bash scripts/run_sample.sh
```

### Cách 3: Biên dịch trực tiếp

```bash
g++ -std=c++17 -Wall -Wextra -pedantic encrypt.cpp -o encrypt
g++ -std=c++17 -Wall -Wextra -pedantic decrypt.cpp -o decrypt
printf "hello FIT4012 AES\n" | ./encrypt
./decrypt
```

### Cách 4: Dùng CMake

```bash
cmake -S . -B build
cmake --build build
printf "hello FIT4012 AES\n" | ./build/encrypt
./build/decrypt
```

## 3. Input / Đầu vào

Chương trình mã hóa `encrypt.cpp` nhận plaintext từ bàn phím bằng `cin.getline(...)`.

Chương trình giải mã `decrypt.cpp` đọc ciphertext từ file `message.aes`.

Cả hai chương trình đều đọc khóa AES-128 từ file `keyfile`. File `keyfile` đang dùng định dạng 16 byte biểu diễn dưới dạng hex, ví dụ:

```text
01 04 02 03 01 03 04 0A 09 0B 07 0F 0F 06 03 00
```

## 4. Output / Đầu ra

- `encrypt` in ciphertext theo dạng hex ra màn hình.
- `encrypt` ghi ciphertext ra file `message.aes`.
- `decrypt` đọc `message.aes`, in plaintext dạng hex và plaintext dạng ký tự.

## 5. Padding đang dùng

Code hiện tại dùng **zero padding**: nếu plaintext không chia hết cho 16 byte, chương trình thêm byte `0x00` cho đủ block 128-bit.

Lưu ý: zero padding phù hợp để minh họa nhập môn, nhưng không phải lựa chọn an toàn/đầy đủ cho ứng dụng thực tế vì có thể gây nhập nhằng giữa byte dữ liệu thật `0x00` và byte padding.

## 6. Tests bắt buộc

Repo đã chuẩn bị 5 test shell cơ bản:

- `tests/test_aes_compile.sh`
- `tests/test_encrypt_decrypt_roundtrip.sh`
- `tests/test_multiblock_padding.sh`
- `tests/test_tamper_negative.sh`
- `tests/test_wrong_key_negative.sh`

Chạy toàn bộ test:

```bash
make test
```

## 7. Logs / Minh chứng

Thư mục `logs/` dùng để lưu minh chứng học tập, ví dụ:

- output khi build chương trình
- output khi encrypt/decrypt thành công
- output khi test sai khóa
- output khi test ciphertext bị sửa/tamper
- ảnh chụp màn hình hoặc log terminal khi nộp bài

## 8. Ethics & Safe use

- Chỉ chạy và kiểm thử trên dữ liệu học tập hoặc dữ liệu giả lập.
- Không dùng repo này để tấn công, che giấu dữ liệu vi phạm hoặc can thiệp hệ thống thật.
- Không trình bày đây là công cụ bảo mật sẵn sàng cho môi trường sản xuất.
- Nếu tham khảo mã, tài liệu, công cụ hoặc AI, phải ghi nguồn rõ ràng.
- Khi cộng tác nhóm, cần trung thực học thuật và mô tả đúng phần việc của mình.

## 9. Checklist nộp bài

Trước khi nộp, sinh viên cần có:

- `encrypt.cpp`, `decrypt.cpp`, `structures.h`
- `README.md` đã mô tả rõ cách chạy, input, output, padding
- `report-1page.md` đã hoàn thiện
- `tests/` có ít nhất 5 test
- có negative test cho `tamper` và `wrong key`
- `logs/` có ít nhất 1 file minh chứng thật
- không còn dòng `TODO_STUDENT` trong README/report/tests

## 10. Lưu ý kỹ thuật

Bản code này phục vụ mục tiêu học thuật để sinh viên quan sát các bước cơ bản của AES-128: SubBytes, ShiftRows, MixColumns, AddRoundKey và KeyExpansion.

Một số điểm sinh viên có thể cải tiến:

- tách code thành thư viện thay vì để nhiều logic trong `main`
- đọc/ghi binary an toàn hơn thay vì phụ thuộc vào chuỗi C-style
- chuyển từ zero padding sang PKCS#7 padding
- thêm chế độ nhập key từ bàn phím
- thêm mode lựa chọn encrypt/decrypt trong một chương trình duy nhất
- kiểm thử bằng known answer test vector chuẩn AES
