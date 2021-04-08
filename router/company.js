const companyController = require('../controller/company');

module.exports = (app) => {
    app.get('/api/companies', companyController.getAll);
    app.post('/api/company', companyController.create);
    app.put('/api/companies/:id', companyController.update);
    app.delete('/api/companies/:id', companyController.delete);
}