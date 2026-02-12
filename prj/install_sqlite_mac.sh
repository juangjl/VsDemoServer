#!/usr/bin/env bash
# install_sqlite_mac.sh
# Install SQLite for macOS, supports Apple Silicon and Intel chipset
# Author: jl_jaung@vsigntek.com

set -euo pipefail

echo "=============================================="
echo "[1/5] Check Homebrew ..."
echo "=============================================="
if ! command -v brew >/dev/null 2>&1; then
  echo "(X) Homebrew not detected, please install it:"
  echo "   /bin/bash -c \"\$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)\""
  exit 1
fi

echo "(V)Homebrew has been installed"

echo "=============================================="
echo "[2/5] Install SQLite ..."
echo "=============================================="
brew update
brew install sqlite

echo "=============================================="
echo "[3/5] Createsymbolic link(make sure sqlite3.h could be found by system)..."
echo "=============================================="

SQLITE_PREFIX=$(brew --prefix sqlite 2>/dev/null)
INCLUDE_DIR="$SQLITE_PREFIX/include"
LIB_DIR="$SQLITE_PREFIX/lib"

echo "SQLite prefix: $SQLITE_PREFIX"

# Create symbolic link(For /usr/local/include /usr/local/lib or Apple Silicon's /opt/homebrew)
if [[ -d "$INCLUDE_DIR" ]]; then
  sudo mkdir -p /usr/local/include || true
  sudo mkdir -p /usr/local/lib || true
  sudo ln -sf "$INCLUDE_DIR/sqlite3.h" /usr/local/include/sqlite3.h
  sudo ln -sf "$LIB_DIR/libsqlite3.dylib" /usr/local/lib/libsqlite3.dylib
  echo "(V) built /usr/local/include/sqlite3.h 與 /usr/local/lib/libsqlite3.dylib"
fi

echo "=============================================="
echo "[4/5] Verify the installation of sqlite3 ..."
echo "=============================================="
if [[ -f "$INCLUDE_DIR/sqlite3.h" ]]; then
  echo "(V) sqlite3.h found in $INCLUDE_DIR"
else
  echo "(X) Couldn't find sqlite3.h, please make sure the brew install successfully donoe"
  exit 1
fi

echo "[Check pkg-config]"
if pkg-config --exists sqlite3; then
  echo "(V) pkg-config 已偵測到 sqlite3"
  echo "   CFLAGS: $(pkg-config --cflags sqlite3)"
  echo "   LIBS  : $(pkg-config --libs sqlite3)"
else
  echo "(!) pkg-config not registed sqlite3, will add  sqlite3.pc link..."
  PC_PATH=$(pkg-config --variable pc_path pkg-config | cut -d: -f1)
  sudo ln -sf "$SQLITE_PREFIX/lib/pkgconfig/sqlite3.pc" "$PC_PATH/sqlite3.pc" || true
fi

echo "=============================================="
echo "[5/5] compile test ..."
echo "=============================================="
cat >/tmp/test_sqlite.cpp <<'CPP'
#include <sqlite3.h>
#include <cstdio>
int main() {
  printf("SQLite version: %s\n", sqlite3_libversion());
  return 0;
}
CPP

g++ -std=c++11 /tmp/test_sqlite.cpp -o /tmp/test_sqlite $(pkg-config --cflags --libs sqlite3)
echo "(V) Complile test successfully done, the result:"
/tmp/test_sqlite

echo "=============================================="
echo "GOOD! SQLite installation done, system can use #include <sqlite3.h>"
echo "=============================================="

