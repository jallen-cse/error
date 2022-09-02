
#ifndef INCLUDE_JACK_ERROR_HPP
#define INCLUDE_JACK_ERROR_HPP

#include <string>
#include <sstream>
#include <stdexcept>
#include <cstring>

// class Error : public std::string
// {
//   public:
//     static Error some(const char* c)
//     {
//         return Error(c);
//     }

//     static Error none()
//     {
//         return Error();
//     }

//     explicit inline operator bool()
//     {
//         return !empty();
//     }

//   protected:
//     Error() : std::string()
//     {
//     }

//     Error(Error&& from) : std::string(std::move(from))
//     {
//     }

//     Error(const Error& other) : std::string(other)
//     {
//     }

//     Error(const char* cstr) : std::string(cstr)
//     {
//     }
// };

// Error do_something()
// {
//     return Error::none();
// }


namespace jack
{

/**
 * @brief A human-readable error description
 */
class reason : protected std::string
{
    // friend class jack::error;
  
  public:
    static inline reason some(const char* message)
    {
        return reason(message);
    }

    static inline reason some(const std::string& message)
    {
        return reason(message);
    }

    static inline reason some(std::string&& message)
    {
        return reason(std::move(message));
    }

    static inline reason none()
    {
        return {};
    }

    // template <typename fn>
    // reason& if_some(const fn& f)
    // {
    //     if (*this)
    //         f(this);
    //     return *this;
    // }

    // template <typename fn>
    // reason& if_none(const fn& f)
    // {
    //     if (!*this)
    //         f();
    //     return *this;
    // }

    /**
     * @brief Wrap this reason with additional context (prepend)
     * 
     */
    inline void wrap()
    {
    }

    /**
     * @brief Extend this reason with additional information (append)
     * 
     */
    inline void extend()
    {
    }

    /**
     * @brief This will evaluate to true even if reason::some
     * was called with an empty string.  An empty string is not
     * a reason!
     * 
     * @return true if reason not empty
     */
    explicit inline operator bool()
    {
        return !empty();
    }



    reason(reason&& from) : std::string(std::move(from))
    {
    }

    reason(const reason& other) : std::string(other)
    {
    }

  protected:
    reason() : std::string()
    {
    }

    reason(const char* c_str) : std::string(c_str)
    {
    }

    reason(const std::string& str) : std::string(str)
    {
    }

    reason(std::string&& str) : std::string(std::move(str))
    {
    }
};

// class nonzero_error_code : public std::runtime_error
// {
//     nonzero_error_code() : std::runtime_error("'code' argument must be non-zero")
//     {
//     }
// };

// template <typename CodeTp = short>
// class Error
// {
//   public:

//     // // template <int code>
//     // static inline error some(int code, reason reason)
//     // {
//     //     return error(true, code, reason);
//     // }

//     // static inline error none() 
//     // {
//     //     return error(false, 0, "");
//     // }


//     /**
//      * @brief Update this' own values according to
//      * the given error (this is really a copy assign).
//      * 
//      * @param from error to copy from
//      */
//     void update(const error& from)
//     {
//     }

//     /**
//      * @brief Consume the given error object and update
//      * this' own values (this is really a move assign).
//      * 
//      * @param from error to move from
//      */
//     void update(error&& from)
//     {
//         m_some = from.m_some;        
//         m_code = from.m_code;
//         m_        
//     }

//     explicit inline operator bool() const
//     {
//         return m_code != 0;
//     }

//   protected:
//     error(bool some, int code, const reason& reason) :
//             m_some(some), m_code(code), m_reason(reason)    
//     {
//     }

//     error(bool some, int code, reason&& reason) :
//             m_some(some), m_code(code), m_reason(std::move(reason))
//     {
//     }

//     const bool m_some;
//     const int m_code;
//     reason m_reason;
// };

namespace detail {

template <typename Arg>
inline void variadic_strcat_(std::stringstream& stream,
        const Arg arg)
{
    stream << arg;
}

template <typename Current, typename Next, typename... Remainder>
inline void variadic_strcat_(std::stringstream& stream, 
        const Current current, const Next next, 
        const Remainder... remainder)
{
    stream << current;
    variadic_strcat_(stream, next, remainder...);
}

template <typename... Args>
inline std::string make_str(const Args... args)
{
    std::stringstream ss;
    variadic_strcat_(ss, args...);
    return ss.str();
}

}

class error
{
  public:

    error(error&& other) = default;
    error(const error& other) = default;

    error(int code, const char* reason) :
        m_code(code), m_reason(reason)
    {
    }

    error(int code, const std::string& reason) :
            m_code(code), m_reason(reason)    
    {
    }

    error(int code, std::string&& reason) :
            m_code(code), m_reason(std::move(reason))
    {
    }

    template <typename... str_args>
    error(int code, const str_args... args) :
            m_code(code), m_reason(detail::make_str(args...))
    {
    }

    /**
     * @brief Wrap this reason with additional context (prepend). 
     */
    void wrap(const std::string& context)
    {   
        m_reason.reserve(context.size() + 2);           // TODO benchmark preemptive reserves
        m_reason.insert(0, ": ").insert(0, context);
    }

    /**
     * @brief Extend this reason with additional information (append).
     */
    void extend(const std::string& detail)
    {
        m_reason.reserve(detail.size() + 2);            // TODO benchmark preemptive reserves
        m_reason.append(": ").append(detail);
    }

    error& operator=(const error& other)
    {
        if (this != &other)
        {
            m_code = other.m_code;
            m_reason = other.m_reason;
        }
        return *this;
    }

    error& operator=(error&& other)
    {
        if (this != &other)
        {
            m_code = other.m_code;
            m_reason = std::move(other.m_reason);
        }
        return *this;
    }

    int code() const
    {
        return m_code;
    }

    const std::string& reason() const &
    {
        return m_reason;
    }

    std::string&& reason() &&
    {
        return std::move(m_reason);
    }

  protected:
    int m_code;
    std::string m_reason;
};








}

#endif // #ifndef