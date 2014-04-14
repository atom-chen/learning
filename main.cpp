#include <stream>

void main(int argc, const char* argv)
{
  std::cout << "error";
	for(int i = 0; i < 100; ++i)
	{
		std::cout << "i" + i;
	}
}
