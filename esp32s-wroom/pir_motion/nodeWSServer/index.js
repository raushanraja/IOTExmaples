const socket = require("socket.io")();
const util = require("util");
const asyncExec = util.promisify(require("child_process").exec);
const { exec } = require("child_process");
let count = 0;


socket.on("connection", (client) => {
  console.log("Connected", " client id:", client.id);
  client.emit("init", { message: "connected" });
  client.on("message", (message) => handleMessage(message, client));
});


function handleMessage(message, client) {
  var today  = new Date().toLocaleString();
  console.log(message);
  if (message.a == 1) {
    count++;
    console.log('Activity Detected ', count , " " , today);
    socket.emit('message',count+". Activity Detected at "+ today);
  }
  
}

socket.listen(3001);
