const r = require('rethinkdb');

module.exports = {
    getAll: async () => {
        const connect = await r.connect();
        return r.table('employee').coerceTo('array').run(connect)
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
        return r.table('employee').get(id).delete().run(connect)
    }
}