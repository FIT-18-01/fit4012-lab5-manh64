#!/usr/bin/env bash
set -euo pipefail

make all

PLAINTEXT="hello FIT4012 AES"

{
  echo "[INFO] Plaintext: ${PLAINTEXT}"
  echo "[INFO] Running ./encrypt"
  printf "%s\n" "$PLAINTEXT" | ./encrypt
  echo
  echo "[INFO] Running ./decrypt"
  ./decrypt
} | tee logs/sample-run.log
