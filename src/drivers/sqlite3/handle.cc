#include "common.h"

namespace dbi {

    Sqlite3Handle::Sqlite3Handle() {
        tr_nesting = 0;
        conn       = 0;
        _result    = 0;
    }

    Sqlite3Handle::Sqlite3Handle(string dbname) {
        tr_nesting = 0;
        conn       = 0;
        _dbname    = dbname;
        _result    = 0;

        reconnect();
    }

    void Sqlite3Handle::setTimeZoneOffset(int tzhour, int tzmin) {
    }

    void Sqlite3Handle::setTimeZone(char *name) {
    }

    Sqlite3Handle::~Sqlite3Handle() {
        cleanup();
    }

    void Sqlite3Handle::cleanup() {
        if (_result) delete _result;
        if (conn)    sqlite3_close(conn);

        _result = 0;
        conn    = 0;
    }

    Sqlite3Result* Sqlite3Handle::result() {
        Sqlite3Result *instance = _result;
        _result = 0;
        return instance;
    }

    uint32_t Sqlite3Handle::execute(string sql) {
        _sql = sql;

        Sqlite3Statement st(sql, conn);
        st.execute();
        _result = st.result();

        return _result->rows();
    }

    uint32_t Sqlite3Handle::execute(string sql, vector<Param> &bind) {
        _sql = sql;

        Sqlite3Statement st(sql, conn);
        st.execute(bind);
        _result = st.result();

        return _result->rows();
    }

    int Sqlite3Handle::socket() {
        throw RuntimeError("Sqlite3Handle::socket is not implemented");
        return 0;
    }

    Sqlite3Result* Sqlite3Handle::aexecute(string sql) {
        throw RuntimeError("Sqlite3Handle::aexecute is not implemented");
        return 0;
    }

    Sqlite3Result* Sqlite3Handle::aexecute(string sql, vector<Param> &bind) {
        throw RuntimeError("Sqlite3Handle::aexecute is not implemented");
        return 0;
    }

    void Sqlite3Handle::initAsync() {
        throw RuntimeError("Sqlite3Handle::initAsync is not implemented");
    }

    bool Sqlite3Handle::isBusy() {
        throw RuntimeError("Sqlite3Handle::busy is not implemented");
        return false;
    }

    bool Sqlite3Handle::cancel() {
        throw RuntimeError("Sqlite3Handle::cancel is not implemented");
        return false;
    }

    Sqlite3Statement* Sqlite3Handle::prepare(string sql) {
        return new Sqlite3Statement(sql, conn);
    }

    void Sqlite3Handle::_execute(string sql) {
        char *error;
        if (sqlite3_exec(conn, sql.c_str(), 0, 0, &error) != SQLITE_OK) {
            snprintf(errormsg, 8192, "%s", error);
            sqlite3_free(error);
            throw RuntimeError(errormsg);
        }
    }

    bool Sqlite3Handle::begin() {
        _execute("BEGIN");
        tr_nesting++;
        return true;
    };

    bool Sqlite3Handle::commit() {
        _execute("COMMIT");
        tr_nesting = 0;
        return true;
    };

    bool Sqlite3Handle::rollback() {
        _execute("ROLLBACK");
        tr_nesting = 0;
        return true;
    };

    bool Sqlite3Handle::begin(string name) {
        if (tr_nesting == 0) {
            begin();
            tr_nesting = 0;
        }
        _execute("SAVEPOINT " + name);
        tr_nesting++;
        return true;
    };

    bool Sqlite3Handle::commit(string name) {
        _execute("RELEASE SAVEPOINT " + name);
        tr_nesting--;
        if (tr_nesting == 0)
            commit();
        return true;
    };

    bool Sqlite3Handle::rollback(string name) {
        _execute("ROLLBACK TO SAVEPOINT " + name);
        tr_nesting--;
        if (tr_nesting == 0)
            rollback();
        return true;
    };

    bool Sqlite3Handle::close() {
        if (conn) sqlite3_close(conn);
        conn = 0;
        return true;
    }

    void Sqlite3Handle::reconnect() {
        close();
        int flags = SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE;
        if (sqlite3_open_v2(_dbname.c_str(), &conn, flags, 0) != SQLITE_OK) {
            snprintf(errormsg, 8192, "%s", sqlite3_errmsg(conn));
            throw ConnectionError(errormsg);
        }
    }

    void Sqlite3Handle::boom(const char* m) {
    }

    uint64_t Sqlite3Handle::write(string table, FieldSet &fields, IO* io) {
        return 0;
    }

    string Sqlite3Handle::escape(string value) {
        char *sqlite3_escaped = sqlite3_mprintf("%Q", value.c_str());
        string escaped = sqlite3_escaped;
        sqlite3_free(sqlite3_escaped);
        return escaped;
    }

    string Sqlite3Handle::driver() {
        return DRIVER_NAME;
    }

}
