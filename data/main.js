function sendMessage(text) {
    fetch("/text_hu", {
        method: "POST",
        body: text
    });
}

function sendPayload(text) {
    fetch("/payload_hu", {
        method: "POST",
        body: text
    }).then(() => {
        fetch("/payload_hu_exec", {
            method: "POST",
            body: "true"
        });
    });
}

/* ---------------- HID MOD BITS ---------------- */
const MOD = {
    LCTRL: 0x01,
    LSHIFT: 0x02,
    LALT: 0x04,
    LGUI: 0x08,
    RCTRL: 0x10,
    RSHIFT: 0x20,
    RALT: 0x40, // AltGr
    RGUI: 0x80
};

let modState = 0;
const MAX_KEYS = 6;
let pressedKeys = [];

/* ---------------- SEND REPORT ---------------- */
function sendReport() {
    let reportKeys = pressedKeys.slice();
    while (reportKeys.length < MAX_KEYS) {
        reportKeys.push(0);
    }
    const keys = reportKeys
        .slice(0, MAX_KEYS)
        .map(k => k.toString(16).padStart(2, "0"))
        .join(",")

    const body =
        keys + "|" +
        modState.toString(16).padStart(2, "0");

    fetch("/report", {
        method: "POST",
        body
    });
}

function keyDown(code) {
    if (pressedKeys.includes(code)) return;
    if (pressedKeys.length < MAX_KEYS) {
        pressedKeys.push(code);
        sendReport();
    }
}

function keyUp(code) {
    const i = pressedKeys.indexOf(code);
    if (i !== -1) {
        pressedKeys.splice(i, 1);
        sendReport();
    }
}


/* ---------------- MOD TOGGLE ---------------- */
function mod(bit, btn) {
    modState ^= bit;
    btn.classList.toggle("active");
    sendReport();
    render();
}

/* ---------------- KEY DEFINITIONS ---------------- */
/* label, altLabel, hidCode, shiftLabel, altShiftLabel */
const rows = [
    [
        ["Esc", "", 0x29],
        ["F1", "", 0xC2], ["F2", "", 0xC3], ["F3", "", 0xC4], ["F4", "", 0xC5], ["F5", "", 0xC6], ["F6", "", 0xC7],
        ["F7", "", 0xC8], ["F8", "", 0xC9], ["F9", "", 0xCA], ["F10", "", 0xCB], ["F11", "", 0xCC], ["F12", "", 0xCD],
    ],
    [
        ["0", "", 0x35, "§", ""], ["1", "~", 0x1E, "'", ""], ["2", "ˇ", 0x1F, "\"", ""], ["3", "^", 0x20, "+", ""],
        ["4", "˘", 0x21, "!", ""], ["5", "°", 0x22, "%", ""], ["6", "˛", 0x23, "/", ""], ["7", "`", 0x24, "=", ""],
        ["8", "˙", 0x25, "(", ""], ["9", "´", 0x26, ")", ""],
        ["ö", "˝", 0x27, "Ö", ""], ["ü", "¨", 0x2D, "Ü", ""], ["ó", "¸", 0x2E, "Ó", ""],
        ["Back", "", 0x2A, "wide"]
    ],
    [
        ["Tab", "", 0x2B, "wide"],
        ["q", "\\", 0x14, "Q", ""], ["w", "|", 0x1A, "W", ""], ["e", "Ä", 0x08, "E", ""], ["r", "", 0x15, "R", ""],
        ["t", "", 0x17, "T", ""], ["z", "", 0x1C, "Z", ""], ["u", "€", 0x18, "U", ""],
        ["i", "Í", 0x0C, "I", ""], ["o", "", 0x12, "O", ""], ["p", "", 0x13, "P", ""],
        ["ő", "÷", 0x2F, "Ő", ""], ["ú", "×", 0x30, "Ú", ""], ["ű", "¤", 0x31, "Ű", ""]
    ],
    [
        ["Caps", "", 0x39, "xwide"],
        ["a", "ä", 0x04, "A", ""], ["s", "đ", 0x16, "S", ""], ["d", "Đ", 0x07, "D", ""], ["f", "[", 0x09, "F", ""],
        ["g", "]", 0x0A, "G", ""], ["h", "", 0x0B, "H", ""], ["j", "í", 0x0D, "J", ""],
        ["k", "ł", 0x0E, "K", ""], ["l", "Ł", 0x0F, "L", ""],
        ["é", "$", 0x31, "É", ""], ["á", "ß", 0x34, "Á", ""],
        ["Enter", "", 0x28, "xwide"]
    ],
    [
        ["Shift", "", MOD.LSHIFT, "mod", "wide"],
        ["í", "<", 0x64, "Í", ""],
        ["y", ">", 0x1D, "Y", ""], ["x", "#", 0x1B, "X", ""], ["c", "&", 0x06, "C", ""], ["v", "@", 0x19, "V", ""],
        ["b", "{", 0x05, "B", ""], ["n", "}", 0x11, "N", ""], ["m", "<", 0x10, "M", ""],
        [",", ";", 0x36, "?", ""], [".", ">", 0x37, ":", ""], ["-", "*", 0x38, "_", ""],
        ["Shift", "", MOD.RSHIFT, "mod", "xwide"]
    ],
    [
        ["Ctrl", "", MOD.LCTRL, "mod", "wide"],
        ["Win", "", MOD.LGUI, "mod"],
        ["Alt", "", MOD.LALT, "mod"],
        ["Space", "", 0x2C, "space"],
        ["AltGr", "", MOD.RALT, "mod"],
        ["Menu", "", 0x65],
        ["Ctrl", "", MOD.RCTRL, "mod", "wide"]
    ],
    [
        ["Ins", "", 0x49],
        ["Home", "", 0x4A],
        ["PgUp", "", 0x4B]
    ],
    [
        ["Del", "", 0x4C],
        ["End", "", 0x4D],
        ["PgDn", "", 0x4E]
    ],
    [
        ["", "", null],       // spacer
        ["↑", "", 0x52],
        ["", "", null]
    ],
    [
        ["←", "", 0x50],
        ["↓", "", 0x51],
        ["→", "", 0x4F]
    ]
];

/* ---------------- RENDER ---------------- */
function render() {
    const kb = document.getElementById("kb");
    kb.innerHTML = "";

    const shifted = modState & (MOD.LSHIFT | MOD.RSHIFT);
    const ralt = modState & MOD.RALT;

    rows.forEach(row => {
        const r = document.createElement("div");
        r.className = "row";

        row.forEach(k => {
            const b = document.createElement("button");
            b.className = "key";

            if (k.includes("wide")) b.classList.add("wide");
            if (k.includes("xwide")) b.classList.add("xwide");
            if (k.includes("space")) b.classList.add("space");

            if (k[3] === "mod") {
                b.classList.add("mod");
                if (modState & k[2]) b.classList.add("active");
                b.textContent = k[0];
                b.onclick = () => mod(k[2], b);
            } else {
                let shift = "";
                if (k[3] && k[3].length === 1) {
                    shift = k[3];
                }
                const main = k[0];
                const alt = k[1];
                let altshift = k[4];
                if (altshift && altshift.length !== 1) {
                    altshift = "";
                }

                b.innerHTML = shifted ? ((ralt && altshift) ? altshift : (shift ? shift : main)) : ((ralt && alt) ? alt : main);

                if (k[2] !== null) {
                    b.onmousedown = () => {
                        b.classList.add("pressed");
                        keyDown(k[2]);
                    };
                    b.onmouseup = () => {
                        b.classList.remove("pressed");
                        keyUp(k[2]);
                    };
                    b.onmouseleave = () => {
                        b.classList.remove("pressed");
                        keyUp(k[2]);
                    };
                    b.ontouchstart = e => {
                        e.preventDefault(); keyDown(k[2]);
                        b.classList.add("pressed");
                    };
                    b.ontouchend = e => {
                        e.preventDefault(); keyUp(k[2]);
                        b.classList.remove("pressed");
                    };
                } else {
                    b.style.visibility = "hidden";
                }
            }

            r.appendChild(b);
        });

        kb.appendChild(r);
    });
}

document.addEventListener("DOMContentLoaded", () => {
    render();
});
