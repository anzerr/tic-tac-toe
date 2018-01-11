
var action = require('./action.js');

var obj = function(core) {
	this.core = core;
	this._interface = new action(this);
	this._player = {};
	this._map = {};
	this.ping();
};
obj.prototype = {
	ping: function() {
		if (this.core._client[0]) {
			this.core._client[0].write('0', function(e, r) {
				//console.log('wat', e, r);
			});
		}
		var self = this;
		setTimeout(function() {
			self.ping();
		}, 1000 / 16);
	},
	action: function(key, data) {
		data = data.split(';');
		for (var i in data) {
			//console.log(i, data[i]);
			this._interface.run(key, data[i][0], data[i].split(','));
		}
	},
	message: function(key, data) {
		if (data == 'map') {
			this.send(JSON.stringify({
				action: 'map',
				data: this._map
			}), key);
			return;
		}
		if (data == 'turn') {
			this.send(JSON.stringify({
				action: 'turn',
				data: this._turn
			}), key);
			return;
		}

		if (this.core._client[key]) {
			this.core._client[key].write(data, function(e, r) {
				//console.log('wat', e, r);
			});
		}
	},

	sendAll: function(data, key) {
		for (var i in this.core._user) {
			if (key != i || key == undefined) {
				this.send(data, i);
			}
		}
	},
	sendOther: function(data, key) {
		this.sendAll(data, key)
	},
	send: function(data, key) {
		if (this.core._user[key]) {
			//console.log('send socket c', key, data);
			this.core._user[key].send(data);
		}
	}
};

module.exports = obj;
