#ifndef LART_JUNK_HH
#define LART_JUNK_HH

#include <memory>

#include <lart/heap.h>
#include <lart/junk_base.h>

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
	
		typedef std::shared_ptr<junk<T>> junk_ptr;

		/**
			@brief The only way to create a junk<T> is by using this create() method
		*/
		static junk_ptr create(const T& t = T()) 
		{
			return  (
				heap::get()->add(
					junk_ptr(new junk<T>(t))
				)
			);
		}
	
	
		virtual ~junk() { /* std::cout << "~junk()" << std::endl; */ }
	
		private:
			/**
				@brief Force creation on the heap by making the constructor private.
			*/
			junk(const T &t = T()) : t(t) { 
				/* std::cout << "junk()" << std::endl; */
			}
	};

	/**
	 * @brief A utility function so the compiler can infer the type of T from the argument.
	 */
	template<class T>
	typename junk<T>::junk_ptr make_junk(const T& t = T())
	{
		return junk<T>::create(t);
	}
}


#endif
