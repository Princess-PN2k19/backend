const r = require('rethinkdb');

module.exports = {
    login: async (data) => {
        const connect = await r.connect();
        return r.table('user').filter({ "username": data.username }).coerceTo('array').run(connect)
    },

    register: async (data) => {
        const connect = await r.connect();
        return r.table('user').insert(data).run(connect)
    },
}
