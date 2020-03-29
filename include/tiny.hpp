#ifndef _TINY_HPP_
#define _TINY_HPP_
#include <type_traits>
#include <exception>
namespace shell_pointer
{
	class null_pointer_access : public std::exception
	{
	public:
		using std::exception::exception;
	};

	template <typename T,typename = std::void_t<>>
	struct has_is_modified : std::false_type
	{
	};

	template <typename T>
	struct has_is_modified<T,std::void_t<void(decltype(std::declval<T>().is_modified()))>> 
		: std::true_type
	{
	};
	// 重载 shell_sync 可以获得更好的性能
	template <typename T>
	void shell_sync(T* master, T* slave)
	{
		*master = *slave;
	}
}
#endif // !_TINY_HPP_
