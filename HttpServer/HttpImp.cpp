#include "HttpImp.h"
#include "servant/Application.h"
#include "servant/Communicator.h"

using namespace std;
using namespace tars;

//////////////////////////////////////////////////////
//回调类重写
class TarsinfoCallback : public TarsInfoPrxCallback
{
public:
    ~TarsinfoCallback(){}

    /*
    * @brief 回调函数, 服务调用成功时调用
    */
    virtual void callback_showInfo(tars::Int32 ret,  const HttpApp::GitHubInfo& sRsp)
    {
        TLOGDEBUG("TarsInfoCallback callback_showInfo ret: " << ret << endl);
    }

    /*
    * @brief 异常回调函数, 服务调用异常时调用
    */
    virtual void callback_showInfo_exception(tars::Int32 ret)
    {
        TLOGERROR("TarsInfoCallback callback_showInfo_exception ret: " << ret << endl);
    }
private:

};

//////////////////////////////////////////////////////
void HttpImp::initialize()
{
    //initialize servant here:
    //...
    _pPrxTarsInfo = Application::getCommunicator()->stringToProxy<TarsInfoPrx>("HttpApp.TarsInfoServer.TarsInfoObj");
}

//////////////////////////////////////////////////////
void HttpImp::destroy()
{
    //destroy servant here:
    //...
}

int HttpImp::doRequest(TarsCurrentPtr current, vector<char> &buffer)
{
    TC_HttpRequest req;
    TC_HttpResponse rsp;

	// parse request header
    vector<char> v = current->getRequestBuffer();
    string sBuf;
    sBuf.assign(&v[0], v.size());
    req.decode(sBuf);

    int ret = doRequest(req, rsp);

    rsp.encode(buffer);

    return ret;
}

int HttpImp::doRequest(const TC_HttpRequest &req, TC_HttpResponse &rsp)
{
	string msg = "Hello Tars!";
    try
    {
        int     iRet    = -1;
        string  source  = "Tars";

        HttpApp::GitHubInfo result;
        iRet = _pPrxTarsInfo->showInfo(source, result);

        if (iRet == 0) {
            msg = "source: "                            + result.source + "<br>" +
                  "total count: "                       + TC_Common::tostr<tars::Int32>(result.total_count) + "<br>" +
                  "closed count: "                      + TC_Common::tostr<tars::Int32>(result.closed_count) + "<br>" +
                  "avg time: "                          + result.avg_time + "<br>" +
                  "most concerned issue(s): "           + result.most_concerned_issue + "<br>" +
                  "comment number: "                    + TC_Common::tostr<tars::Int32>(result.comment_number) + "<br>" +
                  "issue with longest handling time: "  + result.issue_with_longest_handling_time + "<br>" +
                  "longest handling time: "             + result.longest_handling_time;
        }
    }
    catch(exception &ex)
    {
        TLOGERROR("HttpImp::doRequest exception:" << ex.what() << endl);
    }
    rsp.setContentType("text/html");
    rsp.setResponse(msg.c_str(), msg.size());
    return 0;   
}
