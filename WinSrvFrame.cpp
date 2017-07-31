
// TestSysService.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "SrvBase.h"
#include "SrvInstaller.h"

//此类继承基础服务类
class VKTestService :public ServiceBase
{
public:
#ifdef _DEBUG
    VKTestService(const CString&name)
        :ServiceBase(name,
        name,
        SERVICE_DEMAND_START,
        SERVICE_ERROR_NORMAL,
        SERVICE_ACCEPT_STOP
        ){}
#else
    VKTestService(const CString&name)
        :ServiceBase(name,
        name,
        SERVICE_AUTO_START,
        SERVICE_ERROR_NORMAL,
        SERVICE_ACCEPT_STOP
        ){}
#endif
private:
    //服务开启时调用的接口（此接口不能阻塞，如果阻塞服务会一直显示开启中）
    void OnStart(DWORD argc, TCHAR* argv[])
    {
        OutputDebugString(_T("TestService:Application start running!!!\n"));
    }

    //服务关闭时调用的接口（此接口不能阻塞，如果阻塞服务会一直显示关闭中）
    void OnStop()
    {
        OutputDebugString(_T("TestService:Application end\n"));
    }
public:
    //如果是以服务的方式运行就到此函数里面直接调用ServiceBase的Run函数。此函数调用完以后，服务会调用OnStart函数。
    //如果是控制台的方式运行就到此函数里面写主要的逻辑代码。此函数一般也会到内部调用OnStart和OnStop函数。此函数为主线程，所以不能退出。
    bool Run(LPCTSTR param = _T(""))
    {
        if (_tcscmp(param, _T("console")) == 0)
        {
            //Todo：控制台运行处理 调用OnStart和OnStop

            TCHAR cinCmd[128];
            bool bStart = false;

            while (1)//控制台运行主线程不能退出 
            {
                _tprintf(_T("->input cmd\r\n"));

                _tscanf_s(_T("%s"), cinCmd, 128);
                if (_tcsncmp(cinCmd, _T("?"), 1) == 0)
                {
                    _tprintf(_T("\r\n========================================\r\n"));
                    _tprintf(_T("\"?\"     -show cmd help\r\n"));
                    _tprintf(_T("\"start\" -start service\r\n"));
                    _tprintf(_T("\"stop\"  -stop service\r\n"));
                    _tprintf(_T("\"exit\"  -exit service\r\n"));
                    _tprintf(_T("========================================\r\n"));
                }
                else if (_tcsncmp(cinCmd, _T("start"), 5) == 0)
                {
                    if (!bStart)
                    {
                        OnStart(0, NULL);

                        _tprintf(_T("\r\n========================================\r\n"));
                        _tprintf(_T("-> start service\r\n"));
                        _tprintf(_T("========================================\r\n"));
                    }
                    bStart = true;
                }
                else if (_tcsncmp(cinCmd, _T("stop"), 4) == 0)
                {
                    if (bStart)
                    {
                        OnStop();

                        _tprintf(_T("\n========================================\n"));
                        _tprintf(_T("-> stop service\r\n"));
                        _tprintf(_T("========================================\n"));
                    }

                    bStart = false;
                }
                else if (_tcsncmp(cinCmd, _T("exit"), 4) == 0)
                {

                    _tprintf(_T("\r\n========================================\r\n"));
                    _tprintf(_T("-> exit service\r\n"));
                    _tprintf(_T("========================================\r\n"));

                    break;
                }
                else
                {
                    _tprintf(_T("invalid cmd\r\n"));
                }
            }

            if (bStart)
                OnStop();

            return true;
        }

        return ServiceBase::Run();//服务的方式运行
    }
};


int _tmain(int argc, _TCHAR* argv[])
{
    //创建服务对象
    VKTestService service("VKService");

    //带有参数的调用
    if (argc > 1)
    {
        if (_tcscmp(argv[1], _T("install")) == 0) {//安装服务
            OutputDebugString(_T("TestService:Installing service\n"));
            if (!ServiceInstaller::Install(service)) {
                OutputDebugString(_T("TestService:Couldn't install service\n"));
                return -1;
            }

            OutputDebugString(_T("TestService:Service installed\n"));
            return 0;
        }

        if (_tcscmp(argv[1], _T("uninstall")) == 0) {//卸载服务
            OutputDebugString(_T("TestService:Uninstalling service\n"));
            if (!ServiceInstaller::Uninstall(service)) {
                OutputDebugString(_T("TestService:Couldn't uninstall service: %d\n"));
                return -1;
            }

            OutputDebugString(_T("TestService:Service uninstalled\n"));
            return 0;
        }

        if (_tcscmp(argv[1], _T("console")) == 0) {//以控制台的方式运行
            OutputDebugString(_T("TestService:console running\n"));
            service.Run(_T("console"));
            return 0;
        }
    }
    else
    {//以服务的方式运行
        OutputDebugString(_T("TestService:start service"));
        service.Run();
    }
    return 0;
}


