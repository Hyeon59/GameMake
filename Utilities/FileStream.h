#pragma once

enum EFileStream : UINT
{
	FILE_STREAM_READ = 1U << 0,
	FILE_STREAM_WRITE = 1U << 1,
};

class Rect;
class BaseMap;
class WayPoint;
class FileStream
{
public:
	FileStream(string path, UINT flags);
	~FileStream();

	void Close();

	template<typename T, typename = typename enable_if<
		is_same<T, bool>::value ||
		is_same<T, char>::value ||
		is_same<T, int>::value  ||
		is_same<T, UINT>::value ||
		is_same<T, float>::value ||
		is_same<T, D3DXVECTOR3>::value||
		is_same<T, D3DXVECTOR2>::value||
		is_same<T, D3DXMATRIX>::value||
		is_same<T, D3DXCOLOR>::value||
		is_same<T, VertexColor>::value||
		is_same<T, VertexTexture>::value 
	>::type>
	void Write(T value)
	{
		out.write((const char*)&value, sizeof(T));
	}

	void Write(Rect* rect);

	template<typename T, typename = typename enable_if<
		is_same<T, bool>::value ||
		is_same<T, char>::value ||
		is_same<T, int>::value ||
		is_same<T, UINT>::value ||
		is_same<T, float>::value ||
		is_same<T, D3DXVECTOR3>::value ||
		is_same<T, D3DXVECTOR2>::value ||
		is_same<T, D3DXMATRIX>::value ||
		is_same<T, D3DXCOLOR>::value ||
		is_same<T, VertexColor>::value ||
		is_same<T, VertexTexture>::value
	>::type>
	T Read()
	{
		T value;
		in.read((char*)&value, sizeof(T));
		return value;
	} 

	void Read(Rect* rect);
	void Write(string value);
	string Read();

	void Read2(Rect& rect);
	Rect* ReadRect();

	void Write(BaseMap* value);
	BaseMap* ReadBaseMap();

	void Read(WayPoint& wayPoint);
	void Write(WayPoint* wayPoint);
	WayPoint* ReadWayPoint();

	//새로운 Write함수
	void Write2(Rect* ract);
	Rect* ReadRect2();
	

private:
	ofstream out; //쓰기
	ifstream in;  //읽기

	UINT flags = 0;
};