#ifndef _BLOCK_BASE_HPP_
#define _BLOCK_BASE_HPP_
#include "shell_fwd.hpp"
namespace shell_pointer
{
	// shell_ptr 的公有基类,提供构造函数和通用方法
	template <typename object, bool stat>
	class _shell_base
	{

	public:
		_shell_base();
		_shell_base(object* o_ptr);
		template <typename ... args_t, typename = std::enable_if_t<std::is_constructible_v<object, args_t...>> >
		_shell_base(args_t&& ... args_v);
		void reset(object* o_ptr = nullptr);		
		void swap(_shell_base<object, stat>& swp);	
		object const* get() const;					
		object* get();								
		bool unique()const;							
		std::size_t use_count()const;				
		virtual ~_shell_base();
		bool is_null()const;

	protected:
		core_block<object, stat>* _block;
	};
	// 构造函数,用于构造数据块资源
	// 默认构造函数,不尽兴数据块资源构造
	template <typename object, bool stat>
	_shell_base<object, stat>::_shell_base()
		:_block(nullptr)
	{}

	// 使用客户提供的对象指针构造数据块
	template <typename object, bool stat>
	_shell_base<object, stat>::_shell_base(object* o_ptr)
		: _block(new core_block<object, stat>(o_ptr))
	{}

	// 使用用户类型的构造参数构造数据块 
	template <typename object, bool stat>
	template <typename ... args_t, typename>
	_shell_base<object, stat>::_shell_base(args_t&& ... args_v)
		: _block(new core_block<object, stat>(std::forward<args_t>(args_v) ...))
	{}

	// reset 函数
	template <typename object, bool stat>
	void _shell_base<object, stat>::reset(object* o_ptr)
	{
		shell_ptr<object>(o_ptr).swap(*this);
	}
	
	// 获取数据 const 指针
	template <typename object, bool stat>
	object const* _shell_base<object, stat>::get() const
	{
		if (is_null())
			return nullptr;
		return _block->core_object;
	}
	
	// 获取数据指针
	template <typename object, bool stat>
	object* _shell_base<object, stat>::get()
	{
		if (is_null())
			return nullptr;
		return _block->core_object;
	}

	// 负责调用 dtor 虚函数析构资源
	// 派生类负责重写自己的析构方法
	// 派生类无需释放基类资源
	template <typename object, bool stat>
	_shell_base<object, stat>::~_shell_base()
	{
	}

	// 当前对象的是否因有一个引用
	template <typename object, bool stat>
	bool _shell_base<object, stat>::unique()const
	{
		if (is_null())
			return false;
		return _block->reference_count == 1;
	}

	// 交换数据块地址
	template <typename object, bool stat>
	void _shell_base<object, stat>::swap(_shell_base<object, stat>& swp)
	{
		std::swap(_block, swp._block);
	}

	// 当前 shell_ptr 的引用数
	template <typename object, bool stat>
	std::size_t _shell_base<object, stat>::use_count()const
	{
		if (is_null())
			return 0;
		return _block->reference_count;
	}

	// 当前的 shell_ptr 是否为空
	template <typename object, bool stat>
	bool _shell_base<object, stat>::is_null()const
	{
		return _block == nullptr;
	}
}

#endif // !_BLOCK_BASE_HPP_
