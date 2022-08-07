#include "Framework.h"
#include "PathMgr.h"

PathMgr::PathMgr(const wstring& _contentPath)
{
	GetCurrentDirectory(MAX_PATH, m_contentPath);
	size_t sLen = wcslen(m_contentPath);
	
	while (m_contentPath[sLen] != L'\\')
		sLen--;
	
	m_contentPath[sLen] = L'\0';
	
	wstring temp = _contentPath;
	temp += L"\0";
	wcscat_s(m_contentPath, MAX_PATH, temp.c_str());
}

PathMgr::~PathMgr() = default;

wstring PathMgr::GetRelativePath(const wchar_t* _filePath)
{
	wstring filePath = _filePath;
	size_t absLen = wcslen(m_contentPath);
	size_t fullLen = filePath.length();

	return filePath.substr(absLen, fullLen - absLen);
}
