#ifndef _SHELL_PTR_PT_HPP_
#define _SHELL_PTR_PT_HPP_
#include "shell_fwd.hpp"
#include "core_block.hpp"
#include "shell_base.hpp"
namespace shell_pointer
{
	template <typename object>
	class shell_ptr<object, true> : public _shell_base<object,true>
	{

	public:
		using _shell_base<object, true>::_shell_base;

		shell_ptr(shell_ptr<object, true> const& sp);
		template <typename cvt_object, typename = std::enable_if_t<std::is_convertible_v<cvt_object, object>> >
		shell_ptr(shell_ptr<cvt_object, true> const& sp);

		object const* operator->()const;
		object* operator->();
		object const& operator*()const;
		object& operator*();

		void alone();
		~shell_ptr();
	private:
		void cctor(shell_ptr<object, true> const& sp);
		void dtor();
	private:
		using _shell_base<object, true>::_block;
	};
	
	// �������캯�����ŵ������ඨ�������
	template <typename object>
	shell_ptr<object, true>::shell_ptr(shell_ptr<object, true> const& sp)
	{
		cctor(sp);
	}

	// ����װ�����͵Ŀ������캯��,�������
	template <typename object>
	template <typename cvt_object,typename>
	shell_ptr<object, true>::shell_ptr(shell_ptr<cvt_object, true> const& sp)
	{
		cctor(sp);
	}

	// ��������Դ���� const ����
	template <typename object>
	object const* shell_ptr<object, true>::operator->()const
	{
		return _block->core_object;
	}

	// ����ϴβ����Ƿ��޸�,��������Ӧ�Ĳ���
	// ����Դ���� non const ����
	template <typename object>
	object* shell_ptr<object, true>::operator->()
	{
		bool tmp_state = _block->is_modified();		// is_modified ��������
		// δ���޸� �� ���޸ĵ�ֻ����������������
		if (!tmp_state || (tmp_state && this->unique()))
		{
			_block->post(this);				// ע�᱾�η���
			return _block->core_object;
		}
		// ���޸ģ�����������ȡ��������һ��
		// ���ȿ��� _block ,��ֹ����һ���޸�������ʱ.���� slave_ Ϊ��
		core_block<object, true>* tmp = _block;
		// ����֪ͨ����
		tmp->notify();
		// ��������
		tmp->sync();
		// ע�᱾�η���
		tmp->post(this);
		// ��������ǰ����������������
		return _block->core_object;
	}
	
	// ��ȡ������Դ�� const ����
	template <typename object>
	object const& shell_ptr<object, true>::operator*()const
	{
		return *operator->();
	}

	// ��ȡ������Դ�� non const ����
	template <typename object>
	object& shell_ptr<object, true>::operator*()
	{
		return *operator->();
	}

	// ��������,�������������,��Ϊǰ������������������ؼ��
	template <typename object>
	void shell_ptr<object, true>::alone()
	{
		// ���ȿ�¡��ǰ���е� block ��������ȥ
		object* clone_core = new object(*_block->core_object);
		// Ȼ������ԭ������������
		_block->dereference();
		// �����Լ���������
		_block = new core_block<object, true>(clone_core);
	}
	
	// ������������Դ
	// ���಻����Դ��������,�������ݿ��������������ͬ
	// ������������,�������������ʵ���������ݿ�����
	// ��д�Լ�����������
	template <typename object>
	shell_ptr<object, true>::~shell_ptr()
	{
		dtor();
	}

	// ����������������
	template <typename object>
	void shell_ptr<object, true>::dtor()
	{
		// ���ȼ��,��ǰ���е��������Ƿ�Ϊ��
		if (_block == nullptr)
			return;
		// �����ǰ���ݿ���Լ�����,������
		if (this->unique())
		{
			delete _block;
			_block = nullptr;
			return;
		}
		// ����,���ٶ����ݿ������
		_block->dereference();
		// Ȼ��,��鵱ǰ���������Ƿ����1��
	#ifndef NON_DISCARD_SLAVE_WHEN_REFERENCE_UNIQUE
		if (this->unique())
		{
			delete _block->slave_object;
			_block->slave_object = nullptr;
		}
	#endif
	}
	
	// ר���Ŀ������캯����������
	template <typename object>
	void shell_ptr<object, true>::cctor(shell_ptr<object, true> const& sp)
	{
		// ������������ַ
		_block = sp._block;
		// ���� +1
		_block->reference();
		// ��鱸��������Ϊ��ʱ, ���ñ���������
		if (_block->slave_object == nullptr)
			_block->create_slave();
	}
}
#endif // !_SHELL_PTR_PT_HPP_
