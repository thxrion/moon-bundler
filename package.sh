if [ $# -eq 0 ]; then
    echo "No arguments provided. Try 'help' for usage information."
    exit 1
fi

display_usage() {
    echo "Script options:"
    echo "  help          🧙 Display help message"
    echo "  build         👷 Build project"
    echo "  test          🐞 Run tests"
    echo "  clean         🧹 Clean build directory"
}

while [ $# -gt 0 ]; do
    case $1 in
        help)
            display_usage
            exit 0
            ;;
        build)
            bash ./scripts/build.sh            
            exit 0
            ;;
        test)
            bash ./scripts/test.sh
            exit 0
            ;;
        clean)
            rm -rf bin
            exit 0
            ;;
        *)
            echo "Invalid option: $1. Try 'help' for usage information."
            exit 1
            ;;
    esac
done