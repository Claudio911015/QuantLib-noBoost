/* -*- mode: c++; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*- */

/*!
Copyright (C) 2014 Jose Aparicio

This file is part of QuantLib, a free-software/open-source library
for financial quantitative analysts and developers - http://quantlib.org/

QuantLib is free software: you can redistribute it and/or modify it
under the terms of the QuantLib license.  You should have received a
copy of the license along with this program; if not, please email
<quantlib-dev@lists.sf.net>. The license is also available online at
<http://quantlib.org/license.shtml>.
This program is distributed in the hope that it will be useful, but WITHOUT
ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
FOR A PARTICULAR PURPOSE.  See the license for more details.
*/

#include <ql/qldefines.hpp>
#include <ql/experimental/math/multidimintegrator.hpp>
#include <ql/experimental/math/multidimquadrature.hpp>
#include <ql/math/integrals/trapezoidintegral.hpp>


#include <chrono>


#include <iomanip>
#include <iostream>

using namespace QuantLib;
using namespace std;

#if defined(QL_ENABLE_SESSIONS)
namespace QuantLib {

    Integer sessionId() { return 0; }

}
#endif

// Correct value is: (e^{-.25} \sqrt{\pi})^{dimension}
struct integrand {
    Real operator()(const std::vector<Real>& arg) const {
        Real sum = 1.;
        for(Size i=0; i<arg.size(); i++) 
            sum *= std::exp(-arg[i]*arg[i]) * std::cos(arg[i]);
        return sum;
    }
};

int main() {
    std::chrono::time_point<std::chrono::steady_clock> startT =  std::chrono::steady_clock::now();
    std::cout << std::endl;

    /* 
    Integrates the function above over several dimensions, the size of the 
    vector argument is the dimension one.
    Both algorithms are not really on the same stand since the quadrature 
    will be incorrect to use if the integrand is not appropiately behaved. Over 
    dimension 3 you might need to modify the points in the integral to retain a 
    sensible computing time.
    */
    Size dimension = 3;
    Real exactSol = std::pow(std::exp(-.25) * 
        std::sqrt(M_PI), static_cast<Real>(dimension));

    std::function<Real(const std::vector<Real>& arg)> f = integrand();

    GaussianQuadMultidimIntegrator intg(dimension, 15);

    startT = std::chrono::steady_clock::now();
    Real valueQuad = intg(f);
    std::chrono::time_point<std::chrono::steady_clock> endT = std::chrono::steady_clock::now();
    Real secondsQuad = static_cast<double>((endT - startT).count()) / 1.0e9;

    std::vector<std::shared_ptr<Integrator> > integrals;
    for(Size i=0; i<dimension; i++)
        integrals.emplace_back(
        std::make_shared<TrapezoidIntegral<Default> >(1.e-4, 20));
    std::vector<Real> a_limits(integrals.size(), -4.);
    std::vector<Real> b_limits(integrals.size(), 4.);
    MultidimIntegral testIntg(integrals);

    startT = std::chrono::steady_clock::now();
    Real valueGrid = testIntg(f, a_limits, b_limits);
    endT = std::chrono::steady_clock::now();
    Real secondsGrid = static_cast<double>((endT - startT).count()) / 1.0e9;

    cout << fixed << setprecision(4);
    cout << endl << "-------------- " << endl
         << "Exact: " << exactSol << endl
         << "Quad: " << valueQuad << endl
         << "Grid: " << valueGrid << endl
         << endl;

    cout
        << "Seconds for Quad: " << secondsQuad << endl
        << "Seconds for Grid: " << secondsGrid << endl;
    return 0;
}
