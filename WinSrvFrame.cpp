
// TestSysService.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include "SrvBase.h"
#include "SrvInstaller.h"

//����̳л���������
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
    //������ʱ���õĽӿڣ��˽ӿڲ���������������������һֱ��ʾ�����У�
    void OnStart(DWORD argc, TCHAR* argv[])
    {
        OutputDebugString(_T("TestService:Application start running!!!\n"));
    }

    //����ر�ʱ���õĽӿڣ��˽ӿڲ���������������������һֱ��ʾ�ر��У�
    void OnStop()
    {
        OutputDebugString(_T("TestService:Application end\n"));
    }
public:
    //������Է���ķ�ʽ���о͵��˺�������ֱ�ӵ���ServiceBase��Run�������˺����������Ժ󣬷�������OnStart������
    //����ǿ���̨�ķ�ʽ���о͵��˺�������д��Ҫ���߼����롣�˺���һ��Ҳ�ᵽ�ڲ�����OnStart��OnStop�������˺���Ϊ���̣߳����Բ����˳���
    bool Run(LPCTSTR param = _T(""))
    {
        if (_tcscmp(param, _T("console")) == 0)
        {
            //Todo������̨���д��� ����OnStart��OnStop

            TCHAR cinCmd[128];
            bool bStart = false;

            while (1)//����̨�������̲߳����˳� 
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

        return ServiceBase::Run();//����ķ�ʽ����
    }
};


int _tmain(int argc, _TCHAR* argv[])
{
    //�����������
    VKTestService service("VKService");

    //���в����ĵ���
    if (argc > 1)
    {
        if (_tcscmp(argv[1], _T("install")) == 0) {//��װ����
            OutputDebugString(_T("TestService:Installing service\n"));
            if (!ServiceInstaller::Install(service)) {
                OutputDebugString(_T("TestService:Couldn't install service\n"));
                return -1;
            }

            OutputDebugString(_T("TestService:Service installed\n"));
            return 0;
        }

        if (_tcscmp(argv[1], _T("uninstall")) == 0) {//ж�ط���
            OutputDebugString(_T("TestService:Uninstalling service\n"));
            if (!ServiceInstaller::Uninstall(service)) {
                OutputDebugString(_T("TestService:Couldn't uninstall service: %d\n"));
                return -1;
            }

            OutputDebugString(_T("TestService:Service uninstalled\n"));
            return 0;
        }

        if (_tcscmp(argv[1], _T("console")) == 0) {//�Կ���̨�ķ�ʽ����
            OutputDebugString(_T("TestService:console running\n"));
            service.Run(_T("console"));
            return 0;
        }
    }
    else
    {//�Է���ķ�ʽ����
        OutputDebugString(_T("TestService:start service"));
        service.Run();
    }
    return 0;
}


