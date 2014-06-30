var user_id='97bfaa168842';
var board_id='test_board_1';

var map_w = 10;
var map_h = 10;

var request = require('request');
var async = require('async');

function GetURL(url, final_callback) {
    request(url, function (error, response, body) {	
	final_callback(JSON.parse(body));
    });
}

// Returns array of boards.
function GetBoards(callback) {
    var url = "https://student.people.co/api/challenge/battleship/" + user_id + "/boards";
    GetURL(url, function(json) {
	callback(json);
    });
}

// Returns board information
function GetBoard(board_id, callback) {
    var url = "https://student.people.co/api/challenge/battleship/" + user_id + "/boards/" + board_id;
    GetURL(url, function(json) {
	callback(json);
    });
}

function Shoot(position, board_id, callback) {
    var url = "https://student.people.co/api/challenge/battleship/" + user_id + "/boards/" + board_id + "/" + position;
    GetURL(url, function(json) {
	callback(json);
    });
}

function DidHit(json) {
    return json.is_hit;
}

function IsFinished(json) {
    return json.is_finished;
}

function DidSink(json) {
    return json.sunk;
}

function Reset(board_id) {
    var url = "https://student.people.co/api/challenge/battleship/" + user_id + "/boards/" + board_id;
    request.del(url);
}

function PointToPosition(x, y) {
    var first = String.fromCharCode(65 + x);
    var y1 = y + 1;
    var second = y1.toString();

    return first + second;
}

var total_moves = 0;
function EasyStrat(map, board_id) {
    for (var x = 0; x < map_w; x++) {
	for (var y = 0; y < map_h; y++) {
	    if (map[x][y] == 0) {
		Shoot(PointToPosition(x, y), board_id, function(response) {
		    total_moves++;
		    if (DidHit(response)) {
			console.log('Hit ship at: (' + x + ', ' + y + ')');
			map[x][y] = 1;
		    } else {
			map[x][y] = -1;
		    }
		    
		    if (DidSink(response)) {
			console.log('Sunk ship at: (' + x + ', ' + y + ')');
		    }
		    
		    if (IsFinished(response)) {
			console.log('FINISHED! (' + total_moves + ')');
			process.exit(0);
		    }

		    EasyStrat(map, board_id);
		});
		return;
	    }
	}
    }
}

var map = new Array(map_w);
for (var i = 0; i < map_w; i++) {
    map[i] = new Array(map_h);
}

for (var x = 0; x < map_w; x++) {
    for (var y = 0; y < map_h; y++) {
	map[x][y] = 0;
    }
}

Reset(board_id);
EasyStrat(map, board_id);
