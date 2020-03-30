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
	
	// 拷贝构造函数，放到派生类定义和声明
	template <typename object>
	shell_ptr<object, true>::shell_ptr(shell_ptr<object, true> const& sp)
	{
		cctor(sp);
	}

	// 可以装换类型的拷贝构造函数,添加引用
	template <typename object>
	template <typename cvt_object,typename>
	shell_ptr<object, true>::shell_ptr(shell_ptr<cvt_object, true> const& sp)
	{
		cctor(sp);
	}

	// 对数据资源进行 const 访问
	template <typename object>
	object const* shell_ptr<object, true>::operator->()const
	{
		return _block->core_object;
	}

	// 检查上次操作是否修改,并做出响应的操作
	// 对资源进行 non const 访问
	template <typename object>
	object* shell_ptr<object, true>::operator->()
	{
		bool tmp_state = _block->is_modified();		// is_modified 即读即销
		// 未被修改 或 被修改但只有自身在引用数据
		if (!tmp_state || (tmp_state && this->unique()))
		{
			_block->post(this);				// 注册本次访问
			return _block->core_object;
		}
		// 被修改，且引用数据取数量大于一个
		// 首先拷贝 _block ,防止当上一次修改是自身时.引发 slave_ 为空
		core_block<object, true>* tmp = _block;
		// 首先通知更新
		tmp->notify();
		// 回溯数据
		tmp->sync();
		// 注册本次访问
		tmp->post(this);
		// 返回自身当前数据区的主数据区
		return _block->core_object;
	}
	
	// 获取数据资源的 const 访问
	template <typename object>
	object const& shell_ptr<object, true>::operator*()const
	{
		return *operator->();
	}

	// 获取数据资源的 non const 访问
	template <typename object>
	object& shell_ptr<object, true>::operator*()
	{
		return *operator->();
	}

	// 独立操作,不检查引用数量,因为前面会对引用数量进行相关检查
	template <typename object>
	void shell_ptr<object, true>::alone()
	{
		// 首先克隆当前持有的 block 的主数据去
		object* clone_core = new object(*_block->core_object);
		// 然后解除对原数据区的引用
		_block->dereference();
		// 创建自己的数据区
		_block = new core_block<object, true>(clone_core);
	}
	
	// 派生类析构资源
	// 基类不对资源进行析构,由于数据块的析构操作不相同
	// 所以析构操作,由派生类根据所实例化的数据块类型
	// 编写自己的析构方法
	template <typename object>
	shell_ptr<object, true>::~shell_ptr()
	{
		dtor();
	}

	// 析构函数辅助函数
	template <typename object>
	void shell_ptr<object, true>::dtor()
	{
		// 首先检查,当前持有的数据区是否为空
		if (_block == nullptr)
			return;
		// 如果当前数据块仅自己持有,则析构
		if (this->unique())
		{
			delete _block;
			_block = nullptr;
			return;
		}
		// 否则,减少对数据块的引用
		_block->dereference();
		// 然后,检查当前引用数量是否多于1个
	#ifndef NON_DISCARD_SLAVE_WHEN_REFERENCE_UNIQUE
		if (this->unique())
		{
			delete _block->slave_object;
			_block->slave_object = nullptr;
		}
	#endif
	}
	
	// 专属的拷贝构造函数辅助函数
	template <typename object>
	void shell_ptr<object, true>::cctor(shell_ptr<object, true> const& sp)
	{
		// 复制数据区地址
		_block = sp._block;
		// 引用 +1
		_block->reference();
		// 检查备份数据区为空时, 启用备份数据区
		if (_block->slave_object == nullptr)
			_block->create_slave();
	}
}
#endif // !_SHELL_PTR_PT_HPP_
