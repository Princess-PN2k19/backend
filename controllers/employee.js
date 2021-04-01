const Employee = require('../models/employee')

module.exports = {
    getAllEmployees: async (req, res) => {
        const employees = await Employee.getAll();

        return res.status(200).send(employees);
    },
    create: async (req, res) => {
        const employee = await Employee.create(req.body);

        return res.status(201).send({
            error: false,
            employee
        })
    },
    update: async (req, res) => {
        const { id } = req.params;

        if (!id) {
            return res.status(400).send({
                message: 'Id parameter is missing.'
            })
        }

        const employee = await Employee.update(id, req.body);

        return res.status(202).send({
            error: false,
            employee
        })
    },
    delete: async (req, res) => {
        const { id } = req.params;

        const employee = await Employee.delete(id);

        return res.status(202).send({
            error: false,
            employee
        })
    }
}