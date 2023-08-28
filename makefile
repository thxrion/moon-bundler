.PHONY: build clean test
.SILENT:

build:
	bash scripts/build.sh

clean:
	bash scripts/clean.sh

test:
	bash scripts/test.sh