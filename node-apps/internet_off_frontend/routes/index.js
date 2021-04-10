var express = require("express");
var router = express.Router();
var { ObjectID } = require("mongodb");
var _ = require("lodash");
const app = require("../app");
const {fetchData,addDataToLocal} = require("../utils/utils");
const { request } = require("../app");


router.get("/", async function (req, res, next) {
  const db = req.app.locals.mongoClient;
  if (_.isEmpty(req.app.locals.dataSet)) {
    console.log("Inside IF");
    const offTime = await fetchData(db,20);
    const addToLocal = await addDataToLocal(offTime,req.app.locals.dataSet);
    req.app.locals.cursor = offTime;
    res.render("index", {
      title: "BSNL Down Time",
      offTime: req.app.locals.dataSet,
    });
  } else {
    console.log("Inside else");
    res.render("index", {
      title: "Personal BSNL Down Time",
      offTime: req.app.locals.dataSet,
    });
  }
});

router.get("/more",async function(req,res,next){
  let isMore = false;
  isMore = await req.app.locals.cursor.hasNext();
  const offTime = await cursor.next();
  console.log(offTime);

  // await new Promise((res,rej)=>{

  //   if (isMore){
  //     console.log(offTime);
  //   }
  // });

  return res.json({isMore:isMore})
})

module.exports = router;
