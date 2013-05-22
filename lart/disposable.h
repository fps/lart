#ifndef DISPOSABLE_HH
#define DISPOSABLE_HH

#include <memory>

#include "disposable_base.h"
#include "heap.h"

#include <iostream>

/**
	@brief The base class for all things to be managed by the heap.

	See heap.h for more details.
*/
template <class T>
struct disposable : public disposable_base {
	T t;

	/**
		The only way to create a disposable<T> is by using this create() method
	*/
	static disposable_base_ptr create(const T& t = T()) 
	{
		return std::make_shared (
			heap::get()->add(
				std::make_shared(new disposable<T>(t))
			)
		);
	}


	virtual ~disposable() { /* std::cout << "~disposable()" << std::endl; */ }

	private:
		/**
			Force creation on the heap by making the constructor private.
		*/
		disposable(const T &t = T()) : t(t) { 
			// std::cout << "disposable()" << std::endl;
		}
};


#endif
