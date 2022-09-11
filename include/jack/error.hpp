
#ifndef INCLUDE_JACK_ERROR_HPP
#define INCLUDE_JACK_ERROR_HPP

#include <string>
#include <sstream>
#include <cstring>          // dangling?
#include <type_traits>

namespace jack
{

namespace detail 
{

// prevent collisions with other jack defs
#ifndef JACK_DETAIL_VARIADIC_MAKE_STR
#define JACK_DETAIL_VARIADIC_MAKE_STR

template <typename t>
using cref_or_val = typename std::conditional<
        std::is_fundamental<t>::value, t, const t&>::type;

template <typename last_t>
inline void variadic_strcat(std::stringstream& stream,
        cref_or_val<last_t> last)
{
    stream << last;
}

template <typename current_t, typename... remainder_ts>
inline void variadic_strcat(std::stringstream& stream,
        cref_or_val<current_t> current, 
        remainder_ts&&... remainder)
{
    stream << current;
    variadic_strcat<remainder_ts...>(stream, 
            std::forward<remainder_ts>(remainder)...);
}

/**
 * @brief Construct a string from an arbitrary series of parameters
 * using a std::stringstream and operator<<.
 * 
 * @return std::string 
 */
template <typename... str_ts>
inline std::string make_str(str_ts&&... args)
{
    std::stringstream ss;
    variadic_strcat<str_ts...>(ss, 
            std::forward<str_ts>(args)...);
    return ss.str();
}

/**
 * @brief Eat empty make_str calls w/o the fluff.
 */
inline std::string make_str()
{
    return {};
}

#endif // #ifndef JACK_DETAIL_VARIADIC_MAKE_STR

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

    /**
     * @brief Wrap this reason with additional context (prepend).
     * 
     * @param context values to contruct a string from
     */
    template <typename... str_args>
    reason& wrap(str_args... context)
    {
        const auto ctx_str = detail::make_str(
                std::forward<str_args>(context)...);
        reserve(ctx_str.size() + 2);
        insert(0, ": ").insert(0, ctx_str);
        return *this;
    }
    
    /**
     * @brief Wrap this reason with additional context (prepend).
     * 
     * @param context c string to copy from
     */
    reason& wrap(const char* context)
    {
        reserve(std::string::traits_type::length(context) + 2);
        insert(0, ": ").insert(0, context);
        return *this;
    }

    /**
     * @brief Wrap this reason with additional context (prepend).
     * 
     * @param context std::string to copy from
     */
    reason& wrap(const std::string& context)
    {
        reserve(context.size() + 2);           // TODO benchmark preemptive reserves
        insert(0, ": ").insert(0, context);
        return *this;
    }

    /**
     * @brief Wrap this reason with additional context (prepend).
     * 
     * @param context reason to copy from
     */
    reason& wrap(const reason& context)
    {
        reserve(context.size() + 2);
        insert(0, ": ").insert(0, context);
        return *this;
    }

    /**
     * @brief Extend this reason with additional information (append).
     * 
     * @param info values to construct a string from
     */
    template <typename... str_args>
    reason& extend(str_args... info)
    {
        const auto info_str = detail::make_str(
                std::forward<str_args>(info)...);
        reserve(info_str.size() + 2);
        append(": ").append(info_str);
        return *this;
    }

    /**
     * @brief Extend this reason with additional information (append).
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
     * @brief Extend this reason with additional information (append).
     * 
     * @param info std::string to copy from
     */
    reason& extend(const std::string& info)
    {
        reserve(info.size() + 2);            // TODO benchmark preemptive reserves
        append(": ").append(info);
        return *this;
    }

    /**
     * @brief Extend this reason with additional information (append).
     * 
     * @param info reason to copy from
     */
    reason& extend(const reason& info)
    {
        reserve(info.size() + 2);            // TODO benchmark preemptive reserves
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

    /**
     * @brief Wrap this error's reason with additional context (prepend).
     * 
     * @param context values to construct a string from
     */
    template <typename... str_args>
    error& wrap(str_args&&... context)
    {
        desc.wrap(std::forward<str_args>(context)...);
        return *this;
    }

    /**
     * @brief Extend this error's reason with additional information (append).
     * 
     * @param info values to construct a string from
     */
    template <typename... str_args>
    error& extend(str_args&&... info)
    {
        desc.extend(std::forward<str_args>(info)...);
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