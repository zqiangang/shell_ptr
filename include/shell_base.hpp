#ifndef _BLOCK_BASE_HPP_
#define _BLOCK_BASE_HPP_
#include "shell_fwd.hpp"
namespace shell_pointer
{
	template <typename object, bool stat>
	class _shell_base
	{

	public:
		// 构造函数
		_shell_base();
		_shell_base(object* o_ptr);
		template <typename ... args_t, typename = std::enable_if_t<std::is_constructible_v<object, args_t...>> >
		_shell_base(args_t&& ... args_v);
		// 拷贝构造函数
		
		void reset(object* o_ptr = nullptr);		// 依赖 构造函数和 swap
		void swap(_shell_base<object, stat>& swp);	// 以来 std::swap
		object const* get() const;					// 依赖于 core_object
		object* get();								// 依赖于 core_object
		bool unique()const;							// 依赖于 reference_count
		std::size_t use_count()const;				// 依赖于 reference_count
		// 析构函数
		virtual ~_shell_base();
		bool is_null()const;

	protected:
		core_block<object, stat>* _block;
	};

	template <typename object, bool stat>
	_shell_base<object, stat>::_shell_base()
		:_block(nullptr)
	{}

	template <typename object, bool stat>
	_shell_base<object, stat>::_shell_base(object* o_ptr)
		: _block(new core_block<object, stat>(o_ptr))
	{}

	template <typename object, bool stat>
	template <typename ... args_t, typename>
	_shell_base<object, stat>::_shell_base(args_t&& ... args_v)
		: _block(new core_block<object, stat>(std::forward<args_t>(args_v) ...))
	{}


	template <typename object, bool stat>
	void _shell_base<object, stat>::reset(object* o_ptr)
	{
		shell_ptr<object>(o_ptr).swap(*this);
	}

	template <typename object, bool stat>
	object const* _shell_base<object, stat>::get() const
	{
		if (is_null())
			return nullptr;
		return _block->core_object;
	}

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


	template <typename object, bool stat>
	bool _shell_base<object, stat>::unique()const
	{
		if (is_null())
			return false;
		return _block->reference_count == 1;
	}


	template <typename object, bool stat>
	void _shell_base<object, stat>::swap(_shell_base<object, stat>& swp)
	{
		std::swap(_block, swp._block);
	}

	template <typename object, bool stat>
	std::size_t _shell_base<object, stat>::use_count()const
	{
		if (is_null())
			return 0;
		return _block->reference_count;
	}

	template <typename object, bool stat>
	bool _shell_base<object, stat>::is_null()const
	{
		return _block == nullptr;
	}
}

#endif // !_BLOCK_BASE_HPP_
