var fs = require('fs'),
	http = require('http'),
	url = require('url'),
	socketio = require('socket.io');

var gameInterface = fs.createWriteStream("/tmp/interface");

var server = http.createServer(function(req, res) {

var request = url.parse(req.url, true);
var action = request.pathname;

   if (action == '/images/PresentButton.png') {
   	var img = fs.readFileSync('images/PresentButton.png');
   	res.writeHead(200, {'Content-Type': 'image/png' });
   	res.end(img, 'binary');
   } else if (action == '/images/background.jpg') {
   	var img = fs.readFileSync('images/background.jpg');
   	res.writeHead(200, {'Content-Type': 'image/jpg' });
   	res.end(img, 'binary');
   } else {
		res.writeHead(200, { 'Content-type': 'text/html'});
		res.end(fs.readFileSync(__dirname + '/index.html'));
	 }
}).listen(8080, function() {
	console.log('Listening at: http://localhost:8080');
});

socketio.listen(server).on('connection', function (socket) {
	socket.on('message', function (msg) {
		console.log('Message Received: ', msg);
		socket.broadcast.emit('message', msg);
		//fs.writeFile("/tmp/interface", "f\n");
		gameInterface.write("f");
		});
});
