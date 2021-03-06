#include "dbic++.h"
#include <getopt.h>
#include <unistd.h>

/*----------------------------------------------------------------------------------

   To compile:

   g++ -o check check.cc `pkg-config --libs --cflags dbic++`

----------------------------------------------------------------------------------*/

using namespace std;
using namespace dbi;

int rows, iter;
char buffer[8192], driver[512];

void top() {
    sprintf(buffer, "ps -o 'vsize=' -o 'rss=' -o 'cmd=' -p%d", getpid());
    FILE *top = popen(buffer, "r");
    fgets(buffer, 8192, top);
    pclose(top);
    buffer[strlen(buffer)-1] = 0;
    printf("%s\n", buffer);
}

void parseOptions(int argc, char **argv) {
    int option_index, c;
    static struct option long_options[] = {
        { "iter",   required_argument, 0, 'n' },
        { "rows",   required_argument, 0, 'r' },
        { "driver", required_argument, 0, 'd' }
    };

    rows = 500;
    iter = 50;
    strcpy(driver, "postgresql");

    while ((c = getopt_long(argc, argv, "d:n:r:", long_options, &option_index)) >= 0) {
        if (c == 0) c = long_options[option_index].val;

        switch(c) {
            case 'r': rows = atol(optarg);    break;
            case 'n': iter = atol(optarg);    break;
            case 'd': strcpy(driver, optarg); break;
            default: exit(1);
        }
    }
}

int main(int argc, char *argv[]) {
    ResultRow r;
    parseOptions(argc, argv);
    dbiInitialize();

    bool sqlite3 = strcmp(driver, "sqlite3") == 0;

    for (int times = 0; times < 200; times++) {
        Handle h (driver, getlogin(), "", sqlite3 ? ":memory:" : "dbicpp");

        printf("-- run %d --\t", times);
        top();

        // create test table
        h.execute("drop table if exists users");
        h.execute("create table users(id integer primary key, name text, email text, created_at timestamp)");

        // insert some test data
        Statement ins (h, "insert into users(id, name, email, created_at) values(?, ?, ?, current_timestamp)");

        for (int n = 0; n < rows; n++) {
            sprintf(buffer, "test %d", n);
            ins % (long)(n+1);
            ins % buffer;
            ins % "test@example.com";
            ins.execute();
        }

        Query sel (h, "select id, name, email from users order by id");
        for (int n = 0; n < iter; n++) {
            sel.execute();
            while (sel.read(r)) { r.clear(); }
        }

        Statement upd (h, "update users set name = ?, email = ? where id = ?");
        for (int n = 0; n < iter; n++) {
            sprintf(buffer, "test %d", n+1);
            upd % buffer;
            upd % "test@example.com";
            upd % (long)(n+1);
            upd.execute();
        }

        FieldSet fields(3, "id", "name", "email");
        StringIO buffer;
        buffer.writef("%d\tuser1\tuser1@local\n", rows + 1);
        buffer.writef("%d\tuser2\tuser2@local\n", rows + 2);
        h.write("users", fields, &buffer);
    }

    dbiShutdown();
    return 0;
}
