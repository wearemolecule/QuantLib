
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
/*! \file swaption.cpp
    \brief Swaption

    \fullpath
    ql/Instruments/%swaption.cpp
*/

// $Id$

#include <ql/Instruments/swaption.hpp>
#include <ql/CashFlows/floatingratecoupon.hpp>
#include <ql/Solvers1D/brent.hpp>

namespace QuantLib {

    namespace Instruments {

        using CashFlows::FloatingRateCoupon;

        Swaption::Swaption(
            const SimpleSwap& swap, const Exercise& exercise,
            const RelinkableHandle<TermStructure>& termStructure,
            const Handle<OptionPricingEngine>& engine)
        : Option(engine), swap_(swap), exercise_(exercise),
          termStructure_(termStructure) {}

        Swaption::~Swaption() {}

        void Swaption::setupEngine() const {
            SwaptionParameters* parameters =
                dynamic_cast<SwaptionParameters*>(
                    engine_->parameters());
            QL_REQUIRE(parameters != 0,
                       "pricing engine does not supply needed parameters");

            Date today = termStructure_->settlementDate();
            DayCounter counter = termStructure_->dayCounter();
            Size i;

            const std::vector<Handle<CashFlow> >& fixedLeg = swap_.fixedLeg();
            parameters->payFixed = swap_.payFixedRate();
            parameters->fixedPayTimes.clear();
            parameters->fixedCoupons.clear();
            for (i=0; i<fixedLeg.size(); i++) {
                Time time = counter.yearFraction(today, fixedLeg[i]->date());
                parameters->fixedPayTimes.push_back(time);
                parameters->fixedCoupons.push_back(fixedLeg[i]->amount());
            }

            parameters->floatingResetTimes.clear();
            parameters->floatingPayTimes.clear();
            parameters->nominals.clear();
            const std::vector<Handle<CashFlow> >& floatingLeg = 
                swap_.floatingLeg();
            std::vector<Handle<CashFlow> >::const_iterator begin, end;
            begin = floatingLeg.begin();
            end   = floatingLeg.end();
            for (; begin != end; ++begin) {
                Handle<FloatingRateCoupon> coupon = *begin;
                QL_ENSURE(!coupon.isNull(), "not a floating rate coupon");
                Date beginDate = coupon->accrualStartDate();
                Time time = counter.yearFraction(today, beginDate);
                parameters->floatingResetTimes.push_back(time);
                time = counter.yearFraction(today, coupon->date());
                parameters->floatingPayTimes.push_back(time);
                parameters->nominals.push_back(coupon->nominal());
            }

            parameters->exerciseType = exercise_.type();
            parameters->exerciseTimes.clear();
            const std::vector<Date> dates = exercise_.dates();
            for (i=0; i<dates.size(); i++) {
                Time time = counter.yearFraction(today, dates[i]);
                parameters->exerciseTimes.push_back(time);
            }

        }

    }

    namespace Pricers {

        Arguments* SwaptionPricingEngine::parameters() {
            return &parameters_;
        }

        void SwaptionPricingEngine::validateParameters() const {
            QL_REQUIRE(
                parameters_.fixedPayTimes.size() == 
                parameters_.fixedCoupons.size(), "Invalid pricing parameters");
        }

        const Results* SwaptionPricingEngine::results() const {
            return &results_;
        }

    }

}

