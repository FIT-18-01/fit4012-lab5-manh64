#!/usr/bin/env bash
set -euo pipefail

fail() {
  echo "[FAIL] $1" >&2
  exit 1
}

pass() {
  echo "[PASS] $1"
}

file_exists() {
  local path="$1"
  [[ -f "$path" ]] || fail "Thiếu file bắt buộc: $path"
}

dir_exists() {
  local path="$1"
  [[ -d "$path" ]] || fail "Thiếu thư mục bắt buộc: $path/"
}

contains_ci() {
  local path="$1"
  local pattern="$2"
  grep -R -E -i -q "$pattern" "$path"
}

file_exists "encrypt.cpp"
file_exists "decrypt.cpp"
file_exists "structures.h"
file_exists "README.md"
file_exists "report-1page.md"
file_exists "Makefile"
dir_exists "tests"
dir_exists "logs"
pass "Đủ các file/thư mục bắt buộc cơ bản."

TEST_COUNT=$(find tests -maxdepth 1 -type f -name '*.sh' | wc -l | tr -d ' ')
if [[ "$TEST_COUNT" -lt 5 ]]; then
  fail "Thư mục tests/ phải có ít nhất 5 file test. Hiện có: $TEST_COUNT"
fi
pass "Thư mục tests/ có ít nhất 5 file test."

contains_ci README.md 'how to run|cách chạy|build and run|compile|biên dịch' \
  || fail "README.md phải có hướng dẫn chạy chương trình."
contains_ci README.md 'input|đầu vào' \
  || fail "README.md phải mô tả input/đầu vào."
contains_ci README.md 'output|đầu ra' \
  || fail "README.md phải mô tả output/đầu ra."
contains_ci README.md 'padding' \
  || fail "README.md phải giải thích padding đang dùng hoặc không dùng."
contains_ci README.md 'ethics|safe use|an toàn|đạo đức' \
  || fail "README.md phải có mục Ethics & Safe use / an toàn sử dụng."
pass "README.md có các mục tối thiểu theo yêu cầu."

contains_ci report-1page.md 'mục tiêu|objective' \
  || fail "report-1page.md phải có mục Mục tiêu / Objective."
contains_ci report-1page.md 'cách làm|phương pháp|approach|method' \
  || fail "report-1page.md phải có mục Cách làm / Approach / Method."
contains_ci report-1page.md 'kết quả|result' \
  || fail "report-1page.md phải có mục Kết quả / Result."
contains_ci report-1page.md 'kết luận|conclusion' \
  || fail "report-1page.md phải có mục Kết luận / Conclusion."
pass "report-1page.md có đủ các mục tối thiểu."

LOG_EVIDENCE_COUNT=$(find logs -maxdepth 1 -type f ! -name '.gitkeep' ! -name 'README.md' | wc -l | tr -d ' ')
if [[ "$LOG_EVIDENCE_COUNT" -lt 1 ]]; then
  fail "Thư mục logs/ phải có ít nhất 1 file minh chứng thật."
fi
pass "Có file minh chứng trong logs/."

contains_ci tests 'tamper|flip[ -]?1[ -]?byte|bit flip|sửa 1 byte' \
  || fail "tests/ phải có negative test cho tamper / flip 1 byte."
contains_ci tests 'wrong key|invalid key|incorrect key|sai key|khóa sai' \
  || fail "tests/ phải có negative test cho wrong key."
pass "Có dấu hiệu negative tests cho tamper và wrong key."

SOURCE_FILES=$(find . -maxdepth 2 \( -name '*.cpp' -o -name '*.h' -o -name '*.hpp' \) ! -path './.github/*' ! -path './build/*')
grep -E -i -q 'cin\s*>>|getline\s*\(' $SOURCE_FILES \
  || fail "Chưa thấy dấu hiệu nhập từ bàn phím trong source code."
pass "Có dấu hiệu nhập dữ liệu từ bàn phím trong source code."

if grep -R -n "TODO_STUDENT" README.md report-1page.md tests/; then
  fail "Vẫn còn placeholder TODO_STUDENT trong README/report/tests."
fi
pass "Không còn placeholder TODO_STUDENT trong README/report/tests."

echo "[SUCCESS] Repo đáp ứng bộ kiểm tra nộp bài cơ bản cho FIT4012 Lab 4 AES."
