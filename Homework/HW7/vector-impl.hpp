template <typename _Tp>
class __detail::__iterator {
public:
    // Default constructor
    __iterator(): _ptr(nullptr) {}
    // Copy constructor
    __iterator(const __iterator& other) {
        _ptr = other._ptr;
    }
    // Initialize the iterator with a pointer
    explicit __iterator(_Tp *ptr) {
        _ptr = ptr;
    }

    ~__iterator()=default;

    // Copy assignment operator
    // Return self
    __iterator<_Tp>& operator=(const __iterator& that) {
        _ptr = that._ptr;
        return *this;
    }

    // Dereference/Arrow operator
    // Return self
    _Tp& operator*() {
        return *_ptr;
    }
    _Tp* operator->() {
        return _ptr;
    }

    // Comparison operator
    bool operator==(const __iterator& that) {
        return _ptr == that._ptr;
    }
    bool operator!=(const __iterator& that) {
        return _ptr != that._ptr;
    }

    // Self increment and decrement operator (prefix)
    // Return self
    __iterator<_Tp>& operator++() {
        ++_ptr;
        return *this;
    }
    __iterator<_Tp>& operator--() {
        --_ptr;
        return *this;
    }

    // Self increment and decrement operator (postfix)
    // Return another obj
    __iterator<_Tp> operator++(int) {
        __iterator<_Tp> origin = *this;
        _ptr++;
        return origin;
    }
    __iterator<_Tp> operator--(int) {
        __iterator<_Tp> origin = *this;
        _ptr--;
        return origin;
    }

    // Arithmetic operator
    __iterator<_Tp> operator+(int n) {
        return __iterator((_ptr + n));
    }
    __iterator<_Tp> operator-(int n) {
        return __iterator((_ptr - n));
    }
    // Difference between iter
    int operator-(const __iterator& that) {
        return _ptr - that._ptr;
    }

    // Compound assignment operator
    // Return self
    __iterator<_Tp>& operator+=(int n) {
        _ptr += n;
        return *this;
    }
    __iterator<_Tp>& operator-=(int n) {
        _ptr -= n;
        return *this;
    }

    // Data pointer
    _Tp *_ptr;
};

template <typename _Tp>
class __detail::__const_iterator {
public:
    // Default constructor
    __const_iterator(): _ptr(nullptr) {}
    // Copy constructor
    __const_iterator(const __const_iterator& other) {
        _ptr = other._ptr;
    }
    // Convert constructor
    explicit __const_iterator (const __iterator<_Tp>& other) {
        _ptr = other._ptr;
    }
    // Initialize the iterator with a pointer
    explicit __const_iterator(const _Tp *ptr) {
        _ptr = ptr;
    }

    ~__const_iterator()=default;

    // Copy assignment operator
    // Return self
    __const_iterator<_Tp>& operator=(const __const_iterator& that) {
        _ptr = that._ptr;
        return *this;
    }
    __const_iterator<_Tp>& operator=(const __iterator<_Tp>& that) {
        _ptr = that._ptr;
        return *this;
    }

    // Dereference/Arrow operator
    // Return self
    const _Tp& operator*() {
        return *_ptr;
    }
    const _Tp* operator->() {
        return _ptr;
    }

    // Comparison operator
    bool operator==(const __const_iterator& that) {
        return _ptr == that._ptr;
    }
    bool operator!=(const __const_iterator& that) {
        return _ptr != that._ptr;
    }

    // Self increment and decrement operator (prefix)
    // Return self
    __const_iterator<_Tp>& operator++() {
        ++_ptr;
        return *this;
    }
    __const_iterator<_Tp>& operator--() {
        --_ptr;
        return *this;
    }

    // Self increment and decrement operator (postfix)
    // Return another obj
    __const_iterator<_Tp> operator++(int) {
        __const_iterator<_Tp> origin = *this;
        _ptr++;
        return origin;
    }
    __const_iterator operator--(int) {
        __const_iterator<_Tp> origin = *this;
        _ptr--;
        return origin;
    }

    // Arithmetic operator
    __const_iterator<_Tp> operator+(int n) {
        return __const_iterator((_ptr + n));
    }
    __const_iterator<_Tp> operator-(int n) {
        return __const_iterator((_ptr - n));
    }
    // Difference between iter
    int operator-(const __const_iterator& that) {
        return _ptr - that._ptr;
    }

    // Compound assignment operator
    // Return self
    __const_iterator<_Tp>& operator+=(int n) {
        _ptr += n;
        return *this;
    }
    __const_iterator<_Tp>& operator-=(int n) {
        _ptr -= n;
        return *this;
    }

    // Data pointer
    const _Tp *_ptr;
};

// Below is for vector //
// Table of renaming
// typedef size_t                                           size_type;
// typedef _Tp                                              value_type;
// typedef _Tp*                                             pointer;
// typedef _Tp&                                             reference;
// typedef const _Tp&                                       const_reference;
// typedef ptrdiff_t                                        difference_type;
// typedef typename __detail::__iterator<_Tp>               iterator;
// typedef typename __detail::__const_iterator<_Tp>         const_iterator;

// Default constructor
template <typename _Tp>
vector<_Tp>::vector(): _M_size(0), _M_capacity(2) {
    // Default set size to 0 and capacity to 2
    _M_data = new value_type[_M_capacity]();
}

// Create a vector with $size copies of $value
template <typename _Tp>
vector<_Tp>::vector(size_type size, const _Tp &value): _M_size(0), _M_capacity(2) {
    if (size > 0) {
        _M_size = _M_capacity = size;
        _M_data = new value_type[_M_capacity]();
        // Copy values
        for (size_type i = 0; i < size; ++i) {
            _M_data[i] = value;
        }
    } else {
        // Default set size to 0 and capacity to 2
        _M_data = new value_type[_M_capacity]();
    }
}
// Create a vector consists of the elements in the initialize_list
template <typename _Tp>
vector<_Tp>::vector(std::initializer_list<_Tp> l): _M_size(0), _M_capacity(2) {
    if (l.size() > 0) {
        _M_size = _M_capacity = l.size();
        _M_data = new value_type[_M_capacity]();
        int i = 0;
        // Copy values
        for (auto data: l) {
            _M_data[i++] = data;
        }
    } else {
        // Default set size to 0 and capacity to 2
        _M_data = new value_type[_M_capacity]();
    }
}

// Destructor
template <typename _Tp>
vector<_Tp>::~vector() {
    // Free heap
    delete[] _M_data;
}

// Subscript access to the data contains in the vector
template <typename _Tp>
typename vector<_Tp>::reference
vector<_Tp>::operator[](typename vector<_Tp>::size_type n) {
    // Just return the value
    return _M_data[n];
}

// The size of the vector
template <typename _Tp>
typename vector<_Tp>::size_type
vector<_Tp>::size() const {
    return _M_size;
}

// Iterators
template <typename _Tp>
typename vector<_Tp>::iterator
vector<_Tp>::begin() {
    // Use iter_constructor
    return iterator(&(_M_data[0]));
}
template <typename _Tp>
typename vector<_Tp>::iterator
vector<_Tp>::end() {
    // Use iter_constructor
    return iterator(&(_M_data[_M_size - 1]) + 1);
}

// Const iterators
template <typename _Tp>
typename vector<_Tp>::const_iterator
vector<_Tp>::cbegin() const {
    // Use const_iter_constructor
    return const_iterator(&(_M_data[0]));
}
template <typename _Tp>
typename vector<_Tp>::const_iterator
vector<_Tp>::cend() const {
    // Use const_iter_constructor
    return const_iterator(&(_M_data[_M_size - 1]) + 1);
}

// Insert given value into vector before specified iterator
// Return an iterator that points to the inserted data
template <typename _Tp>
typename vector<_Tp>::iterator
vector<_Tp>::insert(iterator pos, const _Tp &value) {
    // Calculate the diff first to avoid memory issue
    difference_type diff = end() - pos;
    while (_M_size >= _M_capacity) {
        _grow();
    }
    // std::cout << "diff: " << diff << " _M_size: " << _M_size << " _M_capacity: " << _M_capacity << std::endl;
    // Copy original value
    for (size_type i = _M_size; i > _M_size - diff; --i) {
        _M_data[i] = _M_data[i - 1];
    }
    // Update
    _M_data[_M_size - diff] = value;
    ++_M_size;
    // Use iter_constructor
    // -1 for the changed _M_size
    return iterator(&(_M_data[_M_size - 1 - diff]));
}
template <typename _Tp>
typename vector<_Tp>::iterator
vector<_Tp>::insert(const_iterator pos, const _Tp &value) {
    // Calculate diff first to avoid memory issue
    difference_type diff = cend() - pos;
    while (_M_size >= _M_capacity) {
        _grow();
    }
    // std::cout << "diff: " << diff << " _M_size: " << _M_size << " _M_capacity: " << _M_capacity << std::endl;
    // Copy original data
    for (size_type i = _M_size; i > _M_size - diff; --i) {
        _M_data[i] = _M_data[i - 1];
    }
    // Update
    _M_data[_M_size - diff] = value;
    ++_M_size;
    // Use iter_constructor
    // -1 for the changed _M_size
    return iterator(&(_M_data[_M_size- 1 - diff]));
}

// Add data to the end of the vector
template <typename _Tp>
void vector<_Tp>::push_back(const _Tp &value) {
    while (_M_size >= _M_capacity) {
        _grow();
    }
    // std::cout << _M_capacity;
    // Update
    _M_data[_M_size] = value;
    ++_M_size;
}

// Remove element at given position
// Return an iterator to the next element (or end())
template <typename _Tp>
typename vector<_Tp>::iterator
vector<_Tp>::erase(iterator pos) {
    // Calculate diff first to avoid memory issue
    difference_type diff = end() - pos;
    // std::cout << "diff: " << diff << " _M_size: " << _M_size << " _M_capacity: " << _M_capacity << std::endl;
    // Copy original data
    for (size_type i = _M_size - diff; i < _M_size - 1; ++i) {
        _M_data[i] = _M_data[i + 1];
    }
    // Update
    --_M_size;
    if (diff == 1) {
        return end();
    } else {
        // Use iter_constructor
        // +1 for the changed _M_size
        return iterator(&(_M_data[_M_size + 1 - diff]));
    }
}
template <typename _Tp>
typename vector<_Tp>::iterator
vector<_Tp>::erase(const_iterator pos) {
    // Calculate diff first to avoid memory issue
    difference_type diff = cend() - pos;
    // std::cout << "diff: " << diff << " _M_size: " << _M_size << " _M_capacity: " << _M_capacity << std::endl;
    // Copy original data
    for (size_type i = _M_size - diff; i < _M_size - 1; ++i) {
        _M_data[i] = _M_data[i + 1];
    }
    // Update
    --_M_size;
    if (diff == 1) {
        return end();
    } else {
        // Use iter_constructor
        // +1 for the changed _M_size
        return iterator(&(_M_data[_M_size + 1 - diff]));
    }
}

// Remove last element
template <typename _Tp>
void vector<_Tp>::pop_back() {
    // Just deduct the size
    --_M_size;
}

template <typename _Tp>
inline void vector<_Tp>::_grow() {
    // Update capacity
    size_type new_capacity = _M_capacity;
    while (new_capacity <= _M_size) {
        // My method to grow
        new_capacity *= 2;
    }
    auto new_M_data = new value_type[new_capacity]();
    // std::cout << new_capacity;
    // Copy original data
    for (size_type i = 0; i < _M_size; ++i) {
        new_M_data[i] = _M_data[i];
    }
    // Free original pointer
    pointer tmp = _M_data;
    _M_data = new_M_data;
    delete[] tmp;
    // Update
    _M_capacity = new_capacity;
}

