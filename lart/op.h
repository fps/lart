#include <memory>
#include <lart/ringbuffer.h>
#include <iostream>

namespace lart
{
	
	struct op_base 
	{
		virtual ~op_base() { }
		virtual void exec() = 0;
	};
	
	typedef std::shared_ptr<op_base> op_base_ptr;
	
	template<class T> 
	struct op : op_base
	{
		T m_t;
		op(T t) : m_t(t) { }
		virtual void exec() { m_t(); }
	};
	
	
	template<class T>
	op_base_ptr make_op(T t)
	{
		return op_base_ptr(new op<T>(t));
	}
	
}

