
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
 * @return std::string constructed from given args
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
 * @brief No-argument overload of detail::make_str that avoids
 * useless calls when returning an empty std::string.
 * 
 * @return empty std::string
 */
inline std::string make_str()
{
    return {};
}

#endif // #ifndef

} // namespace detail

/**
 * @brief A human-readable error description.
 */
class reason : private std::string
{
  public:

    // Expose std::string member functions 
    using std::string::c_str;

    /**
     * @brief Prevent default reason construction.
     */
    reason() = delete;

    /**
     * @brief Construct a new reason object by moving from
     * another reason object.
     * 
     * @param reason reason to move from
     */
    reason(reason&& reason) = default;

    /**
     * @brief Construct a new reason object by copying from
     * another reason object.
     * 
     * @param reason reason to copy from
     */
    reason(const reason& reason) = default;

    /**
     * @brief Construct a new reason object by copying from
     * a c string.
     * 
     * @param c_str c string to copy from
     */
    reason(const char* c_str) : std::string(c_str)
    {
    }

    /**
     * @brief Construct a new reason object by copying from
     * a std::string.
     * 
     * @param str std::string to copy from
     */
    reason(const std::string& str) : std::string(str)
    {
    }

    /**
     * @brief Construct a new reason object by moving from
     * a std::string.
     * 
     * @param str std::string to move from
     */
    reason(std::string&& str) : std::string(std::move(str))
    {
    }

    /**
     * @brief Construct a new reason object from an arbitrary series
     * of parameters using a std::stringstream and operator<<.
     * 
     * @tparam str_args types of arguments used to construct the reason
     * @param str values to construct a reason from
     */
    template <typename... str_args>
    reason(str_args... str) : std::string(detail::make_str(
            std::forward<str_args>(str)...))
    {
    }

    /**
     * @brief Copy assignment operator.
     * 
     * @param other reason to copy from
     * @return reference to this reason
     */
    reason& operator=(const reason& other) = default;
    
    /**
     * @brief Move assignment operator.
     * 
     * @param from reason to move from
     * @return reference to this reason
     */
    reason& operator=(reason&& from) = default;

    /**
     * @brief Wrap this reason with additional context (prepend).
     * 
     * @param context values to contruct a string from
     * @return reference to this reason
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
     * @return reference to this reason
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
     * @return reference to this reason
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
     * @return reference to this reason
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
     * @return reference to this reason
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
     * @return reference to this reason
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
     * @return reference to this reason
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
     * @return reference to this reason
     */
    reason& extend(const reason& info)
    {
        reserve(info.size() + 2);            // TODO benchmark preemptive reserves
        append(": ").append(info);
        return *this;
    }
};

/**
 * @brief Implement stream operator for reason class.
 * 
 * @param os std::ostream reference to write to
 * @param reason reason to write from
 * @return reference to param os
 */
inline std::ostream& operator<<(std::ostream& os, const reason& reason)
{
    return os << reason.c_str();
}

/**
 * @brief A human-readable error description with a
 * paired code for programmatic error handling. 
 */
class error
{
  public:

    /**
     * @brief Prevent default error construction.
     */
    error() = delete;

    /**
     * @brief Construct a new error object by moving from
     * another error object.
     * 
     * @param other error to move from
     */
    error(error&& other) = default;
    
    /**
     * @brief Construct a new error object by copying from
     * another error object.
     * 
     * @param other error to copy from
     */
    error(const error& other) = default;
    
    /**
     * @brief Construct a new error object by accepting a 
     * code and a reason object to copy from.
     * 
     * @param code error code
     * @param reason reason to copy from
     */
    error(int code, const reason& reason) :
            code(code), desc(reason)
    {
    }

    /**
     * @brief Construct a new error object by accepting a 
     * code and a reason object to move from.
     * 
     * @param code error code
     * @param reason reason to move from
     */
    error(int code, reason&& reason) :
            code(code), desc(std::move(reason))
    {
    }
    
    /**
     * @brief Construct a new error object by accepting a 
     * code and a c string to copy from.
     * 
     * @param code error code
     * @param reason c string to copy from
     */
    error(int code, const char* reason) :
            code(code), desc(reason)
    {
    }

    /**
     * @brief Construct a new error object by accepting a 
     * code and a std::string to copy from.
     * 
     * @param code error code
     * @param reason std::string to copy from
     */
    error(int code, const std::string& reason) :
            code(code), desc(reason)
    {
    }
    
    /**
     * @brief Construct a new error object by accepting a 
     * code and a std::string to move from.
     * 
     * @param code error code
     * @param reason std::string to move from
     */
    error(int code, std::string&& reason) : 
            code(code), desc(std::move(reason))
    {
    }
    
    /**
     * @brief Construct a new error object from an arbitrary series
     * of parameters using a std::stringstream and operator<<.
     * 
     * @tparam str_args types of arguments used to construct the reason
     * @param code error code
     * @param reason values to construct a reason from
     */
    template <typename... str_args>
    error(int code, str_args... reason) :
            code(code), desc(reason...)
    {
    }
    
    /**
     * @brief Copy assignment operator.
     * 
     * @param other error to copy from
     * @return reference to this error
     */
    error& operator=(const error& other) = default;

    /**
     * @brief Move assignment operator.
     * 
     * @param from error to move from
     * @return reference to this error
     */
    error& operator=(error&& from) = default;

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

    /// @brief Signed integer error code.
    int code;

    /// @brief Human-readable error description.
    reason desc;
};

/**
 * @brief A human-readable error description with a
 * paired code for programmatic error handling. 
 * 
 * @tparam default integer to represent default state
 */
// template <int default>
// class derror;

namespace debug
{

/**
 * @brief Produce a friendly debug string.
 * 
 * @param error error to copy from
 * @return debug string from given error
 */
inline std::string str(const error& error)
{
    return detail::make_str("error { code: ", error.code,
            ", desc: \"", error.desc, "\" }");
}

} // namespace debug

} // namespace jack

#endif // #ifndef