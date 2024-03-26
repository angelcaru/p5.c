all: web/dvd.wasm web/gol.wasm web/graphics.wasm web/fractree.wasm web/rust.wasm

web/%.wasm: web/examples/%.c p5.h
	clang -O3 -target wasm32 \
		$< \
		-o $@ \
		-I . \
		-nostdlib -Wl,--allow-undefined -Wl,--no-entry \
		-Wl,--export=setup -Wl,--export=draw -Wl,--export=preload -Wl,--export=__heap_base \
		-Wl,--import-table 

# TODO: `rustc` for some reason doesn't import the table correctly even though it has
# the same linker as `clang`. We need to either:
#   A. figure it out
#   B. remove function pointers from the library
web/%.wasm: web/examples/%.rs p5.rs
	rustc --crate-type bin --target=wasm32-unknown-unknown \
		-Clink-args="--allow-undefined --no-entry --export=__heap_base" \
	    -o web/rust.wasm web/examples/rust.rs
