const positionController = require('../controller/position');

module.exports = (app) => {
    app.get('/api/positions', positionController.getAllPositions);
    app.post('/api/position/create', positionController.create);
}