#include <memory>
#include <iostream>
#include <vector>

#include <lart/ringbuffer.h>
#include <lart/op.h>
#include <lart/junk.h>

#include <jack/jack.h>

using namespace lart;

/**
	A simple example that has some state that can be modified
*/
struct oscillator
{
	float m_freq;
};

typedef std::shared_ptr<junk<std::vector<oscillator>>> oscillators;

extern "C" 
{
	int process(jack_nframes_t nframes, void *arg);
}

struct client
{
	oscillators m_oscillators;

	jack_client_t *m_jack_client;

	client() :
		m_oscillators(make(std::vector<oscillator>()))
	{

	}
};

int main()
{
	auto t = lart::make_op([]() { std::cout << "hi" << std::endl; });
	auto rb = ringbuffer<lart::op_base_ptr>(10);
	rb.write(t);

	auto t2 = rb.read();
	t2->exec();
}

