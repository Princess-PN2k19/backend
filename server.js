const express = require('express');
const app = express();
const PORT = process.env.PORT || 5000;

//IMPORT MODEL
require('./model/company');
require('./model/employee');
require('./model/user');

app.use(express.json());

//IMPORT ROUTE
require('./router/company')(app);
require('./router/employee')(app);
require('./router/user')(app);
require('./router/position')(app);

app.listen(PORT, async () => {
    console.log(`app running on port ${PORT}`)
});
