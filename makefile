bundler: src/main.c
	mkdir -p build
	gcc -o build/lua_bundler.exe src/main.c src/io.c src/io.h