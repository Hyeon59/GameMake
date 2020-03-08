#pragma once

class Path
{
public:
	static bool ExistFile(string path);
	static bool ExistFile(wstring path);

	static bool ExistDirectory(string path);
	static bool ExistDirectory(wstring path);

	static string GetDirectoryName(string path);
	static wstring GetDirectoryName(wstring path);

	static string GetExtension(string path);
	static wstring GetExtension(wstring path);

	static string GetFileName(string path);
	static wstring GetFileName(wstring path);

	static string GetFileNameWithoutExtension(string path);
	static wstring GetFileNameWithoutExtension(wstring path);

	static string GetFileNameWithoutIndex(string path);
	static wstring GetFileNameWithoutIndex(wstring path);

	static void GetFiles(vector<string>* files, string path, string filter, bool bFindSubFolder);
	static void GetFiles(vector<wstring>* files, wstring path, wstring filter, bool bFindSubFolder);

	const static WCHAR* TextFilter;
	const static WCHAR* TileDataFilter;
	const static WCHAR* JsonDataFilter;

	static void OpenFileDialog(
		wstring file, const WCHAR* filter, 
		wstring folder, function<void(wstring)> func, HWND hwnd = NULL);

	static void SaveFileDialog(
		wstring file, const WCHAR* filter,
		wstring folder, function<void(wstring)> func, HWND hwnd = NULL);
};

