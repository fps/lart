#include <memory>
#include <iostream>

#include <lart/ringbuffer.h>
#include <lart/op.h>


int main()
{
	auto t = lart::make_op([]() { std::cout << "hi" << std::endl; });
	auto rb = ringbuffer<lart::op_base_ptr>(10);
	rb.write(t);

	auto t2 = rb.read();
	t2->exec();
}

