
/*
 Copyright (C) 2000, 2001, 2002 Sadruddin Rejeb

 This file is part of QuantLib, a free-software/open-source library
 for financial quantitative analysts and developers - http://quantlib.org/

 QuantLib is free software developed by the QuantLib Group; you can
 redistribute it and/or modify it under the terms of the QuantLib License;
 either version 1.0, or (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 QuantLib License for more details.

 You should have received a copy of the QuantLib License along with this
 program; if not, please email ferdinando@ametrano.net

 The QuantLib License is also available at http://quantlib.org/license.html
 The members of the QuantLib Group are listed in the QuantLib License
*/
/*! \file treecapfloor.hpp
    \brief Cap/Floor calculated using a tree

    \fullpath
    ql/Pricers/%treecapfloor.hpp
*/

// $Id$

#ifndef quantlib_pricers_tree_cap_floor_h
#define quantlib_pricers_tree_cap_floor_h

#include <ql/Instruments/capfloor.hpp>

namespace QuantLib {

    namespace Pricers {

        //! Cap/Floor priced in a tree
        class TreeCapFloor : public CapFloorPricingEngine {
          public:
            TreeCapFloor() {}
            TreeCapFloor(Size timeSteps) : timeSteps_(timeSteps) {}
            void calculate() const;
          private:
            Size timeSteps_;
        };

    }

}

#endif
