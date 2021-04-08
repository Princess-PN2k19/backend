const employeeController = require('../controller/employee');

module.exports = (app) => {
    app.get('/api/employees', employeeController.getAllEmployee);
    app.post('/api/employee', employeeController.create);
    app.put('/api/employees/:id', employeeController.update);
    app.delete('/api/employees/:id', employeeController.delete);
}