#ifndef _TINY_HPP_
#define _TINY_HPP_
#include <type_traits>
#include <exception>
namespace shell_pointer
{
	template <typename T,typename = std::void_t<>>
	struct has_is_modified : std::false_type
	{
	};

	template <typename T>
	struct has_is_modified<T,std::void_t<void(decltype(std::declval<T>().is_modified()))>> 
		: std::true_type
	{
	};
	
	// ���� shell_sync,�����ض�����޸ĺ�ͬ������
	template <typename T>
	void shell_sync(T* master, T* slave)
	{
		*master = *slave;
	}
}
#endif // !_TINY_HPP_
