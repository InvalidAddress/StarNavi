//==============================================================================
// Date Created:		17 April 2011
// Last Updated:		17 April 2011
//
// File name:			Functors.h
// Programmer:			Matthew Hydock
//
// File description:	Classes to facilitate the creation of "functors", or
//						objects that act like functions. Useful for when you
//						want a function pointer to point to a member function
//						of an object.
//
// Original code found at: http://www.newty.de/fpt/functor.html
//==============================================================================

// abstract base class
class AbstractFunctor
{
	public:

	// two possible functions to call member function. virtual cause derived
	// classes will use a pointer to an object and a pointer to a member function
	// to make the function call
	virtual void operator()() = 0;		// call using operator
	virtual void Call() = 0;			// call using function
};


// derived template class
template <class TClass> class Functor:public AbstractFunctor
{
	private:
		void (TClass::*fpt)();			// pointer to member function
		TClass* pt2Object;				// pointer to object

	public:
		// constructor - takes pointer to an object and pointer to a member and stores
		// them in two private variables
		Functor(TClass* _pt2Object, void (TClass::*_fpt)())
		{ pt2Object = _pt2Object;  fpt=_fpt; };

		// override operator "()"
		virtual void operator()()
		{ (*pt2Object.*fpt)();};		// execute member function

		// override function "Call"
		virtual void Call()
		{ (*pt2Object.*fpt)();};		// execute member function
};

class NullFunctor:public AbstractFunctor
{
	public:

	void operator()() {return;};		// call using operator
	void Call() {return;};				// call using function
};
