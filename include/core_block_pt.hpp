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

		void create_slave();				// 创建从对象
		bool is_modified();					// 是否修改
		void notify();						// 通知 shell_ptr 当前数据去已被修改
		void sync();						// 同步从数据区到主数据区
		void reference();					// 引用+1
		void dereference();					// 引用-1
		void post(shell_ptr<core>*);		// 注册访问对象

		std::size_t reference_count;		// 引用计数
		core* core_object;				// 优先修改 主数据去
		core* slave_object;					// 从数据去，用于恢复主数据去的修改
		shell_ptr<core>* last_access_object;// 最近一次访问对象
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

	template <typename core>
	void core_block<core,true>::create_slave()
	{
		slave_object = new core(*core_object);
	}

	template <typename core>
	bool core_block<core,true>::is_modified()
	{
		return core_object->is_modified();
	}

	template <typename core>
	void core_block<core,true>::notify()
	{
		last_access_object->alone();
	}

	template <typename core>
	void core_block<core,true>::sync()
	{
		shell_pointer::shell_sync(core_object, slave_object);
	}

	template <typename core>
	void core_block<core,true>::post(shell_ptr<core>* s_ptr)
	{
		last_access_object = s_ptr;
	}

	template <typename core>
	void core_block<core,true>::reference()
	{
		reference_count += 1;
	}

	template <typename core>
	void core_block<core,true>::dereference()
	{
		reference_count -= 1;
	}
}
#endif // !_CORE_BLOCK_PT_HPP_
