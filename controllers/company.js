const Company = require('../models/company')

module.exports = {
    getAll: async (req, res) => {
        const companies = await Company.getAll();

        return res.status(200).send(companies);
    },
    create: async (req, res) => {
        const company = await Company.create(req.body);

        return res.status(201).send({
            error: false,
            company
        })
    },
    update: async (req, res) => {
        const { id } = req.params;

        if (!id) {
            return res.status(400).send({
                message: 'Id parameter is missing.'
            })
        }

        const company = await Company.update(id, req.body);

        return res.status(202).send({
            error: false,
            company
        })
    },
    delete: async (req, res) => {
        const { id } = req.params;

        if (!id) {
            return res.status(400).send({
                message: 'Id parameter is missing.'
            })
        }

        const company = await Company.delete(id);

        return res.status(202).send({
            error: false,
            company
        })
    }
}