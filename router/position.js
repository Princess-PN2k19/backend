const positionController = require('../controller/position');

module.exports = (app) => {
    app.get('/api/positions', positionController.getAllPosition);
    app.post('/api/position/create', positionController.create);
}