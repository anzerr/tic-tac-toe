
var obj = function(game) {
	this.game = game;
};
obj.prototype = {
	_type: {
		t: function(self, key, data) {
			self.game._turn = data[1] || 0;
			console.log('turn', self.game._turn);
			self.game.sendAll(JSON.stringify({
				action: 'turn',
				data: self.game._turn
			}));
		},
		m: function(self, key, data) {
			var size = data[1], mat = [];
			for (var x = 0; x < size; x++) {
				mat[x] = [];
				for (var y = 0; y < size; y++) {
					mat[x][y] = data[2 + (x * size) + y];
				}
			}
			self.game._map = {
				size: data[1],
				matrix: mat
			};

			console.log('map', self.game._map);
			self.game.sendAll(JSON.stringify({
				action: 'map',
				data: self.game._map
			}));
		},
		w: function(self, key, data) {
			self.game.sendAll(JSON.stringify({
				action: 'won',
				data: data[1]
			}));
		}
	},

	run: function(key, type, data) {
		if (this._type[type]) {
			console.log(type, data);
			this._type[type](this, key, data);
		}
	}
};

module.exports = obj;
