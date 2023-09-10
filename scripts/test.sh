set -e

EXECUTABLE="bin/bundler.exe"

TESTS=$(find tests/* -maxdepth 0 -type d -exec basename {} \;)

if [ ! -x "$EXECUTABLE" ]; then
    echo "Executable not found at $EXECUTABLE"
    exit 1
fi

for test in $TESTS; do
    echo "Running test: $test"

    BUNDLE_PATH="tests/${test}/bundle"

    rm -rf $BUNDLE_PATH

    (cd "tests/$test" && "../../$EXECUTABLE")

    echo "1. Looking for bundle."

    if ! [ -d $BUNDLE_PATH ]; then
        echo ""
        echo -e "Test \e[31mfailed\e[0m. Bundle not found.\n"

        continue
    else
        echo "Bundle found."
    fi

    echo "2. Comparing bundle with expected result."

    TEST_BUILD="$BUNDLE_PATH/result.lua"
    EXPECTED_BUILD="tests/$test-expected.lua"

    if ! cmp -s "$TEST_BUILD" "$EXPECTED_BUILD"; then
        echo -e "Test \e[31mfailed\e[0m. Bundle does not match expected result\n"
        continue
    else
        echo "Bundle matches expected result."
    fi

    echo -e "Test \e[32mpassed\e[0m.\n"
done