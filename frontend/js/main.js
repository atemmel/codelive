console.log("Script running");

function setupListeners() {
	document.getElementById("send").addEventListener("click", function() {
		var ws = new WebSocket("ws://localhost:8080/chat");

		ws.onopen = function() {
			console.log("Socket is open");
			var contents = document.getElementById("input");
			ws.send(contents.value);  // Sends a message.
		};
	});
}

document.addEventListener("DOMContentLoaded", (event) => {
	setupListeners();
	console.log("Listeners are up");
});
