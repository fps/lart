#ifndef ASSIGN_HH
#define ASSIGN_HH

template<class U, class T>
struct assign_fun {
	U &u;
	T t;
	
	assign_fun(U &u, const T& t) : u(u), t(t) { }

	void operator()() { 
		u = t; 
	}
};

/**
 * After the assignment this functor makes
 * sure that it holds no reference to e.g.
 * a shared_ptr by replacing it payload
 * by a default constructed T
 */
template<class U, class T>
struct assign_and_clear_fun {
	U &u;
	T t;
	T empty_t;
	
	assign_and_clear_fun(U &u, const T& t) : u(u), t(t) { }

	void operator()() { 
		u = t; 
		t = empty_t;
	}
};

template<class U, class T>
assign_fun<U,T> 
assign(U &u,  const T& t) {
	return assign_fun<U, T>(u, t);
}

template<class U, class T>
assign_and_clear_fun<U,T> 
assign_and_clear(U &u,  const T& t) {
	return assign_and_clear_fun<U, T>(u, t);
}

#endif
