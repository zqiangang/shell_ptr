#include "../include/shell_ptr.hpp"
#include <iostream>
class test_shell
{
public:
	test_shell(int sv)
	{
		is_modi = false;
		v = sv;
	}

	~test_shell()
	{
		std::cout << "被析构" << std::endl;
	}

	test_shell(test_shell const& a)
	{
		is_modi = false;
		v = a.v;
	}

	test_shell& operator=(test_shell const& a)
	{
		is_modi = false;
		v = a.v;
		return *this;
	}

	bool is_modified()
	{	// 即用即销
		bool stat = is_modi;
		is_modi = false;
		return stat;
	}

	void seta(int sv)
	{
		v = sv;
		is_modi = true;
	}

	int geta()const
	{
		return v;
	}

private:
	bool is_modi;
	int v;
};

void example_has_func()
{
	shell_pointer::shell_ptr<test_shell> spt(1234);

	shell_pointer::shell_ptr<test_shell> cspt(spt);
	shell_pointer::shell_ptr<test_shell> cspt1(cspt);

	std::cout << "debug read : " << std::endl;
	std::cout << "spt : " << spt->geta() << std::endl;
	std::cout << "cspt : " << cspt->geta() << std::endl;
	std::cout << "cspt1 : " << cspt1->geta() << std::endl;

	std::cout << "debug write : " << std::endl;
	spt->seta(911199);
	std::cout << "spt : " << spt->geta() << std::endl;
	std::cout << "cspt : " << cspt->geta() << std::endl;
	std::cout << "cspt1 : " << cspt1->geta() << std::endl;
	cspt.reset(new test_shell(123));
	std::cout << "spt : " << spt->geta() << std::endl;
	std::cout << "cspt : " << cspt->geta() << std::endl;
	std::cout << "cspt1 : " << cspt1->geta() << std::endl;
	std::cout << cspt1.use_count() << std::endl;

	std::cout << "operator * :  before " << spt->geta() << std::endl;
	(*spt).seta(1290);
	std::cout << "operator * :  after " << spt->geta() << std::endl;
}