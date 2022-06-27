#include "Tool.h"
#include "json/json.h"
namespace Tool
{
    std::vector<std::string> SplitString(const std::string &s, const std::string &c)
    {
        std::vector<std::string> v;
        std::string::size_type pos1, pos2;
        pos2 = s.find(c);
        pos1 = 0;
        while (std::string::npos != pos2)
        {
            v.push_back(s.substr(pos1, pos2 - pos1));

            pos1 = pos2 + c.size();
            pos2 = s.find(c, pos1);
        }
        if (pos1 != s.length())
            v.push_back(s.substr(pos1));

        return v;
    }

    std::string JsonToString(const Json::Value &root)
    {
        static Json::Value def = []()
        {
            Json::Value def;
            Json::StreamWriterBuilder::setDefaults(&def);
            def["emitUTF8"] = true;
            return def;
        }();

        std::ostringstream stream;
        Json::StreamWriterBuilder stream_builder;
        stream_builder.settings_ = def; // Config emitUTF8
        std::unique_ptr<Json::StreamWriter> writer(stream_builder.newStreamWriter());
        writer->write(root, &stream);
        return stream.str();
    }
} 

