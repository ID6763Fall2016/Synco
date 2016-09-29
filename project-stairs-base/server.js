var Engine = require('tingodb')();
var database = new Engine.Db(__dirname + '/db',{});
var databaseName = 'Synco-Stair';
var databasePeople = 'Synco-Stair-People';
var databaseSensor = 'SyncoStairs_';

var sideThreshold = 200;

function mapSyncoDates(results) {
  var syncoDatesArray = {};
  return Array.prototype.reduce.call(results, function(collection, item) {
    var datetime =  new Date(item.datetime.getFullYear(), item.datetime.getMonth(), item.datetime.getDate());
    if (!syncoDatesArray[datetime]) {
      syncoDatesArray[datetime] = datetime;
      collection.push(datetime);
    }
    return collection;
  }, []);
}

function getSyncoDates(callback){
  var sampleCollection = database.collection(databaseName);

  sampleCollection
  .find({"_id": { $gte: 0 }}, {"_id":0, "datetime": true})
  .sort({"datetime":-1})
  .toArray(function(err, docList){
    callback(mapSyncoDates(docList));
  });
};


function getDBQueriesByDateQuery(collection, datetime, callback){
  var nextDate = new Date(datetime.getTime() + 24*60*60*1000);

  collection
  .find({"datetime": {$gte: datetime, $lt: nextDate}}, {"_id": 0})
  .sort({"datetime":1})
  .toArray(function(err, docList){
    callback(docList);
  });
};

function getDBQueriesByDate(collection, datetime) {
  getDBQueriesByDateQuery(collection, datetime, function(results) {
    // Send synco
    console.log(results);
  });
}

function getPeopleByDateByHour(datetime, callback) {
  var sampleCollection = database.collection(databasePeople);

  getDBQueriesByDateQuery(sampleCollection, datetime, function(results) {
    console.log('getPeopleByDateByHour', datetime);

    var resultsByHour = Array.prototype.reduce.call(results, function(collection, item) {
      var hour = item.datetime.getHours();
      var directionItem = collection[item.direction];
      var side = item.distance < sideThreshold;
      if (directionItem[hour]) {
        directionItem[hour].count++;
      } else {
        directionItem[hour] = {
          count: 1,
          left: 0,
          right: 0
        };
      }
      if (side) {
        directionItem[hour].right++
      } else {
        directionItem[hour].left++
      }
      return collection;
    }, {
      'up': {},
      'down': {}
    });

    // Send synco people
    callback(datetime, resultsByHour);
  });
}

function getSensorByDateByHour(position, datetime, callback) {
  var sampleCollection = database.collection(databaseSensor + position);

  getDBQueriesByDateQuery(sampleCollection, datetime, function(results) {
    console.log('getSensorByDateByHour', position, datetime);
    var resultsByHour = Array.prototype.reduce.call(results, function(collection, item) {
      var hour = item.datetime.getHours();
      var side = item.distance < sideThreshold;
      if (collection[hour]) {
        collection[hour].count++;
      } else {
        collection[hour] = {
          count: 1,
          left: 0,
          right: 0
        };
      }
      if (side) {
        collection[hour].right++
      } else {
        collection[hour].left++
      }
      return collection;
    }, {});

    // Send synco sensor by position
    callback(position, datetime, resultsByHour);
  });
}


//getSensorByDateByHour('bottom', results[0]);
//getSensorByDateByHour('top', results[0]);

//getPeopleByDateByHour(results[0]);

var app = require('http').createServer(handler);
var io = require('socket.io')(app);
var fs = require('fs');
var ip = require("ip");

var db = new Engine.Db(__dirname + '/db',{});

function handler (req, res) {
  fs.readFile(__dirname + '/public/index.html',
    function (err, data) {
      res.writeHead(200);
      res.end(data);
    });
  console.log("user connected");
}

console.log("Listening post 8000");
console.log ( ip.address() + ":8000");
app.listen(8000);

io.on('connection', function (socket) {
  console.log("user connected to socket");

  function sendSensorData(position, datetime, results) {
    socket.emit('syncoSensorData', {
      "position": position,
      "datetime": datetime,
      "results": results
    });
  }

  function sendPeopleData(datetime, results) {
    socket.emit('syncoPeopleData', {
      "datetime": datetime,
      "results": results
    });
  }
  
  socket.on('requestSyncoByDate', function(data){
    var datetime = new Date(data);
    getSensorByDateByHour('bottom', datetime, sendSensorData);
    getSensorByDateByHour('top', datetime, sendSensorData);
    getPeopleByDateByHour(datetime, sendPeopleData);
  });

  console.log('getSyncoDates');

  getSyncoDates(function(results) {
    console.log(results);
    socket.emit('syncoDates', results);
  });
  
  socket.on('disconnect', function(){
    console.log("user disconnected from socket");
  });
});