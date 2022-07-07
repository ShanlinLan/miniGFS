#pragma once

// 2022/5/6

// 禁止某个类型的拷贝、复制、转移构造函数 
#define DISALLOW_COPY_MOVE_AND_ASSIGN(TypeName) TypeName(const TypeName&) = delete; TypeName(const TypeName&&) = delete;  TypeName& operator=(const TypeName&) = delete
