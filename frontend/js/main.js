console.log("Script running");
let Range = ace.require('ace/range').Range;
let url = "ws://" + location.host + ":80/chat";
var ws = new WebSocket(url);
var editor = null;
var modified = false;
var cursor = null;
var queue = [];

function tick() {
	if(modified) {
		modified = false;
		return;
	}

	if(queue.length > 0) {
		console.log("Sending queue: ", queue);
		var data = {
			queue: queue
		};
		ws.send(JSON.stringify(data) );
		queue = [];
	}
}

function type(delta) {
	if(!modified) {
		return;
	}
	console.log("Appending action to queue", delta);
	queue.push(delta);
}

function max(a, b) {
	return a > b ? a : b;
}

function applyChanges(changes) {
	editor.setReadOnly(true);
	var cursor = editor.selection.getCursor();
	console.log(changes);
	queue.reverse() //This part just works, idk why
	changes.forEach( (change) => {
		if(change.action == "insert") {
			if(change.lines.length == 1) {
				editor.gotoLine(change.start.row + 1, 
					change.start.column);
				editor.insert(change.lines[0]);
				if(change.start.row == cursor.row) {	//Om texten rör sig mot höger
					if(change.start.column < cursor.column) cursor.column++;
				}
			} else {	//Newline
				editor.gotoLine(change.start.row + 1, 
					change.start.column);
				editor.splitLine();
				if(cursor.row > change.start.row) {
					cursor.row++;
				}
			}
		} else if(change.action == "remove") {
			var range = new Range(change.start.row, change.start.column,
				change.end.row, change.end.column);
			editor.session.remove(range);
		} else if(change.action == "delete") {
			var range = new Range(change.start.row, change.start.column,
				change.end.row, change.end.column);
			editor.session.remove(range);
			if(change.start.row == cursor.row) {	//Om texten rör sig mot vänster
				if(change.start.column < cursor.column) cursor.column--;
			}
		}
		editor.session.getUndoManager().$undoStack.pop();
	});
	editor.gotoLine(cursor.row + 1, cursor.column);
	editor.setReadOnly(false);
}

function setupListeners() {
	ws.onopen = (event) => {
		console.log("Socket is open");
	};
	ws.onclose = (event) => {
		console.log("🦀 Socket is gone! 🦀");
	};
	ws.onmessage = (event) => {
		console.log("Message: ", event.data, " recieved");
		if(event.data == "null") return;

		var message = JSON.parse(event.data);
		console.log("Message recieved: ", message);
		applyChanges(message.queue);

		/*
		if(message.str == editor.getValue() ) return;
		modified = false;
		editor.setValue(message.str);
		editor.clearSelection();
		editor.gotoLine(cursor);
		*/
	};
	setInterval( () => {
		if(ws.readyState == ws.OPEN) {
			console.log("Pulse");
			ws.send("");
		}
	}, 20000);
	setInterval(tick, 1000);
	editor = ace.edit("editor");
	editor.setTheme("ace/theme/monokai");
    editor.session.setMode("ace/mode/c_cpp");
	editor.addEventListener("change", type);
	//cursor = editor.selection.getCursor();

	//lmao
	//https://github.com/ajaxorg/ace/issues/211#issuecomment-2733468
	document.getElementById("editor").firstChild.addEventListener("keydown", () => {
		var key = event.keyCode || event.charCode;
		if( key == 8 || key == 46 ) {
			var newCursor = editor.selection.getCursor();
			var change = {
				action: "delete",
				start: newCursor,
				end: cursor
			};

			queue.push(change);
		}
		modified = true;
		cursor = editor.selection.getCursor();
	});
};

document.addEventListener("DOMContentLoaded", (event) => {
	setupListeners();
	console.log("Listeners are up");
});
