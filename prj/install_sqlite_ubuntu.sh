#!/usr/bin/env bash
# install_ubuntu_sqlite.sh
# Install SQLite (sqlite3) and its header files

set -euo pipefail

echo "[1/5] Updating apt index..."
sudo apt-get update -y

echo "[2/5] Installing packages..."
# sqlite3:     CLI toolkit
# libsqlite3-0: runtime
# libsqlite3-dev: for development, sqlite3.h included
# pkg-config:  for compile falgs, such as  -I / -L -l variables
# build-essential: g++/make basic compile toolkit
sudo apt-get install -y \
  sqlite3 \
  libsqlite3-0 \
  libsqlite3-dev \
  pkg-config \
  build-essential

echo "[3/5] Verifying header and pkg-config..."
if [[ -f /usr/include/sqlite3.h ]]; then
  echo "  - Found header: /usr/include/sqlite3.h"
else
  echo "  ! sqlite3.h not found under /usr/include (unexpected)"
  exit 1
fi

if pkg-config --exists sqlite3; then
  echo "  - pkg-config cflags: $(pkg-config --cflags sqlite3)"
  echo "  - pkg-config libs:   $(pkg-config --libs   sqlite3)"
else
  echo "  ! pkg-config cannot find sqlite3 (unexpected)"
  exit 1
fi

echo "[4/5] Building a tiny compile/link test..."
cat >/tmp/test_sqlite.cpp <<'CPP'
#include <sqlite3.h>
#include <cstdio>
int main() {
  printf("sqlite3 version: %s\n", sqlite3_libversion());
  return 0;
}
CPP

g++ -std=c++11 /tmp/test_sqlite.cpp -o /tmp/test_sqlite $(pkg-config --cflags --libs sqlite3)

echo "[5/5] Running the test binary..."
/tmp/test_sqlite

cat <<'NOTE'

Done.

if it is needed for makefile, it is suggested for the setting (pkg-config installed): 

  CFLAG_SQLITE := $(shell pkg-config --cflags sqlite3)
  LFLAG_SQLITE := $(shell pkg-config --libs   sqlite3)

  CFLAGS = ... $(CFLAG_SQLITE) ...
  LFLAGS = ... $(LFLAG_SQLITE) ...

or for (no pkg-config's fallback):
  CFLAGS += -I/usr/include
  LFLAGS += -lsqlite3 -ldl -lpthread

NOTE

