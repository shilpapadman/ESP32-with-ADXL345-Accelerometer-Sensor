


function getFunctionHandler (req, res){

    console.log("Landing on the Ping")

res.json({"ping" : "Got you!"});
 
}
module.exports = {
    ping : getFunctionHandler
 }