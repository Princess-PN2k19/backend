const r = require('rethinkdb');
const User = require('../models/user');

module.exports = {

    login: async (req, res) => {

        const user = await User.login(req.body);
        console.log("REQ", req.body.username)
        const result = r.table('user').get(req.body.username).values();
        console.log("Length", result)

        if (r.table('user').get(req.body.username).values().length !== 0) {
            return res.status(200).send({
                error: false,
                user
            })
        } else {
            return res.status(200).send({
                error: true,
                user
            })
        }


    },
    register: async (req, res) => {
        const user = await User.register(req.body);

        return res.status(201).send({
            error: false,
            user
        })
    },

    // getAll: async (req, res) => {
    //     const users = await User.getAll('user');

    //     return res.status(200).send(users);
    // },
    // create: async (req, res) => {
    //     const user = await User.create('user', req.body);

    //     return res.status(201).send({
    //         error: false,
    //         user
    //     })
    // },
    // update: async (req, res) => {
    //     const { id } = req.params;

    //     if (!id) {
    //         return res.status(400).send({
    //             message: 'Id parameter is missing.'
    //         })
    //     }

    //     const user = await User.update('user', id, req.body);

    //     return res.status(202).send({
    //         error: false,
    //         user
    //     })
    // },
    // delete: async (req, res) => {
    //     const { id } = req.params;

    //     if (!id) {
    //         return res.status(400).send({
    //             message: 'Id parameter is missing.'
    //         })
    //     }

    //     const user = await User.delete('user', id);

    //     return res.status(202).send({
    //         error: false,
    //         user
    //     })
    // }

}