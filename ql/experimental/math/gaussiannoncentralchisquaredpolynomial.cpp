/* -*- mode: c++; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*- */

/*
 Copyright (C) 2016 Klaus Spanderen

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


#include <ql/errors.hpp>
#include <ql/experimental/math/gaussiannoncentralchisquaredpolynomial.hpp>
#include <ql/math/distributions/chisquaredistribution.hpp>

#include <limits>

#ifndef MULTIPRECISION_NON_CENTRAL_CHI_SQUARED_QUADRATURE

#include <cstdlib>

#endif

namespace QuantLib {

    namespace {

        typedef GaussNonCentralChiSquaredPolynomial::mp_float mp_float;

#ifdef MULTIPRECISION_NON_CENTRAL_CHI_SQUARED_QUADRATURE

            #define NM 36
            #define Power(x,y) powq(x, y)
            #define isNaN(x) isnanq(x)
            #define quietNaN() nanq(NULL)
            mp_float mpfc(const char *s) {return strtoflt128(s, NULL); };
            Real convert2Real(const mp_float& value) {
                return static_cast<Real>(value);
            }
#else
            #define NM 28
            #define Power(x, y) std::pow(x, y)
            #define isNaN(x) std::isnan(x)
            #define quietNaN() std::numeric_limits<Real>::quiet_NaN()

            Real mpfc(const char *s) { return atof(s); }

            Real convert2Real(mp_float value) { return value; }
#endif

#define moment(n, x) mp_float f_##n(double _nu, double _lambda) \
          {                                                             \
             const mp_float lambda(_lambda);                            \
             const mp_float nu(_nu);                                    \
                                                                        \
             return x;                                                  \
          }

        moment(0, 1.0 + 0.0 * (lambda + nu));

        moment(1, lambda + nu);

        moment(2, Power(lambda, 2) + mpfc("2") * lambda * (mpfc("2") + nu) + nu * (mpfc("2") + nu));

        moment(3, Power(lambda, 3) + mpfc("3") * Power(lambda, 2) * (mpfc("4") + nu) +
                  mpfc("3") * lambda * (mpfc("8") + mpfc("6") * nu + Power(nu, 2)) +
                  nu * (mpfc("8") + mpfc("6") * nu + Power(nu, 2)));

        moment(4, Power(lambda, 4) + mpfc("4") * Power(lambda, 3) * (mpfc("6") + nu) +
                  mpfc("6") * Power(lambda, 2) * (mpfc("24") + mpfc("10") * nu + Power(nu, 2)) +
                  mpfc("4") * lambda * (mpfc("48") + mpfc("44") * nu + mpfc("12") * Power(nu, 2) + Power(nu, 3)) +
                  nu * (mpfc("48") + mpfc("44") * nu + mpfc("12") * Power(nu, 2) + Power(nu, 3)));

        moment(5, Power(lambda, 5) + mpfc("5") * Power(lambda, 4) * (mpfc("8") + nu) +
                  mpfc("10") * Power(lambda, 3) * (mpfc("48") + mpfc("14") * nu + Power(nu, 2)) +
                  mpfc("10") * Power(lambda, 2) *
                  (mpfc("192") + mpfc("104") * nu + mpfc("18") * Power(nu, 2) + Power(nu, 3)) + mpfc("5") * lambda *
                                                                                                (mpfc("384") +
                                                                                                 mpfc("400") * nu +
                                                                                                 mpfc("140") *
                                                                                                 Power(nu, 2) +
                                                                                                 mpfc("20") *
                                                                                                 Power(nu, 3) +
                                                                                                 Power(nu, 4)) + nu *
                                                                                                                 (mpfc("384") +
                                                                                                                  mpfc("400") *
                                                                                                                  nu +
                                                                                                                  mpfc("140") *
                                                                                                                  Power(nu,
                                                                                                                        2) +
                                                                                                                  mpfc("20") *
                                                                                                                  Power(nu,
                                                                                                                        3) +
                                                                                                                  Power(nu,
                                                                                                                        4)));

        moment(6, Power(lambda, 6) + mpfc("6") * Power(lambda, 5) * (mpfc("10") + nu) +
                  mpfc("15") * Power(lambda, 4) * (mpfc("80") + mpfc("18") * nu + Power(nu, 2)) +
                  mpfc("20") * Power(lambda, 3) *
                  (mpfc("480") + mpfc("188") * nu + mpfc("24") * Power(nu, 2) + Power(nu, 3)) +
                  mpfc("15") * Power(lambda, 2) *
                  (mpfc("1920") + mpfc("1232") * nu + mpfc("284") * Power(nu, 2) + mpfc("28") * Power(nu, 3) +
                   Power(nu, 4)) + mpfc("6") * lambda *
                                   (mpfc("3840") + mpfc("4384") * nu + mpfc("1800") * Power(nu, 2) +
                                    mpfc("340") * Power(nu, 3) + mpfc("30") * Power(nu, 4) + Power(nu, 5)) + nu *
                                                                                                             (mpfc("3840") +
                                                                                                              mpfc("4384") *
                                                                                                              nu +
                                                                                                              mpfc("1800") *
                                                                                                              Power(nu,
                                                                                                                    2) +
                                                                                                              mpfc("340") *
                                                                                                              Power(nu,
                                                                                                                    3) +
                                                                                                              mpfc("30") *
                                                                                                              Power(nu,
                                                                                                                    4) +
                                                                                                              Power(nu,
                                                                                                                    5)));

        moment(7, Power(lambda, 7) + mpfc("7") * Power(lambda, 6) * (mpfc("12") + nu) +
                  mpfc("21") * Power(lambda, 5) * (mpfc("120") + mpfc("22") * nu + Power(nu, 2)) +
                  mpfc("35") * Power(lambda, 4) *
                  (mpfc("960") + mpfc("296") * nu + mpfc("30") * Power(nu, 2) + Power(nu, 3)) +
                  mpfc("35") * Power(lambda, 3) *
                  (mpfc("5760") + mpfc("2736") * nu + mpfc("476") * Power(nu, 2) + mpfc("36") * Power(nu, 3) +
                   Power(nu, 4)) + mpfc("21") * Power(lambda, 2) *
                                   (mpfc("23040") + mpfc("16704") * nu + mpfc("4640") * Power(nu, 2) +
                                    mpfc("620") * Power(nu, 3) + mpfc("40") * Power(nu, 4) + Power(nu, 5)) +
                  mpfc("7") * lambda *
                  (mpfc("46080") + mpfc("56448") * nu + mpfc("25984") * Power(nu, 2) + mpfc("5880") * Power(nu, 3) +
                   mpfc("700") * Power(nu, 4) + mpfc("42") * Power(nu, 5) + Power(nu, 6)) + nu * (mpfc("46080") +
                                                                                                  mpfc("56448") * nu +
                                                                                                  mpfc("25984") *
                                                                                                  Power(nu, 2) +
                                                                                                  mpfc("5880") *
                                                                                                  Power(nu, 3) +
                                                                                                  mpfc("700") *
                                                                                                  Power(nu, 4) +
                                                                                                  mpfc("42") *
                                                                                                  Power(nu, 5) +
                                                                                                  Power(nu, 6)));

        moment(8, Power(lambda, 8) + mpfc("8") * Power(lambda, 7) * (mpfc("14") + nu) +
                  mpfc("28") * Power(lambda, 6) * (mpfc("168") + mpfc("26") * nu + Power(nu, 2)) +
                  mpfc("56") * Power(lambda, 5) *
                  (mpfc("1680") + mpfc("428") * nu + mpfc("36") * Power(nu, 2) + Power(nu, 3)) +
                  mpfc("70") * Power(lambda, 4) *
                  (mpfc("13440") + mpfc("5104") * nu + mpfc("716") * Power(nu, 2) + mpfc("44") * Power(nu, 3) +
                   Power(nu, 4)) + mpfc("56") * Power(lambda, 3) *
                                   (mpfc("80640") + mpfc("44064") * nu + mpfc("9400") * Power(nu, 2) +
                                    mpfc("980") * Power(nu, 3) + mpfc("50") * Power(nu, 4) + Power(nu, 5)) +
                  mpfc("28") * Power(lambda, 2) *
                  (mpfc("322560") + mpfc("256896") * nu + mpfc("81664") * Power(nu, 2) + mpfc("13320") * Power(nu, 3) +
                   mpfc("1180") * Power(nu, 4) + mpfc("54") * Power(nu, 5) + Power(nu, 6)) + mpfc("8") * lambda *
                                                                                             (mpfc("645120") +
                                                                                              mpfc("836352") * nu +
                                                                                              mpfc("420224") *
                                                                                              Power(nu, 2) +
                                                                                              mpfc("108304") *
                                                                                              Power(nu, 3) +
                                                                                              mpfc("15680") *
                                                                                              Power(nu, 4) +
                                                                                              mpfc("1288") *
                                                                                              Power(nu, 5) +
                                                                                              mpfc("56") *
                                                                                              Power(nu, 6) +
                                                                                              Power(nu, 7)) + nu *
                                                                                                              (mpfc("645120") +
                                                                                                               mpfc("836352") *
                                                                                                               nu +
                                                                                                               mpfc("420224") *
                                                                                                               Power(nu,
                                                                                                                     2) +
                                                                                                               mpfc("108304") *
                                                                                                               Power(nu,
                                                                                                                     3) +
                                                                                                               mpfc("15680") *
                                                                                                               Power(nu,
                                                                                                                     4) +
                                                                                                               mpfc("1288") *
                                                                                                               Power(nu,
                                                                                                                     5) +
                                                                                                               mpfc("56") *
                                                                                                               Power(nu,
                                                                                                                     6) +
                                                                                                               Power(nu,
                                                                                                                     7)));

        moment(9, Power(lambda, 9) + mpfc("9") * Power(lambda, 8) * (mpfc("16") + nu) +
                  mpfc("36") * Power(lambda, 7) * (mpfc("224") + mpfc("30") * nu + Power(nu, 2)) +
                  mpfc("84") * Power(lambda, 6) *
                  (mpfc("2688") + mpfc("584") * nu + mpfc("42") * Power(nu, 2) + Power(nu, 3)) +
                  mpfc("126") * Power(lambda, 5) *
                  (mpfc("26880") + mpfc("8528") * nu + mpfc("1004") * Power(nu, 2) + mpfc("52") * Power(nu, 3) +
                   Power(nu, 4)) + mpfc("126") * Power(lambda, 4) *
                                   (mpfc("215040") + mpfc("95104") * nu + mpfc("16560") * Power(nu, 2) +
                                    mpfc("1420") * Power(nu, 3) + mpfc("60") * Power(nu, 4) + Power(nu, 5)) +
                  mpfc("84") * Power(lambda, 3) *
                  (mpfc("1290240") + mpfc("785664") * nu + mpfc("194464") * Power(nu, 2) +
                   mpfc("25080") * Power(nu, 3) + mpfc("1780") * Power(nu, 4) + mpfc("66") * Power(nu, 5) +
                   Power(nu, 6)) + mpfc("36") * Power(lambda, 2) *
                                   (mpfc("5160960") + mpfc("4432896") * nu + mpfc("1563520") * Power(nu, 2) +
                                    mpfc("294784") * Power(nu, 3) + mpfc("32200") * Power(nu, 4) +
                                    mpfc("2044") * Power(nu, 5) + mpfc("70") * Power(nu, 6) + Power(nu, 7)) +
                  mpfc("9") * lambda * (mpfc("10321920") + mpfc("14026752") * nu + mpfc("7559936") * Power(nu, 2) +
                                        mpfc("2153088") * Power(nu, 3) + mpfc("359184") * Power(nu, 4) +
                                        mpfc("36288") * Power(nu, 5) + mpfc("2184") * Power(nu, 6) +
                                        mpfc("72") * Power(nu, 7) + Power(nu, 8)) + nu * (mpfc("10321920") +
                                                                                          mpfc("14026752") * nu +
                                                                                          mpfc("7559936") *
                                                                                          Power(nu, 2) +
                                                                                          mpfc("2153088") *
                                                                                          Power(nu, 3) +
                                                                                          mpfc("359184") *
                                                                                          Power(nu, 4) +
                                                                                          mpfc("36288") * Power(nu, 5) +
                                                                                          mpfc("2184") * Power(nu, 6) +
                                                                                          mpfc("72") * Power(nu, 7) +
                                                                                          Power(nu, 8)));

        moment(10, Power(lambda, 10) + mpfc("10") * Power(lambda, 9) * (mpfc("18") + nu) +
                   mpfc("45") * Power(lambda, 8) * (mpfc("288") + mpfc("34") * nu + Power(nu, 2)) +
                   mpfc("120") * Power(lambda, 7) *
                   (mpfc("4032") + mpfc("764") * nu + mpfc("48") * Power(nu, 2) + Power(nu, 3)) +
                   mpfc("210") * Power(lambda, 6) *
                   (mpfc("48384") + mpfc("13200") * nu + mpfc("1340") * Power(nu, 2) + mpfc("60") * Power(nu, 3) +
                    Power(nu, 4)) + mpfc("252") * Power(lambda, 5) *
                                    (mpfc("483840") + mpfc("180384") * nu + mpfc("26600") * Power(nu, 2) +
                                     mpfc("1940") * Power(nu, 3) + mpfc("70") * Power(nu, 4) + Power(nu, 5)) +
                   mpfc("210") * Power(lambda, 4) *
                   (mpfc("3870720") + mpfc("1926912") * nu + mpfc("393184") * Power(nu, 2) +
                    mpfc("42120") * Power(nu, 3) + mpfc("2500") * Power(nu, 4) + mpfc("78") * Power(nu, 5) +
                    Power(nu, 6)) + mpfc("120") * Power(lambda, 3) *
                                    (mpfc("23224320") + mpfc("15432192") * nu + mpfc("4286016") * Power(nu, 2) +
                                     mpfc("645904") * Power(nu, 3) + mpfc("57120") * Power(nu, 4) +
                                     mpfc("2968") * Power(nu, 5) + mpfc("84") * Power(nu, 6) + Power(nu, 7)) +
                   mpfc("45") * Power(lambda, 2) *
                   (mpfc("92897280") + mpfc("84953088") * nu + mpfc("32576256") * Power(nu, 2) +
                    mpfc("6869632") * Power(nu, 3) + mpfc("874384") * Power(nu, 4) + mpfc("68992") * Power(nu, 5) +
                    mpfc("3304") * Power(nu, 6) + mpfc("88") * Power(nu, 7) + Power(nu, 8)) + mpfc("10") * lambda *
                                                                                              (mpfc("185794560") +
                                                                                               mpfc("262803456") * nu +
                                                                                               mpfc("150105600") *
                                                                                               Power(nu, 2) +
                                                                                               mpfc("46315520") *
                                                                                               Power(nu, 3) +
                                                                                               mpfc("8618400") *
                                                                                               Power(nu, 4) +
                                                                                               mpfc("1012368") *
                                                                                               Power(nu, 5) +
                                                                                               mpfc("75600") *
                                                                                               Power(nu, 6) +
                                                                                               mpfc("3480") *
                                                                                               Power(nu, 7) +
                                                                                               mpfc("90") *
                                                                                               Power(nu, 8) +
                                                                                               Power(nu, 9)) + nu *
                                                                                                               (mpfc("185794560") +
                                                                                                                mpfc("262803456") *
                                                                                                                nu +
                                                                                                                mpfc("150105600") *
                                                                                                                Power(nu,
                                                                                                                      2) +
                                                                                                                mpfc("46315520") *
                                                                                                                Power(nu,
                                                                                                                      3) +
                                                                                                                mpfc("8618400") *
                                                                                                                Power(nu,
                                                                                                                      4) +
                                                                                                                mpfc("1012368") *
                                                                                                                Power(nu,
                                                                                                                      5) +
                                                                                                                mpfc("75600") *
                                                                                                                Power(nu,
                                                                                                                      6) +
                                                                                                                mpfc("3480") *
                                                                                                                Power(nu,
                                                                                                                      7) +
                                                                                                                mpfc("90") *
                                                                                                                Power(nu,
                                                                                                                      8) +
                                                                                                                Power(nu,
                                                                                                                      9)));

        moment(11, Power(lambda, 11) + mpfc("11") * Power(lambda, 10) * (mpfc("20") + nu) +
                   mpfc("55") * Power(lambda, 9) * (mpfc("360") + mpfc("38") * nu + Power(nu, 2)) +
                   mpfc("165") * Power(lambda, 8) *
                   (mpfc("5760") + mpfc("968") * nu + mpfc("54") * Power(nu, 2) + Power(nu, 3)) +
                   mpfc("330") * Power(lambda, 7) *
                   (mpfc("80640") + mpfc("19312") * nu + mpfc("1724") * Power(nu, 2) + mpfc("68") * Power(nu, 3) +
                    Power(nu, 4)) + mpfc("462") * Power(lambda, 6) *
                                    (mpfc("967680") + mpfc("312384") * nu + mpfc("40000") * Power(nu, 2) +
                                     mpfc("2540") * Power(nu, 3) + mpfc("80") * Power(nu, 4) + Power(nu, 5)) +
                   mpfc("462") * Power(lambda, 5) *
                   (mpfc("9676800") + mpfc("4091520") * nu + mpfc("712384") * Power(nu, 2) +
                    mpfc("65400") * Power(nu, 3) + mpfc("3340") * Power(nu, 4) + mpfc("90") * Power(nu, 5) +
                    Power(nu, 6)) + mpfc("330") * Power(lambda, 4) *
                                    (mpfc("77414400") + mpfc("42408960") * nu + mpfc("9790592") * Power(nu, 2) +
                                     mpfc("1235584") * Power(nu, 3) + mpfc("92120") * Power(nu, 4) +
                                     mpfc("4060") * Power(nu, 5) + mpfc("98") * Power(nu, 6) + Power(nu, 7)) +
                   mpfc("165") * Power(lambda, 3) *
                   (mpfc("464486400") + mpfc("331868160") * nu + mpfc("101152512") * Power(nu, 2) +
                    mpfc("17204096") * Power(nu, 3) + mpfc("1788304") * Power(nu, 4) + mpfc("116480") * Power(nu, 5) +
                    mpfc("4648") * Power(nu, 6) + mpfc("104") * Power(nu, 7) + Power(nu, 8)) +
                   mpfc("55") * Power(lambda, 2) *
                   (mpfc("1857945600") + mpfc("1791959040") * nu + mpfc("736478208") * Power(nu, 2) +
                    mpfc("169968896") * Power(nu, 3) + mpfc("24357312") * Power(nu, 4) +
                    mpfc("2254224") * Power(nu, 5) + mpfc("135072") * Power(nu, 6) + mpfc("5064") * Power(nu, 7) +
                    mpfc("108") * Power(nu, 8) + Power(nu, 9)) + mpfc("11") * lambda *
                                                                 (mpfc("3715891200") + mpfc("5441863680") * nu +
                                                                  mpfc("3264915456") * Power(nu, 2) +
                                                                  mpfc("1076416000") * Power(nu, 3) +
                                                                  mpfc("218683520") * Power(nu, 4) +
                                                                  mpfc("28865760") * Power(nu, 5) +
                                                                  mpfc("2524368") * Power(nu, 6) +
                                                                  mpfc("145200") * Power(nu, 7) +
                                                                  mpfc("5280") * Power(nu, 8) +
                                                                  mpfc("110") * Power(nu, 9) + Power(nu, 10)) + nu *
                                                                                                                (mpfc("3715891200") +
                                                                                                                 mpfc("5441863680") *
                                                                                                                 nu +
                                                                                                                 mpfc("3264915456") *
                                                                                                                 Power(nu,
                                                                                                                       2) +
                                                                                                                 mpfc("1076416000") *
                                                                                                                 Power(nu,
                                                                                                                       3) +
                                                                                                                 mpfc("218683520") *
                                                                                                                 Power(nu,
                                                                                                                       4) +
                                                                                                                 mpfc("28865760") *
                                                                                                                 Power(nu,
                                                                                                                       5) +
                                                                                                                 mpfc("2524368") *
                                                                                                                 Power(nu,
                                                                                                                       6) +
                                                                                                                 mpfc("145200") *
                                                                                                                 Power(nu,
                                                                                                                       7) +
                                                                                                                 mpfc("5280") *
                                                                                                                 Power(nu,
                                                                                                                       8) +
                                                                                                                 mpfc("110") *
                                                                                                                 Power(nu,
                                                                                                                       9) +
                                                                                                                 Power(nu,
                                                                                                                       10)));

        moment(12, Power(lambda, 12) + mpfc("12") * Power(lambda, 11) * (mpfc("22") + nu) +
                   mpfc("66") * Power(lambda, 10) * (mpfc("440") + mpfc("42") * nu + Power(nu, 2)) +
                   mpfc("220") * Power(lambda, 9) *
                   (mpfc("7920") + mpfc("1196") * nu + mpfc("60") * Power(nu, 2) + Power(nu, 3)) +
                   mpfc("495") * Power(lambda, 8) *
                   (mpfc("126720") + mpfc("27056") * nu + mpfc("2156") * Power(nu, 2) + mpfc("76") * Power(nu, 3) +
                    Power(nu, 4)) + mpfc("792") * Power(lambda, 7) *
                                    (mpfc("1774080") + mpfc("505504") * nu + mpfc("57240") * Power(nu, 2) +
                                     mpfc("3220") * Power(nu, 3) + mpfc("90") * Power(nu, 4) + Power(nu, 5)) +
                   mpfc("924") * Power(lambda, 6) *
                   (mpfc("21288960") + mpfc("7840128") * nu + mpfc("1192384") * Power(nu, 2) +
                    mpfc("95880") * Power(nu, 3) + mpfc("4300") * Power(nu, 4) + mpfc("102") * Power(nu, 5) +
                    Power(nu, 6)) + mpfc("792") * Power(lambda, 5) *
                                    (mpfc("212889600") + mpfc("99690240") * nu + mpfc("19763968") * Power(nu, 2) +
                                     mpfc("2151184") * Power(nu, 3) + mpfc("138880") * Power(nu, 4) +
                                     mpfc("5320") * Power(nu, 5) + mpfc("112") * Power(nu, 6) + Power(nu, 7)) +
                   mpfc("495") * Power(lambda, 4) *
                   (mpfc("1703116800") + mpfc("1010411520") * nu + mpfc("257801984") * Power(nu, 2) +
                    mpfc("36973440") * Power(nu, 3) + mpfc("3262224") * Power(nu, 4) + mpfc("181440") * Power(nu, 5) +
                    mpfc("6216") * Power(nu, 6) + mpfc("120") * Power(nu, 7) + Power(nu, 8)) +
                   mpfc("220") * Power(lambda, 3) *
                   (mpfc("10218700800") + mpfc("7765585920") * nu + mpfc("2557223424") * Power(nu, 2) +
                    mpfc("479642624") * Power(nu, 3) + mpfc("56546784") * Power(nu, 4) +
                    mpfc("4350864") * Power(nu, 5) + mpfc("218736") * Power(nu, 6) + mpfc("6936") * Power(nu, 7) +
                    mpfc("126") * Power(nu, 8) + Power(nu, 9)) + mpfc("66") * Power(lambda, 2) *
                                                                 (mpfc("40874803200") + mpfc("41281044480") * nu +
                                                                  mpfc("17994479616") * Power(nu, 2) +
                                                                  mpfc("4475793920") * Power(nu, 3) +
                                                                  mpfc("705829760") * Power(nu, 4) +
                                                                  mpfc("73950240") * Power(nu, 5) +
                                                                  mpfc("5225808") * Power(nu, 6) +
                                                                  mpfc("246480") * Power(nu, 7) +
                                                                  mpfc("7440") * Power(nu, 8) +
                                                                  mpfc("130") * Power(nu, 9) + Power(nu, 10)) +
                   mpfc("12") * lambda *
                   (mpfc("81749606400") + mpfc("123436892160") * nu + mpfc("77270003712") * Power(nu, 2) +
                    mpfc("26946067456") * Power(nu, 3) + mpfc("5887453440") * Power(nu, 4) +
                    mpfc("853730240") * Power(nu, 5) + mpfc("84401856") * Power(nu, 6) +
                    mpfc("5718768") * Power(nu, 7) + mpfc("261360") * Power(nu, 8) + mpfc("7700") * Power(nu, 9) +
                    mpfc("132") * Power(nu, 10) + Power(nu, 11)) + nu *
                                                                   (mpfc("81749606400") + mpfc("123436892160") * nu +
                                                                    mpfc("77270003712") * Power(nu, 2) +
                                                                    mpfc("26946067456") * Power(nu, 3) +
                                                                    mpfc("5887453440") * Power(nu, 4) +
                                                                    mpfc("853730240") * Power(nu, 5) +
                                                                    mpfc("84401856") * Power(nu, 6) +
                                                                    mpfc("5718768") * Power(nu, 7) +
                                                                    mpfc("261360") * Power(nu, 8) +
                                                                    mpfc("7700") * Power(nu, 9) +
                                                                    mpfc("132") * Power(nu, 10) + Power(nu, 11)));

        moment(13, Power(lambda, 13) + mpfc("13") * Power(lambda, 12) * (mpfc("24") + nu) +
                   mpfc("78") * Power(lambda, 11) * (mpfc("528") + mpfc("46") * nu + Power(nu, 2)) +
                   mpfc("286") * Power(lambda, 10) *
                   (mpfc("10560") + mpfc("1448") * nu + mpfc("66") * Power(nu, 2) + Power(nu, 3)) +
                   mpfc("715") * Power(lambda, 9) *
                   (mpfc("190080") + mpfc("36624") * nu + mpfc("2636") * Power(nu, 2) + mpfc("84") * Power(nu, 3) +
                    Power(nu, 4)) + mpfc("1287") * Power(lambda, 8) *
                                    (mpfc("3041280") + mpfc("776064") * nu + mpfc("78800") * Power(nu, 2) +
                                     mpfc("3980") * Power(nu, 3) + mpfc("100") * Power(nu, 4) + Power(nu, 5)) +
                   mpfc("1716") * Power(lambda, 7) *
                   (mpfc("42577920") + mpfc("13906176") * nu + mpfc("1879264") * Power(nu, 2) +
                    mpfc("134520") * Power(nu, 3) + mpfc("5380") * Power(nu, 4) + mpfc("114") * Power(nu, 5) +
                    Power(nu, 6)) + mpfc("1716") * Power(lambda, 6) *
                                    (mpfc("510935040") + mpfc("209452032") * nu + mpfc("36457344") * Power(nu, 2) +
                                     mpfc("3493504") * Power(nu, 3) + mpfc("199080") * Power(nu, 4) +
                                     mpfc("6748") * Power(nu, 5) + mpfc("126") * Power(nu, 6) + Power(nu, 7)) +
                   mpfc("1287") * Power(lambda, 5) *
                   (mpfc("5109350400") + mpfc("2605455360") * nu + mpfc("574025472") * Power(nu, 2) +
                    mpfc("71392384") * Power(nu, 3) + mpfc("5484304") * Power(nu, 4) + mpfc("266560") * Power(nu, 5) +
                    mpfc("8008") * Power(nu, 6) + mpfc("136") * Power(nu, 7) + Power(nu, 8)) +
                   mpfc("715") * Power(lambda, 4) *
                   (mpfc("40874803200") + mpfc("25952993280") * nu + mpfc("7197659136") * Power(nu, 2) +
                    mpfc("1145164544") * Power(nu, 3) + mpfc("115266816") * Power(nu, 4) +
                    mpfc("7616784") * Power(nu, 5) + mpfc("330624") * Power(nu, 6) + mpfc("9096") * Power(nu, 7) +
                    mpfc("144") * Power(nu, 8) + Power(nu, 9)) + mpfc("286") * Power(lambda, 3) *
                                                                 (mpfc("245248819200") + mpfc("196592762880") * nu +
                                                                  mpfc("69138948096") * Power(nu, 2) +
                                                                  mpfc("14068646400") * Power(nu, 3) +
                                                                  mpfc("1836765440") * Power(nu, 4) +
                                                                  mpfc("160967520") * Power(nu, 5) +
                                                                  mpfc("9600528") * Power(nu, 6) +
                                                                  mpfc("385200") * Power(nu, 7) +
                                                                  mpfc("9960") * Power(nu, 8) +
                                                                  mpfc("150") * Power(nu, 9) + Power(nu, 10)) +
                   mpfc("78") * Power(lambda, 2) *
                   (mpfc("980995276800") + mpfc("1031619870720") * nu + mpfc("473148555264") * Power(nu, 2) +
                    mpfc("125413533696") * Power(nu, 3) + mpfc("21415708160") * Power(nu, 4) +
                    mpfc("2480635520") * Power(nu, 5) + mpfc("199369632") * Power(nu, 6) +
                    mpfc("11141328") * Power(nu, 7) + mpfc("425040") * Power(nu, 8) + mpfc("10560") * Power(nu, 9) +
                    mpfc("154") * Power(nu, 10) + Power(nu, 11)) + mpfc("13") * lambda *
                                                                   (mpfc("1961990553600") + mpfc("3044235018240") * nu +
                                                                    mpfc("1977916981248") * Power(nu, 2) +
                                                                    mpfc("723975622656") * Power(nu, 3) +
                                                                    mpfc("168244950016") * Power(nu, 4) +
                                                                    mpfc("26376979200") * Power(nu, 5) +
                                                                    mpfc("2879374784") * Power(nu, 6) +
                                                                    mpfc("221652288") * Power(nu, 7) +
                                                                    mpfc("11991408") * Power(nu, 8) +
                                                                    mpfc("446160") * Power(nu, 9) +
                                                                    mpfc("10868") * Power(nu, 10) +
                                                                    mpfc("156") * Power(nu, 11) + Power(nu, 12)) + nu *
                                                                                                                   (mpfc("1961990553600") +
                                                                                                                    mpfc("3044235018240") *
                                                                                                                    nu +
                                                                                                                    mpfc("1977916981248") *
                                                                                                                    Power(nu,
                                                                                                                          2) +
                                                                                                                    mpfc("723975622656") *
                                                                                                                    Power(nu,
                                                                                                                          3) +
                                                                                                                    mpfc("168244950016") *
                                                                                                                    Power(nu,
                                                                                                                          4) +
                                                                                                                    mpfc("26376979200") *
                                                                                                                    Power(nu,
                                                                                                                          5) +
                                                                                                                    mpfc("2879374784") *
                                                                                                                    Power(nu,
                                                                                                                          6) +
                                                                                                                    mpfc("221652288") *
                                                                                                                    Power(nu,
                                                                                                                          7) +
                                                                                                                    mpfc("11991408") *
                                                                                                                    Power(nu,
                                                                                                                          8) +
                                                                                                                    mpfc("446160") *
                                                                                                                    Power(nu,
                                                                                                                          9) +
                                                                                                                    mpfc("10868") *
                                                                                                                    Power(nu,
                                                                                                                          10) +
                                                                                                                    mpfc("156") *
                                                                                                                    Power(nu,
                                                                                                                          11) +
                                                                                                                    Power(nu,
                                                                                                                          12)));

        moment(14, Power(lambda, 14) + mpfc("14") * Power(lambda, 13) * (mpfc("26") + nu) +
                   mpfc("91") * Power(lambda, 12) * (mpfc("624") + mpfc("50") * nu + Power(nu, 2)) +
                   mpfc("364") * Power(lambda, 11) *
                   (mpfc("13728") + mpfc("1724") * nu + mpfc("72") * Power(nu, 2) + Power(nu, 3)) +
                   mpfc("1001") * Power(lambda, 10) *
                   (mpfc("274560") + mpfc("48208") * nu + mpfc("3164") * Power(nu, 2) + mpfc("92") * Power(nu, 3) +
                    Power(nu, 4)) + mpfc("2002") * Power(lambda, 9) *
                                    (mpfc("4942080") + mpfc("1142304") * nu + mpfc("105160") * Power(nu, 2) +
                                     mpfc("4820") * Power(nu, 3) + mpfc("110") * Power(nu, 4) + Power(nu, 5)) +
                   mpfc("3003") * Power(lambda, 8) *
                   (mpfc("79073280") + mpfc("23218944") * nu + mpfc("2824864") * Power(nu, 2) +
                    mpfc("182280") * Power(nu, 3) + mpfc("6580") * Power(nu, 4) + mpfc("126") * Power(nu, 5) +
                    Power(nu, 6)) + mpfc("3432") * Power(lambda, 7) *
                                    (mpfc("1107025920") + mpfc("404138496") * nu + mpfc("62767040") * Power(nu, 2) +
                                     mpfc("5376784") * Power(nu, 3) + mpfc("274400") * Power(nu, 4) +
                                     mpfc("8344") * Power(nu, 5) + mpfc("140") * Power(nu, 6) + Power(nu, 7)) +
                   mpfc("3003") * Power(lambda, 6) *
                   (mpfc("13284311040") + mpfc("5956687872") * nu + mpfc("1157342976") * Power(nu, 2) +
                    mpfc("127288448") * Power(nu, 3) + mpfc("8669584") * Power(nu, 4) + mpfc("374528") * Power(nu, 5) +
                    mpfc("10024") * Power(nu, 6) + mpfc("152") * Power(nu, 7) + Power(nu, 8)) +
                   mpfc("2002") * Power(lambda, 5) *
                   (mpfc("132843110400") + mpfc("72851189760") * nu + mpfc("17530117632") * Power(nu, 2) +
                    mpfc("2430227456") * Power(nu, 3) + mpfc("213984288") * Power(nu, 4) +
                    mpfc("12414864") * Power(nu, 5) + mpfc("474768") * Power(nu, 6) + mpfc("11544") * Power(nu, 7) +
                    mpfc("162") * Power(nu, 8) + Power(nu, 9)) + mpfc("1001") * Power(lambda, 4) *
                                                                 (mpfc("1062744883200") + mpfc("715652628480") * nu +
                                                                  mpfc("213092130816") * Power(nu, 2) +
                                                                  mpfc("36971937280") * Power(nu, 3) +
                                                                  mpfc("4142101760") * Power(nu, 4) +
                                                                  mpfc("313303200") * Power(nu, 5) +
                                                                  mpfc("16213008") * Power(nu, 6) +
                                                                  mpfc("567120") * Power(nu, 7) +
                                                                  mpfc("12840") * Power(nu, 8) +
                                                                  mpfc("170") * Power(nu, 9) + Power(nu, 10)) +
                   mpfc("364") * Power(lambda, 3) *
                   (mpfc("6376469299200") + mpfc("5356660654080") * nu + mpfc("1994205413376") * Power(nu, 2) +
                    mpfc("434923754496") * Power(nu, 3) + mpfc("61824547840") * Power(nu, 4) +
                    mpfc("6021920960") * Power(nu, 5) + mpfc("410581248") * Power(nu, 6) +
                    mpfc("19615728") * Power(nu, 7) + mpfc("644160") * Power(nu, 8) + mpfc("13860") * Power(nu, 9) +
                    mpfc("176") * Power(nu, 10) + Power(nu, 11)) + mpfc("91") * Power(lambda, 2) *
                                                                   (mpfc("25505877196800") +
                                                                    mpfc("27803111915520") * nu +
                                                                    mpfc("13333482307584") * Power(nu, 2) +
                                                                    mpfc("3733900431360") * Power(nu, 3) +
                                                                    mpfc("682221945856") * Power(nu, 4) +
                                                                    mpfc("85912231680") * Power(nu, 5) +
                                                                    mpfc("7664245952") * Power(nu, 6) +
                                                                    mpfc("489044160") * Power(nu, 7) +
                                                                    mpfc("22192368") * Power(nu, 8) +
                                                                    mpfc("699600") * Power(nu, 9) +
                                                                    mpfc("14564") * Power(nu, 10) +
                                                                    mpfc("180") * Power(nu, 11) + Power(nu, 12)) +
                   mpfc("14") * lambda *
                   (mpfc("51011754393600") + mpfc("81112101027840") * nu + mpfc("54470076530688") * Power(nu, 2) +
                    mpfc("20801283170304") * Power(nu, 3) + mpfc("5098344323072") * Power(nu, 4) +
                    mpfc("854046409216") * Power(nu, 5) + mpfc("101240723584") * Power(nu, 6) +
                    mpfc("8642334272") * Power(nu, 7) + mpfc("533428896") * Power(nu, 8) +
                    mpfc("23591568") * Power(nu, 9) + mpfc("728728") * Power(nu, 10) + mpfc("14924") * Power(nu, 11) +
                    mpfc("182") * Power(nu, 12) + Power(nu, 13)) + nu * (mpfc("51011754393600") +
                                                                         mpfc("81112101027840") * nu +
                                                                         mpfc("54470076530688") * Power(nu, 2) +
                                                                         mpfc("20801283170304") * Power(nu, 3) +
                                                                         mpfc("5098344323072") * Power(nu, 4) +
                                                                         mpfc("854046409216") * Power(nu, 5) +
                                                                         mpfc("101240723584") * Power(nu, 6) +
                                                                         mpfc("8642334272") * Power(nu, 7) +
                                                                         mpfc("533428896") * Power(nu, 8) +
                                                                         mpfc("23591568") * Power(nu, 9) +
                                                                         mpfc("728728") * Power(nu, 10) +
                                                                         mpfc("14924") * Power(nu, 11) +
                                                                         mpfc("182") * Power(nu, 12) + Power(nu, 13)));

        moment(15, Power(lambda, 15) + mpfc("15") * Power(lambda, 14) * (mpfc("28") + nu) +
                   mpfc("105") * Power(lambda, 13) * (mpfc("728") + mpfc("54") * nu + Power(nu, 2)) +
                   mpfc("455") * Power(lambda, 12) *
                   (mpfc("17472") + mpfc("2024") * nu + mpfc("78") * Power(nu, 2) + Power(nu, 3)) +
                   mpfc("1365") * Power(lambda, 11) *
                   (mpfc("384384") + mpfc("62000") * nu + mpfc("3740") * Power(nu, 2) + mpfc("100") * Power(nu, 3) +
                    Power(nu, 4)) + mpfc("3003") * Power(lambda, 10) *
                                    (mpfc("7687680") + mpfc("1624384") * nu + mpfc("136800") * Power(nu, 2) +
                                     mpfc("5740") * Power(nu, 3) + mpfc("120") * Power(nu, 4) + Power(nu, 5)) +
                   mpfc("5005") * Power(lambda, 9) *
                   (mpfc("138378240") + mpfc("36926592") * nu + mpfc("4086784") * Power(nu, 2) +
                    mpfc("240120") * Power(nu, 3) + mpfc("7900") * Power(nu, 4) + mpfc("138") * Power(nu, 5) +
                    Power(nu, 6)) + mpfc("6435") * Power(lambda, 8) *
                                    (mpfc("2214051840") + mpfc("729203712") * nu + mpfc("102315136") * Power(nu, 2) +
                                     mpfc("7928704") * Power(nu, 3) + mpfc("366520") * Power(nu, 4) +
                                     mpfc("10108") * Power(nu, 5) + mpfc("154") * Power(nu, 6) + Power(nu, 7)) +
                   mpfc("6435") * Power(lambda, 7) *
                   (mpfc("30996725760") + mpfc("12422903808") * nu + mpfc("2161615616") * Power(nu, 2) +
                    mpfc("213316992") * Power(nu, 3) + mpfc("13059984") * Power(nu, 4) + mpfc("508032") * Power(nu, 5) +
                    mpfc("12264") * Power(nu, 6) + mpfc("168") * Power(nu, 7) + Power(nu, 8)) +
                   mpfc("5005") * Power(lambda, 6) *
                   (mpfc("371960709120") + mpfc("180071571456") * nu + mpfc("38362291200") * Power(nu, 2) +
                    mpfc("4721419520") * Power(nu, 3) + mpfc("370036800") * Power(nu, 4) +
                    mpfc("19156368") * Power(nu, 5) + mpfc("655200") * Power(nu, 6) + mpfc("14280") * Power(nu, 7) +
                    mpfc("180") * Power(nu, 8) + Power(nu, 9)) + mpfc("3003") * Power(lambda, 5) *
                                                                 (mpfc("3719607091200") + mpfc("2172676423680") * nu +
                                                                  mpfc("563694483456") * Power(nu, 2) +
                                                                  mpfc("85576486400") * Power(nu, 3) +
                                                                  mpfc("8421787520") * Power(nu, 4) +
                                                                  mpfc("561600480") * Power(nu, 5) +
                                                                  mpfc("25708368") * Power(nu, 6) +
                                                                  mpfc("798000") * Power(nu, 7) +
                                                                  mpfc("16080") * Power(nu, 8) +
                                                                  mpfc("190") * Power(nu, 9) + Power(nu, 10)) +
                   mpfc("1365") * Power(lambda, 4) *
                   (mpfc("29756856729600") + mpfc("21101018480640") * nu + mpfc("6682232291328") * Power(nu, 2) +
                    mpfc("1248306374656") * Power(nu, 3) + mpfc("152950786560") * Power(nu, 4) +
                    mpfc("12914591360") * Power(nu, 5) + mpfc("767267424") * Power(nu, 6) +
                    mpfc("32092368") * Power(nu, 7) + mpfc("926640") * Power(nu, 8) + mpfc("17600") * Power(nu, 9) +
                    mpfc("198") * Power(nu, 10) + Power(nu, 11)) + mpfc("455") * Power(lambda, 3) *
                                                                   (mpfc("178541140377600") +
                                                                    mpfc("156362967613440") * nu +
                                                                    mpfc("61194412228608") * Power(nu, 2) +
                                                                    mpfc("14172070539264") * Power(nu, 3) +
                                                                    mpfc("2166011094016") * Power(nu, 4) +
                                                                    mpfc("230438334720") * Power(nu, 5) +
                                                                    mpfc("17518195904") * Power(nu, 6) +
                                                                    mpfc("959821632") * Power(nu, 7) +
                                                                    mpfc("37652208") * Power(nu, 8) +
                                                                    mpfc("1032240") * Power(nu, 9) +
                                                                    mpfc("18788") * Power(nu, 10) +
                                                                    mpfc("204") * Power(nu, 11) + Power(nu, 12)) +
                   mpfc("105") * Power(lambda, 2) *
                   (mpfc("714164561510400") + mpfc("803993010831360") * nu + mpfc("401140616527872") * Power(nu, 2) +
                    mpfc("117882694385664") * Power(nu, 3) + mpfc("22836114915328") * Power(nu, 4) +
                    mpfc("3087764432896") * Power(nu, 5) + mpfc("300511118336") * Power(nu, 6) +
                    mpfc("21357482432") * Power(nu, 7) + mpfc("1110430464") * Power(nu, 8) +
                    mpfc("41781168") * Power(nu, 9) + mpfc("1107392") * Power(nu, 10) + mpfc("19604") * Power(nu, 11) +
                    mpfc("208") * Power(nu, 12) + Power(nu, 13)) + mpfc("15") * lambda * (mpfc("1428329123020800") +
                                                                                          mpfc("2322150583173120") *
                                                                                          nu +
                                                                                          mpfc("1606274243887104") *
                                                                                          Power(nu, 2) +
                                                                                          mpfc("636906005299200") *
                                                                                          Power(nu, 3) +
                                                                                          mpfc("163554924216320") *
                                                                                          Power(nu, 4) +
                                                                                          mpfc("29011643781120") *
                                                                                          Power(nu, 5) +
                                                                                          mpfc("3688786669568") *
                                                                                          Power(nu, 6) +
                                                                                          mpfc("343226083200") *
                                                                                          Power(nu, 7) +
                                                                                          mpfc("23578343360") *
                                                                                          Power(nu, 8) +
                                                                                          mpfc("1193992800") *
                                                                                          Power(nu, 9) +
                                                                                          mpfc("43995952") *
                                                                                          Power(nu, 10) +
                                                                                          mpfc("1146600") *
                                                                                          Power(nu, 11) +
                                                                                          mpfc("20020") *
                                                                                          Power(nu, 12) +
                                                                                          mpfc("210") * Power(nu, 13) +
                                                                                          Power(nu, 14)) + nu *
                                                                                                           (mpfc("1428329123020800") +
                                                                                                            mpfc("2322150583173120") *
                                                                                                            nu +
                                                                                                            mpfc("1606274243887104") *
                                                                                                            Power(nu,
                                                                                                                  2) +
                                                                                                            mpfc("636906005299200") *
                                                                                                            Power(nu,
                                                                                                                  3) +
                                                                                                            mpfc("163554924216320") *
                                                                                                            Power(nu,
                                                                                                                  4) +
                                                                                                            mpfc("29011643781120") *
                                                                                                            Power(nu,
                                                                                                                  5) +
                                                                                                            mpfc("3688786669568") *
                                                                                                            Power(nu,
                                                                                                                  6) +
                                                                                                            mpfc("343226083200") *
                                                                                                            Power(nu,
                                                                                                                  7) +
                                                                                                            mpfc("23578343360") *
                                                                                                            Power(nu,
                                                                                                                  8) +
                                                                                                            mpfc("1193992800") *
                                                                                                            Power(nu,
                                                                                                                  9) +
                                                                                                            mpfc("43995952") *
                                                                                                            Power(nu,
                                                                                                                  10) +
                                                                                                            mpfc("1146600") *
                                                                                                            Power(nu,
                                                                                                                  11) +
                                                                                                            mpfc("20020") *
                                                                                                            Power(nu,
                                                                                                                  12) +
                                                                                                            mpfc("210") *
                                                                                                            Power(nu,
                                                                                                                  13) +
                                                                                                            Power(nu,
                                                                                                                  14)));

        moment(16, Power(lambda, 16) + mpfc("16") * Power(lambda, 15) * (mpfc("30") + nu) +
                   mpfc("120") * Power(lambda, 14) * (mpfc("840") + mpfc("58") * nu + Power(nu, 2)) +
                   mpfc("560") * Power(lambda, 13) *
                   (mpfc("21840") + mpfc("2348") * nu + mpfc("84") * Power(nu, 2) + Power(nu, 3)) +
                   mpfc("1820") * Power(lambda, 12) *
                   (mpfc("524160") + mpfc("78192") * nu + mpfc("4364") * Power(nu, 2) + mpfc("108") * Power(nu, 3) +
                    Power(nu, 4)) + mpfc("4368") * Power(lambda, 11) *
                                    (mpfc("11531520") + mpfc("2244384") * nu + mpfc("174200") * Power(nu, 2) +
                                     mpfc("6740") * Power(nu, 3) + mpfc("130") * Power(nu, 4) + Power(nu, 5)) +
                   mpfc("8008") * Power(lambda, 10) *
                   (mpfc("230630400") + mpfc("56419200") * nu + mpfc("5728384") * Power(nu, 2) +
                    mpfc("309000") * Power(nu, 3) + mpfc("9340") * Power(nu, 4) + mpfc("150") * Power(nu, 5) +
                    Power(nu, 6)) + mpfc("11440") * Power(lambda, 9) *
                                    (mpfc("4151347200") + mpfc("1246176000") * nu + mpfc("159530112") * Power(nu, 2) +
                                     mpfc("11290384") * Power(nu, 3) + mpfc("477120") * Power(nu, 4) +
                                     mpfc("12040") * Power(nu, 5) + mpfc("168") * Power(nu, 6) + Power(nu, 7)) +
                   mpfc("12870") * Power(lambda, 8) *
                   (mpfc("66421555200") + mpfc("24090163200") * nu + mpfc("3798657792") * Power(nu, 2) +
                    mpfc("340176256") * Power(nu, 3) + mpfc("18924304") * Power(nu, 4) + mpfc("669760") * Power(nu, 5) +
                    mpfc("14728") * Power(nu, 6) + mpfc("184") * Power(nu, 7) + Power(nu, 8)) +
                   mpfc("11440") * Power(lambda, 7) *
                   (mpfc("929901772800") + mpfc("403683840000") * nu + mpfc("77271372288") * Power(nu, 2) +
                    mpfc("8561125376") * Power(nu, 3) + mpfc("605116512") * Power(nu, 4) +
                    mpfc("28300944") * Power(nu, 5) + mpfc("875952") * Power(nu, 6) + mpfc("17304") * Power(nu, 7) +
                    mpfc("198") * Power(nu, 8) + Power(nu, 9)) + mpfc("8008") * Power(lambda, 6) *
                                                                 (mpfc("11158821273600") + mpfc("5774107852800") * nu +
                                                                  mpfc("1330940307456") * Power(nu, 2) +
                                                                  mpfc("180004876800") * Power(nu, 3) +
                                                                  mpfc("15822523520") * Power(nu, 4) +
                                                                  mpfc("944727840") * Power(nu, 5) +
                                                                  mpfc("38812368") * Power(nu, 6) +
                                                                  mpfc("1083600") * Power(nu, 7) +
                                                                  mpfc("19680") * Power(nu, 8) +
                                                                  mpfc("210") * Power(nu, 9) + Power(nu, 10)) +
                   mpfc("4368") * Power(lambda, 5) *
                   (mpfc("111588212736000") + mpfc("68899899801600") * nu + mpfc("19083510927360") * Power(nu, 2) +
                    mpfc("3130989075456") * Power(nu, 3) + mpfc("338230112000") * Power(nu, 4) +
                    mpfc("25269801920") * Power(nu, 5) + mpfc("1332851520") * Power(nu, 6) +
                    mpfc("49648368") * Power(nu, 7) + mpfc("1280400") * Power(nu, 8) + mpfc("21780") * Power(nu, 9) +
                    mpfc("220") * Power(nu, 10) + Power(nu, 11)) + mpfc("1820") * Power(lambda, 4) *
                                                                   (mpfc("892705701888000") +
                                                                    mpfc("662787411148800") * nu +
                                                                    mpfc("221567987220480") * Power(nu, 2) +
                                                                    mpfc("44131423531008") * Power(nu, 3) +
                                                                    mpfc("5836829971456") * Power(nu, 4) +
                                                                    mpfc("540388527360") * Power(nu, 5) +
                                                                    mpfc("35932614080") * Power(nu, 6) +
                                                                    mpfc("1730038464") * Power(nu, 7) +
                                                                    mpfc("59891568") * Power(nu, 8) +
                                                                    mpfc("1454640") * Power(nu, 9) +
                                                                    mpfc("23540") * Power(nu, 10) +
                                                                    mpfc("228") * Power(nu, 11) + Power(nu, 12)) +
                   mpfc("560") * Power(lambda, 3) *
                   (mpfc("5356234211328000") + mpfc("4869430168780800") * nu + mpfc("1992195334471680") * Power(nu, 2) +
                    mpfc("486356528406528") * Power(nu, 3) + mpfc("79152403359744") * Power(nu, 4) +
                    mpfc("9079161135616") * Power(nu, 5) + mpfc("755984211840") * Power(nu, 6) +
                    mpfc("46312844864") * Power(nu, 7) + mpfc("2089387872") * Power(nu, 8) +
                    mpfc("68619408") * Power(nu, 9) + mpfc("1595880") * Power(nu, 10) + mpfc("24908") * Power(nu, 11) +
                    mpfc("234") * Power(nu, 12) + Power(nu, 13)) + mpfc("120") * Power(lambda, 2) *
                                                                   (mpfc("21424936845312000") +
                                                                    mpfc("24833954886451200") * nu +
                                                                    mpfc("12838211506667520") * Power(nu, 2) +
                                                                    mpfc("3937621448097792") * Power(nu, 3) +
                                                                    mpfc("802966141845504") * Power(nu, 4) +
                                                                    mpfc("115469047902208") * Power(nu, 5) +
                                                                    mpfc("12103097982976") * Power(nu, 6) +
                                                                    mpfc("941235591296") * Power(nu, 7) +
                                                                    mpfc("54670396352") * Power(nu, 8) +
                                                                    mpfc("2363865504") * Power(nu, 9) +
                                                                    mpfc("75002928") * Power(nu, 10) +
                                                                    mpfc("1695512") * Power(nu, 11) +
                                                                    mpfc("25844") * Power(nu, 12) +
                                                                    mpfc("238") * Power(nu, 13) + Power(nu, 14)) +
                   mpfc("16") * lambda * (mpfc("42849873690624000") + mpfc("71092846618214400") * nu +
                                          mpfc("50510377899786240") * Power(nu, 2) +
                                          mpfc("20713454402863104") * Power(nu, 3) +
                                          mpfc("5543553731788800") * Power(nu, 4) +
                                          mpfc("1033904237649920") * Power(nu, 5) +
                                          mpfc("139675243868160") * Power(nu, 6) +
                                          mpfc("13985569165568") * Power(nu, 7) + mpfc("1050576384000") * Power(nu, 8) +
                                          mpfc("59398127360") * Power(nu, 9) + mpfc("2513871360") * Power(nu, 10) +
                                          mpfc("78393952") * Power(nu, 11) + mpfc("1747200") * Power(nu, 12) +
                                          mpfc("26320") * Power(nu, 13) + mpfc("240") * Power(nu, 14) + Power(nu, 15)) +
                   nu * (mpfc("42849873690624000") + mpfc("71092846618214400") * nu +
                         mpfc("50510377899786240") * Power(nu, 2) + mpfc("20713454402863104") * Power(nu, 3) +
                         mpfc("5543553731788800") * Power(nu, 4) + mpfc("1033904237649920") * Power(nu, 5) +
                         mpfc("139675243868160") * Power(nu, 6) + mpfc("13985569165568") * Power(nu, 7) +
                         mpfc("1050576384000") * Power(nu, 8) + mpfc("59398127360") * Power(nu, 9) +
                         mpfc("2513871360") * Power(nu, 10) + mpfc("78393952") * Power(nu, 11) +
                         mpfc("1747200") * Power(nu, 12) + mpfc("26320") * Power(nu, 13) + mpfc("240") * Power(nu, 14) +
                         Power(nu, 15)));

        moment(17, Power(lambda, 17) + mpfc("17") * Power(lambda, 16) * (mpfc("32") + nu) +
                   mpfc("136") * Power(lambda, 15) * (mpfc("960") + mpfc("62") * nu + Power(nu, 2)) +
                   mpfc("680") * Power(lambda, 14) *
                   (mpfc("26880") + mpfc("2696") * nu + mpfc("90") * Power(nu, 2) + Power(nu, 3)) +
                   mpfc("2380") * Power(lambda, 13) *
                   (mpfc("698880") + mpfc("96976") * nu + mpfc("5036") * Power(nu, 2) + mpfc("116") * Power(nu, 3) +
                    Power(nu, 4)) + mpfc("6188") * Power(lambda, 12) *
                                    (mpfc("16773120") + mpfc("3026304") * nu + mpfc("217840") * Power(nu, 2) +
                                     mpfc("7820") * Power(nu, 3) + mpfc("140") * Power(nu, 4) + Power(nu, 5)) +
                   mpfc("12376") * Power(lambda, 11) *
                   (mpfc("369008640") + mpfc("83351808") * nu + mpfc("7818784") * Power(nu, 2) +
                    mpfc("389880") * Power(nu, 3) + mpfc("10900") * Power(nu, 4) + mpfc("162") * Power(nu, 5) +
                    Power(nu, 6)) + mpfc("19448") * Power(lambda, 10) *
                                    (mpfc("7380172800") + mpfc("2036044800") * nu + mpfc("239727488") * Power(nu, 2) +
                                     mpfc("15616384") * Power(nu, 3) + mpfc("607880") * Power(nu, 4) +
                                     mpfc("14140") * Power(nu, 5) + mpfc("182") * Power(nu, 6) + Power(nu, 7)) +
                   mpfc("24310") * Power(lambda, 9) *
                   (mpfc("132843110400") + mpfc("44028979200") * nu + mpfc("6351139584") * Power(nu, 2) +
                    mpfc("520822400") * Power(nu, 3) + mpfc("26558224") * Power(nu, 4) + mpfc("862400") * Power(nu, 5) +
                    mpfc("17416") * Power(nu, 6) + mpfc("200") * Power(nu, 7) + Power(nu, 8)) +
                   mpfc("24310") * Power(lambda, 8) *
                   (mpfc("2125489766400") + mpfc("837306777600") * nu + mpfc("145647212544") * Power(nu, 2) +
                    mpfc("14684297984") * Power(nu, 3) + mpfc("945753984") * Power(nu, 4) +
                    mpfc("40356624") * Power(nu, 5) + mpfc("1141056") * Power(nu, 6) + mpfc("20616") * Power(nu, 7) +
                    mpfc("216") * Power(nu, 8) + Power(nu, 9)) + mpfc("19448") * Power(lambda, 7) *
                                                                 (mpfc("29756856729600") + mpfc("13847784652800") * nu +
                                                                  mpfc("2876367753216") * Power(nu, 2) +
                                                                  mpfc("351227384320") * Power(nu, 3) +
                                                                  mpfc("27924853760") * Power(nu, 4) +
                                                                  mpfc("1510746720") * Power(nu, 5) +
                                                                  mpfc("56331408") * Power(nu, 6) +
                                                                  mpfc("1429680") * Power(nu, 7) +
                                                                  mpfc("23640") * Power(nu, 8) +
                                                                  mpfc("230") * Power(nu, 9) + Power(nu, 10)) +
                   mpfc("12376") * Power(lambda, 6) *
                   (mpfc("357082280755200") + mpfc("195930272563200") * nu + mpfc("48364197691392") * Power(nu, 2) +
                    mpfc("7091096365056") * Power(nu, 3) + mpfc("686325629440") * Power(nu, 4) +
                    mpfc("46053814400") * Power(nu, 5) + mpfc("2186723616") * Power(nu, 6) +
                    mpfc("73487568") * Power(nu, 7) + mpfc("1713360") * Power(nu, 8) + mpfc("26400") * Power(nu, 9) +
                    mpfc("242") * Power(nu, 10) + Power(nu, 11)) + mpfc("6188") * Power(lambda, 5) *
                                                                   (mpfc("3570822807552000") +
                                                                    mpfc("2316385006387200") * nu +
                                                                    mpfc("679572249477120") * Power(nu, 2) +
                                                                    mpfc("119275161341952") * Power(nu, 3) +
                                                                    mpfc("13954352659456") * Power(nu, 4) +
                                                                    mpfc("1146863773440") * Power(nu, 5) +
                                                                    mpfc("67921050560") * Power(nu, 6) +
                                                                    mpfc("2921599296") * Power(nu, 7) +
                                                                    mpfc("90621168") * Power(nu, 8) +
                                                                    mpfc("1977360") * Power(nu, 9) +
                                                                    mpfc("28820") * Power(nu, 10) +
                                                                    mpfc("252") * Power(nu, 11) + Power(nu, 12)) +
                   mpfc("2380") * Power(lambda, 4) * (mpfc("28566582460416000") + mpfc("22101902858649600") * nu +
                                                      mpfc("7752963002204160") * Power(nu, 2) +
                                                      mpfc("1633773540212736") * Power(nu, 3) +
                                                      mpfc("230909982617600") * Power(nu, 4) +
                                                      mpfc("23129262846976") * Power(nu, 5) +
                                                      mpfc("1690232177920") * Power(nu, 6) +
                                                      mpfc("91293844928") * Power(nu, 7) +
                                                      mpfc("3646568640") * Power(nu, 8) +
                                                      mpfc("106440048") * Power(nu, 9) +
                                                      mpfc("2207920") * Power(nu, 10) + mpfc("30836") * Power(nu, 11) +
                                                      mpfc("260") * Power(nu, 12) + Power(nu, 13)) +
                   mpfc("680") * Power(lambda, 3) * (mpfc("171399494762496000") + mpfc("161177999612313600") * nu +
                                                     mpfc("68619680871874560") * Power(nu, 2) +
                                                     mpfc("17555604243480576") * Power(nu, 3) +
                                                     mpfc("3019233435918336") * Power(nu, 4) +
                                                     mpfc("369685559699456") * Power(nu, 5) +
                                                     mpfc("33270655914496") * Power(nu, 6) +
                                                     mpfc("2237995247488") * Power(nu, 7) +
                                                     mpfc("113173256768") * Power(nu, 8) +
                                                     mpfc("4285208928") * Power(nu, 9) +
                                                     mpfc("119687568") * Power(nu, 10) +
                                                     mpfc("2392936") * Power(nu, 11) + mpfc("32396") * Power(nu, 12) +
                                                     mpfc("266") * Power(nu, 13) + Power(nu, 14)) +
                   mpfc("136") * Power(lambda, 2) * (mpfc("685597979049984000") + mpfc("816111493211750400") * nu +
                                                     mpfc("435656723099811840") * Power(nu, 2) +
                                                     mpfc("138842097845796864") * Power(nu, 3) +
                                                     mpfc("29632537987153920") * Power(nu, 4) +
                                                     mpfc("4497975674716160") * Power(nu, 5) +
                                                     mpfc("502768183357440") * Power(nu, 6) +
                                                     mpfc("42222636904448") * Power(nu, 7) +
                                                     mpfc("2690688274560") * Power(nu, 8) +
                                                     mpfc("130314092480") * Power(nu, 9) +
                                                     mpfc("4763959200") * Power(nu, 10) +
                                                     mpfc("129259312") * Power(nu, 11) +
                                                     mpfc("2522520") * Power(nu, 12) + mpfc("33460") * Power(nu, 13) +
                                                     mpfc("270") * Power(nu, 14) + Power(nu, 15)) +
                   mpfc("17") * lambda * (mpfc("1371195958099968000") + mpfc("2317820965473484800") * nu +
                                          mpfc("1687424939411374080") * Power(nu, 2) +
                                          mpfc("713340918791405568") * Power(nu, 3) +
                                          mpfc("198107173820104704") * Power(nu, 4) +
                                          mpfc("38628489336586240") * Power(nu, 5) +
                                          mpfc("5503512041431040") * Power(nu, 6) +
                                          mpfc("587213457166336") * Power(nu, 7) +
                                          mpfc("47604013453568") * Power(nu, 8) + mpfc("2951316459520") * Power(nu, 9) +
                                          mpfc("139842010880") * Power(nu, 10) + mpfc("5022477824") * Power(nu, 11) +
                                          mpfc("134304352") * Power(nu, 12) + mpfc("2589440") * Power(nu, 13) +
                                          mpfc("34000") * Power(nu, 14) + mpfc("272") * Power(nu, 15) + Power(nu, 16)) +
                   nu * (mpfc("1371195958099968000") + mpfc("2317820965473484800") * nu +
                         mpfc("1687424939411374080") * Power(nu, 2) + mpfc("713340918791405568") * Power(nu, 3) +
                         mpfc("198107173820104704") * Power(nu, 4) + mpfc("38628489336586240") * Power(nu, 5) +
                         mpfc("5503512041431040") * Power(nu, 6) + mpfc("587213457166336") * Power(nu, 7) +
                         mpfc("47604013453568") * Power(nu, 8) + mpfc("2951316459520") * Power(nu, 9) +
                         mpfc("139842010880") * Power(nu, 10) + mpfc("5022477824") * Power(nu, 11) +
                         mpfc("134304352") * Power(nu, 12) + mpfc("2589440") * Power(nu, 13) +
                         mpfc("34000") * Power(nu, 14) + mpfc("272") * Power(nu, 15) + Power(nu, 16)));

        moment(18, Power(lambda, 18) + mpfc("18") * Power(lambda, 17) * (mpfc("34") + nu) +
                   mpfc("153") * Power(lambda, 16) * (mpfc("1088") + mpfc("66") * nu + Power(nu, 2)) +
                   mpfc("816") * Power(lambda, 15) *
                   (mpfc("32640") + mpfc("3068") * nu + mpfc("96") * Power(nu, 2) + Power(nu, 3)) +
                   mpfc("3060") * Power(lambda, 14) *
                   (mpfc("913920") + mpfc("118544") * nu + mpfc("5756") * Power(nu, 2) + mpfc("124") * Power(nu, 3) +
                    Power(nu, 4)) + mpfc("8568") * Power(lambda, 13) *
                                    (mpfc("23761920") + mpfc("3996064") * nu + mpfc("268200") * Power(nu, 2) +
                                     mpfc("8980") * Power(nu, 3) + mpfc("150") * Power(nu, 4) + Power(nu, 5)) +
                   mpfc("18564") * Power(lambda, 12) *
                   (mpfc("570286080") + mpfc("119667456") * nu + mpfc("10432864") * Power(nu, 2) +
                    mpfc("483720") * Power(nu, 3) + mpfc("12580") * Power(nu, 4) + mpfc("174") * Power(nu, 5) +
                    Power(nu, 6)) + mpfc("31824") * Power(lambda, 11) *
                                    (mpfc("12546293760") + mpfc("3202970112") * nu + mpfc("349190464") * Power(nu, 2) +
                                     mpfc("21074704") * Power(nu, 3) + mpfc("760480") * Power(nu, 4) +
                                     mpfc("16408") * Power(nu, 5) + mpfc("196") * Power(nu, 6) + Power(nu, 7)) +
                   mpfc("43758") * Power(lambda, 10) *
                   (mpfc("250925875200") + mpfc("76605696000") * nu + mpfc("10186779392") * Power(nu, 2) +
                    mpfc("770684544") * Power(nu, 3) + mpfc("36284304") * Power(nu, 4) +
                    mpfc("1088640") * Power(nu, 5) + mpfc("20328") * Power(nu, 6) + mpfc("216") * Power(nu, 7) +
                    Power(nu, 8)) + mpfc("48620") * Power(lambda, 9) *
                                    (mpfc("4516665753600") + mpfc("1629828403200") * nu +
                                     mpfc("259967725056") * Power(nu, 2) + mpfc("24059101184") * Power(nu, 3) +
                                     mpfc("1423802016") * Power(nu, 4) + mpfc("55879824") * Power(nu, 5) +
                                     mpfc("1454544") * Power(nu, 6) + mpfc("24216") * Power(nu, 7) +
                                     mpfc("234") * Power(nu, 8) + Power(nu, 9)) + mpfc("43758") * Power(lambda, 8) *
                                                                                  (mpfc("72266652057600") +
                                                                                   mpfc("30593920204800") * nu +
                                                                                   mpfc("5789312004096") *
                                                                                   Power(nu, 2) +
                                                                                   mpfc("644913344000") * Power(nu, 3) +
                                                                                   mpfc("46839933440") * Power(nu, 4) +
                                                                                   mpfc("2317879200") * Power(nu, 5) +
                                                                                   mpfc("79152528") * Power(nu, 6) +
                                                                                   mpfc("1842000") * Power(nu, 7) +
                                                                                   mpfc("27960") * Power(nu, 8) +
                                                                                   mpfc("250") * Power(nu, 9) +
                                                                                   Power(nu, 10)) +
                   mpfc("31824") * Power(lambda, 7) *
                   (mpfc("1011733128806400") + mpfc("500581534924800") * nu + mpfc("111644288262144") * Power(nu, 2) +
                    mpfc("14818098820096") * Power(nu, 3) + mpfc("1300672412160") * Power(nu, 4) +
                    mpfc("79290242240") * Power(nu, 5) + mpfc("3426014592") * Power(nu, 6) +
                    mpfc("104940528") * Power(nu, 7) + mpfc("2233440") * Power(nu, 8) + mpfc("31460") * Power(nu, 9) +
                    mpfc("264") * Power(nu, 10) + Power(nu, 11)) + mpfc("18564") * Power(lambda, 6) *
                                                                   (mpfc("12140797545676800") +
                                                                    mpfc("7018711547904000") * nu +
                                                                    mpfc("1840312994070528") * Power(nu, 2) +
                                                                    mpfc("289461474103296") * Power(nu, 3) +
                                                                    mpfc("30426167766016") * Power(nu, 4) +
                                                                    mpfc("2252155319040") * Power(nu, 5) +
                                                                    mpfc("120402417344") * Power(nu, 6) +
                                                                    mpfc("4685300928") * Power(nu, 7) +
                                                                    mpfc("131741808") * Power(nu, 8) +
                                                                    mpfc("2610960") * Power(nu, 9) +
                                                                    mpfc("34628") * Power(nu, 10) +
                                                                    mpfc("276") * Power(nu, 11) + Power(nu, 12)) +
                   mpfc("8568") * Power(lambda, 5) * (mpfc("121407975456768000") + mpfc("82327913024716800") * nu +
                                                      mpfc("25421841488609280") * Power(nu, 2) +
                                                      mpfc("4734927735103488") * Power(nu, 3) +
                                                      mpfc("593723151763456") * Power(nu, 4) +
                                                      mpfc("52947720956416") * Power(nu, 5) +
                                                      mpfc("3456179492480") * Power(nu, 6) +
                                                      mpfc("167255426624") * Power(nu, 7) +
                                                      mpfc("6002719008") * Power(nu, 8) +
                                                      mpfc("157851408") * Power(nu, 9) +
                                                      mpfc("2957240") * Power(nu, 10) + mpfc("37388") * Power(nu, 11) +
                                                      mpfc("286") * Power(nu, 12) + Power(nu, 13)) +
                   mpfc("3060") * Power(lambda, 4) * (mpfc("971263803654144000") + mpfc("780031279654502400") * nu +
                                                      mpfc("285702644933591040") * Power(nu, 2) +
                                                      mpfc("63301263369437184") * Power(nu, 3) +
                                                      mpfc("9484712949211136") * Power(nu, 4) +
                                                      mpfc("1017304919414784") * Power(nu, 5) +
                                                      mpfc("80597156896256") * Power(nu, 6) +
                                                      mpfc("4794222905472") * Power(nu, 7) +
                                                      mpfc("215277178688") * Power(nu, 8) +
                                                      mpfc("7265530272") * Power(nu, 9) +
                                                      mpfc("181509328") * Power(nu, 10) +
                                                      mpfc("3256344") * Power(nu, 11) + mpfc("39676") * Power(nu, 12) +
                                                      mpfc("294") * Power(nu, 13) + Power(nu, 14)) +
                   mpfc("816") * Power(lambda, 3) * (mpfc("5827582821924864000") + mpfc("5651451481581158400") * nu +
                                                     mpfc("2494247149256048640") * Power(nu, 2) +
                                                     mpfc("665510225150214144") * Power(nu, 3) +
                                                     mpfc("120209541064704000") * Power(nu, 4) +
                                                     mpfc("15588542465699840") * Power(nu, 5) +
                                                     mpfc("1500887860792320") * Power(nu, 6) +
                                                     mpfc("109362494329088") * Power(nu, 7) +
                                                     mpfc("6085885977600") * Power(nu, 8) +
                                                     mpfc("258870360320") * Power(nu, 9) +
                                                     mpfc("8354586240") * Power(nu, 10) +
                                                     mpfc("201047392") * Power(nu, 11) +
                                                     mpfc("3494400") * Power(nu, 12) + mpfc("41440") * Power(nu, 13) +
                                                     mpfc("300") * Power(nu, 14) + Power(nu, 15)) +
                   mpfc("153") * Power(lambda, 2) * (mpfc("23310331287699456000") + mpfc("28433388748249497600") * nu +
                                                     mpfc("15628440078605352960") * Power(nu, 2) +
                                                     mpfc("5156288049856905216") * Power(nu, 3) +
                                                     mpfc("1146348389409030144") * Power(nu, 4) +
                                                     mpfc("182563710927503360") * Power(nu, 5) +
                                                     mpfc("21592093908869120") * Power(nu, 6) +
                                                     mpfc("1938337838108672") * Power(nu, 7) +
                                                     mpfc("133706038239488") * Power(nu, 8) +
                                                     mpfc("7121367418880") * Power(nu, 9) +
                                                     mpfc("292288705280") * Power(nu, 10) +
                                                     mpfc("9158775808") * Power(nu, 11) +
                                                     mpfc("215024992") * Power(nu, 12) +
                                                     mpfc("3660160") * Power(nu, 13) + mpfc("42640") * Power(nu, 14) +
                                                     mpfc("304") * Power(nu, 15) + Power(nu, 16)) +
                   mpfc("18") * lambda * (mpfc("46620662575398912000") + mpfc("80177108784198451200") * nu +
                                          mpfc("59690268905460203520") * Power(nu, 2) +
                                          mpfc("25941016178319163392") * Power(nu, 3) +
                                          mpfc("7448984828674965504") * Power(nu, 4) +
                                          mpfc("1511475811264036864") * Power(nu, 5) +
                                          mpfc("225747898745241600") * Power(nu, 6) +
                                          mpfc("25468769585086464") * Power(nu, 7) +
                                          mpfc("2205749914587648") * Power(nu, 8) +
                                          mpfc("147948773077248") * Power(nu, 9) +
                                          mpfc("7705944829440") * Power(nu, 10) + mpfc("310606256896") * Power(nu, 11) +
                                          mpfc("9588825792") * Power(nu, 12) + mpfc("222345312") * Power(nu, 13) +
                                          mpfc("3745440") * Power(nu, 14) + mpfc("43248") * Power(nu, 15) +
                                          mpfc("306") * Power(nu, 16) + Power(nu, 17)) + nu *
                                                                                         (mpfc("46620662575398912000") +
                                                                                          mpfc("80177108784198451200") *
                                                                                          nu +
                                                                                          mpfc("59690268905460203520") *
                                                                                          Power(nu, 2) +
                                                                                          mpfc("25941016178319163392") *
                                                                                          Power(nu, 3) +
                                                                                          mpfc("7448984828674965504") *
                                                                                          Power(nu, 4) +
                                                                                          mpfc("1511475811264036864") *
                                                                                          Power(nu, 5) +
                                                                                          mpfc("225747898745241600") *
                                                                                          Power(nu, 6) +
                                                                                          mpfc("25468769585086464") *
                                                                                          Power(nu, 7) +
                                                                                          mpfc("2205749914587648") *
                                                                                          Power(nu, 8) +
                                                                                          mpfc("147948773077248") *
                                                                                          Power(nu, 9) +
                                                                                          mpfc("7705944829440") *
                                                                                          Power(nu, 10) +
                                                                                          mpfc("310606256896") *
                                                                                          Power(nu, 11) +
                                                                                          mpfc("9588825792") *
                                                                                          Power(nu, 12) +
                                                                                          mpfc("222345312") *
                                                                                          Power(nu, 13) +
                                                                                          mpfc("3745440") *
                                                                                          Power(nu, 14) +
                                                                                          mpfc("43248") *
                                                                                          Power(nu, 15) +
                                                                                          mpfc("306") * Power(nu, 16) +
                                                                                          Power(nu, 17)));

        moment(19, Power(lambda, 19) + mpfc("19") * Power(lambda, 18) * (mpfc("36") + nu) +
                   mpfc("171") * Power(lambda, 17) * (mpfc("1224") + mpfc("70") * nu + Power(nu, 2)) +
                   mpfc("969") * Power(lambda, 16) *
                   (mpfc("39168") + mpfc("3464") * nu + mpfc("102") * Power(nu, 2) + Power(nu, 3)) +
                   mpfc("3876") * Power(lambda, 15) *
                   (mpfc("1175040") + mpfc("143088") * nu + mpfc("6524") * Power(nu, 2) + mpfc("132") * Power(nu, 3) +
                    Power(nu, 4)) + mpfc("11628") * Power(lambda, 14) *
                                    (mpfc("32901120") + mpfc("5181504") * nu + mpfc("325760") * Power(nu, 2) +
                                     mpfc("10220") * Power(nu, 3) + mpfc("160") * Power(nu, 4) + Power(nu, 5)) +
                   mpfc("27132") * Power(lambda, 13) *
                   (mpfc("855429120") + mpfc("167620224") * nu + mpfc("13651264") * Power(nu, 2) +
                    mpfc("591480") * Power(nu, 3) + mpfc("14380") * Power(nu, 4) + mpfc("186") * Power(nu, 5) +
                    Power(nu, 6)) + mpfc("50388") * Power(lambda, 12) *
                                    (mpfc("20530298880") + mpfc("4878314496") * nu + mpfc("495250560") * Power(nu, 2) +
                                     mpfc("27846784") * Power(nu, 3) + mpfc("936600") * Power(nu, 4) +
                                     mpfc("18844") * Power(nu, 5) + mpfc("210") * Power(nu, 6) + Power(nu, 7)) +
                   mpfc("75582") * Power(lambda, 11) *
                   (mpfc("451666575360") + mpfc("127853217792") * nu + mpfc("15773826816") * Power(nu, 2) +
                    mpfc("1107879808") * Power(nu, 3) + mpfc("48451984") * Power(nu, 4) +
                    mpfc("1351168") * Power(nu, 5) + mpfc("23464") * Power(nu, 6) + mpfc("232") * Power(nu, 7) +
                    Power(nu, 8)) + mpfc("92378") * Power(lambda, 10) *
                                    (mpfc("9033331507200") + mpfc("3008730931200") * nu +
                                     mpfc("443329754112") * Power(nu, 2) + mpfc("37931422976") * Power(nu, 3) +
                                     mpfc("2076919488") * Power(nu, 4) + mpfc("75475344") * Power(nu, 5) +
                                     mpfc("1820448") * Power(nu, 6) + mpfc("28104") * Power(nu, 7) +
                                     mpfc("252") * Power(nu, 8) + Power(nu, 9)) + mpfc("92378") * Power(lambda, 9) *
                                                                                  (mpfc("162599967129600") +
                                                                                   mpfc("63190488268800") * nu +
                                                                                   mpfc("10988666505216") *
                                                                                   Power(nu, 2) +
                                                                                   mpfc("1126095367680") *
                                                                                   Power(nu, 3) +
                                                                                   mpfc("75315973760") * Power(nu, 4) +
                                                                                   mpfc("3435475680") * Power(nu, 5) +
                                                                                   mpfc("108243408") * Power(nu, 6) +
                                                                                   mpfc("2326320") * Power(nu, 7) +
                                                                                   mpfc("32640") * Power(nu, 8) +
                                                                                   mpfc("270") * Power(nu, 9) +
                                                                                   Power(nu, 10)) +
                   mpfc("75582") * Power(lambda, 8) *
                   (mpfc("2601599474073600") + mpfc("1173647779430400") * nu + mpfc("239009152352256") * Power(nu, 2) +
                    mpfc("29006192388096") * Power(nu, 3) + mpfc("2331150947840") * Power(nu, 4) +
                    mpfc("130283584640") * Power(nu, 5) + mpfc("5167370208") * Power(nu, 6) +
                    mpfc("145464528") * Power(nu, 7) + mpfc("2848560") * Power(nu, 8) + mpfc("36960") * Power(nu, 9) +
                    mpfc("286") * Power(nu, 10) + Power(nu, 11)) + mpfc("50388") * Power(lambda, 7) *
                                                                   (mpfc("36422392637030400") +
                                                                    mpfc("19032668386099200") * nu +
                                                                    mpfc("4519775912361984") * Power(nu, 2) +
                                                                    mpfc("645095845785600") * Power(nu, 3) +
                                                                    mpfc("61642305657856") * Power(nu, 4) +
                                                                    mpfc("4155121132800") * Power(nu, 5) +
                                                                    mpfc("202626767552") * Power(nu, 6) +
                                                                    mpfc("7203873600") * Power(nu, 7) +
                                                                    mpfc("185344368") * Power(nu, 8) +
                                                                    mpfc("3366000") * Power(nu, 9) +
                                                                    mpfc("40964") * Power(nu, 10) +
                                                                    mpfc("300") * Power(nu, 11) + Power(nu, 12)) +
                   mpfc("27132") * Power(lambda, 6) * (mpfc("437068711644364800") + mpfc("264814413270220800") * nu +
                                                       mpfc("73269979334443008") * Power(nu, 2) +
                                                       mpfc("12260926061789184") * Power(nu, 3) +
                                                       mpfc("1384803513679872") * Power(nu, 4) +
                                                       mpfc("111503759251456") * Power(nu, 5) +
                                                       mpfc("6586642343424") * Power(nu, 6) +
                                                       mpfc("289073250752") * Power(nu, 7) +
                                                       mpfc("9428006016") * Power(nu, 8) +
                                                       mpfc("225736368") * Power(nu, 9) +
                                                       mpfc("3857568") * Power(nu, 10) + mpfc("44564") * Power(nu, 11) +
                                                       mpfc("312") * Power(nu, 12) + Power(nu, 13)) +
                   mpfc("11628") * Power(lambda, 5) * (mpfc("4370687116443648000") + mpfc("3085212844346572800") * nu +
                                                       mpfc("997514206614650880") * Power(nu, 2) +
                                                       mpfc("195879239952334848") * Power(nu, 3) +
                                                       mpfc("26108961198587904") * Power(nu, 4) +
                                                       mpfc("2499841106194432") * Power(nu, 5) +
                                                       mpfc("177370182685696") * Power(nu, 6) +
                                                       mpfc("9477374850944") * Power(nu, 7) +
                                                       mpfc("383353310912") * Power(nu, 8) +
                                                       mpfc("11685369696") * Power(nu, 9) +
                                                       mpfc("264312048") * Power(nu, 10) +
                                                       mpfc("4303208") * Power(nu, 11) + mpfc("47684") * Power(nu, 12) +
                                                       mpfc("322") * Power(nu, 13) + Power(nu, 14)) +
                   mpfc("3876") * Power(lambda, 4) * (mpfc("34965496931549184000") + mpfc("29052389871216230400") * nu +
                                                      mpfc("11065326497263779840") * Power(nu, 2) +
                                                      mpfc("2564548126233329664") * Power(nu, 3) +
                                                      mpfc("404750929541038080") * Power(nu, 4) +
                                                      mpfc("46107690048143360") * Power(nu, 5) +
                                                      mpfc("3918802567680000") * Power(nu, 6) +
                                                      mpfc("253189181493248") * Power(nu, 7) +
                                                      mpfc("12544201338240") * Power(nu, 8) +
                                                      mpfc("476836268480") * Power(nu, 9) +
                                                      mpfc("13799866080") * Power(nu, 10) +
                                                      mpfc("298737712") * Power(nu, 11) +
                                                      mpfc("4684680") * Power(nu, 12) + mpfc("50260") * Power(nu, 13) +
                                                      mpfc("330") * Power(nu, 14) + Power(nu, 15)) +
                   mpfc("969") * Power(lambda, 3) *
                   (mpfc("209792981589295104000") + mpfc("209279836158846566400") * nu +
                    mpfc("95444348854798909440") * Power(nu, 2) + mpfc("26452615254663757824") * Power(nu, 3) +
                    mpfc("4993053703479558144") * Power(nu, 4) + mpfc("681397069829898240") * Power(nu, 5) +
                    mpfc("69620505454223360") * Power(nu, 6) + mpfc("5437937656639488") * Power(nu, 7) +
                    mpfc("328454389522688") * Power(nu, 8) + mpfc("15405218949120") * Power(nu, 9) +
                    mpfc("559635464960") * Power(nu, 10) + mpfc("15592292352") * Power(nu, 11) +
                    mpfc("326845792") * Power(nu, 12) + mpfc("4986240") * Power(nu, 13) +
                    mpfc("52240") * Power(nu, 14) + mpfc("336") * Power(nu, 15) + Power(nu, 16)) +
                   mpfc("171") * Power(lambda, 2) *
                   (mpfc("839171926357180416000") + mpfc("1046912326224681369600") * nu +
                    mpfc("591057231578042204160") * Power(nu, 2) + mpfc("201254809873453940736") * Power(nu, 3) +
                    mpfc("46424830068581990400") * Power(nu, 4) + mpfc("7718641982799151104") * Power(nu, 5) +
                    mpfc("959879091646791680") * Power(nu, 6) + mpfc("91372256080781312") * Power(nu, 7) +
                    mpfc("6751755214730240") * Power(nu, 8) + mpfc("390075265319168") * Power(nu, 9) +
                    mpfc("17643760808960") * Power(nu, 10) + mpfc("622004634368") * Power(nu, 11) +
                    mpfc("16899675520") * Power(nu, 12) + mpfc("346790752") * Power(nu, 13) +
                    mpfc("5195200") * Power(nu, 14) + mpfc("53584") * Power(nu, 15) + mpfc("340") * Power(nu, 16) +
                    Power(nu, 17)) + mpfc("19") * lambda *
                                     (mpfc("1678343852714360832000") + mpfc("2932996578806543155200") * nu +
                                      mpfc("2229026789380765777920") * Power(nu, 2) +
                                      mpfc("993566851324950085632") * Power(nu, 3) +
                                      mpfc("294104470010617921536") * Power(nu, 4) +
                                      mpfc("61862114034180292608") * Power(nu, 5) +
                                      mpfc("9638400166092734464") * Power(nu, 6) +
                                      mpfc("1142623603808354304") * Power(nu, 7) +
                                      mpfc("104875766510241792") * Power(nu, 8) +
                                      mpfc("7531905745368576") * Power(nu, 9) +
                                      mpfc("425362786937088") * Power(nu, 10) + mpfc("18887770077696") * Power(nu, 11) +
                                      mpfc("655803985408") * Power(nu, 12) + mpfc("17593257024") * Power(nu, 13) +
                                      mpfc("357181152") * Power(nu, 14) + mpfc("5302368") * Power(nu, 15) +
                                      mpfc("54264") * Power(nu, 16) + mpfc("342") * Power(nu, 17) + Power(nu, 18)) +
                   nu * (mpfc("1678343852714360832000") + mpfc("2932996578806543155200") * nu +
                         mpfc("2229026789380765777920") * Power(nu, 2) + mpfc("993566851324950085632") * Power(nu, 3) +
                         mpfc("294104470010617921536") * Power(nu, 4) + mpfc("61862114034180292608") * Power(nu, 5) +
                         mpfc("9638400166092734464") * Power(nu, 6) + mpfc("1142623603808354304") * Power(nu, 7) +
                         mpfc("104875766510241792") * Power(nu, 8) + mpfc("7531905745368576") * Power(nu, 9) +
                         mpfc("425362786937088") * Power(nu, 10) + mpfc("18887770077696") * Power(nu, 11) +
                         mpfc("655803985408") * Power(nu, 12) + mpfc("17593257024") * Power(nu, 13) +
                         mpfc("357181152") * Power(nu, 14) + mpfc("5302368") * Power(nu, 15) +
                         mpfc("54264") * Power(nu, 16) + mpfc("342") * Power(nu, 17) + Power(nu, 18)));

        moment(20, Power(lambda, 20) + mpfc("20") * Power(lambda, 19) * (mpfc("38") + nu) +
                   mpfc("190") * Power(lambda, 18) * (mpfc("1368") + mpfc("74") * nu + Power(nu, 2)) +
                   mpfc("1140") * Power(lambda, 17) *
                   (mpfc("46512") + mpfc("3884") * nu + mpfc("108") * Power(nu, 2) + Power(nu, 3)) +
                   mpfc("4845") * Power(lambda, 16) *
                   (mpfc("1488384") + mpfc("170800") * nu + mpfc("7340") * Power(nu, 2) + mpfc("140") * Power(nu, 3) +
                    Power(nu, 4)) + mpfc("15504") * Power(lambda, 15) *
                                    (mpfc("44651520") + mpfc("6612384") * nu + mpfc("391000") * Power(nu, 2) +
                                     mpfc("11540") * Power(nu, 3) + mpfc("170") * Power(nu, 4) + Power(nu, 5)) +
                   mpfc("38760") * Power(lambda, 14) *
                   (mpfc("1250242560") + mpfc("229798272") * nu + mpfc("17560384") * Power(nu, 2) +
                    mpfc("714120") * Power(nu, 3) + mpfc("16300") * Power(nu, 4) + mpfc("198") * Power(nu, 5) +
                    Power(nu, 6)) + mpfc("77520") * Power(lambda, 13) *
                                    (mpfc("32506306560") + mpfc("7224997632") * nu + mpfc("686368256") * Power(nu, 2) +
                                     mpfc("36127504") * Power(nu, 3) + mpfc("1137920") * Power(nu, 4) +
                                     mpfc("21448") * Power(nu, 5) + mpfc("224") * Power(nu, 6) + Power(nu, 7)) +
                   mpfc("125970") * Power(lambda, 12) *
                   (mpfc("780151357440") + mpfc("205906249728") * nu + mpfc("23697835776") * Power(nu, 2) +
                    mpfc("1553428352") * Power(nu, 3) + mpfc("63437584") * Power(nu, 4) +
                    mpfc("1652672") * Power(nu, 5) + mpfc("26824") * Power(nu, 6) + mpfc("248") * Power(nu, 7) +
                    Power(nu, 8)) + mpfc("167960") * Power(lambda, 11) *
                                    (mpfc("17163329863680") + mpfc("5310088851456") * nu +
                                     mpfc("727258636800") * Power(nu, 2) + mpfc("57873259520") * Power(nu, 3) +
                                     mpfc("2949055200") * Power(nu, 4) + mpfc("99796368") * Power(nu, 5) +
                                     mpfc("2242800") * Power(nu, 6) + mpfc("32280") * Power(nu, 7) +
                                     mpfc("270") * Power(nu, 8) + Power(nu, 9)) + mpfc("184756") * Power(lambda, 10) *
                                                                                  (mpfc("343266597273600") +
                                                                                   mpfc("123365106892800") * nu +
                                                                                   mpfc("19855261587456") *
                                                                                   Power(nu, 2) +
                                                                                   mpfc("1884723827200") *
                                                                                   Power(nu, 3) +
                                                                                   mpfc("116854363520") * Power(nu, 4) +
                                                                                   mpfc("4944982560") * Power(nu, 5) +
                                                                                   mpfc("144652368") * Power(nu, 6) +
                                                                                   mpfc("2888400") * Power(nu, 7) +
                                                                                   mpfc("37680") * Power(nu, 8) +
                                                                                   mpfc("290") * Power(nu, 9) +
                                                                                   Power(nu, 10)) +
                   mpfc("167960") * Power(lambda, 9) *
                   (mpfc("6178798750924800") + mpfc("2563838521344000") * nu + mpfc("480759815467008") * Power(nu, 2) +
                    mpfc("53780290477056") * Power(nu, 3) + mpfc("3988102370560") * Power(nu, 4) +
                    mpfc("205864049600") * Power(nu, 5) + mpfc("7548725184") * Power(nu, 6) +
                    mpfc("196643568") * Power(nu, 7) + mpfc("3566640") * Power(nu, 8) + mpfc("42900") * Power(nu, 9) +
                    mpfc("308") * Power(nu, 10) + Power(nu, 11)) + mpfc("125970") * Power(lambda, 8) *
                                                                   (mpfc("98860780014796800") +
                                                                    mpfc("47200215092428800") * nu +
                                                                    mpfc("10255995568816128") * Power(nu, 2) +
                                                                    mpfc("1341244463099904") * Power(nu, 3) +
                                                                    mpfc("117589928406016") * Power(nu, 4) +
                                                                    mpfc("7281927164160") * Power(nu, 5) +
                                                                    mpfc("326643652544") * Power(nu, 6) +
                                                                    mpfc("10695022272") * Power(nu, 7) +
                                                                    mpfc("253709808") * Power(nu, 8) +
                                                                    mpfc("4253040") * Power(nu, 9) +
                                                                    mpfc("47828") * Power(nu, 10) +
                                                                    mpfc("324") * Power(nu, 11) + Power(nu, 12)) +
                   mpfc("77520") * Power(lambda, 7) * (mpfc("1384050920207155200") + mpfc("759663791308800000") * nu +
                                                       mpfc("190784153055854592") * Power(nu, 2) +
                                                       mpfc("29033418052214784") * Power(nu, 3) +
                                                       mpfc("2987503460784128") * Power(nu, 4) +
                                                       mpfc("219536908704256") * Power(nu, 5) +
                                                       mpfc("11854938299776") * Power(nu, 6) +
                                                       mpfc("476373964352") * Power(nu, 7) +
                                                       mpfc("14246959584") * Power(nu, 8) +
                                                       mpfc("313252368") * Power(nu, 9) +
                                                       mpfc("4922632") * Power(nu, 10) + mpfc("52364") * Power(nu, 11) +
                                                       mpfc("338") * Power(nu, 12) + Power(nu, 13)) +
                   mpfc("38760") * Power(lambda, 6) *
                   (mpfc("16608611042485862400") + mpfc("10500016415912755200") * nu +
                    mpfc("3049073627979055104") * Power(nu, 2) + mpfc("539185169682432000") * Power(nu, 3) +
                    mpfc("64883459581624320") * Power(nu, 4) + mpfc("5621946365235200") * Power(nu, 5) +
                    mpfc("361796168301568") * Power(nu, 6) + mpfc("17571425872000") * Power(nu, 7) +
                    mpfc("647337479360") * Power(nu, 8) + mpfc("18005988000") * Power(nu, 9) +
                    mpfc("372323952") * Power(nu, 10) + mpfc("5551000") * Power(nu, 11) +
                    mpfc("56420") * Power(nu, 12) + mpfc("350") * Power(nu, 13) + Power(nu, 14)) +
                   mpfc("15504") * Power(lambda, 5) *
                   (mpfc("166086110424858624000") + mpfc("121608775201613414400") * nu +
                    mpfc("40990752695703306240") * Power(nu, 2) + mpfc("8440925324803375104") * Power(nu, 3) +
                    mpfc("1188019765498675200") * Power(nu, 4) + mpfc("121102923233976320") * Power(nu, 5) +
                    mpfc("9239908048250880") * Power(nu, 6) + mpfc("537510427021568") * Power(nu, 7) +
                    mpfc("24044800665600") * Power(nu, 8) + mpfc("827397359360") * Power(nu, 9) +
                    mpfc("21729227520") * Power(nu, 10) + mpfc("427833952") * Power(nu, 11) +
                    mpfc("6115200") * Power(nu, 12) + mpfc("59920") * Power(nu, 13) + mpfc("360") * Power(nu, 14) +
                    Power(nu, 15)) + mpfc("4845") * Power(lambda, 4) *
                                     (mpfc("1328688883398868992000") + mpfc("1138956312037765939200") * nu +
                                      mpfc("449534796767239864320") * Power(nu, 2) +
                                      mpfc("108518155294130307072") * Power(nu, 3) +
                                      mpfc("17945083448792776704") * Power(nu, 4) +
                                      mpfc("2156843151370485760") * Power(nu, 5) +
                                      mpfc("195022187619983360") * Power(nu, 6) +
                                      mpfc("13539991464423424") * Power(nu, 7) +
                                      mpfc("729868832346368") * Power(nu, 8) + mpfc("30663979540480") * Power(nu, 9) +
                                      mpfc("1001231179520") * Power(nu, 10) + mpfc("25151899136") * Power(nu, 11) +
                                      mpfc("476755552") * Power(nu, 12) + mpfc("6594560") * Power(nu, 13) +
                                      mpfc("62800") * Power(nu, 14) + mpfc("368") * Power(nu, 15) + Power(nu, 16)) +
                   mpfc("1140") * Power(lambda, 3) *
                   (mpfc("7972133300393213952000") + mpfc("8162426755625464627200") * nu +
                    mpfc("3836165092641205125120") * Power(nu, 2) + mpfc("1100643728532021706752") * Power(nu, 3) +
                    mpfc("216188655986886967296") * Power(nu, 4) + mpfc("30886142357015691264") * Power(nu, 5) +
                    mpfc("3326976277090385920") * Power(nu, 6) + mpfc("276262136406523904") * Power(nu, 7) +
                    mpfc("17919204458501632") * Power(nu, 8) + mpfc("913852709589248") * Power(nu, 9) +
                    mpfc("36671366617600") * Power(nu, 10) + mpfc("1152142574336") * Power(nu, 11) +
                    mpfc("28012432448") * Power(nu, 12) + mpfc("516322912") * Power(nu, 13) +
                    mpfc("6971360") * Power(nu, 14) + mpfc("65008") * Power(nu, 15) + mpfc("374") * Power(nu, 16) +
                    Power(nu, 17)) + mpfc("190") * Power(lambda, 2) *
                                     (mpfc("31888533201572855808000") + mpfc("40621840322895072460800") * nu +
                                      mpfc("23507087126190285127680") * Power(nu, 2) +
                                      mpfc("8238740006769291952128") * Power(nu, 3) +
                                      mpfc("1965398352479569575936") * Power(nu, 4) +
                                      mpfc("339733225414949732352") * Power(nu, 5) +
                                      mpfc("44194047465377234944") * Power(nu, 6) +
                                      mpfc("4432024822716481536") * Power(nu, 7) +
                                      mpfc("347938954240530432") * Power(nu, 8) +
                                      mpfc("21574615296858624") * Power(nu, 9) +
                                      mpfc("1060538176059648") * Power(nu, 10) +
                                      mpfc("41279936914944") * Power(nu, 11) + mpfc("1264192304128") * Power(nu, 12) +
                                      mpfc("30077724096") * Power(nu, 13) + mpfc("544208352") * Power(nu, 14) +
                                      mpfc("7231392") * Power(nu, 15) + mpfc("66504") * Power(nu, 16) +
                                      mpfc("378") * Power(nu, 17) + Power(nu, 18)) + mpfc("20") * lambda *
                                                                                     (mpfc("63777066403145711616000") +
                                                                                      mpfc("113132213847363000729600") *
                                                                                      nu +
                                                                                      mpfc("87636014575275642716160") *
                                                                                      Power(nu, 2) +
                                                                                      mpfc("39984567139728869031936") *
                                                                                      Power(nu, 3) +
                                                                                      mpfc("12169536711728431104000") *
                                                                                      Power(nu, 4) +
                                                                                      mpfc("2644864803309469040640") *
                                                                                      Power(nu, 5) +
                                                                                      mpfc("428121320345704202240") *
                                                                                      Power(nu, 6) +
                                                                                      mpfc("53058097110810198016") *
                                                                                      Power(nu, 7) +
                                                                                      mpfc("5127902731197542400") *
                                                                                      Power(nu, 8) +
                                                                                      mpfc("391088184834247680") *
                                                                                      Power(nu, 9) +
                                                                                      mpfc("23695691648977920") *
                                                                                      Power(nu, 10) +
                                                                                      mpfc("1143098049889536") *
                                                                                      Power(nu, 11) +
                                                                                      mpfc("43808321523200") *
                                                                                      Power(nu, 12) +
                                                                                      mpfc("1324347752320") *
                                                                                      Power(nu, 13) +
                                                                                      mpfc("31166140800") *
                                                                                      Power(nu, 14) +
                                                                                      mpfc("558671136") *
                                                                                      Power(nu, 15) +
                                                                                      mpfc("7364400") * Power(nu, 16) +
                                                                                      mpfc("67260") * Power(nu, 17) +
                                                                                      mpfc("380") * Power(nu, 18) +
                                                                                      Power(nu, 19)) + nu *
                                                                                                       (mpfc("63777066403145711616000") +
                                                                                                        mpfc("113132213847363000729600") *
                                                                                                        nu +
                                                                                                        mpfc("87636014575275642716160") *
                                                                                                        Power(nu, 2) +
                                                                                                        mpfc("39984567139728869031936") *
                                                                                                        Power(nu, 3) +
                                                                                                        mpfc("12169536711728431104000") *
                                                                                                        Power(nu, 4) +
                                                                                                        mpfc("2644864803309469040640") *
                                                                                                        Power(nu, 5) +
                                                                                                        mpfc("428121320345704202240") *
                                                                                                        Power(nu, 6) +
                                                                                                        mpfc("53058097110810198016") *
                                                                                                        Power(nu, 7) +
                                                                                                        mpfc("5127902731197542400") *
                                                                                                        Power(nu, 8) +
                                                                                                        mpfc("391088184834247680") *
                                                                                                        Power(nu, 9) +
                                                                                                        mpfc("23695691648977920") *
                                                                                                        Power(nu, 10) +
                                                                                                        mpfc("1143098049889536") *
                                                                                                        Power(nu, 11) +
                                                                                                        mpfc("43808321523200") *
                                                                                                        Power(nu, 12) +
                                                                                                        mpfc("1324347752320") *
                                                                                                        Power(nu, 13) +
                                                                                                        mpfc("31166140800") *
                                                                                                        Power(nu, 14) +
                                                                                                        mpfc("558671136") *
                                                                                                        Power(nu, 15) +
                                                                                                        mpfc("7364400") *
                                                                                                        Power(nu, 16) +
                                                                                                        mpfc("67260") *
                                                                                                        Power(nu, 17) +
                                                                                                        mpfc("380") *
                                                                                                        Power(nu, 18) +
                                                                                                        Power(nu, 19)));

        moment(21, Power(lambda, 21) + mpfc("21") * Power(lambda, 20) * (mpfc("40") + nu) +
                   mpfc("210") * Power(lambda, 19) * (mpfc("1520") + mpfc("78") * nu + Power(nu, 2)) +
                   mpfc("1330") * Power(lambda, 18) *
                   (mpfc("54720") + mpfc("4328") * nu + mpfc("114") * Power(nu, 2) + Power(nu, 3)) +
                   mpfc("5985") * Power(lambda, 17) *
                   (mpfc("1860480") + mpfc("201872") * nu + mpfc("8204") * Power(nu, 2) + mpfc("148") * Power(nu, 3) +
                    Power(nu, 4)) + mpfc("20349") * Power(lambda, 16) *
                                    (mpfc("59535360") + mpfc("8320384") * nu + mpfc("464400") * Power(nu, 2) +
                                     mpfc("12940") * Power(nu, 3) + mpfc("180") * Power(nu, 4) + Power(nu, 5)) +
                   mpfc("54264") * Power(lambda, 15) *
                   (mpfc("1786060800") + mpfc("309146880") * nu + mpfc("22252384") * Power(nu, 2) +
                    mpfc("852600") * Power(nu, 3) + mpfc("18340") * Power(nu, 4) + mpfc("210") * Power(nu, 5) +
                    Power(nu, 6)) + mpfc("116280") * Power(lambda, 14) *
                                    (mpfc("50009702400") + mpfc("10442173440") * nu + mpfc("932213632") * Power(nu, 2) +
                                     mpfc("46125184") * Power(nu, 3) + mpfc("1366120") * Power(nu, 4) +
                                     mpfc("24220") * Power(nu, 5) + mpfc("238") * Power(nu, 6) + Power(nu, 7)) +
                   mpfc("203490") * Power(lambda, 13) *
                   (mpfc("1300252262400") + mpfc("321506211840") * nu + mpfc("34679727872") * Power(nu, 2) +
                    mpfc("2131468416") * Power(nu, 3) + mpfc("81644304") * Power(nu, 4) +
                    mpfc("1995840") * Power(nu, 5) + mpfc("30408") * Power(nu, 6) + mpfc("264") * Power(nu, 7) +
                    Power(nu, 8)) + mpfc("293930") * Power(lambda, 12) *
                                    (mpfc("31206054297600") + mpfc("9016401346560") * nu +
                                     mpfc("1153819680768") * Power(nu, 2) + mpfc("85834969856") * Power(nu, 3) +
                                     mpfc("4090931712") * Power(nu, 4) + mpfc("129544464") * Power(nu, 5) +
                                     mpfc("2725632") * Power(nu, 6) + mpfc("36744") * Power(nu, 7) +
                                     mpfc("288") * Power(nu, 8) + Power(nu, 9)) + mpfc("352716") * Power(lambda, 11) *
                                                                                  (mpfc("686533194547200") +
                                                                                   mpfc("229566883921920") * nu +
                                                                                   mpfc("34400434323456") *
                                                                                   Power(nu, 2) +
                                                                                   mpfc("3042189017600") *
                                                                                   Power(nu, 3) +
                                                                                   mpfc("175835467520") * Power(nu, 4) +
                                                                                   mpfc("6940909920") * Power(nu, 5) +
                                                                                   mpfc("189508368") * Power(nu, 6) +
                                                                                   mpfc("3534000") * Power(nu, 7) +
                                                                                   mpfc("43080") * Power(nu, 8) +
                                                                                   mpfc("310") * Power(nu, 9) +
                                                                                   Power(nu, 10)) +
                   mpfc("352716") * Power(lambda, 10) *
                   (mpfc("13730663890944000") + mpfc("5277870872985600") * nu + mpfc("917575570391040") * Power(nu, 2) +
                    mpfc("95244214675456") * Power(nu, 3) + mpfc("6558898368000") * Power(nu, 4) +
                    mpfc("314653665920") * Power(nu, 5) + mpfc("10731077280") * Power(nu, 6) +
                    mpfc("260188368") * Power(nu, 7) + mpfc("4395600") * Power(nu, 8) + mpfc("49280") * Power(nu, 9) +
                    mpfc("330") * Power(nu, 10) + Power(nu, 11)) + mpfc("293930") * Power(lambda, 9) *
                                                                   (mpfc("247151950036992000") +
                                                                    mpfc("108732339604684800") * nu +
                                                                    mpfc("21794231140024320") * Power(nu, 2) +
                                                                    mpfc("2631971434549248") * Power(nu, 3) +
                                                                    mpfc("213304385299456") * Power(nu, 4) +
                                                                    mpfc("12222664354560") * Power(nu, 5) +
                                                                    mpfc("507813056960") * Power(nu, 6) +
                                                                    mpfc("15414467904") * Power(nu, 7) +
                                                                    mpfc("339309168") * Power(nu, 8) +
                                                                    mpfc("5282640") * Power(nu, 9) +
                                                                    mpfc("55220") * Power(nu, 10) +
                                                                    mpfc("348") * Power(nu, 11) + Power(nu, 12)) +
                   mpfc("203490") * Power(lambda, 8) * (mpfc("3954431200591872000") + mpfc("1986869383711948800") * nu +
                                                        mpfc("457440037845073920") * Power(nu, 2) +
                                                        mpfc("63905774092812288") * Power(nu, 3) +
                                                        mpfc("6044841599340544") * Power(nu, 4) +
                                                        mpfc("408867014972416") * Power(nu, 5) +
                                                        mpfc("20347673265920") * Power(nu, 6) +
                                                        mpfc("754444543424") * Power(nu, 7) +
                                                        mpfc("20843414592") * Power(nu, 8) +
                                                        mpfc("423831408") * Power(nu, 9) +
                                                        mpfc("6166160") * Power(nu, 10) +
                                                        mpfc("60788") * Power(nu, 11) + mpfc("364") * Power(nu, 12) +
                                                        Power(nu, 13)) + mpfc("116280") * Power(lambda, 7) *
                                                                         (mpfc("55362036808286208000") +
                                                                          mpfc("31770602572559155200") * nu +
                                                                          mpfc("8391029913542983680") * Power(nu, 2) +
                                                                          mpfc("1352120875144445952") * Power(nu, 3) +
                                                                          mpfc("148533556483579904") * Power(nu, 4) +
                                                                          mpfc("11768979808954368") * Power(nu, 5) +
                                                                          mpfc("693734440695296") * Power(nu, 6) +
                                                                          mpfc("30909896873856") * Power(nu, 7) +
                                                                          mpfc("1046252347712") * Power(nu, 8) +
                                                                          mpfc("26777054304") * Power(nu, 9) +
                                                                          mpfc("510157648") * Power(nu, 10) +
                                                                          mpfc("7017192") * Power(nu, 11) +
                                                                          mpfc("65884") * Power(nu, 12) +
                                                                          mpfc("378") * Power(nu, 13) + Power(nu, 14)) +
                   mpfc("54264") * Power(lambda, 6) *
                   (mpfc("664344441699434496000") + mpfc("436609267678996070400") * nu +
                    mpfc("132462961535074959360") * Power(nu, 2) + mpfc("24616480415276335104") * Power(nu, 3) +
                    mpfc("3134523552947404800") * Power(nu, 4) + mpfc("289761314191032320") * Power(nu, 5) +
                    mpfc("20093793097297920") * Power(nu, 6) + mpfc("1064653203181568") * Power(nu, 7) +
                    mpfc("43464925046400") * Power(nu, 8) + mpfc("1367576999360") * Power(nu, 9) +
                    mpfc("32898946080") * Power(nu, 10) + mpfc("594363952") * Power(nu, 11) +
                    mpfc("7807800") * Power(nu, 12) + mpfc("70420") * Power(nu, 13) + mpfc("390") * Power(nu, 14) +
                    Power(nu, 15)) + mpfc("20349") * Power(lambda, 5) *
                                     (mpfc("6643444416994344960000") + mpfc("5030437118489395200000") * nu +
                                      mpfc("1761238883029745664000") * Power(nu, 2) +
                                      mpfc("378627765687838310400") * Power(nu, 3) +
                                      mpfc("55961715944750383104") * Power(nu, 4) +
                                      mpfc("6032136694857728000") * Power(nu, 5) +
                                      mpfc("490699245164011520") * Power(nu, 6) +
                                      mpfc("30740325129113600") * Power(nu, 7) +
                                      mpfc("1499302453645568") * Power(nu, 8) + mpfc("57140695040000") * Power(nu, 9) +
                                      mpfc("1696566460160") * Power(nu, 10) + mpfc("38842585600") * Power(nu, 11) +
                                      mpfc("672441952") * Power(nu, 12) + mpfc("8512000") * Power(nu, 13) +
                                      mpfc("74320") * Power(nu, 14) + mpfc("400") * Power(nu, 15) + Power(nu, 16)) +
                   mpfc("5985") * Power(lambda, 4) *
                   (mpfc("53147555335954759680000") + mpfc("46886941364909506560000") * nu +
                    mpfc("19120348182727360512000") * Power(nu, 2) + mpfc("4790261008532452147200") * Power(nu, 3) +
                    mpfc("826321493245841375232") * Power(nu, 4) + mpfc("104218809503612207104") * Power(nu, 5) +
                    mpfc("9957730656169820160") * Power(nu, 6) + mpfc("736621846196920320") * Power(nu, 7) +
                    mpfc("42734744758278144") * Power(nu, 8) + mpfc("1956428013965568") * Power(nu, 9) +
                    mpfc("70713226721280") * Power(nu, 10) + mpfc("2007307144960") * Power(nu, 11) +
                    mpfc("44222121216") * Power(nu, 12) + mpfc("740537952") * Power(nu, 13) +
                    mpfc("9106560") * Power(nu, 14) + mpfc("77520") * Power(nu, 15) + mpfc("408") * Power(nu, 16) +
                    Power(nu, 17)) + mpfc("1330") * Power(lambda, 3) *
                                     (mpfc("318885332015728558080000") + mpfc("334469203525411799040000") * nu +
                                      mpfc("161609030461273669632000") * Power(nu, 2) +
                                      mpfc("47861914233922073395200") * Power(nu, 3) +
                                      mpfc("9748189968007500398592") * Power(nu, 4) +
                                      mpfc("1451634350267514617856") * Power(nu, 5) +
                                      mpfc("163965193440631128064") * Power(nu, 6) +
                                      mpfc("14377461733351342080") * Power(nu, 7) +
                                      mpfc("993030314746589184") * Power(nu, 8) +
                                      mpfc("54473312842071552") * Power(nu, 9) +
                                      mpfc("2380707374293248") * Power(nu, 10) +
                                      mpfc("82757069591040") * Power(nu, 11) + mpfc("2272639872256") * Power(nu, 12) +
                                      mpfc("48665348928") * Power(nu, 13) + mpfc("795177312") * Power(nu, 14) +
                                      mpfc("9571680") * Power(nu, 15) + mpfc("79968") * Power(nu, 16) +
                                      mpfc("414") * Power(nu, 17) + Power(nu, 18)) + mpfc("210") * Power(lambda, 2) *
                                                                                     (mpfc("1275541328062914232320000") +
                                                                                      mpfc("1656762146117375754240000") *
                                                                                      nu +
                                                                                      mpfc("980905325370506477568000") *
                                                                                      Power(nu, 2) +
                                                                                      mpfc("353056687396961963212800") *
                                                                                      Power(nu, 3) +
                                                                                      mpfc("86854674105952074989568") *
                                                                                      Power(nu, 4) +
                                                                                      mpfc("15554727369077558870016") *
                                                                                      Power(nu, 5) +
                                                                                      mpfc("2107495124030039130112") *
                                                                                      Power(nu, 6) +
                                                                                      mpfc("221475040374036496384") *
                                                                                      Power(nu, 7) +
                                                                                      mpfc("18349582992337698816") *
                                                                                      Power(nu, 8) +
                                                                                      mpfc("1210923566114875392") *
                                                                                      Power(nu, 9) +
                                                                                      mpfc("63996142339244544") *
                                                                                      Power(nu, 10) +
                                                                                      mpfc("2711735652657408") *
                                                                                      Power(nu, 11) +
                                                                                      mpfc("91847629080064") *
                                                                                      Power(nu, 12) +
                                                                                      mpfc("2467301267968") *
                                                                                      Power(nu, 13) +
                                                                                      mpfc("51846058176") *
                                                                                      Power(nu, 14) +
                                                                                      mpfc("833464032") *
                                                                                      Power(nu, 15) +
                                                                                      mpfc("9891552") * Power(nu, 16) +
                                                                                      mpfc("81624") * Power(nu, 17) +
                                                                                      mpfc("418") * Power(nu, 18) +
                                                                                      Power(nu, 19)) +
                   mpfc("21") * lambda * (mpfc("2551082656125828464640000") + mpfc("4589065620297665740800000") * nu +
                                          mpfc("3618572796858388709376000") * Power(nu, 2) +
                                          mpfc("1687018700164430403993600") * Power(nu, 3) +
                                          mpfc("526766035608866113191936") * Power(nu, 4) +
                                          mpfc("117964128844107192729600") * Power(nu, 5) +
                                          mpfc("19769717617137637130240") * Power(nu, 6) +
                                          mpfc("2550445204778112122880") * Power(nu, 7) +
                                          mpfc("258174206358711894016") * Power(nu, 8) +
                                          mpfc("20771430124567449600") * Power(nu, 9) +
                                          mpfc("1338915850793364480") * Power(nu, 10) +
                                          mpfc("69419613644559360") * Power(nu, 11) +
                                          mpfc("2895430910817536") * Power(nu, 12) +
                                          mpfc("96782231616000") * Power(nu, 13) +
                                          mpfc("2570993384320") * Power(nu, 14) + mpfc("53512986240") * Power(nu, 15) +
                                          mpfc("853247136") * Power(nu, 16) + mpfc("10054800") * Power(nu, 17) +
                                          mpfc("82460") * Power(nu, 18) + mpfc("420") * Power(nu, 19) + Power(nu, 20)) +
                   nu * (mpfc("2551082656125828464640000") + mpfc("4589065620297665740800000") * nu +
                         mpfc("3618572796858388709376000") * Power(nu, 2) +
                         mpfc("1687018700164430403993600") * Power(nu, 3) +
                         mpfc("526766035608866113191936") * Power(nu, 4) +
                         mpfc("117964128844107192729600") * Power(nu, 5) +
                         mpfc("19769717617137637130240") * Power(nu, 6) +
                         mpfc("2550445204778112122880") * Power(nu, 7) + mpfc("258174206358711894016") * Power(nu, 8) +
                         mpfc("20771430124567449600") * Power(nu, 9) + mpfc("1338915850793364480") * Power(nu, 10) +
                         mpfc("69419613644559360") * Power(nu, 11) + mpfc("2895430910817536") * Power(nu, 12) +
                         mpfc("96782231616000") * Power(nu, 13) + mpfc("2570993384320") * Power(nu, 14) +
                         mpfc("53512986240") * Power(nu, 15) + mpfc("853247136") * Power(nu, 16) +
                         mpfc("10054800") * Power(nu, 17) + mpfc("82460") * Power(nu, 18) +
                         mpfc("420") * Power(nu, 19) + Power(nu, 20)));

        moment(22, Power(lambda, 22) + mpfc("22") * Power(lambda, 21) * (mpfc("42") + nu) +
                   mpfc("231") * Power(lambda, 20) * (mpfc("1680") + mpfc("82") * nu + Power(nu, 2)) +
                   mpfc("1540") * Power(lambda, 19) *
                   (mpfc("63840") + mpfc("4796") * nu + mpfc("120") * Power(nu, 2) + Power(nu, 3)) +
                   mpfc("7315") * Power(lambda, 18) *
                   (mpfc("2298240") + mpfc("236496") * nu + mpfc("9116") * Power(nu, 2) + mpfc("156") * Power(nu, 3) +
                    Power(nu, 4)) + mpfc("26334") * Power(lambda, 17) *
                                    (mpfc("78140160") + mpfc("10339104") * nu + mpfc("546440") * Power(nu, 2) +
                                     mpfc("14420") * Power(nu, 3) + mpfc("190") * Power(nu, 4) + Power(nu, 5)) +
                   mpfc("74613") * Power(lambda, 16) *
                   (mpfc("2500485120") + mpfc("408991488") * nu + mpfc("27825184") * Power(nu, 2) +
                    mpfc("1007880") * Power(nu, 3) + mpfc("20500") * Power(nu, 4) + mpfc("222") * Power(nu, 5) +
                    Power(nu, 6)) + mpfc("170544") * Power(lambda, 15) *
                                    (mpfc("75014553600") + mpfc("14770229760") * nu +
                                     mpfc("1243747008") * Power(nu, 2) + mpfc("58061584") * Power(nu, 3) +
                                     mpfc("1622880") * Power(nu, 4) + mpfc("27160") * Power(nu, 5) +
                                     mpfc("252") * Power(nu, 6) + Power(nu, 7)) + mpfc("319770") * Power(lambda, 14) *
                                                                                  (mpfc("2100407500800") +
                                                                                   mpfc("488580986880") * nu +
                                                                                   mpfc("49595145984") * Power(nu, 2) +
                                                                                   mpfc("2869471360") * Power(nu, 3) +
                                                                                   mpfc("103502224") * Power(nu, 4) +
                                                                                   mpfc("2383360") * Power(nu, 5) +
                                                                                   mpfc("34216") * Power(nu, 6) +
                                                                                   mpfc("280") * Power(nu, 7) +
                                                                                   Power(nu, 8)) +
                   mpfc("497420") * Power(lambda, 13) *
                   (mpfc("54610595020800") + mpfc("14803513159680") * nu + mpfc("1778054782464") * Power(nu, 2) +
                    mpfc("124201401344") * Power(nu, 3) + mpfc("5560529184") * Power(nu, 4) +
                    mpfc("165469584") * Power(nu, 5) + mpfc("3272976") * Power(nu, 6) + mpfc("41496") * Power(nu, 7) +
                    mpfc("306") * Power(nu, 8) + Power(nu, 9)) + mpfc("646646") * Power(lambda, 12) *
                                                                 (mpfc("1310654280499200") +
                                                                  mpfc("409894910853120") * nu +
                                                                  mpfc("57476827938816") * Power(nu, 2) +
                                                                  mpfc("4758888414720") * Power(nu, 3) +
                                                                  mpfc("257654101760") * Power(nu, 4) +
                                                                  mpfc("9531799200") * Power(nu, 5) +
                                                                  mpfc("244021008") * Power(nu, 6) +
                                                                  mpfc("4268880") * Power(nu, 7) +
                                                                  mpfc("48840") * Power(nu, 8) +
                                                                  mpfc("330") * Power(nu, 9) + Power(nu, 10)) +
                   mpfc("705432") * Power(lambda, 11) * (mpfc("28834394170982400") + mpfc("10328342319267840") * nu +
                                                         mpfc("1674385125507072") * Power(nu, 2) +
                                                         mpfc("162172373062656") * Power(nu, 3) +
                                                         mpfc("10427278653440") * Power(nu, 4) +
                                                         mpfc("467353684160") * Power(nu, 5) +
                                                         mpfc("14900261376") * Power(nu, 6) +
                                                         mpfc("337936368") * Power(nu, 7) +
                                                         mpfc("5343360") * Power(nu, 8) + mpfc("56100") * Power(nu, 9) +
                                                         mpfc("352") * Power(nu, 10) + Power(nu, 11)) +
                   mpfc("646646") * Power(lambda, 10) * (mpfc("576687883419648000") + mpfc("235401240556339200") * nu +
                                                         mpfc("43816044829409280") * Power(nu, 2) +
                                                         mpfc("4917832586760192") * Power(nu, 3) +
                                                         mpfc("370717946131456") * Power(nu, 4) +
                                                         mpfc("19774352336640") * Power(nu, 5) +
                                                         mpfc("765358911680") * Power(nu, 6) +
                                                         mpfc("21658988736") * Power(nu, 7) +
                                                         mpfc("444803568") * Power(nu, 8) +
                                                         mpfc("6465360") * Power(nu, 9) +
                                                         mpfc("63140") * Power(nu, 10) + mpfc("372") * Power(nu, 11) +
                                                         Power(nu, 12)) + mpfc("497420") * Power(lambda, 9) *
                                                                          (mpfc("10380381901553664000") +
                                                                           mpfc("4813910213433753600") * nu +
                                                                           mpfc("1024090047485706240") * Power(nu, 2) +
                                                                           mpfc("132337031391092736") * Power(nu, 3) +
                                                                           mpfc("11590755617126400") * Power(nu, 4) +
                                                                           mpfc("726656288190976") * Power(nu, 5) +
                                                                           mpfc("33550812746880") * Power(nu, 6) +
                                                                           mpfc("1155220708928") * Power(nu, 7) +
                                                                           mpfc("29665452960") * Power(nu, 8) +
                                                                           mpfc("561180048") * Power(nu, 9) +
                                                                           mpfc("7601880") * Power(nu, 10) +
                                                                           mpfc("69836") * Power(nu, 11) +
                                                                           mpfc("390") * Power(nu, 12) +
                                                                           Power(nu, 13)) +
                   mpfc("319770") * Power(lambda, 8) *
                   (mpfc("166086110424858624000") + mpfc("87402945316493721600") * nu +
                    mpfc("21199350973205053440") * Power(nu, 2) + mpfc("3141482549743190016") * Power(nu, 3) +
                    mpfc("317789121265115136") * Power(nu, 4) + mpfc("23217256228182016") * Power(nu, 5) +
                    mpfc("1263469292141056") * Power(nu, 6) + mpfc("52034344089728") * Power(nu, 7) +
                    mpfc("1629867956288") * Power(nu, 8) + mpfc("38644333728") * Power(nu, 9) +
                    mpfc("682810128") * Power(nu, 10) + mpfc("8719256") * Power(nu, 11) +
                    mpfc("76076") * Power(nu, 12) + mpfc("406") * Power(nu, 13) + Power(nu, 14)) +
                   mpfc("170544") * Power(lambda, 7) *
                   (mpfc("2325205545948020736000") + mpfc("1389727344855770726400") * nu +
                    mpfc("384193858941364469760") * Power(nu, 2) + mpfc("65180106669609713664") * Power(nu, 3) +
                    mpfc("7590530247454801920") * Power(nu, 4) + mpfc("642830708459663360") * Power(nu, 5) +
                    mpfc("40905826318156800") * Power(nu, 6) + mpfc("1991950109397248") * Power(nu, 7) +
                    mpfc("74852495477760") * Power(nu, 8) + mpfc("2170888628480") * Power(nu, 9) +
                    mpfc("48203675520") * Power(nu, 10) + mpfc("804879712") * Power(nu, 11) +
                    mpfc("9784320") * Power(nu, 12) + mpfc("81760") * Power(nu, 13) + mpfc("420") * Power(nu, 14) +
                    Power(nu, 15)) + mpfc("74613") * Power(lambda, 6) *
                                     (mpfc("27902466551376248832000") + mpfc("19001933684217269452800") * nu +
                                      mpfc("6000053652152144363520") * Power(nu, 2) +
                                      mpfc("1166355138976681033728") * Power(nu, 3) +
                                      mpfc("156266469639067336704") * Power(nu, 4) +
                                      mpfc("15304498748970762240") * Power(nu, 5) +
                                      mpfc("1133700624277544960") * Power(nu, 6) +
                                      mpfc("64809227630923776") * Power(nu, 7) +
                                      mpfc("2890180055130368") * Power(nu, 8) + mpfc("100903159019520") * Power(nu, 9) +
                                      mpfc("2749332734720") * Power(nu, 10) + mpfc("57862232064") * Power(nu, 11) +
                                      mpfc("922291552") * Power(nu, 12) + mpfc("10765440") * Power(nu, 13) +
                                      mpfc("86800") * Power(nu, 14) + mpfc("432") * Power(nu, 15) + Power(nu, 16)) +
                   mpfc("26334") * Power(lambda, 5) *
                   (mpfc("279024665513762488320000") + mpfc("217921803393548943360000") * nu +
                    mpfc("79002470205738713088000") * Power(nu, 2) + mpfc("17663605041918954700800") * Power(nu, 3) +
                    mpfc("2729019835367354400768") * Power(nu, 4) + mpfc("309311457128774959104") * Power(nu, 5) +
                    mpfc("26641504991746211840") * Power(nu, 6) + mpfc("1781792900586782720") * Power(nu, 7) +
                    mpfc("93711028182227456") * Power(nu, 8) + mpfc("3899211645325568") * Power(nu, 9) +
                    mpfc("128396486366720") * Power(nu, 10) + mpfc("3327955055360") * Power(nu, 11) +
                    mpfc("67085147584") * Power(nu, 12) + mpfc("1029945952") * Power(nu, 13) +
                    mpfc("11633440") * Power(nu, 14) + mpfc("91120") * Power(nu, 15) + mpfc("442") * Power(nu, 16) +
                    Power(nu, 17)) + mpfc("7315") * Power(lambda, 4) *
                                     (mpfc("2232197324110099906560000") + mpfc("2022399092662154035200000") * nu +
                                      mpfc("849941565039458648064000") * Power(nu, 2) +
                                      mpfc("220311310541090350694400") * Power(nu, 3) +
                                      mpfc("39495763724857789906944") * Power(nu, 4) +
                                      mpfc("5203511492397554073600") * Power(nu, 5) +
                                      mpfc("522443497062744653824") * Power(nu, 6) +
                                      mpfc("40895848196440473600") * Power(nu, 7) +
                                      mpfc("2531481126044602368") * Power(nu, 8) +
                                      mpfc("124904721344832000") * Power(nu, 9) +
                                      mpfc("4926383536259328") * Power(nu, 10) +
                                      mpfc("155020126809600") * Power(nu, 11) + mpfc("3864636236032") * Power(nu, 12) +
                                      mpfc("75324715200") * Power(nu, 13) + mpfc("1123013472") * Power(nu, 14) +
                                      mpfc("12362400") * Power(nu, 15) + mpfc("94656") * Power(nu, 16) +
                                      mpfc("450") * Power(nu, 17) + Power(nu, 18)) + mpfc("1540") * Power(lambda, 3) *
                                                                                     (mpfc("13393183944660599439360000") +
                                                                                      mpfc("14366591880083024117760000") *
                                                                                      nu +
                                                                                      mpfc("7122048482898905923584000") *
                                                                                      Power(nu, 2) +
                                                                                      mpfc("2171809428286000752230400") *
                                                                                      Power(nu, 3) +
                                                                                      mpfc("457285892890237090136064") *
                                                                                      Power(nu, 4) +
                                                                                      mpfc("70716832679243114348544") *
                                                                                      Power(nu, 5) +
                                                                                      mpfc("8338172474774021996544") *
                                                                                      Power(nu, 6) +
                                                                                      mpfc("767818586241387495424") *
                                                                                      Power(nu, 7) +
                                                                                      mpfc("56084734952708087808") *
                                                                                      Power(nu, 8) +
                                                                                      mpfc("3280909454113594368") *
                                                                                      Power(nu, 9) +
                                                                                      mpfc("154463022562387968") *
                                                                                      Power(nu, 10) +
                                                                                      mpfc("5856504297116928") *
                                                                                      Power(nu, 11) +
                                                                                      mpfc("178207944225792") *
                                                                                      Power(nu, 12) +
                                                                                      mpfc("4316584527232") *
                                                                                      Power(nu, 13) +
                                                                                      mpfc("82062796032") *
                                                                                      Power(nu, 14) +
                                                                                      mpfc("1197187872") *
                                                                                      Power(nu, 15) +
                                                                                      mpfc("12930336") * Power(nu, 16) +
                                                                                      mpfc("97356") * Power(nu, 17) +
                                                                                      mpfc("456") * Power(nu, 18) +
                                                                                      Power(nu, 19)) +
                   mpfc("231") * Power(lambda, 2) *
                   (mpfc("53572735778642397757440000") + mpfc("70859551464992695910400000") * nu +
                    mpfc("42854785811678647812096000") * Power(nu, 2) +
                    mpfc("15809286196042908932505600") * Power(nu, 3) +
                    mpfc("4000952999846949112774656") * Power(nu, 4) + mpfc("740153223607209547530240") * Power(nu, 5) +
                    mpfc("104069522578339202334720") * Power(nu, 6) + mpfc("11409446819739571978240") * Power(nu, 7) +
                    mpfc("992157526052219846656") * Power(nu, 8) + mpfc("69208372769162465280") * Power(nu, 9) +
                    mpfc("3898761544363146240") * Power(nu, 10) + mpfc("177889039750855680") * Power(nu, 11) +
                    mpfc("6569336074020096") * Power(nu, 12) + mpfc("195474282334720") * Power(nu, 13) +
                    mpfc("4644835711360") * Power(nu, 14) + mpfc("86851547520") * Power(nu, 15) +
                    mpfc("1248909216") * Power(nu, 16) + mpfc("13319760") * Power(nu, 17) +
                    mpfc("99180") * Power(nu, 18) + mpfc("460") * Power(nu, 19) + Power(nu, 20)) + mpfc("22") * lambda *
                                                                                                   (mpfc("107145471557284795514880000") +
                                                                                                    mpfc("195291838708627789578240000") *
                                                                                                    nu +
                                                                                                    mpfc("156569123088349991534592000") *
                                                                                                    Power(nu, 2) +
                                                                                                    mpfc("74473358203764465677107200") *
                                                                                                    Power(nu, 3) +
                                                                                                    mpfc("23811192195736807158054912") *
                                                                                                    Power(nu, 4) +
                                                                                                    mpfc("5481259447061368207835136") *
                                                                                                    Power(nu, 5) +
                                                                                                    mpfc("948292268763887952199680") *
                                                                                                    Power(nu, 6) +
                                                                                                    mpfc("126888416217818346291200") *
                                                                                                    Power(nu, 7) +
                                                                                                    mpfc("13393761871844011671552") *
                                                                                                    Power(nu, 8) +
                                                                                                    mpfc("1130574271590544777216") *
                                                                                                    Power(nu, 9) +
                                                                                                    mpfc("77005895857888757760") *
                                                                                                    Power(nu, 10) +
                                                                                                    mpfc("4254539623864857600") *
                                                                                                    Power(nu, 11) +
                                                                                                    mpfc("191027711898895872") *
                                                                                                    Power(nu, 12) +
                                                                                                    mpfc("6960284638689536") *
                                                                                                    Power(nu, 13) +
                                                                                                    mpfc("204763953757440") *
                                                                                                    Power(nu, 14) +
                                                                                                    mpfc("4818538806400") *
                                                                                                    Power(nu, 15) +
                                                                                                    mpfc("89349365952") *
                                                                                                    Power(nu, 16) +
                                                                                                    mpfc("1275548736") *
                                                                                                    Power(nu, 17) +
                                                                                                    mpfc("13518120") *
                                                                                                    Power(nu, 18) +
                                                                                                    mpfc("100100") *
                                                                                                    Power(nu, 19) +
                                                                                                    mpfc("462") *
                                                                                                    Power(nu, 20) +
                                                                                                    Power(nu, 21)) +
                   nu * (mpfc("107145471557284795514880000") + mpfc("195291838708627789578240000") * nu +
                         mpfc("156569123088349991534592000") * Power(nu, 2) +
                         mpfc("74473358203764465677107200") * Power(nu, 3) +
                         mpfc("23811192195736807158054912") * Power(nu, 4) +
                         mpfc("5481259447061368207835136") * Power(nu, 5) +
                         mpfc("948292268763887952199680") * Power(nu, 6) +
                         mpfc("126888416217818346291200") * Power(nu, 7) +
                         mpfc("13393761871844011671552") * Power(nu, 8) +
                         mpfc("1130574271590544777216") * Power(nu, 9) + mpfc("77005895857888757760") * Power(nu, 10) +
                         mpfc("4254539623864857600") * Power(nu, 11) + mpfc("191027711898895872") * Power(nu, 12) +
                         mpfc("6960284638689536") * Power(nu, 13) + mpfc("204763953757440") * Power(nu, 14) +
                         mpfc("4818538806400") * Power(nu, 15) + mpfc("89349365952") * Power(nu, 16) +
                         mpfc("1275548736") * Power(nu, 17) + mpfc("13518120") * Power(nu, 18) +
                         mpfc("100100") * Power(nu, 19) + mpfc("462") * Power(nu, 20) + Power(nu, 21)));

        moment(23, Power(lambda, 23) + mpfc("23") * Power(lambda, 22) * (mpfc("44") + nu) +
                   mpfc("253") * Power(lambda, 21) * (mpfc("1848") + mpfc("86") * nu + Power(nu, 2)) +
                   mpfc("1771") * Power(lambda, 20) *
                   (mpfc("73920") + mpfc("5288") * nu + mpfc("126") * Power(nu, 2) + Power(nu, 3)) +
                   mpfc("8855") * Power(lambda, 19) *
                   (mpfc("2808960") + mpfc("274864") * nu + mpfc("10076") * Power(nu, 2) + mpfc("164") * Power(nu, 3) +
                    Power(nu, 4)) + mpfc("33649") * Power(lambda, 18) *
                                    (mpfc("101122560") + mpfc("12704064") * nu + mpfc("637600") * Power(nu, 2) +
                                     mpfc("15980") * Power(nu, 3) + mpfc("200") * Power(nu, 4) + Power(nu, 5)) +
                   mpfc("100947") * Power(lambda, 17) *
                   (mpfc("3438167040") + mpfc("533060736") * nu + mpfc("34382464") * Power(nu, 2) +
                    mpfc("1180920") * Power(nu, 3) + mpfc("22780") * Power(nu, 4) + mpfc("234") * Power(nu, 5) +
                    Power(nu, 6)) + mpfc("245157") * Power(lambda, 16) *
                                    (mpfc("110021345280") + mpfc("20496110592") * nu +
                                     mpfc("1633299584") * Power(nu, 2) + mpfc("72171904") * Power(nu, 3) +
                                     mpfc("1909880") * Power(nu, 4) + mpfc("30268") * Power(nu, 5) +
                                     mpfc("266") * Power(nu, 6) + Power(nu, 7)) + mpfc("490314") * Power(lambda, 15) *
                                                                                  (mpfc("3300640358400") +
                                                                                   mpfc("724904663040") * nu +
                                                                                   mpfc("69495098112") * Power(nu, 2) +
                                                                                   mpfc("3798456704") * Power(nu, 3) +
                                                                                   mpfc("129468304") * Power(nu, 4) +
                                                                                   mpfc("2817920") * Power(nu, 5) +
                                                                                   mpfc("38248") * Power(nu, 6) +
                                                                                   mpfc("296") * Power(nu, 7) +
                                                                                   Power(nu, 8)) +
                   mpfc("817190") * Power(lambda, 14) *
                   (mpfc("92417930035200") + mpfc("23597970923520") * nu + mpfc("2670767410176") * Power(nu, 2) +
                    mpfc("175851885824") * Power(nu, 3) + mpfc("7423569216") * Power(nu, 4) +
                    mpfc("208370064") * Power(nu, 5) + mpfc("3888864") * Power(nu, 6) + mpfc("46536") * Power(nu, 7) +
                    mpfc("324") * Power(nu, 8) + Power(nu, 9)) + mpfc("1144066") * Power(lambda, 13) *
                                                                 (mpfc("2402866180915200") +
                                                                  mpfc("705965174046720") * nu +
                                                                  mpfc("93037923588096") * Power(nu, 2) +
                                                                  mpfc("7242916441600") * Power(nu, 3) +
                                                                  mpfc("368864685440") * Power(nu, 4) +
                                                                  mpfc("12841190880") * Power(nu, 5) +
                                                                  mpfc("309480528") * Power(nu, 6) +
                                                                  mpfc("5098800") * Power(nu, 7) +
                                                                  mpfc("54960") * Power(nu, 8) +
                                                                  mpfc("350") * Power(nu, 9) + Power(nu, 10)) +
                   mpfc("1352078") * Power(lambda, 12) * (mpfc("57668788341964800") + mpfc("19346030358036480") * nu +
                                                          mpfc("2938875340161024") * Power(nu, 2) +
                                                          mpfc("266867918186496") * Power(nu, 3) +
                                                          mpfc("16095668892160") * Power(nu, 4) +
                                                          mpfc("677053266560") * Power(nu, 5) +
                                                          mpfc("20268723552") * Power(nu, 6) +
                                                          mpfc("431851728") * Power(nu, 7) +
                                                          mpfc("6417840") * Power(nu, 8) +
                                                          mpfc("63360") * Power(nu, 9) + mpfc("374") * Power(nu, 10) +
                                                          Power(nu, 11)) + mpfc("1352078") * Power(lambda, 11) *
                                                                           (mpfc("1268713343523225600") +
                                                                            mpfc("483281456218767360") * nu +
                                                                            mpfc("84001287841579008") * Power(nu, 2) +
                                                                            mpfc("8809969540263936") * Power(nu, 3) +
                                                                            mpfc("620972633814016") * Power(nu, 4) +
                                                                            mpfc("30990840756480") * Power(nu, 5) +
                                                                            mpfc("1122965184704") * Power(nu, 6) +
                                                                            mpfc("29769461568") * Power(nu, 7) +
                                                                            mpfc("573044208") * Power(nu, 8) +
                                                                            mpfc("7811760") * Power(nu, 9) +
                                                                            mpfc("71588") * Power(nu, 10) +
                                                                            mpfc("396") * Power(nu, 11) +
                                                                            Power(nu, 12)) +
                   mpfc("1144066") * Power(lambda, 10) *
                   (mpfc("25374266870464512000") + mpfc("10934342467898572800") * nu +
                    mpfc("2163307213050347520") * Power(nu, 2) + mpfc("260200678646857728") * Power(nu, 3) +
                    mpfc("21229422216544256") * Power(nu, 4) + mpfc("1240789448943616") * Power(nu, 5) +
                    mpfc("53450144450560") * Power(nu, 6) + mpfc("1718354416064") * Power(nu, 7) +
                    mpfc("41230345728") * Power(nu, 8) + mpfc("729279408") * Power(nu, 9) +
                    mpfc("9243520") * Power(nu, 10) + mpfc("79508") * Power(nu, 11) + mpfc("416") * Power(nu, 12) +
                    Power(nu, 13)) + mpfc("817190") * Power(lambda, 9) *
                                     (mpfc("456736803668361216000") + mpfc("222192431292638822400") * nu +
                                      mpfc("49873872302804828160") * Power(nu, 2) +
                                      mpfc("6846919428693786624") * Power(nu, 3) +
                                      mpfc("642330278544654336") * Power(nu, 4) +
                                      mpfc("43563632297529344") * Power(nu, 5) +
                                      mpfc("2202892049053696") * Power(nu, 6) + mpfc("84380523939712") * Power(nu, 7) +
                                      mpfc("2460500639168") * Power(nu, 8) + mpfc("54357375072") * Power(nu, 9) +
                                      mpfc("895662768") * Power(nu, 10) + mpfc("10674664") * Power(nu, 11) +
                                      mpfc("86996") * Power(nu, 12) + mpfc("434") * Power(nu, 13) + Power(nu, 14)) +
                   mpfc("490314") * Power(lambda, 8) *
                   (mpfc("7307788858693779456000") + mpfc("4011815704350582374400") * nu +
                    mpfc("1020174388137516072960") * Power(nu, 2) + mpfc("159424583161905414144") * Power(nu, 3) +
                    mpfc("17124203885408256000") * Power(nu, 4) + mpfc("1339348395305123840") * Power(nu, 5) +
                    mpfc("78809905082388480") * Power(nu, 6) + mpfc("3552980432089088") * Power(nu, 7) +
                    mpfc("123748534166400") * Power(nu, 8) + mpfc("3330218640320") * Power(nu, 9) +
                    mpfc("68687979360") * Power(nu, 10) + mpfc("1066457392") * Power(nu, 11) +
                    mpfc("12066600") * Power(nu, 12) + mpfc("93940") * Power(nu, 13) + mpfc("450") * Power(nu, 14) +
                    Power(nu, 15)) + mpfc("245157") * Power(lambda, 7) *
                                     (mpfc("102309044021712912384000") + mpfc("63473208719601932697600") * nu +
                                      mpfc("18294257138275807395840") * Power(nu, 2) +
                                      mpfc("3252118552404191870976") * Power(nu, 3) +
                                      mpfc("399163437557620998144") * Power(nu, 4) +
                                      mpfc("35875081419679989760") * Power(nu, 5) +
                                      mpfc("2442687066458562560") * Power(nu, 6) +
                                      mpfc("128551631131635712") * Power(nu, 7) +
                                      mpfc("5285459910418688") * Power(nu, 8) + mpfc("170371595130880") * Power(nu, 9) +
                                      mpfc("4291850351360") * Power(nu, 10) + mpfc("83618382848") * Power(nu, 11) +
                                      mpfc("1235389792") * Power(nu, 12) + mpfc("13381760") * Power(nu, 13) +
                                      mpfc("100240") * Power(nu, 14) + mpfc("464") * Power(nu, 15) + Power(nu, 16)) +
                   mpfc("100947") * Power(lambda, 6) *
                   (mpfc("1227708528260554948608000") + mpfc("863987548656936104755200") * nu +
                    mpfc("283004294378911621447680") * Power(nu, 2) + mpfc("57319679767126109847552") * Power(nu, 3) +
                    mpfc("8042079803095643848704") * Power(nu, 4) + mpfc("829664414593780875264") * Power(nu, 5) +
                    mpfc("65187326217182740480") * Power(nu, 6) + mpfc("3985306640038191104") * Power(nu, 7) +
                    mpfc("191977150056659968") * Power(nu, 8) + mpfc("7329919051989248") * Power(nu, 9) +
                    mpfc("221873799347200") * Power(nu, 10) + mpfc("5295270945536") * Power(nu, 11) +
                    mpfc("98443060352") * Power(nu, 12) + mpfc("1395970912") * Power(nu, 13) +
                    mpfc("14584640") * Power(nu, 14) + mpfc("105808") * Power(nu, 15) + mpfc("476") * Power(nu, 16) +
                    Power(nu, 17)) + mpfc("33649") * Power(lambda, 5) *
                                     (mpfc("12277085282605549486080000") + mpfc("9867584014829915996160000") * nu +
                                      mpfc("3694030492446052319232000") * Power(nu, 2) +
                                      mpfc("856201092050172719923200") * Power(nu, 3) +
                                      mpfc("137740477798082548334592") * Power(nu, 4) +
                                      mpfc("16338723949033452601344") * Power(nu, 5) +
                                      mpfc("1481537676765608280064") * Power(nu, 6) +
                                      mpfc("105040392617564651520") * Power(nu, 7) +
                                      mpfc("5905078140604790784") * Power(nu, 8) +
                                      mpfc("265276340576552448") * Power(nu, 9) +
                                      mpfc("9548657045461248") * Power(nu, 10) +
                                      mpfc("274826508802560") * Power(nu, 11) + mpfc("6279701549056") * Power(nu, 12) +
                                      mpfc("112402769472") * Power(nu, 13) + mpfc("1541817312") * Power(nu, 14) +
                                      mpfc("15642720") * Power(nu, 15) + mpfc("110568") * Power(nu, 16) +
                                      mpfc("486") * Power(nu, 17) + Power(nu, 18)) + mpfc("8855") * Power(lambda, 4) *
                                                                                     (mpfc("98216682260844395888640000") +
                                                                                      mpfc("91217757401244877455360000") *
                                                                                      nu +
                                                                                      mpfc("39419827954398334550016000") *
                                                                                      Power(nu, 2) +
                                                                                      mpfc("10543639228847434078617600") *
                                                                                      Power(nu, 3) +
                                                                                      mpfc("1958124914434833106599936") *
                                                                                      Power(nu, 4) +
                                                                                      mpfc("268450269390350169145344") *
                                                                                      Power(nu, 5) +
                                                                                      mpfc("28191025363158318841856") *
                                                                                      Power(nu, 6) +
                                                                                      mpfc("2321860817706125492224") *
                                                                                      Power(nu, 7) +
                                                                                      mpfc("152281017742402977792") *
                                                                                      Power(nu, 8) +
                                                                                      mpfc("8027288865217210368") *
                                                                                      Power(nu, 9) +
                                                                                      mpfc("341665596940242432") *
                                                                                      Power(nu, 10) +
                                                                                      mpfc("11747269115881728") *
                                                                                      Power(nu, 11) +
                                                                                      mpfc("325064121195008") *
                                                                                      Power(nu, 12) +
                                                                                      mpfc("7178923704832") *
                                                                                      Power(nu, 13) +
                                                                                      mpfc("124737307968") *
                                                                                      Power(nu, 14) +
                                                                                      mpfc("1666959072") *
                                                                                      Power(nu, 15) +
                                                                                      mpfc("16527264") * Power(nu, 16) +
                                                                                      mpfc("114456") * Power(nu, 17) +
                                                                                      mpfc("494") * Power(nu, 18) +
                                                                                      Power(nu, 19)) +
                   mpfc("1771") * Power(lambda, 3) *
                   (mpfc("589300093565066375331840000") + mpfc("645523226668313660620800000") * nu +
                    mpfc("327736725127634884755456000") * Power(nu, 2) +
                    mpfc("102681663327482939021721600") * Power(nu, 3) +
                    mpfc("22292388715456432718217216") * Power(nu, 4) +
                    mpfc("3568826530776934121472000") * Power(nu, 5) + mpfc("437596421569300082196480") * Power(nu, 6) +
                    mpfc("42122190269395071795200") * Power(nu, 7) + mpfc("3235546924160543358976") * Power(nu, 8) +
                    mpfc("200444750933706240000") * Power(nu, 9) + mpfc("10077282446858664960") * Power(nu, 10) +
                    mpfc("412149211635532800") * Power(nu, 11) + mpfc("13697653843051776") * Power(nu, 12) +
                    mpfc("368137663424000") * Power(nu, 13) + mpfc("7927347552640") * Power(nu, 14) +
                    mpfc("134739062400") * Power(nu, 15) + mpfc("1766122656") * Power(nu, 16) +
                    mpfc("17214000") * Power(nu, 17) + mpfc("117420") * Power(nu, 18) + mpfc("500") * Power(nu, 19) +
                    Power(nu, 20)) + mpfc("253") * Power(lambda, 2) *
                                     (mpfc("2357200374260265501327360000") + mpfc("3171393000238321017815040000") * nu +
                                      mpfc("1956470127178853199642624000") * Power(nu, 2) +
                                      mpfc("738463378437566640842342400") * Power(nu, 3) +
                                      mpfc("191851218189308669894590464") * Power(nu, 4) +
                                      mpfc("36567694838564169204105216") * Power(nu, 5) +
                                      mpfc("5319212217054134450257920") * Power(nu, 6) +
                                      mpfc("606085182646880369377280") * Power(nu, 7) +
                                      mpfc("55064377966037245231104") * Power(nu, 8) +
                                      mpfc("4037325927895368318976") * Power(nu, 9) +
                                      mpfc("240753880721140899840") * Power(nu, 10) +
                                      mpfc("11725879293400796160") * Power(nu, 11) +
                                      mpfc("466939827007739904") * Power(nu, 12) +
                                      mpfc("15170204496747776") * Power(nu, 13) +
                                      mpfc("399847053634560") * Power(nu, 14) + mpfc("8466303802240") * Power(nu, 15) +
                                      mpfc("141803553024") * Power(nu, 16) + mpfc("1834978656") * Power(nu, 17) +
                                      mpfc("17683680") * Power(nu, 18) + mpfc("119420") * Power(nu, 19) +
                                      mpfc("504") * Power(nu, 20) + Power(nu, 21)) + mpfc("23") * lambda *
                                                                                     (mpfc("4714400748520531002654720000") +
                                                                                      mpfc("8699986374736907536957440000") *
                                                                                      nu +
                                                                                      mpfc("7084333254596027417100288000") *
                                                                                      Power(nu, 2) +
                                                                                      mpfc("3433396884053986481327308800") *
                                                                                      Power(nu, 3) +
                                                                                      mpfc("1122165814816183980631523328") *
                                                                                      Power(nu, 4) +
                                                                                      mpfc("264986607866437008302800896") *
                                                                                      Power(nu, 5) +
                                                                                      mpfc("47206119272672438104621056") *
                                                                                      Power(nu, 6) +
                                                                                      mpfc("6531382582347895189012480") *
                                                                                      Power(nu, 7) +
                                                                                      mpfc("716213938578954859839488") *
                                                                                      Power(nu, 8) +
                                                                                      mpfc("63139029821827981869056") *
                                                                                      Power(nu, 9) +
                                                                                      mpfc("4518833689337650118656") *
                                                                                      Power(nu, 10) +
                                                                                      mpfc("264205639307942492160") *
                                                                                      Power(nu, 11) +
                                                                                      mpfc("12659758947416275968") *
                                                                                      Power(nu, 12) +
                                                                                      mpfc("497280236001235456") *
                                                                                      Power(nu, 13) +
                                                                                      mpfc("15969898604016896") *
                                                                                      Power(nu, 14) +
                                                                                      mpfc("416779661239040") *
                                                                                      Power(nu, 15) +
                                                                                      mpfc("8749910908288") *
                                                                                      Power(nu, 16) +
                                                                                      mpfc("145473510336") *
                                                                                      Power(nu, 17) +
                                                                                      mpfc("1870346016") *
                                                                                      Power(nu, 18) +
                                                                                      mpfc("17922520") * Power(nu, 19) +
                                                                                      mpfc("120428") * Power(nu, 20) +
                                                                                      mpfc("506") * Power(nu, 21) +
                                                                                      Power(nu, 22)) + nu *
                                                                                                       (mpfc("4714400748520531002654720000") +
                                                                                                        mpfc("8699986374736907536957440000") *
                                                                                                        nu +
                                                                                                        mpfc("7084333254596027417100288000") *
                                                                                                        Power(nu, 2) +
                                                                                                        mpfc("3433396884053986481327308800") *
                                                                                                        Power(nu, 3) +
                                                                                                        mpfc("1122165814816183980631523328") *
                                                                                                        Power(nu, 4) +
                                                                                                        mpfc("264986607866437008302800896") *
                                                                                                        Power(nu, 5) +
                                                                                                        mpfc("47206119272672438104621056") *
                                                                                                        Power(nu, 6) +
                                                                                                        mpfc("6531382582347895189012480") *
                                                                                                        Power(nu, 7) +
                                                                                                        mpfc("716213938578954859839488") *
                                                                                                        Power(nu, 8) +
                                                                                                        mpfc("63139029821827981869056") *
                                                                                                        Power(nu, 9) +
                                                                                                        mpfc("4518833689337650118656") *
                                                                                                        Power(nu, 10) +
                                                                                                        mpfc("264205639307942492160") *
                                                                                                        Power(nu, 11) +
                                                                                                        mpfc("12659758947416275968") *
                                                                                                        Power(nu, 12) +
                                                                                                        mpfc("497280236001235456") *
                                                                                                        Power(nu, 13) +
                                                                                                        mpfc("15969898604016896") *
                                                                                                        Power(nu, 14) +
                                                                                                        mpfc("416779661239040") *
                                                                                                        Power(nu, 15) +
                                                                                                        mpfc("8749910908288") *
                                                                                                        Power(nu, 16) +
                                                                                                        mpfc("145473510336") *
                                                                                                        Power(nu, 17) +
                                                                                                        mpfc("1870346016") *
                                                                                                        Power(nu, 18) +
                                                                                                        mpfc("17922520") *
                                                                                                        Power(nu, 19) +
                                                                                                        mpfc("120428") *
                                                                                                        Power(nu, 20) +
                                                                                                        mpfc("506") *
                                                                                                        Power(nu, 21) +
                                                                                                        Power(nu, 22)));

        moment(24, Power(lambda, 24) + mpfc("24") * Power(lambda, 23) * (mpfc("46") + nu) +
                   mpfc("276") * Power(lambda, 22) * (mpfc("2024") + mpfc("90") * nu + Power(nu, 2)) +
                   mpfc("2024") * Power(lambda, 21) *
                   (mpfc("85008") + mpfc("5804") * nu + mpfc("132") * Power(nu, 2) + Power(nu, 3)) +
                   mpfc("10626") * Power(lambda, 20) *
                   (mpfc("3400320") + mpfc("317168") * nu + mpfc("11084") * Power(nu, 2) + mpfc("172") * Power(nu, 3) +
                    Power(nu, 4)) + mpfc("42504") * Power(lambda, 19) *
                                    (mpfc("129212160") + mpfc("15452704") * nu + mpfc("738360") * Power(nu, 2) +
                                     mpfc("17620") * Power(nu, 3) + mpfc("210") * Power(nu, 4) + Power(nu, 5)) +
                   mpfc("134596") * Power(lambda, 18) *
                   (mpfc("4651637760") + mpfc("685509504") * nu + mpfc("42033664") * Power(nu, 2) +
                    mpfc("1372680") * Power(nu, 3) + mpfc("25180") * Power(nu, 4) + mpfc("246") * Power(nu, 5) +
                    Power(nu, 6)) + mpfc("346104") * Power(lambda, 17) *
                                    (mpfc("158155683840") + mpfc("27958960896") * nu +
                                     mpfc("2114654080") * Power(nu, 2) + mpfc("88704784") * Power(nu, 3) +
                                     mpfc("2228800") * Power(nu, 4) + mpfc("33544") * Power(nu, 5) +
                                     mpfc("280") * Power(nu, 6) + Power(nu, 7)) + mpfc("735471") * Power(lambda, 16) *
                                                                                  (mpfc("5060981882880") +
                                                                                   mpfc("1052842432512") * nu +
                                                                                   mpfc("95627891456") * Power(nu, 2) +
                                                                                   mpfc("4953207168") * Power(nu, 3) +
                                                                                   mpfc("160026384") * Power(nu, 4) +
                                                                                   mpfc("3302208") * Power(nu, 5) +
                                                                                   mpfc("42504") * Power(nu, 6) +
                                                                                   mpfc("312") * Power(nu, 7) +
                                                                                   Power(nu, 8)) +
                   mpfc("1307504") * Power(lambda, 15) *
                   (mpfc("151829456486400") + mpfc("36646254858240") * nu + mpfc("3921679176192") * Power(nu, 2) +
                    mpfc("244224106496") * Power(nu, 3) + mpfc("9753998688") * Power(nu, 4) +
                    mpfc("259092624") * Power(nu, 5) + mpfc("4577328") * Power(nu, 6) + mpfc("51864") * Power(nu, 7) +
                    mpfc("342") * Power(nu, 8) + Power(nu, 9)) + mpfc("1961256") * Power(lambda, 14) *
                                                                 (mpfc("4251224781619200") +
                                                                  mpfc("1177924592517120") * nu +
                                                                  mpfc("146453271791616") * Power(nu, 2) +
                                                                  mpfc("10759954158080") * Power(nu, 3) +
                                                                  mpfc("517336069760") * Power(nu, 4) +
                                                                  mpfc("17008592160") * Power(nu, 5) +
                                                                  mpfc("387257808") * Power(nu, 6) +
                                                                  mpfc("6029520") * Power(nu, 7) +
                                                                  mpfc("61440") * Power(nu, 8) +
                                                                  mpfc("370") * Power(nu, 9) + Power(nu, 10)) +
                   mpfc("2496144") * Power(lambda, 13) * (mpfc("110531844322099200") + mpfc("34877264187064320") * nu +
                                                          mpfc("4985709659099136") * Power(nu, 2) +
                                                          mpfc("426212079901696") * Power(nu, 3) +
                                                          mpfc("24210691971840") * Power(nu, 4) +
                                                          mpfc("959559465920") * Power(nu, 5) +
                                                          mpfc("27077295168") * Power(nu, 6) +
                                                          mpfc("544025328") * Power(nu, 7) +
                                                          mpfc("7626960") * Power(nu, 8) +
                                                          mpfc("71060") * Power(nu, 9) + mpfc("396") * Power(nu, 10) +
                                                          Power(nu, 11)) + mpfc("2704156") * Power(lambda, 12) *
                                                                           (mpfc("2652764263730380800") +
                                                                            mpfc("947586184811642880") * nu +
                                                                            mpfc("154534296005443584") * Power(nu, 2) +
                                                                            mpfc("15214799576739840") * Power(nu, 3) +
                                                                            mpfc("1007268687225856") * Power(nu, 4) +
                                                                            mpfc("47240119153920") * Power(nu, 5) +
                                                                            mpfc("1609414549952") * Power(nu, 6) +
                                                                            mpfc("40133903040") * Power(nu, 7) +
                                                                            mpfc("727072368") * Power(nu, 8) +
                                                                            mpfc("9332400") * Power(nu, 9) +
                                                                            mpfc("80564") * Power(nu, 10) +
                                                                            mpfc("420") * Power(nu, 11) +
                                                                            Power(nu, 12)) +
                   mpfc("2496144") * Power(lambda, 11) *
                   (mpfc("58360813802068377600") + mpfc("23499660329586524160") * nu +
                    mpfc("4347340696931401728") * Power(nu, 2) + mpfc("489259886693720064") * Power(nu, 3) +
                    mpfc("37374710695708672") * Power(nu, 4) + mpfc("2046551308612096") * Power(nu, 5) +
                    mpfc("82647239252864") * Power(nu, 6) + mpfc("2492360416832") * Power(nu, 7) +
                    mpfc("56129495136") * Power(nu, 8) + mpfc("932385168") * Power(nu, 9) +
                    mpfc("11104808") * Power(nu, 10) + mpfc("89804") * Power(nu, 11) + mpfc("442") * Power(nu, 12) +
                    Power(nu, 13)) + mpfc("1961256") * Power(lambda, 10) *
                                     (mpfc("1167216276041367552000") + mpfc("528354020393798860800") * nu +
                                      mpfc("110446474268214558720") * Power(nu, 2) +
                                      mpfc("14132538430805803008") * Power(nu, 3) +
                                      mpfc("1236754100607893504") * Power(nu, 4) +
                                      mpfc("78305736867950592") * Power(nu, 5) +
                                      mpfc("3699496093669376") * Power(nu, 6) + mpfc("132494447589504") * Power(nu, 7) +
                                      mpfc("3614950319552") * Power(nu, 8) + mpfc("74777198496") * Power(nu, 9) +
                                      mpfc("1154481328") * Power(nu, 10) + mpfc("12900888") * Power(nu, 11) +
                                      mpfc("98644") * Power(nu, 12) + mpfc("462") * Power(nu, 13) + Power(nu, 14)) +
                   mpfc("1307504") * Power(lambda, 9) *
                   (mpfc("21009892968744615936000") + mpfc("10677588643129747046400") * nu +
                    mpfc("2516390557221660917760") * Power(nu, 2) + mpfc("364832166022719012864") * Power(nu, 3) +
                    mpfc("36394112241747886080") * Power(nu, 4) + mpfc("2646257364231004160") * Power(nu, 5) +
                    mpfc("144896666553999360") * Power(nu, 6) + mpfc("6084396150280448") * Power(nu, 7) +
                    mpfc("197563553341440") * Power(nu, 8) + mpfc("4960939892480") * Power(nu, 9) +
                    mpfc("95557862400") * Power(nu, 10) + mpfc("1386697312") * Power(nu, 11) +
                    mpfc("14676480") * Power(nu, 12) + mpfc("106960") * Power(nu, 13) + mpfc("480") * Power(nu, 14) +
                    Power(nu, 15)) + mpfc("735471") * Power(lambda, 8) *
                                     (mpfc("336158287499913854976000") + mpfc("191851311258820568678400") * nu +
                                      mpfc("50939837558676321730560") * Power(nu, 2) +
                                      mpfc("8353705213585165123584") * Power(nu, 3) +
                                      mpfc("947137961890685190144") * Power(nu, 4) +
                                      mpfc("78734230069443952640") * Power(nu, 5) +
                                      mpfc("4964604029094993920") * Power(nu, 6) +
                                      mpfc("242247004958486528") * Power(nu, 7) +
                                      mpfc("9245413003743488") * Power(nu, 8) + mpfc("276938591621120") * Power(nu, 9) +
                                      mpfc("6489865690880") * Power(nu, 10) + mpfc("117745019392") * Power(nu, 11) +
                                      mpfc("1621520992") * Power(nu, 12) + mpfc("16387840") * Power(nu, 13) +
                                      mpfc("114640") * Power(nu, 14) + mpfc("496") * Power(nu, 15) + Power(nu, 16)) +
                   mpfc("346104") * Power(lambda, 7) *
                   (mpfc("4706216024998793969664000") + mpfc("3022076645123401816473600") * nu +
                    mpfc("905009037080289072906240") * Power(nu, 2) + mpfc("167891710548868633460736") * Power(nu, 3) +
                    mpfc("21613636680054757785600") * Power(nu, 4) + mpfc("2049417182862900527104") * Power(nu, 5) +
                    mpfc("148238686476773867520") * Power(nu, 6) + mpfc("8356062098513805312") * Power(nu, 7) +
                    mpfc("371682787010895360") * Power(nu, 8) + mpfc("13122553286439168") * Power(nu, 9) +
                    mpfc("367796711293440") * Power(nu, 10) + mpfc("8138295962368") * Power(nu, 11) +
                    mpfc("140446313280") * Power(nu, 12) + mpfc("1850950752") * Power(nu, 13) +
                    mpfc("17992800") * Power(nu, 14) + mpfc("121584") * Power(nu, 15) + mpfc("510") * Power(nu, 16) +
                    Power(nu, 17)) + mpfc("134596") * Power(lambda, 6) *
                                     (mpfc("56474592299985527635968000") + mpfc("40971135766479615767347200") * nu +
                                      mpfc("13882185090086870691348480") * Power(nu, 2) +
                                      mpfc("2919709563666712674435072") * Power(nu, 3) +
                                      mpfc("427255350709525726887936") * Power(nu, 4) +
                                      mpfc("46206642874409564110848") * Power(nu, 5) +
                                      mpfc("3828281420584186937344") * Power(nu, 6) +
                                      mpfc("248511431658939531264") * Power(nu, 7) +
                                      mpfc("12816255542644549632") * Power(nu, 8) +
                                      mpfc("529153426448165376") * Power(nu, 9) +
                                      mpfc("17536113821960448") * Power(nu, 10) +
                                      mpfc("465456262841856") * Power(nu, 11) + mpfc("9823651721728") * Power(nu, 12) +
                                      mpfc("162657722304") * Power(nu, 13) + mpfc("2066864352") * Power(nu, 14) +
                                      mpfc("19451808") * Power(nu, 15) + mpfc("127704") * Power(nu, 16) +
                                      mpfc("522") * Power(nu, 17) + Power(nu, 18)) + mpfc("42504") * Power(lambda, 5) *
                                                                                     (mpfc("564745922999855276359680000") +
                                                                                      mpfc("466185949964781685309440000") *
                                                                                      nu +
                                                                                      mpfc("179792986667348322680832000") *
                                                                                      Power(nu, 2) +
                                                                                      mpfc("43079280726753997435699200") *
                                                                                      Power(nu, 3) +
                                                                                      mpfc("7192263070761969943314432") *
                                                                                      Power(nu, 4) +
                                                                                      mpfc("889321779453621367996416") *
                                                                                      Power(nu, 5) +
                                                                                      mpfc("84489457080251433484288") *
                                                                                      Power(nu, 6) +
                                                                                      mpfc("6313395737173582249984") *
                                                                                      Power(nu, 7) +
                                                                                      mpfc("376673987085385027584") *
                                                                                      Power(nu, 8) +
                                                                                      mpfc("18107789807126203392") *
                                                                                      Power(nu, 9) +
                                                                                      mpfc("704514564667769856") *
                                                                                      Power(nu, 10) +
                                                                                      mpfc("22190676450379008") *
                                                                                      Power(nu, 11) +
                                                                                      mpfc("563692780059136") *
                                                                                      Power(nu, 12) +
                                                                                      mpfc("11450228944768") *
                                                                                      Power(nu, 13) +
                                                                                      mpfc("183326365824") *
                                                                                      Power(nu, 14) +
                                                                                      mpfc("2261382432") *
                                                                                      Power(nu, 15) +
                                                                                      mpfc("20728848") * Power(nu, 16) +
                                                                                      mpfc("132924") * Power(nu, 17) +
                                                                                      mpfc("532") * Power(nu, 18) +
                                                                                      Power(nu, 19)) +
                   mpfc("10626") * Power(lambda, 4) *
                   (mpfc("4517967383998842210877440000") + mpfc("4294233522718108758835200000") * nu +
                    mpfc("1904529843303568266756096000") * Power(nu, 2) +
                    mpfc("524427232481380302166425600") * Power(nu, 3) +
                    mpfc("100617385292849756982214656") * Power(nu, 4) +
                    mpfc("14306837306390940887285760") * Power(nu, 5) +
                    mpfc("1565237436095632835870720") * Power(nu, 6) + mpfc("134996622977640091484160") * Power(nu, 7) +
                    mpfc("9326787633856662470656") * Power(nu, 8) + mpfc("521536305542394654720") * Power(nu, 9) +
                    mpfc("23743906324468362240") * Power(nu, 10) + mpfc("882039976270801920") * Power(nu, 11) +
                    mpfc("26700218690852096") * Power(nu, 12) + mpfc("655294611617280") * Power(nu, 13) +
                    mpfc("12916839871360") * Power(nu, 14) + mpfc("201417425280") * Power(nu, 15) +
                    mpfc("2427213216") * Power(nu, 16) + mpfc("21792240") * Power(nu, 17) +
                    mpfc("137180") * Power(nu, 18) + mpfc("540") * Power(nu, 19) + Power(nu, 20)) +
                   mpfc("2024") * Power(lambda, 3) *
                   (mpfc("27107804303993053265264640000") + mpfc("30283368520307494763888640000") * nu +
                    mpfc("15721412582539518359371776000") * Power(nu, 2) +
                    mpfc("5051093238191850079754649600") * Power(nu, 3) +
                    mpfc("1128131544238478844059713536") * Power(nu, 4) +
                    mpfc("186458409131195402305929216") * Power(nu, 5) +
                    mpfc("23698261922964737902510080") * Power(nu, 6) +
                    mpfc("2375217173961473384775680") * Power(nu, 7) + mpfc("190957348780780066308096") * Power(nu, 8) +
                    mpfc("12456005467111030398976") * Power(nu, 9) + mpfc("663999743489204828160") * Power(nu, 10) +
                    mpfc("29036146182093173760") * Power(nu, 11) + mpfc("1042241288415914496") * Power(nu, 12) +
                    mpfc("30631986360555776") * Power(nu, 13) + mpfc("732795650845440") * Power(nu, 14) +
                    mpfc("14125344423040") * Power(nu, 15) + mpfc("215980704576") * Power(nu, 16) +
                    mpfc("2557966656") * Power(nu, 17) + mpfc("22615320") * Power(nu, 18) +
                    mpfc("140420") * Power(nu, 19) + mpfc("546") * Power(nu, 20) + Power(nu, 21)) +
                   mpfc("276") * Power(lambda, 2) *
                   (mpfc("108431217215972213061058560000") + mpfc("148241278385223032320819200000") * nu +
                    mpfc("93169018850465568201375744000") * Power(nu, 2) +
                    mpfc("35925785535306918678390374400") * Power(nu, 3) +
                    mpfc("9563619415145765455993503744") * Power(nu, 4) +
                    mpfc("1873965180763260453283430400") * Power(nu, 5) +
                    mpfc("281251456823054353915969536") * Power(nu, 6) +
                    mpfc("33199130618810631441612800") * Power(nu, 7) +
                    mpfc("3139046569084593650008064") * Power(nu, 8) + mpfc("240781370649224187904000") * Power(nu, 9) +
                    mpfc("15112004441067849711616") * Power(nu, 10) + mpfc("780144328217577523200") * Power(nu, 11) +
                    mpfc("33205111335756831744") * Power(nu, 12) + mpfc("1164769233858137600") * Power(nu, 13) +
                    mpfc("33563168963937536") * Power(nu, 14) + mpfc("789297028537600") * Power(nu, 15) +
                    mpfc("14989267241344") * Power(nu, 16) + mpfc("226212571200") * Power(nu, 17) +
                    mpfc("2648427936") * Power(nu, 18) + mpfc("23177000") * Power(nu, 19) +
                    mpfc("142604") * Power(nu, 20) + mpfc("550") * Power(nu, 21) + Power(nu, 22)) +
                   mpfc("24") * lambda *
                   (mpfc("216862434431944426122117120000") + mpfc("404913773986418277702696960000") * nu +
                    mpfc("334579316086154168723570688000") * Power(nu, 2) +
                    mpfc("165020589921079405558156492800") * Power(nu, 3) +
                    mpfc("55053024365598449590377381888") * Power(nu, 4) +
                    mpfc("13311549776672286362560364544") * Power(nu, 5) +
                    mpfc("2436468094409369161115369472") * Power(nu, 6) +
                    mpfc("347649718060675616799195136") * Power(nu, 7) +
                    mpfc("39477223756979818741628928") * Power(nu, 8) +
                    mpfc("3620609310383042025816064") * Power(nu, 9) +
                    mpfc("271005379531359887327232") * Power(nu, 10) + mpfc("16672293097503004758016") * Power(nu, 11) +
                    mpfc("846554550889091186688") * Power(nu, 12) + mpfc("35534649803473106944") * Power(nu, 13) +
                    mpfc("1231895571786012672") * Power(nu, 14) + mpfc("35141763021012736") * Power(nu, 15) +
                    mpfc("819275563020288") * Power(nu, 16) + mpfc("15441692383744") * Power(nu, 17) +
                    mpfc("231509427072") * Power(nu, 18) + mpfc("2694781936") * Power(nu, 19) +
                    mpfc("23462208") * Power(nu, 20) + mpfc("143704") * Power(nu, 21) + mpfc("552") * Power(nu, 22) +
                    Power(nu, 23)) + nu * (mpfc("216862434431944426122117120000") +
                                           mpfc("404913773986418277702696960000") * nu +
                                           mpfc("334579316086154168723570688000") * Power(nu, 2) +
                                           mpfc("165020589921079405558156492800") * Power(nu, 3) +
                                           mpfc("55053024365598449590377381888") * Power(nu, 4) +
                                           mpfc("13311549776672286362560364544") * Power(nu, 5) +
                                           mpfc("2436468094409369161115369472") * Power(nu, 6) +
                                           mpfc("347649718060675616799195136") * Power(nu, 7) +
                                           mpfc("39477223756979818741628928") * Power(nu, 8) +
                                           mpfc("3620609310383042025816064") * Power(nu, 9) +
                                           mpfc("271005379531359887327232") * Power(nu, 10) +
                                           mpfc("16672293097503004758016") * Power(nu, 11) +
                                           mpfc("846554550889091186688") * Power(nu, 12) +
                                           mpfc("35534649803473106944") * Power(nu, 13) +
                                           mpfc("1231895571786012672") * Power(nu, 14) +
                                           mpfc("35141763021012736") * Power(nu, 15) +
                                           mpfc("819275563020288") * Power(nu, 16) +
                                           mpfc("15441692383744") * Power(nu, 17) +
                                           mpfc("231509427072") * Power(nu, 18) + mpfc("2694781936") * Power(nu, 19) +
                                           mpfc("23462208") * Power(nu, 20) + mpfc("143704") * Power(nu, 21) +
                                           mpfc("552") * Power(nu, 22) + Power(nu, 23)));

        moment(25, Power(lambda, 25) + mpfc("25") * Power(lambda, 24) * (mpfc("48") + nu) +
                   mpfc("300") * Power(lambda, 23) * (mpfc("2208") + mpfc("94") * nu + Power(nu, 2)) +
                   mpfc("2300") * Power(lambda, 22) *
                   (mpfc("97152") + mpfc("6344") * nu + mpfc("138") * Power(nu, 2) + Power(nu, 3)) +
                   mpfc("12650") * Power(lambda, 21) *
                   (mpfc("4080384") + mpfc("363600") * nu + mpfc("12140") * Power(nu, 2) + mpfc("180") * Power(nu, 3) +
                    Power(nu, 4)) + mpfc("53130") * Power(lambda, 20) *
                                    (mpfc("163215360") + mpfc("18624384") * nu + mpfc("849200") * Power(nu, 2) +
                                     mpfc("19340") * Power(nu, 3) + mpfc("220") * Power(nu, 4) + Power(nu, 5)) +
                   mpfc("177100") * Power(lambda, 19) *
                   (mpfc("6202183680") + mpfc("870941952") * nu + mpfc("50893984") * Power(nu, 2) +
                    mpfc("1584120") * Power(nu, 3) + mpfc("27700") * Power(nu, 4) + mpfc("258") * Power(nu, 5) +
                    Power(nu, 6)) + mpfc("480700") * Power(lambda, 18) *
                                    (mpfc("223278612480") + mpfc("37556093952") * nu +
                                     mpfc("2703125376") * Power(nu, 2) + mpfc("107922304") * Power(nu, 3) +
                                     mpfc("2581320") * Power(nu, 4) + mpfc("36988") * Power(nu, 5) +
                                     mpfc("294") * Power(nu, 6) + Power(nu, 7)) + mpfc("1081575") * Power(lambda, 17) *
                                                                                  (mpfc("7591472824320") +
                                                                                   mpfc("1500185806848") * nu +
                                                                                   mpfc("129462356736") * Power(nu, 2) +
                                                                                   mpfc("6372483712") * Power(nu, 3) +
                                                                                   mpfc("195687184") * Power(nu, 4) +
                                                                                   mpfc("3838912") * Power(nu, 5) +
                                                                                   mpfc("46984") * Power(nu, 6) +
                                                                                   mpfc("328") * Power(nu, 7) +
                                                                                   Power(nu, 8)) +
                   mpfc("2042975") * Power(lambda, 16) *
                   (mpfc("242927130378240") + mpfc("55597418643456") * nu + mpfc("5642981222400") * Power(nu, 2) +
                    mpfc("333381835520") * Power(nu, 3) + mpfc("12634473600") * Power(nu, 4) +
                    mpfc("318532368") * Power(nu, 5) + mpfc("5342400") * Power(nu, 6) + mpfc("57480") * Power(nu, 7) +
                    mpfc("360") * Power(nu, 8) + Power(nu, 9)) + mpfc("3268760") * Power(lambda, 15) *
                                                                 (mpfc("7287813911347200") +
                                                                  mpfc("1910849689681920") * nu +
                                                                  mpfc("224886855315456") * Power(nu, 2) +
                                                                  mpfc("15644436288000") * Power(nu, 3) +
                                                                  mpfc("712416043520") * Power(nu, 4) +
                                                                  mpfc("22190444640") * Power(nu, 5) +
                                                                  mpfc("478804368") * Power(nu, 6) +
                                                                  mpfc("7066800") * Power(nu, 7) +
                                                                  mpfc("68280") * Power(nu, 8) +
                                                                  mpfc("390") * Power(nu, 9) + Power(nu, 10)) +
                   mpfc("4457400") * Power(lambda, 14) * (mpfc("204058789517721600") + mpfc("60791605222440960") * nu +
                                                          mpfc("8207681638514688") * Power(nu, 2) +
                                                          mpfc("662931071379456") * Power(nu, 3) +
                                                          mpfc("35592085506560") * Power(nu, 4) +
                                                          mpfc("1333748493440") * Power(nu, 5) +
                                                          mpfc("35596966944") * Power(nu, 6) +
                                                          mpfc("676674768") * Power(nu, 7) +
                                                          mpfc("8978640") * Power(nu, 8) +
                                                          mpfc("79200") * Power(nu, 9) + mpfc("418") * Power(nu, 10) +
                                                          Power(nu, 11)) + mpfc("5200300") * Power(lambda, 13) *
                                                                           (mpfc("5305528527460761600") +
                                                                            mpfc("1784640525301186560") * nu +
                                                                            mpfc("274191327823822848") * Power(nu, 2) +
                                                                            mpfc("25443889494380544") * Power(nu, 3) +
                                                                            mpfc("1588325294550016") * Power(nu, 4) +
                                                                            mpfc("70269546336000") * Power(nu, 5) +
                                                                            mpfc("2259269633984") * Power(nu, 6) +
                                                                            mpfc("53190510912") * Power(nu, 7) +
                                                                            mpfc("910119408") * Power(nu, 8) +
                                                                            mpfc("11037840") * Power(nu, 9) +
                                                                            mpfc("90068") * Power(nu, 10) +
                                                                            mpfc("444") * Power(nu, 11) +
                                                                            Power(nu, 12)) +
                   mpfc("5200300") * Power(lambda, 12) *
                   (mpfc("127332684659058278400") + mpfc("48136901134689239040") * nu +
                    mpfc("8365232393072934912") * Power(nu, 2) + mpfc("884844675688955904") * Power(nu, 3) +
                    mpfc("63563696563580928") * Power(nu, 4) + mpfc("3274794406614016") * Power(nu, 5) +
                    mpfc("124492017551616") * Power(nu, 6) + mpfc("3535841895872") * Power(nu, 7) +
                    mpfc("75033376704") * Power(nu, 8) + mpfc("1175027568") * Power(nu, 9) +
                    mpfc("13199472") * Power(nu, 10) + mpfc("100724") * Power(nu, 11) + mpfc("468") * Power(nu, 12) +
                    Power(nu, 13)) + mpfc("4457400") * Power(lambda, 11) *
                                     (mpfc("2801319062499282124800") + mpfc("1186344509622221537280") * nu +
                                      mpfc("232172013782293807104") * Power(nu, 2) +
                                      mpfc("27831815258229964800") * Power(nu, 3) +
                                      mpfc("2283246000087736320") * Power(nu, 4) +
                                      mpfc("135609173509089280") * Power(nu, 5) +
                                      mpfc("6013618792749568") * Power(nu, 6) + mpfc("202280539260800") * Power(nu, 7) +
                                      mpfc("5186576183360") * Power(nu, 8) + mpfc("100883983200") * Power(nu, 9) +
                                      mpfc("1465415952") * Power(nu, 10) + mpfc("15415400") * Power(nu, 11) +
                                      mpfc("111020") * Power(nu, 12) + mpfc("490") * Power(nu, 13) + Power(nu, 14)) +
                   mpfc("3268760") * Power(lambda, 10) *
                   (mpfc("56026381249985642496000") + mpfc("26528209254943712870400") * nu +
                    mpfc("5829784785268097679360") * Power(nu, 2) + mpfc("788808318946893103104") * Power(nu, 3) +
                    mpfc("73496735259984691200") * Power(nu, 4) + mpfc("4995429470269521920") * Power(nu, 5) +
                    mpfc("255881549364080640") * Power(nu, 6) + mpfc("10059229577965568") * Power(nu, 7) +
                    mpfc("306012062928000") * Power(nu, 8) + mpfc("7204255847360") * Power(nu, 9) +
                    mpfc("130192302240") * Power(nu, 10) + mpfc("1773723952") * Power(nu, 11) +
                    mpfc("17635800") * Power(nu, 12) + mpfc("120820") * Power(nu, 13) + mpfc("510") * Power(nu, 14) +
                    Power(nu, 15)) + mpfc("2042975") * Power(lambda, 9) *
                                     (mpfc("1008474862499741564928000") + mpfc("533534147838972474163200") * nu +
                                      mpfc("131464335389769471098880") * Power(nu, 2) +
                                      mpfc("20028334526312173535232") * Power(nu, 3) +
                                      mpfc("2111749553626617544704") * Power(nu, 4) +
                                      mpfc("163414465724836085760") * Power(nu, 5) +
                                      mpfc("9601297358822973440") * Power(nu, 6) +
                                      mpfc("436947681767460864") * Power(nu, 7) +
                                      mpfc("15567446710669568") * Power(nu, 8) +
                                      mpfc("435688668180480") * Power(nu, 9) + mpfc("9547717287680") * Power(nu, 10) +
                                      mpfc("162119333376") * Power(nu, 11) + mpfc("2091168352") * Power(nu, 12) +
                                      mpfc("19810560") * Power(nu, 13) + mpfc("130000") * Power(nu, 14) +
                                      mpfc("528") * Power(nu, 15) + Power(nu, 16)) +
                   mpfc("1081575") * Power(lambda, 8) *
                   (mpfc("16135597799995865038848000") + mpfc("9545021227923301151539200") * nu +
                    mpfc("2636963514075284011745280") * Power(nu, 2) + mpfc("451917687810764247662592") * Power(nu, 3) +
                    mpfc("53816327384338054250496") * Power(nu, 4) + mpfc("4726381005223994916864") * Power(nu, 5) +
                    mpfc("317035223466003660800") * Power(nu, 6) + mpfc("16592460267102347264") * Power(nu, 7) +
                    mpfc("686026829138173952") * Power(nu, 8) + mpfc("22538465401557248") * Power(nu, 9) +
                    mpfc("588452144783360") * Power(nu, 10) + mpfc("12141626621696") * Power(nu, 11) +
                    mpfc("195578027008") * Power(nu, 12) + mpfc("2408137312") * Power(nu, 13) +
                    mpfc("21890560") * Power(nu, 14) + mpfc("138448") * Power(nu, 15) + mpfc("544") * Power(nu, 16) +
                    Power(nu, 17)) + mpfc("480700") * Power(lambda, 7) *
                                     (mpfc("225898369199942110543872000") + mpfc("149765894990922081160396800") * nu +
                                      mpfc("46462510424977277315973120") * Power(nu, 2) +
                                      mpfc("8963811143425983479021568") * Power(nu, 3) +
                                      mpfc("1205346271191497007169536") * Power(nu, 4) +
                                      mpfc("119985661457473983086592") * Power(nu, 5) +
                                      mpfc("9164874133748046168064") * Power(nu, 6) +
                                      mpfc("549329667205436522496") * Power(nu, 7) +
                                      mpfc("26196835875036782592") * Power(nu, 8) +
                                      mpfc("1001565344759975424") * Power(nu, 9) +
                                      mpfc("30776795428524288") * Power(nu, 10) +
                                      mpfc("758434917487104") * Power(nu, 11) + mpfc("14879718999808") * Power(nu, 12) +
                                      mpfc("229291949376") * Power(nu, 13) + mpfc("2714605152") * Power(nu, 14) +
                                      mpfc("23828832") * Power(nu, 15) + mpfc("146064") * Power(nu, 16) +
                                      mpfc("558") * Power(nu, 17) + Power(nu, 18)) + mpfc("177100") * Power(lambda, 6) *
                                                                                     (mpfc("2710780430399305326526464000") +
                                                                                      mpfc("2023089109091007084468633600") *
                                                                                      nu +
                                                                                      mpfc("707316020090649408952074240") *
                                                                                      Power(nu, 2) +
                                                                                      mpfc("154028244146089079064231936") *
                                                                                      Power(nu, 3) +
                                                                                      mpfc("23427966397723947565056000") *
                                                                                      Power(nu, 4) +
                                                                                      mpfc("2645174208681184804208640") *
                                                                                      Power(nu, 5) +
                                                                                      mpfc("229964151062450537103360") *
                                                                                      Power(nu, 6) +
                                                                                      mpfc("15756830140213284438016") *
                                                                                      Power(nu, 7) +
                                                                                      mpfc("863691697705877913600") *
                                                                                      Power(nu, 8) +
                                                                                      mpfc("38215620012156487680") *
                                                                                      Power(nu, 9) +
                                                                                      mpfc("1370886889902266880") *
                                                                                      Power(nu, 10) +
                                                                                      mpfc("39878014438369536") *
                                                                                      Power(nu, 11) +
                                                                                      mpfc("936991545484800") *
                                                                                      Power(nu, 12) +
                                                                                      mpfc("17631222392320") *
                                                                                      Power(nu, 13) +
                                                                                      mpfc("261867211200") *
                                                                                      Power(nu, 14) +
                                                                                      mpfc("3000551136") *
                                                                                      Power(nu, 15) +
                                                                                      mpfc("25581600") * Power(nu, 16) +
                                                                                      mpfc("152760") * Power(nu, 17) +
                                                                                      mpfc("570") * Power(nu, 18) +
                                                                                      Power(nu, 19)) +
                   mpfc("53130") * Power(lambda, 5) *
                   (mpfc("27107804303993053265264640000") + mpfc("22941671521309376171212800000") * nu +
                    mpfc("9096249309997501173989376000") * Power(nu, 2) +
                    mpfc("2247598461551540199594393600") * Power(nu, 3) +
                    mpfc("388307908123328554714791936") * Power(nu, 4) +
                    mpfc("49879708484535795607142400") * Power(nu, 5) +
                    mpfc("4944815719305690175242240") * Power(nu, 6) + mpfc("387532452464583381483520") * Power(nu, 7) +
                    mpfc("24393747117272063574016") * Power(nu, 8) + mpfc("1245847897827442790400") * Power(nu, 9) +
                    mpfc("51924488911179156480") * Power(nu, 10) + mpfc("1769667034285962240") * Power(nu, 11) +
                    mpfc("49247929893217536") * Power(nu, 12) + mpfc("1113303769408000") * Power(nu, 13) +
                    mpfc("20249894504320") * Power(nu, 14) + mpfc("291872722560") * Power(nu, 15) +
                    mpfc("3256367136") * Power(nu, 16) + mpfc("27109200") * Power(nu, 17) +
                    mpfc("158460") * Power(nu, 18) + mpfc("580") * Power(nu, 19) + Power(nu, 20)) +
                   mpfc("12650") * Power(lambda, 4) *
                   (mpfc("216862434431944426122117120000") + mpfc("210641176474468062634967040000") * nu +
                    mpfc("95711666001289385563127808000") * Power(nu, 2) +
                    mpfc("27077037002409822770744524800") * Power(nu, 3) +
                    mpfc("5354061726538168637312729088") * Power(nu, 4) +
                    mpfc("787345575999614919571931136") * Power(nu, 5) +
                    mpfc("89438234238981317009080320") * Power(nu, 6) +
                    mpfc("8045075339022357227110400") * Power(nu, 7) + mpfc("582682429402759890075648") * Power(nu, 8) +
                    mpfc("34360530299891605897216") * Power(nu, 9) + mpfc("1661243809116876042240") * Power(nu, 10) +
                    mpfc("66081825185466854400") * Power(nu, 11) + mpfc("2163650473431702528") * Power(nu, 12) +
                    mpfc("58154360048481536") * Power(nu, 13) + mpfc("1275302925442560") * Power(nu, 14) +
                    mpfc("22584876284800") * Power(nu, 15) + mpfc("317923659648") * Power(nu, 16) +
                    mpfc("3473240736") * Power(nu, 17) + mpfc("28376880") * Power(nu, 18) +
                    mpfc("163100") * Power(nu, 19) + mpfc("588") * Power(nu, 20) + Power(nu, 21)) +
                   mpfc("2300") * Power(lambda, 3) *
                   (mpfc("1301174606591666556732702720000") + mpfc("1480709493278752801931919360000") * nu +
                    mpfc("784911172482204376013733888000") * Power(nu, 2) +
                    mpfc("258173888015748322187594956800") * Power(nu, 3) +
                    mpfc("59201407361638834594620899328") * Power(nu, 4) +
                    mpfc("10078135182535858154744315904") * Power(nu, 5) +
                    mpfc("1323974981433502821626413056") * Power(nu, 6) +
                    mpfc("137708686273115460371742720") * Power(nu, 7) +
                    mpfc("11541169915438916567564288") * Power(nu, 8) +
                    mpfc("788845611202109525458944") * Power(nu, 9) + mpfc("44327993154592862150656") * Power(nu, 10) +
                    mpfc("2057734760229677168640") * Power(nu, 11) + mpfc("79063728026057069568") * Power(nu, 12) +
                    mpfc("2512576633722591744") * Power(nu, 13) + mpfc("65806177601136896") * Power(nu, 14) +
                    mpfc("1410812183151360") * Power(nu, 15) + mpfc("24492418242688") * Power(nu, 16) +
                    mpfc("338763104064") * Power(nu, 17) + mpfc("3643502016") * Power(nu, 18) +
                    mpfc("29355480") * Power(nu, 19) + mpfc("166628") * Power(nu, 20) + mpfc("594") * Power(nu, 21) +
                    Power(nu, 22)) + mpfc("300") * Power(lambda, 2) * (mpfc("5204698426366666226930810880000") +
                                                                       mpfc("7224012579706677764460380160000") * nu +
                                                                       mpfc("4620354183207570305986854912000") *
                                                                       Power(nu, 2) +
                                                                       mpfc("1817606724545197664764113715200") *
                                                                       Power(nu, 3) +
                                                                       mpfc("494979517462303660566078554112") *
                                                                       Power(nu, 4) +
                                                                       mpfc("99513948091782267213598162944") *
                                                                       Power(nu, 5) +
                                                                       mpfc("15374035108269869441249968128") *
                                                                       Power(nu, 6) +
                                                                       mpfc("1874809726525964663113383936") *
                                                                       Power(nu, 7) +
                                                                       mpfc("183873365934871126641999872") *
                                                                       Power(nu, 8) +
                                                                       mpfc("14696552360247354669400064") *
                                                                       Power(nu, 9) + mpfc("966157583820480974061568") *
                                                                                      Power(nu, 10) +
                                                                       mpfc("52558932195511570825216") * Power(nu, 11) +
                                                                       mpfc("2373989672333905446912") * Power(nu, 12) +
                                                                       mpfc("89114034560947436544") * Power(nu, 13) +
                                                                       mpfc("2775801344127139328") * Power(nu, 14) +
                                                                       mpfc("71449426333742336") * Power(nu, 15) +
                                                                       mpfc("1508781856122112") * Power(nu, 16) +
                                                                       mpfc("25847470658944") * Power(nu, 17) +
                                                                       mpfc("353337112128") * Power(nu, 18) +
                                                                       mpfc("3760923936") * Power(nu, 19) +
                                                                       mpfc("30021992") * Power(nu, 20) +
                                                                       mpfc("169004") * Power(nu, 21) +
                                                                       mpfc("598") * Power(nu, 22) + Power(nu, 23)) +
                   mpfc("25") * lambda *
                   (mpfc("10409396852733332453861621760000") + mpfc("19652723585780021755851571200000") * nu +
                    mpfc("16464720946121818376434089984000") * Power(nu, 2) +
                    mpfc("8255567632297965635515082342400") * Power(nu, 3) +
                    mpfc("2807565759469804985896270823424") * Power(nu, 4) +
                    mpfc("694007413645868194993274880000") * Power(nu, 5) +
                    mpfc("130262018308322006096098099200") * Power(nu, 6) +
                    mpfc("19123654561321798767476736000") * Power(nu, 7) +
                    mpfc("2242556458395706916397383680") * Power(nu, 8) +
                    mpfc("213266470655365835980800000") * Power(nu, 9) +
                    mpfc("16628867527888316617523200") * Power(nu, 10) +
                    mpfc("1071275448211504115712000") * Power(nu, 11) +
                    mpfc("57306911540179381719040") * Power(nu, 12) + mpfc("2552217741455800320000") * Power(nu, 13) +
                    mpfc("94665637249201715200") * Power(nu, 14) + mpfc("2918700196794624000") * Power(nu, 15) +
                    mpfc("74466990045986560") * Power(nu, 16) + mpfc("1560476797440000") * Power(nu, 17) +
                    mpfc("26554144883200") * Power(nu, 18) + mpfc("360858960000") * Power(nu, 19) +
                    mpfc("3820967920") * Power(nu, 20) + mpfc("30360000") * Power(nu, 21) +
                    mpfc("170200") * Power(nu, 22) + mpfc("600") * Power(nu, 23) + Power(nu, 24)) + nu *
                                                                                                    (mpfc("10409396852733332453861621760000") +
                                                                                                     mpfc("19652723585780021755851571200000") *
                                                                                                     nu +
                                                                                                     mpfc("16464720946121818376434089984000") *
                                                                                                     Power(nu, 2) +
                                                                                                     mpfc("8255567632297965635515082342400") *
                                                                                                     Power(nu, 3) +
                                                                                                     mpfc("2807565759469804985896270823424") *
                                                                                                     Power(nu, 4) +
                                                                                                     mpfc("694007413645868194993274880000") *
                                                                                                     Power(nu, 5) +
                                                                                                     mpfc("130262018308322006096098099200") *
                                                                                                     Power(nu, 6) +
                                                                                                     mpfc("19123654561321798767476736000") *
                                                                                                     Power(nu, 7) +
                                                                                                     mpfc("2242556458395706916397383680") *
                                                                                                     Power(nu, 8) +
                                                                                                     mpfc("213266470655365835980800000") *
                                                                                                     Power(nu, 9) +
                                                                                                     mpfc("16628867527888316617523200") *
                                                                                                     Power(nu, 10) +
                                                                                                     mpfc("1071275448211504115712000") *
                                                                                                     Power(nu, 11) +
                                                                                                     mpfc("57306911540179381719040") *
                                                                                                     Power(nu, 12) +
                                                                                                     mpfc("2552217741455800320000") *
                                                                                                     Power(nu, 13) +
                                                                                                     mpfc("94665637249201715200") *
                                                                                                     Power(nu, 14) +
                                                                                                     mpfc("2918700196794624000") *
                                                                                                     Power(nu, 15) +
                                                                                                     mpfc("74466990045986560") *
                                                                                                     Power(nu, 16) +
                                                                                                     mpfc("1560476797440000") *
                                                                                                     Power(nu, 17) +
                                                                                                     mpfc("26554144883200") *
                                                                                                     Power(nu, 18) +
                                                                                                     mpfc("360858960000") *
                                                                                                     Power(nu, 19) +
                                                                                                     mpfc("3820967920") *
                                                                                                     Power(nu, 20) +
                                                                                                     mpfc("30360000") *
                                                                                                     Power(nu, 21) +
                                                                                                     mpfc("170200") *
                                                                                                     Power(nu, 22) +
                                                                                                     mpfc("600") *
                                                                                                     Power(nu, 23) +
                                                                                                     Power(nu, 24)));

        moment(26, Power(lambda, 26) + mpfc("26") * Power(lambda, 25) * (mpfc("50") + nu) +
                   mpfc("325") * Power(lambda, 24) * (mpfc("2400") + mpfc("98") * nu + Power(nu, 2)) +
                   mpfc("2600") * Power(lambda, 23) *
                   (mpfc("110400") + mpfc("6908") * nu + mpfc("144") * Power(nu, 2) + Power(nu, 3)) +
                   mpfc("14950") * Power(lambda, 22) *
                   (mpfc("4857600") + mpfc("414352") * nu + mpfc("13244") * Power(nu, 2) + mpfc("188") * Power(nu, 3) +
                    Power(nu, 4)) + mpfc("65780") * Power(lambda, 21) *
                                    (mpfc("204019200") + mpfc("22260384") * nu + mpfc("970600") * Power(nu, 2) +
                                     mpfc("21140") * Power(nu, 3) + mpfc("230") * Power(nu, 4) + Power(nu, 5)) +
                   mpfc("230230") * Power(lambda, 20) *
                   (mpfc("8160768000") + mpfc("1094434560") * nu + mpfc("61084384") * Power(nu, 2) +
                    mpfc("1816200") * Power(nu, 3) + mpfc("30340") * Power(nu, 4) + mpfc("270") * Power(nu, 5) +
                    Power(nu, 6)) + mpfc("657800") * Power(lambda, 19) *
                                    (mpfc("310109184000") + mpfc("49749281280") * nu +
                                     mpfc("3415641152") * Power(nu, 2) + mpfc("130099984") * Power(nu, 3) +
                                     mpfc("2969120") * Power(nu, 4) + mpfc("40600") * Power(nu, 5) +
                                     mpfc("308") * Power(nu, 6) + Power(nu, 7)) + mpfc("1562275") * Power(lambda, 18) *
                                                                                  (mpfc("11163930624000") +
                                                                                   mpfc("2101083310080") * nu +
                                                                                   mpfc("172712362752") * Power(nu, 2) +
                                                                                   mpfc("8099240576") * Power(nu, 3) +
                                                                                   mpfc("236988304") * Power(nu, 4) +
                                                                                   mpfc("4430720") * Power(nu, 5) +
                                                                                   mpfc("51688") * Power(nu, 6) +
                                                                                   mpfc("344") * Power(nu, 7) +
                                                                                   Power(nu, 8)) +
                   mpfc("3124550") * Power(lambda, 17) *
                   (mpfc("379573641216000") + mpfc("82600763166720") * nu + mpfc("7973303643648") * Power(nu, 2) +
                    mpfc("448086542336") * Power(nu, 3) + mpfc("16156842912") * Power(nu, 4) +
                    mpfc("387632784") * Power(nu, 5) + mpfc("6188112") * Power(nu, 6) + mpfc("63384") * Power(nu, 7) +
                    mpfc("378") * Power(nu, 8) + Power(nu, 9)) + mpfc("5311735") * Power(lambda, 16) *
                                                                 (mpfc("12146356518912000") +
                                                                  mpfc("3022798062551040") * nu +
                                                                  mpfc("337746479763456") * Power(nu, 2) +
                                                                  mpfc("22312072998400") * Power(nu, 3) +
                                                                  mpfc("965105515520") * Power(nu, 4) +
                                                                  mpfc("28561092000") * Power(nu, 5) +
                                                                  mpfc("585652368") * Power(nu, 6) +
                                                                  mpfc("8216400") * Power(nu, 7) +
                                                                  mpfc("75480") * Power(nu, 8) +
                                                                  mpfc("410") * Power(nu, 9) + Power(nu, 10)) +
                   mpfc("7726160") * Power(lambda, 15) * (mpfc("364390695567360000") + mpfc("102830298395443200") * nu +
                                                          mpfc("13155192455454720") * Power(nu, 2) +
                                                          mpfc("1007108669715456") * Power(nu, 3) +
                                                          mpfc("51265238464000") * Power(nu, 4) +
                                                          mpfc("1821938275520") * Power(nu, 5) +
                                                          mpfc("46130663040") * Power(nu, 6) +
                                                          mpfc("832144368") * Power(nu, 7) +
                                                          mpfc("10480800") * Power(nu, 8) +
                                                          mpfc("87780") * Power(nu, 9) + mpfc("440") * Power(nu, 10) +
                                                          Power(nu, 11)) + mpfc("9657700") * Power(lambda, 14) *
                                                                           (mpfc("10202939475886080000") +
                                                                            mpfc("3243639050639769600") * nu +
                                                                            mpfc("471175687148175360") * Power(nu, 2) +
                                                                            mpfc("41354235207487488") * Power(nu, 3) +
                                                                            mpfc("2442535346707456") * Power(nu, 4) +
                                                                            mpfc("102279510178560") * Power(nu, 5) +
                                                                            mpfc("3113596840640") * Power(nu, 6) +
                                                                            mpfc("69430705344") * Power(nu, 7) +
                                                                            mpfc("1125606768") * Power(nu, 8) +
                                                                            mpfc("12938640") * Power(nu, 9) +
                                                                            mpfc("100100") * Power(nu, 10) +
                                                                            mpfc("468") * Power(nu, 11) +
                                                                            Power(nu, 12)) +
                   mpfc("10400600") * Power(lambda, 13) *
                   (mpfc("265276426373038080000") + mpfc("94537554792520089600") * nu +
                    mpfc("15494206916492328960") * Power(nu, 2) + mpfc("1546385802542850048") * Power(nu, 3) +
                    mpfc("104860154221881344") * Power(nu, 4) + mpfc("5101802611350016") * Power(nu, 5) +
                    mpfc("183233028035200") * Power(nu, 6) + mpfc("4918795179584") * Power(nu, 7) +
                    mpfc("98696481312") * Power(nu, 8) + mpfc("1462011408") * Power(nu, 9) +
                    mpfc("15541240") * Power(nu, 10) + mpfc("112268") * Power(nu, 11) + mpfc("494") * Power(nu, 12) +
                    Power(nu, 13)) + mpfc("9657700") * Power(lambda, 12) *
                                     (mpfc("6366634232952913920000") + mpfc("2534177741393520230400") * nu +
                                      mpfc("466398520788335984640") * Power(nu, 2) +
                                      mpfc("52607466177520730112") * Power(nu, 3) +
                                      mpfc("4063029503868002304") * Power(nu, 4) +
                                      mpfc("227303416894281728") * Power(nu, 5) +
                                      mpfc("9499395284194816") * Power(nu, 6) + mpfc("301284112345216") * Power(nu, 7) +
                                      mpfc("7287510731072") * Power(nu, 8) + mpfc("133784755104") * Power(nu, 9) +
                                      mpfc("1835001168") * Power(nu, 10) + mpfc("18235672") * Power(nu, 11) +
                                      mpfc("124124") * Power(nu, 12) + mpfc("518") * Power(nu, 13) + Power(nu, 14)) +
                   mpfc("7726160") * Power(lambda, 11) *
                   (mpfc("140065953124964106240000") + mpfc("62118544543610358988800") * nu +
                    mpfc("12794945198736911892480") * Power(nu, 2) + mpfc("1623762776693792047104") * Power(nu, 3) +
                    mpfc("141994115262616780800") * Power(nu, 4) + mpfc("9063704675542200320") * Power(nu, 5) +
                    mpfc("436290113146567680") * Power(nu, 6) + mpfc("16127645755789568") * Power(nu, 7) +
                    mpfc("461609348428800") * Power(nu, 8) + mpfc("10230775343360") * Power(nu, 9) +
                    mpfc("174154780800") * Power(nu, 10) + mpfc("2236185952") * Power(nu, 11) +
                    mpfc("20966400") * Power(nu, 12) + mpfc("135520") * Power(nu, 13) + mpfc("540") * Power(nu, 14) +
                    Power(nu, 15)) + mpfc("5311735") * Power(lambda, 10) *
                                     (mpfc("2801319062499282124800000") + mpfc("1382436843997171286016000") * nu +
                                      mpfc("318017448518348596838400") * Power(nu, 2) +
                                      mpfc("45270200732612752834560") * Power(nu, 3) +
                                      mpfc("4463645081946127663104") * Power(nu, 4) +
                                      mpfc("323268208773460787200") * Power(nu, 5) +
                                      mpfc("17789506938473553920") * Power(nu, 6) +
                                      mpfc("758843028262359040") * Power(nu, 7) +
                                      mpfc("25359832724365568") * Power(nu, 8) +
                                      mpfc("666224855296000") * Power(nu, 9) + mpfc("13713870959360") * Power(nu, 10) +
                                      mpfc("218878499840") * Power(nu, 11) + mpfc("2655513952") * Power(nu, 12) +
                                      mpfc("23676800") * Power(nu, 13) + mpfc("146320") * Power(nu, 14) +
                                      mpfc("560") * Power(nu, 15) + Power(nu, 16)) +
                   mpfc("3124550") * Power(lambda, 9) *
                   (mpfc("50423743124987078246400000") + mpfc("27685182254448365273088000") * nu +
                    mpfc("7106750917327446029107200") * Power(nu, 2) +
                    mpfc("1132881061705378147860480") * Power(nu, 3) + mpfc("125615812207643050770432") * Power(nu, 4) +
                    mpfc("10282472839868421832704") * Power(nu, 5) + mpfc("643479333665984757760") * Power(nu, 6) +
                    mpfc("31448681447196016640") * Power(nu, 7) + mpfc("1215320017300939264") * Power(nu, 8) +
                    mpfc("37351880119693568") * Power(nu, 9) + mpfc("913074532564480") * Power(nu, 10) +
                    mpfc("17653683956480") * Power(nu, 11) + mpfc("266677750976") * Power(nu, 12) +
                    mpfc("3081696352") * Power(nu, 13) + mpfc("26310560") * Power(nu, 14) +
                    mpfc("156400") * Power(nu, 15) + mpfc("578") * Power(nu, 16) + Power(nu, 17)) +
                   mpfc("1562275") * Power(lambda, 8) *
                   (mpfc("806779889999793251942400000") + mpfc("493386659196160922615808000") * nu +
                    mpfc("141393196931687501738803200") * Power(nu, 2) +
                    mpfc("25232847904613496394874880") * Power(nu, 3) +
                    mpfc("3142734057027666960187392") * Power(nu, 4) + mpfc("290135377645537800093696") * Power(nu, 5) +
                    mpfc("20578142178524177956864") * Power(nu, 6) + mpfc("1146658236821121024000") * Power(nu, 7) +
                    mpfc("50893801724011044864") * Power(nu, 8) + mpfc("1812950099216036352") * Power(nu, 9) +
                    mpfc("51961072640725248") * Power(nu, 10) + mpfc("1195533475868160") * Power(nu, 11) +
                    mpfc("21920527972096") * Power(nu, 12) + mpfc("315984892608") * Power(nu, 13) +
                    mpfc("3502665312") * Power(nu, 14) + mpfc("28812960") * Power(nu, 15) +
                    mpfc("165648") * Power(nu, 16) + mpfc("594") * Power(nu, 17) + Power(nu, 18)) +
                   mpfc("657800") * Power(lambda, 7) *
                   (mpfc("11294918459997105527193600000") + mpfc("7714193118746046168563712000") * nu +
                    mpfc("2472891416239785946959052800") * Power(nu, 2) +
                    mpfc("494653067596276451267051520") * Power(nu, 3) +
                    mpfc("69231124703000833837498368") * Power(nu, 4) +
                    mpfc("7204629344065196161499136") * Power(nu, 5) + mpfc("578229368144876291489792") * Power(nu, 6) +
                    mpfc("36631357494019872292864") * Power(nu, 7) + mpfc("1859171460957275652096") * Power(nu, 8) +
                    mpfc("76275103113035553792") * Power(nu, 9) + mpfc("2540405116186189824") * Power(nu, 10) +
                    mpfc("68698541302879488") * Power(nu, 11) + mpfc("1502420867477504") * Power(nu, 12) +
                    mpfc("26344316468608") * Power(nu, 13) + mpfc("365022206976") * Power(nu, 14) +
                    mpfc("3906046752") * Power(nu, 15) + mpfc("31132032") * Power(nu, 16) +
                    mpfc("173964") * Power(nu, 17) + mpfc("608") * Power(nu, 18) + Power(nu, 19)) +
                   mpfc("230230") * Power(lambda, 6) *
                   (mpfc("135539021519965266326323200000") + mpfc("103865235884949659549958144000") * nu +
                    mpfc("37388890113623477532072345600") * Power(nu, 2) +
                    mpfc("8408728227395103362163671040") * Power(nu, 3) +
                    mpfc("1325426564032286457317031936") * Power(nu, 4) +
                    mpfc("155686676831783187775488000") * Power(nu, 5) +
                    mpfc("14143381761803711659376640") * Power(nu, 6) +
                    mpfc("1017805658073114759004160") * Power(nu, 7) + mpfc("58941415025507180118016") * Power(nu, 8) +
                    mpfc("2774472698313702297600") * Power(nu, 9) + mpfc("106759964507269831680") * Power(nu, 10) +
                    mpfc("3364787611820743680") * Power(nu, 11) + mpfc("86727591712609536") * Power(nu, 12) +
                    mpfc("1818552665100800") * Power(nu, 13) + mpfc("30724582952320") * Power(nu, 14) +
                    mpfc("411894768000") * Power(nu, 15) + mpfc("4279631136") * Power(nu, 16) +
                    mpfc("33219600") * Power(nu, 17) + mpfc("181260") * Power(nu, 18) + mpfc("620") * Power(nu, 19) +
                    Power(nu, 20)) + mpfc("65780") * Power(lambda, 5) * (mpfc("1355390215199652663263232000000") +
                                                                         mpfc("1174191380369461861825904640000") * nu +
                                                                         mpfc("477754137021184434870681600000") *
                                                                         Power(nu, 2) +
                                                                         mpfc("121476172387574511153709056000") *
                                                                         Power(nu, 3) +
                                                                         mpfc("21662993867717967935333990400") *
                                                                         Power(nu, 4) +
                                                                         mpfc("2882293332350118335071911936") *
                                                                         Power(nu, 5) +
                                                                         mpfc("297120494449820304369254400") *
                                                                         Power(nu, 6) +
                                                                         mpfc("24321438342534859249418240") *
                                                                         Power(nu, 7) +
                                                                         mpfc("1607219808328186560184320") *
                                                                         Power(nu, 8) +
                                                                         mpfc("86686142008644203094016") *
                                                                         Power(nu, 9) + mpfc("3842072343386400614400") *
                                                                                        Power(nu, 10) +
                                                                         mpfc("140407840625477268480") * Power(nu, 11) +
                                                                         mpfc("4232063528946839040") * Power(nu, 12) +
                                                                         mpfc("104913118363617536") * Power(nu, 13) +
                                                                         mpfc("2125798494624000") * Power(nu, 14) +
                                                                         mpfc("34843530632320") * Power(nu, 15) +
                                                                         mpfc("454691079360") * Power(nu, 16) +
                                                                         mpfc("4611827136") * Power(nu, 17) +
                                                                         mpfc("35032200") * Power(nu, 18) +
                                                                         mpfc("187460") * Power(nu, 19) +
                                                                         mpfc("630") * Power(nu, 20) + Power(nu, 21)) +
                   mpfc("14950") * Power(lambda, 4) *
                   (mpfc("10843121721597221306105856000000") + mpfc("10748921258155347557870469120000") * nu +
                    mpfc("4996224476538937340791357440000") * Power(nu, 2) +
                    mpfc("1449563516121780524100354048000") * Power(nu, 3) +
                    mpfc("294780123329318254636380979200") * Power(nu, 4) +
                    mpfc("44721340526518914615909285888") * Power(nu, 5) +
                    mpfc("5259257287948680770025947136") * Power(nu, 6) +
                    mpfc("491692001190099178364600320") * Power(nu, 7) +
                    mpfc("37179196809160351730892800") * Power(nu, 8) +
                    mpfc("2300708944397340184936448") * Power(nu, 9) +
                    mpfc("117422720755735408009216") * Power(nu, 10) + mpfc("4965335068390218762240") * Power(nu, 11) +
                    mpfc("174264348857051980800") * Power(nu, 12) + mpfc("5071368475855779328") * Power(nu, 13) +
                    mpfc("121919506320609536") * Power(nu, 14) + mpfc("2404546739682560") * Power(nu, 15) +
                    mpfc("38481059267200") * Power(nu, 16) + mpfc("491585696448") * Power(nu, 17) +
                    mpfc("4892084736") * Power(nu, 18) + mpfc("36531880") * Power(nu, 19) +
                    mpfc("192500") * Power(nu, 20) + mpfc("638") * Power(nu, 21) + Power(nu, 22)) +
                   mpfc("2600") * Power(lambda, 3) *
                   (mpfc("65058730329583327836635136000000") + mpfc("75336649270529306653328670720000") * nu +
                    mpfc("40726268117388971602618613760000") * Power(nu, 2) +
                    mpfc("13693605573269620485393481728000") * Power(nu, 3) +
                    mpfc("3218244256097690051918639923200") * Power(nu, 4) +
                    mpfc("563108166488431742331836694528") * Power(nu, 5) +
                    mpfc("76276884254210999236064968704") * Power(nu, 6) +
                    mpfc("8209409295089275840213549056") * Power(nu, 7) +
                    mpfc("714767182045061288749957120") * Power(nu, 8) +
                    mpfc("50983450475544392840511488") * Power(nu, 9) +
                    mpfc("3005245268931752632991744") * Power(nu, 10) +
                    mpfc("147214731166076720582656") * Power(nu, 11) + mpfc("6010921161532530647040") * Power(nu, 12) +
                    mpfc("204692559712186656768") * Power(nu, 13) + mpfc("5802885513779436544") * Power(nu, 14) +
                    mpfc("136346786758704896") * Power(nu, 15) + mpfc("2635433095285760") * Power(nu, 16) +
                    mpfc("41430573445888") * Power(nu, 17) + mpfc("520938204864") * Power(nu, 18) +
                    mpfc("5111276016") * Power(nu, 19) + mpfc("37686880") * Power(nu, 20) +
                    mpfc("196328") * Power(nu, 21) + mpfc("644") * Power(nu, 22) + Power(nu, 23)) +
                   mpfc("325") * Power(lambda, 2) *
                   (mpfc("260234921318333311346540544000000") + mpfc("366405327411700554449949818880000") * nu +
                    mpfc("238241721740085193063803125760000") * Power(nu, 2) +
                    mpfc("95500690410467453544192540672000") * Power(nu, 3) +
                    mpfc("26566582597660380693068041420800") * Power(nu, 4) +
                    mpfc("5470676922051417021245986701312") * Power(nu, 5) +
                    mpfc("868215703505275739276096569344") * Power(nu, 6) +
                    mpfc("109114521434568102596919164928") * Power(nu, 7) +
                    mpfc("11068478023269520995213377536") * Power(nu, 8) +
                    mpfc("918700983947238860112003072") * Power(nu, 9) +
                    mpfc("63004431551271403372478464") * Power(nu, 10) +
                    mpfc("3594104193596059515322368") * Power(nu, 11) +
                    mpfc("171258415812206843170816") * Power(nu, 12) + mpfc("6829691400381277274112") * Power(nu, 13) +
                    mpfc("227904101767304402944") * Power(nu, 14) + mpfc("6348272660814256128") * Power(nu, 15) +
                    mpfc("146888519139847936") * Power(nu, 16) + mpfc("2801155389069312") * Power(nu, 17) +
                    mpfc("43514326265344") * Power(nu, 18) + mpfc("541383308928") * Power(nu, 19) +
                    mpfc("5262023536") * Power(nu, 20) + mpfc("38472192") * Power(nu, 21) +
                    mpfc("198904") * Power(nu, 22) + mpfc("648") * Power(nu, 23) + Power(nu, 24)) +
                   mpfc("26") * lambda *
                   (mpfc("520469842636666622693081088000000") + mpfc("993045576141734420246440181760000") * nu +
                    mpfc("842888770891870940577556070400000") * Power(nu, 2) +
                    mpfc("429243102561020100152188207104000") * Power(nu, 3) +
                    mpfc("148633855605788214930328623513600") * Power(nu, 4) +
                    mpfc("37507936441763214735560014823424") * Power(nu, 5) +
                    mpfc("7207108329061968499798179840000") * Power(nu, 6) +
                    mpfc("1086444746374411944469934899200") * Power(nu, 7) +
                    mpfc("131251477481107144587345920000") * Power(nu, 8) +
                    mpfc("12905879991163998715437383680") * Power(nu, 9) +
                    mpfc("1044709847049781666856960000") * Power(nu, 10) +
                    mpfc("70192639938463522403123200") * Power(nu, 11) +
                    mpfc("3936621025220473201664000") * Power(nu, 12) +
                    mpfc("184917798612969397719040") * Power(nu, 13) + mpfc("7285499603915886080000") * Power(nu, 14) +
                    mpfc("240600647088932915200") * Power(nu, 15) + mpfc("6642049699093952000") * Power(nu, 16) +
                    mpfc("152490829917986560") * Power(nu, 17) + mpfc("2888184041600000") * Power(nu, 18) +
                    mpfc("44597092883200") * Power(nu, 19) + mpfc("551907356000") * Power(nu, 20) +
                    mpfc("5338967920") * Power(nu, 21) + mpfc("38870000") * Power(nu, 22) +
                    mpfc("200200") * Power(nu, 23) + mpfc("650") * Power(nu, 24) + Power(nu, 25)) + nu *
                                                                                                    (mpfc("520469842636666622693081088000000") +
                                                                                                     mpfc("993045576141734420246440181760000") *
                                                                                                     nu +
                                                                                                     mpfc("842888770891870940577556070400000") *
                                                                                                     Power(nu, 2) +
                                                                                                     mpfc("429243102561020100152188207104000") *
                                                                                                     Power(nu, 3) +
                                                                                                     mpfc("148633855605788214930328623513600") *
                                                                                                     Power(nu, 4) +
                                                                                                     mpfc("37507936441763214735560014823424") *
                                                                                                     Power(nu, 5) +
                                                                                                     mpfc("7207108329061968499798179840000") *
                                                                                                     Power(nu, 6) +
                                                                                                     mpfc("1086444746374411944469934899200") *
                                                                                                     Power(nu, 7) +
                                                                                                     mpfc("131251477481107144587345920000") *
                                                                                                     Power(nu, 8) +
                                                                                                     mpfc("12905879991163998715437383680") *
                                                                                                     Power(nu, 9) +
                                                                                                     mpfc("1044709847049781666856960000") *
                                                                                                     Power(nu, 10) +
                                                                                                     mpfc("70192639938463522403123200") *
                                                                                                     Power(nu, 11) +
                                                                                                     mpfc("3936621025220473201664000") *
                                                                                                     Power(nu, 12) +
                                                                                                     mpfc("184917798612969397719040") *
                                                                                                     Power(nu, 13) +
                                                                                                     mpfc("7285499603915886080000") *
                                                                                                     Power(nu, 14) +
                                                                                                     mpfc("240600647088932915200") *
                                                                                                     Power(nu, 15) +
                                                                                                     mpfc("6642049699093952000") *
                                                                                                     Power(nu, 16) +
                                                                                                     mpfc("152490829917986560") *
                                                                                                     Power(nu, 17) +
                                                                                                     mpfc("2888184041600000") *
                                                                                                     Power(nu, 18) +
                                                                                                     mpfc("44597092883200") *
                                                                                                     Power(nu, 19) +
                                                                                                     mpfc("551907356000") *
                                                                                                     Power(nu, 20) +
                                                                                                     mpfc("5338967920") *
                                                                                                     Power(nu, 21) +
                                                                                                     mpfc("38870000") *
                                                                                                     Power(nu, 22) +
                                                                                                     mpfc("200200") *
                                                                                                     Power(nu, 23) +
                                                                                                     mpfc("650") *
                                                                                                     Power(nu, 24) +
                                                                                                     Power(nu, 25)));

        moment(27, Power(lambda, 27) + mpfc("27") * Power(lambda, 26) * (mpfc("52") + nu) +
                   mpfc("351") * Power(lambda, 25) * (mpfc("2600") + mpfc("102") * nu + Power(nu, 2)) +
                   mpfc("2925") * Power(lambda, 24) *
                   (mpfc("124800") + mpfc("7496") * nu + mpfc("150") * Power(nu, 2) + Power(nu, 3)) +
                   mpfc("17550") * Power(lambda, 23) *
                   (mpfc("5740800") + mpfc("469616") * nu + mpfc("14396") * Power(nu, 2) + mpfc("196") * Power(nu, 3) +
                    Power(nu, 4)) + mpfc("80730") * Power(lambda, 22) *
                                    (mpfc("252595200") + mpfc("26403904") * nu + mpfc("1103040") * Power(nu, 2) +
                                     mpfc("23020") * Power(nu, 3) + mpfc("240") * Power(nu, 4) + Power(nu, 5)) +
                   mpfc("296010") * Power(lambda, 21) *
                   (mpfc("10608998400") + mpfc("1361559168") * nu + mpfc("72731584") * Power(nu, 2) +
                    mpfc("2069880") * Power(nu, 3) + mpfc("33100") * Power(nu, 4) + mpfc("282") * Power(nu, 5) +
                    Power(nu, 6)) + mpfc("888030") * Power(lambda, 20) *
                                    (mpfc("424359936000") + mpfc("65071365120") * nu +
                                     mpfc("4270822528") * Power(nu, 2) + mpfc("155526784") * Power(nu, 3) +
                                     mpfc("3393880") * Power(nu, 4) + mpfc("44380") * Power(nu, 5) +
                                     mpfc("322") * Power(nu, 6) + Power(nu, 7)) + mpfc("2220075") * Power(lambda, 19) *
                                                                                  (mpfc("16125677568000") +
                                                                                   mpfc("2897071810560") * nu +
                                                                                   mpfc("227362621184") * Power(nu, 2) +
                                                                                   mpfc("10180840320") * Power(nu, 3) +
                                                                                   mpfc("284494224") * Power(nu, 4) +
                                                                                   mpfc("5080320") * Power(nu, 5) +
                                                                                   mpfc("56616") * Power(nu, 6) +
                                                                                   mpfc("360") * Power(nu, 7) +
                                                                                   Power(nu, 8)) +
                   mpfc("4686825") * Power(lambda, 18) *
                   (mpfc("580524392448000") + mpfc("120420262748160") * nu + mpfc("11082126173184") * Power(nu, 2) +
                    mpfc("593872872704") * Power(nu, 3) + mpfc("20422632384") * Power(nu, 4) +
                    mpfc("467385744") * Power(nu, 5) + mpfc("7118496") * Power(nu, 6) + mpfc("69576") * Power(nu, 7) +
                    mpfc("396") * Power(nu, 8) + Power(nu, 9)) + mpfc("8436285") * Power(lambda, 17) *
                                                                 (mpfc("19737829343232000") +
                                                                  mpfc("4674813325885440") * nu +
                                                                  mpfc("497212552636416") * Power(nu, 2) +
                                                                  mpfc("31273803845120") * Power(nu, 3) +
                                                                  mpfc("1288242373760") * Power(nu, 4) +
                                                                  mpfc("36313747680") * Power(nu, 5) +
                                                                  mpfc("709414608") * Power(nu, 6) +
                                                                  mpfc("9484080") * Power(nu, 7) +
                                                                  mpfc("83040") * Power(nu, 8) +
                                                                  mpfc("430") * Power(nu, 9) + Power(nu, 10)) +
                   mpfc("13037895") * Power(lambda, 16) *
                   (mpfc("631610538983424000") + mpfc("169331855771566080") * nu +
                    mpfc("20585615010250752") * Power(nu, 2) + mpfc("1497974275680256") * Power(nu, 3) +
                    mpfc("72497559805440") * Power(nu, 4) + mpfc("2450282299520") * Power(nu, 5) +
                    mpfc("59015015136") * Power(nu, 6) + mpfc("1012905168") * Power(nu, 7) +
                    mpfc("12141360") * Power(nu, 8) + mpfc("96800") * Power(nu, 9) + mpfc("462") * Power(nu, 10) +
                    Power(nu, 11)) + mpfc("17383860") * Power(lambda, 15) *
                                     (mpfc("18948316169502720000") + mpfc("5711566212130406400") * nu +
                                      mpfc("786900306079088640") * Power(nu, 2) +
                                      mpfc("65524843280658432") * Power(nu, 3) +
                                      mpfc("3672901069843456") * Power(nu, 4) + mpfc("146006028791040") * Power(nu, 5) +
                                      mpfc("4220732753600") * Power(nu, 6) + mpfc("89402170176") * Power(nu, 7) +
                                      mpfc("1377145968") * Power(nu, 8) + mpfc("15045360") * Power(nu, 9) +
                                      mpfc("110660") * Power(nu, 10) + mpfc("492") * Power(nu, 11) + Power(nu, 12)) +
                   mpfc("20058300") * Power(lambda, 14) *
                   (mpfc("530552852746076160000") + mpfc("178872170109154099200") * nu +
                    mpfc("27744774782344888320") * Power(nu, 2) + mpfc("2621595917937524736") * Power(nu, 3) +
                    mpfc("168366073236275200") * Power(nu, 4) + mpfc("7761069875992576") * Power(nu, 5) +
                    mpfc("264186545891840") * Power(nu, 6) + mpfc("6723993518528") * Power(nu, 7) +
                    mpfc("127962257280") * Power(nu, 8) + mpfc("1798416048") * Power(nu, 9) +
                    mpfc("18143840") * Power(nu, 10) + mpfc("124436") * Power(nu, 11) + mpfc("520") * Power(nu, 12) +
                    Power(nu, 13)) + mpfc("20058300") * Power(lambda, 13) *
                                     (mpfc("13794374171397980160000") + mpfc("5181229275584082739200") * nu +
                                      mpfc("900236314450121195520") * Power(nu, 2) +
                                      mpfc("95906268648720531456") * Power(nu, 3) +
                                      mpfc("6999113822080679936") * Power(nu, 4) +
                                      mpfc("370153890012082176") * Power(nu, 5) +
                                      mpfc("14629920069180416") * Power(nu, 6) +
                                      mpfc("439010377373568") * Power(nu, 7) + mpfc("10051012207808") * Power(nu, 8) +
                                      mpfc("174721074528") * Power(nu, 9) + mpfc("2270155888") * Power(nu, 10) +
                                      mpfc("21379176") * Power(nu, 11) + mpfc("137956") * Power(nu, 12) +
                                      mpfc("546") * Power(nu, 13) + Power(nu, 14)) +
                   mpfc("17383860") * Power(lambda, 12) *
                   (mpfc("331064980113551523840000") + mpfc("138143876785415965900800") * nu +
                    mpfc("26786900822386991431680") * Power(nu, 2) + mpfc("3201986762019413950464") * Power(nu, 3) +
                    mpfc("263885000378656849920") * Power(nu, 4) + mpfc("15882807182370652160") * Power(nu, 5) +
                    mpfc("721271971672412160") * Power(nu, 6) + mpfc("25166169126146048") * Power(nu, 7) +
                    mpfc("680234670360960") * Power(nu, 8) + mpfc("14244317996480") * Power(nu, 9) +
                    mpfc("229204815840") * Power(nu, 10) + mpfc("2783256112") * Power(nu, 11) +
                    mpfc("24690120") * Power(nu, 12) + mpfc("151060") * Power(nu, 13) + mpfc("570") * Power(nu, 14) +
                    Power(nu, 15)) + mpfc("13037895") * Power(lambda, 11) *
                                     (mpfc("7283429562498133524480000") + mpfc("3370230269392702773657600") * nu +
                                      mpfc("727455694877929777397760") * Power(nu, 2) +
                                      mpfc("97230609586814098341888") * Power(nu, 3) +
                                      mpfc("9007456770349864648704") * Power(nu, 4) +
                                      mpfc("613306758390811197440") * Power(nu, 5) +
                                      mpfc("31750790559163719680") * Power(nu, 6) +
                                      mpfc("1274927692447625216") * Power(nu, 7) +
                                      mpfc("40131331874087168") * Power(nu, 8) +
                                      mpfc("993609666283520") * Power(nu, 9) + mpfc("19286823944960") * Power(nu, 10) +
                                      mpfc("290436450304") * Power(nu, 11) + mpfc("3326438752") * Power(nu, 12) +
                                      mpfc("28013440") * Power(nu, 13) + mpfc("163600") * Power(nu, 14) +
                                      mpfc("592") * Power(nu, 15) + Power(nu, 16)) +
                   mpfc("8436285") * Power(lambda, 10) *
                   (mpfc("145668591249962670489600000") + mpfc("74688034950352188997632000") * nu +
                    mpfc("17919344166951298321612800") * Power(nu, 2) +
                    mpfc("2672067886614211744235520") * Power(nu, 3) + mpfc("277379744993811391315968") * Power(nu, 4) +
                    mpfc("21273591938166088597504") * Power(nu, 5) + mpfc("1248322569574085591040") * Power(nu, 6) +
                    mpfc("57249344408116224000") * Power(nu, 7) + mpfc("2077554329929368576") * Power(nu, 8) +
                    mpfc("60003525199757568") * Power(nu, 9) + mpfc("1379346145182720") * Power(nu, 10) +
                    mpfc("25095552951040") * Power(nu, 11) + mpfc("356965225344") * Power(nu, 12) +
                    mpfc("3886707552") * Power(nu, 13) + mpfc("31285440") * Power(nu, 14) +
                    mpfc("175440") * Power(nu, 15) + mpfc("612") * Power(nu, 16) + Power(nu, 17)) +
                   mpfc("4686825") * Power(lambda, 9) *
                   (mpfc("2622034642499328068812800000") + mpfc("1490053220356302072446976000") * nu +
                    mpfc("397236229955475558786662400") * Power(nu, 2) +
                    mpfc("66016566126007109717852160") * Power(nu, 3) +
                    mpfc("7664903296502816787922944") * Power(nu, 4) + mpfc("660304399880800986071040") * Power(nu, 5) +
                    mpfc("43743398190499629236224") * Power(nu, 6) + mpfc("2278810768920177623040") * Power(nu, 7) +
                    mpfc("94645322346844858368") * Power(nu, 8) + mpfc("3157617783525004800") * Power(nu, 9) +
                    mpfc("84831755813046528") * Power(nu, 10) + mpfc("1831066098301440") * Power(nu, 11) +
                    mpfc("31520927007232") * Power(nu, 12) + mpfc("426925961280") * Power(nu, 13) +
                    mpfc("4449845472") * Power(nu, 14) + mpfc("34443360") * Power(nu, 15) +
                    mpfc("186456") * Power(nu, 16) + mpfc("630") * Power(nu, 17) + Power(nu, 18)) +
                   mpfc("2220075") * Power(lambda, 8) *
                   (mpfc("41952554279989249101004800000") + mpfc("26462886168200161227964416000") * nu +
                    mpfc("7845832899643911013033574400") * Power(nu, 2) +
                    mpfc("1453501287971589314272296960") * Power(nu, 3) +
                    mpfc("188655018870052178324619264") * Power(nu, 4) +
                    mpfc("18229773694595632565059584") * Power(nu, 5) +
                    mpfc("1360198770928795053850624") * Power(nu, 6) + mpfc("80204370493222471204864") * Power(nu, 7) +
                    mpfc("3793135926469695356928") * Power(nu, 8) + mpfc("145167206883244935168") * Power(nu, 9) +
                    mpfc("4514925876533749248") * Power(nu, 10) + mpfc("114128813385869568") * Power(nu, 11) +
                    mpfc("2335400930417152") * Power(nu, 12) + mpfc("38351742387712") * Power(nu, 13) +
                    mpfc("498123488832") * Power(nu, 14) + mpfc("5000939232") * Power(nu, 15) +
                    mpfc("37426656") * Power(nu, 16) + mpfc("196536") * Power(nu, 17) + mpfc("646") * Power(nu, 18) +
                    Power(nu, 19)) + mpfc("888030") * Power(lambda, 7) * (mpfc("587335759919849487414067200000") +
                                                                          mpfc("412432960634791506292506624000") * nu +
                                                                          mpfc("136304546763214915410434457600") *
                                                                          Power(nu, 2) +
                                                                          mpfc("28194850931246161412845731840") *
                                                                          Power(nu, 3) +
                                                                          mpfc("4094671552152319810816966656") *
                                                                          Power(nu, 4) +
                                                                          mpfc("443871850594391034235453440") *
                                                                          Power(nu, 5) +
                                                                          mpfc("37272556487598763318968320") *
                                                                          Power(nu, 6) +
                                                                          mpfc("2483059957833909650718720") *
                                                                          Power(nu, 7) +
                                                                          mpfc("133308273463798206201856") *
                                                                          Power(nu, 8) +
                                                                          mpfc("5825476822835124449280") *
                                                                          Power(nu, 9) + mpfc("208376169154717424640") *
                                                                                         Power(nu, 10) +
                                                                          mpfc("6112729263935923200") * Power(nu, 11) +
                                                                          mpfc("146824426411709696") * Power(nu, 12) +
                                                                          mpfc("2872325323845120") * Power(nu, 13) +
                                                                          mpfc("45325471231360") * Power(nu, 14) +
                                                                          mpfc("568136638080") * Power(nu, 15) +
                                                                          mpfc("5524912416") * Power(nu, 16) +
                                                                          mpfc("40178160") * Power(nu, 17) +
                                                                          mpfc("205580") * Power(nu, 18) +
                                                                          mpfc("660") * Power(nu, 19) + Power(nu, 20)) +
                   mpfc("296010") * Power(lambda, 6) *
                   (mpfc("7048029119038193848968806400000") + mpfc("5536531287537347562924146688000") * nu +
                    mpfc("2048087521793370491217720115200") * Power(nu, 2) +
                    mpfc("474642757938168852364583239680") * Power(nu, 3) +
                    mpfc("77330909557073999142649331712") * Power(nu, 4) +
                    mpfc("9421133759285012221642407936") * Power(nu, 5) +
                    mpfc("891142528445576194063073280") * Power(nu, 6) +
                    mpfc("67069275981605679127592960") * Power(nu, 7) +
                    mpfc("4082759239399488125140992") * Power(nu, 8) + mpfc("203213995337819699593216") * Power(nu, 9) +
                    mpfc("8325990852691733544960") * Power(nu, 10) + mpfc("281728920321948503040") * Power(nu, 11) +
                    mpfc("7874622380876439552") * Power(nu, 12) + mpfc("181292330297851136") * Power(nu, 13) +
                    mpfc("3416230978621440") * Power(nu, 14) + mpfc("52143110888320") * Power(nu, 15) +
                    mpfc("634435587072") * Power(nu, 16) + mpfc("6007050336") * Power(nu, 17) +
                    mpfc("42645120") * Power(nu, 18) + mpfc("213500") * Power(nu, 19) + mpfc("672") * Power(nu, 20) +
                    Power(nu, 21)) + mpfc("80730") * Power(lambda, 5) * (mpfc("70480291190381938489688064000000") +
                                                                         mpfc("62413341994411669478210273280000") * nu +
                                                                         mpfc("26017406505471052475101347840000") *
                                                                         Power(nu, 2) +
                                                                         mpfc("6794515101175059014863552512000") *
                                                                         Power(nu, 3) +
                                                                         mpfc("1247951853508908843791076556800") *
                                                                         Power(nu, 4) +
                                                                         mpfc("171542247149924121359073411072") *
                                                                         Power(nu, 5) +
                                                                         mpfc("18332559043740774162273140736") *
                                                                         Power(nu, 6) +
                                                                         mpfc("1561835288261632985339002880") *
                                                                         Power(nu, 7) +
                                                                         mpfc("107896868375600560379002880") *
                                                                         Power(nu, 8) +
                                                                         mpfc("6114899192777685121073152") *
                                                                         Power(nu, 9) +
                                                                         mpfc("286473903864737035042816") *
                                                                         Power(nu, 10) +
                                                                         mpfc("11143280055911218575360") *
                                                                         Power(nu, 11) +
                                                                         mpfc("360475144130712898560") * Power(nu, 12) +
                                                                         mpfc("9687545683854950912") * Power(nu, 13) +
                                                                         mpfc("215454640084065536") * Power(nu, 14) +
                                                                         mpfc("3937662087504640") * Power(nu, 15) +
                                                                         mpfc("58487466759040") * Power(nu, 16) +
                                                                         mpfc("694506090432") * Power(nu, 17) +
                                                                         mpfc("6433501536") * Power(nu, 18) +
                                                                         mpfc("44780120") * Power(nu, 19) +
                                                                         mpfc("220220") * Power(nu, 20) +
                                                                         mpfc("682") * Power(nu, 21) + Power(nu, 22)) +
                   mpfc("17550") * Power(lambda, 4) *
                   (mpfc("563842329523055507917504512000000") + mpfc("569787027145675294315370250240000") * nu +
                    mpfc("270552594038180089279021056000000") * Power(nu, 2) +
                    mpfc("80373527314871524594009767936000") * Power(nu, 3) +
                    mpfc("16778129929246329765192164966400") * Power(nu, 4) +
                    mpfc("2620289830708301814663663845376") * Power(nu, 5) +
                    mpfc("318202719499850314657258536960") * Power(nu, 6) +
                    mpfc("30827241349833838044985163776") * Power(nu, 7) +
                    mpfc("2425010235266437468371025920") * Power(nu, 8) +
                    mpfc("156816061917822041347588096") * Power(nu, 9) +
                    mpfc("8406690423695581401415680") * Power(nu, 10) +
                    mpfc("375620144312026783645696") * Power(nu, 11) + mpfc("14027081208956921763840") * Power(nu, 12) +
                    mpfc("437975509601552505856") * Power(nu, 13) + mpfc("11411182804527475200") * Power(nu, 14) +
                    mpfc("246955936784102656") * Power(nu, 15) + mpfc("4405561821576960") * Power(nu, 16) +
                    mpfc("64043515482496") * Power(nu, 17) + mpfc("745974102720") * Power(nu, 18) +
                    mpfc("6791742496") * Power(nu, 19) + mpfc("46541880") * Power(nu, 20) +
                    mpfc("225676") * Power(nu, 21) + mpfc("690") * Power(nu, 22) + Power(nu, 23)) +
                   mpfc("2925") * Power(lambda, 3) *
                   (mpfc("3383053977138333047505027072000000") + mpfc("3982564492397107273809726013440000") * nu +
                    mpfc("2193102591374755829989496586240000") * Power(nu, 2) +
                    mpfc("752793757927409236843079663616000") * Power(nu, 3) +
                    mpfc("181042306890349503185162757734400") * Power(nu, 4) +
                    mpfc("32499868913496140653174148038656") * Power(nu, 5) +
                    mpfc("4529506147707403702607215067136") * Power(nu, 6) +
                    mpfc("503166167598853342927169519616") * Power(nu, 7) +
                    mpfc("45377302761432462855211319296") * Power(nu, 8) +
                    mpfc("3365906606773369716456554496") * Power(nu, 9) +
                    mpfc("207256204459995529756082176") * Power(nu, 10) +
                    mpfc("10660411289567742103289856") * Power(nu, 11) +
                    mpfc("459782631565768314228736") * Power(nu, 12) + mpfc("16654934266566236798976") * Power(nu, 13) +
                    mpfc("506442606428717357056") * Power(nu, 14) + mpfc("12892918425232091136") * Power(nu, 15) +
                    mpfc("273389307713564416") * Power(nu, 16) + mpfc("4789822914471936") * Power(nu, 17) +
                    mpfc("68519360098816") * Power(nu, 18) + mpfc("786724557696") * Power(nu, 19) +
                    mpfc("7070993776") * Power(nu, 20) + mpfc("47895936") * Power(nu, 21) +
                    mpfc("229816") * Power(nu, 22) + mpfc("696") * Power(nu, 23) + Power(nu, 24)) +
                   mpfc("351") * Power(lambda, 2) *
                   (mpfc("13532215908553332190020108288000000") + mpfc("19313311946726762142743931125760000") * nu +
                    mpfc("12754974857896130593767712358400000") * Power(nu, 2) +
                    mpfc("5204277623084392777361815240704000") * Power(nu, 3) +
                    mpfc("1476962985488807249583730694553600") * Power(nu, 4) +
                    mpfc("311041782544334065797859349889024") * Power(nu, 5) +
                    mpfc("50617893504325755463603008307200") * Power(nu, 6) +
                    mpfc("6542170818102817074315893145600") * Power(nu, 7) +
                    mpfc("684675378644583194348014796800") * Power(nu, 8) +
                    mpfc("58840929188525941721037537280") * Power(nu, 9) +
                    mpfc("4194931424613351835480883200") * Power(nu, 10) +
                    mpfc("249897849618266498169241600") * Power(nu, 11) +
                    mpfc("12499541815830815360204800") * Power(nu, 12) +
                    mpfc("526402368632033261424640") * Power(nu, 13) + mpfc("18680704692281106227200") * Power(nu, 14) +
                    mpfc("558014280129645721600") * Power(nu, 15) + mpfc("13986475656086348800") * Power(nu, 16) +
                    mpfc("292548599371452160") * Power(nu, 17) + mpfc("5063900354867200") * Power(nu, 18) +
                    mpfc("71666258329600") * Power(nu, 19) + mpfc("815008532800") * Power(nu, 20) +
                    mpfc("7262577520") * Power(nu, 21) + mpfc("48815200") * Power(nu, 22) +
                    mpfc("232600") * Power(nu, 23) + mpfc("700") * Power(nu, 24) + Power(nu, 25)) +
                   mpfc("27") * lambda *
                   (mpfc("27064431817106664380040216576000000") + mpfc("52158839802006856475507970539520000") * nu +
                    mpfc("44823261662519023330279355842560000") * Power(nu, 2) +
                    mpfc("23163530104064916148491342839808000") * Power(nu, 3) +
                    mpfc("8158203594062007276529276629811200") * Power(nu, 4) +
                    mpfc("2099046550577475381179449394331648") * Power(nu, 5) +
                    mpfc("412277569552985576725065366503424") * Power(nu, 6) +
                    mpfc("63702235140531389612234794598400") * Power(nu, 7) +
                    mpfc("7911521575391983463011922739200") * Power(nu, 8) +
                    mpfc("802357237021635077790089871360") * Power(nu, 9) +
                    mpfc("67230792037752645391999303680") * Power(nu, 10) +
                    mpfc("4694727123849884831819366400") * Power(nu, 11) +
                    mpfc("274896933249928128889651200") * Power(nu, 12) +
                    mpfc("13552346553094881883054080") * Power(nu, 13) +
                    mpfc("563763778016595473879040") * Power(nu, 14) + mpfc("19796733252540397670400") * Power(nu, 15) +
                    mpfc("585987231441818419200") * Power(nu, 16) + mpfc("14571572854829253120") * Power(nu, 17) +
                    mpfc("302676400081186560") * Power(nu, 18) + mpfc("5207232871526400") * Power(nu, 19) +
                    mpfc("73296275395200") * Power(nu, 20) + mpfc("829533687840") * Power(nu, 21) +
                    mpfc("7360207920") * Power(nu, 22) + mpfc("49280400") * Power(nu, 23) +
                    mpfc("234000") * Power(nu, 24) + mpfc("702") * Power(nu, 25) + Power(nu, 26)) + nu *
                                                                                                    (mpfc("27064431817106664380040216576000000") +
                                                                                                     mpfc("52158839802006856475507970539520000") *
                                                                                                     nu +
                                                                                                     mpfc("44823261662519023330279355842560000") *
                                                                                                     Power(nu, 2) +
                                                                                                     mpfc("23163530104064916148491342839808000") *
                                                                                                     Power(nu, 3) +
                                                                                                     mpfc("8158203594062007276529276629811200") *
                                                                                                     Power(nu, 4) +
                                                                                                     mpfc("2099046550577475381179449394331648") *
                                                                                                     Power(nu, 5) +
                                                                                                     mpfc("412277569552985576725065366503424") *
                                                                                                     Power(nu, 6) +
                                                                                                     mpfc("63702235140531389612234794598400") *
                                                                                                     Power(nu, 7) +
                                                                                                     mpfc("7911521575391983463011922739200") *
                                                                                                     Power(nu, 8) +
                                                                                                     mpfc("802357237021635077790089871360") *
                                                                                                     Power(nu, 9) +
                                                                                                     mpfc("67230792037752645391999303680") *
                                                                                                     Power(nu, 10) +
                                                                                                     mpfc("4694727123849884831819366400") *
                                                                                                     Power(nu, 11) +
                                                                                                     mpfc("274896933249928128889651200") *
                                                                                                     Power(nu, 12) +
                                                                                                     mpfc("13552346553094881883054080") *
                                                                                                     Power(nu, 13) +
                                                                                                     mpfc("563763778016595473879040") *
                                                                                                     Power(nu, 14) +
                                                                                                     mpfc("19796733252540397670400") *
                                                                                                     Power(nu, 15) +
                                                                                                     mpfc("585987231441818419200") *
                                                                                                     Power(nu, 16) +
                                                                                                     mpfc("14571572854829253120") *
                                                                                                     Power(nu, 17) +
                                                                                                     mpfc("302676400081186560") *
                                                                                                     Power(nu, 18) +
                                                                                                     mpfc("5207232871526400") *
                                                                                                     Power(nu, 19) +
                                                                                                     mpfc("73296275395200") *
                                                                                                     Power(nu, 20) +
                                                                                                     mpfc("829533687840") *
                                                                                                     Power(nu, 21) +
                                                                                                     mpfc("7360207920") *
                                                                                                     Power(nu, 22) +
                                                                                                     mpfc("49280400") *
                                                                                                     Power(nu, 23) +
                                                                                                     mpfc("234000") *
                                                                                                     Power(nu, 24) +
                                                                                                     mpfc("702") *
                                                                                                     Power(nu, 25) +
                                                                                                     Power(nu, 26)));
#ifdef MULTIPRECISION_NON_CENTRAL_CHI_SQUARED_QUADRATURE
                                                                                                                                moment(28, Power(lambda,28) + mpfc("28")*Power(lambda,27)*(mpfc("54") + nu) + mpfc("378")*Power(lambda,26)*(mpfc("2808") + mpfc("106")*nu + Power(nu,2)) + mpfc("3276")*Power(lambda,25)*(mpfc("140400") + mpfc("8108")*nu + mpfc("156")*Power(nu,2) + Power(nu,3)) + mpfc("20475")*Power(lambda,24)*(mpfc("6739200") + mpfc("529584")*nu + mpfc("15596")*Power(nu,2) + mpfc("204")*Power(nu,3) + Power(nu,4)) + mpfc("98280")*Power(lambda,23)*(mpfc("310003200") + mpfc("31100064")*nu + mpfc("1247000")*Power(nu,2) + mpfc("24980")*Power(nu,3) + mpfc("250")*Power(nu,4) + Power(nu,5)) + mpfc("376740")*Power(lambda,22)*(mpfc("13640140800") + mpfc("1678406016")*nu + mpfc("85968064")*Power(nu,2) + mpfc("2346120")*Power(nu,3) + mpfc("35980")*Power(nu,4) + mpfc("294")*Power(nu,5) + Power(nu,6)) + mpfc("1184040")*Power(lambda,21)*(mpfc("572885913600") + mpfc("84133193472")*nu + mpfc("5289064704")*Power(nu,2) + mpfc("184505104")*Power(nu,3) + mpfc("3857280")*Power(nu,4) + mpfc("48328")*Power(nu,5) + mpfc("336")*Power(nu,6) + Power(nu,7)) + mpfc("3108105")*Power(lambda,20)*(mpfc("22915436544000") + mpfc("3938213652480")*nu + mpfc("295695781632")*Power(nu,2) + mpfc("12669268864")*Power(nu,3) + mpfc("338796304")*Power(nu,4) + mpfc("5790400")*Power(nu,5) + mpfc("61768")*Power(nu,6) + mpfc("376")*Power(nu,7) + Power(nu,8)) + mpfc("6906900")*Power(lambda,19)*(mpfc("870786588672000") + mpfc("172567555338240")*nu + mpfc("15174653354496")*Power(nu,2) + mpfc("777127998464")*Power(nu,3) + mpfc("25543528416")*Power(nu,4) + mpfc("558831504")*Power(nu,5) + mpfc("8137584")*Power(nu,6) + mpfc("76056")*Power(nu,7) + mpfc("414")*Power(nu,8) + Power(nu,9)) + mpfc("13123110")*Power(lambda,18)*(mpfc("31348317192192000") + mpfc("7083218580848640")*nu + mpfc("718855076100096")*Power(nu,2) + mpfc("43151261299200")*Power(nu,3) + mpfc("1696695021440")*Power(nu,4) + mpfc("45661462560")*Power(nu,5) + mpfc("851784528")*Power(nu,6) + mpfc("10875600")*Power(nu,7) + mpfc("90960")*Power(nu,8) + mpfc("450")*Power(nu,9) + Power(nu,10)) + mpfc("21474180")*Power(lambda,17)*(mpfc("1065842784534528000") + mpfc("272177748941045760")*nu + mpfc("31524291168251904")*Power(nu,2) + mpfc("2185997960272896")*Power(nu,3) + mpfc("100838892028160")*Power(nu,4) + mpfc("3249184748480")*Power(nu,5) + mpfc("74622136512")*Power(nu,6) + mpfc("1221554928")*Power(nu,7) + mpfc("13968240")*Power(nu,8) + mpfc("106260")*Power(nu,9) + mpfc("484")*Power(nu,10) + Power(nu,11)) + mpfc("30421755")*Power(lambda,16)*(mpfc("34106969105104896000") + mpfc("9775530750647992320")*nu + mpfc("1280955066325106688")*Power(nu,2) + mpfc("101476225896984576")*Power(nu,3) + mpfc("5412842505174016")*Power(nu,4) + mpfc("204812803979520")*Power(nu,5) + mpfc("5637093116864")*Power(nu,6) + mpfc("113711894208")*Power(nu,7) + mpfc("1668538608")*Power(nu,8) + mpfc("17368560")*Power(nu,9) + mpfc("121748")*Power(nu,10) + mpfc("516")*Power(nu,11) + Power(nu,12)) + mpfc("37442160")*Power(lambda,15)*(mpfc("1023209073153146880000") + mpfc("327372891624544665600")*nu + mpfc("48204182740401192960")*Power(nu,2) + mpfc("4325241843234643968")*Power(nu,3) + mpfc("263861501052205056")*Power(nu,4) + mpfc("11557226624559616")*Power(nu,5) + mpfc("373925597485440")*Power(nu,6) + mpfc("9048449943104")*Power(nu,7) + mpfc("163768052448")*Power(nu,8) + mpfc("2189595408")*Power(nu,9) + mpfc("21021000")*Power(nu,10) + mpfc("137228")*Power(nu,11) + mpfc("546")*Power(nu,12) + Power(nu,13)) + mpfc("40116600")*Power(lambda,14)*(mpfc("28649854048288112640000") + mpfc("10189650038640397516800")*nu + mpfc("1677090008355778068480")*Power(nu,2) + mpfc("169310954350971224064")*Power(nu,3) + mpfc("11713363872696385536")*Power(nu,4) + mpfc("587463846539874304")*Power(nu,5) + mpfc("22027143354151936")*Power(nu,6) + mpfc("627282195892352")*Power(nu,7) + mpfc("13633955411648")*Power(nu,8) + mpfc("225076723872")*Power(nu,9) + mpfc("2778183408")*Power(nu,10) + mpfc("24863384")*Power(nu,11) + mpfc("152516")*Power(nu,12) + mpfc("574")*Power(nu,13) + Power(nu,14)) + mpfc("37442160")*Power(lambda,13)*(mpfc("744896205255490928640000") + mpfc("293580755052938448076800")*nu + mpfc("53793990255890627297280")*Power(nu,2) + mpfc("6079174821481029894144")*Power(nu,3) + mpfc("473858415041077248000")*Power(nu,4) + mpfc("26987423882733117440")*Power(nu,5) + mpfc("1160169573747824640")*Power(nu,6) + mpfc("38336480447353088")*Power(nu,7) + mpfc("981765036595200")*Power(nu,8) + mpfc("19485950232320")*Power(nu,9) + mpfc("297309492480")*Power(nu,10) + mpfc("3424631392")*Power(nu,11) + mpfc("28828800")*Power(nu,12) + mpfc("167440")*Power(nu,13) + mpfc("600")*Power(nu,14) + Power(nu,15)) + mpfc("30421755")*Power(lambda,12)*(mpfc("17877508926131782287360000") + mpfc("7790834326526013682483200")*nu + mpfc("1584636521194313503211520")*Power(nu,2) + mpfc("199694185971435344756736")*Power(nu,3) + mpfc("17451776782466883846144")*Power(nu,4) + mpfc("1121556588226672066560")*Power(nu,5) + mpfc("54831493652680908800")*Power(nu,6) + mpfc("2080245104484298752")*Power(nu,7) + mpfc("61898841325637888")*Power(nu,8) + mpfc("1449427842170880")*Power(nu,9) + mpfc("26621378051840")*Power(nu,10) + mpfc("379500645888")*Power(nu,11) + mpfc("4116522592")*Power(nu,12) + mpfc("32847360")*Power(nu,13) + mpfc("181840")*Power(nu,14) + mpfc("624")*Power(nu,15) + Power(nu,16)) + mpfc("21474180")*Power(lambda,11)*(mpfc("393305196374899210321920000") + mpfc("189275864109704083301990400")*nu + mpfc("42652837792800910753136640")*Power(nu,2) + mpfc("5977908612565891087859712")*Power(nu,3) + mpfc("583633275185706789371904")*Power(nu,4) + mpfc("42126021723453669310464")*Power(nu,5) + mpfc("2327849448585652060160")*Power(nu,6) + mpfc("100596885951335481344")*Power(nu,7) + mpfc("3442019613648332288")*Power(nu,8) + mpfc("93786253853397248")*Power(nu,9) + mpfc("2035098159311360")*Power(nu,10) + mpfc("34970392261376")*Power(nu,11) + mpfc("470064142912")*Power(nu,12) + mpfc("4839164512")*Power(nu,13) + mpfc("36847840")*Power(nu,14) + mpfc("195568")*Power(nu,15) + mpfc("646")*Power(nu,16) + Power(nu,17)) + mpfc("13123110")*Power(lambda,10)*(mpfc("7866103927497984206438400000") + mpfc("4178822478568980876361728000")*nu + mpfc("1042332619965722298364723200")*Power(nu,2) + mpfc("162211010044118732510330880")*Power(nu,3) + mpfc("17650574116280026875297792")*Power(nu,4) + mpfc("1426153709654780175581184")*Power(nu,5) + mpfc("88683010695166710513664")*Power(nu,6) + mpfc("4339787167612361687040")*Power(nu,7) + mpfc("169437278224302127104")*Power(nu,8) + mpfc("5317744690716277248")*Power(nu,9) + mpfc("134488217039624448")*Power(nu,10) + mpfc("2734506004538880")*Power(nu,11) + mpfc("44371675119616")*Power(nu,12) + mpfc("566847433152")*Power(nu,13) + mpfc("5576121312")*Power(nu,14) + mpfc("40759200")*Power(nu,15) + mpfc("208488")*Power(nu,16) + mpfc("666")*Power(nu,17) + Power(nu,18)) + mpfc("6906900")*Power(lambda,9)*(mpfc("141589870694963715715891200000") + mpfc("83084908541739639980949504000")*nu + mpfc("22940809637951982246926745600")*Power(nu,2) + mpfc("3962130800759859483550679040")*Power(nu,3) + mpfc("479921344137159216265691136")*Power(nu,4) + mpfc("43321340890066070035759104")*Power(nu,5) + mpfc("3022447902167780964827136")*Power(nu,6) + mpfc("166799179712189220880384")*Power(nu,7) + mpfc("7389658175649799974912")*Power(nu,8) + mpfc("265156682657195117568")*Power(nu,9) + mpfc("7738532597429517312")*Power(nu,10) + mpfc("183709325121324288")*Power(nu,11) + mpfc("3533196156691968")*Power(nu,12) + mpfc("54574928916352")*Power(nu,13) + mpfc("667217616768")*Power(nu,14) + mpfc("6309786912")*Power(nu,15) + mpfc("44511984")*Power(nu,16) + mpfc("220476")*Power(nu,17) + mpfc("684")*Power(nu,18) + Power(nu,19)) + mpfc("3108105")*Power(lambda,8)*(mpfc("2265437931119419451454259200000") + mpfc("1470948407362797955411083264000")*nu + mpfc("450137862748971355931777433600")*Power(nu,2) + mpfc("86334902450109733983737610240")*Power(nu,3) + mpfc("11640872306954406943801737216")*Power(nu,4) + mpfc("1173062798378216336837836800")*Power(nu,5) + mpfc("91680507324750565472993280")*Power(nu,6) + mpfc("5691234777562808498913280")*Power(nu,7) + mpfc("285033710522586020478976")*Power(nu,8) + mpfc("11632165098164921856000")*Power(nu,9) + mpfc("388973204216067394560")*Power(nu,10) + mpfc("10677881799370705920")*Power(nu,11) + mpfc("240240463628395776")*Power(nu,12) + mpfc("4406395019353600")*Power(nu,13) + mpfc("65250410784640")*Power(nu,14) + mpfc("768174207360")*Power(nu,15) + mpfc("7021978656")*Power(nu,16) + mpfc("48039600")*Power(nu,17) + mpfc("231420")*Power(nu,18) + mpfc("700")*Power(nu,19) + Power(nu,20)) + mpfc("1184040")*Power(lambda,7)*(mpfc("31716131035671872320359628800000") + mpfc("22858715634198590827209424896000")*nu + mpfc("7772878485848396938455967334400")*Power(nu,2) + mpfc("1658826497050507631704103976960")*Power(nu,3) + mpfc("249307114747471431196961931264")*Power(nu,4) + mpfc("28063751484249435659531452416")*Power(nu,5) + mpfc("2456589900924724253459742720")*Power(nu,6) + mpfc("171357794210629884457779200")*Power(nu,7) + mpfc("9681706724879012785618944")*Power(nu,8) + mpfc("447884021896894926462976")*Power(nu,9) + mpfc("17077789957189865379840")*Power(nu,10) + mpfc("538463549407257277440")*Power(nu,11) + mpfc("14041248290168246784")*Power(nu,12) + mpfc("301929993899346176")*Power(nu,13) + mpfc("5319900770338560")*Power(nu,14) + mpfc("76004849687680")*Power(nu,15) + mpfc("866481908544")*Power(nu,16) + mpfc("7694533056")*Power(nu,17) + mpfc("51279480")*Power(nu,18) + mpfc("241220")*Power(nu,19) + mpfc("714")*Power(nu,20) + Power(nu,21)) + mpfc("376740")*Power(lambda,6)*(mpfc("380593572428062467844315545600000") + mpfc("306020718646054962246872727552000")*nu + mpfc("116133257464379354088681032908800")*Power(nu,2) + mpfc("27678796450454488518905215057920")*Power(nu,3) + mpfc("4650511874020164806067647152128")*Power(nu,4) + mpfc("586072132558464659111339360256")*Power(nu,5) + mpfc("57542830295346126701048365056")*Power(nu,6) + mpfc("4512883431452282866953093120")*Power(nu,7) + mpfc("287538274909178037885206528")*Power(nu,8) + mpfc("15056314987641751903174656")*Power(nu,9) + mpfc("652817501383173311021056")*Power(nu,10) + mpfc("23539352550076952709120")*Power(nu,11) + mpfc("706958528889276238848")*Power(nu,12) + mpfc("17664408216960400896")*Power(nu,13) + mpfc("365768803143408896")*Power(nu,14) + mpfc("6231958966590720")*Power(nu,15) + mpfc("86402632590208")*Power(nu,16) + mpfc("958816305216")*Power(nu,17) + mpfc("8309886816")*Power(nu,18) + mpfc("54174120")*Power(nu,19) + mpfc("249788")*Power(nu,20) + mpfc("726")*Power(nu,21) + Power(nu,22)) + mpfc("98280")*Power(lambda,5)*(mpfc("3805935724280624678443155456000000") + mpfc("3440800758888612090313042821120000")*nu + mpfc("1467353293289848503133683056640000")*Power(nu,2) + mpfc("392921221968924239277733183488000")*Power(nu,3) + mpfc("74183915190656136579581686579200")*Power(nu,4) + mpfc("10511233199604811397181040754688")*Power(nu,5) + mpfc("1161500435511925926121823010816")*Power(nu,6) + mpfc("102671664609868955370579296256")*Power(nu,7) + mpfc("7388266180544063245805158400")*Power(nu,8) + mpfc("438101424785595556916953088")*Power(nu,9) + mpfc("21584490001473485013385216")*Power(nu,10) + mpfc("888211026883942838112256")*Power(nu,11) + mpfc("30608937838969715097600")*Power(nu,12) + mpfc("883602611058880247808")*Power(nu,13) + mpfc("21322096248394489856")*Power(nu,14) + mpfc("428088392809316096")*Power(nu,15) + mpfc("7095985292492800")*Power(nu,16) + mpfc("95990795642368")*Power(nu,17) + mpfc("1041915173376")*Power(nu,18) + mpfc("8851628016")*Power(nu,19) + mpfc("56672000")*Power(nu,20) + mpfc("257048")*Power(nu,21) + mpfc("736")*Power(nu,22) + Power(nu,23)) + mpfc("20475")*Power(lambda,4)*(mpfc("30447485794244997427545243648000000") + mpfc("31332341795389521400947498024960000")*nu + mpfc("15179627105207400115382507274240000")*Power(nu,2) + mpfc("4610723069041242417355548524544000")*Power(nu,3) + mpfc("986392543494173331914386676121600")*Power(nu,4) + mpfc("158273780787494627757030012616704")*Power(nu,5) + mpfc("19803236683700218806155624841216")*Power(nu,6) + mpfc("1982873752390877569086457380864")*Power(nu,7) + mpfc("161777794054221461337020563456")*Power(nu,8) + mpfc("10893077578828827701140783104")*Power(nu,9) + mpfc("610777344797383437024034816")*Power(nu,10) + mpfc("28690178216545027718283264")*Power(nu,11) + mpfc("1133082529595700558893056")*Power(nu,12) + mpfc("37677758727440757080064")*Power(nu,13) + mpfc("1054179381046036166656")*Power(nu,14) + mpfc("24746803390869018624")*Power(nu,15) + mpfc("484856275149258496")*Power(nu,16) + mpfc("7863911657631744")*Power(nu,17) + mpfc("104326117029376")*Power(nu,18) + mpfc("1112728197504")*Power(nu,19) + mpfc("9305004016")*Power(nu,20) + mpfc("58728384")*Power(nu,21) + mpfc("262936")*Power(nu,22) + mpfc("744")*Power(nu,23) + Power(nu,24)) + mpfc("3276")*Power(lambda,3)*(mpfc("182684914765469984565271461888000000") + mpfc("218441536566582125833230231797760000")*nu + mpfc("122410104426633922093242541670400000")*Power(nu,2) + mpfc("42843965519454854619515798421504000")*Power(nu,3) + mpfc("10529078330006282408841868581273600")*Power(nu,4) + mpfc("1936035228219141098456566751821824")*Power(nu,5) + mpfc("277093200889695940593963761664000")*Power(nu,6) + mpfc("31700479198045484220674369126400")*Power(nu,7) + mpfc("2953540516716206337108580761600")*Power(nu,8) + mpfc("227136259527194427543865262080")*Power(nu,9) + mpfc("14557741647613128323284992000")*Power(nu,10) + mpfc("782918414096653603333734400")*Power(nu,11) + mpfc("35488673394119231071641600")*Power(nu,12) + mpfc("1359149081960345101373440")*Power(nu,13) + mpfc("44002835013716974080000")*Power(nu,14) + mpfc("1202660201391250278400")*Power(nu,15) + mpfc("27655941041764569600")*Power(nu,16) + mpfc("532039745095048960")*Power(nu,17) + mpfc("8489868359808000")*Power(nu,18) + mpfc("111002486214400")*Power(nu,19) + mpfc("1168558221600")*Power(nu,20) + mpfc("9657374320")*Power(nu,21) + mpfc("60306000")*Power(nu,22) + mpfc("267400")*Power(nu,23) + mpfc("750")*Power(nu,24) + Power(nu,25)) + mpfc("378")*Power(lambda,2)*(mpfc("730739659061879938261085847552000000") + mpfc("1056451061031798487898192389079040000")*nu + mpfc("708081954273117814206200398479360000")*Power(nu,2) + mpfc("293785966504453340571305735356416000")*Power(nu,3) + mpfc("84960278839479984254883272746598400")*Power(nu,4) + mpfc("18273219242882846802668135588560896")*Power(nu,5) + mpfc("3044408031777924860832421798477824")*Power(nu,6) + mpfc("403895117681877877476661238169600")*Power(nu,7) + mpfc("43514641264910309569108692172800")*Power(nu,8) + mpfc("3862085554824984047284041809920")*Power(nu,9) + mpfc("285367226117646940837005230080")*Power(nu,10) + mpfc("17689415303999742736619929600")*Power(nu,11) + mpfc("924873107673130527620300800")*Power(nu,12) + mpfc("40925269721960611477135360")*Power(nu,13) + mpfc("1535160422015212997693440")*Power(nu,14) + mpfc("48813475819281975193600")*Power(nu,15) + mpfc("1313283965558308556800")*Power(nu,16) + mpfc("29784100022144765440")*Power(nu,17) + mpfc("565999218534280960")*Power(nu,18) + mpfc("8933878304665600")*Power(nu,19) + mpfc("115676719100800")*Power(nu,20) + mpfc("1207187718880")*Power(nu,21) + mpfc("9898598320")*Power(nu,22) + mpfc("61375600")*Power(nu,23) + mpfc("270400")*Power(nu,24) + mpfc("754")*Power(nu,25) + Power(nu,26)) + mpfc("28")*lambda*(mpfc("1461479318123759876522171695104000000") + mpfc("2843641781125476914057470625710080000")*nu + mpfc("2472614969578034116310593186037760000")*Power(nu,2) + mpfc("1295653887282024495348811869192192000")*Power(nu,3) + mpfc("463706524183413309081072280849612800")*Power(nu,4) + mpfc("121506717325245677860219543923720192")*Power(nu,5) + mpfc("24362035306438696524332979185516544")*Power(nu,6) + mpfc("3852198267141680615785744274817024")*Power(nu,7) + mpfc("490924400211698496614878622515200")*Power(nu,8) + mpfc("51238812374560277663676775792640")*Power(nu,9) + mpfc("4432820007060277928958052270080")*Power(nu,10) + mpfc("320746056725646426310245089280")*Power(nu,11) + mpfc("19539161519346003791860531200")*Power(nu,12) + mpfc("1006723647117051750574571520")*Power(nu,13) + mpfc("43995590565991037472522240")*Power(nu,14) + mpfc("1632787373653776948080640")*Power(nu,15) + mpfc("51440043750398592307200")*Power(nu,16) + mpfc("1372852165602598087680")*Power(nu,17) + mpfc("30916098459213327360")*Power(nu,18) + mpfc("583866975143612160")*Power(nu,19) + mpfc("9165231742867200")*Power(nu,20) + mpfc("118091094538560")*Power(nu,21) + mpfc("1226984915520")*Power(nu,22) + mpfc("10021349520")*Power(nu,23) + mpfc("61916400")*Power(nu,24) + mpfc("271908")*Power(nu,25) + mpfc("756")*Power(nu,26) + Power(nu,27)) + nu*(mpfc("1461479318123759876522171695104000000") + mpfc("2843641781125476914057470625710080000")*nu + mpfc("2472614969578034116310593186037760000")*Power(nu,2) + mpfc("1295653887282024495348811869192192000")*Power(nu,3) + mpfc("463706524183413309081072280849612800")*Power(nu,4) + mpfc("121506717325245677860219543923720192")*Power(nu,5) + mpfc("24362035306438696524332979185516544")*Power(nu,6) + mpfc("3852198267141680615785744274817024")*Power(nu,7) + mpfc("490924400211698496614878622515200")*Power(nu,8) + mpfc("51238812374560277663676775792640")*Power(nu,9) + mpfc("4432820007060277928958052270080")*Power(nu,10) + mpfc("320746056725646426310245089280")*Power(nu,11) + mpfc("19539161519346003791860531200")*Power(nu,12) + mpfc("1006723647117051750574571520")*Power(nu,13) + mpfc("43995590565991037472522240")*Power(nu,14) + mpfc("1632787373653776948080640")*Power(nu,15) + mpfc("51440043750398592307200")*Power(nu,16) + mpfc("1372852165602598087680")*Power(nu,17) + mpfc("30916098459213327360")*Power(nu,18) + mpfc("583866975143612160")*Power(nu,19) + mpfc("9165231742867200")*Power(nu,20) + mpfc("118091094538560")*Power(nu,21) + mpfc("1226984915520")*Power(nu,22) + mpfc("10021349520")*Power(nu,23) + mpfc("61916400")*Power(nu,24) + mpfc("271908")*Power(nu,25) + mpfc("756")*Power(nu,26) + Power(nu,27)));
        moment(29, Power(lambda,29) + mpfc("29")*Power(lambda,28)*(mpfc("56") + nu) + mpfc("406")*Power(lambda,27)*(mpfc("3024") + mpfc("110")*nu + Power(nu,2)) + mpfc("3654")*Power(lambda,26)*(mpfc("157248") + mpfc("8744")*nu + mpfc("162")*Power(nu,2) + Power(nu,3)) + mpfc("23751")*Power(lambda,25)*(mpfc("7862400") + mpfc("594448")*nu + mpfc("16844")*Power(nu,2) + mpfc("212")*Power(nu,3) + Power(nu,4)) + mpfc("118755")*Power(lambda,24)*(mpfc("377395200") + mpfc("36395904")*nu + mpfc("1402960")*Power(nu,2) + mpfc("27020")*Power(nu,3) + mpfc("260")*Power(nu,4) + Power(nu,5)) + mpfc("475020")*Power(lambda,23)*(mpfc("17360179200") + mpfc("2051606784")*nu + mpfc("100932064")*Power(nu,2) + mpfc("2645880")*Power(nu,3) + mpfc("38980")*Power(nu,4) + mpfc("306")*Power(nu,5) + Power(nu,6)) + mpfc("1560780")*Power(lambda,22)*(mpfc("763847884800") + mpfc("107630877696")*nu + mpfc("6492617600")*Power(nu,2) + mpfc("217350784")*Power(nu,3) + mpfc("4361000")*Power(nu,4) + mpfc("52444")*Power(nu,5) + mpfc("350")*Power(nu,6) + Power(nu,7)) + mpfc("4292145")*Power(lambda,21)*(mpfc("32081611161600") + mpfc("5284344748032")*nu + mpfc("380320816896")*Power(nu,2) + mpfc("15621350528")*Power(nu,3) + mpfc("400512784")*Power(nu,4) + mpfc("6563648")*Power(nu,5) + mpfc("67144")*Power(nu,6) + mpfc("392")*Power(nu,7) + Power(nu,8)) + mpfc("10015005")*Power(lambda,20)*(mpfc("1283264446464000") + mpfc("243455401082880")*nu + mpfc("20497177423872")*Power(nu,2) + mpfc("1005174838016")*Power(nu,3) + mpfc("31641861888")*Power(nu,4) + mpfc("663058704")*Power(nu,5) + mpfc("9249408")*Power(nu,6) + mpfc("82824")*Power(nu,7) + mpfc("432")*Power(nu,8) + Power(nu,9)) + mpfc("20030010")*Power(lambda,19)*(mpfc("48764048965632000") + mpfc("10534569687613440")*nu + mpfc("1022348143190016")*Power(nu,2) + mpfc("58693821268480")*Power(nu,3) + mpfc("2207565589760")*Power(nu,4) + mpfc("56838092640")*Power(nu,5) + mpfc("1014536208")*Power(nu,6) + mpfc("12396720")*Power(nu,7) + mpfc("99240")*Power(nu,8) + mpfc("470")*Power(nu,9) + Power(nu,10)) + mpfc("34597290")*Power(lambda,18)*(mpfc("1755505762762752000") + mpfc("428008557719715840")*nu + mpfc("47339102842454016")*Power(nu,2) + mpfc("3135325708855296")*Power(nu,3) + mpfc("138166182499840")*Power(nu,4) + mpfc("4253736924800")*Power(nu,5) + mpfc("93361396128")*Power(nu,6) + mpfc("1460818128")*Power(nu,7) + mpfc("15969360")*Power(nu,8) + mpfc("116160")*Power(nu,9) + mpfc("506")*Power(nu,10) + Power(nu,11)) + mpfc("51895935")*Power(lambda,17)*(mpfc("59687195933933568000") + mpfc("16307796725233090560")*nu + mpfc("2037538054363152384")*Power(nu,2) + mpfc("153940176943534080")*Power(nu,3) + mpfc("7832975913849856")*Power(nu,4) + mpfc("282793237943040")*Power(nu,5) + mpfc("7428024393152")*Power(nu,6) + mpfc("143029212480")*Power(nu,7) + mpfc("2003776368")*Power(nu,8) + mpfc("19918800")*Power(nu,9) + mpfc("133364")*Power(nu,10) + mpfc("540")*Power(nu,11) + Power(nu,12)) + mpfc("67863915")*Power(lambda,16)*(mpfc("1909990269885874176000") + mpfc("581536691141392465920")*nu + mpfc("81509014464853966848")*Power(nu,2) + mpfc("6963623716556242944")*Power(nu,3) + mpfc("404595406186729472")*Power(nu,4) + mpfc("16882359528027136")*Power(nu,5) + mpfc("520490018523904")*Power(nu,6) + mpfc("12004959192512")*Power(nu,7) + mpfc("207150056256")*Power(nu,8) + mpfc("2641177968")*Power(nu,9) + mpfc("24186448")*Power(nu,10) + mpfc("150644")*Power(nu,11) + mpfc("572")*Power(nu,12) + Power(nu,13)) + mpfc("77558760")*Power(lambda,15)*(mpfc("57299708096576225280000") + mpfc("19356091004127648153600")*nu + mpfc("3026807125087011471360")*Power(nu,2) + mpfc("290417725961541255168")*Power(nu,3) + mpfc("19101485902158127104")*Power(nu,4) + mpfc("911066192027543552")*Power(nu,5) + mpfc("32497060083744256")*Power(nu,6) + mpfc("880638794299264")*Power(nu,7) + mpfc("18219460880192")*Power(nu,8) + mpfc("286385395296")*Power(nu,9) + mpfc("3366771408")*Power(nu,10) + mpfc("28705768")*Power(nu,11) + mpfc("167804")*Power(nu,12) + mpfc("602")*Power(nu,13) + Power(nu,14)) + mpfc("77558760")*Power(lambda,14)*(mpfc("1604391826704134307840000") + mpfc("599270256212150373580800")*nu + mpfc("104106690506563969351680")*Power(nu,2) + mpfc("11158503452010166616064")*Power(nu,3) + mpfc("825259331221968814080")*Power(nu,4) + mpfc("44611339278929346560")*Power(nu,5) + mpfc("1820983874372382720")*Power(nu,6) + mpfc("57154946324123648")*Power(nu,7) + mpfc("1390783698944640")*Power(nu,8) + mpfc("26238251948480")*Power(nu,9) + mpfc("380654994720")*Power(nu,10) + mpfc("4170532912")*Power(nu,11) + mpfc("33404280")*Power(nu,12) + mpfc("184660")*Power(nu,13) + mpfc("630")*Power(nu,14) + Power(nu,15)) + mpfc("67863915")*Power(lambda,13)*(mpfc("41714187494307492003840000") + mpfc("17185418488220044020940800")*nu + mpfc("3306044209382813576724480")*Power(nu,2) + mpfc("394227780258828301369344")*Power(nu,3) + mpfc("32615246063781355782144")*Power(nu,4) + mpfc("1985154152474131824640")*Power(nu,5) + mpfc("91956920012611297280")*Power(nu,6) + mpfc("3307012478799597568")*Power(nu,7) + mpfc("93315322496684288")*Power(nu,8) + mpfc("2072978249605120")*Power(nu,9) + mpfc("36135281811200")*Power(nu,10) + mpfc("489088850432")*Power(nu,11) + mpfc("5039044192")*Power(nu,12) + mpfc("38205440")*Power(nu,13) + mpfc("201040")*Power(nu,14) + mpfc("656")*Power(nu,15) + Power(nu,16)) + mpfc("51895935")*Power(lambda,12)*(mpfc("1001140499863379808092160000") + mpfc("454164231211588548506419200")*nu + mpfc("96530479513407569862328320")*Power(nu,2) + mpfc("12767510935594692809588736")*Power(nu,3) + mpfc("1176993685789580840140800")*Power(nu,4) + mpfc("80258945723160519573504")*Power(nu,5) + mpfc("4192120232776802959360")*Power(nu,6) + mpfc("171325219503801638912")*Power(nu,7) + mpfc("5546580218720020480")*Power(nu,8) + mpfc("143066800487207168")*Power(nu,9) + mpfc("2940225013073920")*Power(nu,10) + mpfc("47873414221568")*Power(nu,11) + mpfc("610025911040")*Power(nu,12) + mpfc("5955974752")*Power(nu,13) + mpfc("43030400")*Power(nu,14) + mpfc("216784")*Power(nu,15) + mpfc("680")*Power(nu,16) + Power(nu,17)) + mpfc("34597290")*Power(lambda,11)*(mpfc("22025090996994355778027520000") + mpfc("10992753586518327875233382400")*nu + mpfc("2577834780506555085477642240")*Power(nu,2) + mpfc("377415720096490811673280512")*Power(nu,3) + mpfc("38661372022965471292686336")*Power(nu,4) + mpfc("2942690491699112270757888")*Power(nu,5) + mpfc("172485590844250184679424")*Power(nu,6) + mpfc("7961275061860439015424")*Power(nu,7) + mpfc("293349984315642089472")*Power(nu,8) + mpfc("8694049829438578176")*Power(nu,9) + mpfc("207751750774833408")*Power(nu,10) + mpfc("3993440125948416")*Power(nu,11) + mpfc("61293984264448")*Power(nu,12) + mpfc("741057355584")*Power(nu,13) + mpfc("6902643552")*Power(nu,14) + mpfc("47799648")*Power(nu,15) + mpfc("231744")*Power(nu,16) + mpfc("702")*Power(nu,17) + Power(nu,18)) + mpfc("20030010")*Power(lambda,10)*(mpfc("440501819939887115560550400000") + mpfc("241880162727360913282695168000")*nu + mpfc("62549449196649429584786227200")*Power(nu,2) + mpfc("10126149182436371318943252480")*Power(nu,3) + mpfc("1150643160555800237527007232")*Power(nu,4) + mpfc("97515181856947716707844096")*Power(nu,5) + mpfc("6392402308584115964346368")*Power(nu,6) + mpfc("331711092081458964987904")*Power(nu,7) + mpfc("13828274748173280804864")*Power(nu,8) + mpfc("467230980904413652992")*Power(nu,9) + mpfc("12849084844935246336")*Power(nu,10) + mpfc("287620553293801728")*Power(nu,11) + mpfc("5219319811237376")*Power(nu,12) + mpfc("76115131376128")*Power(nu,13) + mpfc("879110226624")*Power(nu,14) + mpfc("7858636512")*Power(nu,15) + mpfc("52434528")*Power(nu,16) + mpfc("245784")*Power(nu,17) + mpfc("722")*Power(nu,18) + Power(nu,19)) + mpfc("10015005")*Power(lambda,9)*(mpfc("7929032758917968080089907200000") + mpfc("4794344749032383554649063424000")*nu + mpfc("1367770248267050645808847257600")*Power(nu,2) + mpfc("244820134480504113325764771840")*Power(nu,3) + mpfc("30837726072440775594429382656")*Power(nu,4) + mpfc("2905916433980859138268200960")*Power(nu,5) + mpfc("212578423411461804066078720")*Power(nu,6) + mpfc("12363201966050377334128640")*Power(nu,7) + mpfc("580620037548578019475456")*Power(nu,8) + mpfc("22238432404452726558720")*Power(nu,9) + mpfc("698514508113248087040")*Power(nu,10) + mpfc("18026254804223677440")*Power(nu,11) + mpfc("381568309896074496")*Power(nu,12) + mpfc("6589392176007680")*Power(nu,13) + mpfc("91939115455360")*Power(nu,14) + mpfc("1020565683840")*Power(nu,15) + mpfc("8802458016")*Power(nu,16) + mpfc("56858640")*Power(nu,17) + mpfc("258780")*Power(nu,18) + mpfc("740")*Power(nu,19) + Power(nu,20)) + mpfc("4292145")*Power(lambda,8)*(mpfc("126864524142687489281438515200000") + mpfc("84638548743436104954474921984000")*nu + mpfc("26678668721305193887590619545600")*Power(nu,2) + mpfc("5284892399955116459021083607040")*Power(nu,3) + mpfc("738223751639556522836634894336")*Power(nu,4) + mpfc("77332389016134521806720598016")*Power(nu,5) + mpfc("6307171208564248003325460480")*Power(nu,6) + mpfc("410389654868267841412136960")*Power(nu,7) + mpfc("21653122566827625645735936")*Power(nu,8) + mpfc("936434956019821644414976")*Power(nu,9) + mpfc("33414664534264695951360")*Power(nu,10) + mpfc("986934584980826926080")*Power(nu,11) + mpfc("24131347762560869376")*Power(nu,12) + mpfc("486998584712197376")*Power(nu,13) + mpfc("8060418023293440")*Power(nu,14) + mpfc("108268166396800")*Power(nu,15) + mpfc("1161405012096")*Power(nu,16) + mpfc("9712196256")*Power(nu,17) + mpfc("60999120")*Power(nu,18) + mpfc("270620")*Power(nu,19) + mpfc("756")*Power(nu,20) + Power(nu,21)) + mpfc("1560780")*Power(lambda,7)*(mpfc("1776103337997624849940139212800000") + mpfc("1311804206550792958644087422976000")*nu + mpfc("458139910841708819380743595622400")*Power(nu,2) + mpfc("100667162320676824313885790044160")*Power(nu,3) + mpfc("15620024922908907778733972127744")*Power(nu,4) + mpfc("1820877197865439828130723266560")*Power(nu,5) + mpfc("165632785936033993853277044736")*Power(nu,6) + mpfc("12052626376719997783095377920")*Power(nu,7) + mpfc("713533370803854600452440064")*Power(nu,8) + mpfc("34763211951105128667545600")*Power(nu,9) + mpfc("1404240259499527387734016")*Power(nu,10) + mpfc("47231748723996272916480")*Power(nu,11) + mpfc("1324773453656679097344")*Power(nu,12) + mpfc("30949327948531632640")*Power(nu,13) + mpfc("599844437038305536")*Power(nu,14) + mpfc("9576172352848640")*Power(nu,15) + mpfc("124527836566144")*Power(nu,16) + mpfc("1297375759680")*Power(nu,17) + mpfc("10566183936")*Power(nu,18) + mpfc("64787800")*Power(nu,19) + mpfc("281204")*Power(nu,20) + mpfc("770")*Power(nu,21) + Power(nu,22)) + mpfc("475020")*Power(lambda,6)*(mpfc("21313240055971498199281670553600000") + mpfc("17517753816607140353669188288512000")*nu + mpfc("6809483136651298791213010570444800")*Power(nu,2) + mpfc("1666145858689830711147373076152320")*Power(nu,3) + mpfc("288107461395583717658693455577088")*Power(nu,4) + mpfc("37470551297294185716302651326464")*Power(nu,5) + mpfc("3808470629097847754370047803392")*Power(nu,6) + mpfc("310264302456673967250421579776")*Power(nu,7) + mpfc("20615026826366252988524658688")*Power(nu,8) + mpfc("1130691914217116144462987264")*Power(nu,9) + mpfc("51614095065099457320353792")*Power(nu,10) + mpfc("1971021244187482662731776")*Power(nu,11) + mpfc("63129030167876422084608")*Power(nu,12) + mpfc("1696165389039058689024")*Power(nu,13) + mpfc("38147461192991299072")*Power(nu,14) + mpfc("714758505272489216")*Power(nu,15) + mpfc("11070506391642368")*Power(nu,16) + mpfc("140096345682304")*Power(nu,17) + mpfc("1424169966912")*Power(nu,18) + mpfc("11343637536")*Power(nu,19) + mpfc("68162248")*Power(nu,20) + mpfc("290444")*Power(nu,21) + mpfc("782")*Power(nu,22) + Power(nu,23)) + mpfc("118755")*Power(lambda,5)*(mpfc("213132400559714981992816705536000000") + mpfc("196490778222042901735973553438720000")*nu + mpfc("85612585183120128265799293992960000")*Power(nu,2) + mpfc("23470941723549605902686741331968000")*Power(nu,3) + mpfc("4547220472645667887734307631923200")*Power(nu,4) + mpfc("662812974368525574821719968841728")*Power(nu,5) + mpfc("75555257588272663260003129360384")*Power(nu,6) + mpfc("6911113653664587426874263601152")*Power(nu,7) + mpfc("516414570720336497135668166656")*Power(nu,8) + mpfc("31921945968537414433154531328")*Power(nu,9) + mpfc("1646832864868110717666525184")*Power(nu,10) + mpfc("71324307506974283947671552")*Power(nu,11) + mpfc("2602311545866246883577856")*Power(nu,12) + mpfc("80090684058267008974848")*Power(nu,13) + mpfc("2077640000968971679744")*Power(nu,14) + mpfc("45295046245716191232")*Power(nu,15) + mpfc("825463569188912896")*Power(nu,16) + mpfc("12471469848465408")*Power(nu,17) + mpfc("154338045351424")*Power(nu,18) + mpfc("1537606342272")*Power(nu,19) + mpfc("12025260016")*Power(nu,20) + mpfc("71066688")*Power(nu,21) + mpfc("298264")*Power(nu,22) + mpfc("792")*Power(nu,23) + Power(nu,24)) + mpfc("23751")*Power(lambda,4)*(mpfc("1705059204477719855942533644288000000") + mpfc("1785058626336058195880605133045760000")*nu + mpfc("881391459687003927862367905382400000")*Power(nu,2) + mpfc("273380118971516975487293224648704000")*Power(nu,3) + mpfc("59848705504714949004561202387353600")*Power(nu,4) + mpfc("9849724267593872486308067382657024")*Power(nu,5) + mpfc("1267255035074706880901745003724800")*Power(nu,6) + mpfc("130844166817589362674997238169600")*Power(nu,7) + mpfc("11042430219427279403959608934400")*Power(nu,8) + mpfc("771790138468635812600904417280")*Power(nu,9) + mpfc("45096608887482300174486732800")*Power(nu,10) + mpfc("2217427324923904989247897600")*Power(nu,11) + mpfc("92142799873904259016294400")*Power(nu,12) + mpfc("3243037018332382955376640")*Power(nu,13) + mpfc("96711804066018782412800")*Power(nu,14) + mpfc("2440000370934701209600")*Power(nu,15) + mpfc("51898754799227494400")*Power(nu,16) + mpfc("925235327976636160")*Power(nu,17) + mpfc("13706174211276800")*Power(nu,18) + mpfc("166638896089600")*Power(nu,19) + mpfc("1633808422400")*Power(nu,20) + mpfc("12593793520")*Power(nu,21) + mpfc("73452800")*Power(nu,22) + mpfc("304600")*Power(nu,23) + mpfc("800")*Power(nu,24) + Power(nu,25)) + mpfc("3654")*Power(lambda,3)*(mpfc("10230355226866319135655201865728000000") + mpfc("12415410962494069031226164442562560000")*nu + mpfc("7073407384458081763054812565340160000")*Power(nu,2) + mpfc("2521672173516105780786127253274624000")*Power(nu,3) + mpfc("632472351999806669514660438972825600")*Power(nu,4) + mpfc("118947051110278183922409606683295744")*Power(nu,5) + mpfc("17453254478042113771718537405005824")*Power(nu,6) + mpfc("2052320035980243056951728432742400")*Power(nu,7) + mpfc("197098748134153039098754891776000")*Power(nu,8) + mpfc("15673171050239094279565035438080")*Power(nu,9) + mpfc("1042369791793529613647824814080")*Power(nu,10) + mpfc("58401172837025730109974118400")*Power(nu,11) + mpfc("2770284124167330543345664000")*Power(nu,12) + mpfc("111601021983898556748554240")*Power(nu,13) + mpfc("3823307842728495649853440")*Power(nu,14) + mpfc("111351806291626989670400")*Power(nu,15) + mpfc("2751392899730066176000")*Power(nu,16) + mpfc("57450166767087311360")*Power(nu,17) + mpfc("1007472373244296960")*Power(nu,18) + mpfc("14706007587814400")*Power(nu,19) + mpfc("176441746624000")*Power(nu,20) + mpfc("1709371183520")*Power(nu,21) + mpfc("13034510320")*Power(nu,22) + mpfc("75280400")*Power(nu,23) + mpfc("309400")*Power(nu,24) + mpfc("806")*Power(nu,25) + Power(nu,26)) + mpfc("406")*Power(lambda,2)*(mpfc("40921420907465276542620807462912000000") + mpfc("59891999076842595260559859635978240000")*nu + mpfc("40709040500326396083445414703923200000")*Power(nu,2) + mpfc("17160096078522504886199321578438656000")*Power(nu,3) + mpfc("5051561581515332458844769009165926400")*Power(nu,4) + mpfc("1108260556440919405204298865706008576")*Power(nu,5) + mpfc("188760069022446639009283756303319040")*Power(nu,6) + mpfc("25662534621963085999525451135975424")*Power(nu,7) + mpfc("2840715028516855213346747999846400")*Power(nu,8) + mpfc("259791432335109416217015033528320")*Power(nu,9) + mpfc("19842650217413212734156334694400")*Power(nu,10) + mpfc("1275974483141632534087721287680")*Power(nu,11) + mpfc("69482309333695052283356774400")*Power(nu,12) + mpfc("3216688212102924770339880960")*Power(nu,13) + mpfc("126894253354812539347968000")*Power(nu,14) + mpfc("4268715067895003608535040")*Power(nu,15) + mpfc("122357377890547254374400")*Power(nu,16) + mpfc("2981193566798415421440")*Power(nu,17) + mpfc("61480056260064499200")*Power(nu,18) + mpfc("1066296403595554560")*Power(nu,19) + mpfc("15411774574310400")*Power(nu,20) + mpfc("183279231358080")*Power(nu,21) + mpfc("1761509224800")*Power(nu,22) + mpfc("13335631920")*Power(nu,23) + mpfc("76518000")*Power(nu,24) + mpfc("312624")*Power(nu,25) + mpfc("810")*Power(nu,26) + Power(nu,27)) + mpfc("29")*lambda*(mpfc("81842841814930553085241614925824000000") + mpfc("160705419061150467063740526734868480000")*nu + mpfc("141310080077495387427450689043824640000")*Power(nu,2) + mpfc("75029232657371405855844057860800512000")*Power(nu,3) + mpfc("27263219241553169803888859596770508800")*Power(nu,4) + mpfc("7268082694397171269253366740577943552")*Power(nu,5) + mpfc("1485780694485812683222866378312646656")*Power(nu,6) + mpfc("240085138266372811008334658575269888")*Power(nu,7) + mpfc("31343964678996796426218947135668224")*Power(nu,8) + mpfc("3360297893187074045780778066903040")*Power(nu,9) + mpfc("299476732769935841685327702917120")*Power(nu,10) + mpfc("22394599183696477802331777269760")*Power(nu,11) + mpfc("1414939101809022638654434836480")*Power(nu,12) + mpfc("75915685757900901824036536320")*Power(nu,13) + mpfc("3470476718812549849035816960")*Power(nu,14) + mpfc("135431683490602546565038080")*Power(nu,15) + mpfc("4513429823676098117283840")*Power(nu,16) + mpfc("128319765024144085217280")*Power(nu,17) + mpfc("3104153679318544419840")*Power(nu,18) + mpfc("63612649067255608320")*Power(nu,19) + mpfc("1097119952744175360")*Power(nu,20) + mpfc("15778333037026560")*Power(nu,21) + mpfc("186802249807680")*Power(nu,22) + mpfc("1788180488640")*Power(nu,23) + mpfc("13488667920")*Power(nu,24) + mpfc("77143248")*Power(nu,25) + mpfc("314244")*Power(nu,26) + mpfc("812")*Power(nu,27) + Power(nu,28)) + nu*(mpfc("81842841814930553085241614925824000000") + mpfc("160705419061150467063740526734868480000")*nu + mpfc("141310080077495387427450689043824640000")*Power(nu,2) + mpfc("75029232657371405855844057860800512000")*Power(nu,3) + mpfc("27263219241553169803888859596770508800")*Power(nu,4) + mpfc("7268082694397171269253366740577943552")*Power(nu,5) + mpfc("1485780694485812683222866378312646656")*Power(nu,6) + mpfc("240085138266372811008334658575269888")*Power(nu,7) + mpfc("31343964678996796426218947135668224")*Power(nu,8) + mpfc("3360297893187074045780778066903040")*Power(nu,9) + mpfc("299476732769935841685327702917120")*Power(nu,10) + mpfc("22394599183696477802331777269760")*Power(nu,11) + mpfc("1414939101809022638654434836480")*Power(nu,12) + mpfc("75915685757900901824036536320")*Power(nu,13) + mpfc("3470476718812549849035816960")*Power(nu,14) + mpfc("135431683490602546565038080")*Power(nu,15) + mpfc("4513429823676098117283840")*Power(nu,16) + mpfc("128319765024144085217280")*Power(nu,17) + mpfc("3104153679318544419840")*Power(nu,18) + mpfc("63612649067255608320")*Power(nu,19) + mpfc("1097119952744175360")*Power(nu,20) + mpfc("15778333037026560")*Power(nu,21) + mpfc("186802249807680")*Power(nu,22) + mpfc("1788180488640")*Power(nu,23) + mpfc("13488667920")*Power(nu,24) + mpfc("77143248")*Power(nu,25) + mpfc("314244")*Power(nu,26) + mpfc("812")*Power(nu,27) + Power(nu,28)));
        moment(30, Power(lambda,30) + mpfc("30")*Power(lambda,29)*(mpfc("58") + nu) + mpfc("435")*Power(lambda,28)*(mpfc("3248") + mpfc("114")*nu + Power(nu,2)) + mpfc("4060")*Power(lambda,27)*(mpfc("175392") + mpfc("9404")*nu + mpfc("168")*Power(nu,2) + Power(nu,3)) + mpfc("27405")*Power(lambda,26)*(mpfc("9120384") + mpfc("664400")*nu + mpfc("18140")*Power(nu,2) + mpfc("220")*Power(nu,3) + Power(nu,4)) + mpfc("142506")*Power(lambda,25)*(mpfc("456019200") + mpfc("42340384")*nu + mpfc("1571400")*Power(nu,2) + mpfc("29140")*Power(nu,3) + mpfc("270")*Power(nu,4) + Power(nu,5)) + mpfc("593775")*Power(lambda,24)*(mpfc("21888921600") + mpfc("2488357632")*nu + mpfc("117767584")*Power(nu,2) + mpfc("2970120")*Power(nu,3) + mpfc("42100")*Power(nu,4) + mpfc("318")*Power(nu,5) + Power(nu,6)) + mpfc("2035800")*Power(lambda,23)*(mpfc("1006890393600") + mpfc("136353372672")*nu + mpfc("7905666496")*Power(nu,2) + mpfc("254393104")*Power(nu,3) + mpfc("4906720")*Power(nu,4) + mpfc("56728")*Power(nu,5) + mpfc("364")*Power(nu,6) + Power(nu,7)) + mpfc("5852925")*Power(lambda,22)*(mpfc("44303177318400") + mpfc("7006438791168")*nu + mpfc("484202698496")*Power(nu,2) + mpfc("19098963072")*Power(nu,3) + mpfc("470288784")*Power(nu,4) + mpfc("7402752")*Power(nu,5) + mpfc("72744")*Power(nu,6) + mpfc("408")*Power(nu,7) + Power(nu,8)) + mpfc("14307150")*Power(lambda,21)*(mpfc("1860733447372800") + mpfc("338573606547456")*nu + mpfc("27342952128000")*Power(nu,2) + mpfc("1286359147520")*Power(nu,3) + mpfc("38851092000")*Power(nu,4) + mpfc("781204368")*Power(nu,5) + mpfc("10458000")*Power(nu,6) + mpfc("89880")*Power(nu,7) + mpfc("450")*Power(nu,8) + Power(nu,9)) + mpfc("30045015")*Power(lambda,20)*(mpfc("74429337894912000") + mpfc("15403677709271040")*nu + mpfc("1432291691667456")*Power(nu,2) + mpfc("78797318028800")*Power(nu,3) + mpfc("2840402827520")*Power(nu,4) + mpfc("70099266720")*Power(nu,5) + mpfc("1199524368")*Power(nu,6) + mpfc("14053200")*Power(nu,7) + mpfc("107880")*Power(nu,8) + mpfc("490")*Power(nu,9) + Power(nu,10)) + mpfc("54627300")*Power(lambda,19)*(mpfc("2828314840006656000") + mpfc("659769090847211520")*nu + mpfc("69830761992634368")*Power(nu,2) + mpfc("4426589776761856")*Power(nu,3) + mpfc("186732625474560")*Power(nu,4) + mpfc("5504174962880")*Power(nu,5) + mpfc("115681192704")*Power(nu,6) + mpfc("1733545968")*Power(nu,7) + mpfc("18152640")*Power(nu,8) + mpfc("126500")*Power(nu,9) + mpfc("528")*Power(nu,10) + Power(nu,11)) + mpfc("86493225")*Power(lambda,18)*(mpfc("101819334240239616000") + mpfc("26580002110506270720")*nu + mpfc("3173676522582048768")*Power(nu,2) + mpfc("229187993956061184")*Power(nu,3) + mpfc("11148964293846016")*Power(nu,4) + mpfc("384882924138240")*Power(nu,5) + mpfc("9668697900224")*Power(nu,6) + mpfc("178088847552")*Power(nu,7) + mpfc("2387041008")*Power(nu,8) + mpfc("22706640")*Power(nu,9) + mpfc("145508")*Power(nu,10) + mpfc("564")*Power(nu,11) + Power(nu,12)) + mpfc("119759850")*Power(lambda,17)*(mpfc("3461857364168146944000") + mpfc("1005539405997452820480")*nu + mpfc("134485003878295928832")*Power(nu,2) + mpfc("10966068317088129024")*Power(nu,3) + mpfc("608252779946825728")*Power(nu,4) + mpfc("24234983714546176")*Power(nu,5) + mpfc("713618652745856")*Power(nu,6) + mpfc("15723718716992")*Power(nu,7) + mpfc("259248241824")*Power(nu,8) + mpfc("3159066768")*Power(nu,9) + mpfc("27653912")*Power(nu,10) + mpfc("164684")*Power(nu,11) + mpfc("598")*Power(nu,12) + Power(nu,13)) + mpfc("145422675")*Power(lambda,16)*(mpfc("110779435653380702208000") + mpfc("35639118356086637199360")*nu + mpfc("5309059530102922543104")*Power(nu,2) + mpfc("485399190025116057600")*Power(nu,3) + mpfc("30430157275386552320")*Power(nu,4) + mpfc("1383772258812303360")*Power(nu,5) + mpfc("47070780602413568")*Power(nu,6) + mpfc("1216777651689600")*Power(nu,7) + mpfc("24019662455360")*Power(nu,8) + mpfc("360338378400")*Power(nu,9) + mpfc("4043991952")*Power(nu,10) + mpfc("32923800")*Power(nu,11) + mpfc("183820")*Power(nu,12) + mpfc("630")*Power(nu,13) + Power(nu,14)) + mpfc("155117520")*Power(lambda,15)*(mpfc("3323383069601421066240000") + mpfc("1179952986335979818188800")*nu + mpfc("194910904259174313492480")*Power(nu,2) + mpfc("19871035230856404271104")*Power(nu,3) + mpfc("1398303908286712627200")*Power(nu,4) + mpfc("71943325039755653120")*Power(nu,5) + mpfc("2795895676884710400")*Power(nu,6) + mpfc("83574110153101568")*Power(nu,7) + mpfc("1937367525350400")*Power(nu,8) + mpfc("34829813807360")*Power(nu,9) + mpfc("481658136960")*Power(nu,10) + mpfc("5031705952")*Power(nu,11) + mpfc("38438400")*Power(nu,12) + mpfc("202720")*Power(nu,13) + mpfc("660")*Power(nu,14) + Power(nu,15)) + mpfc("145422675")*Power(lambda,14)*(mpfc("93054725948839789854720000") + mpfc("36362066687008855975526400")*nu + mpfc("6637458305592860595978240")*Power(nu,2) + mpfc("751299890723153633083392")*Power(nu,3) + mpfc("59023544662884357832704")*Power(nu,4) + mpfc("3412717009399870914560")*Power(nu,5) + mpfc("150228403992527544320")*Power(nu,6) + mpfc("5135970761171554304")*Power(nu,7) + mpfc("137820400862912768")*Power(nu,8) + mpfc("2912602311956480")*Power(nu,9) + mpfc("48316241642240")*Power(nu,10) + mpfc("622545903616")*Power(nu,11) + mpfc("6107981152")*Power(nu,12) + mpfc("44114560")*Power(nu,13) + mpfc("221200")*Power(nu,14) + mpfc("688")*Power(nu,15) + Power(nu,16)) + mpfc("119759850")*Power(lambda,13)*(mpfc("2419422874669834536222720000") + mpfc("1038468459811070045218406400")*nu + mpfc("208935982632423231470960640")*Power(nu,2) + mpfc("26171255464394855056146432")*Power(nu,3) + mpfc("2285912051958146936733696")*Power(nu,4) + mpfc("147754186907281001611264")*Power(nu,5) + mpfc("7318655513205587066880")*Power(nu,6) + mpfc("283763643782987956224")*Power(nu,7) + mpfc("8719301183607286272")*Power(nu,8) + mpfc("213548060973781248")*Power(nu,9) + mpfc("4168824594654720")*Power(nu,10) + mpfc("64502435136256")*Power(nu,11) + mpfc("781353413568")*Power(nu,12) + mpfc("7254959712")*Power(nu,13) + mpfc("49865760")*Power(nu,14) + mpfc("239088")*Power(nu,15) + mpfc("714")*Power(nu,16) + Power(nu,17)) + mpfc("86493225")*Power(lambda,12)*(mpfc("58066148992076028869345280000") + mpfc("27342665910135515621464473600")*nu + mpfc("6052932042989227600521461760")*Power(nu,2) + mpfc("837046113777899752818475008")*Power(nu,3) + mpfc("81033144711390381537755136")*Power(nu,4) + mpfc("5832012537732890975404032")*Power(nu,5) + mpfc("323401919224215091216384")*Power(nu,6) + mpfc("14128982963997298016256")*Power(nu,7) + mpfc("493026872189562826752")*Power(nu,8) + mpfc("13844454646978036224")*Power(nu,9) + mpfc("313599851245494528")*Power(nu,10) + mpfc("5716883037924864")*Power(nu,11) + mpfc("83254917061888")*Power(nu,12) + mpfc("955472446656")*Power(nu,13) + mpfc("8451737952")*Power(nu,14) + mpfc("55603872")*Power(nu,15) + mpfc("256224")*Power(nu,16) + mpfc("738")*Power(nu,17) + Power(nu,18)) + mpfc("54627300")*Power(lambda,11)*(mpfc("1277455277825672635125596160000") + mpfc("659604799015057372541563699200")*nu + mpfc("160507170855898522832936632320")*Power(nu,2) + mpfc("24467946546103022162527911936")*Power(nu,3) + mpfc("2619775297428488146649088000")*Power(nu,4) + mpfc("209337420541513982996643840")*Power(nu,5) + mpfc("12946854760665622982164480")*Power(nu,6) + mpfc("634239544432155647574016")*Power(nu,7) + mpfc("24975574152167680204800")*Power(nu,8) + mpfc("797604874423079623680")*Power(nu,9) + mpfc("20743651374378915840")*Power(nu,10) + mpfc("439371278079841536")*Power(nu,11) + mpfc("7548491213286400")*Power(nu,12) + mpfc("104275310888320")*Power(nu,13) + mpfc("1141410681600")*Power(nu,14) + mpfc("9675023136")*Power(nu,15) + mpfc("61240800")*Power(nu,16) + mpfc("272460")*Power(nu,17) + mpfc("760")*Power(nu,18) + Power(nu,19)) + mpfc("30045015")*Power(lambda,10)*(mpfc("25549105556513452702511923200000") + mpfc("14469551258126820085956870144000")*nu + mpfc("3869748216133027829200296345600")*Power(nu,2) + mpfc("649866101777958966083494871040")*Power(nu,3) + mpfc("76863452494672785095509671936")*Power(nu,4) + mpfc("6806523708258767806581964800")*Power(nu,5) + mpfc("468274515754826442639933440")*Power(nu,6) + mpfc("25631645649308735933644800")*Power(nu,7) + mpfc("1133751027475509251670016")*Power(nu,8) + mpfc("40927671640629272678400")*Power(nu,9) + mpfc("1212477901910657940480")*Power(nu,10) + mpfc("29531076935975746560")*Power(nu,11) + mpfc("590341102345569536")*Power(nu,12) + mpfc("9633997431052800")*Power(nu,13) + mpfc("127103524520320")*Power(nu,14) + mpfc("1334911144320")*Power(nu,15) + mpfc("10899839136")*Power(nu,16) + mpfc("66690000")*Power(nu,17) + mpfc("287660")*Power(nu,18) + mpfc("780")*Power(nu,19) + Power(nu,20)) + mpfc("14307150")*Power(lambda,9)*(mpfc("459883900017242148645214617600000") + mpfc("286001028202796214249735585792000")*nu + mpfc("84125019148521321011562204364800")*Power(nu,2) + mpfc("15567338048136289218703204024320")*Power(nu,3) + mpfc("2033408246682069097802668965888")*Power(nu,4) + mpfc("199380879243330605613985038336")*Power(nu,5) + mpfc("15235464991845643774100766720")*Power(nu,6) + mpfc("929644137442383689445539840")*Power(nu,7) + mpfc("46039164143867902463705088")*Power(nu,8) + mpfc("1870449117006836159881216")*Power(nu,9) + mpfc("62752273875021115607040")*Power(nu,10) + mpfc("1744037286758221378560")*Power(nu,11) + mpfc("40157216778195998208")*Power(nu,12) + mpfc("763753056104519936")*Power(nu,13) + mpfc("11921860872418560")*Power(nu,14) + mpfc("151131925118080")*Power(nu,15) + mpfc("1531108248768")*Power(nu,16) + mpfc("12100259136")*Power(nu,17) + mpfc("71867880")*Power(nu,18) + mpfc("301700")*Power(nu,19) + mpfc("798")*Power(nu,20) + Power(nu,21)) + mpfc("5852925")*Power(lambda,8)*(mpfc("7358142400275874378323433881600000") + mpfc("5035900351261981576640983990272000")*nu + mpfc("1632001334579137350434730855628800")*Power(nu,2) + mpfc("333202427918701948510813468753920")*Power(nu,3) + mpfc("48101869995049394783545907478528")*Power(nu,4) + mpfc("5223502314575358787626429579264")*Power(nu,5) + mpfc("443148319112860905999597305856")*Power(nu,6) + mpfc("30109771190923782805229404160")*Power(nu,7) + mpfc("1666270763744270128864821248")*Power(nu,8) + mpfc("75966350015977281021804544")*Power(nu,9) + mpfc("2874485499007174009593856")*Power(nu,10) + mpfc("90656870463152657664000")*Power(nu,11) + mpfc("2386552755209357349888")*Power(nu,12) + mpfc("52377265675868317184")*Power(nu,13) + mpfc("954502830063216896")*Power(nu,14) + mpfc("14339971674307840")*Power(nu,15) + mpfc("175629657098368")*Power(nu,16) + mpfc("1724712394944")*Power(nu,17) + mpfc("13250145216")*Power(nu,18) + mpfc("76695080")*Power(nu,19) + mpfc("314468")*Power(nu,20) + mpfc("814")*Power(nu,21) + Power(nu,22)) + mpfc("2035800")*Power(lambda,7)*(mpfc("103013993603862241296528074342400000") + mpfc("77860747317943616451297209745408000")*nu + mpfc("27883919035369904482727215969075200")*Power(nu,2) + mpfc("6296835325440964629586119418183680")*Power(nu,3) + mpfc("1006628607849393475480456173453312")*Power(nu,4) + mpfc("121230902399104417810315921588224")*Power(nu,5) + mpfc("11427578782155411471620791861248")*Power(nu,6) + mpfc("864685115785793865272808964096")*Power(nu,7) + mpfc("53437561883343564609336901632")*Power(nu,8) + mpfc("2729799663967952063170084864")*Power(nu,9) + mpfc("116209147002077717156118528")*Power(nu,10) + mpfc("4143681685491311216889856")*Power(nu,11) + mpfc("124068609036083660562432")*Power(nu,12) + mpfc("3119834474671513790464")*Power(nu,13) + mpfc("65740305296753353728")*Power(nu,14) + mpfc("1155262433503526656")*Power(nu,15) + mpfc("16798786873684992")*Power(nu,16) + mpfc("199775630627584")*Power(nu,17) + mpfc("1910214427968")*Power(nu,18) + mpfc("14323876336")*Power(nu,19) + mpfc("81097632")*Power(nu,20) + mpfc("325864")*Power(nu,21) + mpfc("828")*Power(nu,22) + Power(nu,23)) + mpfc("593775")*Power(lambda,6)*(mpfc("1236167923246346895558336892108800000") + mpfc("1037342961419185638712094591287296000")*nu + mpfc("412467775742382470244023801374310400")*Power(nu,2) + mpfc("103445942940661480037760648987279360")*Power(nu,3) + mpfc("18376378619633686335351593499623424")*Power(nu,4) + mpfc("2461399436638646489204247232512000")*Power(nu,5) + mpfc("258361847784969355469765423923200")*Power(nu,6) + mpfc("21803800171584937854894499430400")*Power(nu,7) + mpfc("1505935858385916640584851783680")*Power(nu,8) + mpfc("86195157850958989367377920000")*Power(nu,9) + mpfc("4124309427992884669043507200")*Power(nu,10) + mpfc("165933327227973451758796800")*Power(nu,11) + mpfc("5632504993924315143639040")*Power(nu,12) + mpfc("161506622732141826048000")*Power(nu,13) + mpfc("3908718138232554035200")*Power(nu,14) + mpfc("79603454498795673600")*Power(nu,15) + mpfc("1356847875987746560")*Power(nu,16) + mpfc("19196094441216000")*Power(nu,17) + mpfc("222698203763200")*Power(nu,18) + mpfc("2082100944000")*Power(nu,19) + mpfc("15297047920")*Power(nu,20) + mpfc("85008000")*Power(nu,21) + mpfc("335800")*Power(nu,22) + mpfc("840")*Power(nu,23) + Power(nu,24)) + mpfc("142506")*Power(lambda,5)*(mpfc("12361679232463468955583368921088000000") + mpfc("11609597537438203282679282804981760000")*nu + mpfc("5162020718843010341152332605030400000")*Power(nu,2) + mpfc("1446927205148997270621630291247104000")*Power(nu,3) + mpfc("287209729136998343391276583983513600")*Power(nu,4) + mpfc("42990372986020151227394065824743424")*Power(nu,5) + mpfc("5045017914488340043901901471744000")*Power(nu,6) + mpfc("476399849500818734018710418227200")*Power(nu,7) + mpfc("36863158755444104260743017267200")*Power(nu,8) + mpfc("2367887436895506534258630983680")*Power(nu,9) + mpfc("127438252130887836057812992000")*Power(nu,10) + mpfc("5783642700272619186631475200")*Power(nu,11) + mpfc("222258377167216603195187200")*Power(nu,12) + mpfc("7247571221245733404119040")*Power(nu,13) + mpfc("200593804114467366400000")*Power(nu,14) + mpfc("4704752683220510771200")*Power(nu,15) + mpfc("93171933258673139200")*Power(nu,16) + mpfc("1548808820399906560")*Power(nu,17) + mpfc("21423076478848000")*Power(nu,18) + mpfc("243519213203200")*Power(nu,19) + mpfc("2235071423200")*Power(nu,20) + mpfc("16147127920")*Power(nu,21) + mpfc("88366000")*Power(nu,22) + mpfc("344200")*Power(nu,23) + mpfc("850")*Power(nu,24) + Power(nu,25)) + mpfc("27405")*Power(lambda,4)*(mpfc("98893433859707751644666951368704000000") + mpfc("105238459531969095217017631360942080000")*nu + mpfc("52905763288182286011897943645224960000")*Power(nu,2) + mpfc("16737438360034988506125374935007232000")*Power(nu,3) + mpfc("3744605038244984017751842963115212800")*Power(nu,4) + mpfc("631132713025159553210429110581460992")*Power(nu,5) + mpfc("83350516301926871578609277598695424")*Power(nu,6) + mpfc("8856216710494889916051584817561600")*Power(nu,7) + mpfc("771305119544371568104654556364800")*Power(nu,8) + mpfc("55806258250608156534812065136640")*Power(nu,9) + mpfc("3387393453942609222721134919680")*Power(nu,10) + mpfc("173707393733068789550864793600")*Power(nu,11) + mpfc("7561709717610352012192972800")*Power(nu,12) + mpfc("280238946937182470428139520")*Power(nu,13) + mpfc("8852321654161472335319040")*Power(nu,14) + mpfc("238231825580231452569600")*Power(nu,15) + mpfc("5450128149289895884800")*Power(nu,16) + mpfc("105562403821872391680")*Power(nu,17) + mpfc("1720193432230690560")*Power(nu,18) + mpfc("23371230184473600")*Power(nu,19) + mpfc("261399784588800")*Power(nu,20) + mpfc("2364248446560")*Power(nu,21) + mpfc("16854055920")*Power(nu,22) + mpfc("91119600")*Power(nu,23) + mpfc("351000")*Power(nu,24) + mpfc("858")*Power(nu,25) + Power(nu,26)) + mpfc("4060")*Power(lambda,3)*(mpfc("593360603158246509868001708212224000000") + mpfc("730324191051522322946772739534356480000")*nu + mpfc("422673039261062811288405293232291840000")*Power(nu,2) + mpfc("153330393448392217048650193255268352000")*Power(nu,3) + mpfc("39205068589504892612636432713698508800")*Power(nu,4) + mpfc("7531401316395941337014417626603978752")*Power(nu,5) + mpfc("1131235810836720782682084776173633536")*Power(nu,6) + mpfc("136487816564896211074918786504065024")*Power(nu,7) + mpfc("13484047427761119324679512155750400")*Power(nu,8) + mpfc("1106142669048020507313526947184640")*Power(nu,9) + mpfc("76130618974263811871138874654720")*Power(nu,10) + mpfc("4429637816341021960026323681280")*Power(nu,11) + mpfc("219077652038730901624022630400")*Power(nu,12) + mpfc("9243143399233446834761809920")*Power(nu,13) + mpfc("333352876862151304440053760")*Power(nu,14) + mpfc("10281712607642861050736640")*Power(nu,15) + mpfc("270932594475970827878400")*Power(nu,16) + mpfc("6083502572221130234880")*Power(nu,17) + mpfc("115883564415256535040")*Power(nu,18) + mpfc("1860420813337532160")*Power(nu,19) + mpfc("24939628892006400")*Power(nu,20) + mpfc("275585275268160")*Power(nu,21) + mpfc("2465372782080")*Power(nu,22) + mpfc("17400773520")*Power(nu,23) + mpfc("93225600")*Power(nu,24) + mpfc("356148")*Power(nu,25) + mpfc("864")*Power(nu,26) + Power(nu,27)) + mpfc("435")*Power(lambda,2)*(mpfc("2373442412632986039472006832848896000000") + mpfc("3514657367364335801655092666349649920000")*nu + mpfc("2421016348095773568100393912463523840000")*Power(nu,2) + mpfc("1035994613054631679483006066253365248000")*Power(nu,3) + mpfc("310150667806411787499195924110062387200")*Power(nu,4) + mpfc("69330673855088657960694103220114423808")*Power(nu,5) + mpfc("12056344559742824467742756731298512896")*Power(nu,6) + mpfc("1677187077096305626981759922189893632")*Power(nu,7) + mpfc("190424006275940688373636835127066624")*Power(nu,8) + mpfc("17908618103953201353933619944488960")*Power(nu,9) + mpfc("1410665144945075754798082445803520")*Power(nu,10) + mpfc("93849170239627899711244169379840")*Power(nu,11) + mpfc("5305948424495945566522414202880")*Power(nu,12) + mpfc("256050225635664688963069870080")*Power(nu,13) + mpfc("10576554906682052052522024960")*Power(nu,14) + mpfc("374479727292722748643000320")*Power(nu,15) + mpfc("11365442985546744362250240")*Power(nu,16) + mpfc("295266604764855348817920")*Power(nu,17) + mpfc("6547036829882156375040")*Power(nu,18) + mpfc("123325247668606663680")*Power(nu,19) + mpfc("1960179328905557760")*Power(nu,20) + mpfc("26041969993079040")*Power(nu,21) + mpfc("285446766396480")*Power(nu,22) + mpfc("2534975876160")*Power(nu,23) + mpfc("17773675920")*Power(nu,24) + mpfc("94650192")*Power(nu,25) + mpfc("359604")*Power(nu,26) + mpfc("868")*Power(nu,27) + Power(nu,28)) + mpfc("30")*lambda*(mpfc("4746884825265972078944013665697792000000") + mpfc("9402757147361657642782192165548195840000")*nu + mpfc("8356690063555882937855880491276697600000")*Power(nu,2) + mpfc("4493005574205036927066406044970254336000")*Power(nu,3) + mpfc("1656295948667455254481397914473490022400")*Power(nu,4) + mpfc("448812015516589103420584130550291234816")*Power(nu,5) + mpfc("93443362974574306896179616682711449600")*Power(nu,6) + mpfc("15410718713935435721706276575678300160")*Power(nu,7) + mpfc("2058035089648187003729033592444026880")*Power(nu,8) + mpfc("226241242483847091081504075016044544")*Power(nu,9) + mpfc("20729948393843352863529784836096000")*Power(nu,10) + mpfc("1598363485424331554220570784563200")*Power(nu,11) + mpfc("104461067088619790844288997785600")*Power(nu,12) + mpfc("5818048875767274944448553943040")*Power(nu,13) + mpfc("277203335449028793068113920000")*Power(nu,14) + mpfc("11325514361267497549808025600")*Power(nu,15) + mpfc("397210613263816237367500800")*Power(nu,16) + mpfc("11955976195076455059886080")*Power(nu,17) + mpfc("308360678424619661568000")*Power(nu,18) + mpfc("6793687325219369702400")*Power(nu,19) + mpfc("127245606326417779200")*Power(nu,20) + mpfc("2012263268891715840")*Power(nu,21) + mpfc("26612863525872000")*Power(nu,22) + mpfc("290516718148800")*Power(nu,23) + mpfc("2570523228000")*Power(nu,24) + mpfc("17962976304")*Power(nu,25) + mpfc("95369400")*Power(nu,26) + mpfc("361340")*Power(nu,27) + mpfc("870")*Power(nu,28) + Power(nu,29)) + nu*(mpfc("4746884825265972078944013665697792000000") + mpfc("9402757147361657642782192165548195840000")*nu + mpfc("8356690063555882937855880491276697600000")*Power(nu,2) + mpfc("4493005574205036927066406044970254336000")*Power(nu,3) + mpfc("1656295948667455254481397914473490022400")*Power(nu,4) + mpfc("448812015516589103420584130550291234816")*Power(nu,5) + mpfc("93443362974574306896179616682711449600")*Power(nu,6) + mpfc("15410718713935435721706276575678300160")*Power(nu,7) + mpfc("2058035089648187003729033592444026880")*Power(nu,8) + mpfc("226241242483847091081504075016044544")*Power(nu,9) + mpfc("20729948393843352863529784836096000")*Power(nu,10) + mpfc("1598363485424331554220570784563200")*Power(nu,11) + mpfc("104461067088619790844288997785600")*Power(nu,12) + mpfc("5818048875767274944448553943040")*Power(nu,13) + mpfc("277203335449028793068113920000")*Power(nu,14) + mpfc("11325514361267497549808025600")*Power(nu,15) + mpfc("397210613263816237367500800")*Power(nu,16) + mpfc("11955976195076455059886080")*Power(nu,17) + mpfc("308360678424619661568000")*Power(nu,18) + mpfc("6793687325219369702400")*Power(nu,19) + mpfc("127245606326417779200")*Power(nu,20) + mpfc("2012263268891715840")*Power(nu,21) + mpfc("26612863525872000")*Power(nu,22) + mpfc("290516718148800")*Power(nu,23) + mpfc("2570523228000")*Power(nu,24) + mpfc("17962976304")*Power(nu,25) + mpfc("95369400")*Power(nu,26) + mpfc("361340")*Power(nu,27) + mpfc("870")*Power(nu,28) + Power(nu,29)));
        moment(31, Power(lambda,31) + mpfc("31")*Power(lambda,30)*(mpfc("60") + nu) + mpfc("465")*Power(lambda,29)*(mpfc("3480") + mpfc("118")*nu + Power(nu,2)) + mpfc("4495")*Power(lambda,28)*(mpfc("194880") + mpfc("10088")*nu + mpfc("174")*Power(nu,2) + Power(nu,3)) + mpfc("31465")*Power(lambda,27)*(mpfc("10523520") + mpfc("739632")*nu + mpfc("19484")*Power(nu,2) + mpfc("228")*Power(nu,3) + Power(nu,4)) + mpfc("169911")*Power(lambda,26)*(mpfc("547223040") + mpfc("48984384")*nu + mpfc("1752800")*Power(nu,2) + mpfc("31340")*Power(nu,3) + mpfc("280")*Power(nu,4) + Power(nu,5)) + mpfc("736281")*Power(lambda,25)*(mpfc("27361152000") + mpfc("2996442240")*nu + mpfc("136624384")*Power(nu,2) + mpfc("3319800")*Power(nu,3) + mpfc("45340")*Power(nu,4) + mpfc("330")*Power(nu,5) + Power(nu,6)) + mpfc("2629575")*Power(lambda,24)*(mpfc("1313335296000") + mpfc("171190379520")*nu + mpfc("9554412672")*Power(nu,2) + mpfc("295974784")*Power(nu,3) + mpfc("5496120")*Power(nu,4) + mpfc("61180")*Power(nu,5) + mpfc("378")*Power(nu,6) + Power(nu,7)) + mpfc("7888725")*Power(lambda,23)*(mpfc("60413423616000") + mpfc("9188092753920")*nu + mpfc("610693362432")*Power(nu,2) + mpfc("23169252736")*Power(nu,3) + mpfc("548796304")*Power(nu,4) + mpfc("8310400")*Power(nu,5) + mpfc("78568")*Power(nu,6) + mpfc("424")*Power(nu,7) + Power(nu,8)) + mpfc("20160075")*Power(lambda,22)*(mpfc("2658190639104000") + mpfc("464689504788480")*nu + mpfc("36058600700928")*Power(nu,2) + mpfc("1630140482816")*Power(nu,3) + mpfc("47316290112")*Power(nu,4) + mpfc("914453904")*Power(nu,5) + mpfc("11767392")*Power(nu,6) + mpfc("97224")*Power(nu,7) + mpfc("468")*Power(nu,8) + Power(nu,9)) + mpfc("44352165")*Power(lambda,21)*(mpfc("111644006842368000") + mpfc("22175149840220160")*nu + mpfc("1979150734227456")*Power(nu,2) + mpfc("104524500979200")*Power(nu,3) + mpfc("3617424667520")*Power(nu,4) + mpfc("85723354080")*Power(nu,5) + mpfc("1408684368")*Power(nu,6) + mpfc("15850800")*Power(nu,7) + mpfc("116880")*Power(nu,8) + mpfc("510")*Power(nu,9) + Power(nu,10)) + mpfc("84672315")*Power(lambda,20)*(mpfc("4465760273694720000") + mpfc("998650000451174400")*nu + mpfc("101341179209318400")*Power(nu,2) + mpfc("6160130773395456")*Power(nu,3) + mpfc("249221487680000")*Power(nu,4) + mpfc("7046358830720")*Power(nu,5) + mpfc("142070728800")*Power(nu,6) + mpfc("2042716368")*Power(nu,7) + mpfc("20526000")*Power(nu,8) + mpfc("137280")*Power(nu,9) + mpfc("550")*Power(nu,10) + Power(nu,11)) + mpfc("141120525")*Power(lambda,19)*(mpfc("169698890400399360000") + mpfc("42414460290839347200")*nu + mpfc("4849614810405273600")*Power(nu,2) + mpfc("335426148598345728")*Power(nu,3) + mpfc("15630547305235456")*Power(nu,4) + mpfc("516983123247360")*Power(nu,5) + mpfc("12445046525120")*Power(nu,6) + mpfc("219693950784")*Power(nu,7) + mpfc("2822704368")*Power(nu,8) + mpfc("25742640")*Power(nu,9) + mpfc("158180")*Power(nu,10) + mpfc("588")*Power(nu,11) + Power(nu,12)) + mpfc("206253075")*Power(lambda,18)*(mpfc("6109160054414376960000") + mpfc("1696619460870615859200")*nu + mpfc("217000593465429196800")*Power(nu,2) + mpfc("16924956159945719808")*Power(nu,3) + mpfc("898125851586822144")*Power(nu,4) + mpfc("34241939742140416")*Power(nu,5) + mpfc("965004798151680")*Power(nu,6) + mpfc("20354028753344")*Power(nu,7) + mpfc("321311308032")*Power(nu,8) + mpfc("3749439408")*Power(nu,9) + mpfc("31437120")*Power(nu,10) + mpfc("179348")*Power(nu,11) + mpfc("624")*Power(nu,12) + Power(nu,13)) + mpfc("265182525")*Power(lambda,17)*(mpfc("207711441850088816640000") + mpfc("63794221724015316172800")*nu + mpfc("9074639638695208550400")*Power(nu,2) + mpfc("792449102903583670272")*Power(nu,3) + mpfc("47461235113897672704")*Power(nu,4) + mpfc("2062351802819596288")*Power(nu,5) + mpfc("67052102879297536")*Power(nu,6) + mpfc("1657041775765376")*Power(nu,7) + mpfc("31278613226432")*Power(nu,8) + mpfc("448792247904")*Power(nu,9) + mpfc("4818301488")*Power(nu,10) + mpfc("37534952")*Power(nu,11) + mpfc("200564")*Power(nu,12) + mpfc("658")*Power(nu,13) + Power(nu,14)) + mpfc("300540195")*Power(lambda,16)*(mpfc("6646766139202842132480000") + mpfc("2249126537018578934169600")*nu + mpfc("354182690162261989785600")*Power(nu,2) + mpfc("34433010931609885999104")*Power(nu,3) + mpfc("2311208626548309196800")*Power(nu,4) + mpfc("113456492804124753920")*Power(nu,5) + mpfc("4208019094957117440")*Power(nu,6) + mpfc("120077439703789568")*Power(nu,7) + mpfc("2657957399011200")*Power(nu,8) + mpfc("45639965159360")*Power(nu,9) + mpfc("602977895520")*Power(nu,10) + mpfc("6019419952")*Power(nu,11) + mpfc("43953000")*Power(nu,12) + mpfc("221620")*Power(nu,13) + mpfc("690")*Power(nu,14) + Power(nu,15)) + mpfc("300540195")*Power(lambda,15)*(mpfc("199402984176085263974400000") + mpfc("74120562249760210157568000")*nu + mpfc("12874607241886438627737600")*Power(nu,2) + mpfc("1387173018110558569758720")*Power(nu,3) + mpfc("103769269728059161903104")*Power(nu,4) + mpfc("5714903410672051814400")*Power(nu,5) + mpfc("239697065652838277120")*Power(nu,6) + mpfc("7810342286070804480")*Power(nu,7) + mpfc("199816161674125568")*Power(nu,8) + mpfc("4027156353792000")*Power(nu,9) + mpfc("63729302024960")*Power(nu,10) + mpfc("783560494080")*Power(nu,11) + mpfc("7338009952")*Power(nu,12) + mpfc("50601600")*Power(nu,13) + mpfc("242320")*Power(nu,14) + mpfc("720")*Power(nu,15) + Power(nu,16)) + mpfc("265182525")*Power(lambda,14)*(mpfc("5583283556930387391283200000") + mpfc("2274778727169371148386304000")*nu + mpfc("434609565022580491734220800")*Power(nu,2) + mpfc("51715451748982078580981760")*Power(nu,3) + mpfc("4292712570496215103045632")*Power(nu,4) + mpfc("263786565226876612706304")*Power(nu,5) + mpfc("12426421248951523573760")*Power(nu,6) + mpfc("458386649662820802560")*Power(nu,7) + mpfc("13405194812946320384")*Power(nu,8) + mpfc("312576539580301568")*Power(nu,9) + mpfc("5811576810490880")*Power(nu,10) + mpfc("85668995859200")*Power(nu,11) + mpfc("989024772736")*Power(nu,12) + mpfc("8754854752")*Power(nu,13) + mpfc("57386560")*Power(nu,14) + mpfc("262480")*Power(nu,15) + mpfc("748")*Power(nu,16) + Power(nu,17)) + mpfc("206253075")*Power(lambda,13)*(mpfc("145165372480190072173363200000") + mpfc("64727530463334037249327104000")*nu + mpfc("13574627417756463933476044800")*Power(nu,2) + mpfc("1779211310496114534839746560")*Power(nu,3) + mpfc("163325978581883671260168192")*Power(nu,4) + mpfc("11151163266395007033409536")*Power(nu,5) + mpfc("586873517699616225624064")*Power(nu,6) + mpfc("24344474140184864440320")*Power(nu,7) + mpfc("806921714799425132544")*Power(nu,8) + mpfc("21532184842034161152")*Power(nu,9) + mpfc("463677536653064448")*Power(nu,10) + mpfc("8038970702830080")*Power(nu,11) + mpfc("111383639950336")*Power(nu,12) + mpfc("1216650996288")*Power(nu,13) + mpfc("10246905312")*Power(nu,14) + mpfc("64211040")*Power(nu,15) + mpfc("281928")*Power(nu,16) + mpfc("774")*Power(nu,17) + Power(nu,18)) + mpfc("141120525")*Power(lambda,12)*(mpfc("3483968939524561732160716800000") + mpfc("1698626103600206966157213696000")*nu + mpfc("390518588489489171652752179200")*Power(nu,2) + mpfc("56275698869663212769629962240")*Power(nu,3) + mpfc("5699034796461322645083783168")*Power(nu,4) + mpfc("430953896975363840061997056")*Power(nu,5) + mpfc("25236127691185796448387072")*Power(nu,6) + mpfc("1171140897064052972191744")*Power(nu,7) + mpfc("43710595295371067621376")*Power(nu,8) + mpfc("1323694151008245000192")*Power(nu,9) + mpfc("32660445721707707904")*Power(nu,10) + mpfc("656612833520986368")*Power(nu,11) + mpfc("10712178061638144")*Power(nu,12) + mpfc("140583263861248")*Power(nu,13) + mpfc("1462576723776")*Power(nu,14) + mpfc("11787970272")*Power(nu,15) + mpfc("70977312")*Power(nu,16) + mpfc("300504")*Power(nu,17) + mpfc("798")*Power(nu,18) + Power(nu,19)) + mpfc("84672315")*Power(lambda,11)*(mpfc("76647316669540358107535769600000") + mpfc("40853743218729114987619418112000")*nu + mpfc("10290035050368968742517761638400")*Power(nu,2) + mpfc("1628583963622079852584611348480")*Power(nu,3) + mpfc("181654464391812310961473191936")*Power(nu,4) + mpfc("15180020529919327126447718400")*Power(nu,5) + mpfc("986148706181451361926512640")*Power(nu,6) + mpfc("51001227426594961836605440")*Power(nu,7) + mpfc("2132773993562216459862016")*Power(nu,8) + mpfc("72831866617552457625600")*Power(nu,9) + mpfc("2042223956885814574080")*Power(nu,10) + mpfc("47105928059169408000")*Power(nu,11) + mpfc("892280750877025536")*Power(nu,12) + mpfc("13805009866585600")*Power(nu,13) + mpfc("172759951784320")*Power(nu,14) + mpfc("1721912069760")*Power(nu,15) + mpfc("13349471136")*Power(nu,16) + mpfc("77588400")*Power(nu,17) + mpfc("318060")*Power(nu,18) + mpfc("820")*Power(nu,19) + Power(nu,20)) + mpfc("44352165")*Power(lambda,10)*(mpfc("1532946333390807162150715392000000") + mpfc("893722181044122657859924131840000")*nu + mpfc("246654444226108489837974650880000")*Power(nu,2) + mpfc("42861714322810565794209988608000")*Power(nu,3) + mpfc("5261673251458326071814075187200")*Power(nu,4) + mpfc("485254874990198853490427559936")*Power(nu,5) + mpfc("34902994653548354364977971200")*Power(nu,6) + mpfc("2006173254713350598658621440")*Power(nu,7) + mpfc("93656707297839291033845760")*Power(nu,8) + mpfc("3589411325913265612374016")*Power(nu,9) + mpfc("113676345755268749107200")*Power(nu,10) + mpfc("2984342518069202734080")*Power(nu,11) + mpfc("64951543076709918720")*Power(nu,12) + mpfc("1168380948208737536")*Power(nu,13) + mpfc("17260208902272000")*Power(nu,14) + mpfc("207198193179520")*Power(nu,15) + mpfc("1988901492480")*Power(nu,16) + mpfc("14901239136")*Power(nu,17) + mpfc("83949600")*Power(nu,18) + mpfc("334460")*Power(nu,19) + mpfc("840")*Power(nu,20) + Power(nu,21)) + mpfc("20160075")*Power(lambda,9)*(mpfc("27593034001034528918712877056000000") + mpfc("17619945592185015003629349765120000")*nu + mpfc("5333502177114075474943467847680000")*Power(nu,2) + mpfc("1018165302036698674133754445824000")*Power(nu,3) + mpfc("137571832849060435086863341977600")*Power(nu,4) + mpfc("13996261001281905434641771266048")*Power(nu,5) + mpfc("1113508778754069232060031041536")*Power(nu,6) + mpfc("71014113238388665140833157120")*Power(nu,7) + mpfc("3691993986074457837267845120")*Power(nu,8) + mpfc("158266111164278072056578048")*Power(nu,9) + mpfc("5635585549508103096303616")*Power(nu,10) + mpfc("167394511080514398320640")*Power(nu,11) + mpfc("4153470293449981271040")*Power(nu,12) + mpfc("85982400144467194368")*Power(nu,13) + mpfc("1479064708449633536")*Power(nu,14) + mpfc("20989776379503360")*Power(nu,15) + mpfc("242998420044160")*Power(nu,16) + mpfc("2257123796928")*Power(nu,17) + mpfc("16412331936")*Power(nu,18) + mpfc("89969880")*Power(nu,19) + mpfc("349580")*Power(nu,20) + mpfc("858")*Power(nu,21) + Power(nu,22)) + mpfc("7888725")*Power(lambda,8)*(mpfc("441488544016552462699406032896000000") + mpfc("309512163475994768976782473297920000")*nu + mpfc("102955980426010222602724835328000000")*Power(nu,2) + mpfc("21624147009701254261083538980864000")*Power(nu,3) + mpfc("3219314627621665635523567917465600")*Power(nu,4) + mpfc("361512008869570922041131682234368")*Power(nu,5) + mpfc("31812401461347013147602267930624")*Power(nu,6) + mpfc("2249734590568287874313361555456")*Power(nu,7) + mpfc("130086017015579990537118679040")*Power(nu,8) + mpfc("6224251764702906990173093888")*Power(nu,9) + mpfc("248435479956407721597435904")*Power(nu,10) + mpfc("8313897726796333469433856")*Power(nu,11) + mpfc("233850035775714098657280")*Power(nu,12) + mpfc("5529188695761456380928")*Power(nu,13) + mpfc("109647435479661330944")*Power(nu,14) + mpfc("1814901130521687296")*Power(nu,15) + mpfc("24877751100209920")*Power(nu,16) + mpfc("279112400795008")*Power(nu,17) + mpfc("2519721107904")*Power(nu,18) + mpfc("17851850016")*Power(nu,19) + mpfc("95563160")*Power(nu,20) + mpfc("363308")*Power(nu,21) + mpfc("874")*Power(nu,22) + Power(nu,23)) + mpfc("2629575")*Power(lambda,7)*(mpfc("6180839616231734477791684460544000000") + mpfc("4774658832680479228374360659066880000")*nu + mpfc("1750895889440137885414930167889920000")*Power(nu,2) + mpfc("405694038561827782257894381060096000")*Power(nu,3) + mpfc("66694551796404573158413489825382400")*Power(nu,4) + mpfc("8280482751795658544099411468746752")*Power(nu,5) + mpfc("806885629328429106107563433263104")*Power(nu,6) + mpfc("63308685729303043387989329707008")*Power(nu,7) + mpfc("4070938828786407741833023062016")*Power(nu,8) + mpfc("217225541721420688399541993472")*Power(nu,9) + mpfc("9702348484092615092537196544")*Power(nu,10) + mpfc("364830048131556390169509888")*Power(nu,11) + mpfc("11587798227656330850635776")*Power(nu,12) + mpfc("311258677516374487990272")*Power(nu,13) + mpfc("7064252792476715014144")*Power(nu,14) + mpfc("135056051306964953088")*Power(nu,15) + mpfc("2163189645924626176")*Power(nu,16) + mpfc("28785324711340032")*Power(nu,17) + mpfc("314388496305664")*Power(nu,18) + mpfc("2769647008128")*Power(nu,19) + mpfc("19189734256")*Power(nu,20) + mpfc("100649472")*Power(nu,21) + mpfc("375544")*Power(nu,22) + mpfc("888")*Power(nu,23) + Power(nu,24)) + mpfc("736281")*Power(lambda,6)*(mpfc("74170075394780813733500213526528000000") + mpfc("63476745608397485218284012369346560000")*nu + mpfc("25785409505962133853353522673745920000")*Power(nu,2) + mpfc("6619224352182071272509662740611072000")*Power(nu,3) + mpfc("1206028660118682660158856258964684800")*Power(nu,4) + mpfc("166060344817952475687606427450343424")*Power(nu,5) + mpfc("17963110303736807817390172667904000")*Power(nu,6) + mpfc("1566589858080065626763435389747200")*Power(nu,7) + mpfc("112159951674739936289985606451200")*Power(nu,8) + mpfc("6677645329443456002627526983680")*Power(nu,9) + mpfc("333653723530532069509988352000")*Power(nu,10) + mpfc("14080309061671291774571315200")*Power(nu,11) + mpfc("503883626863432360377139200")*Power(nu,12) + mpfc("15322902357852824706519040")*Power(nu,13) + mpfc("396029711026095068160000")*Power(nu,14) + mpfc("8684925408160294451200")*Power(nu,15) + mpfc("161014327058060467200")*Power(nu,16) + mpfc("2508613542460706560")*Power(nu,17) + mpfc("32557986667008000")*Power(nu,18) + mpfc("347624260403200")*Power(nu,19) + mpfc("2999923819200")*Power(nu,20) + mpfc("20397527920")*Power(nu,21) + mpfc("105156000")*Power(nu,22) + mpfc("386200")*Power(nu,23) + mpfc("900")*Power(nu,24) + Power(nu,25)) + mpfc("169911")*Power(lambda,5)*(mpfc("741700753947808137335002135265280000000") + mpfc("708937531478755665916340337219993600000")*nu + mpfc("321330840668018823751819239106805760000")*Power(nu,2) + mpfc("91977653027782846578450150079856640000")*Power(nu,3) + mpfc("18679510953368897874098225330257920000")*Power(nu,4) + mpfc("2866632108298207417034920533468119040")*Power(nu,5) + mpfc("345691447855320553861508154129383424")*Power(nu,6) + mpfc("33629008884537464085024526565376000")*Power(nu,7) + mpfc("2688189374827464989663291454259200")*Power(nu,8) + mpfc("178936404969174496316260876288000")*Power(nu,9) + mpfc("10014182564748776697727410503680")*Power(nu,10) + mpfc("474456814147244987255701504000")*Power(nu,11) + mpfc("19119145330305615378342707200")*Power(nu,12) + mpfc("657112650441960607442329600")*Power(nu,13) + mpfc("19283199468113775388119040")*Power(nu,14) + mpfc("482878965107698012672000")*Power(nu,15) + mpfc("10295068678740899123200")*Power(nu,16) + mpfc("186100462482667532800")*Power(nu,17) + mpfc("2834193409130786560")*Power(nu,18) + mpfc("36034229271040000")*Power(nu,19) + mpfc("377623498595200")*Power(nu,20) + mpfc("3203899098400")*Power(nu,21) + mpfc("21449087920")*Power(nu,22) + mpfc("109018000")*Power(nu,23) + mpfc("395200")*Power(nu,24) + mpfc("910")*Power(nu,25) + Power(nu,26)) + mpfc("31465")*Power(lambda,4)*(mpfc("5933606031582465098680017082122240000000") + mpfc("6413201005777853464665724833025228800000")*nu + mpfc("3279584256822906255930894250074439680000")*Power(nu,2) + mpfc("1057152064890281596379420439745658880000")*Power(nu,3) + mpfc("241413740654734029571235952721920000000")*Power(nu,4) + mpfc("41612567819754557210377589598002872320")*Power(nu,5) + mpfc("5632163691140771847926985766503186432")*Power(nu,6) + mpfc("614723518931620266541704366652391424")*Power(nu,7) + mpfc("55134523883157184002330858199449600")*Power(nu,8) + mpfc("4119680614580860960193378464563200")*Power(nu,9) + mpfc("259049865487164709898080160317440")*Power(nu,10) + mpfc("13809837077926736595773022535680")*Power(nu,11) + mpfc("627409976789689910282443161600")*Power(nu,12) + mpfc("24376046533841300237881344000")*Power(nu,13) + mpfc("811378246186870810547281920")*Power(nu,14) + mpfc("23146231188975359489495040")*Power(nu,15) + mpfc("565239514537625205657600")*Power(nu,16) + mpfc("11783872378602239385600")*Power(nu,17) + mpfc("208774009755713825280")*Power(nu,18) + mpfc("3122467243299106560")*Power(nu,19) + mpfc("39055217259801600")*Power(nu,20) + mpfc("403254691382400")*Power(nu,21) + mpfc("3375491801760")*Power(nu,22) + mpfc("22321231920")*Power(nu,23) + mpfc("112179600")*Power(nu,24) + mpfc("402480")*Power(nu,25) + mpfc("918")*Power(nu,26) + Power(nu,27)) + mpfc("4495")*Power(lambda,3)*(mpfc("35601636189494790592080102492733440000000") + mpfc("44412812066249585886674366080273612800000")*nu + mpfc("26090706546715291000251090333471866880000")*Power(nu,2) + mpfc("9622496646164595834207416888548392960000")*Power(nu,3) + mpfc("2505634508818685773806836156077178880000")*Power(nu,4) + mpfc("491089147573261372833501490309937233920")*Power(nu,5) + mpfc("75405549966599188297939504197021990912")*Power(nu,6) + mpfc("9320504804730493447177211966417534976")*Power(nu,7) + mpfc("945530662230563370555689515849089024")*Power(nu,8) + mpfc("79852607570642349763491128986828800")*Power(nu,9) + mpfc("5673979807503849219581859426467840")*Power(nu,10) + mpfc("341908887954725129472718295531520")*Power(nu,11) + mpfc("17574296938664876057467681505280")*Power(nu,12) + mpfc("773666255992737711709731225600")*Power(nu,13) + mpfc("29244316010962525101165035520")*Power(nu,14) + mpfc("950255633320722967484252160")*Power(nu,15) + mpfc("26537668276201110723440640")*Power(nu,16) + mpfc("635942748809238641971200")*Power(nu,17) + mpfc("13036516437136522337280")*Power(nu,18) + mpfc("227508813215508464640")*Power(nu,19) + mpfc("3356798546857916160")*Power(nu,20) + mpfc("41474745408096000")*Power(nu,21) + mpfc("423507642192960")*Power(nu,22) + mpfc("3509419193280")*Power(nu,23) + mpfc("22994309520")*Power(nu,24) + mpfc("114594480")*Power(nu,25) + mpfc("407988")*Power(nu,26) + mpfc("924")*Power(nu,27) + Power(nu,28)) + mpfc("465")*Power(lambda,2)*(mpfc("142406544757979162368320409970933760000000") + mpfc("213252884454493134138777566813827891200000")*nu + mpfc("148775638253110749887678727414161080320000")*Power(nu,2) + mpfc("64580693131373674337080757887665438720000")*Power(nu,3) + mpfc("19645034681439338929434761512857108480000")*Power(nu,4) + mpfc("4469991099111731265140842117316927815680")*Power(nu,5) + mpfc("792711347439658126025259507098025197568")*Power(nu,6) + mpfc("112687569185521162086648352062692130816")*Power(nu,7) + mpfc("13102627453652746929399970029813891072")*Power(nu,8) + mpfc("1264941092513132769609654031796404224")*Power(nu,9) + mpfc("102548526800657746641818566692700160")*Power(nu,10) + mpfc("7041615359322749737472732608593920")*Power(nu,11) + mpfc("412206075709384633702589021552640")*Power(nu,12) + mpfc("20668961962635826904306606407680")*Power(nu,13) + mpfc("890643520036587812114391367680")*Power(nu,14) + mpfc("33045338544245416971102044160")*Power(nu,15) + mpfc("1056406306425527410378014720")*Power(nu,16) + mpfc("29081439271438065291325440")*Power(nu,17) + mpfc("688088814557784731320320")*Power(nu,18) + mpfc("13946551689998556195840")*Power(nu,19) + mpfc("240936007402940129280")*Power(nu,20) + mpfc("3522697528490300160")*Power(nu,21) + mpfc("43168775976867840")*Power(nu,22) + mpfc("437545318966080")*Power(nu,23) + mpfc("3601396431360")*Power(nu,24) + mpfc("23452687440")*Power(nu,25) + mpfc("116226432")*Power(nu,26) + mpfc("411684")*Power(nu,27) + mpfc("928")*Power(nu,28) + Power(nu,29)) + mpfc("31")*lambda*(mpfc("284813089515958324736640819941867520000000") + mpfc("568912313666965430645875543598589542400000")*nu + mpfc("510804160960714633914135021642150051840000")*Power(nu,2) + mpfc("277937024515858098561840243189491957760000")*Power(nu,3) + mpfc("103870762494252352195950280913379655680000")*Power(nu,4) + mpfc("28585016879662801459716445747490964111360")*Power(nu,5) + mpfc("6055413793991047517191361131512978210816")*Power(nu,6) + mpfc("1018086485810700450198556211223409459200")*Power(nu,7) + mpfc("138892824092826655945448292122319912960")*Power(nu,8) + mpfc("15632509638679012468619278093406699520")*Power(nu,9) + mpfc("1470038146114448262893291165181804544")*Power(nu,10) + mpfc("116631757519303246116764031909888000")*Power(nu,11) + mpfc("7866027510741519004877910651699200")*Power(nu,12) + mpfc("453543999634656287511202234368000")*Power(nu,13) + mpfc("22450249002709002528535389143040")*Power(nu,14) + mpfc("956734197125078646056595456000")*Power(nu,15) + mpfc("35158151157096471791858073600")*Power(nu,16) + mpfc("1114569184968403540960665600")*Power(nu,17) + mpfc("30457616900553634753966080")*Power(nu,18) + mpfc("715981917937781843712000")*Power(nu,19) + mpfc("14428423704804436454400")*Power(nu,20) + mpfc("247981402459920729600")*Power(nu,21) + mpfc("3609035080444035840")*Power(nu,22) + mpfc("44043866614800000")*Power(nu,23) + mpfc("444748111828800")*Power(nu,24) + mpfc("3648301806240")*Power(nu,25) + mpfc("23685140304")*Power(nu,26) + mpfc("117049800")*Power(nu,27) + mpfc("413540")*Power(nu,28) + mpfc("930")*Power(nu,29) + Power(nu,30)) + nu*(mpfc("284813089515958324736640819941867520000000") + mpfc("568912313666965430645875543598589542400000")*nu + mpfc("510804160960714633914135021642150051840000")*Power(nu,2) + mpfc("277937024515858098561840243189491957760000")*Power(nu,3) + mpfc("103870762494252352195950280913379655680000")*Power(nu,4) + mpfc("28585016879662801459716445747490964111360")*Power(nu,5) + mpfc("6055413793991047517191361131512978210816")*Power(nu,6) + mpfc("1018086485810700450198556211223409459200")*Power(nu,7) + mpfc("138892824092826655945448292122319912960")*Power(nu,8) + mpfc("15632509638679012468619278093406699520")*Power(nu,9) + mpfc("1470038146114448262893291165181804544")*Power(nu,10) + mpfc("116631757519303246116764031909888000")*Power(nu,11) + mpfc("7866027510741519004877910651699200")*Power(nu,12) + mpfc("453543999634656287511202234368000")*Power(nu,13) + mpfc("22450249002709002528535389143040")*Power(nu,14) + mpfc("956734197125078646056595456000")*Power(nu,15) + mpfc("35158151157096471791858073600")*Power(nu,16) + mpfc("1114569184968403540960665600")*Power(nu,17) + mpfc("30457616900553634753966080")*Power(nu,18) + mpfc("715981917937781843712000")*Power(nu,19) + mpfc("14428423704804436454400")*Power(nu,20) + mpfc("247981402459920729600")*Power(nu,21) + mpfc("3609035080444035840")*Power(nu,22) + mpfc("44043866614800000")*Power(nu,23) + mpfc("444748111828800")*Power(nu,24) + mpfc("3648301806240")*Power(nu,25) + mpfc("23685140304")*Power(nu,26) + mpfc("117049800")*Power(nu,27) + mpfc("413540")*Power(nu,28) + mpfc("930")*Power(nu,29) + Power(nu,30)));
        moment(32, Power(lambda,32) + mpfc("32")*Power(lambda,31)*(mpfc("62") + nu) + mpfc("496")*Power(lambda,30)*(mpfc("3720") + mpfc("122")*nu + Power(nu,2)) + mpfc("4960")*Power(lambda,29)*(mpfc("215760") + mpfc("10796")*nu + mpfc("180")*Power(nu,2) + Power(nu,3)) + mpfc("35960")*Power(lambda,28)*(mpfc("12082560") + mpfc("820336")*nu + mpfc("20876")*Power(nu,2) + mpfc("236")*Power(nu,3) + Power(nu,4)) + mpfc("201376")*Power(lambda,27)*(mpfc("652458240") + mpfc("56380704")*nu + mpfc("1947640")*Power(nu,2) + mpfc("33620")*Power(nu,3) + mpfc("290")*Power(nu,4) + Power(nu,5)) + mpfc("906192")*Power(lambda,26)*(mpfc("33927828480") + mpfc("3584254848")*nu + mpfc("157657984")*Power(nu,2) + mpfc("3695880")*Power(nu,3) + mpfc("48700")*Power(nu,4) + mpfc("342")*Power(nu,5) + Power(nu,6)) + mpfc("3365856")*Power(lambda,25)*(mpfc("1696391424000") + mpfc("213140570880")*nu + mpfc("11467154048")*Power(nu,2) + mpfc("342451984")*Power(nu,3) + mpfc("6130880")*Power(nu,4) + mpfc("65800")*Power(nu,5) + mpfc("392")*Power(nu,6) + Power(nu,7)) + mpfc("10518300")*Power(lambda,24)*(mpfc("81426788352000") + mpfc("11927138826240")*nu + mpfc("763563965184")*Power(nu,2) + mpfc("27904849280")*Power(nu,3) + mpfc("636734224")*Power(nu,4) + mpfc("9289280")*Power(nu,5) + mpfc("84616")*Power(nu,6) + mpfc("440")*Power(nu,7) + Power(nu,8)) + mpfc("28048800")*Power(lambda,23)*(mpfc("3745632264192000") + mpfc("630075174359040")*nu + mpfc("47051081224704")*Power(nu,2) + mpfc("2047187032064")*Power(nu,3) + mpfc("57194623584")*Power(nu,4) + mpfc("1064041104")*Power(nu,5) + mpfc("13181616")*Power(nu,6) + mpfc("104856")*Power(nu,7) + mpfc("486")*Power(nu,8) + Power(nu,9)) + mpfc("64512240")*Power(lambda,22)*(mpfc("164807819624448000") + mpfc("31468939935989760")*nu + mpfc("2700322748246016")*Power(nu,2) + mpfc("137127310635520")*Power(nu,3) + mpfc("4563750469760")*Power(nu,4) + mpfc("104012432160")*Power(nu,5) + mpfc("1644032208")*Power(nu,6) + mpfc("17795280")*Power(nu,7) + mpfc("126240")*Power(nu,8) + mpfc("530")*Power(nu,9) + Power(nu,10)) + mpfc("129024480")*Power(lambda,21)*(mpfc("6921928424226816000") + mpfc("1486503296936017920")*nu + mpfc("144882495362322432")*Power(nu,2) + mpfc("8459669794937856")*Power(nu,3) + mpfc("328804830365440")*Power(nu,4) + mpfc("8932272620480")*Power(nu,5) + mpfc("173061784896")*Power(nu,6) + mpfc("2391433968")*Power(nu,7) + mpfc("23097360")*Power(nu,8) + mpfc("148500")*Power(nu,9) + mpfc("572")*Power(nu,10) + Power(nu,11)) + mpfc("225792840")*Power(lambda,20)*(mpfc("276877136969072640000") + mpfc("66382060301667532800")*nu + mpfc("7281803111428915200")*Power(nu,2) + mpfc("483269287159836672")*Power(nu,3) + mpfc("21611863009555456")*Power(nu,4) + mpfc("686095735184640")*Power(nu,5) + mpfc("15854744016320")*Power(nu,6) + mpfc("268719143616")*Power(nu,7) + mpfc("3315328368")*Power(nu,8) + mpfc("29037360")*Power(nu,9) + mpfc("171380")*Power(nu,10) + mpfc("612")*Power(nu,11) + Power(nu,12)) + mpfc("347373600")*Power(lambda,19)*(mpfc("10521331204824760320000") + mpfc("2799395428432438886400")*nu + mpfc("343090578535966310400")*Power(nu,2) + mpfc("25646036023502708736")*Power(nu,3) + mpfc("1304520081522944000")*Power(nu,4) + mpfc("47683500946571776")*Power(nu,5) + mpfc("1288576007804800")*Power(nu,6) + mpfc("26066071473728")*Power(nu,7) + mpfc("394701621600")*Power(nu,8) + mpfc("4418748048")*Power(nu,9) + mpfc("35549800")*Power(nu,10) + mpfc("194636")*Power(nu,11) + mpfc("650")*Power(nu,12) + Power(nu,13)) + mpfc("471435600")*Power(lambda,18)*(mpfc("378767923373691371520000") + mpfc("111299566628392560230400")*nu + mpfc("15150656255727226060800")*Power(nu,2) + mpfc("1266347875382063824896")*Power(nu,3) + mpfc("72608758958328692736")*Power(nu,4) + mpfc("3021126115599527936")*Power(nu,5) + mpfc("94072237227544576")*Power(nu,6) + mpfc("2226954580859008")*Power(nu,7) + mpfc("40275329851328")*Power(nu,8) + mpfc("553776551328")*Power(nu,9) + mpfc("5698540848")*Power(nu,10) + mpfc("42556696")*Power(nu,11) + mpfc("218036")*Power(nu,12) + mpfc("686")*Power(nu,13) + Power(nu,14)) + mpfc("565722720")*Power(lambda,17)*(mpfc("12878109394705506631680000") + mpfc("4162953188739038419353600")*nu + mpfc("626421879323118246297600")*Power(nu,2) + mpfc("58206484018717396107264")*Power(nu,3) + mpfc("3735045679965239377920")*Power(nu,4) + mpfc("175327046888712642560")*Power(nu,5) + mpfc("6219582181336043520")*Power(nu,6) + mpfc("169788692976750848")*Power(nu,7) + mpfc("3596315795804160")*Power(nu,8) + mpfc("59103732596480")*Power(nu,9) + mpfc("747526940160")*Power(nu,10) + mpfc("7145468512")*Power(nu,11) + mpfc("49969920")*Power(nu,12) + mpfc("241360")*Power(nu,13) + mpfc("720")*Power(nu,14) + Power(nu,15)) + mpfc("601080390")*Power(lambda,16)*(mpfc("412099500630576212213760000") + mpfc("146092611434354736050995200")*nu + mpfc("24208453327078822300876800")*Power(nu,2) + mpfc("2489029367922074921730048")*Power(nu,3) + mpfc("177727945777605056200704")*Power(nu,4) + mpfc("9345511180404043939840")*Power(nu,5) + mpfc("374353676691466035200")*Power(nu,6) + mpfc("11652820356592070656")*Power(nu,7) + mpfc("284870798442483968")*Power(nu,8) + mpfc("5487635238891520")*Power(nu,9) + mpfc("83024594681600")*Power(nu,10) + mpfc("976181932544")*Power(nu,11) + mpfc("8744505952")*Power(nu,12) + mpfc("57693440")*Power(nu,13) + mpfc("264400")*Power(nu,14) + mpfc("752")*Power(nu,15) + Power(nu,16)) + mpfc("565722720")*Power(lambda,15)*(mpfc("12362985018917286366412800000") + mpfc("4794877843661218293743616000")*nu + mpfc("872346211246719405077299200")*Power(nu,2) + mpfc("98879334364741069952778240")*Power(nu,3) + mpfc("7820867741250226607751168")*Power(nu,4) + mpfc("458093281189726374395904")*Power(nu,5) + mpfc("20576121481148024995840")*Power(nu,6) + mpfc("723938287389228154880")*Power(nu,7) + mpfc("20198944309866589696")*Power(nu,8) + mpfc("449499855609229568")*Power(nu,9) + mpfc("7978373079339520")*Power(nu,10) + mpfc("112310052657920")*Power(nu,11) + mpfc("1238517111104")*Power(nu,12) + mpfc("10475309152")*Power(nu,13) + mpfc("65625440")*Power(nu,14) + mpfc("286960")*Power(nu,15) + mpfc("782")*Power(nu,16) + Power(nu,17)) + mpfc("471435600")*Power(lambda,14)*(mpfc("346163580529684018259558400000") + mpfc("146619564641431398591234048000")*nu + mpfc("29220571758569361635907993600")*Power(nu,2) + mpfc("3640967573459469363755089920")*Power(nu,3) + mpfc("317863631119747414969810944")*Power(nu,4) + mpfc("20647479614562565090836480")*Power(nu,5) + mpfc("1034224682661871074279424")*Power(nu,6) + mpfc("40846393528046413332480")*Power(nu,7) + mpfc("1289508728065492666368")*Power(nu,8) + mpfc("32784940266925017600")*Power(nu,9) + mpfc("672894301830736128")*Power(nu,10) + mpfc("11123054553761280")*Power(nu,11) + mpfc("146988531768832")*Power(nu,12) + mpfc("1531825767360")*Power(nu,13) + mpfc("12312821472")*Power(nu,14) + mpfc("73660320")*Power(nu,15) + mpfc("308856")*Power(nu,16) + mpfc("810")*Power(nu,17) + Power(nu,18)) + mpfc("347373600")*Power(lambda,13)*(mpfc("9000253093771784474748518400000") + mpfc("4158272261206900381631643648000")*nu + mpfc("906354430364234801124841881600")*Power(nu,2) + mpfc("123885728668515565093540331520")*Power(nu,3) + mpfc("11905421982572902152970174464")*Power(nu,4) + mpfc("854698101098374107331559424")*Power(nu,5) + mpfc("47537321363771213022101504")*Power(nu,6) + mpfc("2096230914391077820923904")*Power(nu,7) + mpfc("74373620457749222658048")*Power(nu,8) + mpfc("2141917175005543123968")*Power(nu,9) + mpfc("50280192114524156928")*Power(nu,10) + mpfc("962093720228529408")*Power(nu,11) + mpfc("14944756379750912")*Power(nu,12) + mpfc("186816001720192")*Power(nu,13) + mpfc("1851959125632")*Power(nu,14) + mpfc("14227989792")*Power(nu,15) + mpfc("81690576")*Power(nu,16) + mpfc("329916")*Power(nu,17) + mpfc("836")*Power(nu,18) + Power(nu,19)) + mpfc("225792840")*Power(lambda,12)*(mpfc("216006074250522827393964441600000") + mpfc("108798787362737393633907965952000")*nu + mpfc("25910778589948535608627848806400")*Power(nu,2) + mpfc("3879611918408608363369809838080")*Power(nu,3) + mpfc("409615856250265216764824518656")*Power(nu,4) + mpfc("32418176408933880728927600640")*Power(nu,5) + mpfc("1995593813828883219861995520")*Power(nu,6) + mpfc("97846863309157080724275200")*Power(nu,7) + mpfc("3881197805377059164717056")*Power(nu,8) + mpfc("125779632657882257633280")*Power(nu,9) + mpfc("3348641785754122890240")*Power(nu,10) + mpfc("73370441400008862720")*Power(nu,11) + mpfc("1320767873342551296")*Power(nu,12) + mpfc("19428340421035520")*Power(nu,13) + mpfc("231263020735360")*Power(nu,14) + mpfc("2193430880640")*Power(nu,15) + mpfc("16188563616")*Power(nu,16) + mpfc("89608560")*Power(nu,17) + mpfc("349980")*Power(nu,18) + mpfc("860")*Power(nu,19) + Power(nu,20)) + mpfc("129024480")*Power(lambda,11)*(mpfc("4752133633511502202667217715200000") + mpfc("2609579396230745487339939692544000")*nu + mpfc("678835916341605177023720639692800")*Power(nu,2) + mpfc("111262240794937919602763665244160")*Power(nu,3) + mpfc("12891160755914443132195949248512")*Power(nu,4) + mpfc("1122815737246810592801231732736")*Power(nu,5) + mpfc("76321240313169311565891502080")*Power(nu,6) + mpfc("4148224806630338995796049920")*Power(nu,7) + mpfc("183233215027452382348050432")*Power(nu,8) + mpfc("6648349723850468832649216")*Power(nu,9) + mpfc("199449751944472961218560")*Power(nu,10) + mpfc("4962791496554317870080")*Power(nu,11) + mpfc("102427334613544991232")*Power(nu,12) + mpfc("1748191362605332736")*Power(nu,13) + mpfc("24516126877213440")*Power(nu,14) + mpfc("279518500109440")*Power(nu,15) + mpfc("2549579280192")*Power(nu,16) + mpfc("18159951936")*Power(nu,17) + mpfc("97308120")*Power(nu,18) + mpfc("368900")*Power(nu,19) + mpfc("882")*Power(nu,20) + Power(nu,21)) + mpfc("64512240")*Power(lambda,10)*(mpfc("95042672670230044053344354304000000") + mpfc("56943721558126411949466011566080000")*nu + mpfc("16186297723062849027814352486400000")*Power(nu,2) + mpfc("2904080732240363569078993944576000")*Power(nu,3) + mpfc("369085455913226782246682650214400")*Power(nu,4) + mpfc("35347475500850654988220583903232")*Power(nu,5) + mpfc("2649240543510196824119061774336")*Power(nu,6) + mpfc("159285736445776091481812500480")*Power(nu,7) + mpfc("7812889107179386642757058560")*Power(nu,8) + mpfc("316200209504461759001034752")*Power(nu,9) + mpfc("10637344762739928057020416")*Power(nu,10) + mpfc("298705581875559318620160")*Power(nu,11) + mpfc("7011338188825217694720")*Power(nu,12) + mpfc("137391161865651645952")*Power(nu,13) + mpfc("2238513900149601536")*Power(nu,14) + mpfc("30106496879402240")*Power(nu,15) + mpfc("330510085713280")*Power(nu,16) + mpfc("2912778318912")*Power(nu,17) + mpfc("20106114336")*Power(nu,18) + mpfc("104686120")*Power(nu,19) + mpfc("386540")*Power(nu,20) + mpfc("902")*Power(nu,21) + Power(nu,22)) + mpfc("28048800")*Power(lambda,9)*(mpfc("1710768108064140792960198377472000000") + mpfc("1120029660716505459143732562493440000")*nu + mpfc("348297080573257694450124356321280000")*Power(nu,2) + mpfc("68459750903389393271236243488768000")*Power(nu,3) + mpfc("9547618938678445649519281648435200")*Power(nu,4) + mpfc("1005340014928538572034653160472576")*Power(nu,5) + mpfc("83033805284034197822363695841280")*Power(nu,6) + mpfc("5516383799534166470791686782976")*Power(nu,7) + mpfc("299917740375005051051439554560")*Power(nu,8) + mpfc("13504492878259698304775684096")*Power(nu,9) + mpfc("507672415233780464027402240")*Power(nu,10) + mpfc("16014045236499995792183296")*Power(nu,11) + mpfc("424909669274413237125120")*Power(nu,12) + mpfc("9484379102406947321856")*Power(nu,13) + mpfc("177684412068344473600")*Power(nu,14) + mpfc("2780430843978841856")*Power(nu,15) + mpfc("36055678422241280")*Power(nu,16) + mpfc("382940095453696")*Power(nu,17) + mpfc("3274688376960")*Power(nu,18) + mpfc("21990464496")*Power(nu,19) + mpfc("111643840")*Power(nu,20) + mpfc("402776")*Power(nu,21) + mpfc("920")*Power(nu,22) + Power(nu,23)) + mpfc("10518300")*Power(lambda,8)*(mpfc("27372289729026252687363174039552000000") + mpfc("19631242679528228139259919377367040000")*nu + mpfc("6692782949888628570345722263633920000")*Power(nu,2) + mpfc("1443653095027487986789904252141568000")*Power(nu,3) + mpfc("221221653922244523663544749863731200")*Power(nu,4) + mpfc("25633059177535062802073732215996416")*Power(nu,5) + mpfc("2333880899473085737192472293933056")*Power(nu,6) + mpfc("171295946076580861355030684368896")*Power(nu,7) + mpfc("10315067645534247287614719655936")*Power(nu,8) + mpfc("515989626427160223927850500096")*Power(nu,9) + mpfc("21627251522000185729214119936")*Power(nu,10) + mpfc("763897139017780396702334976")*Power(nu,11) + mpfc("22812599944890607586185216")*Power(nu,12) + mpfc("576659734912924394274816")*Power(nu,13) + mpfc("12327329695500458899456")*Power(nu,14) + mpfc("222171305572005943296")*Power(nu,15) + mpfc("3357321698734702336")*Power(nu,16) + mpfc("42182719949500416")*Power(nu,17) + mpfc("435335109485056")*Power(nu,18) + mpfc("3626535808896")*Power(nu,19) + mpfc("23776765936")*Power(nu,20) + mpfc("118088256")*Power(nu,21) + mpfc("417496")*Power(nu,22) + mpfc("936")*Power(nu,23) + Power(nu,24)) + mpfc("3365856")*Power(lambda,7)*(mpfc("383212056206367537623084436553728000000") + mpfc("302209687242421446637002045322690560000")*nu + mpfc("113330203977969028124100031068241920000")*Power(nu,2) + mpfc("26903926280273460385404381793615872000")*Power(nu,3) + mpfc("4540756249938911318079530750233804800")*Power(nu,4) + mpfc("580084482407735402892577000887681024")*Power(nu,5) + mpfc("58307391770158263122768344331059200")*Power(nu,6) + mpfc("4732024144545217796162901875097600")*Power(nu,7) + mpfc("315706893114060323381636759552000")*Power(nu,8) + mpfc("17538922415514490422604626657280")*Power(nu,9) + mpfc("818771147735162824136848179200")*Power(nu,10) + mpfc("32321811468249111283046809600")*Power(nu,11) + mpfc("1083273538246248902908928000")*Power(nu,12) + mpfc("30885836233671549106032640")*Power(nu,13) + mpfc("749242350649930818867200")*Power(nu,14) + mpfc("15437727973508542105600")*Power(nu,15) + mpfc("269173809354291776000")*Power(nu,16) + mpfc("3947879778027708160")*Power(nu,17) + mpfc("48277411482291200")*Power(nu,18) + mpfc("486106610809600")*Power(nu,19) + mpfc("3959410532000")*Power(nu,20) + mpfc("25430001520")*Power(nu,21) + mpfc("123933200")*Power(nu,22) + mpfc("430600")*Power(nu,23) + mpfc("950")*Power(nu,24) + Power(nu,25)) + mpfc("906192")*Power(lambda,6)*(mpfc("4598544674476410451477013238644736000000") + mpfc("4009728303115424897267108980426014720000")*nu + mpfc("1662172134978049784126202418141593600000")*Power(nu,2) + mpfc("436177319341250552748952612591632384000")*Power(nu,3) + mpfc("81393001279540396202358750796421529600")*Power(nu,4) + mpfc("11501770038831736152790454760885977088")*Power(nu,5) + mpfc("1279773183649634560365797132860391424")*Power(nu,6) + mpfc("115091681504700876676723166832230400")*Power(nu,7) + mpfc("8520506861913941676742542989721600")*Power(nu,8) + mpfc("526173962100234208452892279439360")*Power(nu,9) + mpfc("27364176188336444312246804807680")*Power(nu,10) + mpfc("1206632885354152159533409894400")*Power(nu,11) + mpfc("45321093927204098117953945600")*Power(nu,12) + mpfc("1453903573050307492181319680")*Power(nu,13) + mpfc("39876744441470718932439040")*Power(nu,14) + mpfc("934495086332033324134400")*Power(nu,15) + mpfc("18667813685760043417600")*Power(nu,16) + mpfc("316548366690624273920")*Power(nu,17) + mpfc("4527208715815202560")*Power(nu,18) + mpfc("54110690812006400")*Power(nu,19) + mpfc("533619537193600")*Power(nu,20) + mpfc("4264570550240")*Power(nu,21) + mpfc("26917199920")*Power(nu,22) + mpfc("129100400")*Power(nu,23) + mpfc("442000")*Power(nu,24) + mpfc("962")*Power(nu,25) + Power(nu,26)) + mpfc("201376")*Power(lambda,5)*(mpfc("45985446744764104514770132386447360000000") + mpfc("44695827705630659424148103042904883200000")*nu + mpfc("20631449652895922738529133161841950720000")*Power(nu,2) + mpfc("6023945328390555311615728544057917440000")*Power(nu,3) + mpfc("1250107332136654514772540120555847680000")*Power(nu,4) + mpfc("196410701667857757730263298405281300480")*Power(nu,5) + mpfc("24299501875328081756448426089489891328")*Power(nu,6) + mpfc("2430689998696643327133028801182695424")*Power(nu,7) + mpfc("200296750123840293444148596729446400")*Power(nu,8) + mpfc("13782246482916283761271465784115200")*Power(nu,9) + mpfc("799815723983598651575360327516160")*Power(nu,10) + mpfc("39430505041877965907580903751680")*Power(nu,11) + mpfc("1659843824626193140712949350400")*Power(nu,12) + mpfc("59860129657707173039767142400")*Power(nu,13) + mpfc("1852671017465014681505710080")*Power(nu,14) + mpfc("49221695304791052173783040")*Power(nu,15) + mpfc("1121173223189633758310400")*Power(nu,16) + mpfc("21833297352666286156800")*Power(nu,17) + mpfc("361820453848776299520")*Power(nu,18) + mpfc("5068315623935266560")*Power(nu,19) + mpfc("59446886183942400")*Power(nu,20) + mpfc("576265242696000")*Power(nu,21) + mpfc("4533742549440")*Power(nu,22) + mpfc("28208203920")*Power(nu,23) + mpfc("133520400")*Power(nu,24) + mpfc("451620")*Power(nu,25) + mpfc("972")*Power(nu,26) + Power(nu,27)) + mpfc("35960")*Power(lambda,4)*(mpfc("367883573958112836118161059091578880000000") + mpfc("403552068389809379907954956729686425600000")*nu + mpfc("209747424928798041332381168337640488960000")*Power(nu,2) + mpfc("68823012280020365231454961514305290240000")*Power(nu,3) + mpfc("16024803985483791429796049508504698880000")*Power(nu,4) + mpfc("2821392945479516576614646507798098083840")*Power(nu,5) + mpfc("390806716670482411781850707121200431104")*Power(nu,6) + mpfc("43745021864901228373512656498951454720")*Power(nu,7) + mpfc("4033063999687365674686217575018266624")*Power(nu,8) + mpfc("310554721987170563534320323002368000")*Power(nu,9) + mpfc("20180772274785072973874348404244480")*Power(nu,10) + mpfc("1115259764318622378836007557529600")*Power(nu,11) + mpfc("52709255638887511033284498554880")*Power(nu,12) + mpfc("2138724861887850525031086489600")*Power(nu,13) + mpfc("74681497797427290491812823040")*Power(nu,14) + mpfc("2246444579903343098895974400")*Power(nu,15) + mpfc("58191081090308122240266240")*Power(nu,16) + mpfc("1295839602010964047564800")*Power(nu,17) + mpfc("24727860983456496552960")*Power(nu,18) + mpfc("402366978840258432000")*Power(nu,19) + mpfc("5543890713406805760")*Power(nu,20) + mpfc("64057008125510400")*Power(nu,21) + mpfc("612535183091520")*Power(nu,22) + mpfc("4759408180800")*Power(nu,23) + mpfc("29276367120")*Power(nu,24) + mpfc("137133360")*Power(nu,25) + mpfc("459396")*Power(nu,26) + mpfc("980")*Power(nu,27) + Power(nu,28)) + mpfc("4960")*Power(lambda,3)*(mpfc("2207301443748677016708966354549473280000000") + mpfc("2789195984296969115565890799469697433600000")*nu + mpfc("1662036617962597627902241966755529359360000")*Power(nu,2) + mpfc("622685498608920232721110937423472230400000")*Power(nu,3) + mpfc("164971836192923113810231258565333483520000")*Power(nu,4) + mpfc("32953161658360890889483928555293287383040")*Power(nu,5) + mpfc("5166233245502411047305750750525300670464")*Power(nu,6) + mpfc("653276847859889782022926646114909159424")*Power(nu,7) + mpfc("67943405863025422421629961949061054464")*Power(nu,8) + mpfc("5896392331610389055892139513032474624")*Power(nu,9) + mpfc("431639355635881001377566413427834880")*Power(nu,10) + mpfc("26872330860696807246890393749422080")*Power(nu,11) + mpfc("1431515298151947445035714548858880")*Power(nu,12) + mpfc("65541604810214614183471017492480")*Power(nu,13) + mpfc("2586813848672414267981963427840")*Power(nu,14) + mpfc("88160165276847349085188669440")*Power(nu,15) + mpfc("2595591066445191832337571840")*Power(nu,16) + mpfc("65966118702373906525655040")*Power(nu,17) + mpfc("1444206767911703026882560")*Power(nu,18) + mpfc("27142062856498047144960")*Power(nu,19) + mpfc("435630323120699266560")*Power(nu,20) + mpfc("5928232762159868160")*Power(nu,21) + mpfc("67732219224059520")*Power(nu,22) + mpfc("641091632176320")*Power(nu,23) + mpfc("4935066383520")*Power(nu,24) + mpfc("30099167280")*Power(nu,25) + mpfc("139889736")*Power(nu,26) + mpfc("465276")*Power(nu,27) + mpfc("986")*Power(nu,28) + Power(nu,29)) + mpfc("496")*Power(lambda,2)*(mpfc("8829205774994708066835865418197893120000000") + mpfc("13364085380936553478972529552428263014400000")*nu + mpfc("9437342456147359627174858666491814871040000")*Power(nu,2) + mpfc("4152778612398278558786685716449418280960000")*Power(nu,3) + mpfc("1282572843380612687962035971684806164480000")*Power(nu,4) + mpfc("296784482826366677368166972786506633052160")*Power(nu,5) + mpfc("53618094640370535078706931557394490064896")*Power(nu,6) + mpfc("7779340636941970175397457334984937308160")*Power(nu,7) + mpfc("925050471311991471709446493911153377280")*Power(nu,8) + mpfc("91528975189466978645198520001190952960")*Power(nu,9) + mpfc("7622949754153913061402405166743814144")*Power(nu,10) + mpfc("539128679078668230365127988425523200")*Power(nu,11) + mpfc("32598392053304597027033251944857600")*Power(nu,12) + mpfc("1693681717392805901769598618828800")*Power(nu,13) + mpfc("75888860204904271255398871203840")*Power(nu,14) + mpfc("2939454509779803664322718105600")*Power(nu,15) + mpfc("98542529542628116414538956800")*Power(nu,16) + mpfc("2859455541254687458440192000")*Power(nu,17) + mpfc("71742945774020718633185280")*Power(nu,18) + mpfc("1552775019337695215462400")*Power(nu,19) + mpfc("28884584148980844211200")*Power(nu,20) + mpfc("459343254169338739200")*Power(nu,21) + mpfc("6199161639056106240")*Power(nu,22) + mpfc("70296585752764800")*Power(nu,23) + mpfc("660831897710400")*Power(nu,24) + mpfc("5055463052640")*Power(nu,25) + mpfc("30658726224")*Power(nu,26) + mpfc("141750840")*Power(nu,27) + mpfc("469220")*Power(nu,28) + mpfc("990")*Power(nu,29) + Power(nu,30)) + mpfc("32")*lambda*(mpfc("17658411549989416133671730836395786240000000") + mpfc("35557376536867815024780924523054419148800000")*nu + mpfc("32238770293231272733322246885411892756480000")*Power(nu,2) + mpfc("17742899680943916744748230099390651432960000")*Power(nu,3) + mpfc("6717924299159503934710757659819030609920000")*Power(nu,4) + mpfc("1876141809033346042698369917257819430584320")*Power(nu,5) + mpfc("404020672107107747525580835901295613181952")*Power(nu,6) + mpfc("69176775914254475429501846227364364681216")*Power(nu,7) + mpfc("9629441579565953118816350322807244062720")*Power(nu,8) + mpfc("1108108421690925428999843533913535283200")*Power(nu,9) + mpfc("106774874697774804768003330334678581248")*Power(nu,10) + mpfc("8701207112311249522132661143594860544")*Power(nu,11) + mpfc("604325463185277424419194492315238400")*Power(nu,12) + mpfc("35985755488090208830572449182515200")*Power(nu,13) + mpfc("1845459437802614444280396361236480")*Power(nu,14) + mpfc("81767769224463878584044307415040")*Power(nu,15) + mpfc("3136539568865059897151796019200")*Power(nu,16) + mpfc("104261440625137491331419340800")*Power(nu,17) + mpfc("3002941432802728895706562560")*Power(nu,18) + mpfc("74848495812696109064110080")*Power(nu,19) + mpfc("1610544187635656903884800")*Power(nu,20) + mpfc("29803270657319521689600")*Power(nu,21) + mpfc("471741577447450951680")*Power(nu,22) + mpfc("6339754810561635840")*Power(nu,23) + mpfc("71618249548185600")*Power(nu,24) + mpfc("670942823815680")*Power(nu,25) + mpfc("5116780505088")*Power(nu,26) + mpfc("30942227904")*Power(nu,27) + mpfc("142689280")*Power(nu,28) + mpfc("471200")*Power(nu,29) + mpfc("992")*Power(nu,30) + Power(nu,31)) + nu*(mpfc("17658411549989416133671730836395786240000000") + mpfc("35557376536867815024780924523054419148800000")*nu + mpfc("32238770293231272733322246885411892756480000")*Power(nu,2) + mpfc("17742899680943916744748230099390651432960000")*Power(nu,3) + mpfc("6717924299159503934710757659819030609920000")*Power(nu,4) + mpfc("1876141809033346042698369917257819430584320")*Power(nu,5) + mpfc("404020672107107747525580835901295613181952")*Power(nu,6) + mpfc("69176775914254475429501846227364364681216")*Power(nu,7) + mpfc("9629441579565953118816350322807244062720")*Power(nu,8) + mpfc("1108108421690925428999843533913535283200")*Power(nu,9) + mpfc("106774874697774804768003330334678581248")*Power(nu,10) + mpfc("8701207112311249522132661143594860544")*Power(nu,11) + mpfc("604325463185277424419194492315238400")*Power(nu,12) + mpfc("35985755488090208830572449182515200")*Power(nu,13) + mpfc("1845459437802614444280396361236480")*Power(nu,14) + mpfc("81767769224463878584044307415040")*Power(nu,15) + mpfc("3136539568865059897151796019200")*Power(nu,16) + mpfc("104261440625137491331419340800")*Power(nu,17) + mpfc("3002941432802728895706562560")*Power(nu,18) + mpfc("74848495812696109064110080")*Power(nu,19) + mpfc("1610544187635656903884800")*Power(nu,20) + mpfc("29803270657319521689600")*Power(nu,21) + mpfc("471741577447450951680")*Power(nu,22) + mpfc("6339754810561635840")*Power(nu,23) + mpfc("71618249548185600")*Power(nu,24) + mpfc("670942823815680")*Power(nu,25) + mpfc("5116780505088")*Power(nu,26) + mpfc("30942227904")*Power(nu,27) + mpfc("142689280")*Power(nu,28) + mpfc("471200")*Power(nu,29) + mpfc("992")*Power(nu,30) + Power(nu,31)));
        moment(33, Power(lambda,33) + mpfc("33")*Power(lambda,32)*(mpfc("64") + nu) + mpfc("528")*Power(lambda,31)*(mpfc("3968") + mpfc("126")*nu + Power(nu,2)) + mpfc("5456")*Power(lambda,30)*(mpfc("238080") + mpfc("11528")*nu + mpfc("186")*Power(nu,2) + Power(nu,3)) + mpfc("40920")*Power(lambda,29)*(mpfc("13808640") + mpfc("906704")*nu + mpfc("22316")*Power(nu,2) + mpfc("244")*Power(nu,3) + Power(nu,4)) + mpfc("237336")*Power(lambda,28)*(mpfc("773283840") + mpfc("64584064")*nu + mpfc("2156400")*Power(nu,2) + mpfc("35980")*Power(nu,3) + mpfc("300")*Power(nu,4) + Power(nu,5)) + mpfc("1107568")*Power(lambda,27)*(mpfc("41757327360") + mpfc("4260823296")*nu + mpfc("181029664")*Power(nu,2) + mpfc("4099320")*Power(nu,3) + mpfc("52180")*Power(nu,4) + mpfc("354")*Power(nu,5) + Power(nu,6)) + mpfc("4272048")*Power(lambda,26)*(mpfc("2171381022720") + mpfc("263320138752")*nu + mpfc("13674365824")*Power(nu,2) + mpfc("394194304")*Power(nu,3) + mpfc("6812680")*Power(nu,4) + mpfc("70588")*Power(nu,5) + mpfc("406")*Power(nu,6) + Power(nu,7)) + mpfc("13884156")*Power(lambda,25)*(mpfc("108569051136000") + mpfc("15337387960320")*nu + mpfc("947038429952")*Power(nu,2) + mpfc("33384081024")*Power(nu,3) + mpfc("734828304")*Power(nu,4) + mpfc("10342080")*Power(nu,5) + mpfc("90888")*Power(nu,6) + mpfc("456")*Power(nu,7) + Power(nu,8)) + mpfc("38567100")*Power(lambda,24)*(mpfc("5211314454528000") + mpfc("844763673231360")*nu + mpfc("60795232598016")*Power(nu,2) + mpfc("2549474319104")*Power(nu,3) + mpfc("68655839616")*Power(nu,4) + mpfc("1231248144")*Power(nu,5) + mpfc("14704704")*Power(nu,6) + mpfc("112776")*Power(nu,7) + mpfc("504")*Power(nu,8) + Power(nu,9)) + mpfc("92561040")*Power(lambda,23)*(mpfc("239720464908288000") + mpfc("44070443423170560")*nu + mpfc("3641344372740096")*Power(nu,2) + mpfc("178071051276800")*Power(nu,3) + mpfc("5707642941440")*Power(nu,4) + mpfc("125293254240")*Power(nu,5) + mpfc("1907664528")*Power(nu,6) + mpfc("19892400")*Power(nu,7) + mpfc("135960")*Power(nu,8) + mpfc("550")*Power(nu,9) + Power(nu,10)) + mpfc("193536720")*Power(lambda,22)*(mpfc("10547700455964672000") + mpfc("2178819975527792640")*nu + mpfc("204289595823734784")*Power(nu,2) + mpfc("11476470628919296")*Power(nu,3) + mpfc("429207340700160")*Power(nu,4) + mpfc("11220546128000")*Power(nu,5) + mpfc("209230493472")*Power(nu,6) + mpfc("2782930128")*Power(nu,7) + mpfc("25874640")*Power(nu,8) + mpfc("160160")*Power(nu,9) + mpfc("594")*Power(nu,10) + Power(nu,11)) + mpfc("354817320")*Power(lambda,21)*(mpfc("443003419150516224000") + mpfc("102058139428131962880")*nu + mpfc("10758983000124653568")*Power(nu,2) + mpfc("686301362238345216")*Power(nu,3) + mpfc("29503178938326016")*Power(nu,4) + mpfc("900470278076160")*Power(nu,5) + mpfc("20008226853824")*Power(nu,6) + mpfc("326113558848")*Power(nu,7) + mpfc("3869665008")*Power(nu,8) + mpfc("32601360")*Power(nu,9) + mpfc("185108")*Power(nu,10) + mpfc("636")*Power(nu,11) + Power(nu,12)) + mpfc("573166440")*Power(lambda,20)*(mpfc("17720136766020648960000") + mpfc("4525328996275794739200")*nu + mpfc("532417459433118105600")*Power(nu,2) + mpfc("38211037489658462208")*Power(nu,3) + mpfc("1866428519771385856")*Power(nu,4) + mpfc("65521990061372416")*Power(nu,5) + mpfc("1700799352229120")*Power(nu,6) + mpfc("33052769207744")*Power(nu,7) + mpfc("480900159168")*Power(nu,8) + mpfc("5173719408")*Power(nu,9) + mpfc("40005680")*Power(nu,10) + mpfc("210548")*Power(nu,11) + mpfc("676")*Power(nu,12) + Power(nu,13)) + mpfc("818809200")*Power(lambda,19)*(mpfc("673365197108784660480000") + mpfc("189682638624500849049600")*nu + mpfc("24757192454734282752000")*Power(nu,2) + mpfc("1984436884040139669504")*Power(nu,3) + mpfc("109135321240971124736")*Power(nu,4) + mpfc("4356264142103537664")*Power(nu,5) + mpfc("130152365446078976")*Power(nu,6) + mpfc("2956804582123392")*Power(nu,7) + mpfc("51326975256128")*Power(nu,8) + mpfc("677501496672")*Power(nu,9) + mpfc("6693935248")*Power(nu,10) + mpfc("48006504")*Power(nu,11) + mpfc("236236")*Power(nu,12) + mpfc("714")*Power(nu,13) + Power(nu,14)) + mpfc("1037158320")*Power(lambda,18)*(mpfc("24241147095916247777280000") + mpfc("7501940187590815226265600")*nu + mpfc("1080941566994935028121600")*Power(nu,2) + mpfc("96196920280179310854144")*Power(nu,3) + mpfc("5913308448715100160000")*Power(nu,4) + mpfc("265960830356698480640")*Power(nu,5) + mpfc("9041749298162380800")*Power(nu,6) + mpfc("236597330402521088")*Power(nu,7) + mpfc("4804575691344000")*Power(nu,8) + mpfc("75717029136320")*Power(nu,9) + mpfc("918483165600")*Power(nu,10) + mpfc("8422169392")*Power(nu,11) + mpfc("56511000")*Power(nu,12) + mpfc("261940")*Power(nu,13) + mpfc("750")*Power(nu,14) + Power(nu,15)) + mpfc("1166803110")*Power(lambda,17)*(mpfc("824199001261152424427520000") + mpfc("279307113474003965470310400")*nu + mpfc("44253953465418606182400000")*Power(nu,2) + mpfc("4351636856521031597162496")*Power(nu,3) + mpfc("297249407536492716294144")*Power(nu,4) + mpfc("14955976680842848501760")*Power(nu,5) + mpfc("573380306494219427840")*Power(nu,6) + mpfc("17086058531848097792")*Power(nu,7) + mpfc("399952903908217088")*Power(nu,8) + mpfc("7378954681978880")*Power(nu,9) + mpfc("106945456766720")*Power(nu,10) + mpfc("1204836924928")*Power(nu,11) + mpfc("10343543392")*Power(nu,12) + mpfc("65416960")*Power(nu,13) + mpfc("287440")*Power(nu,14) + mpfc("784")*Power(nu,15) + Power(nu,16)) + mpfc("1166803110")*Power(lambda,16)*(mpfc("26374368040356877581680640000") + mpfc("9762026632429279319477452800")*nu + mpfc("1695433624367399363307110400")*Power(nu,2) + mpfc("183506332874091617291599872")*Power(nu,3) + mpfc("13863617897688798518575104")*Power(nu,4) + mpfc("775840661323463868350464")*Power(nu,5) + mpfc("33304146488657870192640")*Power(nu,6) + mpfc("1120134179513358557184")*Power(nu,7) + mpfc("29884551456911044608")*Power(nu,8) + mpfc("636079453731541248")*Power(nu,9) + mpfc("10801209298513920")*Power(nu,10) + mpfc("145500238364416")*Power(nu,11) + mpfc("1535830313472")*Power(nu,12) + mpfc("12436886112")*Power(nu,13) + mpfc("74615040")*Power(nu,14) + mpfc("312528")*Power(nu,15) + mpfc("816")*Power(nu,16) + Power(nu,17)) + mpfc("1037158320")*Power(lambda,15)*(mpfc("791231041210706327450419200000") + mpfc("319235167013235257166004224000")*nu + mpfc("60625035363451260218690764800")*Power(nu,2) + mpfc("7200623610590147882055106560")*Power(nu,3) + mpfc("599414869804755572848852992")*Power(nu,4) + mpfc("37138837737392714569089024")*Power(nu,5) + mpfc("1774965055983199974129664")*Power(nu,6) + mpfc("66908171874058626908160")*Power(nu,7) + mpfc("2016670723220689895424")*Power(nu,8) + mpfc("48966935068857282048")*Power(nu,9) + mpfc("960115732686958848")*Power(nu,10) + mpfc("15166216449446400")*Power(nu,11) + mpfc("191575147768576")*Power(nu,12) + mpfc("1908936896832")*Power(nu,13) + mpfc("14675337312")*Power(nu,14) + mpfc("83990880")*Power(nu,15) + mpfc("337008")*Power(nu,16) + mpfc("846")*Power(nu,17) + Power(nu,18)) + mpfc("818809200")*Power(lambda,14)*(mpfc("22154469153899777168611737600000") + mpfc("9729815717581293528098537472000")*nu + mpfc("2016736157189870543289345638400")*Power(nu,2) + mpfc("262242496459975400916233748480")*Power(nu,3) + mpfc("23984239965123303921822990336")*Power(nu,4) + mpfc("1639302326451751580783345664")*Power(nu,5) + mpfc("86837859304922313844719616")*Power(nu,6) + mpfc("3648393868456841527558144")*Power(nu,7) + mpfc("123374952124237943980032")*Power(nu,8) + mpfc("3387744905148693792768")*Power(nu,9) + mpfc("75850175584092129792")*Power(nu,10) + mpfc("1384769793271458048")*Power(nu,11) + mpfc("20530320586966528")*Power(nu,12) + mpfc("245025380879872")*Power(nu,13) + mpfc("2319846341568")*Power(nu,14) + mpfc("17027081952")*Power(nu,15) + mpfc("93427104")*Power(nu,16) + mpfc("360696")*Power(nu,17) + mpfc("874")*Power(nu,18) + Power(nu,19)) + mpfc("573166440")*Power(lambda,13)*(mpfc("576016198001394206383905177600000") + mpfc("275129677811013408899173711872000")*nu + mpfc("62164955804517927653621524070400")*Power(nu,2) + mpfc("8835041065149230967111423098880")*Power(nu,3) + mpfc("885832735553181302883631497216")*Power(nu,4) + mpfc("66606100452868845022189977600")*Power(nu,5) + mpfc("3897086668379731740746055680")*Power(nu,6) + mpfc("181696099884800193561231360")*Power(nu,7) + mpfc("6856142623687028071038976")*Power(nu,8) + mpfc("211456319658103982592000")*Power(nu,9) + mpfc("5359849470335089167360")*Power(nu,10) + mpfc("111854190209150039040")*Power(nu,11) + mpfc("1918558128532587776")*Power(nu,12) + mpfc("26900980489843200")*Power(nu,13) + mpfc("305341385760640")*Power(nu,14) + mpfc("2762550472320")*Power(nu,15) + mpfc("19456186656")*Power(nu,16) + mpfc("102805200")*Power(nu,17) + mpfc("383420")*Power(nu,18) + mpfc("900")*Power(nu,19) + Power(nu,20)) + mpfc("354817320")*Power(lambda,12)*(mpfc("13824388752033460953213724262400000") + mpfc("7179128465465716019964074262528000")*nu + mpfc("1767088617119443672586090289561600")*Power(nu,2) + mpfc("274205941368099470864295678443520")*Power(nu,3) + mpfc("30095026718425582236318579032064")*Power(nu,4) + mpfc("2484379146422033583416190959616")*Power(nu,5) + mpfc("160136180493982406800095313920")*Power(nu,6) + mpfc("8257793065614936386215608320")*Power(nu,7) + mpfc("346243522853288867266166784")*Power(nu,8) + mpfc("11931094295481523653246976")*Power(nu,9) + mpfc("340092706946146122608640")*Power(nu,10) + mpfc("8044350035354690104320")*Power(nu,11) + mpfc("157899585293932145664")*Power(nu,12) + mpfc("2564181660288824576")*Power(nu,13) + mpfc("34229173748098560")*Power(nu,14) + mpfc("371642597096320")*Power(nu,15) + mpfc("3229498952064")*Power(nu,16) + mpfc("21923511456")*Power(nu,17) + mpfc("112007280")*Power(nu,18) + mpfc("405020")*Power(nu,19) + mpfc("924")*Power(nu,20) + Power(nu,21)) + mpfc("193536720")*Power(lambda,11)*(mpfc("304136552544736140970701933772800000") + mpfc("171765214992279213392423358038016000")*nu + mpfc("46055078042093476816858060632883200")*Power(nu,2) + mpfc("7799619327217632031600595215319040")*Power(nu,3) + mpfc("936296529173462280063304417148928")*Power(nu,4) + mpfc("84751367939710321071474780143616")*Power(nu,5) + mpfc("6007375117289646533018287865856")*Power(nu,6) + mpfc("341807627937511007296838696960")*Power(nu,7) + mpfc("15875150568387291466071277568")*Power(nu,8) + mpfc("608727597353882387637600256")*Power(nu,9) + mpfc("19413133848296738350637056")*Power(nu,10) + mpfc("517068407723949304903680")*Power(nu,11) + mpfc("11518140911821197308928")*Power(nu,12) + mpfc("214311581820286286336")*Power(nu,13) + mpfc("3317223482746992896")*Power(nu,14) + mpfc("42405310884217600")*Power(nu,15) + mpfc("442691574041728")*Power(nu,16) + mpfc("3711816204096")*Power(nu,17) + mpfc("24387671616")*Power(nu,18) + mpfc("120917720")*Power(nu,19) + mpfc("425348")*Power(nu,20) + mpfc("946")*Power(nu,21) + Power(nu,22)) + mpfc("92561040")*Power(lambda,10)*(mpfc("6082731050894722819414038675456000000") + mpfc("3739440852390320408819169094533120000")*nu + mpfc("1092866775834148749729584570695680000")*Power(nu,2) + mpfc("202047464586446117448869964939264000")*Power(nu,3) + mpfc("26525549910686877632866683558297600")*Power(nu,4) + mpfc("2631323887967668701492800020021248")*Power(nu,5) + mpfc("204898870285503251731840537460736")*Power(nu,6) + mpfc("12843527676039866678955061805056")*Power(nu,7) + mpfc("659310639305256836618264248320")*Power(nu,8) + mpfc("28049702515464939218823282688")*Power(nu,9) + mpfc("996990274319817154650341376")*Power(nu,10) + mpfc("29754502002775724448710656")*Power(nu,11) + mpfc("747431225960373251082240")*Power(nu,12) + mpfc("15804372548226923035648")*Power(nu,13) + mpfc("280656051475226144256")*Power(nu,14) + mpfc("4165329700431344896")*Power(nu,15) + mpfc("51259142365052160")*Power(nu,16) + mpfc("516927898123648")*Power(nu,17) + mpfc("4199569636416")*Power(nu,18) + mpfc("26806026016")*Power(nu,19) + mpfc("129424680")*Power(nu,20) + mpfc("444268")*Power(nu,21) + mpfc("966")*Power(nu,22) + Power(nu,23)) + mpfc("38567100")*Power(lambda,9)*(mpfc("109489158916105010749452696158208000000") + mpfc("73392666393920490178159082377052160000")*nu + mpfc("23411042817404997903951691367055360000")*Power(nu,2) + mpfc("4729721138390178863809243939602432000")*Power(nu,3) + mpfc("679507362978809914840470268988620800")*Power(nu,4) + mpfc("73889379894104914259737083918680064")*Power(nu,5) + mpfc("6319503553106727232665929694314496")*Power(nu,6) + mpfc("436082368454220851953031649951744")*Power(nu,7) + mpfc("24711119183534489738083818274816")*Power(nu,8) + mpfc("1164205284583625742557083336704")*Power(nu,9) + mpfc("45995527453221648002529427456")*Power(nu,10) + mpfc("1532571310369780194727133184")*Power(nu,11) + mpfc("43208264070062442968190976")*Power(nu,12) + mpfc("1031909931828457865723904")*Power(nu,13) + mpfc("20856181474780993632256")*Power(nu,14) + mpfc("355631986082990352384")*Power(nu,15) + mpfc("5087994263002283776")*Power(nu,16) + mpfc("60563844531277824")*Power(nu,17) + mpfc("592520151579136")*Power(nu,18) + mpfc("4682078104704")*Power(nu,19) + mpfc("29135670256")*Power(nu,20) + mpfc("137421504")*Power(nu,21) + mpfc("461656")*Power(nu,22) + mpfc("984")*Power(nu,23) + Power(nu,24)) + mpfc("13884156")*Power(lambda,8)*(mpfc("1751826542657680171991243138531328000000") + mpfc("1283771821218832853599998014191042560000")*nu + mpfc("447969351472400456641386144249937920000")*Power(nu,2) + mpfc("99086581031647859724899594400694272000")*Power(nu,3) + mpfc("15601838946051137501256768243420364800")*Power(nu,4) + mpfc("1861737441284488542996263611687501824")*Power(nu,5) + mpfc("175001436743812549982391959027712000")*Power(nu,6) + mpfc("13296821448374260863914436093542400")*Power(nu,7) + mpfc("831460275390772687762372742348800")*Power(nu,8) + mpfc("43338403736872501618997151662080")*Power(nu,9) + mpfc("1900133723835172110597554176000")*Power(nu,10) + mpfc("70516668419138131118163558400")*Power(nu,11) + mpfc("2223903535490779282218188800")*Power(nu,12) + mpfc("59718822979317768819773440")*Power(nu,13) + mpfc("1365608835424953763840000")*Power(nu,14) + mpfc("26546293252108839270400")*Power(nu,15) + mpfc("437039894291026892800")*Power(nu,16) + mpfc("6057015775502728960")*Power(nu,17) + mpfc("70044166956544000")*Power(nu,18) + mpfc("667433401254400")*Power(nu,19) + mpfc("5148248828800")*Power(nu,20) + mpfc("31334414320")*Power(nu,21) + mpfc("144808000")*Power(nu,22) + mpfc("477400")*Power(nu,23) + mpfc("1000")*Power(nu,24) + Power(nu,25)) + mpfc("4272048")*Power(lambda,7)*(mpfc("24525571597207522407877403939438592000000") + mpfc("19724632039721340122391215337205923840000")*nu + mpfc("7555342741832439246579404033690173440000")*Power(nu,2) + mpfc("1835181485915470492789980465859657728000")*Power(nu,3) + mpfc("317512326276363784742494349808579379200")*Power(nu,4) + mpfc("41666163124033977103204458807045390336")*Power(nu,5) + mpfc("4311757555697864242749751038075469824")*Power(nu,6) + mpfc("361156937021052202077194064337305600")*Power(nu,7) + mpfc("24937265303845078492587654486425600")*Power(nu,8) + mpfc("1438197927706987710428332865617920")*Power(nu,9) + mpfc("69940275870564911167362910126080")*Power(nu,10) + mpfc("2887367081703105946251843993600")*Power(nu,11) + mpfc("101651317916009041069218201600")*Power(nu,12) + mpfc("3059967057201228045695016960")*Power(nu,13) + mpfc("78837346675267121513533440")*Power(nu,14) + mpfc("1737256940954477513625600")*Power(nu,15) + mpfc("32664851772183215769600")*Power(nu,16) + mpfc("521838115148065098240")*Power(nu,17) + mpfc("7037634112894344960")*Power(nu,18) + mpfc("79388234574105600")*Power(nu,19) + mpfc("739508884857600")*Power(nu,20) + mpfc("5586930629280")*Power(nu,21) + mpfc("33361726320")*Power(nu,22) + mpfc("151491600")*Power(nu,23) + mpfc("491400")*Power(nu,24) + mpfc("1014")*Power(nu,25) + Power(nu,26)) + mpfc("1107568")*Power(lambda,6)*(mpfc("294306859166490268894528847273263104000000") + mpfc("261221156073863603876571987985909678080000")*nu + mpfc("110388744941710611081344063741488005120000")*Power(nu,2) + mpfc("29577520572818085160059169624006066176000")*Power(nu,3) + mpfc("5645329401231835909699912663562610278400")*Power(nu,4) + mpfc("817506283764771509980947855493124063232")*Power(nu,5) + mpfc("93407253792408348016201471263951028224")*Power(nu,6) + mpfc("8645640799950490667676079810123137024")*Power(nu,7) + mpfc("660404120667193143988245918174412800")*Power(nu,8) + mpfc("42195640436328931017727648873840640")*Power(nu,9) + mpfc("2277481238153766644436687787130880")*Power(nu,10) + mpfc("104588680851002182522385038049280")*Power(nu,11) + mpfc("4107182896695214439082462412800")*Power(nu,12) + mpfc("138370922602423777617558405120")*Power(nu,13) + mpfc("4006015217304433503857418240")*Power(nu,14) + mpfc("99684429966720851677040640")*Power(nu,15) + mpfc("2129235162220676102860800")*Power(nu,16) + mpfc("38926909153959996948480")*Power(nu,17) + mpfc("606289724502797237760")*Power(nu,18) + mpfc("7990292927783612160")*Power(nu,19) + mpfc("88262341192396800")*Power(nu,20) + mpfc("806552052408960")*Power(nu,21) + mpfc("5987271345120")*Power(nu,22) + mpfc("35179625520")*Power(nu,23) + mpfc("157388400")*Power(nu,24) + mpfc("503568")*Power(nu,25) + mpfc("1026")*Power(nu,26) + Power(nu,27)) + mpfc("237336")*Power(lambda,5)*(mpfc("2943068591664902688945288472732631040000000") + mpfc("2906518419905126307660248727132359884800000")*nu + mpfc("1365108605490969714690012625400789729280000")*Power(nu,2) + mpfc("406163950669891462681935759981548666880000")*Power(nu,3) + mpfc("86030814585136444257058296259632168960000")*Power(nu,4) + mpfc("13820392238879551009509391218493850910720")*Power(nu,5) + mpfc("1751578821688854990142962568132634345472")*Power(nu,6) + mpfc("179863661791913254692962269365182398464")*Power(nu,7) + mpfc("15249682006622422107558538991867265024")*Power(nu,8) + mpfc("1082360525030482454165522406912819200")*Power(nu,9) + mpfc("64970452817866597462094526745149440")*Power(nu,10) + mpfc("3323368046663788469660538167623680")*Power(nu,11) + mpfc("145660509817954326913209662177280")*Power(nu,12) + mpfc("5490892122719452215258046464000")*Power(nu,13) + mpfc("178431074775468112656132587520")*Power(nu,14) + mpfc("5002859516971642020627824640")*Power(nu,15) + mpfc("120976781588927612705648640")*Power(nu,16) + mpfc("2518504253760276072345600")*Power(nu,17) + mpfc("44989806398987969326080")*Power(nu,18) + mpfc("686192653780633359360")*Power(nu,19) + mpfc("8872916339707580160")*Power(nu,20) + mpfc("96327861716486400")*Power(nu,21) + mpfc("866424765860160")*Power(nu,22) + mpfc("6339067600320")*Power(nu,23) + mpfc("36753509520")*Power(nu,24) + mpfc("162424080")*Power(nu,25) + mpfc("513828")*Power(nu,26) + mpfc("1036")*Power(nu,27) + Power(nu,28)) + mpfc("40920")*Power(lambda,4)*(mpfc("23544548733319221511562307781861048320000000") + mpfc("26195215950905913150227278289791510118400000")*nu + mpfc("13827387263832884025180349730338677719040000")*Power(nu,2) + mpfc("4614420210850101416145498705253179064320000")*Power(nu,3) + mpfc("1094410467350983016738402130058606018560000")*Power(nu,4) + mpfc("196593952496172852333133426007582976245760")*Power(nu,5) + mpfc("27833022812390390930653091763554925674496")*Power(nu,6) + mpfc("3190488116024161027686660723054093533184")*Power(nu,7) + mpfc("301861117844892631553430581300120518656")*Power(nu,8) + mpfc("23908566206866281740882718247169818624")*Power(nu,9) + mpfc("1602124147573415233862278620874014720")*Power(nu,10) + mpfc("91557397191176905219378832086138880")*Power(nu,11) + mpfc("4488652125207423084966215465041920")*Power(nu,12) + mpfc("189587646799709944635274033889280")*Power(nu,13) + mpfc("6918340720923197116507107164160")*Power(nu,14) + mpfc("218453950911241248821155184640")*Power(nu,15) + mpfc("5970673769683062922273013760")*Power(nu,16) + mpfc("141124815619009821284413440")*Power(nu,17) + mpfc("2878422704952179826954240")*Power(nu,18) + mpfc("50479347629233036200960")*Power(nu,19) + mpfc("757175984498294000640")*Power(nu,20) + mpfc("9643539233439471360")*Power(nu,21) + mpfc("103259259843367680")*Power(nu,22) + mpfc("917137306662720")*Power(nu,23) + mpfc("6633095676480")*Power(nu,24) + mpfc("38052902160")*Power(nu,25) + mpfc("166534704")*Power(nu,26) + mpfc("522116")*Power(nu,27) + mpfc("1044")*Power(nu,28) + Power(nu,29)) + mpfc("5456")*Power(lambda,3)*(mpfc("141267292399915329069373846691166289920000000") + mpfc("180715844438754700412925977520610109030400000")*nu + mpfc("109159539533903217301309376671823576432640000")*Power(nu,2) + mpfc("41513908528933492522053341961857752104960000")*Power(nu,3) + mpfc("11180883014955999516575911485604815175680000")*Power(nu,4) + mpfc("2273974182328020130737202686104103876034560")*Power(nu,5) + mpfc("363592089370515197917051976588912530292736")*Power(nu,6) + mpfc("46975951508535357096773056101879486873600")*Power(nu,7) + mpfc("5001654823093516817007244210854816645120")*Power(nu,8) + mpfc("445312515086090321998726890783139430400")*Power(nu,9) + mpfc("33521311092306773144056389972413906944")*Power(nu,10) + mpfc("2151468530720476665178551613390848000")*Power(nu,11) + mpfc("118489309942421443729176124876390400")*Power(nu,12) + mpfc("5626178006005682752777859668377600")*Power(nu,13) + mpfc("231097691125249127334316676874240")*Power(nu,14) + mpfc("8229064426390644609434038272000")*Power(nu,15) + mpfc("254277993529339626354793267200")*Power(nu,16) + mpfc("6817422663397121849979494400")*Power(nu,17) + mpfc("158395351848722900246138880")*Power(nu,18) + mpfc("3181298790727578044160000")*Power(nu,19) + mpfc("55022403536222800204800")*Power(nu,20) + mpfc("815037219898930828800")*Power(nu,21) + mpfc("10263094792499677440")*Power(nu,22) + mpfc("108762083683344000")*Power(nu,23) + mpfc("956935880721600")*Power(nu,24) + mpfc("6861413089440")*Power(nu,25) + mpfc("39052110384")*Power(nu,26) + mpfc("169667400")*Power(nu,27) + mpfc("528380")*Power(nu,28) + mpfc("1050")*Power(nu,29) + Power(nu,30)) + mpfc("528")*Power(lambda,2)*(mpfc("565069169599661316277495386764665159680000000") + mpfc("864130670154934130721077756773606726041600000")*nu + mpfc("617354002574367569618163484207904414760960000")*Power(nu,2) + mpfc("275215173649637187389522744519254584852480000")*Power(nu,3) + mpfc("86237440588757490588356987904277012807680000")*Power(nu,4) + mpfc("20276779744268080039524722230021230679818240")*Power(nu,5) + mpfc("3728342539810080922405410592459753997205504")*Power(nu,6) + mpfc("551495895404656626304144200996430477787136")*Power(nu,7) + mpfc("66982570800909424364802032945298753454080")*Power(nu,8) + mpfc("6782904883437878105002151773987374366720")*Power(nu,9) + mpfc("579397759455317414574952450672795058176")*Power(nu,10) + mpfc("42127185215188679804770596425977298944")*Power(nu,11) + mpfc("2625425770490162440095256112896409600")*Power(nu,12) + mpfc("140994021966444174740287563549900800")*Power(nu,13) + mpfc("6550568770506679262115126375874560")*Power(nu,14) + mpfc("264013948830811705772052829962240")*Power(nu,15) + mpfc("9246176400508003114853211340800")*Power(nu,16) + mpfc("281547684182928113754711244800")*Power(nu,17) + mpfc("7451004070792013450964049920")*Power(nu,18) + mpfc("171120547011633212422778880")*Power(nu,19) + mpfc("3401388404872469244979200")*Power(nu,20) + mpfc("58282552415818523520000")*Power(nu,21) + mpfc("856089599068929538560")*Power(nu,22) + mpfc("10698143127233053440")*Power(nu,23) + mpfc("112589827206230400")*Power(nu,24) + mpfc("984381533079360")*Power(nu,25) + mpfc("7017621530976")*Power(nu,26) + mpfc("39730779984")*Power(nu,27) + mpfc("171780920")*Power(nu,28) + mpfc("532580")*Power(nu,29) + mpfc("1054")*Power(nu,30) + Power(nu,31)) + mpfc("33")*lambda*(mpfc("1130138339199322632554990773529330319360000000") + mpfc("2293330509909529577719650900311878611763200000")*nu + mpfc("2098838675303669269957404725189415555563520000")*Power(nu,2) + mpfc("1167784349873641944397208973246413584465920000")*Power(nu,3) + mpfc("447690054827152168566236720327808610467840000")*Power(nu,4) + mpfc("126791000077293650667406432364319474167316480")*Power(nu,5) + mpfc("27733464823888241884335543414940738674229248")*Power(nu,6) + mpfc("4831334330619394175013698994452614952779776")*Power(nu,7) + mpfc("685461037006475475033748266887027984695296")*Power(nu,8) + mpfc("80548380567785180574806336493273502187520")*Power(nu,9) + mpfc("7941700402348512934152056675332964483072")*Power(nu,10) + mpfc("663652129885694774184493643524749656064")*Power(nu,11) + mpfc("47378036756169004684961108651770118144")*Power(nu,12) + mpfc("2907413814423050789575831239996211200")*Power(nu,13) + mpfc("154095159507457533264517816301649920")*Power(nu,14) + mpfc("7078596668168302673659232035799040")*Power(nu,15) + mpfc("282506301631827712001759252643840")*Power(nu,16) + mpfc("9809271768873859342362633830400")*Power(nu,17) + mpfc("296449692324512140656639344640")*Power(nu,18) + mpfc("7793245164815279875809607680")*Power(nu,19) + mpfc("177923323821378150912737280")*Power(nu,20) + mpfc("3517953509704106292019200")*Power(nu,21) + mpfc("59994731613956382597120")*Power(nu,22) + mpfc("877485885323395645440")*Power(nu,23) + mpfc("10923322781645514240")*Power(nu,24) + mpfc("114558590272389120")*Power(nu,25) + mpfc("998416776141312")*Power(nu,26) + mpfc("7097083090944")*Power(nu,27) + mpfc("40074341824")*Power(nu,28) + mpfc("172846080")*Power(nu,29) + mpfc("534688")*Power(nu,30) + mpfc("1056")*Power(nu,31) + Power(nu,32)) + nu*(mpfc("1130138339199322632554990773529330319360000000") + mpfc("2293330509909529577719650900311878611763200000")*nu + mpfc("2098838675303669269957404725189415555563520000")*Power(nu,2) + mpfc("1167784349873641944397208973246413584465920000")*Power(nu,3) + mpfc("447690054827152168566236720327808610467840000")*Power(nu,4) + mpfc("126791000077293650667406432364319474167316480")*Power(nu,5) + mpfc("27733464823888241884335543414940738674229248")*Power(nu,6) + mpfc("4831334330619394175013698994452614952779776")*Power(nu,7) + mpfc("685461037006475475033748266887027984695296")*Power(nu,8) + mpfc("80548380567785180574806336493273502187520")*Power(nu,9) + mpfc("7941700402348512934152056675332964483072")*Power(nu,10) + mpfc("663652129885694774184493643524749656064")*Power(nu,11) + mpfc("47378036756169004684961108651770118144")*Power(nu,12) + mpfc("2907413814423050789575831239996211200")*Power(nu,13) + mpfc("154095159507457533264517816301649920")*Power(nu,14) + mpfc("7078596668168302673659232035799040")*Power(nu,15) + mpfc("282506301631827712001759252643840")*Power(nu,16) + mpfc("9809271768873859342362633830400")*Power(nu,17) + mpfc("296449692324512140656639344640")*Power(nu,18) + mpfc("7793245164815279875809607680")*Power(nu,19) + mpfc("177923323821378150912737280")*Power(nu,20) + mpfc("3517953509704106292019200")*Power(nu,21) + mpfc("59994731613956382597120")*Power(nu,22) + mpfc("877485885323395645440")*Power(nu,23) + mpfc("10923322781645514240")*Power(nu,24) + mpfc("114558590272389120")*Power(nu,25) + mpfc("998416776141312")*Power(nu,26) + mpfc("7097083090944")*Power(nu,27) + mpfc("40074341824")*Power(nu,28) + mpfc("172846080")*Power(nu,29) + mpfc("534688")*Power(nu,30) + mpfc("1056")*Power(nu,31) + Power(nu,32)));
        moment(34, Power(lambda,34) + mpfc("34")*Power(lambda,33)*(mpfc("66") + nu) + mpfc("561")*Power(lambda,32)*(mpfc("4224") + mpfc("130")*nu + Power(nu,2)) + mpfc("5984")*Power(lambda,31)*(mpfc("261888") + mpfc("12284")*nu + mpfc("192")*Power(nu,2) + Power(nu,3)) + mpfc("46376")*Power(lambda,30)*(mpfc("15713280") + mpfc("998928")*nu + mpfc("23804")*Power(nu,2) + mpfc("252")*Power(nu,3) + Power(nu,4)) + mpfc("278256")*Power(lambda,29)*(mpfc("911370240") + mpfc("73651104")*nu + mpfc("2379560")*Power(nu,2) + mpfc("38420")*Power(nu,3) + mpfc("310")*Power(nu,4) + Power(nu,5)) + mpfc("1344904")*Power(lambda,28)*(mpfc("51036733440") + mpfc("5035832064")*nu + mpfc("206906464")*Power(nu,2) + mpfc("4531080")*Power(nu,3) + mpfc("55780")*Power(nu,4) + mpfc("366")*Power(nu,5) + Power(nu,6)) + mpfc("5379616")*Power(lambda,27)*(mpfc("2755983605760") + mpfc("322971664896")*nu + mpfc("16208781120")*Power(nu,2) + mpfc("451584784")*Power(nu,3) + mpfc("7543200")*Power(nu,4) + mpfc("75544")*Power(nu,5) + mpfc("420")*Power(nu,6) + Power(nu,7)) + mpfc("18156204")*Power(lambda,26)*(mpfc("143311147499520") + mpfc("19550510180352")*nu + mpfc("1165828283136")*Power(nu,2) + mpfc("39691189888")*Power(nu,3) + mpfc("843831184")*Power(nu,4) + mpfc("11471488")*Power(nu,5) + mpfc("97384")*Power(nu,6) + mpfc("472")*Power(nu,7) + Power(nu,8)) + mpfc("52451256")*Power(lambda,25)*(mpfc("7165557374976000") + mpfc("1120836656517120")*nu + mpfc("77841924337152")*Power(nu,2) + mpfc("3150387777536")*Power(nu,3) + mpfc("81882749088")*Power(nu,4) + mpfc("1417405584")*Power(nu,5) + mpfc("16340688")*Power(nu,6) + mpfc("120984")*Power(nu,7) + mpfc("522")*Power(nu,8) + Power(nu,9)) + mpfc("131128140")*Power(lambda,24)*(mpfc("343946753998848000") + mpfc("60965716887797760")*nu + mpfc("4857249024700416")*Power(nu,2) + mpfc("229060537658880")*Power(nu,3) + mpfc("7080759733760")*Power(nu,4) + mpfc("149918217120")*Power(nu,5) + mpfc("2201758608")*Power(nu,6) + mpfc("22147920")*Power(nu,7) + mpfc("146040")*Power(nu,8) + mpfc("570")*Power(nu,9) + Power(nu,10)) + mpfc("286097760")*Power(lambda,23)*(mpfc("15821550683947008000") + mpfc("3148369730837544960")*nu + mpfc("284399172024016896")*Power(nu,2) + mpfc("15394033757008896")*Power(nu,3) + mpfc("554775485411840")*Power(nu,4) + mpfc("13976997721280")*Power(nu,5) + mpfc("251199113088")*Power(nu,6) + mpfc("3220562928")*Power(nu,7) + mpfc("28865760")*Power(nu,8) + mpfc("172260")*Power(nu,9) + mpfc("616")*Power(nu,10) + Power(nu,11)) + mpfc("548354040")*Power(lambda,22)*(mpfc("696148230093668352000") + mpfc("154349818840798986240")*nu + mpfc("15661933299894288384")*Power(nu,2) + mpfc("961736657332408320")*Power(nu,3) + mpfc("39804155115129856")*Power(nu,4) + mpfc("1169763385148160")*Power(nu,5) + mpfc("25029758697152")*Power(nu,6) + mpfc("392903881920")*Power(nu,7) + mpfc("4490656368")*Power(nu,8) + mpfc("36445200")*Power(nu,9) + mpfc("199364")*Power(nu,10) + mpfc("660")*Power(nu,11) + Power(nu,12)) + mpfc("927983760")*Power(lambda,21)*(mpfc("29238225663934070784000") + mpfc("7178840621407225774080")*nu + mpfc("812151017436359098368")*Power(nu,2) + mpfc("56054872907855437824")*Power(nu,3) + mpfc("2633511172167862272")*Power(nu,4) + mpfc("88934217291352576")*Power(nu,5) + mpfc("2221013250428544")*Power(nu,6) + mpfc("41531721737792")*Power(nu,7) + mpfc("581511449376")*Power(nu,8) + mpfc("6021354768")*Power(nu,9) + mpfc("44818488")*Power(nu,10) + mpfc("227084")*Power(nu,11) + mpfc("702")*Power(nu,12) + Power(nu,13)) + mpfc("1391975640")*Power(lambda,20)*(mpfc("1169529026557362831360000") + mpfc("316391850520223101747200")*nu + mpfc("39664881318861589708800")*Power(nu,2) + mpfc("3054345933750576611328")*Power(nu,3) + mpfc("161395319794569928704")*Power(nu,4) + mpfc("6190879863821965312")*Power(nu,5) + mpfc("177774747308494336")*Power(nu,6) + mpfc("3882282119940224")*Power(nu,7) + mpfc("64792179712832")*Power(nu,8) + mpfc("822365640096")*Power(nu,9) + mpfc("7814094288")*Power(nu,10) + mpfc("53901848")*Power(nu,11) + mpfc("255164")*Power(nu,12) + mpfc("742")*Power(nu,13) + Power(nu,14)) + mpfc("1855967520")*Power(lambda,19)*(mpfc("44442103009179787591680000") + mpfc("13192419346325840697753600")*nu + mpfc("1823657340636963510681600")*Power(nu,2) + mpfc("155730026801383500939264")*Power(nu,3) + mpfc("9187368085944233902080")*Power(nu,4) + mpfc("396648754619804610560")*Power(nu,5) + mpfc("12946320261544750080")*Power(nu,6) + mpfc("325301467866222848")*Power(nu,7) + mpfc("6344384949027840")*Power(nu,8) + mpfc("96042074036480")*Power(nu,9) + mpfc("1119301223040")*Power(nu,10) + mpfc("9862364512")*Power(nu,11) + mpfc("63598080")*Power(nu,12) + mpfc("283360")*Power(nu,13) + mpfc("780")*Power(nu,14) + Power(nu,15)) + mpfc("2203961430")*Power(lambda,18)*(mpfc("1599915708330472353300480000") + mpfc("519369199476910052710809600")*nu + mpfc("78844083609256527082291200")*Power(nu,2) + mpfc("7429938305486769544495104")*Power(nu,3) + mpfc("486475277895375921414144")*Power(nu,4) + mpfc("23466723252257199882240")*Power(nu,5) + mpfc("862716284035415613440")*Power(nu,6) + mpfc("24657173104728772608")*Power(nu,7) + mpfc("553699326031225088")*Power(nu,8) + mpfc("9801899614341120")*Power(nu,9) + mpfc("136336918065920")*Power(nu,10) + mpfc("1474346345472")*Power(nu,11) + mpfc("12151895392")*Power(nu,12) + mpfc("73799040")*Power(nu,13) + mpfc("311440")*Power(nu,14) + mpfc("816")*Power(nu,15) + Power(nu,16)) + mpfc("2333606220")*Power(lambda,17)*(mpfc("54397134083236060012216320000") + mpfc("19258468490545414145468006400")*nu + mpfc("3200068042191631973508710400")*Power(nu,2) + mpfc("331461985995806691595124736")*Power(nu,3) + mpfc("23970097753929550872576000")*Power(nu,4) + mpfc("1284343868472120717410304")*Power(nu,5) + mpfc("52799076909461330739200")*Power(nu,6) + mpfc("1701060169596193882112")*Power(nu,7) + mpfc("43482950189790425600")*Power(nu,8) + mpfc("886963912918823168")*Power(nu,9) + mpfc("14437354828582400")*Power(nu,10) + mpfc("186464693811968")*Power(nu,11) + mpfc("1887510788800")*Power(nu,12) + mpfc("14661062752")*Power(nu,13) + mpfc("84388000")*Power(nu,14) + mpfc("339184")*Power(nu,15) + mpfc("850")*Power(nu,16) + Power(nu,17)) + mpfc("2203961430")*Power(lambda,16)*(mpfc("1740708290663553920390922240000") + mpfc("670668125780689312667192524800")*nu + mpfc("121660645840677637297746739200")*Power(nu,2) + mpfc("13806851594057446104552701952")*Power(nu,3) + mpfc("1098505114121552319517556736")*Power(nu,4) + mpfc("65069101545037413829705728")*Power(nu,5) + mpfc("2973914329574883301064704")*Power(nu,6) + mpfc("107233002336539534966784")*Power(nu,7) + mpfc("3092514575669487501312")*Power(nu,8) + mpfc("71865795403192766976")*Power(nu,9) + mpfc("1348959267433459968")*Power(nu,10) + mpfc("20404225030565376")*Power(nu,11) + mpfc("246865039053568")*Power(nu,12) + mpfc("2356664796864")*Power(nu,13) + mpfc("17361478752")*Power(nu,14) + mpfc("95241888")*Power(nu,15) + mpfc("366384")*Power(nu,16) + mpfc("882")*Power(nu,17) + Power(nu,18)) + mpfc("1855967520")*Power(lambda,15)*(mpfc("52221248719906617611727667200000") + mpfc("21860752064084233300406697984000")*nu + mpfc("4320487501001018431599594700800")*Power(nu,2) + mpfc("535866193662401020434327797760")*Power(nu,3) + mpfc("46762005017704015690079404032")*Power(nu,4) + mpfc("3050578160472674734408728576")*Power(nu,5) + mpfc("154286531432283912861646848")*Power(nu,6) + mpfc("6190904399671069350068224")*Power(nu,7) + mpfc("200008439606624160006144")*Power(nu,8) + mpfc("5248488437765270510592")*Power(nu,9) + mpfc("112334573426196566016")*Power(nu,10) + mpfc("1961086018350421248")*Power(nu,11) + mpfc("27810176202172416")*Power(nu,12) + mpfc("317564982959488")*Power(nu,13) + mpfc("2877509159424")*Power(nu,14) + mpfc("20218735392")*Power(nu,15) + mpfc("106233408")*Power(nu,16) + mpfc("392844")*Power(nu,17) + mpfc("912")*Power(nu,18) + Power(nu,19)) + mpfc("1391975640")*Power(lambda,14)*(mpfc("1462194964157385293128374681600000") + mpfc("664322306514265150023115210752000")*nu + mpfc("142834402092112749385195349606400")*Power(nu,2) + mpfc("19324740923548247003760773038080")*Power(nu,3) + mpfc("1845202334158113459756551110656")*Power(nu,4) + mpfc("132178193510938908253523804160")*Power(nu,5) + mpfc("7370601040576624294534840320")*Power(nu,6) + mpfc("327631854623073854663557120")*Power(nu,7) + mpfc("11791140708656545830240256")*Power(nu,8) + mpfc("346966115864051734302720")*Power(nu,9) + mpfc("8393856493698774359040")*Power(nu,10) + mpfc("167244981940008360960")*Power(nu,11) + mpfc("2739770952011248896")*Power(nu,12) + mpfc("36701995725038080")*Power(nu,13) + mpfc("398135239423360")*Power(nu,14) + mpfc("3443633750400")*Power(nu,15) + mpfc("23193270816")*Power(nu,16) + mpfc("117233040")*Power(nu,17) + mpfc("418380")*Power(nu,18) + mpfc("940")*Power(nu,19) + Power(nu,20)) + mpfc("927983760")*Power(lambda,13)*(mpfc("38017069068092017621337741721600000") + mpfc("18734574933528279193729370161152000")*nu + mpfc("4378016760909196634038194300518400")*Power(nu,2) + mpfc("645277666104367171482975448596480")*Power(nu,3) + mpfc("67300001611659196957431101915136")*Power(nu,4) + mpfc("5281835365442525074348170018816")*Power(nu,5) + mpfc("323813820565931139911429652480")*Power(nu,6) + mpfc("15889029260776544515787325440")*Power(nu,7) + mpfc("634201513048144046249803776")*Power(nu,8) + mpfc("20812259721121890922110976")*Power(nu,9) + mpfc("565206384700219867637760")*Power(nu,10) + mpfc("12742226024138991744000")*Power(nu,11) + mpfc("238479026692300832256")*Power(nu,12) + mpfc("3694022840862238976")*Power(nu,13) + mpfc("47053511950045440")*Power(nu,14) + mpfc("487669716933760")*Power(nu,15) + mpfc("4046658791616")*Power(nu,16) + mpfc("26241329856")*Power(nu,17) + mpfc("128110920")*Power(nu,18) + mpfc("442820")*Power(nu,19) + mpfc("966")*Power(nu,20) + Power(nu,21)) + mpfc("548354040")*Power(lambda,12)*(mpfc("912409657634208422912105801318400000") + mpfc("487646867472770718270842625589248000")*nu + mpfc("123806977195348998410646033373593600")*Power(nu,2) + mpfc("19864680747414008749629605066833920")*Power(nu,3) + mpfc("2260477704784187898461321894559744")*Power(nu,4) + mpfc("194064050382279798741787182366720")*Power(nu,5) + mpfc("13053367059024872432222481678336")*Power(nu,6) + mpfc("705150522824568208290325463040")*Power(nu,7) + mpfc("31109865573932001625782616064")*Power(nu,8) + mpfc("1133695746355069428380467200")*Power(nu,9) + mpfc("34377212953927167745417216")*Power(nu,10) + mpfc("871019809279555669493760")*Power(nu,11) + mpfc("18465722664754211718144")*Power(nu,12) + mpfc("327135574872994567680")*Power(nu,13) + mpfc("4823307127663329536")*Power(nu,14) + mpfc("58757585156455680")*Power(nu,15) + mpfc("584789527932544")*Power(nu,16) + mpfc("4676450708160")*Power(nu,17) + mpfc("29315991936")*Power(nu,18) + mpfc("138738600")*Power(nu,19) + mpfc("466004")*Power(nu,20) + mpfc("990")*Power(nu,21) + Power(nu,22)) + mpfc("286097760")*Power(lambda,11)*(mpfc("20073012467952585304066327629004800000") + mpfc("11640640742035164224870643564281856000")*nu + mpfc("3211400365770448683305055359808307200")*Power(nu,2) + mpfc("560829953638457190902497344843939840")*Power(nu,3) + mpfc("69595190252666142515778686747148288")*Power(nu,4) + mpfc("6529886813194343470780639906627584")*Power(nu,5) + mpfc("481238125680826992250681779290112")*Power(nu,6) + mpfc("28566678561165373014609641865216")*Power(nu,7) + mpfc("1389567565451072244057543016448")*Power(nu,8) + mpfc("56051171993743529050152894464")*Power(nu,9) + mpfc("1889994431341467118779645952")*Power(nu,10) + mpfc("53539648758077392474279936")*Power(nu,11) + mpfc("1277265707904148327292928")*Power(nu,12) + mpfc("25662705311960092207104")*Power(nu,13) + mpfc("433248331681587817472")*Power(nu,14) + mpfc("6115974001105354496")*Power(nu,15) + mpfc("71622954770971648")*Power(nu,16) + mpfc("687671443512064")*Power(nu,17) + mpfc("5321402530752")*Power(nu,18) + mpfc("32368241136")*Power(nu,19) + mpfc("148990688")*Power(nu,20) + mpfc("487784")*Power(nu,21) + mpfc("1012")*Power(nu,22) + Power(nu,23)) + mpfc("131128140")*Power(lambda,10)*(mpfc("401460249359051706081326552580096000000") + mpfc("252885827308655869801479198914641920000")*nu + mpfc("75868648057444137890971750760448000000")*Power(nu,2) + mpfc("14427999438539592501355002256687104000")*Power(nu,3) + mpfc("1952733758691780041218071079786905600")*Power(nu,4) + mpfc("200192926516553011931391484879699968")*Power(nu,5) + mpfc("16154649326810883315794275492429824")*Power(nu,6) + mpfc("1052571696904134452542874616594432")*Power(nu,7) + mpfc("56358029870186817895760502194176")*Power(nu,8) + mpfc("2510591005325942825060600905728")*Power(nu,9) + mpfc("93851060620572871425745813504")*Power(nu,10) + mpfc("2960787406503014968265244672")*Power(nu,11) + mpfc("79084962916160359020138496")*Power(nu,12) + mpfc("1790519814143350171435008")*Power(nu,13) + mpfc("34327671945591848556544")*Power(nu,14) + mpfc("555567811703694907392")*Power(nu,15) + mpfc("7548433096524787456")*Power(nu,16) + mpfc("85376383641212928")*Power(nu,17) + mpfc("794099494127104")*Power(nu,18) + mpfc("5968767353472")*Power(nu,19) + mpfc("35348054896")*Power(nu,20) + mpfc("158746368")*Power(nu,21) + mpfc("508024")*Power(nu,22) + mpfc("1032")*Power(nu,23) + Power(nu,24)) + mpfc("52451256")*Power(lambda,9)*(mpfc("7226284488462930709463877946441728000000") + mpfc("4953405140914857362507952133043650560000")*nu + mpfc("1618521492342650351838970712602705920000")*Power(nu,2) + mpfc("335572637951156802915361791380815872000")*Power(nu,3) + mpfc("49577207094991633243280281692851404800")*Power(nu,4) + mpfc("5556206435989734255983117807621505024")*Power(nu,5) + mpfc("490976614399148911615688443743436800")*Power(nu,6) + mpfc("35100939871085303461566018591129600")*Power(nu,7) + mpfc("2067016234567497174666563656089600")*Power(nu,8) + mpfc("101548667966053788746851318497280")*Power(nu,9) + mpfc("4199910096496254510724025548800")*Power(nu,10) + mpfc("147145233937627140854520217600")*Power(nu,11) + mpfc("4384316738993901430627737600")*Power(nu,12) + mpfc("111314319570740662105968640")*Power(nu,13) + mpfc("2408417909164003445452800")*Power(nu,14) + mpfc("44327892556258356889600")*Power(nu,15) + mpfc("691439607441141081600")*Power(nu,16) + mpfc("9085208002066620160")*Power(nu,17) + mpfc("99670174535500800")*Power(nu,18) + mpfc("901537306489600")*Power(nu,19) + mpfc("6605032341600")*Power(nu,20) + mpfc("38205489520")*Power(nu,21) + mpfc("167890800")*Power(nu,22) + mpfc("526600")*Power(nu,23) + mpfc("1050")*Power(nu,24) + Power(nu,25)) + mpfc("18156204")*Power(lambda,8)*(mpfc("115620551815406891351422047143067648000000") + mpfc("86480766743100648509591112075140136960000")*nu + mpfc("30849749018397262991931483534686945280000")*Power(nu,2) + mpfc("6987683699561159198484759374695759872000")*Power(nu,3) + mpfc("1128807951471022934807846298466438348800")*Power(nu,4) + mpfc("138476510070827381339010166614795485184")*Power(nu,5) + mpfc("13411832266376116841834132907516493824")*Power(nu,6) + mpfc("1052591652336513767000744741201510400")*Power(nu,7) + mpfc("68173199624165258256231037088563200")*Power(nu,8) + mpfc("3691794922024357794616184752046080")*Power(nu,9) + mpfc("168747229509993860918435727278080")*Power(nu,10) + mpfc("6554233839498288764396349030400")*Power(nu,11) + mpfc("217294301761529563744564019200")*Power(nu,12) + mpfc("6165345852125752024323235840")*Power(nu,13) + mpfc("149849006117364717233213440")*Power(nu,14) + mpfc("3117664190064137155686400")*Power(nu,15) + mpfc("55390926275316614195200")*Power(nu,16) + mpfc("836802935474207004160")*Power(nu,17) + mpfc("10679930794634632960")*Power(nu,18) + mpfc("114094771439334400")*Power(nu,19) + mpfc("1007217823955200")*Power(nu,20) + mpfc("7216320173920")*Power(nu,21) + mpfc("40891742320")*Power(nu,22) + mpfc("176316400")*Power(nu,23) + mpfc("543400")*Power(nu,24) + mpfc("1066")*Power(nu,25) + Power(nu,26)) + mpfc("5379616")*Power(lambda,7)*(mpfc("1618687725415696478919908660002947072000000") + mpfc("1326351286218815970485697616195029565440000")*nu + mpfc("518377253000662330396631881560757370880000")*Power(nu,2) + mpfc("128677320812253491770718114780427583488000")*Power(nu,3) + mpfc("22790995020155480285794607553225896755200")*Power(nu,4) + mpfc("3067479092462606273553988631073575141376")*Power(nu,5) + mpfc("326242161800093017124688027320026398720")*Power(nu,6) + mpfc("28148115399087309579844559284337639424")*Power(nu,7) + mpfc("2007016447074827382587979260441395200")*Power(nu,8) + mpfc("119858328532506267380857623617208320")*Power(nu,9) + mpfc("6054256135164271847474284933939200")*Power(nu,10) + mpfc("260506503262969903619984613703680")*Power(nu,11) + mpfc("9596354064159702656820245299200")*Power(nu,12) + mpfc("303609143691290092085089320960")*Power(nu,13) + mpfc("8263231937768858065588224000")*Power(nu,14) + mpfc("193496304778262637412823040")*Power(nu,15) + mpfc("3893137157918569754419200")*Power(nu,16) + mpfc("67106167371955512253440")*Power(nu,17) + mpfc("986321966599091865600")*Power(nu,18) + mpfc("12277257594785314560")*Power(nu,19) + mpfc("128195820974707200")*Power(nu,20) + mpfc("1108246306390080")*Power(nu,21) + mpfc("7788804566400")*Power(nu,22) + mpfc("43360171920")*Power(nu,23) + mpfc("183924000")*Power(nu,24) + mpfc("558324")*Power(nu,25) + mpfc("1080")*Power(nu,26) + Power(nu,27)) + mpfc("1344904")*Power(lambda,6)*(mpfc("19424252704988357747038903920035364864000000") + mpfc("17534903160041488124748280054343301857280000")*nu + mpfc("7546878322226763935245280194924118016000000")*Power(nu,2) + mpfc("2062505102747704231645249258925888372736000")*Power(nu,3) + mpfc("402169261054119255200253405419138344550400")*Power(nu,4) + mpfc("59600744129706755568442471126108798451712")*Power(nu,5) + mpfc("6982385034063722479050244958913891926016")*Power(nu,6) + mpfc("664019546589140732082822738732078071808")*Power(nu,7) + mpfc("52232312763985238170900310409634381824")*Power(nu,8) + mpfc("3445316389464902591158270743847895040")*Power(nu,9) + mpfc("192509402154477529550549042824478720")*Power(nu,10) + mpfc("9180334174319910690914100298383360")*Power(nu,11) + mpfc("375662752032886335501827557294080")*Power(nu,12) + mpfc("13239663788455183761841317150720")*Power(nu,13) + mpfc("402767926944516388872148008960")*Power(nu,14) + mpfc("10585187595108009714542100480")*Power(nu,15) + mpfc("240213950673285474465853440")*Power(nu,16) + mpfc("4698411166382035901460480")*Power(nu,17) + mpfc("78942030971144614640640")*Power(nu,18) + mpfc("1133649057736515640320")*Power(nu,19) + mpfc("13815607446481800960")*Power(nu,20) + mpfc("141494776651388160")*Power(nu,21) + mpfc("1201711961186880")*Power(nu,22) + mpfc("8309126629440")*Power(nu,23) + mpfc("45567259920")*Power(nu,24) + mpfc("190623888")*Power(nu,25) + mpfc("571284")*Power(nu,26) + mpfc("1092")*Power(nu,27) + Power(nu,28)) + mpfc("278256")*Power(lambda,5)*(mpfc("194242527049883577470389039200353648640000000") + mpfc("194773284305403238994521704463468383436800000")*nu + mpfc("93003686382309127477201082003584482017280000")*Power(nu,2) + mpfc("28171929349703806251697772784183001743360000")*Power(nu,3) + mpfc("6084197713288896783647783313117271818240000")*Power(nu,4) + mpfc("998176702351186810884678116680226329067520")*Power(nu,5) + mpfc("129424594470343980358944920715247717711872")*Power(nu,6) + mpfc("13622580499955129799878472346234672644096")*Power(nu,7) + mpfc("1186342674228993113791825842828421890048")*Power(nu,8) + mpfc("86685476658634264082483017848113332224")*Power(nu,9) + mpfc("5370410411009677886663761172092682240")*Power(nu,10) + mpfc("284312743897676636459690045808312320")*Power(nu,11) + mpfc("12936961694648774045932375871324160")*Power(nu,12) + mpfc("508059389917438173120240728801280")*Power(nu,13) + mpfc("17267343057900347650562797240320")*Power(nu,14) + mpfc("508619802895596486017569013760")*Power(nu,15) + mpfc("12987327101840864459200634880")*Power(nu,16) + mpfc("287198062337105833480458240")*Power(nu,17) + mpfc("5487831476093482047866880")*Power(nu,18) + mpfc("90278521548509771043840")*Power(nu,19) + mpfc("1271805132201333649920")*Power(nu,20) + mpfc("15230555212995682560")*Power(nu,21) + mpfc("153511896263256960")*Power(nu,22) + mpfc("1284803227481280")*Power(nu,23) + mpfc("8764799228640")*Power(nu,24) + mpfc("47473498800")*Power(nu,25) + mpfc("196336728")*Power(nu,26) + mpfc("582204")*Power(nu,27) + mpfc("1102")*Power(nu,28) + Power(nu,29)) + mpfc("46376")*Power(lambda,4)*(mpfc("1553940216399068619763112313602829189120000000") + mpfc("1752428801493109489426562674908100716134400000")*nu + mpfc("938802775363876258812130360492144239575040000")*Power(nu,2) + mpfc("318379121179939577490783264277048495964160000")*Power(nu,3) + mpfc("76845511056014980520880039289121176289280000")*Power(nu,4) + mpfc("14069611332098391270725208246559082450780160")*Power(nu,5) + mpfc("2033573458113938653756237482402208070762496")*Power(nu,6) + mpfc("238405238469985018757972699485125098864640")*Power(nu,7) + mpfc("23113321893787074710213079088862047764480")*Power(nu,8) + mpfc("1879826487498067226451689985613328547840")*Power(nu,9) + mpfc("129648759946711687175793107224854790144")*Power(nu,10) + mpfc("7644912362191090978341281538559180800")*Power(nu,11) + mpfc("387808437454866828827149052778905600")*Power(nu,12) + mpfc("17001436813988279430894301701734400")*Power(nu,13) + mpfc("646198134380640954324743106723840")*Power(nu,14) + mpfc("21336301481065119538703349350400")*Power(nu,15) + mpfc("612518419710323401691174092800")*Power(nu,16) + mpfc("15284911600537711127044300800")*Power(nu,17) + mpfc("331100714145853689863393280")*Power(nu,18) + mpfc("6210059648481560216217600")*Power(nu,19) + mpfc("100452962606120440243200")*Power(nu,20) + mpfc("1393649573905299110400")*Power(nu,21) + mpfc("16458650383101738240")*Power(nu,22) + mpfc("163790322083107200")*Power(nu,23) + mpfc("1354921621310400")*Power(nu,24) + mpfc("9144587219040")*Power(nu,25) + mpfc("49044192624")*Power(nu,26) + mpfc("200994360")*Power(nu,27) + mpfc("591020")*Power(nu,28) + mpfc("1110")*Power(nu,29) + Power(nu,30)) + mpfc("5984")*Power(lambda,3)*(mpfc("9323641298394411718578673881616975134720000000") + mpfc("12068513025357725556322488363051433485926400000")*nu + mpfc("7385245453676367042299344837860966153584640000")*Power(nu,2) + mpfc("2849077502443513723756829946154435215360000000")*Power(nu,3) + mpfc("779452187516029460616063500011775553699840000")*Power(nu,4) + mpfc("161263179048605328145231288768475670993960960")*Power(nu,5) + mpfc("26271052080782023193262633140972330875355136")*Power(nu,6) + mpfc("3464004888933848766304073679312958663950336")*Power(nu,7) + mpfc("377085169832707467019251174018297385451520")*Power(nu,8) + mpfc("34392280818775478068923219002542019051520")*Power(nu,9) + mpfc("2657719047178337349506448628962457288704")*Power(nu,10) + mpfc("175518234119858233045840796456209874944")*Power(nu,11) + mpfc("9971762986920291951304175855232614400")*Power(nu,12) + mpfc("489817058338796505412514862989312000")*Power(nu,13) + mpfc("20878625620272125156842760342077440")*Power(nu,14) + mpfc("774215943267031671556963202826240")*Power(nu,15) + mpfc("25011411999327059948850393907200")*Power(nu,16) + mpfc("704227889313549668453439897600")*Power(nu,17) + mpfc("17271515885412833266224660480")*Power(nu,18) + mpfc("368361072036743051160698880")*Power(nu,19) + mpfc("6812777424118282857676800")*Power(nu,20) + mpfc("108814860049552234905600")*Power(nu,21) + mpfc("1492401476203909539840")*Power(nu,22) + mpfc("17441392315600381440")*Power(nu,23) + mpfc("171919851810969600")*Power(nu,24) + mpfc("1409789144624640")*Power(nu,25) + mpfc("9438852374784")*Power(nu,26) + mpfc("50250158784")*Power(nu,27) + mpfc("204540480")*Power(nu,28) + mpfc("597680")*Power(nu,29) + mpfc("1116")*Power(nu,30) + Power(nu,31)) + mpfc("561")*Power(lambda,2)*(mpfc("37294565193577646874314695526467900538880000000") + mpfc("57597693399825313943868627333822709078425600000")*nu + mpfc("41609494840063193725519867714495298100264960000")*Power(nu,2) + mpfc("18781555463450421937326664622478707015024640000")*Power(nu,3) + mpfc("5966886252507631566221083946201537430159360000")*Power(nu,4) + mpfc("1424504903710450773196988655085678237675683840")*Power(nu,5) + mpfc("266347387371733420918281821332364994495381504")*Power(nu,6) + mpfc("40127071636517418258478927858224165531156480")*Power(nu,7) + mpfc("4972345568264678634381078375386148205756416")*Power(nu,8) + mpfc("514654293107809379294944050028465461657600")*Power(nu,9) + mpfc("45023157007488827466949013518391848206336")*Power(nu,10) + mpfc("3359791983657770281689811814787296788480")*Power(nu,11) + mpfc("215405286067539400851057499877140332544")*Power(nu,12) + mpfc("11931031220275477972954235307189862400")*Power(nu,13) + mpfc("573331560819885006039885904357621760")*Power(nu,14) + mpfc("23975489393340251843070613153382400")*Power(nu,15) + mpfc("874261591264339911352364778455040")*Power(nu,16) + mpfc("27828323556581258622664153497600")*Power(nu,17) + mpfc("773313952855201001518338539520")*Power(nu,18) + mpfc("18744960173559805470867456000")*Power(nu,19) + mpfc("395612181733216182591406080")*Power(nu,20) + mpfc("7248036864316491797299200")*Power(nu,21) + mpfc("114784465954367873064960")*Power(nu,22) + mpfc("1562167045466311065600")*Power(nu,23) + mpfc("18129071722844259840")*Power(nu,24) + mpfc("177559008389468160")*Power(nu,25) + mpfc("1447544554123776")*Power(nu,26) + mpfc("9639853009920")*Power(nu,27) + mpfc("51068320704")*Power(nu,28) + mpfc("206931200")*Power(nu,29) + mpfc("602144")*Power(nu,30) + mpfc("1120")*Power(nu,31) + Power(nu,32)) + mpfc("34")*lambda*(mpfc("74589130387155293748629391052935801077760000000") + mpfc("152489951993228274762051950194113318695731200000")*nu + mpfc("140816683079951701394908362762813305278955520000")*Power(nu,2) + mpfc("79172605766964037600173196959452712130314240000")*Power(nu,3) + mpfc("30715327968465685069768832514881781875343360000")*Power(nu,4) + mpfc("8815896059928533112615061256372893905510727680")*Power(nu,5) + mpfc("1957199678453917615033552297750408226666446848")*Power(nu,6) + mpfc("346601530644768257435239677048813325557694464")*Power(nu,7) + mpfc("50071762773046775527241084608996461942669312")*Power(nu,8) + mpfc("6001654154480297392970966475443079129071616")*Power(nu,9) + mpfc("604700607122787034228842077065249158070272")*Power(nu,10) + mpfc("51742740974804368030328637147966441783296")*Power(nu,11) + mpfc("3790602555792849083391926814541577453568")*Power(nu,12) + mpfc("239267348508090356796965970491520057344")*Power(nu,13) + mpfc("13077694341915247985034007115905105920")*Power(nu,14) + mpfc("621282539606565509726027130664386560")*Power(nu,15) + mpfc("25724012575868931665775342710292480")*Power(nu,16) + mpfc("929918238377502428597693085450240")*Power(nu,17) + mpfc("29374951462291660625700830576640")*Power(nu,18) + mpfc("810803873202320612460073451520")*Power(nu,19) + mpfc("19536184537026237836050268160")*Power(nu,20) + mpfc("410108255461849166186004480")*Power(nu,21) + mpfc("7477605796225227543429120")*Power(nu,22) + mpfc("117908800045300495196160")*Power(nu,23) + mpfc("1598425188911999585280")*Power(nu,24) + mpfc("18484189739623196160")*Power(nu,25) + mpfc("180454097497715712")*Power(nu,26) + mpfc("1466824260143616")*Power(nu,27) + mpfc("9741989651328")*Power(nu,28) + mpfc("51482183104")*Power(nu,29) + mpfc("208135488")*Power(nu,30) + mpfc("604384")*Power(nu,31) + mpfc("1122")*Power(nu,32) + Power(nu,33)) + nu*(mpfc("74589130387155293748629391052935801077760000000") + mpfc("152489951993228274762051950194113318695731200000")*nu + mpfc("140816683079951701394908362762813305278955520000")*Power(nu,2) + mpfc("79172605766964037600173196959452712130314240000")*Power(nu,3) + mpfc("30715327968465685069768832514881781875343360000")*Power(nu,4) + mpfc("8815896059928533112615061256372893905510727680")*Power(nu,5) + mpfc("1957199678453917615033552297750408226666446848")*Power(nu,6) + mpfc("346601530644768257435239677048813325557694464")*Power(nu,7) + mpfc("50071762773046775527241084608996461942669312")*Power(nu,8) + mpfc("6001654154480297392970966475443079129071616")*Power(nu,9) + mpfc("604700607122787034228842077065249158070272")*Power(nu,10) + mpfc("51742740974804368030328637147966441783296")*Power(nu,11) + mpfc("3790602555792849083391926814541577453568")*Power(nu,12) + mpfc("239267348508090356796965970491520057344")*Power(nu,13) + mpfc("13077694341915247985034007115905105920")*Power(nu,14) + mpfc("621282539606565509726027130664386560")*Power(nu,15) + mpfc("25724012575868931665775342710292480")*Power(nu,16) + mpfc("929918238377502428597693085450240")*Power(nu,17) + mpfc("29374951462291660625700830576640")*Power(nu,18) + mpfc("810803873202320612460073451520")*Power(nu,19) + mpfc("19536184537026237836050268160")*Power(nu,20) + mpfc("410108255461849166186004480")*Power(nu,21) + mpfc("7477605796225227543429120")*Power(nu,22) + mpfc("117908800045300495196160")*Power(nu,23) + mpfc("1598425188911999585280")*Power(nu,24) + mpfc("18484189739623196160")*Power(nu,25) + mpfc("180454097497715712")*Power(nu,26) + mpfc("1466824260143616")*Power(nu,27) + mpfc("9741989651328")*Power(nu,28) + mpfc("51482183104")*Power(nu,29) + mpfc("208135488")*Power(nu,30) + mpfc("604384")*Power(nu,31) + mpfc("1122")*Power(nu,32) + Power(nu,33)));
        moment(35, Power(lambda,35) + mpfc("35")*Power(lambda,34)*(mpfc("68") + nu) + mpfc("595")*Power(lambda,33)*(mpfc("4488") + mpfc("134")*nu + Power(nu,2)) + mpfc("6545")*Power(lambda,32)*(mpfc("287232") + mpfc("13064")*nu + mpfc("198")*Power(nu,2) + Power(nu,3)) + mpfc("52360")*Power(lambda,31)*(mpfc("17808384") + mpfc("1097200")*nu + mpfc("25340")*Power(nu,2) + mpfc("260")*Power(nu,3) + Power(nu,4)) + mpfc("324632")*Power(lambda,30)*(mpfc("1068503040") + mpfc("83640384")*nu + mpfc("2617600")*Power(nu,2) + mpfc("40940")*Power(nu,3) + mpfc("320")*Power(nu,4) + Power(nu,5)) + mpfc("1623160")*Power(lambda,29)*(mpfc("61973176320") + mpfc("5919645312")*nu + mpfc("235461184")*Power(nu,2) + mpfc("4992120")*Power(nu,3) + mpfc("59500")*Power(nu,4) + mpfc("378")*Power(nu,5) + Power(nu,6)) + mpfc("6724520")*Power(lambda,28)*(mpfc("3470497873920") + mpfc("393473313792")*nu + mpfc("19105471616")*Power(nu,2) + mpfc("515019904")*Power(nu,3) + mpfc("8324120")*Power(nu,4) + mpfc("80668")*Power(nu,5) + mpfc("434")*Power(nu,6) + Power(nu,7)) + mpfc("23535820")*Power(lambda,27)*(mpfc("187406885191680") + mpfc("24718056818688")*nu + mpfc("1425168781056")*Power(nu,2) + mpfc("46916546432")*Power(nu,3) + mpfc("964522384")*Power(nu,4) + mpfc("12680192")*Power(nu,5) + mpfc("104104")*Power(nu,6) + mpfc("488")*Power(nu,7) + Power(nu,8)) + mpfc("70607460")*Power(lambda,26)*(mpfc("9745158029967360") + mpfc("1472745839763456")*nu + mpfc("98826833433600")*Power(nu,2) + mpfc("3864829195520")*Power(nu,3) + mpfc("97071710400")*Power(nu,4) + mpfc("1623892368")*Power(nu,5) + mpfc("18093600")*Power(nu,6) + mpfc("129480")*Power(nu,7) + mpfc("540")*Power(nu,8) + Power(nu,9)) + mpfc("183579396")*Power(lambda,25)*(mpfc("487257901498368000") + mpfc("83382450018140160")*nu + mpfc("6414087511443456")*Power(nu,2) + mpfc("292068293209600")*Power(nu,3) + mpfc("8718414715520")*Power(nu,4) + mpfc("178266328800")*Power(nu,5) + mpfc("2528572368")*Power(nu,6) + mpfc("24567600")*Power(nu,7) + mpfc("156480")*Power(nu,8) + mpfc("590")*Power(nu,9) + Power(nu,10)) + mpfc("417225900")*Power(lambda,24)*(mpfc("23388379271921664000") + mpfc("4489615502369095680")*nu + mpfc("391258650567426048")*Power(nu,2) + mpfc("20433365585504256")*Power(nu,3) + mpfc("710552199554560")*Power(nu,4) + mpfc("17275198497920")*Power(nu,5) + mpfc("299637802464")*Power(nu,6) + mpfc("3707817168")*Power(nu,7) + mpfc("32078640")*Power(nu,8) + mpfc("184800")*Power(nu,9) + mpfc("638")*Power(nu,10) + Power(nu,11)) + mpfc("834451800")*Power(lambda,23)*(mpfc("1075865446508396544000") + mpfc("229910692380900065280")*nu + mpfc("22487513428470693888")*Power(nu,2) + mpfc("1331193467500621824")*Power(nu,3) + mpfc("53118766765014016")*Power(nu,4) + mpfc("1505211330458880")*Power(nu,5) + mpfc("31058537411264")*Power(nu,6) + mpfc("470197392192")*Power(nu,7) + mpfc("5183434608")*Power(nu,8) + mpfc("40579440")*Power(nu,9) + mpfc("214148")*Power(nu,10) + mpfc("684")*Power(nu,11) + Power(nu,12)) + mpfc("1476337800")*Power(lambda,22)*(mpfc("47338079646369447936000") + mpfc("11191935911267999416320")*nu + mpfc("1219361283233610596352")*Power(nu,2) + mpfc("81060025998498054144")*Power(nu,3) + mpfc("3668419205161238528")*Power(nu,4) + mpfc("119348065305204736")*Power(nu,5) + mpfc("2871786976554496")*Power(nu,6) + mpfc("51747222667712")*Power(nu,7) + mpfc("698268514944")*Power(nu,8) + mpfc("6968929968")*Power(nu,9) + mpfc("50001952")*Power(nu,10) + mpfc("244244")*Power(nu,11) + mpfc("728")*Power(nu,12) + Power(nu,13)) + mpfc("2319959400")*Power(lambda,21)*(mpfc("1988199345147516813312000") + mpfc("517399387919625423421440")*nu + mpfc("62405109807079644463104")*Power(nu,2) + mpfc("4623882375170528870400")*Power(nu,3) + mpfc("235133632615270072320")*Power(nu,4) + mpfc("8681037947979837440")*Power(nu,5) + mpfc("239963118320493568")*Power(nu,6) + mpfc("5045170328598400")*Power(nu,7) + mpfc("81074500295360")*Power(nu,8) + mpfc("990963573600")*Power(nu,9) + mpfc("9069011952")*Power(nu,10) + mpfc("60260200")*Power(nu,11) + mpfc("274820")*Power(nu,12) + mpfc("770")*Power(nu,13) + Power(nu,14)) + mpfc("3247943160")*Power(lambda,20)*(mpfc("79527973805900672532480000") + mpfc("22684174861932533750169600")*nu + mpfc("3013603780202811201945600")*Power(nu,2) + mpfc("247360404813900799279104")*Power(nu,3) + mpfc("14029227679781331763200")*Power(nu,4) + mpfc("582375150534463569920")*Power(nu,5) + mpfc("18279562680799580160")*Power(nu,6) + mpfc("441769931464429568")*Power(nu,7) + mpfc("8288150340412800")*Power(nu,8) + mpfc("120713043239360")*Power(nu,9) + mpfc("1353724051680")*Power(nu,10) + mpfc("11479419952")*Power(nu,11) + mpfc("71253000")*Power(nu,12) + mpfc("305620")*Power(nu,13) + mpfc("810")*Power(nu,14) + Power(nu,15)) + mpfc("4059928950")*Power(lambda,19)*(mpfc("3022063004624225556234240000") + mpfc("941526618559336955038924800")*nu + mpfc("137201118509639359424102400")*Power(nu,2) + mpfc("12413299163131041574551552")*Power(nu,3) + mpfc("780471056645591406280704")*Power(nu,4) + mpfc("36159483400090947420160")*Power(nu,5) + mpfc("1276998532404847616000")*Power(nu,6) + mpfc("35066820076447903744")*Power(nu,7) + mpfc("756719644400115968")*Power(nu,8) + mpfc("12875245983508480")*Power(nu,9) + mpfc("172154557203200")*Power(nu,10) + mpfc("1789942009856")*Power(nu,11) + mpfc("14187033952")*Power(nu,12) + mpfc("82866560")*Power(nu,13) + mpfc("336400")*Power(nu,14) + mpfc("848")*Power(nu,15) + Power(nu,16)) + mpfc("4537567650")*Power(lambda,18)*(mpfc("108794268166472120024432640000") + mpfc("36917021272760355937635532800")*nu + mpfc("5880766884906353894306611200")*Power(nu,2) + mpfc("584079888382356856107958272")*Power(nu,3) + mpfc("40510257202372332200656896")*Power(nu,4) + mpfc("2082212459048865513406464")*Power(nu,5) + mpfc("82131430566665461596160")*Power(nu,6) + mpfc("2539404055156972150784")*Power(nu,7) + mpfc("62308727274852078592")*Power(nu,8) + mpfc("1220228499806421248")*Power(nu,9) + mpfc("19072810042823680")*Power(nu,10) + mpfc("236592469558016")*Power(nu,11) + mpfc("2300675232128")*Power(nu,12) + mpfc("17170230112")*Power(nu,13) + mpfc("94976960")*Power(nu,14) + mpfc("366928")*Power(nu,15) + mpfc("884")*Power(nu,16) + Power(nu,17)) + mpfc("4537567650")*Power(lambda,17)*(mpfc("3699005117660052080830709760000") + mpfc("1363972991440324221904040755200")*nu + mpfc("236863095359576388344060313600")*Power(nu,2) + mpfc("25739483089906487001977192448")*Power(nu,3) + mpfc("1961428633263016150930292736")*Power(nu,4) + mpfc("111305480810033759656476672")*Power(nu,5) + mpfc("4874681098315491207675904")*Power(nu,6) + mpfc("168471168442002514722816")*Power(nu,7) + mpfc("4657900782501942822912")*Power(nu,8) + mpfc("103796496268270401024")*Power(nu,9) + mpfc("1868704041262426368")*Power(nu,10) + mpfc("27116954007796224")*Power(nu,11) + mpfc("314815427450368")*Power(nu,12) + mpfc("2884463055936")*Power(nu,13) + mpfc("20399446752")*Power(nu,14) + mpfc("107452512")*Power(nu,15) + mpfc("396984")*Power(nu,16) + mpfc("918")*Power(nu,17) + Power(nu,18)) + mpfc("4059928950")*Power(lambda,16)*(mpfc("118368163765121666586582712320000") + mpfc("47346140843750427181760013926400")*nu + mpfc("8943592042946768648913970790400")*Power(nu,2) + mpfc("1060526554236583972407330471936")*Power(nu,3) + mpfc("88505199354323003831746560000")*Power(nu,4) + mpfc("5523204019184096459937546240")*Power(nu,5) + mpfc("267295275956129478302105600")*Power(nu,6) + mpfc("10265758488459571678806016")*Power(nu,7) + mpfc("317523993482064685056000")*Power(nu,8) + mpfc("7979388663086595655680")*Power(nu,9) + mpfc("163595025588668044800")*Power(nu,10) + mpfc("2736446569511905536")*Power(nu,11) + mpfc("37191047686208000")*Power(nu,12) + mpfc("407118245240320")*Power(nu,13) + mpfc("3537245352000")*Power(nu,14) + mpfc("23837927136")*Power(nu,15) + mpfc("120156000")*Power(nu,16) + mpfc("426360")*Power(nu,17) + mpfc("950")*Power(nu,18) + Power(nu,19)) + mpfc("3247943160")*Power(lambda,15)*(mpfc("3551044912953649997597481369600000") + mpfc("1538752389077634482039383130112000")*nu + mpfc("315653902132153486649179137638400")*Power(nu,2) + mpfc("40759388670044287821133884948480")*Power(nu,3) + mpfc("3715682534866274087359727271936")*Power(nu,4) + mpfc("254201319929845897629872947200")*Power(nu,5) + mpfc("13542062297867980809000714240")*Power(nu,6) + mpfc("575268030609916628666286080")*Power(nu,7) + mpfc("19791478292921512230486016")*Power(nu,8) + mpfc("556905653374662554726400")*Power(nu,9) + mpfc("12887239430746636999680")*Power(nu,10) + mpfc("245688422674025210880")*Power(nu,11) + mpfc("3852178000098145536")*Power(nu,12) + mpfc("49404595043417600")*Power(nu,13) + mpfc("513235605800320")*Power(nu,14) + mpfc("4252383166080")*Power(nu,15) + mpfc("27442607136")*Power(nu,16) + mpfc("132946800")*Power(nu,17) + mpfc("454860")*Power(nu,18) + mpfc("980")*Power(nu,19) + Power(nu,20)) + mpfc("2319959400")*Power(lambda,14)*(mpfc("99429257562702199932729478348800000") + mpfc("46636111807127415494700209012736000")*nu + mpfc("10377061648777932108216398983987200")*Power(nu,2) + mpfc("1456916784893393545640927916195840")*Power(nu,3) + mpfc("144798499646299962267206248562688")*Power(nu,4) + mpfc("10833319492901959220996169793536")*Power(nu,5) + mpfc("633379064270149360281892945920")*Power(nu,6) + mpfc("29649567154945646411656724480")*Power(nu,7) + mpfc("1129429422811718971119894528")*Power(nu,8) + mpfc("35384836587412063762825216")*Power(nu,9) + mpfc("917748357435568390717440")*Power(nu,10) + mpfc("19766515265619342904320")*Power(nu,11) + mpfc("353549406676773285888")*Power(nu,12) + mpfc("5235506661313838336")*Power(nu,13) + mpfc("63775192005826560")*Power(nu,14) + mpfc("632302334450560")*Power(nu,15) + mpfc("5020776165888")*Power(nu,16) + mpfc("31165117536")*Power(nu,17) + mpfc("145682880")*Power(nu,18) + mpfc("482300")*Power(nu,19) + mpfc("1008")*Power(nu,20) + Power(nu,21)) + mpfc("1476337800")*Power(lambda,13)*(mpfc("2585160696630257198250966437068800000") + mpfc("1311968164548015002794934912679936000")*nu + mpfc("316439714675353650308326582596403200")*Power(nu,2) + mpfc("48256898056006164294880524805079040")*Power(nu,3) + mpfc("5221677775697192564588290378825728")*Power(nu,4) + mpfc("426464806461750902013106663194624")*Power(nu,5) + mpfc("27301175163925842588325386387456")*Power(nu,6) + mpfc("1404267810298736166984967782400")*Power(nu,7) + mpfc("59014732148050339660773982208")*Power(nu,8) + mpfc("2049435174084432628953350144")*Power(nu,9) + mpfc("59246293880736841921478656")*Power(nu,10) + mpfc("1431677754341671306229760")*Power(nu,11) + mpfc("28958799839215448337408")*Power(nu,12) + mpfc("489672579870933082624")*Power(nu,13) + mpfc("6893661653465328896")*Power(nu,14) + mpfc("80215052701541120")*Power(nu,15) + mpfc("762842514763648")*Power(nu,16) + mpfc("5831069221824")*Power(nu,17) + mpfc("34952872416")*Power(nu,18) + mpfc("158222680")*Power(nu,19) + mpfc("508508")*Power(nu,20) + mpfc("1034")*Power(nu,21) + Power(nu,22)) + mpfc("834451800")*Power(lambda,12)*(mpfc("62043856719126172758023194489651200000") + mpfc("34072396645782617265329404341387264000")*nu + mpfc("8906521316756502610194772894993612800")*Power(nu,2) + mpfc("1474605268019501593385459177918300160")*Power(nu,3) + mpfc("173577164672738785844999493896896512")*Power(nu,4) + mpfc("15456833130779214212902850295496704")*Power(nu,5) + mpfc("1081693010395971124132915936493568")*Power(nu,6) + mpfc("61003602611095510595964613165056")*Power(nu,7) + mpfc("2820621381851944318843543355392")*Power(nu,8) + mpfc("108201176326076722755654385664")*Power(nu,9) + mpfc("3471346227222116835068837888")*Power(nu,10) + mpfc("93606559984936953270992896")*Power(nu,11) + mpfc("2126688950482842066327552")*Power(nu,12) + mpfc("40710941756117842320384")*Power(nu,13) + mpfc("655120459554100976128")*Power(nu,14) + mpfc("8818822918302315776")*Power(nu,15) + mpfc("98523273055868672")*Power(nu,16) + mpfc("902788176087424")*Power(nu,17) + mpfc("6669938159808")*Power(nu,18) + mpfc("38750216736")*Power(nu,19) + mpfc("170426872")*Power(nu,20) + mpfc("533324")*Power(nu,21) + mpfc("1058")*Power(nu,22) + Power(nu,23)) + mpfc("417225900")*Power(lambda,11)*(mpfc("1364964847820775800676510278772326400000") + mpfc("811636582926343752595270090000171008000")*nu + mpfc("230015865614425674689614408031246745600")*Power(nu,2) + mpfc("41347837213185537664674874809196216320")*Power(nu,3) + mpfc("5293302890819754881975448043650023424")*Power(nu,4) + mpfc("513627493549881498528862200397824000")*Power(nu,5) + mpfc("39254079359490578943827000898355200")*Power(nu,6) + mpfc("2423772267840072357244137426124800")*Power(nu,7) + mpfc("123057273011838285610522566983680")*Power(nu,8) + mpfc("5201047261025632219467939840000")*Power(nu,9) + mpfc("184570793324963293127168819200")*Power(nu,10) + mpfc("5530690546890729807030681600")*Power(nu,11) + mpfc("140393716895559478730199040")*Power(nu,12) + mpfc("3022329669117434597376000")*Power(nu,13) + mpfc("55123591866308063795200")*Power(nu,14) + mpfc("849134563756751923200")*Power(nu,15) + mpfc("10986334925531426560")*Power(nu,16) + mpfc("118384612929792000")*Power(nu,17) + mpfc("1049526815603200")*Power(nu,18) + mpfc("7522442928000")*Power(nu,19) + mpfc("42499607920")*Power(nu,20) + mpfc("182160000")*Power(nu,21) + mpfc("556600")*Power(nu,22) + mpfc("1080")*Power(nu,23) + Power(nu,24)) + mpfc("183579396")*Power(lambda,10)*(mpfc("27299296956415516013530205575446528000000") + mpfc("17597696506347650852581912078775746560000")*nu + mpfc("5411953895214857246387558250625105920000")*Power(nu,2) + mpfc("1056972609878136427983111904215171072000")*Power(nu,3) + mpfc("147213895029580635304183835682196684800")*Power(nu,4) + mpfc("15565852761817384852552692051606503424")*Power(nu,5) + mpfc("1298709080739693077405402218364928000")*Power(nu,6) + mpfc("87729524716292026088709749420851200")*Power(nu,7) + mpfc("4884917728076838069454588765798400")*Power(nu,8) + mpfc("227078218232350929999881363783680")*Power(nu,9) + mpfc("8892463127524898082011316224000")*Power(nu,10) + mpfc("295184604262777889267782451200")*Power(nu,11) + mpfc("8338564884801919381634662400")*Power(nu,12) + mpfc("200840310277908170677719040")*Power(nu,13) + mpfc("4124801506443595873280000")*Power(nu,14) + mpfc("72106283141443102259200")*Power(nu,15) + mpfc("1068861262267380454400")*Power(nu,16) + mpfc("13354027184127266560")*Power(nu,17) + mpfc("139375149241856000")*Power(nu,18) + mpfc("1199975674163200")*Power(nu,19) + mpfc("8372435086400")*Power(nu,20) + mpfc("46142807920")*Power(nu,21) + mpfc("193292000")*Power(nu,22) + mpfc("578200")*Power(nu,23) + mpfc("1100")*Power(nu,24) + Power(nu,25)) + mpfc("70607460")*Power(lambda,9)*(mpfc("491387345215479288243543700358037504000000") + mpfc("344057834070673231360004622993409966080000")*nu + mpfc("115012866620215081287557960590027653120000")*Power(nu,2) + mpfc("24437460873021312950083572526498185216000")*Power(nu,3) + mpfc("3706822720410587863458420946494711398400")*Power(nu,4) + mpfc("427399244742293562650132292611113746432")*Power(nu,5) + mpfc("38942616215131860245849931982175207424")*Power(nu,6) + mpfc("2877840525632949547002177707940249600")*Power(nu,7) + mpfc("175658043821675111338892347205222400")*Power(nu,8) + mpfc("8972325656259154809452453313904640")*Power(nu,9) + mpfc("387142554527799095476085055815680")*Power(nu,10) + mpfc("14205786004254900088831400345600")*Power(nu,11) + mpfc("445278772189212438137206374400")*Power(nu,12) + mpfc("11953690469804266453833605120")*Power(nu,13) + mpfc("275086737393892896396759040")*Power(nu,14) + mpfc("5422714602989571713945600")*Power(nu,15) + mpfc("91345785862255950438400")*Power(nu,16) + mpfc("1309233751581671252480")*Power(nu,17) + mpfc("15862779870480674560")*Power(nu,18) + mpfc("160974711376793600")*Power(nu,19) + mpfc("1350679505718400")*Power(nu,20) + mpfc("9203005628960")*Power(nu,21) + mpfc("49622063920")*Power(nu,22) + mpfc("203699600")*Power(nu,23) + mpfc("598000")*Power(nu,24) + mpfc("1118")*Power(nu,25) + Power(nu,26)) + mpfc("23535820")*Power(lambda,8)*(mpfc("7862197523447668611896699205728600064000000") + mpfc("5996312690346250990003617668252596961280000")*nu + mpfc("2184263699994114531960931992433852416000000")*Power(nu,2) + mpfc("506012240588556088488895121013998616576000")*Power(nu,3) + mpfc("83746624399590718765418307670413567590400")*Power(nu,4) + mpfc("10545210636287284865860537628272531341312")*Power(nu,5) + mpfc("1050481104184403326583731204325917065216")*Power(nu,6) + mpfc("84988064625259052997884775309219201024")*Power(nu,7) + mpfc("5688369226779751328424455263223808000")*Power(nu,8) + mpfc("319215254321821588290131600227696640")*Power(nu,9) + mpfc("15166606528703940337069814206955520")*Power(nu,10) + mpfc("614435130595877496897387461345280")*Power(nu,11) + mpfc("21330246359282299099026702336000")*Power(nu,12) + mpfc("636537819706080701398544056320")*Power(nu,13) + mpfc("16355078268106552796181749760")*Power(nu,14) + mpfc("361850171041726043819888640")*Power(nu,15) + mpfc("6884247176785666920960000")*Power(nu,16) + mpfc("112293525887562690478080")*Power(nu,17) + mpfc("1563038229509362045440")*Power(nu,18) + mpfc("18438375252509372160")*Power(nu,19) + mpfc("182585583468288000")*Power(nu,20) + mpfc("1497927595781760")*Power(nu,21) + mpfc("9996958651680")*Power(nu,22) + mpfc("52881257520")*Power(nu,23) + mpfc("213267600")*Power(nu,24) + mpfc("615888")*Power(nu,25) + mpfc("1134")*Power(nu,26) + Power(nu,27)) + mpfc("6724520")*Power(lambda,7)*(mpfc("110070765328267360566553788880200400896000000") + mpfc("91810575188295182471947346561264957521920000")*nu + mpfc("36576004490263854437456665562326530785280000")*Power(nu,2) + mpfc("9268435068233899770805463686629833048064000")*Power(nu,3) + mpfc("1678464982182826151204751428399788562841600")*Power(nu,4) + mpfc("231379573307612706887465834466229006368768")*Power(nu,5) + mpfc("25251946094868931438032774488835370254336")*Power(nu,6) + mpfc("2240314008938030068554118058654985879552")*Power(nu,7) + mpfc("164625233800175571595827148994352513024")*Power(nu,8) + mpfc("10157382787285253564486297666411560960")*Power(nu,9) + mpfc("531547745723676753009108999125073920")*Power(nu,10) + mpfc("23768698357046225293633238665789440")*Power(nu,11) + mpfc("913058579625829684283761294049280")*Power(nu,12) + mpfc("30241775835167428918606319124480")*Power(nu,13) + mpfc("865508915459572440545088552960")*Power(nu,14) + mpfc("21420980662690717409660190720")*Power(nu,15) + mpfc("458229631516725380713328640")*Power(nu,16) + mpfc("8456356539211544587653120")*Power(nu,17) + mpfc("134176061100693759114240")*Power(nu,18) + mpfc("1821175483044493255680")*Power(nu,19) + mpfc("20994573421065404160")*Power(nu,20) + mpfc("203556569809232640")*Power(nu,21) + mpfc("1637885016905280")*Power(nu,22) + mpfc("10737296256960")*Power(nu,23) + mpfc("55867003920")*Power(nu,24) + mpfc("221890032")*Power(nu,25) + mpfc("631764")*Power(nu,26) + mpfc("1148")*Power(nu,27) + Power(nu,28)) + mpfc("1623160")*Power(lambda,6)*(mpfc("1320849183939208326798645466562404810752000000") + mpfc("1211797667587809550229921947615379891159040000")*nu + mpfc("530722629071461435721427333309183326945280000")*Power(nu,2) + mpfc("147797225309070651687122229801884527362048000")*Power(nu,3) + mpfc("29410014854427813585262480827427295802163200")*Power(nu,4) + mpfc("4455019861874178633854341441994536639266816")*Power(nu,5) + mpfc("534402926446039884143859128332253449420800")*Power(nu,6) + mpfc("52135714202125292260682191192695200808960")*Power(nu,7) + mpfc("4215816814540136927704043846587216035840")*Power(nu,8) + mpfc("286513827247598614369662720991291244544")*Power(nu,9) + mpfc("16535955735969374600595605655912448000")*Power(nu,10) + mpfc("816772126008231456532707863114547200")*Power(nu,11) + mpfc("34725401312556181505038374194380800")*Power(nu,12) + mpfc("1275959889647838831307037123543040")*Power(nu,13) + mpfc("40627882820682298205147381760000")*Power(nu,14) + mpfc("1122560683411861049461010841600")*Power(nu,15) + mpfc("26919736240891421978220134400")*Power(nu,16) + mpfc("559705909987263915765166080")*Power(nu,17) + mpfc("10066469272419869697024000")*Power(nu,18) + mpfc("156030166897227678182400")*Power(nu,19) + mpfc("2073110364097278105600")*Power(nu,20) + mpfc("23437252258776195840")*Power(nu,21) + mpfc("223211190012096000")*Power(nu,22) + mpfc("1766732571988800")*Power(nu,23) + mpfc("11407700304000")*Power(nu,24) + mpfc("58529684304")*Power(nu,25) + mpfc("229471200")*Power(nu,26) + mpfc("645540")*Power(nu,27) + mpfc("1160")*Power(nu,28) + Power(nu,29)) + mpfc("324632")*Power(lambda,5)*(mpfc("13208491839392083267986454665624048107520000000") + mpfc("13438825859817303829097864942716203722342400000")*nu + mpfc("6519023958302423907444195280707213160611840000")*Power(nu,2) + mpfc("2008694882162167952592649631328028600565760000")*Power(nu,3) + mpfc("441897373853348787539747038076157485383680000")*Power(nu,4) + mpfc("73960213473169599923805895247372662194831360")*Power(nu,5) + mpfc("9799049126334577475292932725317071133474816")*Power(nu,6) + mpfc("1055760068467292806750681040259205457510400")*Power(nu,7) + mpfc("94293882347526661537722629658567361167360")*Power(nu,8) + mpfc("7080955087016123071400671056500128481280")*Power(nu,9) + mpfc("451873384607292360375618777550415724544")*Power(nu,10) + mpfc("24703676996051689165922684287057920000")*Power(nu,11) + mpfc("1164026139133793271583091605058355200")*Power(nu,12) + mpfc("47485000209034569818108745429811200")*Power(nu,13) + mpfc("1682238717854661813358510941143040")*Power(nu,14) + mpfc("51853489654800908699757490176000")*Power(nu,15) + mpfc("1391758045820775269243212185600")*Power(nu,16) + mpfc("32516795340764061135871795200")*Power(nu,17) + mpfc("660370602711462612735406080")*Power(nu,18) + mpfc("11626770941392146478848000")*Power(nu,19) + mpfc("176761270538200459238400")*Power(nu,20) + mpfc("2307482886685040064000")*Power(nu,21) + mpfc("25669364158897155840")*Power(nu,22) + mpfc("240878515731984000")*Power(nu,23) + mpfc("1880809575028800")*Power(nu,24) + mpfc("11992997147040")*Power(nu,25) + mpfc("60824396304")*Power(nu,26) + mpfc("235926600")*Power(nu,27) + mpfc("657140")*Power(nu,28) + mpfc("1170")*Power(nu,29) + Power(nu,30)) + mpfc("52360")*Power(lambda,4)*(mpfc("105667934715136666143891637324992384860160000000") + mpfc("120719098717930513900769374207353677886259200000")*nu + mpfc("65591017526236695088651427188373909007237120000")*Power(nu,2) + mpfc("22588583015599767528185392331331441965137920000")*Power(nu,3) + mpfc("5543873872988958252910625935937288483635200000")*Power(nu,4) + mpfc("1033579081638705586930194200055138782942330880")*Power(nu,5) + mpfc("152352606483846219726149357049909231262629888")*Power(nu,6) + mpfc("18245129674072919929298381047390714793558016")*Power(nu,7) + mpfc("1810111127247506099052462077527744346849280")*Power(nu,8) + mpfc("150941523043655646108927998110568389017600")*Power(nu,9) + mpfc("10695942163874461954405621276903454277632")*Power(nu,10) + mpfc("649502800575705873703000251846879084544")*Power(nu,11) + mpfc("34015886109122035338587417127524761600")*Power(nu,12) + mpfc("1543906140806069830127961568496844800")*Power(nu,13) + mpfc("60942909951871864324976832958955520")*Power(nu,14) + mpfc("2097066635093069082956570862551040")*Power(nu,15) + mpfc("62987554021367110853703187660800")*Power(nu,16) + mpfc("1651892408546887758330186547200")*Power(nu,17) + mpfc("37799760162455762037755043840")*Power(nu,18) + mpfc("753384770242599784566190080")*Power(nu,19) + mpfc("13040861105697750152755200")*Power(nu,20) + mpfc("195221133631680779750400")*Power(nu,21) + mpfc("2512837799956217310720")*Power(nu,22) + mpfc("27596392284753027840")*Power(nu,23) + mpfc("255924992332214400")*Power(nu,24) + mpfc("1976753552205120")*Power(nu,25) + mpfc("12479592317472")*Power(nu,26) + mpfc("62711809104")*Power(nu,27) + mpfc("241183720")*Power(nu,28) + mpfc("666500")*Power(nu,29) + mpfc("1178")*Power(nu,30) + Power(nu,31)) + mpfc("6545")*Power(lambda,3)*(mpfc("634007608290819996863349823949954309160960000000") + mpfc("829982527022719749548507882569114452177715200000")*nu + mpfc("514265203875350684432677937337597131929681920000")*Power(nu,2) + mpfc("201122515619835300257763781176362560798064640000")*Power(nu,3) + mpfc("55851826253533517045649147946955172866949120000")*Power(nu,4) + mpfc("11745348362821191774491791136268121181289185280")*Power(nu,5) + mpfc("1947694720541782905287090342354594170518110208")*Power(nu,6) + mpfc("261823384528283739301939643334253520023977984")*Power(nu,7) + mpfc("29105796437557956523613153512557180874653696")*Power(nu,8) + mpfc("2715760265509439975706030066191154680954880")*Power(nu,9) + mpfc("215117176026902417835361725771989114683392")*Power(nu,10) + mpfc("14592958967328697196623622787984728784896")*Power(nu,11) + mpfc("853598117230438085734524754612027654144")*Power(nu,12) + mpfc("43279322953958454319355186538505830400")*Power(nu,13) + mpfc("1909563600517301016077822566250577920")*Power(nu,14) + mpfc("73525309762430278822716258134261760")*Power(nu,15) + mpfc("2474991959221271748078789988515840")*Power(nu,16) + mpfc("72898908472648437403684306944000")*Power(nu,17) + mpfc("1878690969521622330556716810240")*Power(nu,18) + mpfc("42320068783911360745152184320")*Power(nu,19) + mpfc("831629936876786285482721280")*Power(nu,20) + mpfc("14212187907487834831257600")*Power(nu,21) + mpfc("210298160431418083614720")*Power(nu,22) + mpfc("2678416153664735477760")*Power(nu,23) + mpfc("29131942238746314240")*Power(nu,24) + mpfc("267785513645445120")*Power(nu,25) + mpfc("2051631106109952")*Power(nu,26) + mpfc("12855863172096")*Power(nu,27) + mpfc("64158911424")*Power(nu,28) + mpfc("245182720")*Power(nu,29) + mpfc("673568")*Power(nu,30) + mpfc("1184")*Power(nu,31) + Power(nu,32)) + mpfc("595")*Power(lambda,2)*(mpfc("2536030433163279987453399295799817236643840000000") + mpfc("3953937716381698995057381354226412117871820800000")*nu + mpfc("2887043342524122487279219631919502979896442880000")*Power(nu,2) + mpfc("1318755266354691885463733062043047375121940480000")*Power(nu,3) + mpfc("424529820633969368440360372964183252265861120000")*Power(nu,4) + mpfc("102833219704818284143616312492027657592105861120")*Power(nu,5) + mpfc("19536127244988323395640152505686497863361626112")*Power(nu,6) + mpfc("2994988258654917862494848915691608250614022144")*Power(nu,7) + mpfc("378246570278515565396392257384482243522592768")*Power(nu,8) + mpfc("39968837499595716426437273777321799598473216")*Power(nu,9) + mpfc("3576228969617049647047476969279111139688448")*Power(nu,10) + mpfc("273489011896217206621856216923928029822976")*Power(nu,11) + mpfc("18007351436250449539561721806432839401472")*Power(nu,12) + mpfc("1026715409046271903011945500766050975744")*Power(nu,13) + mpfc("50917577356027658383666476803508142080")*Power(nu,14) + mpfc("2203664839567022131368687598787624960")*Power(nu,15) + mpfc("83425277599315365815031418088325120")*Power(nu,16) + mpfc("2766587593111865497693527216291840")*Power(nu,17) + mpfc("80413672350734926725911174184960")*Power(nu,18) + mpfc("2047971244657267773537325547520")*Power(nu,19) + mpfc("45646588531418505887083069440")*Power(nu,20) + mpfc("888478688506737624807751680")*Power(nu,21) + mpfc("15053380549213507165716480")*Power(nu,22) + mpfc("221011825046077025525760")*Power(nu,23) + mpfc("2794943922619720734720")*Power(nu,24) + mpfc("30203084293328094720")*Power(nu,25) + mpfc("275992038069884928")*Power(nu,26) + mpfc("2103054558798336")*Power(nu,27) + mpfc("13112498817792")*Power(nu,28) + mpfc("65139642304")*Power(nu,29) + mpfc("247876992")*Power(nu,30) + mpfc("678304")*Power(nu,31) + mpfc("1188")*Power(nu,32) + Power(nu,33)) + mpfc("35")*lambda*(mpfc("5072060866326559974906798591599634473287680000000") + mpfc("10443905865926677977568162004252641472387481600000")*nu + mpfc("9728024401429943969615820618065418077664706560000")*Power(nu,2) + mpfc("5524553875233506258206685756005597730140323840000")*Power(nu,3) + mpfc("2167814907622630622344453807971413879653662720000")*Power(nu,4) + mpfc("630196260043605936727592997948238567450072842240")*Power(nu,5) + mpfc("141905474194794930934896617503400653318829113344")*Power(nu,6) + mpfc("25526103762298159120629850337069714364589670400")*Power(nu,7) + mpfc("3751481399211948993287633430460572737659207680")*Power(nu,8) + mpfc("458184245277706998249266804939125842719539200")*Power(nu,9) + mpfc("47121295438829815720532227715880021877850112")*Power(nu,10) + mpfc("4123206993409484060291189403126967199334400")*Power(nu,11) + mpfc("309503714768718105700979660536793708625920")*Power(nu,12) + mpfc("20060782254342993345585612807964941352960")*Power(nu,13) + mpfc("1128550563758327219779278454373067259904")*Power(nu,14) + mpfc("55324907035161702646403852001083392000")*Power(nu,15) + mpfc("2370515394765652862998750434964275200")*Power(nu,16) + mpfc("88958452785539096810418472520908800")*Power(nu,17) + mpfc("2927414937813335351145349564661760")*Power(nu,18) + mpfc("84509614840049462272985825280000")*Power(nu,19) + mpfc("2139264421720104785311491686400")*Power(nu,20) + mpfc("47423545908431981136698572800")*Power(nu,21) + mpfc("918585449605164639139184640")*Power(nu,22) + mpfc("15495404199305661216768000")*Power(nu,23) + mpfc("226601712891316466995200")*Power(nu,24) + mpfc("2855350091206376924160")*Power(nu,25) + mpfc("30755068369467864576")*Power(nu,26) + mpfc("280198147187481600")*Power(nu,27) + mpfc("2129279556433920")*Power(nu,28) + mpfc("13242778102400")*Power(nu,29) + mpfc("65635396288")*Power(nu,30) + mpfc("249233600")*Power(nu,31) + mpfc("680680")*Power(nu,32) + mpfc("1190")*Power(nu,33) + Power(nu,34)) + nu*(mpfc("5072060866326559974906798591599634473287680000000") + mpfc("10443905865926677977568162004252641472387481600000")*nu + mpfc("9728024401429943969615820618065418077664706560000")*Power(nu,2) + mpfc("5524553875233506258206685756005597730140323840000")*Power(nu,3) + mpfc("2167814907622630622344453807971413879653662720000")*Power(nu,4) + mpfc("630196260043605936727592997948238567450072842240")*Power(nu,5) + mpfc("141905474194794930934896617503400653318829113344")*Power(nu,6) + mpfc("25526103762298159120629850337069714364589670400")*Power(nu,7) + mpfc("3751481399211948993287633430460572737659207680")*Power(nu,8) + mpfc("458184245277706998249266804939125842719539200")*Power(nu,9) + mpfc("47121295438829815720532227715880021877850112")*Power(nu,10) + mpfc("4123206993409484060291189403126967199334400")*Power(nu,11) + mpfc("309503714768718105700979660536793708625920")*Power(nu,12) + mpfc("20060782254342993345585612807964941352960")*Power(nu,13) + mpfc("1128550563758327219779278454373067259904")*Power(nu,14) + mpfc("55324907035161702646403852001083392000")*Power(nu,15) + mpfc("2370515394765652862998750434964275200")*Power(nu,16) + mpfc("88958452785539096810418472520908800")*Power(nu,17) + mpfc("2927414937813335351145349564661760")*Power(nu,18) + mpfc("84509614840049462272985825280000")*Power(nu,19) + mpfc("2139264421720104785311491686400")*Power(nu,20) + mpfc("47423545908431981136698572800")*Power(nu,21) + mpfc("918585449605164639139184640")*Power(nu,22) + mpfc("15495404199305661216768000")*Power(nu,23) + mpfc("226601712891316466995200")*Power(nu,24) + mpfc("2855350091206376924160")*Power(nu,25) + mpfc("30755068369467864576")*Power(nu,26) + mpfc("280198147187481600")*Power(nu,27) + mpfc("2129279556433920")*Power(nu,28) + mpfc("13242778102400")*Power(nu,29) + mpfc("65635396288")*Power(nu,30) + mpfc("249233600")*Power(nu,31) + mpfc("680680")*Power(nu,32) + mpfc("1190")*Power(nu,33) + Power(nu,34)));
#endif

        std::vector<std::function<mp_float(Real, Real)> > initMoments() {
            std::vector<std::function<mp_float(Real, Real)> > retVal(NM);
            retVal[0] = std::function < mp_float(Real, Real) > (f_0);
            retVal[1] = std::function < mp_float(Real, Real) > (f_1);
            retVal[2] = std::function < mp_float(Real, Real) > (f_2);
            retVal[3] = std::function < mp_float(Real, Real) > (f_3);
            retVal[4] = std::function < mp_float(Real, Real) > (f_4);
            retVal[5] = std::function < mp_float(Real, Real) > (f_5);
            retVal[6] = std::function < mp_float(Real, Real) > (f_6);
            retVal[7] = std::function < mp_float(Real, Real) > (f_7);
            retVal[8] = std::function < mp_float(Real, Real) > (f_8);
            retVal[9] = std::function < mp_float(Real, Real) > (f_9);
            retVal[10] = std::function < mp_float(Real, Real) > (f_10);
            retVal[11] = std::function < mp_float(Real, Real) > (f_11);
            retVal[12] = std::function < mp_float(Real, Real) > (f_12);
            retVal[13] = std::function < mp_float(Real, Real) > (f_13);
            retVal[14] = std::function < mp_float(Real, Real) > (f_14);
            retVal[15] = std::function < mp_float(Real, Real) > (f_15);
            retVal[16] = std::function < mp_float(Real, Real) > (f_16);
            retVal[17] = std::function < mp_float(Real, Real) > (f_17);
            retVal[18] = std::function < mp_float(Real, Real) > (f_18);
            retVal[19] = std::function < mp_float(Real, Real) > (f_19);
            retVal[20] = std::function < mp_float(Real, Real) > (f_20);
            retVal[21] = std::function < mp_float(Real, Real) > (f_21);
            retVal[22] = std::function < mp_float(Real, Real) > (f_22);
            retVal[23] = std::function < mp_float(Real, Real) > (f_23);
            retVal[24] = std::function < mp_float(Real, Real) > (f_24);
            retVal[25] = std::function < mp_float(Real, Real) > (f_25);
            retVal[26] = std::function < mp_float(Real, Real) > (f_26);
            retVal[27] = std::function < mp_float(Real, Real) > (f_27);
#ifdef MULTIPRECISION_NON_CENTRAL_CHI_SQUARED_QUADRATURE
            retVal[28] = std::function < mp_float(Real, Real) > (f_28);
            retVal[29] = std::function < mp_float(Real, Real) > (f_29);
            retVal[30] = std::function < mp_float(Real, Real) > (f_30);
            retVal[31] = std::function < mp_float(Real, Real) > (f_31);
            retVal[32] = std::function < mp_float(Real, Real) > (f_32);
            retVal[33] = std::function < mp_float(Real, Real) > (f_33);
            retVal[34] = std::function < mp_float(Real, Real) > (f_34);
            retVal[35] = std::function < mp_float(Real, Real) > (f_35);

#endif
            return retVal;
        }

        const std::vector<std::function<mp_float(Real, Real)> > moments
                = initMoments();
    }

    GaussNonCentralChiSquaredPolynomial::GaussNonCentralChiSquaredPolynomial(
            Real nu, Real lambda)
            : nu_(nu),
              lambda_(lambda),
              b_(NM / 2, quietNaN()),
              c_(NM / 2, quietNaN()),
              z_(NM / 2, std::vector<mp_float>(
                      NM, quietNaN())) {}

    mp_float GaussNonCentralChiSquaredPolynomial::z(Integer k, Integer i) const {
        if (k == -1) return mp_float(0.0);

        QL_REQUIRE(k < Integer(z_.size()) && i < Integer(z_.front().size()),
                   "Function value not available for k=" << k << " " <<
                                                         "and i=" << i << " .\n" <<
                                                         "Max integration order is " << z_.size());

        if (isNaN(z_[k][i])) {
            if (k == 0)
                z_[k][i] = moments[i](nu_, lambda_);
            else
                z_[k][i] = z(k - 1, i + 1) - alpha_(k - 1) * z(k - 1, i) - beta_(k - 1) * z(k - 2, i);
        }
        return z_[k][i];
    };

    mp_float GaussNonCentralChiSquaredPolynomial::alpha_(Size u) const {
        QL_REQUIRE(u < b_.size(),
                   "Function value not available for u=" << u << " .\n" <<
                                                         "Max integration order is " << b_.size());

        if (isNaN(b_[u])) {
            if (u == 0)
                b_[u] = moments[1](nu_, lambda_);
            else {
                const Integer iu(u);
                b_[u] = -z(iu - 1, iu) / z(iu - 1, iu - 1) + z(iu, iu + 1) / z(iu, iu);
            }
        }
        return b_[u];
    }

    mp_float GaussNonCentralChiSquaredPolynomial::beta_(Size u) const {
        if (u == 0) return mp_float(1.0);

        QL_REQUIRE(u < b_.size(),
                   "Function value not available for u=" << u << " .\n" <<
                                                         "Max integration order is " << c_.size());

        if (isNaN(c_[u])) {
            const Integer iu(u);
            c_[u] = z(iu, iu) / z(iu - 1, iu - 1);
        }
        return c_[u];
    }

    Real GaussNonCentralChiSquaredPolynomial::alpha(Size u) const {
        return convert2Real(alpha_(u));
    }

    Real GaussNonCentralChiSquaredPolynomial::beta(Size u) const {
        return convert2Real(beta_(u));
    }

    Real GaussNonCentralChiSquaredPolynomial::w(Real x) const {
        return NonCentralChiSquareDistribution(nu_, lambda_)(x);
    }

    Real GaussNonCentralChiSquaredPolynomial::mu_0() const {
        return 1.0;
    }
}
