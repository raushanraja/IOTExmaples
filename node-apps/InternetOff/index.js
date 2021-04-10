const socket = require("socket.io")();
const { MongoClient } = require("mongodb");
const mongoUser = "";
const mongoPass = "";
const mongoIP = "";
const mongoPort = "";
const mongouri =
  "mongodb+srv://" +
  mongoUser +
  ":" +
  mongoPass +
  "@" +
  mongoIP +
  ":" +
  mongoPort +
  "/?poolSize=20&writeConcern=majority";
const statusInfo = {
  isFirstRun: true,
  isDown: false,
  downTimeStart: "",
  downTimeEnd: "",
  updateId: null,
};
let previourTime = 0;

const client = new MongoClient(mon, {
  useNewUrlParser: true,
  useUnifiedTopology: true,
});

socket.on("connection", (client) => {
  console.log("Connected", " client id:", client.id);
  statusInfo.isFirstRun = false;
  statusInfo.isDown = false;
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
      client
        .db("InternetOff")
        .collection("downTime")
        .insertOne({ startTime: currentTime, endTime: null })
        .catch((e) => console.log("Error in updating db\n", e));
    }
  }
}

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
          .insertOne({ startTime: currentTime, endTime: null });
        statusInfo.updateId = resutl.insertedId;
      } catch (e) {
        console.log("Error in updating db\n", e);
      }
    }
  }
}, 5000);

async function startApp() {
  try {
    await client.connect();
    socket.listen(3001);
    checkIsDown();
  } finally {
    client.close();
  }
}
startApp().catch(console.dir);
