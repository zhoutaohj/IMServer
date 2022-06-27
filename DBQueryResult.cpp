#include "DBQueryResult.h"
#include "Tool.h"

DBQueryResult::DBQueryResult(const std::string& column, sql::ResultSet * res)
{
     _column_vec = Tool::SplitString(column,",");
     int index=0;

     for(auto& colum_name : _column_vec)
     {
         _column_index_map[colum_name] =index++;
     }

     while (res->next())
     {
        std::vector<std::string> row_data;
        for(auto column_value:_column_vec)
        {
            row_data.push_back(res->getString(column_value));
        }
        _result_data_vec.push_back(row_data);
     }
     
}

DBQueryResult::DBQueryResult(const DBQueryResult & rvalue):
_column_vec(rvalue._column_vec),_result_data_vec(rvalue._result_data_vec)
{


}

DBQueryResult& DBQueryResult::operator=(const DBQueryResult & rvalue)
{
    std::vector<std::string> v;
    std::vector<std::vector<std::string>> data;
    _column_vec.swap(v);//release column;
    _result_data_vec.swap(data);//release result_data ;

    _column_vec =rvalue._column_vec;
    _result_data_vec = rvalue._result_data_vec;
    return *this;
}

DBQueryResult& DBQueryResult::operator=(DBQueryResult && rvalue)
{
    std::vector<std::string> v;
    std::vector<std::vector<std::string>> data;
    _column_vec.swap(v);//release column;
    _result_data_vec.swap(data);//release result_data ;

    _column_vec = std::move(rvalue._column_vec);
    _result_data_vec = std::move(rvalue._result_data_vec);

    return *this;
}

DBQueryResult::~DBQueryResult()
{

}

DBQueryResult::DBQueryResult(DBQueryResult && rvalue)
{
    _column_vec =std::move(rvalue._column_vec);
    _result_data_vec =std::move(rvalue._result_data_vec);
}

size_t DBQueryResult::GetRowCount() const
{// not thread safe;
    return _result_data_vec.size();
}

std::string DBQueryResult::Get(size_t row ,const std::string& col_name)
{
    if(_result_data_vec.size() >= row)
    {
        auto pos = std::find(_column_vec.begin(),_column_vec.end(),col_name);

        if(pos!=_column_vec.end())
        {
           return _result_data_vec[row][_column_index_map[col_name]];
        }
    }
    
    return "";
}