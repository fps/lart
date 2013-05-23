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
	An oscillator is represented by this struct XD
*/
struct oscillator
{
	float m_freq;
};

typedef std::shared_ptr<junk<std::vector<oscillator>>> oscillators;

extern "C" 
{
	//! Boring!
	int process(jack_nframes_t nframes, void *arg);
}

/**
	A very simple jack client for demonstrating how to
	replace state and how to modify it without having to
	care too much about memory.
*/
struct client
{
	//! A variable number of oscillators.
	oscillators m_oscillators;

	jack_client_t *m_jack_client;
	jack_port_t *m_jack_port;

	//! The ringbuffer we use to pass commands through
	//! to the realtime process thread.
	ringbuffer<std::function<void()>> m_ops;

	//! Initialize the list of oscillators with a junky vector.
	client() :
		m_oscillators(make_junk(std::vector<oscillator>())),
		m_ops(1024)
	{
		std::cout << "client: hi" << std::endl;

		//! Boring :D
		jack_status_t status;
		m_jack_client = jack_client_open("example", JackNullOption, &status);
		if (0 == m_jack_client)
		{
			throw std::runtime_error("oops");
		}

		m_jack_port = jack_port_register(m_jack_client, "out", JACK_DEFAULT_AUDIO_TYPE, JackPortIsOutput | JackPortIsTerminal, 0);
		jack_set_process_callback(m_jack_client, ::process, this);

		std::cout << "client: activating" << std::endl;
		jack_activate(m_jack_client);
	}

	~client()
	{
		std::cout << "client: closing client" << std::endl;
		jack_client_close(m_jack_client);
		std::cout << "client: bye" << std::endl;
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
	//! Super boring!
	int process(jack_nframes_t nframes, void *arg)
	{
		return ((client*)arg)->process(nframes);
	}
}

int main()
{
	client c;

	/**
		Repeatedly change the state drastically by creating some
		new objects. These replace the old objects in the client.
	*/
	for (unsigned index = 0; index < 10; ++index)
	{
		std::vector<oscillator> v(10);
		{
			oscillators o = make_junk(v);
			c.m_ops.write([o, &c]() mutable  { c.m_oscillators = o; o = oscillators(); });
		}
		usleep(1000000);
		std::cout << "cleanup" << std::endl;
		heap::get()->cleanup();
	}

	delete heap::get();
}

