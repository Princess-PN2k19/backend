var r = require('rethinkdbdash')();

module.exports ={
    getAll: async () => {
        return r.table('positions').coerceTo('array').run()
    },
    create: async (data) => {
        return r.table('positions').insert(data).run()
    }
}