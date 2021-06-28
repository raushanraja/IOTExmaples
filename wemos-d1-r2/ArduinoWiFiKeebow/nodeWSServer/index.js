const socket = require("socket.io")();
const util = require("util");
const asyncExec = util.promisify(require("child_process").exec);
const { exec } = require("child_process");

let currentSelectedSink;
let discordCurrentStatus;

socket.on("connection", (client) => {
  console.log("Connected", " client id:", client.id);
  client.emit("init", { message: "connected" });
  client.on("message", (message) => handleMessage(message, client));
});

async function cycleAudioOutput() { 
 const { stdout, stderr } = await asyncExec(
    "pacmd list-sinks | grep -e index:"
  ).catch(error => Promise.reject('error while executing command'));
  
  if (stdout) {
    let outSinkList = stdout
      .trim()
      .split("\n")
      .map((outSink, index) => {
        let splitSink = outSink.trim().split(":");
        if (splitSink[0][0] === "*") currentSelectedSink = index;
        return splitSink[1].trim();
      });
    
      if(currentSelectedSink >= outSinkList.length-1){
        currentSelectedSink = 0;
      }else{
        currentSelectedSink++;
      }
      exec(`pactl set-default-sink ${outSinkList[currentSelectedSink]}`);

  }
}

function openPavucontrol() {
  let x = exec("pidof pavucontrol", function (err, out, code) {
    if (out) {
      exec(`kill ${out}`, (err, out, code) => {
        console.log("killed\r");
      });
    } else {
      exec(`pavucontrol`);
    }
  });
}

function handleDiscord() {
  let x = exec("pidof Discord",(err,out,code)=>{
    err && console.log(err);
    code && console.log(code);
    if(out){
      console.log(out)
      exec(`i3-msg -t get_tree | egrep -o ${`"(class\\":\\".[^\\"]+\\")"`}`,(err,out,code)=>{
      err && console.log(err);
      code && console.log(code);
        if(out){
	        console.log(out)
          app_list = out.trim().split('\n').map(app=>app.split(":")[1].replace(/\"/g, ""))
          if(app_list.includes('discord')){
            exec(`i3-msg '[class=\"discord\"] kill'`,(err,out,code)=>{
              err && console.log(err);
              code && console.log(code);
              out && console.log(out)
            })
          }else{
            exec('discord',(err,out,code)=>{
              err && console.log(err);
              code && console.log(code);
              out && console.log(out)
            })
          }
        }
      })
    }
    else if(err){
      exec('discord',(err,out,code)=>{
        err && console.log(err);
        code && console.log(code);
        out && console.log(out)
      })
    }
    else{
      console.log(code);
    }
  })
}

function handleMessage(message, client) {
  console.log(message);
  if (message.a == 1) {
  console.log('audio');
    try {
      cycleAudioOutput().catch(error=> console.log(error));
    } catch (e) {
      console.log(e);
    }
  }
  if (message.d == 1) {
    console.log('discord');
    try {
      handleDiscord();
    } catch (e) {
      console.log(e);
    }
  }
}

socket.listen(3000);
