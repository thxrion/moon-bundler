TARGET="bundler.exe"
SRC_DIR="src"
BIN_DIR="bin"
TEST_DIR="test"
TEST_BUNDLE_DIR="bundle"

CC="gcc"
LINKER="$CC"
CFLAGS="-O0 -Wall"

EXECUTABLE="$BIN_DIR/$TARGET"
OBJ_DIR="$BIN_DIR/.obj"

if [ $# -eq 0 ]; then
    echo "No arguments provided. Try 'help' for usage information."
    exit 1
fi

build() {
    SRC=$(find "$SRC_DIR" -name "*.c")
    OBJ=$(echo "$SRC" | sed "s|$SRC_DIR/|$OBJ_DIR/|" | sed "s|\.c$|\.o|")

    mkdir -p "$BIN_DIR"

    for src in $SRC; do
        obj=$(echo "$src" | sed "s|$SRC_DIR/|$OBJ_DIR/|" | sed "s|\.c$|\.o|")
        mkdir -p "$(dirname "$obj")"
        $CC -c $CFLAGS "$src" -o "$obj"
    done

    $LINKER $OBJ -o "$BIN_DIR/$TARGET"
}

run_tests() {
    set -e

    if [ ! -x "$EXECUTABLE" ]; then
        echo "Executable not found at $EXECUTABLE"
        exit 1
    fi

    TESTS=$(find $TEST_DIR/* -maxdepth 0 -type d -exec basename {} \;)

    for test in $TESTS; do
        echo "Running test: $test"

        BUNDLE_PATH="$TEST_DIR/${test}/$TEST_BUNDLE_DIR"

        rm -rf $BUNDLE_PATH

        (cd "$TEST_DIR/$test" && "../../$EXECUTABLE")

        echo "1. Looking for bundle."

        if ! [ -d $BUNDLE_PATH ]; then
            echo -e "Test \e[31mfailed\e[0m. Bundle not found.\n"

            continue
        else
            echo "Bundle found."
        fi

        echo "2. Comparing bundle with expected result."

        TEST_BUILD="$BUNDLE_PATH/result.lua"
        EXPECTED_BUILD="$TEST_DIR/$test-expected.lua"

        if ! cmp -s "$TEST_BUILD" "$EXPECTED_BUILD"; then
            echo -e "Test \e[31mfailed\e[0m. Bundle does not match expected result\n"
            continue
        else
            echo "Bundle matches expected result."
        fi

        echo -e "Test \e[32mpassed\e[0m.\n"
    done
}

clean() {
    rm -rf $BIN_DIR

    TESTS=$(find $TEST_DIR/* -maxdepth 0 -type d -exec basename {} \;)

    for test in $TESTS; do
        rm -rf "$TEST_DIR/${test}/$TEST_BUNDLE_DIR"
    done
}

display_usage() {
    echo "Options:"
    echo "  help          Display help message"
    echo "  build         Build project"
    echo "  test          Run tests"
    echo "  clean         Clean build directory"
}

while [ $# -gt 0 ]; do
    case $1 in
        help)
            display_usage
            exit 0
            ;;
        build)
            build        
            exit 0
            ;;
        test)
            run_tests
            exit 0
            ;;
        clean)
            clean
            exit 0
            ;;
        *)
            echo "Invalid option: $1. Try 'help' for usage information."
            exit 1
            ;;
    esac
done