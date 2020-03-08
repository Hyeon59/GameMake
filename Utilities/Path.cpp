#include "stdafx.h"
#include "Path.h"

bool Path::ExistFile(string path)
{
	return ExistFile(String::ToWString(path));
}

bool Path::ExistFile(wstring path)
{
	DWORD fileValue = GetFileAttributes(path.c_str());
	//invalid_file_attribute = -1
	return fileValue < 0xFFFFFFFF;
}

bool Path::ExistDirectory(string path)
{
	return ExistDirectory(String::ToWString(path));
}

bool Path::ExistDirectory(wstring path)
{
	DWORD attribute = GetFileAttributes(path.c_str());

	BOOL temp = attribute != INVALID_FILE_ATTRIBUTES &&
		attribute & FILE_ATTRIBUTE_DIRECTORY;
	
	return temp == TRUE;
}

string Path::GetDirectoryName(string path)
{
	String::Replace(&path, "\\", "/");
	size_t index = path.find_last_of('/');
	return path.substr(0, index + 1);
}

wstring Path::GetDirectoryName(wstring path)
{
	String::Replace(&path, L"\\", L"/");
	size_t index = path.find_last_of('/');
	return path.substr(0, index + 1);
}

string Path::GetExtension(string path)
{
	String::Replace(&path, "\\", "/");
	size_t index = path.find_last_of('.');
	
	return path.substr(index + 1, path.length());
}

wstring Path::GetExtension(wstring path)
{
	String::Replace(&path, L"\\", L"/");
	size_t index = path.find_last_of('.');

	return path.substr(index + 1, path.length());
}

string Path::GetFileName(string path)
{
	String::Replace(&path, "\\", "/");
	size_t index = path.find_last_of('/');

	return path.substr(index + 1, path.length());
}

wstring Path::GetFileName(wstring path)
{
	String::Replace(&path, L"\\", L"/");
	size_t index = path.find_last_of('/');
	size_t index2 = path.find_last_of('.');

	return path.substr(index + 1, path.length());
}

string Path::GetFileNameWithoutExtension(string path)
{
	string str = GetFileName(path);
	size_t index = str.find_last_of('.');

	return str.substr(0, index);
}

wstring Path::GetFileNameWithoutExtension(wstring path)
{
	wstring str = GetFileName(path);
	size_t index = str.find_last_of('.');

	return str.substr(0, index);
}

string Path::GetFileNameWithoutIndex(string path)
{
	string str = GetFileName(path);
	size_t index = str.find_last_of('_');

	return str.substr(0, index);
}

wstring Path::GetFileNameWithoutIndex(wstring path)
{
	wstring str = GetFileName(path);
	size_t index = str.find_last_of('_');

	return str.substr(0, index);
}

void Path::GetFiles(vector<string>* files, string path, string filter, bool bFindSubFolder)
{
	vector<wstring> wFiles;
	wstring wPath = String::ToWString(path);
	wstring wFilter = String::ToWString(filter);
	GetFiles(&wFiles, wPath, wFilter, bFindSubFolder);

	for (wstring str : wFiles)
		files->push_back(String::ToString(str));
}

void Path::GetFiles(vector<wstring>* files, wstring path, wstring filter, bool bFindSubFolder)
{
	wstring file = path + filter;

	WIN32_FIND_DATA findData;
	HANDLE handle = FindFirstFile(file.c_str(), &findData);
	if (handle != INVALID_HANDLE_VALUE)
	{
		do
		{
			if (findData.dwFileAttributes == FILE_ATTRIBUTE_DIRECTORY)
			{
				if (bFindSubFolder == true && findData.cFileName[0] != '.')
				{
					wstring folder = path + wstring(findData.cFileName) + L"/";
					GetFiles(files, folder, filter, bFindSubFolder);
				}
			}
			else
			{
				wstring fileName = path + wstring(findData.cFileName);
				files->push_back(fileName);
			}
		} while (FindNextFile(handle, &findData));

		FindClose(handle);
	}
}

const WCHAR* Path::TextFilter = L"Text file\0*.txt";
const WCHAR* Path::TileDataFilter = L"Tile Data\0*.data";
const WCHAR* Path::JsonDataFilter = L"Json Data\0*.json";

void Path::OpenFileDialog(wstring file, const WCHAR * filter, wstring folder, function<void(wstring)> func, HWND hwnd)
{
	WCHAR name[255];
	wcscpy_s(name, file.c_str());

	wstring tempFolder = folder;
	OPENFILENAME ofn;
	ZeroMemory(&ofn, sizeof(OPENFILENAME));
	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.hwndOwner = hwnd;
	ofn.lpstrFilter = filter;
	ofn.lpstrFile = name;
	ofn.lpstrFileTitle = L"불러오기";
	ofn.nMaxFile = 255;
	ofn.lpstrInitialDir = tempFolder.c_str();
	ofn.Flags = OFN_NOCHANGEDIR;

	if (GetOpenFileName(&ofn) == TRUE)
	{
		if (func)
		{
			wstring loadName = name;
			String::Replace(&loadName, L"\\", L"/");
			func(loadName);
		}
	}
}

void Path::SaveFileDialog(wstring file, const WCHAR * filter, wstring folder, function<void(wstring)> func, HWND hwnd)
{
	WCHAR name[255];
	wcscpy_s(name, file.c_str());

	wstring tempFolder = folder;
	OPENFILENAME ofn;
	ZeroMemory(&ofn, sizeof(OPENFILENAME));
	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.hwndOwner = hwnd;
	ofn.lpstrFilter = filter;
	ofn.lpstrFile = name;
	ofn.lpstrFileTitle = L"저장하기";
	ofn.nMaxFile = 255;
	ofn.lpstrInitialDir = tempFolder.c_str();
	ofn.Flags = OFN_NOCHANGEDIR;

	if (GetSaveFileName(&ofn) == TRUE)
	{
		if (func)
		{
			wstring saveName = name;
			String::Replace(&saveName, L"\\", L"/");
			func(saveName);
		}
	}
}
