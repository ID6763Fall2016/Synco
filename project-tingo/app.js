
var Engine = require('tingodb')();

var database = new Engine.Db(__dirname + '/db',{});
var databaseName = 'tingostuff';

setInterval(function() {
	var sampleCollection = database.collection(databaseName);
	sampleCollection.insert({
		"sensorvalue" : Math.random() * 100,
		"datetime" : new Date()
	});
	console.log("added a sample");
}, 1000);

var getLatestSamples = function(theCount, callback) {
	var sampleCollection = database.collection(databaseName);
	sampleCollection
		.find()
		.sort({"datetime":-1})
		.limit(theCount)
		.toArray(function(err,docList){
			callback(docList);
		});
};

setInterval(function(){
	getLatestSamples(5, function(results) {
		var theValues = []
		for (var i = 0; i < results.length; i++)
		{
			theValues.push(results[i].sensorvalue);
		}
		console.log(theValues);
	});
}, 3000);
