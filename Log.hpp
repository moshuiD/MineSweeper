#pragma once
#include<Windows.h>
#include<stdio.h>
#include<string>
#ifdef _DEBUG
#define Log(msg,...) Log::PrintLog(__FILE__,__LINE__,__func__,msg,##__VA_ARGS__);
#else
#define Log(msg,...) 
#endif

namespace Log 
{
	using std::string;
	inline string Format(const char* format, ...)
	{
		va_list args = nullptr;
		va_start(args, format);
		int Len = _vscprintf(format, args) + 1;
		std::string Buffer(Len, 0);
		if (vsprintf_s(&Buffer[0], Len, format, args) == -1) 
			Buffer.clear();
		if (*args) 
			va_end(args);
		return Buffer;
	}

	inline void PrintLog(const char* file,int line,const char* func,const char* format,...)
	{
		string f(file);
		SYSTEMTIME time{};
		GetLocalTime(&time);
		auto LogTime = Format("%02d:%02d:%02d", time.wHour, time.wMinute, time.wSecond);
		va_list marker = nullptr;
		va_start(marker, format);
		auto num_of_chars = _vscprintf(format, marker);
		if (num_of_chars == -1) 
			return;
		char szBuffer[1024];
		vsprintf_s(szBuffer, static_cast<size_t>(num_of_chars) + 1, format, marker);
		if (*marker)
			va_end(marker);
		printf("[%s] L:[%s:%d] F:[%s] Msg:%s \n", LogTime.c_str(),f.substr(f.find_last_of("\\")+1).c_str(), line, func, szBuffer);
	}
}