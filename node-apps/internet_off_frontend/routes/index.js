var express = require("express");
var router = express.Router();
var _ = require("lodash");
const {fetchData,addDataToLocal} = require("../utils/utils");


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
  req.app.locals.dataSet={};
  const db = req.app.locals.mongoClient;
  const offTime = await fetchData(db,20);
  const addToLocal = await addDataToLocal(offTime,req.app.locals.dataSet);
  return res.json({isMore:isMore})
})

module.exports = router;
