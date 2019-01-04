#define _CRT_SECURE_NO_WARNINGS
#pragma comment(lib,"pdh")
#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <locale.h>
#include <pdh.h>
#include <pdhmsg.h>
#include <tchar.h>
#include <windows.h>
//һ.���ܼ�������飺
//ϵͳ���ܼ�������Ҳ��ö�����û���ù��ɣ����׳��ˣ������ν����ǿ�󣬹���˼����ǿ���ͳ�� Windows ϵͳ��������ָ��Ķ������� Windows 2000 ������ϵͳ�У������Ĭ�ϰ�װ�ͻ��Դ����ܼ��������򣬴�ҿ����� ��������� -������ -�����ܡ��п����ó���
//Windows ϵͳ���ܼ���������ʵʱ�����ϰ���ϵͳ����ָ�꣬�ڸ�ϵͳ����Ļ�ͼ����������һ���ѡ����Ӽ����������п��ܿ������п�ͳ�Ƶ���Ŀ����������̳�￴��һЩ������������Լ�ʵ������������������ʵʱ���ÿ�����̵�CPUʹ���ʣ��ڴ�ʹ��...�ȵȣ���ô����ʹ��ϵͳ���ܼ�����Ӧ�������ѡ���Թ�������ʵ�ַ������뿴���ġ�
//��.����ʵ�֣�
//���API��鼰�������룺
bool pdhdump() {
	//1.PdhOpenQuery���򿪼�����
	PDH_STATUS pdhStatus;
	//  HQUERY hQuery = NULL;
	//  HCOUNTER pCounterHandle = NULL;
	//  pdhStatus = PdhOpenQuery(0, 0, &hQuery);
	//  if (pdhStatus != ERROR_SUCCESS) return false;
	//  //�������������ռ�
	//  pCounterHandle = (HCOUNTER *)GlobalAlloc(GPTR, sizeof(HCOUNTER));
	//  if (pCounterHandle == NULL) return false;
	//2.PdhCloseQuery���رռ�����
	//  pdhStatus = PdhCloseQuery(hQuery);
	//  if (pdhStatus != ERROR_SUCCESS) return false;
	//3.PdhEnumObjects��ö�ټ�����Ŀ���ú�����6����������ϸ�뿴MSDN��
	//ԭ�ͣ�
	//PDH_STATUS PdhEnumObjects(
	//      LPCTSTR szDataSource,    // ����Ϊ NULL
	//      LPCTSTR szMachineName,   // �����������Ϊ��������ΪNULL
	//      LPTSTR mszObjectList,    // ����ȫ�����ü�����Ŀ�Ļ�����
	//      LPDWORD pcchBufferLength,  // ����ȥ��С(���Ϊ 0�����ֵ���������С)
	//      DWORD dwDetailLevel,    // ��ȡ��Ϣ�ļ���
	//      BOOL bRefresh        // һ������Ϊ TRUE
	//      );
	//������
	LPTSTR lpcsMachineName = NULL;
	LPTSTR lpsObjectListBuffer = NULL;
	DWORD dwObjectListSize = 0;

	//��һ���Ȱѻ���ȥ��С��Ϊ0�������ɻ�����軺������С
	pdhStatus = PdhEnumObjects(
		NULL,
		lpcsMachineName,
		lpsObjectListBuffer,
		&dwObjectListSize,
		PERF_DETAIL_WIZARD,
		TRUE);
	if (pdhStatus != ERROR_SUCCESS && pdhStatus != PDH_MORE_DATA) return false;
	//�õ���������С�󣬷��仺�����ڴ�
	lpsObjectListBuffer = (LPTSTR)malloc(dwObjectListSize + 1);
	if (lpsObjectListBuffer == NULL) return false;
	//�ڶ����ڴ˵���ö�ٺ���������ʼö�ټ�����Ŀ
	pdhStatus = PdhEnumObjects(
		NULL,
		lpcsMachineName,
		lpsObjectListBuffer,
		&dwObjectListSize,
		PERF_DETAIL_WIZARD,
		TRUE);
	if (pdhStatus != ERROR_SUCCESS) return false;
	//���滺������ַ
	LPTSTR lpsthisObject = NULL;
	lpsthisObject = lpsObjectListBuffer;
	//��ӡ���п��ü�����Ŀ
	if (dwObjectListSize>0)
		for (; *lpsthisObject != 0; lpsthisObject += (strlen(lpsthisObject) + 1)) {
			printf("Object:%s\n", lpsthisObject);
			//4.PdhEnumObjectItems��ö�ټ�������ʵ��
			//ԭ�ͣ�
			//PDH_STATUS PdhEnumObjectItems(
			//       LPCTSTR szDataSource,        // ����ΪNULL
			//       LPCTSTR szMachineName,       // �����������Ϊ��������ΪNULL
			//       LPCTSTR szObjectName,        // ������Ŀ��ͨ��PdhEnumObjects�����ɻ�����п�����Ŀ��
			//       LPTSTR mszCounterList,       // ������������
			//       LPDWORD pcchCounterListLength,   // ��������������С
			//       LPTSTR mszInstanceList,       // ����ʵ��������
			//       LPDWORD pcchInstanceListLength,   // ����ʵ����������С
			//       DWORD dwDetailLevel,        // ��ȡ��Ϣ�ļ���
			//       DWORD dwFlags            // 0
			//       );
			//�������ú����ķ���ͬ��һ������PdhEnumObjects���������뿴MSDN���ĸ����Ĳ��Թ��̴���
			LPTSTR mszCounterList = NULL;
			LPTSTR mszInstanceList = NULL;
			DWORD dwCounterListLength = 0;
			DWORD dwInstanceListLength = 0;
			//��һ���Ȱѻ���ȥ��С��Ϊ0�������ɻ�����軺������С
			pdhStatus = PdhEnumObjectItems(
				NULL,
				lpcsMachineName,
				lpsthisObject,
				mszCounterList,
				&dwCounterListLength,
				mszInstanceList,
				&dwInstanceListLength,
				PERF_DETAIL_WIZARD,
				0);
			if (pdhStatus != ERROR_SUCCESS && pdhStatus != PDH_MORE_DATA) return false;
			//�õ���������С�󣬷��仺�����ڴ�
			mszCounterList = (LPTSTR)malloc(dwCounterListLength + 1);
			if (mszCounterList == NULL) return false;
			mszInstanceList = (LPTSTR)malloc(dwInstanceListLength + 1);
			if (mszInstanceList == NULL) return false;
			pdhStatus = PdhEnumObjectItems(
				NULL,
				lpcsMachineName,
				lpsthisObject,
				mszCounterList,
				&dwCounterListLength,
				mszInstanceList,
				&dwInstanceListLength,
				PERF_DETAIL_WIZARD,
				0);
			if (pdhStatus != ERROR_SUCCESS) return false;
			LPTSTR lpsthisCounter = NULL;
			lpsthisCounter = mszCounterList;
			if (dwCounterListLength>0)
				for (; *lpsthisCounter != 0; lpsthisCounter += (strlen(lpsthisCounter) + 1)) {
					printf("    Counter:%s\n", lpsthisCounter);
				}
			LPTSTR lpsthisInstance = NULL;
			lpsthisInstance = mszInstanceList;
			if (dwInstanceListLength>0)
				for (; *lpsthisInstance != 0; lpsthisInstance += (strlen(lpsthisInstance) + 1)) {
					printf("    Instance:%s\n", lpsthisInstance);
				}
			free(mszInstanceList);
			free(mszCounterList);
		}
	free(lpsObjectListBuffer);
	return true;
}
//5.PdhAddCounter����Ӽ�����
//ͳ�Ƹ���Ȥ��ϵͳ��Ϣʱ�������Ƚ���Ӧ�ļ�������ӽ���
//ԭ�ͣ�
//PDH_STATUS PdhAddCounter(
//     PDH_HQUERY hQuery,     // ΪPdhOpenQuery�򿪵ľ��
//     LPCTSTR szFullCounterPath, // ������·��(��󳤶�Ϊ PDH_MAX_COUNTER_PATH)
//     DWORD_PTR dwUserData,    // ��Ϊ 0
//     PDH_HCOUNTER* phCounter   // ����������ռ�(��������PdhOpenQuery�������ѷ���)
//     );
//������
// �ѻ�ȡwinlogon.exe���̵�CPUʹ����Ϊ��
// ͨ��ö�ٲ��鿴������Ŀ˵������֪�� process ��Ŀ�Ǻͽ����йص���Ŀ
// ��ͨ��ö�ټ��������������鿴˵������֪�� process ��Ŀ�µ�% Processor Time�������ǹ��ڽ���CPUʹ�ʵ�
// ����ڼ����������п���winlogon����(�����ý�����������)
//  pdhStatus = PdhAddCounter (hQuery, "//process(winlogon)//% Processor Time", 0, pCounterHandle);
//  if (pdhStatus != ERROR_SUCCESS) return false;
//��ʾ����Щ������û��ʵ�������磺Ҫ�õ�ϵͳ�����������������е���������ô�ü�����ĿΪSystem,������ΪSystem Up Time��������ʵ��ΪNULL����ʱ�ļ�����·��Ϊ"/System/System Up Time"
//6.PdhCollectQueryData�� ׼����ȡ��ǰ����
//����:
//  pdhStatus = PdhCollectQueryData(hQuery);
//  if (pdhStatus != ERROR_SUCCESS)return false;
//7.PdhGetFormattedCounterValue���õ�����
//������
//  pdhStatus = PdhGetFormattedCounterValue (pCounterHandle, PDH_FMT_DOUBLE,
//             &dwctrType, &fmtValue);
//  if (pdhStatus != ERROR_SUCCESS) return false;
//  // PDH_FMT_DOUBLE��ʾ����double�����ݣ���Ȼ�����Է���int���������ݣ����MSDN
//  // ��ȡ��һʱ������
//  pdhStatus = PdhCollectQueryData(hQuery);
//  if (pdhStatus != ERROR_SUCCESS)return false;
//��ʾ��pCounterHandleΪPdhAddCounter�õ��ľ�������Բ�ͬ��pCounterHandle��ò�ͬ����ֵ
//8.PdhRemoveCounter���Ƴ�������
//�����ȡĳ�����ֵʱ��Ӧ���Ƴ��ü��������ѽ�ʡ��Դ
//������
//  if (PdhRemoveCounter(pdhCouner) != ERROR_SUCCESS) return false;
//�ú�������Ϊ���������
//���ˣ����ʹ�ü�����ʵʱ����ϵͳ��Ϣ�Ѿ�������ϡ��绹�в����׵���������ϸ�쿴MSDN����Email��wlzqin@sina.com �� QQ��8573980��ϵ��

//��.����ٸ�������һ�����API PdhGetCounterInfo,���API��ʹ�ü��������޹�ϵ�����ǣ�����������������������Ȥ�ļ�������·�����������Եõ�ÿ������������Ŀ��������Ϣ���������ĵ�Ŷ����
//������
//  //�Ի�ȡϵͳ�����������������е�����Ϊ��
//  PDH_COUNTER_INFO pdhCounterInfo;
//  DWORD dwCounterBuffsize;
//  //��Ӽ�����
//  pdhStatus = PdhAddCounter(hQuery, "//System//System Up Time", 0, pCounterHandle);
//  if (pdhStatus != ERROR_SUCCESS) return false;
//  //�õ���������С
//  pdhStatus = PdhGetCounterInfo(*pCounterHandle, TRUE, &dwCounterBuffsize, NULL);
//  if (pdhStatus != ERROR_SUCCESS && pdhStatus != PDH_MORE_DATA) return false;
//  //���û�����
//  BYTE  * byCounterBuff = (BYTE *)malloc(dwCounterBuffsize);
//  //��ȡ��Ϣ
//  pdhStatus = PdhGetCounterInfo (*pCounterHandle, TRUE, &dwCounterBuffsize, (PPDH_COUNTER_INFO)byCounterBuff);
//  if (pdhStatus != ERROR_SUCCESS) return false;
//  pdhCounterInfo = * (PPDH_COUNTER_INFO)byCounterBuff;
//  //��ӡ�õ�����Ϣ
//  printf(pdhCounterInfo->szExplainText);
//  //���ϴ�ӡ����Ϣ���ƣ�"System Up Time ָ��������ϴ��������Ѿ����е�ʱ��(�������)���������ֵ��ʾ����ʱ��͵�ǰʱ��֮�"
//ȫ��������ϣ�ϣ�������ܶԴ������������ף�����졣
//example code:
int ExistSameName(TCHAR *ProcessName) {
	//TODO:�жϵ�ǰ�����б�����û��ͬ������,����з��صڼ���ͬ������
	return 0;
}
char *RenameEx(char *tmp, int num) {
	//TODO:��tmp�е��ַ���ĩβ���#num
	return tmp;
}
int GetCPUUsage(TCHAR *ProcessName) {
	HQUERY          hQuery;
	HCOUNTER        *pCounterHandle;
	PDH_STATUS      pdhStatus;
	PDH_FMT_COUNTERVALUE  fmtValue;
	DWORD           ctrType;
	CHAR            szPathBuffer[MAX_PATH];
	int             RetVal = 0;
	pdhStatus = PdhOpenQuery(NULL, 0, &hQuery);//�򿪲�ѯ����

	pCounterHandle = (HCOUNTER *)GlobalAlloc(GPTR, sizeof(HCOUNTER));

	//�ϳɲ�ѯ�ַ���
	char *process = NULL;    //����֮��Ľ��̼�������
	if (strstr(ProcessName, ".exe") || strstr(ProcessName, ".EXE")) {
		//�������exe��β��ȥ����׺��
		int len = strlen(ProcessName) - 4;
		char * tmp = new char[len + 6];//Ϊ����#num�����ռ�
		memcpy(tmp, ProcessName, len);
		tmp[len] = 0;
		process = tmp;

		int num = ExistSameName(ProcessName);    //��Ҫ����
		if (num) {//����
				  //�����ͬ�����̣���ǰ���������ָ�Ϊ "ProcessName#num"
				  //�� MSDEV.EXE,���� MSDEV,MSDEV#1,MSDEV#2
			process = RenameEx(tmp, num);
		}
	}
	else if (_strcmpi(ProcessName, "System Idle Process")) {
		//�����ϵͳ���н��̣���ָ������������ΪIdle
		int len = strlen("Idle");
		char * tmp = new char[len + 1];
		strncpy(tmp, "Idle", len);
		tmp[len] = 0;
		process = tmp;
	}
	else {
		//��ΪSystem�Ľ���
		process = ProcessName;
	}

	sprintf(szPathBuffer, "//Process(%s)//%% Processor Time", process);
	pdhStatus = PdhAddCounter(hQuery, szPathBuffer, 0, pCounterHandle);
	pdhStatus = PdhCollectQueryData(hQuery);
	pdhStatus = PdhGetFormattedCounterValue(//��ȡ��������ǰֵ
		*pCounterHandle,//���������
		PDH_FMT_LONG | PDH_FMT_NOSCALE,      //format��ʽ
		&ctrType,       //��������
		&fmtValue);     //����ֵ
	if (pdhStatus == ERROR_SUCCESS) {
		//fmtValue.doubleValueΪ��Ҫ�Ľ��
		RetVal = fmtValue.longValue;// [type: double,long,string,large]
	}
	else {
		RetVal = 0;
	}
	pdhStatus = PdhCloseQuery(hQuery);//�رղ�ѯ���
	return RetVal;
}
int main() {
	setlocale(LC_ALL, "chs");
	pdhdump();
	system("pause");
	return 0;
}