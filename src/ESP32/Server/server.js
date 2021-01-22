var express = require('express');
var bodyParser = require('body-parser');

var postHandler = require('./postService');
var getHandler  = require('./getService');
var deleteHandler = require('./deleteService');

var app = express();

app.use(bodyParser.json());

app.get("/ping",getHandler.ping);

app.post("/ingest",postHandler.ingest);

app.delete("/remove",deleteHandler.remove);

app.listen(8000);
 
console.log("Server is up and running!!!!!!!!!");