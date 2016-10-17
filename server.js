var express = require('express');
var app = express();
var server = require('http').createServer(app);
var request = require('request');
var ips = [];

app.use("/add/:ip", function(req, res, next) {
    console.log("Connected : " + req.params.ip);
    ips.push(req.params.ip);
    res.end("done");
});

app.use("/button/:no/:state", function(req, res, next) {
    console.log("button : " + req.params.no + " is " + req.params.state);
    for (var i = 0; i < ips.length; i++) {
        console.log(ips[i]);
        request('http://' + ips[i] + '/LED_' + req.params.no + '_' + req.params.state, function(error, response, body) {
            if (!error && response.statusCode == 200) {
                console.log(body) // Show the HTML for the Google homepage.
            }
        })

    }
    res.end("done");
});
app.set('port', process.env.PORT || 3000);

server.listen(app.get('port'), function() {
    console.log("Started on port :" + app.get('port'));
});
