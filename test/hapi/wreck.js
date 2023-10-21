const Wreck = require('@hapi/wreck');

const example = async function () {
	const { res, payload } = await Wreck.get('http://localhost:8000');
	console.log(payload.toString());
};

const multipleRequests = async () => {
	for (let i = 0; i < 1000; ++i) {
		const { res, payload } = await Wreck.get('http://localhost:8000');
		console.log(payload.toString());
	}
}

const getWithPayload = async function () {
	const { res, payload } = await Wreck.get('http://localhost:8000', { payload: 'yo dude' } );
	console.log(payload.toString());
};

try {
	example();
	//multipleRequests();
	//getWithPayload();
}
catch (ex) {
	console.error(ex);
}
