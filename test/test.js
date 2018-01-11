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
}, randomChar = function(l) {
  var text = "";
  var possible = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789";

  for (var i = 0; i < l; i++)
    text += possible.charAt(Math.floor(Math.random() * possible.length));

  return text;
};

var port = 800 + random(3000), win = (process.platform == 'win32');

var server = bin('0', '../build/server' + (win ? '.exe' : ''), ['--port', port, '-v']);

setTimeout(function() {
	var client = [
		bin('1', '../build/client' + (win ? '.exe' : ''), ['--port', port, '--ip', '127.0.0.1']),
		bin('2', '../build/client' + (win ? '.exe' : ''), ['--port', port, '--ip', '127.0.0.1'])
	]

	var s = 0;
	var run = function() {
		if (!running['0'] && !running['1'] && !running['2']) {
			console.log('----- end', running, exit);
			return;
		}
		var side = (s) ? '2' : '1';

		if (running[side]) {
			var type = random(5), msg = 't';
			if (type == 0) {
				msg = 't,' + random(3) + ',' + random(3) + ';\n'; // valid 
			}
			if (type == 1) {
				msg = 't,' + randomChar(1) + ',' + randomChar(1) + ';\n';
			}
			if (type == 2) {
				msg = 't,' + randomChar(5) + ',' + randomChar(5) + ';\n';
			}
			if (type == 3) {
				msg = 't,' + random(1000) + ',' + random(1000) + ';\n';
			}
			if (type == 4) {
				msg = 't,' + (random(2000) - 1000) + ',' + (random(2000) - 1000) + ';\n';
			}

			console.log('msg ', side, s, ' send', msg);
			client[s].stdin.write(msg, function(err, res) {
				s = (s) ? 0 : 1;
				setTimeout(function() {
					run();
				}, 100);
			});

		}
	}
	setTimeout(function() {
		run();
	}, 500);
}, 1000);
