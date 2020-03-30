#ifndef _SHELL_PTR_PF_HPP_
#define _SHELL_PTR_PF_HPP_
#include "shell_fwd.hpp"
#include "shell_base.hpp"
namespace shell_pointer
{
	template <typename object>
	class shell_ptr<object, false> : public _shell_base<object,false>
	{
	public:
		using _shell_base<object, false>::_shell_base;

		shell_ptr(shell_ptr<object, false> const& sp);
		template <typename cvt_object, typename = std::enable_if_t<std::is_convertible_v<cvt_object, object>> >
		shell_ptr(shell_ptr<cvt_object, false> const& sp);

		object const* read()const;
		object* write();
		~shell_ptr();
	protected:
		void dtor();
		void cctor(shell_ptr<object, false> const& sp);
	private:
		using _shell_base<object, false>::_block;
	};

	template <typename object>
	shell_ptr<object, false>::shell_ptr(shell_ptr<object, false> const& sp)
	{
		cctor(sp);
	}

	template <typename object>
	template <typename cvt_object, typename>
	shell_ptr<object, false>::shell_ptr(shell_ptr<cvt_object, false> const& sp)
	{
		cctor(sp);
	}


	template <typename object>
	shell_ptr<object, false>::~shell_ptr()
	{
		dtor();
	}

	template <typename object>
	void shell_ptr<object, false>::dtor()
	{
		// ���ȼ��,��ǰ���е��������Ƿ�Ϊ��
		if (_block == nullptr)
			return;
		if (this->unique())
		{
			delete _block;
			_block = nullptr;
			return;
		}
		_block->dereference();
	}

	template <typename object>
	void shell_ptr<object, false>::cctor(shell_ptr<object, false> const& sp)
	{
		if (sp.is_null())
			return;
		// ������������ַ
		_block = sp._block;
		// ���� +1
		_block->reference();
	}

	template <typename object>
	object const* shell_ptr<object, false>::read()const
	{
		if (this->is_null())
			return nullptr;
		return _block->core_object;
	}
	// д������Ĭ�ϵ������Ȼ�����д������
	// ���Կ�������ȥ����Ϊ�Լ���������������ȥ
	template <typename object>
	object* shell_ptr<object, false>::write()
	{
		if (this->is_null())
			return nullptr;
		object * clone_core = new object(*_block->core_object);
		_block->dereference();
		_block = new core_block<object, false>(clone_core);
		return _block->core_object;
	}
}
#endif // !_SHELL_PTR_PF_HPP_
