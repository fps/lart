#ifndef HEAP_HH
#define HEAP_HH

#include <list>
#include <iostream>

#include <lart/junk.h>

namespace lart
{

	struct heap {
		std::list<junk_base_ptr> m_junk;
	
		heap() { }

		virtual ~heap() { }
	
		template <class T>
		std::shared_ptr<junk<T>> add(T t) {
			auto ret = std::make_shared<junk<T>>(t);
			m_junk.push_back(ret);
			return ret;
		}
	
		/**
			Note that this function has to be called in the same thread that writes commands, otherwise
			references might go away between the construction of a junk and binding it to a functor
			that uses it.
		*/
		virtual void cleanup() {
			for (auto it = m_junk.begin(); it != m_junk.end();) {
				if (it->unique()) {
					it = m_junk.erase(it);
				} else {
					++it;
				}
			}	
		}
	};
	
	typedef std::shared_ptr<heap> heap_ptr;
}
	

#endif
