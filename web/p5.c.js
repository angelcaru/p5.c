
function strFromCstr(buf, cstr_ptr) {
    let result = "";
    const arr = new Uint8Array(buf, cstr_ptr);
    let i = 0;
    while (true) {
        let ch = arr[i++];
        if (ch === 0) break;
        result += String.fromCharCode(ch);
    }
    return result;
}

const CTYPES = {
    "bool": { size: 1 },
    "u8": { size: 1 },
    "int": { size: 4 },
};

const STRUCTS = {
    "p5_Image": [
        ["loaded", "bool"],
        ["id", "u8"],
    ],
    "p5_Element": [
        ["id", "u8"],
    ],
    "p5_Graphics": [
        ["width", "int"],
        ["height", "int"],
        ["id", "u8"], // ID has to be at the end because of byte alignment
    ],
};

function readInt(buf, ptr, size) {
    let result = 0;
    const arr = new Uint8Array(buf, ptr);

    for (let i = 0; i < size; i++) {
        // NOTE: JS only supports bitwise operations up to 32-bit numbers
        // Since WASM is 32-bit, this is fine.
        result = (result << 8) | arr[i];
    }
    return result;
}

function writeInt(buf, ptr, value, size) {
    let arr;
    switch (size) {
        case 1:
            arr = new Uint8Array(buf, ptr);
            break;
        case 4:
            arr = new Uint32Array(buf, ptr);
            break;
        default:
            throw new Error(`Unsupported integer size: ${size}`);
    }
    arr[0] = value;
}

function readStruct(buf, ptr, structDef) {
    if (structDef.length === 1) {
        const [name, typeName] = structDef[0];
        return { [name]: ptr };
    }
    return readStructPtr(buf, ptr, structDef);
}

function readStructPtr(buf, ptr, structDef) {
    const result = {};
    for (const [name, typeName] of structDef) {
        const { size } = CTYPES[typeName];
        result[name] = readInt(buf, ptr, size);
        ptr += size;
    }

    return result;
}

function writeStruct(buf, ptr, obj, structDef) {
    const arr = new Uint8Array(buf, ptr);
    for (const [name, typeName] of structDef) {
        const { size } = CTYPES[typeName];
        writeInt(buf, ptr, obj[name], size);
        ptr += size;
    }
}

function structSize(structDef) {
    let result = 0;
    for (const [name, typeName] of structDef) {
        const { size } = CTYPES[typeName];
        result += size;
    }
    return result;
}

let heap_base = null;
function alloc(wasm, size) {
    const exports = wasm.instance.exports;
    if (heap_base === null) heap_base = exports.__heap_base.value;
    const tmp = heap_base;
    heap_base += size;
    return tmp;
}

function allocStruct(wasm, obj, structDef) {
    if (structDef.length === 1) {
        const [name, typeName] = structDef[0];
        return obj[name];
    }

    const exports = wasm.instance.exports;
    const buf = exports.memory.buffer;
    const ptr = alloc(wasm, structSize(structDef));
    writeStruct(buf, ptr, obj, structDef);
    return ptr;
}

function createEnv(env) {
    let proxy = new Proxy(env, {
        get(env, prop) {
            return prop in env ? env[prop] : (...args) => {
                console.error(`NOT IMPLEMENTED: ${prop}(${args.join(", ")})`);
                noLoop();
            };
        }
    });
    return proxy;
}

function createTable(name) {
    const table = {
        allocateId() {
            // Specifically allocating 0 as an invalid id (makes it less error-prone)
            let i = 1;
            while (i in table) i++;
            return i;
        },
        insert(obj) {
            const id = table.allocateId();
            table[id] = obj;
            return id;
        },
        get(id) {
            if (!(id in table)) {
                throw new Error(`Bad ${name} id: ${id}`);
            } else {
                return table[id];
            }
        },
    };
    return table;
}

const NULL = 0;

let wasm = null;
let setupRan = false;
let preloadRan = false;

let exports = null;

function preload() {
    const imageTable = createTable("image");
    const domTable = createTable("element");
    const grTable = createTable("graphics");

    const grStack = [];

    function grCurr() {
        if (grStack.length === 0) return window;
        return grStack.at(-1);
    }

    const env = createEnv({
        heapReset() {
            heap_base = null;
        },

        mouseX() {
            return mouseX;
        },
        mouseY() {
            return mouseY;
        },

        setFrameRate(fps) {
            frameRate(fps);
        },
        random(min, max) {
            return random(min, max);
        },

        background(r, g, b) {
            grCurr().background(r, g, b);
        },
        fill(r, g, b) {
            grCurr().fill(r, g, b);
        },
        fillA(r, g, b, a) {
            grCurr().fill(r, g, b, a);
        },
        circle(x, y, d) {
            grCurr().circle(x, y, d);
        },
        ellipse(x, y, w, h) {
            grCurr().ellipse(x, y, w, h);
        },
        point(x, y) {
            grCurr().point(x, y);
        },
        rect(x, y, w, h) {
            grCurr().rect(x, y, w, h);
        },
        stroke(r, g, b) {
            grCurr().stroke(r, g, b);
        },
        strokeWeight(weight) {
            grCurr().strokeWeight(weight);
        },
        noStroke() {
            grCurr().noStroke();
        },
        noFill() {
            grCurr().noFill();
        },

        loadImage(url_ptr, img_ptr) {
            const buf = exports.memory.buffer;
            const url = strFromCstr(buf, url_ptr);
            const imgStruct = readStruct(buf, img_ptr, STRUCTS["p5_Image"]);

            if (imgStruct.loaded) {
                console.error(
                    `Attempt to load image from url ${url} into already used p5_Image struct`);
                return;
            }

            loadImage(url, img => {
                const id = imageTable.insert(img);
                imgStruct.id = id;
                imgStruct.loaded = 1;
                writeStruct(buf, img_ptr, imgStruct, STRUCTS["p5_Image"]);
                // TODO: some way to tell that the image errored from C
            }, err => console.error(`Error loading image from ${url}:`, err));
        },

        image(img_ptr, x, y, w, h) {
            const buf = exports.memory.buffer;
            const { id, loaded } = readStructPtr(buf, img_ptr, STRUCTS["p5_Image"]);

            if (!loaded) {
                throw new Error("Attempt to render unloaded image");
            } else {
                grCurr().image(imageTable.get(id), x, y, w, h);
            }
        },

        print(val) {
            console.log(val);
        },
        print_str(msg_ptr) {
            console.log(strFromCstr(exports.memory.buffer, msg_ptr));
        },

        createCanvas(width, height) {
            const canvas = createCanvas(width, height);
            return allocStruct(wasm, { id: domTable.insert(canvas) }, STRUCTS["p5_Element"]);
        },
        createButton(label_ptr) {
            const label = strFromCstr(exports.memory.buffer, label_ptr);
            const btn = createButton(label);

            return allocStruct(wasm, { id: domTable.insert(btn) }, STRUCTS["p5_Element"]);
        },
        onMousePressed(elt_ptr, callback_idx, user_data) {
            const callback = env.__indirect_function_table.get(callback_idx);
            const { id } = readStructPtr(exports.memory.buffer, elt_ptr, STRUCTS["p5_Element"]);

            domTable.get(id).mousePressed(() => {
                callback(user_data);
            });
        },
        setHTML(elt_ptr, html_cstr) {
            const { id } = readStructPtr(exports.memory.buffer, elt_ptr, STRUCTS["p5_Element"]);
            const elt = domTable.get(id);
            const html = strFromCstr(exports.memory.buffer, html_cstr);
            elt.html(html);
        },

        createGraphics(ptr, width, height) {
            const gr = createGraphics(width, height);
            const id = grTable.insert(gr);

            writeStruct(exports.memory.buffer, ptr, {
                id,
                width,
                height,
            }, STRUCTS["p5_Graphics"]);
        },
        graphicsBegin(gr_ptr) {
            const { id } = readStruct(exports.memory.buffer, gr_ptr, STRUCTS["p5_Graphics"]);
            const gr = grTable.get(id);

            grStack.push(gr);
        },
        graphicsEnd() {
            if (grStack.length === 0) {
                throw new Error("Call to graphicsEnd() without matching graphicsBegin()");
            }
            grStack.pop();
        },
        render(gr_ptr, x, y, w, h) {
            const { id } = readStruct(exports.memory.buffer, gr_ptr, STRUCTS["p5_Graphics"]);
            const gr = grTable.get(id);
            grCurr().image(gr, x, y, w, h);
        },

        __indirect_function_table: new WebAssembly.Table({ initial: 4, element: "anyfunc" }),
    });
    const wasm_file = "/web/" + document.location.search.slice(1);
    WebAssembly.instantiateStreaming(fetch(wasm_file), {
        env,
    }).then(w => {
        wasm = w;
        exports = w.instance.exports
        preloadRan = !("preload" in exports);
    });
    console.log(env);
    ///////////////////////////////
}

function setup() {}

function draw() {
    if (wasm == null) return;

    if (!preloadRan) exports.preload();
    if (!setupRan) {
        exports.setup();
        setupRan = true;
    }
    exports.draw();
}
