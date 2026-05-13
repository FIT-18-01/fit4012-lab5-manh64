# Report 1 page - Lab 4 AES-128

## Mục tiêu

Bài thực hành giúp sinh viên hiểu quy trình mã hóa và giải mã AES-128 ở mức nhập môn, bao gồm xử lý block 128-bit, mở rộng khóa, các phép biến đổi theo vòng và cơ chế padding đơn giản.

## Cách làm / Method

Repo sử dụng 3 file mã nguồn chính: `encrypt.cpp`, `decrypt.cpp`, `structures.h`. File `encrypt.cpp` thực hiện mã hóa plaintext và ghi ciphertext ra `message.aes`. File `decrypt.cpp` đọc `message.aes` để giải mã. File `structures.h` chứa S-box, inverse S-box, các bảng tra cứu nhân trong GF(2^8), RCon và hàm KeyExpansion.

Repo được cấu trúc lại theo mẫu starter repo của FIT4012 Lab 4: có `Makefile`, `CMakeLists.txt`, thư mục `tests/`, `logs/`, `scripts/` và GitHub Actions CI.

## Kết quả / Result

Chương trình có thể biên dịch bằng Makefile hoặc CMake. Khi chạy mẫu với plaintext `hello FIT4012 AES`, chương trình tạo file `message.aes`, sau đó chương trình giải mã đọc file này và khôi phục lại plaintext ban đầu kèm các byte padding `0x00` ở cuối block.

Các test cơ bản gồm: biên dịch, round-trip encrypt/decrypt, plaintext nhiều block, sai khóa và ciphertext bị sửa đổi.

## Kết luận / Conclusion

Bài lab phù hợp để minh họa luồng xử lý AES-128 và giúp sinh viên hiểu mối quan hệ giữa mã hóa, giải mã, key expansion và padding. Hạn chế hiện tại là cách đọc/ghi ciphertext theo C-style string chưa thật sự an toàn cho dữ liệu binary; do đó có thể mở rộng bằng cách đọc/ghi theo byte vector, thêm PKCS#7 padding và bổ sung test vector chuẩn AES.
