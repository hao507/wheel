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
//一.性能计数器简介：
//系统性能计数器？也许好多朋友没有用过吧！（献丑了）此物可谓功能强大，顾名思义就是可以统计 Windows 系统各项性能指标的东西。在 Windows 2000 及以上系统中，如果是默认安装就会自带性能计数器程序，大家可以在 “控制面板 -〉管理 -〉性能”中看到该程序。
//Windows 系统性能计数器可以实时跟踪上百项系统性能指标，在该系统程序的绘图界面上鼠标右击，选择“添加计数器”就有可能看到所有可统计的项目。常常在论坛里看到一些新朋友问如何自己实现任务管理器，及如何实时获得每个进程的CPU使用率，内存使用...等等，那么我想使用系统性能计数器应该是最佳选择。言归正传，实现方法，请看下文。
//二.具体实现：
//相关API简介及事例代码：
bool pdhdump() {
	//1.PdhOpenQuery：打开计数器
	PDH_STATUS pdhStatus;
	//  HQUERY hQuery = NULL;
	//  HCOUNTER pCounterHandle = NULL;
	//  pdhStatus = PdhOpenQuery(0, 0, &hQuery);
	//  if (pdhStatus != ERROR_SUCCESS) return false;
	//  //分配计数器句柄空间
	//  pCounterHandle = (HCOUNTER *)GlobalAlloc(GPTR, sizeof(HCOUNTER));
	//  if (pCounterHandle == NULL) return false;
	//2.PdhCloseQuery：关闭计数器
	//  pdhStatus = PdhCloseQuery(hQuery);
	//  if (pdhStatus != ERROR_SUCCESS) return false;
	//3.PdhEnumObjects：枚举计数项目，该函数有6个参数（详细请看MSDN）
	//原型：
	//PDH_STATUS PdhEnumObjects(
	//      LPCTSTR szDataSource,    // 必须为 NULL
	//      LPCTSTR szMachineName,   // 机器名，如果为本机可以为NULL
	//      LPTSTR mszObjectList,    // 接收全部可用计数项目的缓冲区
	//      LPDWORD pcchBufferLength,  // 缓冲去大小(如果为 0，则该值返回所需大小)
	//      DWORD dwDetailLevel,    // 获取信息的级别
	//      BOOL bRefresh        // 一般设置为 TRUE
	//      );
	//举例：
	LPTSTR lpcsMachineName = NULL;
	LPTSTR lpsObjectListBuffer = NULL;
	DWORD dwObjectListSize = 0;

	//第一步先把缓冲去大小置为0，这样可获得所需缓冲区大小
	pdhStatus = PdhEnumObjects(
		NULL,
		lpcsMachineName,
		lpsObjectListBuffer,
		&dwObjectListSize,
		PERF_DETAIL_WIZARD,
		TRUE);
	if (pdhStatus != ERROR_SUCCESS && pdhStatus != PDH_MORE_DATA) return false;
	//得到缓冲区大小后，分配缓存区内存
	lpsObjectListBuffer = (LPTSTR)malloc(dwObjectListSize + 1);
	if (lpsObjectListBuffer == NULL) return false;
	//第二步在此调用枚举函数真正开始枚举计数项目
	pdhStatus = PdhEnumObjects(
		NULL,
		lpcsMachineName,
		lpsObjectListBuffer,
		&dwObjectListSize,
		PERF_DETAIL_WIZARD,
		TRUE);
	if (pdhStatus != ERROR_SUCCESS) return false;
	//保存缓冲区地址
	LPTSTR lpsthisObject = NULL;
	lpsthisObject = lpsObjectListBuffer;
	//打印所有可用计数项目
	if (dwObjectListSize>0)
		for (; *lpsthisObject != 0; lpsthisObject += (strlen(lpsthisObject) + 1)) {
			printf("Object:%s\n", lpsthisObject);
			//4.PdhEnumObjectItems：枚举计数器及实例
			//原型：
			//PDH_STATUS PdhEnumObjectItems(
			//       LPCTSTR szDataSource,        // 必须为NULL
			//       LPCTSTR szMachineName,       // 机器名，如果为本机可以为NULL
			//       LPCTSTR szObjectName,        // 计数项目（通过PdhEnumObjects函数可获得所有可用项目）
			//       LPTSTR mszCounterList,       // 计数器缓冲区
			//       LPDWORD pcchCounterListLength,   // 计数器缓冲区大小
			//       LPTSTR mszInstanceList,       // 计数实例缓冲区
			//       LPDWORD pcchInstanceListLength,   // 计数实例缓冲区大小
			//       DWORD dwDetailLevel,        // 获取信息的级别
			//       DWORD dwFlags            // 0
			//       );
			//举例：该函数的方法同上一函数（PdhEnumObjects），具体请看MSDN或本文附带的测试工程代码
			LPTSTR mszCounterList = NULL;
			LPTSTR mszInstanceList = NULL;
			DWORD dwCounterListLength = 0;
			DWORD dwInstanceListLength = 0;
			//第一步先把缓冲去大小置为0，这样可获得所需缓冲区大小
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
			//得到缓冲区大小后，分配缓存区内存
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
//5.PdhAddCounter：添加计数器
//统计感兴趣的系统信息时，必须先将对应的计数器添加进来
//原型：
//PDH_STATUS PdhAddCounter(
//     PDH_HQUERY hQuery,     // 为PdhOpenQuery打开的句柄
//     LPCTSTR szFullCounterPath, // 计数器路径(最大长度为 PDH_MAX_COUNTER_PATH)
//     DWORD_PTR dwUserData,    // 置为 0
//     PDH_HCOUNTER* phCounter   // 计数器句柄空间(本文中在PdhOpenQuery函数后已分配)
//     );
//举例：
// 已获取winlogon.exe进程的CPU使用率为例
// 通过枚举并查看计数项目说明可以知道 process 项目是和进程有关的项目
// 再通过枚举计数器和事例并查看说明可以知道 process 项目下的% Processor Time计数器是关于进程CPU使率的
// 最后在计数器事例中看到winlogon进程(表明该进程正在运行)
//  pdhStatus = PdhAddCounter (hQuery, "//process(winlogon)//% Processor Time", 0, pCounterHandle);
//  if (pdhStatus != ERROR_SUCCESS) return false;
//提示：有些计数器没有实例，比如：要得到系统自启动到现在所运行的秒数，那么该计数项目为System,计数器为System Up Time，计数器实例为NULL，这时的计数器路径为"/System/System Up Time"
//6.PdhCollectQueryData： 准备获取当前数据
//举例:
//  pdhStatus = PdhCollectQueryData(hQuery);
//  if (pdhStatus != ERROR_SUCCESS)return false;
//7.PdhGetFormattedCounterValue：得到数据
//举例：
//  pdhStatus = PdhGetFormattedCounterValue (pCounterHandle, PDH_FMT_DOUBLE,
//             &dwctrType, &fmtValue);
//  if (pdhStatus != ERROR_SUCCESS) return false;
//  // PDH_FMT_DOUBLE表示返回double型数据，当然还可以返回int等类型数据，请查MSDN
//  // 获取下一时刻数据
//  pdhStatus = PdhCollectQueryData(hQuery);
//  if (pdhStatus != ERROR_SUCCESS)return false;
//提示：pCounterHandle为PdhAddCounter得到的句柄，可以不同的pCounterHandle获得不同计数值
//8.PdhRemoveCounter：移出计数器
//不想获取某项计数值时，应该移出该计数器，已节省资源
//举例：
//  if (PdhRemoveCounter(pdhCouner) != ERROR_SUCCESS) return false;
//该函数参数为计数器句柄
//至此，如果使用计数器实时跟踪系统信息已经讲解完毕。如还有不明白的朋友请详细察看MSDN或与Email：wlzqin@sina.com 或 QQ：8573980联系。

//三.最后再附带介绍一个相关API PdhGetCounterInfo,这个API与使用计数器并无关系，但是，它可以让你明白你所感兴趣的计数器的路径。既它可以得到每个计数器的项目的描述信息（可是中文的哦！）
//举例：
//  //以获取系统自启动到现在所运行的秒数为例
//  PDH_COUNTER_INFO pdhCounterInfo;
//  DWORD dwCounterBuffsize;
//  //添加计数器
//  pdhStatus = PdhAddCounter(hQuery, "//System//System Up Time", 0, pCounterHandle);
//  if (pdhStatus != ERROR_SUCCESS) return false;
//  //得到缓冲区大小
//  pdhStatus = PdhGetCounterInfo(*pCounterHandle, TRUE, &dwCounterBuffsize, NULL);
//  if (pdhStatus != ERROR_SUCCESS && pdhStatus != PDH_MORE_DATA) return false;
//  //设置缓冲区
//  BYTE  * byCounterBuff = (BYTE *)malloc(dwCounterBuffsize);
//  //获取信息
//  pdhStatus = PdhGetCounterInfo (*pCounterHandle, TRUE, &dwCounterBuffsize, (PPDH_COUNTER_INFO)byCounterBuff);
//  if (pdhStatus != ERROR_SUCCESS) return false;
//  pdhCounterInfo = * (PPDH_COUNTER_INFO)byCounterBuff;
//  //打印得到的信息
//  printf(pdhCounterInfo->szExplainText);
//  //以上打印得信息类似："System Up Time 指计算机自上次启动后已经运行的时间(用秒计算)。这个计数值显示启动时间和当前时间之差。"
//全部介绍完毕，希望本文能对大家有所帮助，祝大家愉快。
//example code:
int ExistSameName(TCHAR *ProcessName) {
	//TODO:判断当前进程列表中有没有同名进程,如果有返回第几个同名进程
	return 0;
}
char *RenameEx(char *tmp, int num) {
	//TODO:在tmp中的字符串末尾添加#num
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
	pdhStatus = PdhOpenQuery(NULL, 0, &hQuery);//打开查询对象

	pCounterHandle = (HCOUNTER *)GlobalAlloc(GPTR, sizeof(HCOUNTER));

	//合成查询字符串
	char *process = NULL;    //处理之后的进程计数器名
	if (strstr(ProcessName, ".exe") || strstr(ProcessName, ".EXE")) {
		//如果是以exe结尾，去掉后缀名
		int len = strlen(ProcessName) - 4;
		char * tmp = new char[len + 6];//为后面#num留出空间
		memcpy(tmp, ProcessName, len);
		tmp[len] = 0;
		process = tmp;

		int num = ExistSameName(ProcessName);    //需要计数
		if (num) {//重名
				  //如果有同名进程，当前计数器名字改为 "ProcessName#num"
				  //如 MSDEV.EXE,则有 MSDEV,MSDEV#1,MSDEV#2
			process = RenameEx(tmp, num);
		}
	}
	else if (_strcmpi(ProcessName, "System Idle Process")) {
		//如果是系统空闲进程，可指定计数器名字为Idle
		int len = strlen("Idle");
		char * tmp = new char[len + 1];
		strncpy(tmp, "Idle", len);
		tmp[len] = 0;
		process = tmp;
	}
	else {
		//名为System的进程
		process = ProcessName;
	}

	sprintf(szPathBuffer, "//Process(%s)//%% Processor Time", process);
	pdhStatus = PdhAddCounter(hQuery, szPathBuffer, 0, pCounterHandle);
	pdhStatus = PdhCollectQueryData(hQuery);
	pdhStatus = PdhGetFormattedCounterValue(//获取计数器当前值
		*pCounterHandle,//计数器句柄
		PDH_FMT_LONG | PDH_FMT_NOSCALE,      //format格式
		&ctrType,       //控制类型
		&fmtValue);     //返回值
	if (pdhStatus == ERROR_SUCCESS) {
		//fmtValue.doubleValue为所要的结果
		RetVal = fmtValue.longValue;// [type: double,long,string,large]
	}
	else {
		RetVal = 0;
	}
	pdhStatus = PdhCloseQuery(hQuery);//关闭查询句柄
	return RetVal;
}
int main() {
	setlocale(LC_ALL, "chs");
	pdhdump();
	system("pause");
	return 0;
}