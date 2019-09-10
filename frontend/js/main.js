console.log("Script running");
var ws = new WebSocket("ws://" + location.host + ":80/chat");

function tick() {
	var data = {
		clear: true
	};
	console.log("Requesting data from backend");
	ws.send(JSON.stringify(data) );
}

function type() {
	var data = {
		str: document.getElementById("input").value,
		clear: false
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
		var input = document.getElementById("input");
		if(event.data == null) return;
		input.value = JSON.parse(event.data).str;
	};
	setInterval(tick, 200);
	var input = document.getElementById("input");
	input.value = "";
	input.addEventListener("input", type);
};

document.addEventListener("DOMContentLoaded", (event) => {
	setupListeners();
	console.log("Listeners are up");
});
