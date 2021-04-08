var r = require('rethinkdbdash')();

module.exports = {
    getAll: async () => {
        return r.table('user').coerceTo('array').run()
    },

    login: async (data) => {
        return r.table('user').filter(r.row('username').eq(data.username)).filter(r.row('password').eq(data.password)).coerceTo('array').run()
    },

    register: async (data) => {
        return r.table('user').insert(data).run()
    },
}
