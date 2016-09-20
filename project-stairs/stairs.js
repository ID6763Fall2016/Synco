var GrovePi = require('node-grovepi').GrovePi;
var GPIO = require('onoff').Gpio;
var Engine = require('tingodb')();

var database = new Engine.Db(__dirname + '/db',{});
var databaseName = 'Synco-Alive';

var Commands = GrovePi.commands;
var Board = GrovePi.board;
var UltrasonicDigitalSensor = GrovePi.sensors.UltrasonicDigital;

var board;

function onSensor(data) {
  var syncoAliveCollection = database.collection(databaseName);
  syncoAliveCollection.insert(data);
}

function SyncoUltrasonicSensor(group_name, digital_port) {
  var sensor = new UltrasonicDigitalSensor(digital_port);
  var name = group_name + '-Ultrasonic';

  return {
    name: name,
    read: function() { return sensor.read(); },
    sensor: sensor
  };
}

function SyncoMotionSensor(group_name, digital_port) {
  var sensor = new GPIO(digital_port, 'in', 'both');
  var name = group_name + '-Motion';

  return {
    name: name,
    sensor: sensor
  };
}

function SyncoSensor(group_name, motion_port, ultrasonic_port) {
  var motion = SyncoMotionSensor(group_name, motion_port);
  var distance = SyncoUltrasonicSensor(group_name, ultrasonic_port);

  // pass the callback function to the
  // as the first argument to watch() and define
  // it all in one step
  motion.sensor.watch(function(err, state) {
    var collection = database.collection(group_name);
    var now = new Date();
    var data;
    var distance;
    if (state) {
      value = state;
      console.log(group_name);
      distance = distance.read();
      if (distance === false) {
        distance = 0;
      }

      data = {
        "datetime" : now,
        "distance" : distance
      };
      collection.insert(data);
      onSensor(data);
    }
  });
}

var sensors = [];

function start() {
  console.log('starting Synco Alive', new Date());

  board = new Board({
    debug: true,
    onError: function(err) {
      console.log('BOARD ERROR');
      console.log(err);
    },
    onInit: function(res) {
      if (res) {
        SyncoSensor('SyncoStairs-Top', 18, 2);
        SyncoSensor('SyncoStairs-Bottom', 4, 8);

        console.log('Synco Alive is ready', new Date());
      } else {
        console.log('SYNCO ALIVE CANNOT START');
      }
    }
  });

  board.init();
}

function onExit(err) {
  console.log('ending');
  board.close();
  process.removeAllListeners();
  process.exit();
  if (typeof err != 'undefined') {
    console.log(err);
  }
}

// starts the test
start();
// catches ctrl+c event
process.on('SIGINT', onExit);


