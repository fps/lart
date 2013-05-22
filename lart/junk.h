#ifndef DISPOSABLE_HH
#define DISPOSABLE_HH

#include <memory>

#include "junk_base.h"
#include "heap.h"

#include <iostream>

namespace lart
{
	/**
		@brief The base class for all things to be managed by the heap.
	
		See heap.h for more details.
	*/
	template <class T>
	struct junk : public junk_base {
		T t;
	
		/**
			The only way to create a junk<T> is by using this create() method
		*/
		static std::shared_ptr<junk<T>> create(const T& t = T()) 
		{
			return std::shared_ptr<junk<T>> (
				heap::get()->add(
					std::shared_ptr<junk<T>>(new junk<T>(t))
				)
			);
		}
	
	
		virtual ~junk() { /* std::cout << "~junk()" << std::endl; */ }
	
		private:
			/**
				Force creation on the heap by making the constructor private.
			*/
			junk(const T &t = T()) : t(t) { 
				// std::cout << "junk()" << std::endl;
			}
	};

	template<class T>
	std::shared_ptr<junk<T>> make_junk(const T& t = T())
	{
		return junk<T>::create(t);
	}
}


#endif
