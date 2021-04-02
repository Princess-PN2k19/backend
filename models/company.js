const r = require('rethinkdb');

module.exports = {
    getAll: async () => {
        const connect = await r.connect();
        return r.table('company').filter({status: 'Active'}).coerceTo('array').run(connect)
    },

    getById: async (id) => {
        const connect = await r.connect();
        return r.table('company').get(id).run(connect)
    },

    create: async (data) => {
        const connect = await r.connect();
        return r.table('company').insert(data).run(connect)
    },

    update: async (id, data) => {
        const connect = await r.connect();
        return r.table('company').get(id).update(data).run(connect)
    },

    delete: async (id) => {
        const connect = await r.connect();
        return r.table('company').get(id).update({status: 'Inactive'}).run(connect)
    },

    getByCompanyName: async (name) => {
        const connect = await r.connect();
        return r.table('company').filter({company_name: name}).coerceTo('array').run(connect)
    }
}
