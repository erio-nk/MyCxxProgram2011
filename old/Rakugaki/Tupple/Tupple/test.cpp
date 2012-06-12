#include <stdio.h>
#include <string>


#include <assert.h>
#include <typeinfo>
#include <vector>

class Tupple
{
	std::vector<void*> _vals;
	std::vector<const char*> _types;

public:
	Tupple() {}

	template <typename T>
	Tupple(T t)
	{
		Add(t);
	}

	~Tupple()
	{
		for (int i = 0; i < _vals.size(); ++i) {
			delete _vals[i];
		}
		_vals.clear();
		_types.clear();
	}

	template <typename T>
	void Add(T val)
	{
		T* t = new T(val);
		_vals.push_back((void*)t);
		_types.push_back(typeid(T).name());
	}

	template <typename T>
	T& Get(int index)
	{
		if (strcmp(typeid(T).name(), _types[index]) != 0) {
			assert(0);
		}
		return *(T*)_vals[index];
	}
};


class Moke
{
	int val;
public:
	Moke(int x) : val (x) {}
};

int main()
{
	Tupple tupple;

	Tupple tupple2(std::string("mokeke"));

	{
		tupple.Add(1);
		
		std::string s("hoge");
		tupple.Add(s);

		tupple.Add(Moke(99));
	}

	{
		int x(tupple.Get<int>(0));

		std::string s(tupple.Get<std::string>(1));

		Moke m(tupple.Get<Moke>(2));

		x = tupple.Get<int>(2);
	}

	return 0;
}
