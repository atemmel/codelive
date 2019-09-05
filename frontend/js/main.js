console.log("Script running");
var ws = new WebSocket("ws://" + location.host + ":80/chat");

function tick() {
	ws.send("");
}

function type() {
	ws.send(document.getElementById("input").value);
}

function setupListeners() {
	ws.onopen = (event) => {
		console.log("Socket is open");
		var input = document.getElementById("input");
		ws.send(input.value);  // Sends a message.
	};
	ws.onclose = (event) => {
		console.log("Socket closed");
	};
	ws.onmessage = (event) => {
		console.log("Recieved message: " + event.data);
		var input = document.getElementById("input");
		input.value = event.data;
	};
	setInterval(tick, 2000);
	var input = document.getElementById("input");
	input.addEventListener("input", type);
};

document.addEventListener("DOMContentLoaded", (event) => {
	setupListeners();
	console.log("Listeners are up");
});
