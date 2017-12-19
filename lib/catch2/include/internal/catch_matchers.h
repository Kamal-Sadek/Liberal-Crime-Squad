/*
 *  Created by Phil Nash on 04/03/2012.
 *  Copyright (c) 2012 Two Blue Cubes Ltd. All rights reserved.
 *
 * Distributed under the Boost Software License, Version 1.0. (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */
#ifndef TWOBLUECUBES_CATCH_MATCHERS_HPP_INCLUDED
#define TWOBLUECUBES_CATCH_MATCHERS_HPP_INCLUDED

#include "catch_common.h"

#include <string>
#include <vector>

namespace Catch {
namespace Matchers {
    namespace Impl {

        template<typename ArgT> struct MatchAllOf;
        template<typename ArgT> struct MatchAnyOf;
        template<typename ArgT> struct MatchNotOf;

        class MatcherUntypedBase {
        public:
            MatcherUntypedBase() = default;
            MatcherUntypedBase ( MatcherUntypedBase const& ) = default;
            MatcherUntypedBase& operator = ( MatcherUntypedBase const& ) = delete;
            std::string toString() const;

        protected:
            virtual ~MatcherUntypedBase();
            virtual std::string describe() const = 0;
            mutable std::string m_cachedToString;
        };

        template<typename ObjectT>
        struct MatcherMethod {
            virtual bool match( ObjectT const& arg ) const = 0;
        };
        template<typename PtrT>
        struct MatcherMethod<PtrT*> {
            virtual bool match( PtrT* arg ) const = 0;
        };

        template<typename ObjectT, typename ComparatorT = ObjectT>
        struct MatcherBase : MatcherUntypedBase, MatcherMethod<ObjectT> {


            MatchAllOf<ComparatorT> operator && ( MatcherBase const& other ) const;
            MatchAnyOf<ComparatorT> operator || ( MatcherBase const& other ) const;
            MatchNotOf<ComparatorT> operator ! () const;
        };

        template<typename ArgT>
        struct MatchAllOf : MatcherBase<ArgT> {
            bool match( ArgT const& arg ) const override {
                for( auto matcher : m_matchers ) {
                    if (!matcher->match(arg))
                        return false;
                }
                return true;
            }
            std::string describe() const override {
                std::string description;
                description.reserve( 4 + m_matchers.size()*32 );
                description += "( ";
                bool first = true;
                for( auto matcher : m_matchers ) {
                    if( first )
                        first = false;
                    else
                        description += " and ";
                    description += matcher->toString();
                }
                description += " )";
                return description;
            }

            MatchAllOf<ArgT>& operator && ( MatcherBase<ArgT> const& other ) {
                m_matchers.push_back( &other );
                return *this;
            }

            std::vector<MatcherBase<ArgT> const*> m_matchers;
        };
        template<typename ArgT>
        struct MatchAnyOf : MatcherBase<ArgT> {

            bool match( ArgT const& arg ) const override {
                for( auto matcher : m_matchers ) {
                    if (matcher->match(arg))
                        return true;
                }
                return false;
            }
            std::string describe() const override {
                std::string description;
                description.reserve( 4 + m_matchers.size()*32 );
                description += "( ";
                bool first = true;
                for( auto matcher : m_matchers ) {
                    if( first )
                        first = false;
                    else
                        description += " or ";
                    description += matcher->toString();
                }
                description += " )";
                return description;
            }

            MatchAnyOf<ArgT>& operator || ( MatcherBase<ArgT> const& other ) {
                m_matchers.push_back( &other );
                return *this;
            }

            std::vector<MatcherBase<ArgT> const*> m_matchers;
        };

        template<typename ArgT>
        struct MatchNotOf : MatcherBase<ArgT> {

            MatchNotOf( MatcherBase<ArgT> const& underlyingMatcher ) : m_underlyingMatcher( underlyingMatcher ) {}

            bool match( ArgT const& arg ) const override {
                return !m_underlyingMatcher.match( arg );
            }

            std::string describe() const override {
                return "not " + m_underlyingMatcher.toString();
            }
            MatcherBase<ArgT> const& m_underlyingMatcher;
        };

        template<typename ObjectT, typename ComparatorT>
        MatchAllOf<ComparatorT> MatcherBase<ObjectT, ComparatorT>::operator && ( MatcherBase const& other ) const {
            return MatchAllOf<ComparatorT>() && *this && other;
        }
        template<typename ObjectT, typename ComparatorT>
        MatchAnyOf<ComparatorT> MatcherBase<ObjectT, ComparatorT>::operator || ( MatcherBase const& other ) const {
            return MatchAnyOf<ComparatorT>() || *this || other;
        }
        template<typename ObjectT, typename ComparatorT>
        MatchNotOf<ComparatorT> MatcherBase<ObjectT, ComparatorT>::operator ! () const {
            return MatchNotOf<ComparatorT>( *this );
        }

    } // namespace Impl

} // namespace Matchers

using namespace Matchers;
using Matchers::Impl::MatcherBase;

} // namespace Catch

#endif // TWOBLUECUBES_CATCH_MATCHERS_HPP_INCLUDED
