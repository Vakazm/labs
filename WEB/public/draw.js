const socket = io.connect ('http://localhost:8000');
let r = randcol (), g = randcol (), b = randcol ();
console.log (r, g, b);

function reloadcanvas () {
    createCanvas (window.innerWidth, window.innerHeight);
    background (50);
}

function setup () {
    addEventListener ("resize", (event) => {});
    onresize = (event) => reloadcanvas ();
    reloadcanvas ();
    
    socket.on ('line', sync);
}

function randcol () {
    return Math.floor (Math.random () * 256);
}

function sync (data) {
    stroke (data.col.r, data.col.g, data.col.b);
    line (data.x, data.y, data.px, data.py);
}

function draw () {
    stroke (r, g, b);
    if (mouseIsPressed === true) {
        line (mouseX, mouseY, pmouseX, pmouseY);
        var data = { x: mouseX, y: mouseY, px: pmouseX, py: pmouseY, col: {r: r, g: g, b: b} };
        socket.emit ('line', data);
    }
}
