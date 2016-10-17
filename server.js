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
    console.log("From IP : " + req.connection.remoteAddress);
    for (var i = 0; i < ips.length; i++) {
        console.log("Sending to IP : " + ips[i]);
        if (req.connection.remoteAddress != ips[i]) {
            request('http://' + ips[i] + '/LED_' + req.params.no + '_' + req.params.state, function(error, response, body) {
                //console.log(response);
            });
        }
    }
    res.end("done");
});
app.set('port', process.env.PORT || 3000);

server.listen(app.get('port'), function() {
    console.log("Started server on port " + app.get('port') + " on IP(s) :");

    var os = require('os');
    var ifaces = os.networkInterfaces();

    Object.keys(ifaces).forEach(function(ifname) {
        var alias = 0;

        ifaces[ifname].forEach(function(iface) {
            if ('IPv4' !== iface.family || iface.internal !== false) {
                // skip over internal (i.e. 127.0.0.1) and non-ipv4 addresses
                return;
            }

            if (alias >= 1) {
                // this single interface has multiple ipv4 addresses
                console.log(ifname + ':' + alias, iface.address);
            } else {
                // this interface has only one ipv4 adress
                console.log(ifname, iface.address);
            }
            ++alias;
        });
    });
});
