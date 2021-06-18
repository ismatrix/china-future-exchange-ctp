//--------------------------插入板块数据--------------------------
db.PRODUCTGROUP.insertMany(
   [
     { groupid: 1, groupcode: "GUIJINSHU", groupname: "贵金属",			productlist:['au','ag'],updatedate : new Date()},
     { groupid: 1, groupcode: "YOUSE", groupname: "有色金属",					productlist:['cu','al','zn','pb','ni','sn'],updatedate : new Date()},
     { groupid: 1, groupcode: "MEITAN", groupname: "煤炭",					productlist:['jm','j','ZC'],updatedate : new Date()},
     { groupid: 1, groupcode: "KUANGCHAN", groupname: "矿产",				productlist:['i'],updatedate : new Date()},
     { groupid: 1, groupcode: "GANGTIE", groupname: "钢铁",					productlist:['rb','i','j','hc','SF','SM'],updatedate : new Date()},
     { groupid: 1, groupcode: "JIANCAI", groupname: "建材",					productlist:['rb','FG'],updatedate : new Date()},
     { groupid: 1, groupcode: "HUAGONG", groupname: "化工",					productlist:['ru','l','TA','v','MA','pp','bu'],updatedate : new Date()},
     { groupid: 1, groupcode: "GUWU", groupname: "谷物",						productlist:['a','WH','c','RI','JR','LR'],updatedate : new Date()},
     { groupid: 1, groupcode: "SILIAO", groupname: "饲料",					productlist:['m','c','RM'],updatedate : new Date()},
     { groupid: 1, groupcode: "YOUZHI", groupname: "油脂",					productlist:['a','y','RS','OI','p'],updatedate : new Date()},
     { groupid: 1, groupcode: "RUANSHANGPIN", groupname: "软商品",		productlist:['CF','SR'],updatedate : new Date()},
     { groupid: 1, groupcode: "NONGFU", groupname: "农副",					productlist:['jd','cs'],updatedate : new Date()},
     { groupid: 1, groupcode: "LINMU", groupname: "林木",						productlist:['fb','bb'],updatedate : new Date()},
     { groupid: 0, groupcode: "INVESMART", groupname: "慧网商品指数",productlist:[],updatedate : new Date()}
   ]
)
//-------------------------------插入主连&指数记录------------------------------------
var insmain = db.INSTRUMENT.aggregate([
  { $match: {"productclass":"1"} },
  {
    $group:{
    _id : "$productid",
    maxins : {$max : "$instrumentid"}
    }
  }
]).map(function(a) {
  return a.maxins;
});
//插入指数数据
db.INSTRUMENT.aggregate([
  { $match: {instrumentid:{$in:insmain}} },
  { $lookup: {
            from: "PRODUCT",
            localField: "productid",
            foreignField: "productid",
            as: "product"
        }},
  { $unwind : "$product"},
  { $project : {
    _id : 0 ,
    "combinationtype" : 1,
    "createdate" : { $literal: "20010101" },
    "deliverymonth" : { $literal: 12 },
    "deliveryyear" : { $literal: 2099 },
    "enddelivdate" : { $literal: "20991201" },
    "exchangeid" : 1,
    "exchangeinstid" : { $concat: [ "$productid", "IDX" ] },
    "expiredate" : { $literal: "20991201" },
    "instlifephase" : 1,
    "instrumentid" : { $concat: [ "$productid", "IDX" ] },
    "instrumentname" : { $concat: [ "$product.productname", "指数" ] },
    "istrading" : 1,
    "longmarginratio" : 1,
    "maxlimitordervolume" : 1,
    "maxmarginsidealgorithm" : 1,
    "maxmarketordervolume" : 1,
    "minlimitordervolume" : 1,
    "minmarketordervolume" : 1,
    "opendate" : { $literal: "20010101" },
    "optionstype" : 1,
    "positiondatetype" : 1,
    "positiontype" : 1,
    "pricetick" : 1,
    "productclass" : { $literal: "9" },
    "productid" : 1,
    "requestid" : 1,
    "shortmarginratio" : 1,
    "startdelivdate" : { $literal: "20991201" },
    "strikeprice" : 1,
    "underlyinginstrid" : 1,
    "underlyingmultiple" : 1,
    "update_date" : 1,
    "volumemultiple" : 1
  } }
]).forEach(function(insidx){
   db.INSTRUMENT.insert(insidx);
});

//插入主连数据
db.INSTRUMENT.aggregate([
  { $match: {instrumentid:{$in:insmain}} },
  { $lookup: {
            from: "PRODUCT",
            localField: "productid",
            foreignField: "productid",
            as: "product"
        }},
  { $unwind : "$product"},
  { $project : {
    _id : 0 ,
    "combinationtype" : 1,
    "createdate" : { $literal: "20010101" },
    "deliverymonth" : { $literal: 12 },
    "deliveryyear" : { $literal: 2099 },
    "enddelivdate" : { $literal: "20991201" },
    "exchangeid" : 1,
    "exchangeinstid" : { $concat: [ "$productid", "0001" ] },
    "expiredate" : { $literal: "20991201" },
    "instlifephase" : 1,
    "instrumentid" : { $concat: [ "$productid", "0001" ] },
    "instrumentname" : { $concat: [ "$product.productname", "主连" ] },
    "istrading" : 1,
    "longmarginratio" : 1,
    "maxlimitordervolume" : 1,
    "maxmarginsidealgorithm" : 1,
    "maxmarketordervolume" : 1,
    "minlimitordervolume" : 1,
    "minmarketordervolume" : 1,
    "opendate" : { $literal: "20010101" },
    "optionstype" : 1,
    "positiondatetype" : 1,
    "positiontype" : 1,
    "pricetick" : 1,
    "productclass" : { $literal: "8" },
    "productid" : 1,
    "requestid" : 1,
    "shortmarginratio" : 1,
    "startdelivdate" : { $literal: "20991201" },
    "strikeprice" : 1,
    "underlyinginstrid" : 1,
    "underlyingmultiple" : 1,
    "update_date" : 1,
    "volumemultiple" : 1
  } }
]).forEach(function(insidx){
   db.INSTRUMENT.insert(insidx);
});

