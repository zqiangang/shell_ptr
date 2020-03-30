#ifndef _SHELL_PTR_HPP_
#define _SHELL_PTR_HPP_
#include "shell_fwd.hpp"
#include "shell_ptr_pt.hpp"
#include "shell_ptr_pf.hpp"

// 定义 NON_DISCARD_SLAVE_WHEN_REFERENCE_UNIQUE
// 使在对象支持 is_modified 方法时,可以选择是否在
// shell_ptr 对象引用数降为 1 时,是否释放备份区资源
// #define NON_DISCARD_SLAVE_WHEN_REFERENCE_UNIQUE

#endif // !_SHELL_PTR_HPP
