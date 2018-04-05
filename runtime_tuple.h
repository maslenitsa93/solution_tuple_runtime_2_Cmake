#pragma once

#include <type_traits>
#include <utility>
#include <string>
#include <vector>
#include <initializer_list>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <algorithm>

template<typename S, typename T>
class is_streamable
{
    template<typename SS, typename TT>
    static auto test(int)
    -> decltype( std::declval<SS&>() << std::declval<TT>(), std::true_type() );

    template<typename, typename>
    static auto test(...) -> std::false_type;

public:
    static const bool value = decltype(test<S,T>(0))::value;
};

template<typename T,
         typename = typename std::enable_if<
         is_streamable<std::stringstream, T>::value>::type>
std::string toString(const T &value)
{
    std::stringstream ss;
    ss<<value;
    return ss.str();
}

inline std::string toString(...)
{
    return "not provide";
}

namespace runtime
{
template <class T>
using StorageType = typename std::decay<T>::type;

struct Any
{
    inline bool is_null() const
    {
        return !ptr;
    }
    
	inline bool not_null() const
    {
        return ptr;
    }

    template<typename U> Any(U&& value)
        : ptr(new Derived<StorageType<U>>(std::forward<U>(value)))
    {
    }

    template<class U> bool is() const
    {
        typedef StorageType<U> T;

        auto derived = dynamic_cast<Derived<T>*> (ptr);

        return derived;
    }

    template<class U>
    StorageType<U>& as()
    {
        typedef StorageType<U> T;

        auto derived = dynamic_cast<Derived<T>*> (ptr);

        if (!derived)
            throw std::bad_cast();

        return derived->value;
    }

    template<class U>
    operator U()
    {
        return as<StorageType<U>>();
    }

    Any()
        : ptr(nullptr)
    {

    }

    Any(Any& that)
        : ptr(that.clone())
    {

    }

    Any(Any&& that)
        : ptr(that.ptr)
    {
        that.ptr = nullptr;
    }

    Any(const Any& that)
        : ptr(that.clone())
    {

    }

    Any(const Any&& that)
        : ptr(that.clone())
    {

    }

    Any& operator=(const Any& a)
    {
        if (ptr == a.ptr)
            return *this;

        auto old_ptr = ptr;

        ptr = a.clone();

        if (old_ptr)
            delete old_ptr;

        return *this;
    }

    Any& operator=(Any&& a)
    {
        if (ptr == a.ptr)
            return *this;

        std::swap(ptr, a.ptr);

        return *this;
    }

    ~Any()
    {
        delete ptr;
    }

private:
    struct Base
    {
        virtual ~Base() {}
        virtual std::string to_string() const = 0;
        virtual Base* clone() const = 0;
    };

    template<typename T>
    struct Derived : Base
    {
        template<typename U> Derived(U&& value) : value(std::forward<U>(value))
        {

        }

        T value;

        Base* clone() const
        {
            return new Derived<T>(value);
        }

        std::string to_string() const
        {
            return toString(value);
        }
    };

    Base* clone() const
    {
        if (ptr)
            return ptr->clone();
        else
            return nullptr;
    }

    Base* ptr;

public:
    std::string to_string() const
    {
        if (!ptr)
            return "null";
        return ptr->to_string();
    }
};

class tuple
{
public:
    tuple() = default;
    tuple(const tuple & other) = default;
    tuple(const std::initializer_list<Any>& list):m_values(list) {}

    template<typename T>
    T& get(std::size_t index)
    {
        return m_values.at(index).as<T>();
    }

    template<typename T>
    const T& get(std::size_t index) const
    {
        return m_values.at(index).as<T>();
    }

    const Any& get(std::size_t index) const
    {
        return m_values.at(index);
    }

    std::size_t size()const
    {
        return m_values.size();
    }

    std::string to_string(const std::string &sep = ",") const
    {
        std::stringstream ss;
        ss<<"{";
        auto size = m_values.size();
        if(size)
        {
            for(std::size_t i=0; i<size-1; i++)
                ss<<m_values[i].to_string()<<sep;
            ss<<m_values[size-1].to_string();
        }
        ss<<"}";
        return ss.str();
    }

    bool operator< (const tuple& rhs) const
    {
        auto size = m_values.size();
        if(size < rhs.size())
            return true;
        else if(size > rhs.size())
            return false;

        for(std::size_t i=0; i<size; i++)
        {
            auto a = get(i).to_string();
            auto b = rhs.get(i).to_string();
            if(a < b)
                return true;
            else if(a > b)
                return false;
        }
        return false;
    }
    bool operator> (const tuple& rhs) const
    {
        return rhs < *this;
    }
    bool operator<=(const tuple& rhs) const
    {
        return !(*this> rhs);
    }
    bool operator>=(const tuple& rhs) const
    {
        return !(*this < rhs);
    }
    bool operator==(const tuple& rhs) const
    {
        auto size = m_values.size();
        if(size != rhs.size())
            return false;

        for(std::size_t i=0; i<size; i++)
        {
            auto a = get(i).to_string();
            auto b = rhs.get(i).to_string();
            if(a != b)
                return false;
        }
        return true;
    }
    bool operator!=(const tuple& rhs) const
    {
        return !(*this== rhs);
    }

private:
    std::vector<Any> m_values;
};

}