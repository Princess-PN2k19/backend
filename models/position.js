const r = require('rethinkdb');

module.exports ={
    getAll: async () => {
        const connect = await r.connect();
        return r.table('positions').coerceTo('array').run(connect)
    },
    create: async (data) => {
        const connect = await r.connect();
        return r.table('positions').insert(data).run(connect)
    }
}