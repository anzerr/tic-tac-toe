var client = function() {
    this._map = [];
    this._turn = 2;
};
client.prototype = {
    drawMap: function() {
        console.log('------------');
        var s = this._map.size;
        for (var x = 0; x < s; x++) {
            var str = '|';
            for (var y = 0; y < s; y++) {
                if (this._map.data[(x * s) + y] != '0') {
                    str += (this._map.data[(x * s) + y] == 1) ? 'x' : 'o';
                } else {
                    str += ' ';
                }
            }
            console.log(str + '|');
        }
        console.log('------------');
    },

    action: function(data) {
        data = data.split(';');
        for (var i in data) {
            if (data[i][0] == 'm') {
                var map = data[i].split(','), d = {
                    size: map[1],
                    data: map.slice(2, map.length)
                };
                this._map = d;
            }
            if (data[i][0] == 't') {
                var turn = data[i].split(','), d = turn[1];
                this._turn = d;
            }

            if (data[i][0] == 'w') {
                console.log(data[i]);
            }
        }
    }
};

var a = function(id) {
    var s = require('net').Socket();
    s.connect(8000, '127.0.0.1');
    var handle = new client(), stop = false;
    /*if (id == 0) {
        setTimeout(function() {
            s.end();
        }, 5000);
    }*/

    var pos = function() {
        return Math.floor(Math.random() * handle._map.size);
    };

    var run = function() {
        if (stop) {
            return;
        }
        if (handle._turn == id && handle._map.size) {
            var r = 't,' + pos() + ',' + pos();
            //console.log('send', r, id);
            s.write(r, function(e, r) {
                setTimeout(function() {
                    run();
                }, 100);
            });
        } else {
            setTimeout(function() {
                run();
            }, 100);
        }
    };
    run();

    s.on('data', function(d) {
        handle.action(d.toString());
        if (id == 0) {
            handle.drawMap();
        }
	    //console.log('--', d.toString());
    });

    s.on('connect', function() {
	    console.log('connected');
        /*s.write(id + '_is_id\n', function(e, r) {
            console.log('send msg', e, r);
        });*/
    });

    s.on('close', function() {
	    console.log('closed');
	    //setTimeout(a, 5000);
        stop = true;
    });

    s.on('error', function(e) {
	    console.log('e', e);
    });
};
for (var i = 0; i < 2; i++) {
    a(i);
}
