var r = require('rethinkdbdash')();

module.exports = {
    getAll: async () => {
        return r.table('employee').filter({status: 'Active'}).coerceTo('array').run()
    },

    getById: async(id) => {
        return r.table('employee').get(id).run()
    },

    create: async (data) => {
        return r.table('employee').insert(data).run()
    },

    update: async (id, data) => {
        console.log(data)
        return r.table('employee').get(id).update(data).run()
    },

    delete: async (id) => {
        return r.table('employee').get(id).update({status: 'Inactive'}).run()
    },

    getByEmployeeName: async (name) => {
        return r.table('employee').filter({employee_name: name , status: 'Active'}).coerceTo('array').run()
    }
}