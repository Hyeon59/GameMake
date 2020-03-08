#include "stdafx.h"
#include "Json.h"



void Json::GetValue(Json::Value & parent, string name, bool & value)
{
	value = parent[name.c_str()].asBool();
}

void Json::GetValue(Json::Value & parent, string name, char & value)
{
	value = (char)(parent[name.c_str()].asInt());
}

void Json::GetValue(Json::Value & parent, string name, int & value)
{
	value = parent[name.c_str()].asInt();
}

void Json::GetValue(Json::Value & parent, string name, UINT & value)
{
	value = parent[name.c_str()].asUInt();
}

void Json::GetValue(Json::Value & parent, string name, float & value)
{
	value = parent[name.c_str()].asFloat();
}

void Json::SetValue(Json::Value & parent, string name, string & value)
{
	parent[name.c_str()].append(value);
}

void Json::GetValue(Json::Value & parent, string name, string & value)
{
	value = parent.get(name.c_str(), "").asString();
	/*value = "";

	for (size_t i = 0; i < parent[name.c_str()].size(); i++)
		value.append(parent[name.c_str()][i].asString());*/
}

void Json::ReadData(wstring file, Json::Value * root)
{
	ifstream stream;

	stream.open(file);
	{
		Json::Reader reader;
		reader.parse(stream, *root);
	}
	stream.close();
}

void Json::WriteData(wstring file, Json::Value * root)
{
	ofstream stream;

	string temp = String::ToString(file);
	stream.open(temp);
	{
		Json::StyledWriter writer;
		stream << writer.write(*root);
	}
	stream.close();
}
