#include "TarsInfoImp.h"
#include "servant/Application.h"
#include <map>
#include <sys/time.h>

using namespace std;

map<string, string> dbConfig;

//////////////////////////////////////////////////////
void TarsInfoImp::initialize()
{
    dbConfig["dbhost"] = "";
    dbConfig["dbuser"] = "";
    dbConfig["dbpass"] = "";
    dbConfig["dbname"] = "";
    dbConfig["charset"] = "";
    dbConfig["dbport"] = "";

    TC_DBConf dbCon;
    dbCon.loadFromMap(dbConfig);
    github_db.init(dbCon);
    github_db.connect();
}

//////////////////////////////////////////////////////
void TarsInfoImp::destroy()
{
    //destroy servant here:
    //...
}

//////////////////////////////////////////////////////
tars::Int32 TarsInfoImp::showInfo(const std::string& source, HttpApp::GitHubInfo &sRsp, tars::TarsCurrentPtr current) 
{
    sRsp = getInfo(source);
    TLOGDEBUG("TarsInfoImp::showInfo: sRsp::" << sRsp.most_concerned_issue << endl);
    return 0;
}

//////////////////////////////////////////////////////
HttpApp::GitHubInfo TarsInfoImp::getInfo(const string& source) 
{
    string tableName = "issue";
    HttpApp::GitHubInfo githubInfo;

    try
    {
        /*数据库操作
        * total_count           issue总数
        * closed_count          closed issue数量
        * query_closed          closed issue查询结果
        * highest_attention     评论数最高的所有issue条目
        * comment_number        评论数量
         */
        tars::Int32         total_count         = github_db.getRecordCount(tableName, "where source='" + source + "'");
        tars::Int32         closed_count        = github_db.getRecordCount(tableName, "where source='" + source + "' and status='closed'");
        TC_Mysql::MysqlData query_closed        = github_db.queryRecord(buildSelectSQL("issue", "opened_time, latest_time, No", source, "status='closed'"));
        TC_Mysql::MysqlData highest_attention   = github_db.queryRecord(buildSelectSQL("issue", "No, comment_number", source, "comment_number in (select max(comment_number) from issue where source='" + source + "')"));
        tars::Int32         comment_number      = 0;

        string      most_concerned_issue             = "None";               // 最受关注issue（评论数最多)
        string      time_utc_format                  = "%Y-%m-%dT%H:%M:%SZ"; // 时间格式
        string      issue_with_longest_handling_time = "None";               // 处理时间最长issue
        string      avg_time                         = "0";                  // 平均处理时间
        long int    avg_time_s                       = 0;                    // 平均处理时间，秒
        string      longest_handling_time            = "0";                  // 最长处理时间
        long int    longest_handling_time_s          = 0;                    // 最长处理时间，秒

        // 计算平均处理时间并获取处理时间最长的issue信息
        if (query_closed.size() > 0) 
        {
            struct tm   opened_time;
            struct tm   latest_time;
            string      No;

            long int    tm_delta;

            for (size_t i = 0; i < query_closed.size(); i++)
            {
                TC_Common::str2tm(query_closed[i]["opened_time"], time_utc_format, opened_time);
                TC_Common::str2tm(query_closed[i]["latest_time"], time_utc_format, latest_time);
                No              = query_closed[i]["No"];

                tm_delta  = (long int)(mktime(&latest_time) - mktime(&opened_time));
                avg_time_s += tm_delta;
                
                if (longest_handling_time_s < tm_delta)
                {
                    longest_handling_time_s             = tm_delta;
                    issue_with_longest_handling_time    = No;
                }
            }

            avg_time_s /= query_closed.size();
        }

        avg_time                = deltaTimeToStr(avg_time_s);
        longest_handling_time   = deltaTimeToStr(longest_handling_time_s);

        if (highest_attention.size() > 0) 
        {
            comment_number = TC_Common::strto<tars::Int32>(highest_attention[0]["comment_number"]);
            most_concerned_issue = "";
            for (size_t i = 0; i < highest_attention.size(); i++) 
            {
                most_concerned_issue += (" " + highest_attention[i]["No"]);
            }
        }

        githubInfo.source                           = source;
        githubInfo.total_count                      = total_count;
        githubInfo.closed_count                     = closed_count;
        githubInfo.avg_time                         = avg_time;
        githubInfo.most_concerned_issue             = most_concerned_issue;
        githubInfo.longest_handling_time            = longest_handling_time;
        githubInfo.issue_with_longest_handling_time = issue_with_longest_handling_time;
        githubInfo.comment_number                   = comment_number;

        TLOGDEBUG("TarInfoImp::getInfo::Data:  total_count: " << total_count 
                << ", closed_count: " << closed_count 
                << ", Highest Attention: " << most_concerned_issue << endl);
    }
    catch (TC_Mysql_Exception &ex) 
    {
        TarsRemoteNotify::getInstance()->report("Mysql Error: " + string(ex.what()));
        cout << "[Mysql Error]: " + string(ex.what()) << endl;
    }

    return githubInfo;
}

//////////////////////////////////////////////////////
string TarsInfoImp::buildSelectSQL(const string& table, const string& columns, const string& source, const string& condition) 
{
    string sql = "";
    if (condition == "")
        sql = "SELECT " + column + 
              " FROM " + table + 
              " WHERE source='" + source + "'";
    else 
        sql = "SELECT " + column + 
              " FROM " + table + 
              " WHERE source='" + source +
              "' and " + condition;

    return sql;
}

//////////////////////////////////////////////////////
string TarsInfoImp::deltaTimeToStr(long int tt)
{
    int day   = (int)(tt / 86400);
    tt       %= 86400;
    int hours = (int)(tt / 3600);
    tt       %= 3600;
    int min   = (int)(tt / 60);

    string deltaT;

    if (day > 0)
    {
        deltaT += TC_Common::tostr<int>(day) + "day";
        if (day > 1)
            deltaT += "s";
        
        deltaT += " ";
    }
    deltaT += TC_Common::tostr<int>(hours) + "h " + TC_Common::tostr<int>(min) + "min";
    
    return deltaT;
}
//////////////////////////////////////////////////////