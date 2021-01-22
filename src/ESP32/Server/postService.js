
// A public function
function postHandler(req, res){
    console.log("Landing on my Post Handler")
     var payload = req.body;
    console.log("The Body contains");
    console.log(payload);
    res.json({"status" : "ok"});
 
} 

module.exports = {
   ingest : postHandler
}


