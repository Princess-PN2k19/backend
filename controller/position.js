const r = require('rethinkdb');
const Position = require('../model/position');

module.exports = {
    getAllPositions: async (req, res) => {
        const positions = await Position.getAll();
        return res.status(200).send(positions);
    },
    create: async (req, res) => {
        const position = await Position.create(req.body);
        const connect = await r.connect();
        const result = r.table('positions').filter({ "role": req.body.role }).coerceTo('array').run(connect)

        if (result.length > 0) {
            return res.status(409).send({
                error: true,
            })
        } else {
            return res.status(201).send({
                error: false,
                position
            })
        }
    }
}