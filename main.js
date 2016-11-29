const {
    app,
    ipcMain,
    BrowserWindow
} = require('electron')

var express = require('express')
var ex_app = express()
var server = require('http').createServer(ex_app)
var request = require('request')

const fs = require('fs')
const path = require('path')

global.ips = []

app.on('ready', () => {
    // Create the browser window.
    global.win = new BrowserWindow({
        width: 980,
        height: 760
    })

    global.win.loadURL(`file://${__dirname}/app/index.html`)

    //global.win.webContents.openDevTools()

    global.win.on('closed', () => {
        global.win = null
    })
})

app.on('window-all-closed', () => {
    // On macOS it is common for applications and their menu bar
    // to stay active until the user quits explicitly with Cmd + Q
    if (process.platform !== 'darwin') {
        app.quit()
    }
})

app.on('activate', () => {
    // On macOS it's common to re-create a window in the app when the
    // dock icon is clicked and there are no other windows open.
    if (global.win === null) {
        createWindow()
    }
})

ipcMain.on('getIP', (event, arg) => {
    event.returnValue = global.serverIP
})

ipcMain.on('getCode', (event, arg) => {
    var s = fs.readFileSync(path.join(__dirname, 'TIVA-IOT-Arduino', 'TIVA-IOT-Arduino.ino'))
    event.returnValue = String(s)
})

require('./server')(ex_app)


ex_app.set('port', process.env.PORT || 3000);

server.listen(ex_app.get('port'), function() {
    console.log("Started server on port " + ex_app.get('port') + " on IP(s) :");

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
                console.log(iface.address);
                global.serverIP = iface.address;
            }
            ++alias;
        });
    });
});
