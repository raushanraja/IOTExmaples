const options = { weekday: 'long', day: 'numeric' ,month: 'long',year: 'numeric',hour:'numeric',minute:'numeric',second:'numeric'  };
async function fetchData(db,limit){
    const offTime = await db
    .db("InternetOff")
    .collection("downTime")
    .find(
      {},
      {
        limit: limit,
        sort: { startTime: -1 },
        projection: { _id: 1, startTime: 1, endTime: 1, duration: 1 },
        timeout : false
      }
    );
    return offTime;
}

function addDataToLocal(offTime,dataSet) {
    return new Promise((res,rej)=>{
        offTime.toArray(function (err, result) {
          if (err) throw err;
          result.map(({ _id, startTime, endTime, duration }, index) => {
            console.log(new Date(startTime).toLocaleString());
            dataSet[startTime] = {
              _id: _id,
              startTime: new Date(startTime).toLocaleDateString("hi-IN",options),
              endTime: endTime?new Date(endTime).toLocaleString("hi-IN",options):"current",
              duration: duration?format((duration)):"current",
            };
            if(index == result.length-1){
              console.log(dataSet);
              return res('done')
            }
          });
        });
      });
}

function format(time) {   
  // Hours, minutes and seconds
  var hrs = ~~(time / 3600);
  var mins = ~~((time % 3600) / 60);
  var secs = ~~time % 60;
  var ret = "";
  if (hrs > 0) {
      ret += "" + hrs + " hr " + (mins < 10 ? "0" : "");
  }
  ret += "" + mins + " min " + (secs < 10 ? "0" : "");
  ret += "" + secs+" sec";
  return ret;
}


module.exports = {
    fetchData:fetchData,
    addDataToLocal:addDataToLocal
}