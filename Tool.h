#ifndef TOOL_H
#define TOOL_H
#include <vector>
#include <string>
#include <json/json.h>
namespace Tool
{
    std::vector<std::string> SplitString(const std::string &s, const std::string &c=" ");
    std::string JsonToString(const Json::Value & root);
};


#endif