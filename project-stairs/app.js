var server;
var sensors;

// Initialize server or stair sensors based on command line arguments

if (process.argv[2] !== 'sensors') {
	console.log('load server');
	server = require('./server.js');
}
if (process.argv[2] !== 'server') {
	console.log('load stairs');
	stairs = require('./stairs.js');
}

if (process.argv[2] !== 'sensors') {
	server.start(sensors);
}

if (process.argv[2] !== 'server') {
	sensors.start(server);
}