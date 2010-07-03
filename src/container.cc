#include "dbic++.h"

namespace dbi {

    string ResultRow::join(string delim) {
        unsigned i;
        stringstream out;

        if (size() > 0) {
            for (i = 0; i < size() - 1; i++)
                out << at(i) << delim;
            out << at(size()-1);
        }

        return out.str();
    }

    void ResultRow::operator<<(string v) {
        this->push_back(PARAM(v));
    }

    void ResultRow::operator<<(Param &v) {
        this->push_back(v);
    }

    vector<string> ResultRowHash::columns(void) {
        vector<string> rs;

        for(map<string,Param>::iterator iter = begin(); iter != end(); ++iter)
            rs.push_back(iter->first);

        return rs;
    }

    ostream& operator<< (ostream &out, ResultRow &r) {
        out << r.join("\t");
        return out;
    }

    ostream& operator<< (ostream &out, ResultRowHash &r) {
        for(map<string,Param>::iterator iter = r.begin(); iter != r.end();) {
            out << iter->first << "\t" << iter->second;
            if (++iter != r.end()) out << "\t";
        }

        return out;
    }
}
