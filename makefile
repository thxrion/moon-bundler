bundler: src/main.c
	mkdir -p build
	gcc -o build/lua_bundler.exe src/main.c