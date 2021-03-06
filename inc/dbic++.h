#ifndef _DBICXX_BASE_H
#define _DBICXX_BASE_H

#include <iostream>
#include <sstream>
#include <string>
#include <cstdarg>
#include <cstdio>
#include <vector>
#include <map>
#include <errno.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/time.h>
#include <dirent.h>
#include <dlfcn.h>
#include <pcrecpp.h>
#include <uuid/uuid.h>
#include <stdint.h>

#define DBI_VERSION      0.6.2

namespace dbi {
    struct null {};
    struct execute {};
}

namespace dbi {
    typedef std::vector<int> int_list_t;
    typedef std::vector<std::string> string_list_t;
}

#include "dbic++/error.h"
#include "dbic++/param.h"
namespace dbi {
    typedef std::vector<dbi::Param> param_list_t;
}

#include "dbic++/container.h"
namespace dbi {
    typedef dbi::FieldSet field_list_t;
}

#define DEFAULT_DRIVER_PATH "/usr/lib/dbic++"

#define DBI_TYPE_UNKNOWN   0
#define DBI_TYPE_INT       1
#define DBI_TYPE_TIME      2
#define DBI_TYPE_TEXT      3
#define DBI_TYPE_FLOAT     4
#define DBI_TYPE_NUMERIC   5
#define DBI_TYPE_BOOLEAN   6
#define DBI_TYPE_BLOB      7
#define DBI_TYPE_DATE      8
#define DBI_TYPE_TIMESTAMP 9

namespace dbi {
    extern bool _trace;
    extern int  _trace_fd;

    class AbstractStatement;
    class AbstractResult;
}

#include "dbic++/util.h"
#include "dbic++/io.h"
#include "dbic++/string_io.h"
#include "dbic++/file_io.h"
#include "dbic++/abstract_handle.h"
#include "dbic++/abstract_result.h"
#include "dbic++/abstract_statement.h"
#include "dbic++/handle.h"
#include "dbic++/statement.h"
#include "dbic++/result.h"
#include "dbic++/query.h"
#include "dbic++/etc.h"

#endif
