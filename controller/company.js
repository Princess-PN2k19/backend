const Company = require('../model/company')

module.exports = {
    getAll: async (req, res) => {
        const companies = await Company.getAll();

        return res.status(200).send(companies);
    },
    create: async (req, res) => {
        const { company_name } = req.body
        const company_by_name = await Company.getByCompanyName(company_name)
        const get_companies = company_by_name.map((item) => item.company_name)

        if (get_companies.includes(company_name)) {
            return res.status(409).send({ //code 409 conflict
                error: true,
                message: 'Company Already Exists!'
            })
        } else {
            const company = await Company.create(req.body);

            return res.status(201).send({
                error: false,
                company
            })
        }

    },
    update: async (req, res) => {
        const { id } = req.params;
        const { company_name } = req.body
        const company_by_name = await Company.getByCompanyName(company_name)
        const get_companies = company_by_name.map((item) => item.company_name)

        if (!id) {
            return res.status(400).send({
                message: 'Id parameter is missing.'
            })
        }

        if (get_companies.includes(company_name)) {
            return res.status(409).send({ //code 409 conflict
                error: true,
                message: 'Company Already Exists!'
            })
        } else {
            const company = await Company.update(id, req.body);

            if (company.replaced > 0) {
                return res.status(200).send({
                    error: false,
                    company
                })
            } else {
                return res.status(400).send({
                    error: true,
                })
            }
        }

    },

    delete: async (req, res) => {
        const { id } = req.params;

        if (!id) {
            return res.status(400).send({
                message: 'Id parameter is missing.'
            })
        }

        const company = await Company.delete(id);

        return res.status(200).send({
            error: false,
            company
        })
    }
}