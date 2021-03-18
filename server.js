const express = require('express');
const r = require('rethinkdb');
const app = express();
const PORT = process.env.PORT || 5000;
const testAPIRouter = require("./routers/testAPI");

//IMPORT MODELS
require('./models/company');
require('./models/employee');
require('./models/user');

app.use(express.json());

//IMPORT ROUTES
require('./routers/company')(app);
require('./routers/employee')(app);
require('./routers/user')(app);
app.use("/testAPI", testAPIRouter);

// CONNECT TO DB
(async () => {
    try {
        const conn = await r.connect();
        console.log(await r.dbList().run(conn))
    } catch (e) {
        console.log('err', e)
    }
})()

app.listen(PORT, async () => {
    console.log(`app running on port ${PORT}`)
});