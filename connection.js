const rethink = require("rethinkdb");

module.exports= async function(){
    return await rethink.connect()
}
