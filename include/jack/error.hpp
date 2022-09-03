
#ifndef INCLUDE_JACK_ERROR_HPP
#define INCLUDE_JACK_ERROR_HPP

#include <string>
#include <sstream>
#include <cstring>

namespace jack
{

namespace detail 
{

template <typename last_t>
inline void variadic_strcat(std::stringstream& stream,
        const last_t last)
{
    stream << last;
}

template <typename current_t, typename next_t, typename... remainder_t>
inline void variadic_strcat(std::stringstream& stream, 
        const current_t current, const next_t next, 
        const remainder_t... remainder)
{
    stream << current;
    variadic_strcat(stream, next, remainder...);
}

template <typename... str_args>
inline std::string make_str(const str_args... args)
{
    std::stringstream ss;
    variadic_strcat(ss, args...);
    return ss.str();
}

} // namespace detail

/**
 * @brief A human-readable error description
 */
class reason : private std::string
{
  public:
    reason() = delete;
    reason(reason&&) = default;
    reason(const reason&) = default;

    reason(const char* c_str) : std::string(c_str)
    {
    }

    reason(const std::string& str) : std::string(str)
    {
    }

    reason(std::string&& str) : std::string(std::move(str))
    {
    }

    template <typename... str_args>
    reason(str_args... str) : std::string(detail::make_str(str...))
    {
    }

    reason& operator=(const reason& other) = default;
    reason& operator=(reason&& from) = default;

    // template <typename... str_args>
    // reason& wrap(str_args... context)
    // {
    //     auto ctx = detail::make_str(context...);
    //     reserve(ctx.size() + 2);
    //     insert(0, ": ").insert(0, ctx);
    //     return *this;
    // }

    /**
     * @brief Wrap this reason with additional context (prepend)
     * 
     * @param context c string to copy from
     */
    // template <>
    reason& wrap(const char* context)
    {
        reserve(std::string::traits_type::length(context) + 2);
        insert(0, ": ").insert(0, context);
        return *this;
    }
    
    /**
     * @brief Wrap this reason with additional context (prepend)
     * 
     * @param contect string to copy from
     */
    // template <>
    reason& wrap(const std::string& context)
    {
        reserve(context.size() + 2);           // TODO benchmark preemptive reserves
        insert(0, ": ").insert(0, context);
        return *this;
    }

    /**
     * @brief Wrap this reason with additional context (prepend)
     * 
     * @param context reason to copy from
     */
    // template <>
    reason& wrap(const reason& context)
    {
        reserve(context.size() + 2);
        insert(0, ": ").insert(0, context);
        return *this;
    }

    /**
     * @brief Extend this reason with additional information (append)
     * 
     * @param info c string to copy from
     */
    reason& extend(const char* info)
    {
        reserve(std::string::traits_type::length(info) + 2);
        append(": ").append(info);
        return *this;
    }

    /**
     * @brief Extend this reason with additional information (append)
     * 
     * @param info string to copy from
     */
    reason& extend(const std::string& info)
    {
        reserve(info.size() + 2);            // TODO benchmark preemptive reserves
        append(": ").append(info);
        return *this;
    }

    /**
     * @brief Extend this reason with additional information (append)
     * 
     * @param info reason to copy from
     */
    reason& extend(const reason& info)
    {
        reserve(info.size() + 2);
        append(": ").append(info);
        return *this;
    }

    /**
     * @brief Return const pointer to null-terminated contents. This is a
     * handle to internal data. Do not modify or dire things may happen
     */
    const char* cstr() const
    {
        return c_str();
    }
};

inline std::ostream& operator<<(std::ostream& os, const reason& reason)
{
    return os << reason.cstr();
}

/**
 * @brief A human-readable error description with a
 * paired code for programmatic error handling. 
 */
class error
{
  public:
    error(error&& other) = default;
    error(const error& other) = default;

    error(int code, const reason& reason) :
            code(code), desc(reason)
    {
    }

    error(int code, reason&& reason) :
            code(code), desc(std::move(reason))
    {
    }

    error(int code, const char* reason) :
            code(code), desc(reason)
    {
    }

    error(int code, const std::string& reason) :
            code(code), desc(reason)
    {
    }

    error(int code, std::string&& reason) : 
            code(code), desc(std::move(reason))
    {
    }

    template <typename... str_args>
    error(int code, str_args... reason) :
            code(code), desc(reason...)
    {
    }

    error& operator=(error&&) = default;
    error& operator=(const error&) = default;

    // template <typename... str_args>
    // error& wrap(str_args... context)
    // {
        // desc.wrap()
    // }

    /**
     * @brief Wrap this error's reason with additional context (prepend)
     * 
     * @param context c string to copy from
     */
    error& wrap(const char* context)
    {
        desc.wrap(context);
        return *this;
    }
    
    /**
     * @brief Wrap this error's reason with additional context (prepend)
     * 
     * @param contect string to copy from
     */
    error& wrap(const std::string& context)
    {
        desc.wrap(context);
        return *this;
    }

    /**
     * @brief Wrap this error's reason with additional context (prepend)
     * 
     * @param context reason to copy from
     */
    error& wrap(const reason& context)
    {
        desc.wrap(context);
        return *this;
    }

    /**
     * @brief Extend this error's reason with additional information (append)
     * 
     * @param info c string to copy from
     */
    error& extend(const char* info)
    {
        desc.extend(info);
        return *this;
    }

    /**
     * @brief Extend this error's reason with additional information (append)
     * 
     * @param info string to copy from
     */
    error& extend(const std::string& info)
    {
        desc.extend(info);
        return *this;
    }

    /**
     * @brief Extend this error's reason with additional information (append)
     * 
     * @param info reason to copy from
     */
    error& extend(const reason& info)
    {
        desc.extend(info);
        return *this;
    }

    /**
     * @brief Produce a human-readable debug string of this error.
     */
    std::string debug_string() const
    {
        return detail::make_str("error { code: ", code,
                ", desc: \"", desc, "\" }");
    }

    int code;
    reason desc;
};

}

#endif // #ifndef