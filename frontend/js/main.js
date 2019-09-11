console.log("Script running");
var ws = new WebSocket("ws://" + location.host + ":80/chat");
var editor = null;
var modified = false;
var cursor = null;

function tick() {
	if(modified) {
		modified = false;
		return;
	}
	var data = {
		clear: true
	};
	console.log("Requesting data from backend");
	ws.send(JSON.stringify(data) );
}

function type(delta) {
	cursor = editor.selection.getCursor();
	if(!modified) {
		return;
	}
	var data = {
		clear: false,
		str: editor.getValue()
	};
	console.log("Sending message: ", data);
	ws.send(JSON.stringify(data) );
}

function setupListeners() {
	ws.onopen = (event) => {
		console.log("Socket is open");
	};
	ws.onclose = (event) => {
		console.log("🦀 Socket is gone! 🦀");
	};
	ws.onmessage = (event) => {
		if(event.data == "null") return;
		var message = JSON.parse(event.data);
		if(message.str == editor.getValue() ) return;
		modified = false;
		editor.setValue(message.str);
		editor.clearSelection();
		editor.gotoLine(cursor);
	};
	setInterval(tick, 1000);
	editor = ace.edit("editor");
	editor.setTheme("ace/theme/monokai");
    editor.session.setMode("ace/mode/c_cpp");
	editor.addEventListener("change", type);
	cursor = editor.selection.getCursor();

	//lmao
	//https://github.com/ajaxorg/ace/issues/211#issuecomment-2733468
	document.getElementById("editor").firstChild.addEventListener("keydown", () => {
		modified = true;
	});
};

document.addEventListener("DOMContentLoaded", (event) => {
	setupListeners();
	console.log("Listeners are up");
});
