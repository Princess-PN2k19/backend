var r = require('rethinkdbdash')();

module.exports = {
    getAll: () => {
        return r.table('employee').filter({status: 'Active'}).coerceTo('array').run()
    },

    getById: (id) => {
        return r.table('employee').get(id).run()
    },

    create: (data) => {
        return r.table('employee').insert(data).run()
    },

    update: (id, data) => {
        console.log(data)
        return r.table('employee').get(id).update(data).run()
    },

    delete: (id) => {
        return r.table('employee').get(id).update({status: 'Inactive'}).run()
    },

    getByEmployeeName: (name) => {
        return r.table('employee').filter({employee_name: name , status: 'Active'}).coerceTo('array').run()
    },

    getByCompany: (company) => {
        return r.table('employee').filter({company_name: company, status: 'Active'}).coerceTo('array').run()
    }
}