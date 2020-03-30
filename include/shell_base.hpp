#ifndef _BLOCK_BASE_HPP_
#define _BLOCK_BASE_HPP_
#include "shell_fwd.hpp"
namespace shell_pointer
{
	// shell_ptr �Ĺ��л���,�ṩ���캯����ͨ�÷���
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
	// ���캯��,���ڹ������ݿ���Դ
	// Ĭ�Ϲ��캯��,���������ݿ���Դ����
	template <typename object, bool stat>
	_shell_base<object, stat>::_shell_base()
		:_block(nullptr)
	{}

	// ʹ�ÿͻ��ṩ�Ķ���ָ�빹�����ݿ�
	template <typename object, bool stat>
	_shell_base<object, stat>::_shell_base(object* o_ptr)
		: _block(new core_block<object, stat>(o_ptr))
	{}

	// ʹ���û����͵Ĺ�������������ݿ� 
	template <typename object, bool stat>
	template <typename ... args_t, typename>
	_shell_base<object, stat>::_shell_base(args_t&& ... args_v)
		: _block(new core_block<object, stat>(std::forward<args_t>(args_v) ...))
	{}

	// reset ����
	template <typename object, bool stat>
	void _shell_base<object, stat>::reset(object* o_ptr)
	{
		shell_ptr<object>(o_ptr).swap(*this);
	}
	
	// ��ȡ���� const ָ��
	template <typename object, bool stat>
	object const* _shell_base<object, stat>::get() const
	{
		if (is_null())
			return nullptr;
		return _block->core_object;
	}
	
	// ��ȡ����ָ��
	template <typename object, bool stat>
	object* _shell_base<object, stat>::get()
	{
		if (is_null())
			return nullptr;
		return _block->core_object;
	}

	// ������� dtor �麯��������Դ
	// �����ฺ����д�Լ�����������
	// �����������ͷŻ�����Դ
	template <typename object, bool stat>
	_shell_base<object, stat>::~_shell_base()
	{
	}

	// ��ǰ������Ƿ�����һ������
	template <typename object, bool stat>
	bool _shell_base<object, stat>::unique()const
	{
		if (is_null())
			return false;
		return _block->reference_count == 1;
	}

	// �������ݿ��ַ
	template <typename object, bool stat>
	void _shell_base<object, stat>::swap(_shell_base<object, stat>& swp)
	{
		std::swap(_block, swp._block);
	}

	// ��ǰ shell_ptr ��������
	template <typename object, bool stat>
	std::size_t _shell_base<object, stat>::use_count()const
	{
		if (is_null())
			return 0;
		return _block->reference_count;
	}

	// ��ǰ�� shell_ptr �Ƿ�Ϊ��
	template <typename object, bool stat>
	bool _shell_base<object, stat>::is_null()const
	{
		return _block == nullptr;
	}
}

#endif // !_BLOCK_BASE_HPP_
