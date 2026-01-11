#!/bin/bash

set -e

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
PROJECT_ROOT="$(dirname "$SCRIPT_DIR")"
BUILD_DIR="$PROJECT_ROOT/build"

BUILD_TESTS="OFF"
RUN_CLEAN=false
RUN_TESTS=false

while getopts "tcr" opt; do
  case $opt in
    t) BUILD_TESTS="ON" ;;
    c) RUN_CLEAN=true ;;
    r) RUN_TESTS=true ;;
    *) echo "Usage: $0 [-t] [-c] [-r]"; exit 1 ;;
  esac
done

echo "--- Checking Dependencies ---"
if ! ldconfig -p | grep -q libpqxx; then
    echo "Error: libpqxx-dev is not installed."
    exit 1
fi

echo "--- Ensuring PostgreSQL is running ---"
if ! pg_isready -h localhost > /dev/null 2>&1; then
    sudo -u postgres /usr/lib/postgresql/16/bin/pg_ctl \
      -D /var/lib/postgresql/16/main \
      -o "-c config_file=/etc/postgresql/16/main/postgresql.conf" \
      start || true
    
    RETRIES=10
    while ! pg_isready -h localhost > /dev/null 2>&1 && [ $RETRIES -gt 0 ]; do
        echo "Waiting for PostgreSQL... ($RETRIES)"
        sleep 1
        ((RETRIES--))
    done
fi

echo "--- Preparing Database ---"
# Сначала ставим пароль через локальный сокет (peer auth), где пароль не нужен
sudo -u postgres psql -c "ALTER USER postgres WITH PASSWORD 'root';"

# Теперь работаем через сеть, передавая пароль напрямую в команде для надежности
export PGPASSWORD='root'

# Создаем БД (флаг -h localhost задействует PGPASSWORD)
psql -h localhost -U postgres -tAc "SELECT 1 FROM pg_database WHERE datname='test_db'" | grep -q 1 || \
psql -h localhost -U postgres -c "CREATE DATABASE test_db;"

if [ -f "$PROJECT_ROOT/sql/init.sql" ]; then
    echo "--- Initializing Schema ---"
    psql -h localhost -U postgres -d test_db -f "$PROJECT_ROOT/sql/init.sql"
fi

unset PGPASSWORD

if [ "$RUN_CLEAN" = true ] && [ -d "$BUILD_DIR" ]; then
    rm -rf "$BUILD_DIR"
fi

mkdir -p "$BUILD_DIR"
cd "$BUILD_DIR"

echo "--- Configuring Project ---"
cmake "$PROJECT_ROOT" -DBUILD_TESTING=$BUILD_TESTS

echo "--- Formatting Code ---"
cmake --build . --target format || true

echo "--- Building Project ---"
cmake --build . -- -j$(nproc)

echo "--- Syncing SQL Resources ---"
if [ -d "$PROJECT_ROOT/sql" ]; then
    cp -r "$PROJECT_ROOT/sql" "$BUILD_DIR/"
    mkdir -p "$BUILD_DIR/tests/sql"
    cp -r "$PROJECT_ROOT/sql/"* "$BUILD_DIR/tests/sql/"
fi

if [ "$RUN_TESTS" = true ]; then
    if [ "$BUILD_TESTS" = "ON" ]; then
        echo "--- Running Tests ---"
        ctest --output-on-failure
    else
        echo "Error: Use -t to build tests first."
        exit 1
    fi
fi

echo "--- Done ---"