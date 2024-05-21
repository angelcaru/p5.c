all: build porth_finish

DOLLAR=$
PORTH_EXAMPLES="net"
porth_finish: build
	printf 'for example in $(PORTH_EXAMPLES);\ndo\ncp web/examples/$(DOLLAR)example.wasm web\ndone' | bash

build: web/dvd.wasm web/gol.wasm web/graphics.wasm \
	web/fractree.wasm web/rust.wasm web/3d.wasm \
	web/pixels.wasm web/rbutton.wasm web/voronoi.wasm \
	web/reveal.wasm web/net.wasm

web/%.wasm: web/examples/%.c p5.h
	clang -O3 -target wasm32 \
		$< \
		-o $@ \
		-I . \
		-nostdlib -Wl,--allow-undefined -Wl,--no-entry \
		-Wl,--export=setup -Wl,--export=draw -Wl,--export=preload -Wl,--export=__heap_base \
		-Wl,--import-table 

web/%.wasm: web/examples/%.rs p5.rs
	rustc --crate-type bin --target=wasm32-unknown-unknown \
		-Clink-args="--allow-undefined --no-entry --export=__heap_base --import-table" \
	    -o $@ $<

# Make sure to change this to wherever you have your porth and 4orth stdlibs
PORTH_INCLUDE = "/usr/include/porth"
web/%.wasm: web/examples/%.porth p5.porth
	4orth -I $(PORTH_INCLUDE) -I . com -wasm $<
