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
            dataSet[_id] = {
              _id: _id,
              startTime: new Date(startTime).toLocaleString(),
              endTime: new Date(endTime).toLocaleString(),
              duration: (duration / 60).toFixed(0),
            };
            if(index == result.length-1){
              return res('done')
            }
          });
        });
      });
}


module.exports = {
    fetchData:fetchData,
    addDataToLocal:addDataToLocal
}