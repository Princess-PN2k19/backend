const r = require('rethinkdb');

module.exports = {
    getAll: async () => {
        const connect = await r.connect();
        return r.table('user').coerceTo('array').run(connect)
    },

    login: async (data) => {
        const connect = await r.connect();
        return r.table('user').filter(r.row('username').eq(data.username)).filter(r.row('password').eq(data.password)).coerceTo('array').run(connect)
    },

    register: async (data) => {
        const connect = await r.connect();
        return r.table('user').insert(data).run(connect)
    },
}
