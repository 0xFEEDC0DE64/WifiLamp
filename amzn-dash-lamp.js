const dash_button = require('node-dash-button');
const http = require('http');
const url = require('url');

dash_button('ac:63:be:07:f0:31', null, null, 'all').on('detected', function() {
    console.log("omg found");
    http.request({
        host: '192.168.0.205',
        port: 80,
        path: '/toggle',
        method: 'GET'
    }).end();
});

console.log('listening...');
