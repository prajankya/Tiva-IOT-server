const express = require('express')
const path = require('path')
const unique = require('array-unique')


function server(ex_app) {
    ex_app.get("/add/:ip", function(req, res, next) {
        console.log("Connected : " + req.params.ip)

        global.ips.push(req.params.ip)
        global.ips = unique(global.ips)
        res.end("done")

        global.win.webContents.send('ips', global.ips)

    });
    setTimeout(function() {

        global.win.webContents.send('ips', global.ips)
    }, 1000)

    ex_app.use(express.static(path.join(__dirname, 'app'), {
        maxAge: 3600
    }));

    process.on('uncaughtException', function(err) {
        console.log(require('util').inspect(err, {
            depth: null
        }));
    });

    // catch 404 and forward to error handler
    ex_app.use(function(req, res, next) {
        res.status(404).end('404 Page Not Found');
    });
}

module.exports = server
