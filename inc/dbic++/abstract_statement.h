namespace dbi {

    using namespace std;
    using namespace pcrecpp;

    /*
        Class: AbstractStatement
        Pure virtual class that defines the api that individual database drivers need to support.
        Use this as a reference only. It is recommended to use the Statement class for any real work.
    */
    class AbstractStatement : public AbstractResultSet {
        public:
        /*
            Function: command
            Returns the SQL statement attached to the statement.
        */
        virtual string command() = 0;

        /*
            Function: execute
            Executes a SQL.

            Parameters:
            sql - SQL without placeholders.

            Returns:
            rows - number of rows affected or returned.
        */
        virtual uint execute() = 0;

        /*
            Function: execute(vector<Param>&)
            executes a SQL with bind values.

            Parameters:
            sql  - SQL with or without placeholders.
            bind - vector<Param> that contains bind values. Refer to the <Param> struct and associated methods.

            Returns:
            rows - number of rows affected or returned.
        */
        virtual uint execute(vector<Param> &bind) = 0;
    };
}



