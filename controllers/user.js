const r = require('rethinkdb');
const User = require('../models/user');

module.exports = {
    login: async (req, res) => {
        const user = await User.login(req.body);
        console.log("USER", user)

        if (user.length > 0) {
            return res.status(200).send({
                error: false,
                user
            })
        } else {
            return res.status(401).send({
                error: true,
                user
            })
        }
    },
    register: async (req, res) => {
        const user = await User.register(req.body);
        const connect = await r.connect();
        const result = await r.table('user').filter({ "username": req.body.username }).coerceTo('array').run(connect)

        if (result > 0) {
            return res.status(409).send({
                error: true,
                user
            })
        } else {
            return res.status(201).send({
                error: false,
                user
            })
        }
    },

}