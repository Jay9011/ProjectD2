#pragma once
class PathMgr
{
public:
	PathMgr(const wstring& _contentPath);
	~PathMgr();

private:
	wchar_t m_contentPath[MAX_PATH]  = {};
	wchar_t m_relativePath[MAX_PATH] = {};

	wstring        GetRelativePath(const wchar_t* _filePath);
	
public:
	const wchar_t* GetContentPath() { return m_contentPath; }

};