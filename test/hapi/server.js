'use strict';

const Hapi = require('@hapi/hapi');
require('dotenv').config();

const init = async () => {

    const server = Hapi.server({
        port: process.env.PORT || 3000,
        host: process.env.ADDY_HOST || 'localhost'
    });

    server.route({
        method: 'GET',
        path: '/',
        handler: (request, h) => {
            return 'Hello World!';
        }
    });

    await server.start();
    console.log('Server running on %s', server.info.uri);
	
};

process.on('unhandledRejection', (err) => {

    console.log(err);
    process.exit(1);
});

init();
