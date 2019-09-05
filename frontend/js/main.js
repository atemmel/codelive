console.log("Script running");

function setupListeners() {
	document.getElementById("send").addEventListener("click", function() {
		var ws = new WebSocket("ws://" + location.host + ":80/chat");

		ws.onopen = (event) => {
			console.log("Socket is open");
			var contents = document.getElementById("input");
			ws.send(contents.value);  // Sends a message.
		};

		ws.onmessage = (event) => {
			console.log("Recieved message: " + event.data);
		};
	});
}

document.addEventListener("DOMContentLoaded", (event) => {
	setupListeners();
	console.log("Listeners are up");
});
