#ifndef _SHELL_FWD_HPP_
#define _SHELL_FWD_HPP_
#include "tiny.hpp"
namespace shell_pointer
{
	template <typename T, bool = has_is_modified<T>::value>
	class shell_ptr;

	template <typename core,bool>
	struct core_block;
}
#endif // !_SHELL_FWD_HPP_
