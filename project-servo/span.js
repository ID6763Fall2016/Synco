var express = require('express');
var app = express();
var server = require('http').createServer(app);
var fs = require('fs');
var WebSocketServer = require('websocket').server;
var Gpio = require('pigpio').Gpio;
var motor = new Gpio(4, {mode: Gpio.OUTPUT});

var clients = [ ];

//Getting request from browser
//and send response back
app.get ('/', function(req, res){    

      fs.readFile('ws.html', 'utf8', function(err, text){
            res.send(text);
        });
});

//Listening to Port 1337 for incoming messages
server.listen(1337, function (){
    console.log((new Date()) + " Server is listening on port 1337... ");
});

websock = new WebSocketServer({
    httpServer: server
});
/*
var servoDirection = 0;
var servoInterval = null;

function stopServo() {
    if (servoInterval) {
        clearInverval(servoInterval);
    }

    servoInterval = null;
}

function startServo() {
    if (servoInterval) {
        clearInverval(servoInterval);
    }

    servoInterval = setInterval(functon() {
        if (servoDirection > 600) {
            servoDirection = 600;
        } else {
            servoDirection = 2400;
        }
    },1000);
}
*/


var pulseWidth = 1000,
    increment = 100;

setInterval(function () {
  motor.servoWrite(pulseWidth);

  pulseWidth += increment;
  if (pulseWidth >= 2400) {
    increment = -100;
  } else if (pulseWidth <= 600) {
    increment = 100;
  }
}, 500);

websock.on('request', function(request) {
    
    console.log((new Date()) + ' Connection from origin ' + request.origin + '.');

    var connection = request.accept(null, request.origin);
    
    console.log((new Date()) + ' Connection accepted.');

    //Incoming Data handling
    connection.on('message', function(message) {

        console.log('Data: ' +  message.utf8Data);
   
        var data = message.utf8Data;
        data = data.slice(1,3);


        // If incoming data is > 2 send a signal to pin 17
        // Set GPIO pin 17 to a PWM of 24%
        // Truns the Servo to it's right
        if (Number(data)>2){        
            motor.servoWrite(2400);
        }

        // If incoming data is > 2 send a signal to pin 17
        // Set GPIO pin 17 to a PWM of 6%
        // Truns the Servo to it's left
        if (Number(data)<(-2)){
            //motor.servoWrite(600);
        }

        // If incoming data is > 2 send a signal to pin 17
        // Set GPIO pin 17 to a PWM of 15%
        // Truns the Servo to it's center position
        if (Number(data)==0){        
            motor.servoWrite(1500);
        }
    });
    
    connection.on('close', function (connection){
        //close connection
        motor.servoWrite(0);
    });

    function closePin(){
        motor.servoWrite(0);
    }

});