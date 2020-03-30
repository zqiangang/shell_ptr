#ifndef _CORE_BLOCK_PT_HPP_
#define _CORE_BLOCK_PT_HPP_
#include "shell_fwd.hpp"
namespace shell_pointer
{
	// 数据控制块
	// core_object 指向真实对象地址
	// reference_count , 表示当前控制块被引用的次数
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

	// 默认构造
	template <typename core>
	core_block<core,true>::core_block()
		: reference_count(1),
		  core_object(nullptr),
		  slave_object(nullptr),
		  last_access_object(nullptr)
	{}

	// 指针构造
	template <typename core>
	core_block<core,true>::core_block(core* c_ptr)
		: reference_count(1),
		  core_object(c_ptr),
		  slave_object(nullptr),
		  last_access_object(nullptr)
	{}

	// 委托构造
	template <typename core>
	template <typename ... args_t>
	core_block<core,true>::core_block(args_t&& ... args_v)
		: core_block(new core(std::forward<args_t>(args_v) ...))
	{}

	// 析构函数
	template <typename core>
	core_block<core,true>::~core_block()
	{
		delete core_object;
		delete slave_object;
	}

	// 创建备份区
	template <typename core>
	void core_block<core,true>::create_slave()
	{
		slave_object = new core(*core_object);
	}
	
	// 上次操作是否修改对象
	template <typename core>
	bool core_block<core,true>::is_modified()
	{
		return core_object->is_modified();
	}

	// 通知最近一次操作的对象进行独立操作
	template <typename core>
	void core_block<core,true>::notify()
	{
		last_access_object->alone();
	}
	
	// 同步操作,同步备份区到主数据区
	// 重载shell_sync函数,可以获得更好的性能
	template <typename core>
	void core_block<core,true>::sync()
	{
		shell_pointer::shell_sync(core_object, slave_object);
	}

	// 注册访问对象
	template <typename core>
	void core_block<core,true>::post(shell_ptr<core>* s_ptr)
	{
		last_access_object = s_ptr;
	}

	// 引用 +1
	template <typename core>
	void core_block<core,true>::reference()
	{
		reference_count += 1;
	}

	// 引用 -1
	template <typename core>
	void core_block<core,true>::dereference()
	{
		reference_count -= 1;
	}
}
#endif // !_CORE_BLOCK_PT_HPP_
