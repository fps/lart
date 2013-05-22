#include <memory>
#include <lart/ringbuffer.h>
#include <iostream>

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

int main()
{
	auto t = make_op([]() { std::cout << "hi" << std::endl; });
	auto rb = ringbuffer<op_base_ptr>(10);
	rb.write(t);

	auto t2 = rb.read();
	t2->exec();
}

