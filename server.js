require('array.prototype.find')

function server(ex_app) {
    ex_app.get("/add/:ip", function(req, res, next) {
        console.log("Connected : " + req.params.ip)
        if (global.ips.find(function() {
                return req.params.ip;
            })) {
            res.end("duplicate")
        } else {
            global.ips.push(req.params.ip)
            res.end("done")
        }
    });
}
module.exports = server
