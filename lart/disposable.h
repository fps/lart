#ifndef DISPOSABLE_HH
#define DISPOSABLE_HH

#include <memory>

#include "disposable_base.h"
#include "heap.h"

#include <iostream>

namespace lart
{
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
		static std::shared_ptr<disposable<T>> create(const T& t = T()) 
		{
			return std::shared_ptr<disposable<T>> (
				heap::get()->add(
					std::shared_ptr<disposable<T>>(new disposable<T>(t))
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

	template<class T>
	std::shared_ptr<disposable<T>> make(const T& t = T())
	{
		return disposable<T>::create(t);
	}
}


#endif
