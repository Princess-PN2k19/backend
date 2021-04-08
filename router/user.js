const userController = require('../controller/user');

module.exports = (app) => {
    app.get('/api/users', userController.getAllUser);
    app.post('/api/user/register', userController.register);
    app.post('/api/user/login', userController.login);
}