#ifndef DISPOSABLE_BASE_HH
#define DISPOSABLE_BASE_HH

#include <memory>

/**
	@brief The base class for all disposable types.

	See disposable.h for more information.
*/
struct disposable_base {
	virtual ~disposable_base() { }
};

typedef std::shared_ptr<disposable_base> disposable_base_ptr;

#endif
