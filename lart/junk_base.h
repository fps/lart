#ifndef LART_JUNK_BASE_HH
#define LART_JUNK_BASE_HH

#include <memory>

#include <lart/heap.h>

#include <iostream>

namespace lart
{
	/**
		@brief The base class for all junk types.
	
		See junk.h for more information.
	*/
	struct junk_base {
		virtual ~junk_base() { }
	};
	
	typedef std::shared_ptr<junk_base> junk_base_ptr;
}


#endif
