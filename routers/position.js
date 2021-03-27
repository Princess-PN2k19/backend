const positionController = require('../controllers/position');

module.exports = (app) => {
    app.get('/api/positions', positionController.getAllPositions);
    app.post('/api/position/create', positionController.create);
}