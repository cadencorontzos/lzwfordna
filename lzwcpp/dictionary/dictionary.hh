#include <iostream>

template <typename value_type> class Dictionary_Entry{
	public:
		value_type value;
		bool exists;
		Dictionary_Entry(bool exist, value_type val) : value(val), exists(exist){};
		Dictionary_Entry(bool exist) : exists(exist){};
};

template <typename key_type, typename value_type> class Dictionary {
 public:
	Dictionary() = default;
	virtual ~Dictionary() = default;
	virtual void set_key(key_type key, value_type value) = 0;
	virtual	Dictionary_Entry<value_type> get_value(key_type key)= 0;
};
