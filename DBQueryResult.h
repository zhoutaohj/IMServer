#ifndef DBQUERYRESULT_H
#define DBQUERYRESULT_H
#include <vector>
#include <string>
#include <cppconn/resultset.h>
#include <map>

class DBQueryResult
{
public:
    DBQueryResult(const std::string& col,sql::ResultSet*);
    ~DBQueryResult();

    DBQueryResult(DBQueryResult&&);
    DBQueryResult(const DBQueryResult&);
    DBQueryResult& operator=(const DBQueryResult&);
    DBQueryResult& operator=(DBQueryResult&&);

    std::string Get(size_t row ,const std::string& col);
    size_t GetRowCount() const;
    //std::string GetColumn() const;

private:
    std::vector<std::string> _column_vec;
    std::map<std::string,int> _column_index_map;
    std::vector<std::vector<std::string>> _result_data_vec;
};

#endif