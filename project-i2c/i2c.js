var i2c = require('i2c');
var address = 0x48;
var wire = new i2c(address, {device: '/dev/i2c-1'}); // point to your i2c address, debug provides REPL interface
var length = 2;

wire.scan(function(err, data) {
	// result contains an array of addresses
	console.log('Scan address');
	console.log(arguments);
});
 
 /*
setInterval(function(){
	wire.writeBytes(0x01, [0xC1,0x83], function(err,res) { //Change bytes based on your ADC
		console.log("error write:"+err)
		wire.readBytes(0x00,length, function(err, res) {
			console.log("error: "+err)
			console.log("res: "+res.readInt16LE())
		});
	});
},1000);
*/


wire.on('data', function(data) {
  	console.log('data:', data);
});