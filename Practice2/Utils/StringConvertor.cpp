#include "StringConvertor.h"

std::wstring StringConvertor::StringToWide(const std::string& str) {
	return std::wstring(str.begin(), str.end());
}