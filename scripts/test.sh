set -e

EXECUTABLE="bin/bundler.exe"

TESTS=$(find test/* -maxdepth 0 -type d -exec basename {} \;)

if [ ! -x "$EXECUTABLE" ]; then
    echo "Executable not found at $EXECUTABLE"
    exit 1
fi

for test in $TESTS; do
    echo "Running test: $test"
    (cd "test/$test" && "../../$EXECUTABLE")
done