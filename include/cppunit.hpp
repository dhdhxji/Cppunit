#ifndef CPPUNIT_HPP
#define CPPUNIT_HPP

#include "detail/core.hpp"
#include "detail/base_conv_policies.hpp"

namespace CppUnit
{
    using Detail::Core::Unit;
    using Detail::Core::BaseUnit;

    using Detail::BaseConvPolicies::RatioConvPolicy;
    using Detail::Util::unit_cast;
}

#endif // CPPUNIT_HPP