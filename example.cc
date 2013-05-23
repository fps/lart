#include <memory>
#include <iostream>
#include <stdexcept>
#include <vector>
#include <functional>

#include <lart/ringbuffer.h>
#include <lart/op.h>
#include <lart/junk.h>

#include <jack/jack.h>

#include <unistd.h>

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
	jack_port_t *m_jack_port;

	ringbuffer<std::function<void()>> m_ops;

	client() :
		m_oscillators(make_junk(std::vector<oscillator>())),
		m_ops(4)
	{
		jack_status_t status;
		m_jack_client = jack_client_open("example", JackNullOption, &status);
		if (0 == m_jack_client)
		{
			throw std::runtime_error("oops");
		}

		m_jack_port = jack_port_register(m_jack_client, "out", JACK_DEFAULT_AUDIO_TYPE, JackPortIsOutput | JackPortIsTerminal, 0);
		jack_set_process_callback(m_jack_client, ::process, this);
		jack_activate(m_jack_client);
	}

	~client()
	{
		jack_client_close(m_jack_client);
	}

	int process(jack_nframes_t nframes)
	{
		while (m_ops.can_read())
		{
			m_ops.snoop()();
			m_ops.read();
		}

		return 0;
	}
};

extern "C" 
{
	int process(jack_nframes_t nframes, void *arg)
	{
		return ((client*)arg)->process(nframes);
	}
}

int main()
{
	client c;

	for (unsigned index = 0; index < 10; ++index)
	{
		std::cout << "---" << std::endl;
		std::vector<oscillator> v(10);
		{
			oscillators o = make_junk(v);
			c.m_ops.write([o, &c]() mutable  { c.m_oscillators = o; o = oscillators(); std::cout << ".\n"; });
		}
		std::cout << "+++" << std::endl;
		usleep(1000000);
		std::cout << "###" << std::endl;
		heap::get()->cleanup();
	}

	delete heap::get();
}

