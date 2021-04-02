const r = require('rethinkdb');

module.exports = {
    getAll: async () => {
        const connect = await r.connect();
        return r.table('employee').filter({status: 'Active'}).coerceTo('array').run(connect)
    },

    getById: async(id) => {
        const connect = await r.connect();
        return r.table('employee').get(id).run(connect)
    },

    create: async (data) => {
        const connect = await r.connect();
        return r.table('employee').insert(data).run(connect)
    },

    update: async (id, data) => {
        const connect = await r.connect();
        console.log(data)
        return r.table('employee').get(id).update(data).run(connect)
    },

    delete: async (id) => {
        const connect = await r.connect();
        return r.table('employee').get(id).update({status: 'Inactive'}).run(connect)
    },

    getByEmployeeName: async (name) => {
        const connect = await r.connect();
        return r.table('employee').filter({employee_name: name}).coerceTo('array').run(connect)
    }
}