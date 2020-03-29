#ifndef _CORE_BLOCK_PF_HPP_
#define _CORE_BLOCK_PF_HPP_
#include "shell_fwd.hpp"
namespace shell_pointer
{
	template <typename core>
	struct core_block<core, false>
	{
		core_block();
		core_block(core* c_ptr);
		template <typename ... args_t>
		core_block(args_t&& ... args_v);
		~core_block();
		void reference();					// ����+1
		void dereference();					// ����-1

		core* core_object;
		std::size_t reference_count;
	};

	// Ĭ�Ϲ���
	template <typename core>
	core_block<core, false>::core_block()
		: reference_count(1),
		  core_object(nullptr)
	{}

	// ָ�빹��
	template <typename core>
	core_block<core, false>::core_block(core* c_ptr)
		: reference_count(1),
		  core_object(c_ptr)
	{}

	// ί�й���
	template <typename core>
	template <typename ... args_t>
	core_block<core, false>::core_block(args_t&& ... args_v)
		: core_block(new core(std::forward<args_t>(args_v) ...))
	{}

	// ��������
	template <typename core>
	core_block<core, false>::~core_block()
	{
		delete core_object;
	}

	template <typename core>
	void core_block<core, false>::reference()					// ����+1
	{
		reference_count += 1;
	}

	template <typename core>
	void core_block<core, false>::dereference()					// ����-1
	{
		reference_count -= 1;
	}
}
#endif // !_CORE_BLOCK_PF_HPP_