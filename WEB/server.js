var express = require ('express');
var app = express ();
var server = app.listen (8000);

app.use (express.static ('public'));
console.log ("Server started");

var socket = require ('socket.io');
var io = socket (server);

io.sockets.on ('connection', connect);

function connect (socket) {
    console.log ('new connection: ' + socket.id);
    socket.on ('line', (data) => {
        socket.broadcast.emit ('line', data);
    });
}
