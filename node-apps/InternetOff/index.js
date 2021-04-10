const socket = require("socket.io")();
const { MongoClient } = require("mongodb");
const dot = require("dotenv").config()
const uri = process.env.DB_URI;
const statusInfo = {
  isFirstRun: true,
  isDown: false,
  downTimeStart: "",
  downTimeEnd: "",
  updateId: null,
};
let previourTime = 0;

const client = new MongoClient(uri, { useNewUrlParser: true, useUnifiedTopology: true });

socket.on("connection", (client) => {
  console.log("Connected", " client id:", client.id);
  if(statusInfo.isFirstRun){
    statusInfo.isFirstRun = false;
    statusInfo.isDown = false;
  }
  client.emit("init", { message: "connected" });
  client.on("message", (message) => handleMessage(message));
});

function handleMessage(message) {
  const currentTime = new Date().getTime();
  console.log(message);
  if (message.a == 1) {
    previourTime = currentTime;
    if (statusInfo.isDown) {
      statusInfo.isDown = false;
      statusInfo.downTimeEnd = currentTime;
      statusInfo.updateId = null;
      const filter = { startTime: statusInfo.downTimeStart }; 
      const options = {};
      const updateDoc = {$set: {endTime:currentTime,duration:(currentTime-statusInfo.downTimeStart)/1000},
      };
      client
        .db("InternetOff")
        .collection("downTime")
        .updateOne(filter,updateDoc,options)
        .catch((e) => console.log("Error in updating db\n", e));
    }
  }
}



async function startApp() {
  try {
    await client.connect();
    socket.listen(9899);
    const checkIsDown = setInterval(async () => {
      const currentTime = new Date().getTime();
      if (currentTime - previourTime > 5000) {
        if (!statusInfo.isFirstRun && statusInfo.updateId == null) {
          statusInfo.isDown = true;
          statusInfo.downTimeStart = currentTime;
          try {
            const resutl = await client
              .db("InternetOff")
              .collection("downTime")
              .insertOne({ startTime: currentTime, endTime: null, duration:null });
            statusInfo.updateId = resutl.insertedId;
          } catch (e) {
            console.log("Error in updating db\n", e);
          }
        }
      }
    }, 5000);
  } catch(e){
    console.log("Error in StartAPP()\n",e);
  }
}
startApp().catch(console.dir);