#ifndef HEAP_HH
#define HEAP_HH

#include <list>
#include <iostream>

#include "junk_base.h"

namespace lart
{

	struct heap {
		std::list<junk_base_ptr> junks;
	
		static heap* instance;
	
		static heap* get() {
			if (instance) return instance;
			return (instance = new heap());
		}
	
		template <class T>
		T add(T d) {
			junks.push_back(d);
			return d;
		}
	
		/**
			Note that this function has to be called in the same thread that writes commands, otherwise
			references might go away between the construction of a junk and binding it to a functor
			that uses it.
		*/
		void cleanup() {
			for (auto it = junks.begin(); it != junks.end();) {
				if (it->unique()) {
					it = junks.erase(it);
				} else {
					++it;
				}
			}	
		}
	
		~heap() { instance = 0; }
	
		protected:
			heap() { }
	};
}
	

#endif
