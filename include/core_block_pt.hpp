#ifndef _CORE_BLOCK_PT_HPP_
#define _CORE_BLOCK_PT_HPP_
#include "shell_fwd.hpp"
namespace shell_pointer
{
	// ���ݿ��ƿ�
	// core_object ָ����ʵ�����ַ
	// reference_count , ��ʾ��ǰ���ƿ鱻���õĴ���
	template <typename core>
	struct core_block<core,true>
	{
		core_block();
		core_block(core* c_ptr);
		template <typename ... args_t>
		core_block(args_t&& ... args_v);
		~core_block();

		void create_slave();				
		bool is_modified();					
		void notify();						
		void sync();						
		void reference();					
		void dereference();					
		void post(shell_ptr<core>*);		

		std::size_t reference_count;		
		core* core_object;				
		core* slave_object;					
		shell_ptr<core>* last_access_object;
	};

	// Ĭ�Ϲ���
	template <typename core>
	core_block<core,true>::core_block()
		: reference_count(1),
		  core_object(nullptr),
		  slave_object(nullptr),
		  last_access_object(nullptr)
	{}

	// ָ�빹��
	template <typename core>
	core_block<core,true>::core_block(core* c_ptr)
		: reference_count(1),
		  core_object(c_ptr),
		  slave_object(nullptr),
		  last_access_object(nullptr)
	{}

	// ί�й���
	template <typename core>
	template <typename ... args_t>
	core_block<core,true>::core_block(args_t&& ... args_v)
		: core_block(new core(std::forward<args_t>(args_v) ...))
	{}

	// ��������
	template <typename core>
	core_block<core,true>::~core_block()
	{
		delete core_object;
		delete slave_object;
	}

	// ����������
	template <typename core>
	void core_block<core,true>::create_slave()
	{
		slave_object = new core(*core_object);
	}
	
	// �ϴβ����Ƿ��޸Ķ���
	template <typename core>
	bool core_block<core,true>::is_modified()
	{
		return core_object->is_modified();
	}

	// ֪ͨ���һ�β����Ķ�����ж�������
	template <typename core>
	void core_block<core,true>::notify()
	{
		last_access_object->alone();
	}
	
	// ͬ������,ͬ������������������
	// ����shell_sync����,���Ի�ø��õ�����
	template <typename core>
	void core_block<core,true>::sync()
	{
		shell_pointer::shell_sync(core_object, slave_object);
	}

	// ע����ʶ���
	template <typename core>
	void core_block<core,true>::post(shell_ptr<core>* s_ptr)
	{
		last_access_object = s_ptr;
	}

	// ���� +1
	template <typename core>
	void core_block<core,true>::reference()
	{
		reference_count += 1;
	}

	// ���� -1
	template <typename core>
	void core_block<core,true>::dereference()
	{
		reference_count -= 1;
	}
}
#endif // !_CORE_BLOCK_PT_HPP_
