var r = require('rethinkdbdash')();

module.exports = {
    getAll: () => {
        return r.table('user').coerceTo('array').run()
    },

    login: (data) => {
        return r.table('user').filter(r.row('username').eq(data.username)).filter(r.row('password').eq(data.password)).filter(r.row('company').eq(data.company)).coerceTo('array').run()
    },

    register: (data) => {
        return r.table('user').insert(data).run()
    },
}
