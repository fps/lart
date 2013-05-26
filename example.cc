#include <memory>
#include <iostream>
#include <stdexcept>
#include <vector>
#include <functional>
#include <cmath>

#include <lart/heap.h>
#include <lart/ringbuffer.h>
#include <lart/junk.h>

#include <jack/jack.h>

#include <unistd.h>

using namespace lart;

/**
	An oscillator is represented by this struct..
*/
struct oscillator
{
	float m_time;
	float m_freq;
	
	//! Create an oscillator with a rather random frequency.
	oscillator() :
		m_time(0.0f),
		m_freq(220.0 + 880.0 * (float)rand() / (float)RAND_MAX)
	{
		
	}
};

typedef std::shared_ptr<junk<std::vector<oscillator>>> oscillators_ptr;

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
	lart::heap m_heap;
	
	//! A variable number of oscillators.
	oscillators_ptr m_oscillators;

	jack_client_t *m_jack_client;
	jack_port_t *m_jack_port;

	//! The ringbuffer we use to pass commands through
	//! to the realtime process thread.
	ringbuffer<std::function<void()>> m_commands;

	//! Initialize the list of oscillators with a junky vector.
	client() :
		m_oscillators(m_heap.add(std::vector<oscillator>())),
		m_commands(1024)
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
		//! Execute commands stored in the commands ringbuffer.
		while (m_commands.can_read())
		{
			//! snoop() returns a reference to the T on the front of the ringbuffer without copying.
			m_commands.snoop()();
			
			//! read_advance advances the read pointer of the ringbuffer without copying.
			m_commands.read_advance();
		}

		float * buffer = (float*)jack_port_get_buffer(m_jack_port, nframes);
		
		std::fill(buffer, buffer + nframes, 0.0f);
		
		//! Get a reference to the oscillators vector to save some typing..
		std::vector<oscillator> &oscillators = m_oscillators->t;
		
		unsigned number_of_oscillators = oscillators.size();
		float normalization = 1.0f/number_of_oscillators;
		
		jack_nframes_t samplerate = jack_get_sample_rate(m_jack_client);
		float delta_t = 1.0f/samplerate;
		
		for (size_t index = 0; index < oscillators.size(); ++index)
		{
			//! Get a reference to the oscillator so we don't call operator[] all the time.
			oscillator &o = oscillators[index];
			
			for (jack_nframes_t frame = 0; frame < nframes; ++frame)
			{
				buffer[frame] += normalization * sin(2.0 * M_PI * o.m_freq * o.m_time);
				o.m_time += delta_t;
			}
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
	for (unsigned index = 0; index < 30; ++index)
	{
		{
			std::vector<oscillator> v(3);
			auto o = c.m_heap.add(v);
			c.m_commands.write([o, &c]() mutable  { c.m_oscillators = o; o = oscillators_ptr(); });
		}

		usleep(1000000);
		std::cout << "cleanup" << std::endl;
		c.m_heap.cleanup();
		usleep(1000000);
	}
}

