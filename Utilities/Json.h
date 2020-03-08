#pragma once

namespace Json
{
	void SetValue(Json::Value& parent, string name, string& value);

	template<typename T, typename = typename enable_if<
		is_same<T, bool>::value ||
		is_same<T, char>::value ||
		is_same<T, int>::value ||
		is_same<T, UINT>::value ||
		is_same<T, float>::value 
	>::type>
		void SetValue(Json::Value& parent, string name, T& value)
	{
		parent[name.c_str()] = value;
	}

	template<typename T, typename = typename enable_if<
		is_same<T, bool>::value ||
		is_same<T, char>::value ||
		is_same<T, int>::value ||
		is_same<T, UINT>::value ||
		is_same<T, float>::value 
	>::type>
		void SetValue(Json::Value& parent, string name, T&& value)
	{
		parent[name.c_str()] = value;
	}

	

	


	void GetValue(Json::Value& parent, string name, bool& value);
	void GetValue(Json::Value& parent, string name, char& value);
	void GetValue(Json::Value& parent, string name, int& value);
	void GetValue(Json::Value& parent, string name, UINT& value);
	void GetValue(Json::Value& parent, string name, float& value);
	void GetValue(Json::Value& parent, string name, string& value);

	void ReadData(wstring file, Json::Value* root);
	void WriteData(wstring file, Json::Value* root);
}