var GrovePi = require('node-grovepi').GrovePi;
var GPIO = require('onoff').Gpio;
var Engine = require('tingodb')();

var database = new Engine.Db(__dirname + '/db',{});
var databaseName = 'Synco-Alive';

var Commands = GrovePi.commands;
var Board = GrovePi.board;
var UltrasonicDigitalSensor = GrovePi.sensors.UltrasonicDigital;

var board;

var ultrasonicThreshold = 350;

function onSensor(datetime) {
  var syncoAliveCollection = database.collection(databaseName);
  syncoAliveCollection.insert({
    "datetime" : datetime
  });
}

function SyncoUltrasonicSensor(digital_port) {
  var sensor = new UltrasonicDigitalSensor(digital_port);
  var name = 'SyncoUltrasonicSensor-' + digital_port;
  var collection = database.collection(name);

  sensor.on('change', function(res) {
    var now = new Date();
    if (res !== false && res < ultrasonicThreshold) {
      collection.insert({
        "distance" : res,
        "datetime" : now
      });
      onSensor(now);
    }
  });
  sensor.watch();
}

function SyncoMotionSensor(digital_port) {
  var sensor = new GPIO(digital_port, 'in', 'both');
  var name = 'SyncoMotionSensor-' + digital_port;
  var collection = database.collection(name);

  // pass the callback function to the
  // as the first argument to watch() and define
  // it all in one step
  sensor.watch(function(err, state) {
    var now = new Date();
    if (state) {
      collection.insert({
        "datetime" : now
      });
      onSensor(now);
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
        SyncoUltrasonicSensor(7);
        SyncoUltrasonicSensor(8);
      } else {
        console.log('SYNCO ALIVE CANNOT START');
      }
    }
  })
  board.init();

  SyncoMotionSensor(18);
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

