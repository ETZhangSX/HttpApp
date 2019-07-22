#ifndef _HttpImp_H_
#define _HttpImp_H_

#include "servant/Application.h"
#include "TarsInfo.h"

using namespace HttpApp;
/**
 * @brief Http服务实现类
 *
 */
class HttpImp : public Servant
{
public:
    /**
     *
     */
    virtual ~HttpImp() {}

    /**
     *
     */
    virtual void initialize();

    /**
     *
     */
    virtual void destroy();

    /**
     * @brief 重写doRequest响应函数
     *
     * @param   current
     * @param   buffer
     * @return  int
     */
	int doRequest(TarsCurrentPtr current, vector<char> &buffer);

private:
    TarsInfoPrx _pPrxTarsInfo;
    /**
     * @brief Http响应具体实现函数
     *
     * @param   req     Http请求
     * @param   rsp     Http响应
     * @return  int     服务响应状态码
     */
	int doRequest(const TC_HttpRequest &req, TC_HttpResponse &rsp);
};
/////////////////////////////////////////////////////
#endif
