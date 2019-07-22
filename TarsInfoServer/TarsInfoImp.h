#ifndef _TarsInfoImp_H_
#define _TarsInfoImp_H_

#include "servant/Application.h"
#include "util/tc_mysql.h"
#include "TarsInfo.h"

/**
 * @brief TarsInfoServer服务Obj TarsInfo的实现类
 *
 */
class TarsInfoImp : public HttpApp::TarsInfo
{
public:
    /**
     * @brief 析构函数
     */
    virtual ~TarsInfoImp() {}

    /**
     * @brief 初始化函数
     * 初始化参数: github_db
     */
    virtual void initialize();

    /**
     *
     */
    virtual void destroy();

    /**
     * @biref 默认生成接口.
     */
    virtual int test(tars::TarsCurrentPtr current) { return 0;};

    /**
    * @brief 服务接口, 获取issue统计数据. 
    *  
    * @param source  统计信息源(Tars, TarsCpp, TarsGo...)
    * @param sRsp    服务响应结果, Tars协议自定义数据类型GitHubInfo
    * @return        服务调用状态码
    */
    virtual tars::Int32 showInfo(const std::string &source, HttpApp::GitHubInfo &sRsp, tars::TarsCurrentPtr current);

private:
    /**
    * @brief Mysql操作类对象
    */
    TC_Mysql github_db;

    /**
    * @brief 获取issue统计数据. 
    *  
    * @param source  统计信息源(Tars, TarsCpp, TarsGo...)
    * @return        Tars协议自定义数据类型GitHubInfo
    */
    HttpApp::GitHubInfo getInfo(const std::string &source);

    /**
    * @brief 构造Select-SQL语句. 
    *  
    * @param table      表名
    * @param columns    列名
    * @param source     信息源
    * @param condition  条件
    * @return           string select-SQL语句
    */
    string buildSelectSQL(const string& table, const string& columns, const string& source, const string& condition);

    /**
    * @brief UTC秒时间差转字符串. 
    *  
    * @param tt     时间差秒数
    * @return       string 时间字符串(DD day HH h MM m)
    */
    string deltaTimeToStr(long int tt);
};
/////////////////////////////////////////////////////
#endif
