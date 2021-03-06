#pragma once

namespace dbi {

    /*
        Class: AbstractHandle
        Pure virtual class that defines the api that individual database drivers need to support.
        Use this as a reference only. It is recommended to use the Handle class for any real work.
    */
    class AbstractHandle {
        public:
        /*
            Function: prepare(string)
            prepares a SQL and returns a statement handle.

            Parameters:
            sql - SQL statement.

            Returns:
            A pointer to AbstractStatement.
        */
        virtual AbstractStatement* prepare(std::string sql) = 0;

        /*
            Function: execute(string)
            Executes a SQL.

            Parameters:
            sql - SQL without placeholders.

            Returns:
            rows - number of rows affected or returned.
        */
        virtual uint32_t execute(std::string sql) = 0;

        /*
            Function: execute(string, param_list_t&)
            executes a SQL with bind values.

            Parameters:
            sql  - SQL with or without placeholders.
            bind - param_list_t that contains bind values. Refer to the <Param> struct and associated methods.

            Returns:
            rows - number of rows affected or returned.
        */
        virtual uint32_t execute(std::string sql, param_list_t &bind) = 0;

        /*
            Function: result
            Returns a pointer to a result object. This needs to be
            deallocated explicitly.

            Returns:
            AbstractResult* - Pointer to the Result set object.
        */
        virtual AbstractResult* result() = 0;

        /*
            Function: begin
            Starts a transaction. This will rollback any previous uncommited transactions on this handle.

            Returns:
            true or false - denotes success or failure.
        */
        virtual bool begin() = 0;

        /*
            Function: commit
            Commits a transaction.

            Returns:
            true or false - denotes success or failure.
        */
        virtual bool commit() = 0;

        /*
            Function: rollback
            Rollback a transaction.

            Returns:
            true or false - denotes success or failure.
        */
        virtual bool rollback() = 0;

        /*
            Function: begin(string)
            Begin a savepoint.

            Parameters:
            name - Savepoint name. This is optional, if none is provided usually a UUID is used.

            Returns:
            true or false - denotes success or failure.
        */
        virtual bool begin(std::string name) = 0;

        /*
            Function: commit(string)
            Commits a savepoint.

            Parameters:
            name - Savepoint name. If none provided it will commit the most recent savepoint.

            Returns:
            true or false - denotes success or failure.
        */
        virtual bool commit(std::string name) = 0;

        /*
            Function: rollback(string)
            Rollback a savepoint.

            Parameters:
            name - Savepoint name. If none provided it will rollback the most recent savepoint.

            Returns:
            true or false - denotes success or failure.
        */
        virtual bool rollback(std::string name) = 0;

        /*
            Function: close
            Close connection.

            Returns:
            true or false - denotes success or failure.
        */
        virtual bool close() = 0;

        /*
            Function: cleanup
            Cleanup connection and release any state buffers. This will be usually called by
            the destructor in Handle class.
        */
        virtual void cleanup() = 0;

        /*
            Function: write(string, FieldSet&, IO*)
            Bulk write data into a database table.

            Parameters:
            table  - table name.
            fields - field names.
            io     - pointer to an IO object.

            *Tip*: Use the FieldSet constructor to create a field name array.

            (begin code)
            FieldSet fields(3, "id", "name", "age");
            (end)

            Returns:
            rows   - The number of rows written to database.
        */
        virtual uint64_t write(std::string table, FieldSet &fields, IO*) = 0;

        /*
            Function: setTimeZoneOffset(int, int)
            Sets the connection timezone offset.

            Parameters:
            hours  - Offset hours from UTC.
            mins   - Offset mins from UTC.
        */
        virtual void setTimeZoneOffset(int hours, int mins) = 0;

        /*
            Function: setTimeZone(char *)
            Sets the connection timezone as a string conforming to zoneinfo.

            Parameters:
            zone - Timezone name in zoneinfo format.
        */
        virtual void setTimeZone(char *zone) = 0;

        /*
            Function: escape(string)
            Escapes a string value for use in SQL queries.

            Parameters:
            value  - string value to be escaped.

            Returns:
            value  - string value that is escaped for the underlying database.
        */
        virtual string escape(std::string) = 0;

        /*
            Function: driver
            Returns the driver for this handle.

            Returns:
            name - string value.
        */
        virtual string driver()  = 0;
        virtual int  socket()    = 0;
        virtual void reconnect() = 0;

        virtual ~AbstractHandle() {}

        /*
            Function: aexecute(string)
            Executes a query asynchronously and returns the result for further polling.

            Returns:
            AbstractResult* - result object
        */
        virtual AbstractResult* aexecute(std::string sql) = 0;

        /*
            Function: aexecute(string,
            Executes a query asynchronously and returns the result for further polling.

            Returns:
            AbstractResult* - result object
        */
        virtual AbstractResult* aexecute(std::string sql, param_list_t &bind) = 0;
        virtual bool cancel() = 0;

        protected:
        virtual void async(bool) = 0;
    };

}
