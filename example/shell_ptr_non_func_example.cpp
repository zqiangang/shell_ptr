#include "../include/shell_ptr.hpp"
#include <iostream>

class test_false
{
public:

	void set_a(int va)
	{
		a = va;
	}

	void set_b(int vb)
	{
		b = vb;
	}

	int const get_a()const
	{
		return a;
	}

	int const get_b()const
	{
		return b;
	}

	void show()const
	{
		std::cout << "a : " << a << " b : " << b << std::endl;
	}
private:
	int a, b;
};

void example_no_func()
{
	shell_pointer::shell_ptr<test_false> spnf(new test_false());
	shell_pointer::shell_ptr<test_false> sspnf1(spnf);
	shell_pointer::shell_ptr<test_false> sspnf2(sspnf1);

	std::cout << "use count : " << std::endl;
	std::cout << "spnf : " << spnf.use_count() << std::endl;
	std::cout << "sspnf1 : " << sspnf1.use_count() << std::endl;
	std::cout << "sspnf2 : " << sspnf2.use_count() << std::endl;
	std::cout << "read a : " << std::endl;
	std::cout << "spnf : " << spnf.read()->get_a() << std::endl;
	std::cout << "sspnf1 : " << sspnf1.read()->get_a() << std::endl;
	std::cout << "sspnf2 : " << sspnf2.read()->get_a() << std::endl;

	std::cout << "spnf write set_a :" << std::endl;
	spnf.write()->set_a(22);
	std::cout << "spnf : " << spnf.read()->get_a() << " use_count : " << spnf.use_count() << std::endl;
	std::cout << "sspnf1 : " << sspnf1.read()->get_a() << " use_count : " << sspnf1.use_count() << std::endl;
	std::cout << "sspnf2 : " << sspnf2.read()->get_a() << " use_count : " << sspnf2.use_count() << std::endl;

	std::cout << "sspnf1 write get_a : " << std::endl;
	sspnf1.write()->get_a();
	std::cout << "spnf : " << spnf.read()->get_a() << " use_count : " << spnf.use_count() << std::endl;
	std::cout << "sspnf1 : " << sspnf1.read()->get_a() << " use_count : " << sspnf1.use_count() << std::endl;
	std::cout << "sspnf2 : " << sspnf2.read()->get_a() << " use_count : " << sspnf2.use_count() << std::endl;
}
