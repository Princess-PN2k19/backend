var r = require('rethinkdbdash')();

module.exports = {
    getAll: () => {
        return r.table('company').filter({status: 'Active'}).coerceTo('array').run()
    },

    getById: (id) => {
        return r.table('company').get(id).run()
    },

    create: (data) => {
        return r.table('company').insert(data).run()
    },

    update: (id, data) => {
        return r.table('company').get(id).update(data).run()
    },

    delete: (id) => {
        return r.table('company').get(id).update({status: 'Inactive'}).run()
    },

    getByCompanyName: (name) => {
        return r.table('company').filter({company_name: name, status: 'Active'}).coerceTo('array').run()
    }
}
