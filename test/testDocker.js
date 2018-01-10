const { spawn } = require('child_process');

var running = {}, exit = {};

var clean = function(data) {
	var d = data.split('');
	var key = ['\u001b', '\u0000'];
	for (var i in d) {
		for (var x in key) {
			if (d[i].match(key[x])) {
				d[i] = '';
			}
		}
	}
	return d.join('');
}

var bin = function(id, bin, arg) {
	const process = spawn(bin, arg);
	running[id] = true;
	process.stdout.on('data', (data) => {
		console.log('stdout:', clean(data.toString()));
	});

	process.stderr.on('data', (data) => {
		console.log('stderr:', id, data);
	});

	process.on('close', (code) => {
		running[id] = false;
		exit[id] = code;
		console.log('closed with code:', code, id);
	});
	return process;
}, random = function(i) {
	return Math.floor(Math.random() * i);
}

var client = [
	bin('1', '../build/client.exe', ['--port', 980, '--ip', '192.168.99.100']),
	bin('2', '../build/client.exe', ['--port', 980, '--ip', '192.168.99.100'])
]

var s = 0;
var run = function() {
	if (!running['1'] && !running['2']) {
		console.log('----- end', running, exit);
		return;
	}
	var side = (s) ? '2' : '1';

	if (running[side]) {
		var msg = 't,' + random(3) + ',' + random(3) + ';\n';
		console.log('msg ', side, s, ' send', msg);
		client[s].stdin.write(msg, function(err, res) {
			s = (s) ? 0 : 1;
			setTimeout(function() {
				run();
			}, 200);
		});

	}
}
setTimeout(function() {
	run();
}, 500);
