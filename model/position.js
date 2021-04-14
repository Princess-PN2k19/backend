var r = require('rethinkdbdash')();

module.exports ={
    getAll: () => {
        return r.table('positions').coerceTo('array').run()
    },
    create: (data) => {
        return r.table('positions').insert(data).run()
    }
}