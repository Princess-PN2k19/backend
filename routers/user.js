const userController = require('../controllers/user');

module.exports = (app) => {
    // app.get('/api/users', userController.getAll);
    // app.post('/api/user', userController.create);
    // app.put('/api/users/:id', userController.update);
    // app.delete('/api/users/:id', userController.delete);
    app.post('/api/user/register', userController.register);
    app.post('/api/user/login', userController.login);
}