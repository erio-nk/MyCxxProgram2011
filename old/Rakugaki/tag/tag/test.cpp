#include <cstdio>
#include <iostream>
#include <string>
#include <sstream>

std::string ConvertTags(const std::string& s)
{
	std::stringstream out;

	std::string::size_type b = 0;
	std::string::size_type p;
	
	while (true) {
		p = s.find("<", b);
		if (std::string::npos == p) {
			out << s.substr(b, s.size()-1);
			break;
		}
	
		out << s.substr(b, p-b);

		static const std::string TAG_PCNAME = "<PC:Name>";
		if (s.compare(p, TAG_PCNAME.length(), TAG_PCNAME) == 0) {
			out << "‚¨‚ê‚¾‚æ‚¨‚ê";
			b = p + TAG_PCNAME.length();
		} else {
			out << s.substr(b, 1);
			++b;
		}
	}

	return out.str();
}



int main()
{
	std::string s = "hoge<PC:Name>moke<PC:Name><aa";
	s = ConvertTags(s);

	std::cout << s << std::endl;

	getchar();
	
	return 0;
}

