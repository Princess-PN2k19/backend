const userController = require('../controllers/user');

module.exports = (app) => {
    app.get('/api/users', userController.getAllUsers);
    app.post('/api/user/register', userController.register);
    app.post('/api/user/login', userController.login);
}