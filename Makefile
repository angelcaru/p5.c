all: web/dvd.wasm web/gol.wasm web/graphics.wasm web/fractree.wasm

web/%.wasm: web/examples/%.c p5.h
	clang -O3 -target wasm32 \
		$< \
		-o $@ \
		-I . \
		-nostdlib -Wl,--allow-undefined -Wl,--no-entry \
		-Wl,--export=setup -Wl,--export=draw -Wl,--export=preload -Wl,--export=__heap_base \
		-Wl,--import-table \
