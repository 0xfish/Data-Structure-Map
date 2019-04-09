#ifndef SMART_H
#define SMART_H
#include <stdexcept>

class null_ptr_exception : public std::exception {
public:
	null_ptr_exception(const char * c) : std::exception(c) { }
};


template <typename T>
class smart_ptr {

public:
	// Create a smart_ptr that is initialized to nullptr. The reference count
	// should be initialized to nullptr.
	smart_ptr() noexcept
	{
		ptr_ = nullptr;
		ref_ = nullptr;
	}
	// Create a smart_ptr that is initialized to raw_ptr. The reference count
	// should be one.
	explicit smart_ptr(T* &raw_ptr)
	{	//Check for bad pointers
		if (raw_ptr == NULL)
			throw null_ptr_exception("Null Pointer Detected!\n");
		int *tmp;
		try { 
		tmp = new int(1); 
		ptr_ = raw_ptr;
		ref_ = tmp;
		}
		catch (std::bad_alloc &e) { throw std::bad_alloc(); }
		//After checking for errors construct the smart pointer
		
	}
	
	// Create a smart_ptr that is initialized to raw_ptr. The reference count
	// should be one. If the constructor fails raw_ptr is deleted.	
	explicit smart_ptr(T* &&raw_ptr) noexcept
	{	
	//Function declared noexcept if anything goes wrong it is handled inside
		try {
			if (raw_ptr == NULL)
				throw null_ptr_exception("Null Pointer Detected!\n");
			ptr_ = raw_ptr;
			ref_ = { new int{1} };
			raw_ptr = NULL;
			}
		catch (...) { delete raw_ptr;}
	}
	// Copy construct a pointer from rhs. The reference count should be 
	// incremented by one.
	smart_ptr(const smart_ptr& rhs)
	{
		if (rhs.ptr_ == NULL || rhs.ref_ == NULL)
			throw null_ptr_exception("Null Pointer Detected!\n");
		ptr_ = rhs.ptr_;
		ref_ = rhs.ref_;
		*ref_ += 1;
	}
	// Move construct a pointer from rhs.
	smart_ptr(smart_ptr&& rhs) noexcept
	{
		if (rhs.ptr_ != NULL && rhs.ref_ != NULL) {
			ptr_ = rhs.ptr_;
			ref_ = rhs.ref_;
			rhs.ptr_ = NULL;
			rhs.ref_ = NULL;
		}
	}
	
	
	// This assignment should make a shallow copy of the right-hand side's
	// pointer data. The reference count should be incremented as appropriate.
	smart_ptr& operator=(const smart_ptr& rhs)
	{
		//Check to see if rhs is the same as THIS
		if (this->ptr_ == rhs.ptr_)
			return *this;
		//If anything is null throw
		if (rhs.ptr_ == NULL || rhs.ref_ == NULL)
			throw null_ptr_exception("Null Pointer Detected!\n");
			ptr_ = rhs.ptr_;
			ref_ = rhs.ref_;
			*ref_ += 1;
	}
	
	// This move assignment should steal the right-hand side's pointer data.
	smart_ptr& operator=(smart_ptr&& rhs) noexcept
	{
		if (this->ptr_ == rhs.ptr_)
			return *this;
		if (rhs != NULL || rhs->ptr_ != NULL || rhs->ref_ != NULL) { //If the stolen data is not equal to NULL then move
			delete ptr_;
			delete ref_;
			ptr_ = rhs.ptr_;
			ref_ = rhs.ref_;
			rhs.ptr_ = NULL;
			rhs.ref_ = NULL;
		}
	}
	
	// If the smart_ptr is either nullptr or has a reference count of one, this
	// function will do nothing and return false. Otherwise, the referred to
	// object's reference count will be decreased and a new deep copy of the
	// object will be created. This new copy will be the object that this
	// smart_ptr points and its reference count will be one. 
	bool clone()
	{
		//If null or the counter is one
		if (this->ptr_ == NULL || *this->ref_ == 1 || this->ref_ == NULL || this == NULL)
		{
			return false;
		}
		else
		{	//Check for a bad_alloc and throw if it happens
			try { 
				T* tmp = new T(*ptr_);
				ptr_ = tmp;
			}
			catch (std::bad_alloc &e) { throw std::bad_alloc(); }
			try { 
				int *tmp2 = new int(1); 
				*ref_ -= 1;
				ref_ = tmp2;
			}
			catch(std::bad_alloc &e) {throw std::bad_alloc(); }
			//Decrement the reference and clone the object	
		}
	}
		
	// Returns the reference count of the pointed to data.
		int ref_count() const
	{
		if (ref_ == NULL && ptr_ == NULL)
			return 0;
		return *ref_;
	}
		// The dereference operator shall return a reference to the referred object.
		// Throws null_ptr_exception on invalid access. 
		T& operator*() 
		{	//Attempting to deference a pointer that is null or if any of its members are null throw
			if (this->ptr_ == NULL || this->ref_ == NULL)
				throw null_ptr_exception("Null Pointer is bad\n");
			else
				return *this->ptr_;
		}

		// The arrow operator shall return the pointer ptr_. Throws null_ptr_exception
		// on invalid access.
	T* operator->()
	{	//If whatever its pointing to is null throw the exception
		if (!ptr_)
			throw null_ptr_exception("Pointer is null!\n");
		try {
			return this->ptr_;
			}
		catch(std::system_error &e)
		{
			throw null_ptr_exception("Access Violation!\n");
		}
	}
	
	~smart_ptr() 
	{
	//Everytime the destructor is called it subtracts 1 from the ref count;
		//If ref is null catch in destructor
		
		if (ref_ != NULL) {
			*ref_ -= 1;
			//Clean up all dynamic memory if the ref count is 0	
			if (*ref_ == 0)
			{
				delete ptr_;
				delete ref_;
			}
		}
	
	}          // deallocate all dynamic memory
private:
	T* ptr_;               // pointer to the referred object
	int* ref_;             // pointer to a reference count

};


#endif // !SMART_
