
let dropdown;
function preload() {
    const dropdown = select("#demo-select");
    const wasm_file = "/web/" + document.location.search.slice(1);
    if (wasm_file !== "/web/") {
        loadWASM(wasm_file)
        dropdown.value(wasm_file);
    }
    ///////////////////////////////
    dropdown.changed(() => {
        document.location = "./?" + dropdown.value();
    });
}
