/*
 *  Copyright 2016 Two Blue Cubes Ltd. All rights reserved.
 *
 *  Distributed under the Boost Software License, Version 1.0. (See accompanying
 *  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */


#if defined(__clang__)
#    pragma clang diagnostic push
#    pragma clang diagnostic ignored "-Wexit-time-destructors"
#endif
 
#include "catch_stringref.h"

#include <ostream>
#include <cassert>
#include <cstring>



namespace Catch {

    auto getEmptyStringRef() -> StringRef {
        static StringRef s_emptyStringRef("");
        return s_emptyStringRef;
    }
    
    StringRef::StringRef() noexcept
    :   StringRef( getEmptyStringRef() )
    {}
    
    StringRef::StringRef( StringRef const& other ) noexcept
    :   m_start( other.m_start ),
        m_size( other.m_size )
    {}
    
    StringRef::StringRef( StringRef&& other ) noexcept
    :   m_start( other.m_start ),
        m_size( other.m_size ),
        m_data( other.m_data )
    {
        other.m_data = nullptr;
    }
    
    StringRef::StringRef( char const* rawChars ) noexcept
    :   m_start( rawChars ),
        m_size( static_cast<size_type>( std::strlen( rawChars ) ) )
    {
        assert( rawChars != nullptr );
    }
    
    StringRef::StringRef( char const* rawChars, size_type size ) noexcept
    :   m_start( rawChars ),
        m_size( size )
    {
        size_type rawSize = rawChars == nullptr ? 0 : static_cast<size_type>( std::strlen( rawChars ) );
        if( rawSize < size )
            m_size = rawSize;
    }
    
    StringRef::StringRef( std::string const& stdString ) noexcept
    :   m_start( stdString.c_str() ),
        m_size( stdString.size() )
    {}

    StringRef::~StringRef() noexcept {
        delete[] m_data;
    }
    
    auto StringRef::operator = ( StringRef other ) noexcept -> StringRef& {
        swap( other );
        return *this;
    }
    StringRef::operator std::string() const {
        return std::string( m_start, m_size );
    }

    void StringRef::swap( StringRef& other ) noexcept {
        std::swap( m_start, other.m_start );
        std::swap( m_size, other.m_size );
        std::swap( m_data, other.m_data );
    }
    
    auto StringRef::c_str() const -> char const* {
        if( isSubstring() )
           const_cast<StringRef*>( this )->takeOwnership();
        return m_start;
    }
    auto StringRef::data() const noexcept -> char const* {
        return m_start;
    }

    auto StringRef::isOwned() const noexcept -> bool {
        return m_data != nullptr;
    }
    auto StringRef::isSubstring() const noexcept -> bool {
        return m_start[m_size] != '\0';
    }
    
    void StringRef::takeOwnership() {
        if( !isOwned() ) {
            m_data = new char[m_size+1];
            memcpy( m_data, m_start, m_size );
            m_data[m_size] = '\0';
            m_start = m_data;
        }
    }
    auto StringRef::substr( size_type start, size_type size ) const noexcept -> StringRef {
        if( start < m_size )
            return StringRef( m_start+start, size );
        else
            return StringRef();
    }
    auto StringRef::operator == ( StringRef const& other ) const noexcept -> bool {
        return
            size() == other.size() &&
            (std::strncmp( m_start, other.m_start, size() ) == 0);
    }
    auto StringRef::operator != ( StringRef const& other ) const noexcept -> bool {
        return !operator==( other );
    }

    auto StringRef::operator[](size_type index) const noexcept -> char {
        return m_start[index];
    }

    auto StringRef::empty() const noexcept -> bool {
        return m_size == 0;
    }

    auto StringRef::size() const noexcept -> size_type {
        return m_size;
    }
    auto StringRef::numberOfCharacters() const noexcept -> size_type {
        size_type noChars = m_size;
        // Make adjustments for uft encodings
        for( size_type i=0; i < m_size; ++i ) {
            char c = m_start[i];
            if( ( c & 0b11000000 ) == 0b11000000 ) {
                if( ( c & 0b11100000 ) == 0b11000000 )
                    noChars--;
                else if( ( c & 0b11110000 ) == 0b11100000 )
                    noChars-=2;
                else if( ( c & 0b11111000 ) == 0b11110000 )
                    noChars-=3;
            }
        }
        return noChars;
    }

    auto operator + ( StringRef const& lhs, StringRef const& rhs ) -> std::string {
        std::string str;
        str.reserve( lhs.size() + rhs.size() );
        str += lhs;
        str += rhs;
        return str;
    }
    auto operator + ( StringRef const& lhs, const char* rhs ) -> std::string {
        return std::string( lhs ) + std::string( rhs );
    }
    auto operator + ( char const* lhs, StringRef const& rhs ) -> std::string {
        return std::string( lhs ) + std::string( rhs );
    }

    auto operator << ( std::ostream& os, StringRef const& str ) -> std::ostream& {
        return os << str.c_str();
    }
        
} // namespace Catch

#if defined(__clang__)
#    pragma clang diagnostic pop
#endif
