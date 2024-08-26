#pragma once
#include "StringConvertor.h"
#include <Windows.h>

class ErrorLogger {
public:
	static void Log(const std::string& message);
	static void Log(HRESULT hr, const std::string& message);
};