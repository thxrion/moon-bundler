TARGET="bundler.exe"
CC="gcc"
LINKER="$CC"
CFLAGS="-O0 -Wall"

SRC_DIR="src"
BIN_DIR="bin"
OBJ_DIR="$BIN_DIR/.obj"

SRC=$(find "$SRC_DIR" -name "*.c")
OBJ=$(echo "$SRC" | sed "s|$SRC_DIR/|$OBJ_DIR/|" | sed "s|\.c$|\.o|")

mkdir -p "$BIN_DIR"

for src in $SRC; do
    obj=$(echo "$src" | sed "s|$SRC_DIR/|$OBJ_DIR/|" | sed "s|\.c$|\.o|")
    mkdir -p "$(dirname "$obj")"
    $CC -c $CFLAGS "$src" -o "$obj"
done

$LINKER $OBJ -o "$BIN_DIR/$TARGET"
