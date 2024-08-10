#ifndef CPPUNIT_HPP
#define CPPUNIT_HPP

#include "detail/base.hpp"
#include "detail/base_conv_policies.hpp"

namespace CppUnit
{
    using Detail::Base::Unit;
    using Detail::Base::BaseUnit;

    using Detail::BaseConvPolicies::RatioConvPolicy;
    using Detail::Util::unit_cast;
}

#endif // CPPUNIT_HPP