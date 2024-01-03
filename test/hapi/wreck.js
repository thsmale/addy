const Wreck = require('@hapi/wreck');
const Lab = require('@hapi/lab');
const Code = require('@hapi/code');
const { spawn } = require('node:child_process')
const { chdir, kill } = require('node:process');

const { expect } = Code;
const lab = exports.lab = Lab.script();

lab.experiment('server can receive and respond to HTTP requests', () => {
	let pid; 
	let output;
	let error;
	let server;
	lab.before(async () => {
		// run the server
		// in the future you should write a test hapi server here as well
		/*
		chdir('../../');
		return new Promise((resolve, reject) => {
			server = spawn('./server', [], { detached: true });
			server.unref();
			server.stdout.on('data', function(data) {
				console.log(data.toString()); 
				return resolve('server started');
			});
			server.stderr.on('data', (data) => {
				console.error(`stderr: ${data}`);
				return reject('server failed to start');
			});
		});
		*/
		
		/*
		if (error) {
			console.error(error);
			throw Error('Error starting server')
		}
		console.log(output.toString());
		*/
	})

	lab.after(() => {
		// kill the process
		/*
		server.kill('SIGINT');
		server.on('close', (code) => {
			if (code !== 0) {
			  console.log(`grep process exited with code ${code}`);
			}
		}); 
		*/
	})

    lab.test('send a single GET request', async () => {
		const { res, payload } = await Wreck.get('http://localhost:3000');
		expect(res.statusCode).to.equal(200);
		expect(payload.toString()).to.equal('hello world\r');
    });

	lab.test('send 100 GET requests', async () => {
		for (let i = 0; i < 100; ++i) {
			const { res, payload } = await Wreck.get('http://localhost:3000');
			expect(res.statusCode).to.equal(200);
			expect(payload.toString()).to.equal('hello world\r');
		}
	})

	/*
	 * TODO: accept payload from server
	lab.test('send a single GET request with payload', async () => {
		const { res, payload } = await Wreck.get('http://localhost:8000', { payload: 'yo dude' });
		expect(res.statusCode).to.equal(200);
		expect(payload.toString()).to.equal('hello world');
	})
	*/
});
