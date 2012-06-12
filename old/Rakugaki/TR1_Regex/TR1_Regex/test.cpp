#include <iostream>
#include <regex>

using namespace std;

bool MatchVarDeclaration(const string& s)
{
	tr1::smatch result;
	if (tr1::regex_match(s, result,
		tr1::regex("^(static\\s*|\\s*)(const\\s|\\s*)(\\w[\\w\\d\\s:]*)\\s+(\\w[\\w\\d]*);")))
	{
		std::string static_ = result.str(1);
		std::string const_ = result.str(2);
		std::string type_ = result.str(3);
		std::string name_ = result.str(4);

		cout << "static=" << static_ << endl;
		cout << "const=" << const_ << endl;
		cout << "type=" << type_ << endl;
		cout << "name=" << name_ << endl;

		return true;
	}
	return false;
}

int main()
{
	MatchVarDeclaration("static const unsigned int var_name;");

	cout << "--------" << endl;

	MatchVarDeclaration("int var_name;");

	return 0;
}
