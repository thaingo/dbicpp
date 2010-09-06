namespace dbi {

    using namespace std;
    using namespace pcrecpp;

    /*
        Class: Statement
        Facade for AbstractStatement, you should be using this for most of the normal work. The class
        wraps the underlying database adapter and deallocates memory allocated from heap when destroyed.

        See <Handle> for an example.
    */
    class Statement {
        private:
        AbstractStatement *st;
        AbstractHandle *h;
        ResultRow params;
        public:
        Statement();
        /*
            Constructor: Statement(AbstractStatement*)
            Constructs a Statement instance given an abstract statement.

            Parameters:
            stmt - AbstractStatement instance.
        */
        Statement(AbstractStatement *stmt);

        /*
            Constructor: Statement(Handle&)
            Constructs a Statement instance given a handle.

            Parameters:
            handle - Handle instance.
        */
        Statement(Handle &handle);

        /*
            Constructor: Statement(Handle&, string)
            Constructs a Statement instance given a handle and a sql.

            Parameters:
            handle - Handle instance.
            sql    - SQL to prepare using the handle.
        */
        Statement(Handle &handle, string sql);

        /*
            Constructor: Statement(Handle *)
            Constructs a Statement instance given a pointer to handle.

            Parameters:
            handle - Pointer to Handle instance.
        */
        Statement(Handle *handle);

        /*
            Constructor: Statement(Handle *, string)
            Constructs a Statement instance given a pointer to a handle and a sql.

            Parameters:
            handle - Pointer to a Handle instance.
            sql    - SQL to prepare using the handle.
        */
        Statement(Handle *handle, string sql);

        ~Statement();

        /*
            Function: rows
            See <AbstractResult::rows()>
        */
        uint32_t rows();

        /*
            Function: columns
            See <AbstractResult::columns()>
        */
        uint32_t columns();

        /*
            Function: fields
            See <AbstractResult::fields()>
        */
        vector<string> fields();

        /*
            Function: types
            See <AbstractResult::types()>
        */
        vector<int>& types();

        /*
            Function: bind(long)
            Bind a long value to statement.

            Parameters:
            value - long
        */
        void bind(long value);

        /*
            Function: bind(double)
            Bind a double value to statement.

            Parameters:
            value - double
        */
        void bind(double value);

        /*
            Function: bind(Param)
            Bind a string or binary value to statement.

            Parameters:
            value - Param
        */
        void bind(Param value);

        /*
            Operator: << (string)
            Dealloactes any work memory and prepares a new statement for execution.
        */
        Statement& operator<<(string sql);

        /*
            Operator: , (string)
            Alias for bind(Param)

            Parameters:
            value - string
        */
        Statement& operator,(string value);
        /*
            Operator: % (string)
            Alias for bind(Param)

            Parameters:
            value - string
        */
        Statement& operator%(string value);

        /*
            Operator: , (long)
            Alias for bind(long)

            Parameters:
            value - long
        */
        Statement& operator,(long value);

        /*
            Operator: % (long)
            Alias for bind(long)

            Parameters:
            value - long
        */
        Statement& operator%(long value);

        /*
            Operator: , (double)
            Alias for bind(double)

            Parameters:
            value - double
        */
        Statement& operator,(double value);

        /*
            Operator: % (double)
            Alias for bind(double)

            Parameters:
            value - double
        */
        Statement& operator%(double value);

        /*
            Operator: , (null())
            Alias for bind(PARAM(null()))

            Parameters:
            value - null()
        */
        Statement& operator,(dbi::null const &e);

        /*
            Operator: % (null())
            Alias for bind(PARAM(null()))

            Parameters:
            value - null()
        */
        Statement& operator%(dbi::null const &e);

        /*
            Operator: , (execute())
            Alias for execute()

            This is mostly syntactic sugar allowing you to do,
            (start code)
                Statement stmt (handle, "select * from users where id = ? and name like ?")
                stmt % 1L, "jon%", execute();
            (end)
        */
        uint32_t       operator,(dbi::execute const &);

        /*
            Function: execute
            Executes the prepared statement along with the bind parameters
            bound using bind functions or operators.

            Returns:
            rows - The number of rows affected or returned.
        */
        uint32_t execute();

        /*
            Function: execute(vector<Param>&)
            See <AbstractStatement::execute(vector<Param>&)>
        */
        uint32_t execute(vector<Param> &bind);

        /*
            Function: read(ResultRow&)
            See <AbstractResult::read(ResultRow&)>
        */
        bool read(ResultRow&);

        /*
            Function: read(ResultRowHash&)
            See <AbstractResult::read(ResultRowHash&)>
        */
        bool read(ResultRowHash&);

        /*
            Function: read(uint32_t, uint32_t, uint64_t*)
            See <AbstractResult::read(uint32_t, uint32_t, uint64_t*)>
        */
        unsigned char* read(uint32_t r, uint32_t c, uint64_t*);

        /*
            Operator: (uint32_t, uint32_t)
            Alias for read(uint32_t, uint32_t)
        */
        unsigned char* operator()(uint32_t r, uint32_t c);

        /*
            Function: tell
            See <AbstractResult::tell()>
        */
        uint32_t tell();

        /*
            Function: seek
            See <AbstractResult::seek()>
        */
        void seek(uint32_t);

        /*
            Function: rewind
            See <AbstractResult::rewind()>
        */
        void rewind();

        /*
            Function: finish
            See <AbstractResult::finish()>
        */
        bool finish();

        /*
            Function: cleanup
            Releases local buffers and deallocates any temporary memory.
        */
        void cleanup();

        /*
            Function: lastInsertID
            See <AbstractResult::lastInsertID()>
        */
        uint64_t lastInsertID();

        /*
            Function: driver
            See <AbstractStatement::driver()>
        */
        string driver();
    };
}
