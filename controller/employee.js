const Employee = require('../model/employee')

module.exports = {
    getAllEmployee: async (req, res) => {
        const employees = await Employee.getAll();

        return res.status(200).send(employees);
    },

    getByCompany: async (req, res) => {
        const employee_by_company = await Employee.getByCompany();

        return res.status(200).send(employee_by_company);
    },

    create: async (req, res) => {
        const { employee_name, employee_position, company_name } = req.body
        const employee_by_name = await Employee.getByEmployeeName(employee_name)
        const get_employees_positions = employee_by_name.map((item) => item.employee_position)
        const get_employees_company = employee_by_name.map((item) => item.company_name)

        if (get_employees_positions.includes(employee_position) && get_employees_company.includes(company_name)) {
            return res.status(409).send({ //code 409 conflict
                error: true,
                message: 'Employee Already Exist!'
            })
        } else {
            const employee = await Employee.create(req.body);

            return res.status(200).send({
                error: false,
            })
        }

    },

    update: async (req, res) => {
        const { id } = req.params;
        const { employee_name, employee_position, company_name } = req.body
        if (!id) {
            return res.status(400).send({
                message: 'Id parameter is missing.'
            })
        }

        const employee_by_name = await Employee.getByEmployeeName(employee_name)
        const get_employees_positions = employee_by_name.map((item) => item.employee_position)
        const get_employees_company = employee_by_name.map((item) => item.company_name)

        if (get_employees_positions.includes(employee_position) && get_employees_company.includes(company_name)) {
            
            return res.status(409).send({ //code 409 conflict
                error: true,
                message: 'Employee Already Exist!'
            })
        }
        else {
            const employee = await Employee.update(id, req.body);
            return res.status(200).send({
                error: false,
                employee
            })
        }
    },

    delete: async (req, res) => {
        const { id } = req.params;

        const employee = await Employee.delete(id);

        return res.status(200).send({
            error: false,
            employee
        })
    },

}