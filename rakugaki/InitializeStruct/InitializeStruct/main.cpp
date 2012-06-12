struct DATA
{
	int _hoge;
	int _fuga;
	int _piyo;
	DATA()
		: _hoge(0)
		, _fuga(0)
		, _piyo(0)
	{}
};

int main()
{
	// error C2552
	DATA data[3] =
	{
		{ 1, 1, 1 },
		{ 2, 2, 2 },
		{ 3, 3, 3 },
	};

	return 0;
}