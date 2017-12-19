/*
 *  Created by Phil on 8/5/2012.
 *  Copyright 2012 Two Blue Cubes Ltd. All rights reserved.
 *
 *  Distributed under the Boost Software License, Version 1.0. (See accompanying
 *  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */
#ifndef TWOBLUECUBES_CATCH_TOSTRING_H_INCLUDED
#define TWOBLUECUBES_CATCH_TOSTRING_H_INCLUDED


#include <sstream>
#include <vector>
#include <cstddef>
#include <type_traits>
#include <string>

#ifdef __OBJC__
#include "catch_objc_arc.hpp"
#endif

#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable:4180) // We attempt to stream a function (address) by const&, which MSVC complains about but is harmless
#endif


// We need a dummy global operator<< so we can bring it into Catch namespace later
struct Catch_global_namespace_dummy;
std::ostream& operator<<(std::ostream&, Catch_global_namespace_dummy);

namespace Catch {
    // Bring in operator<< from global namespace into Catch namespace
    using ::operator<<;

    namespace Detail {

        extern const std::string unprintableString;

        std::string rawMemoryToString( const void *object, std::size_t size );

        template<typename T>
        std::string rawMemoryToString( const T& object ) {
          return rawMemoryToString( &object, sizeof(object) );
        }

        template<typename T>
        class IsStreamInsertable {
            template<typename SS, typename TT>
            static auto test(int)
                -> decltype(std::declval<SS&>() << std::declval<TT>(), std::true_type());

            template<typename, typename>
            static auto test(...)->std::false_type;

        public:
            static const bool value = decltype(test<std::ostream, const T&>(0))::value;
        };

    } // namespace Detail

    // If we decide for C++14, change these to enable_if_ts
    template <typename T>
    struct StringMaker {
        template <typename Fake = T>
        static
        typename std::enable_if<::Catch::Detail::IsStreamInsertable<Fake>::value, std::string>::type
            convert(const Fake& t) {
                std::ostringstream sstr;
                sstr << t;
                return sstr.str();
        }

        template <typename Fake = T>
        static
        typename std::enable_if<!::Catch::Detail::IsStreamInsertable<Fake>::value, std::string>::type
            convert(const Fake&) {
                return Detail::unprintableString;
        }
    };

    namespace Detail {

        // This function dispatches all stringification requests inside of Catch.
        // Should be preferably called fully qualified, like ::Catch::Detail::stringify
        template <typename T>
        std::string stringify(const T& e) {
            return ::Catch::StringMaker<typename std::remove_cv<typename std::remove_reference<T>::type>::type>::convert(e);
        }

    } // namespace Detail


    // Some predefined specializations

    template<>
    struct StringMaker<std::string> {
        static std::string convert(const std::string& str);
    };
    template<>
    struct StringMaker<std::wstring> {
        static std::string convert(const std::wstring& wstr);
    };

    template<>
    struct StringMaker<char const *> {
        static std::string convert(char const * str);
    };
    template<>
    struct StringMaker<char *> {
        static std::string convert(char * str);
    };
    template<>
    struct StringMaker<wchar_t const *> {
        static std::string convert(wchar_t const * str);
    };
    template<>
    struct StringMaker<wchar_t *> {
        static std::string convert(wchar_t * str);
    };

    template<int SZ>
    struct StringMaker<char[SZ]> {
        static std::string convert(const char* str) {
            return ::Catch::Detail::stringify(std::string{ str });
        }
    };
    template<int SZ>
    struct StringMaker<signed char[SZ]> {
        static std::string convert(const char* str) {
            return ::Catch::Detail::stringify(std::string{ str });
        }
    };
    template<int SZ>
    struct StringMaker<unsigned char[SZ]> {
        static std::string convert(const char* str) {
            return ::Catch::Detail::stringify(std::string{ str });
        }
    };

    template<>
    struct StringMaker<int> {
        static std::string convert(int value);
    };
    template<>
    struct StringMaker<long> {
        static std::string convert(long value);
    };
    template<>
    struct StringMaker<long long> {
        static std::string convert(long long value);
    };
    template<>
    struct StringMaker<unsigned int> {
        static std::string convert(unsigned int value);
    };
    template<>
    struct StringMaker<unsigned long> {
        static std::string convert(unsigned long value);
    };
    template<>
    struct StringMaker<unsigned long long> {
        static std::string convert(unsigned long long value);
    };

    template<>
    struct StringMaker<bool> {
        static std::string convert(bool b);
    };

    template<>
    struct StringMaker<char> {
        static std::string convert(char c);
    };
    template<>
    struct StringMaker<signed char> {
        static std::string convert(signed char c);
    };
    template<>
    struct StringMaker<unsigned char> {
        static std::string convert(unsigned char c);
    };

    template<>
    struct StringMaker<std::nullptr_t> {
        static std::string convert(std::nullptr_t);
    };

    template<>
    struct StringMaker<float> {
        static std::string convert(float value);
    };
    template<>
    struct StringMaker<double> {
        static std::string convert(double value);
    };

    template <typename T>
    struct StringMaker<T*> {
        template <typename U>
        static std::string convert(U* p) {
            if (p) {
                return ::Catch::Detail::rawMemoryToString(p);
            } else {
                return "nullptr";
            }
        }
    };

    template <typename R, typename C>
    struct StringMaker<R C::*> {
        static std::string convert(R C::* p) {
            if (p) {
                return ::Catch::Detail::rawMemoryToString(p);
            } else {
                return "nullptr";
            }
        }
    };

    namespace Detail {
        template<typename InputIterator>
        std::string rangeToString(InputIterator first, InputIterator last) {
            std::ostringstream oss;
            oss << "{ ";
            if (first != last) {
                oss << ::Catch::Detail::stringify(*first);
                for (++first; first != last; ++first)
                    oss << ", " << ::Catch::Detail::stringify(*first);
            }
            oss << " }";
            return oss.str();
        }
    }

    template<typename T, typename Allocator>
    struct StringMaker<std::vector<T, Allocator> > {
        static std::string convert( std::vector<T,Allocator> const& v ) {
            return ::Catch::Detail::rangeToString( v.begin(), v.end() );
        }
    };

    template<typename T>
    struct EnumStringMaker {
        static std::string convert(const T& t) {
            return ::Catch::Detail::stringify(static_cast<typename std::underlying_type<T>::type>(t));
        }
    };

#ifdef __OBJC__
    template<>
    struct StringMaker<NSString*> {
        static std::string convert(NSString * nsstring) {
            if (!nsstring)
                return "nil";
            return std::string("@") + [nsstring UTF8String];
        }
    };
    template<>
    struct StringMaker<NSObject*> {
        static std::string convert(NSObject* nsObject) {
            return ::Catch::Detail::stringify([nsObject description]);
        }

    };
    namespace Detail {
        inline std::string stringify( NSString* nsstring ) {
            return StringMaker<NSString*>::convert( nsstring );
        }

    } // namespace Detail
#endif // __OBJC__

} // namespace Catch

//////////////////////////////////////////////////////
// Separate std-lib types stringification, so it can be selectively enabled
// This means that we do not bring in

#if defined(CATCH_CONFIG_ENABLE_ALL_STRINGMAKERS)
#  define CATCH_CONFIG_ENABLE_PAIR_STRINGMAKER
#  define CATCH_CONFIG_ENABLE_TUPLE_STRINGMAKER
#  define CATCH_CONFIG_ENABLE_CHRONO_STRINGMAKER
#endif

// Separate std::pair specialization
#if defined(CATCH_CONFIG_ENABLE_PAIR_STRINGMAKER)
#include <utility>
namespace Catch {
    template<typename T1, typename T2>
    struct StringMaker<std::pair<T1, T2> > {
        static std::string convert(const std::pair<T1, T2>& pair) {
            std::ostringstream oss;
            oss << "{ "
                << ::Catch::Detail::stringify(pair.first)
                << ", "
                << ::Catch::Detail::stringify(pair.second)
                << " }";
            return oss.str();
        }
    };
}
#endif // CATCH_CONFIG_ENABLE_PAIR_STRINGMAKER

// Separate std::tuple specialization
#if defined(CATCH_CONFIG_ENABLE_TUPLE_STRINGMAKER)
#include <tuple>
namespace Catch {
    namespace Detail {
        template<
            typename Tuple,
            std::size_t N = 0,
            bool = (N < std::tuple_size<Tuple>::value)
            >
            struct TupleElementPrinter {
            static void print(const Tuple& tuple, std::ostream& os) {
                os << (N ? ", " : " ")
                    << ::Catch::Detail::stringify(std::get<N>(tuple));
                TupleElementPrinter<Tuple, N + 1>::print(tuple, os);
            }
        };

        template<
            typename Tuple,
            std::size_t N
        >
            struct TupleElementPrinter<Tuple, N, false> {
            static void print(const Tuple&, std::ostream&) {}
        };

    }


    template<typename ...Types>
    struct StringMaker<std::tuple<Types...>> {
        static std::string convert(const std::tuple<Types...>& tuple) {
            std::ostringstream os;
            os << '{';
            Detail::TupleElementPrinter<std::tuple<Types...>>::print(tuple, os);
            os << " }";
            return os.str();
        }
    };
}
#endif // CATCH_CONFIG_ENABLE_TUPLE_STRINGMAKER


// Separate std::chrono::duration specialization
#if defined(CATCH_CONFIG_ENABLE_CHRONO_STRINGMAKER)
#include <ctime>
#include <ratio>
#include <chrono>

template <class Ratio>
struct ratio_string {
    static std::string symbol();
};

template <class Ratio>
std::string ratio_string<Ratio>::symbol() {
    std::ostringstream oss;
    oss << '[' << Ratio::num << '/'
        << Ratio::den << ']';
    return oss.str();
}
template <>
struct ratio_string<std::atto> {
    static std::string symbol() { return "a"; }
};
template <>
struct ratio_string<std::femto> {
    static std::string symbol() { return "f"; }
};
template <>
struct ratio_string<std::pico> {
    static std::string symbol() { return "p"; }
};
template <>
struct ratio_string<std::nano> {
    static std::string symbol() { return "n"; }
};
template <>
struct ratio_string<std::micro> {
    static std::string symbol() { return "u"; }
};
template <>
struct ratio_string<std::milli> {
    static std::string symbol() { return "m"; }
};

namespace Catch {
    ////////////
    // std::chrono::duration specializations
    template<typename Value, typename Ratio>
    struct StringMaker<std::chrono::duration<Value, Ratio>> {
        static std::string convert(std::chrono::duration<Value, Ratio> const& duration) {
            std::ostringstream oss;
            oss << duration.count() << ' ' << ratio_string<Ratio>::symbol() << 's';
            return oss.str();
        }
    };
    template<typename Value>
    struct StringMaker<std::chrono::duration<Value, std::ratio<1>>> {
        static std::string convert(std::chrono::duration<Value, std::ratio<1>> const& duration) {
            std::ostringstream oss;
            oss << duration.count() << " s";
            return oss.str();
        }
    };
    template<typename Value>
    struct StringMaker<std::chrono::duration<Value, std::ratio<60>>> {
        static std::string convert(std::chrono::duration<Value, std::ratio<60>> const& duration) {
            std::ostringstream oss;
            oss << duration.count() << " m";
            return oss.str();
        }
    };
    template<typename Value>
    struct StringMaker<std::chrono::duration<Value, std::ratio<3600>>> {
        static std::string convert(std::chrono::duration<Value, std::ratio<3600>> const& duration) {
            std::ostringstream oss;
            oss << duration.count() << " h";
            return oss.str();
        }
    };

    ////////////
    // std::chrono::time_point specialization
    // Generic time_point cannot be specialized, only std::chrono::time_point<system_clock>
    template<typename Clock, typename Duration>
    struct StringMaker<std::chrono::time_point<Clock, Duration>> {
        static std::string convert(std::chrono::time_point<Clock, Duration> const& time_point) {
            return ::Catch::Detail::stringify(time_point.time_since_epoch()) + " since epoch";
        }
    };
    // std::chrono::time_point<system_clock> specialization
    template<typename Duration>
    struct StringMaker<std::chrono::time_point<std::chrono::system_clock, Duration>> {
        static std::string convert(std::chrono::time_point<std::chrono::system_clock, Duration> const& time_point) {
            auto converted = std::chrono::system_clock::to_time_t(time_point);

#ifdef _MSC_VER
            std::tm timeInfo = {};
            gmtime_s(&timeInfo, &converted);
#else
            std::tm* timeInfo = std::gmtime(&converted);
#endif

            auto const timeStampSize = sizeof("2017-01-16T17:06:45Z");
            char timeStamp[timeStampSize];
            const char * const fmt = "%Y-%m-%dT%H:%M:%SZ";

#ifdef _MSC_VER
            std::strftime(timeStamp, timeStampSize, fmt, &timeInfo);
#else
            std::strftime(timeStamp, timeStampSize, fmt, timeInfo);
#endif
            return std::string(timeStamp);
        }
    };
}
#endif // CATCH_CONFIG_ENABLE_CHRONO_STRINGMAKER


#ifdef _MSC_VER
#pragma warning(pop)
#endif

#endif // TWOBLUECUBES_CATCH_TOSTRING_H_INCLUDED
