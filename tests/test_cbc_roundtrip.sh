#!/usr/bin/env bash
set -euo pipefail

make all

PLAINTEXT="hello FIT4012 AES CBC mode with more than one block"

printf "%s\n" "$PLAINTEXT" | ./cbc_encrypt > /tmp/cbc_encrypt_output.txt

OUTPUT=$(./cbc_decrypt | tr -d '\000')

echo "$OUTPUT" | grep -q "$PLAINTEXT"

echo "[PASS] CBC encrypt/decrypt round-trip recovers plaintext."