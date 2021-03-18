const r = require('rethinkdb');

module.exports = {
    // getAll: async (table) => {
    //     const connect = await r.connect();
    //     return r.table(table).coerceTo('array').run(connect)
    // },

    // getById: async (table, id) => {
    //     const connect = await r.connect();
    //     return r.table(table).get(id).run(connect)
    // },

    // register: async (table, data) => {
    //     const connect = await r.connect();
    //     return r.table(table).insert(data).run(connect)
    // },

    // update: async (table, id, data) => {
    //     const connect = await r.connect();
    //     return r.table(table).get(id).update(data).run(connect)
    // },

    // delete: async (table, id) => {
    //     const connect = await r.connect();
    //     return r.table(table).get(id).delete().run(connect)
    // }
    login: async (data) => {
        const connect = await r.connect();
        console.log("USERNAME", data)
        return r.table('user').get(data.username).run(connect)
    },

    register: async (data) => {
        const connect = await r.connect();
        return r.table('user').insert(data).run(connect)
    },
}
