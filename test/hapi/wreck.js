const Wreck = require('@hapi/wreck');
const Lab = require('@hapi/lab');
const Code = require('@hapi/code');
require('dotenv').config();

const { expect } = Code;
const lab = exports.lab = Lab.script();
const host = process.env.ADDY_HOST;
const port = process.env.PORT;
const url = `http://${host}:${port}`;

lab.experiment('server can receive and respond to HTTP requests', { timeout: 5000 }, () => {
    lab.test('send a single GET request', async () => {
		const { res, payload } = await Wreck.get(url);
		expect(res.statusCode).to.equal(200);
		expect(payload.toString()).to.equal('hello world\r');
    });

	lab.test('send 100 GET requests', async () => {
		for (let i = 0; i < 100; ++i) {
			const { res, payload } = await Wreck.get(url);
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
