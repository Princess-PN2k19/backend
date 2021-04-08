var r = require('rethinkdbdash')();

module.exports = {
    getAll: async () => {
        return r.table('company').filter({status: 'Active'}).coerceTo('array').run()
    },

    getById: async (id) => {
        return r.table('company').get(id).run()
    },

    create: async (data) => {
        return r.table('company').insert(data).run()
    },

    update: async (id, data) => {
        return r.table('company').get(id).update(data).run()
    },

    delete: async (id) => {
        return r.table('company').get(id).update({status: 'Inactive'}).run()
    },

    getByCompanyName: async (name) => {
        return r.table('company').filter({company_name: name, status: 'Active'}).coerceTo('array').run()
    }
}
