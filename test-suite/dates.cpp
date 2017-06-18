/* -*- mode: c++; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*- */

/*
 Copyright (C) 2004, 2009 Ferdinando Ametrano
 Copyright (C) 2006 Katiuscia Manzoni
 Copyright (C) 2003 RiskMap srl
 Copyright (C) 2015 Maddalena Zanzi
 Copyright (c) 2015 Klaus Spanderen

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

#include "utilities.hpp"
#include <ql/time/date.hpp>
#include <ql/time/imm.hpp>
#include <ql/time/ecb.hpp>
#include <ql/time/asx.hpp>
#include <ql/utilities/dataparsers.hpp>


using namespace QuantLib;


TEST_CASE( "Date_ecbDates", "[Date]" ) {
    INFO("Testing ECB dates...");

    std::set<Date> knownDates = ECB::knownDates();
    if (knownDates.empty())
        FAIL("\nempty EBC date vector");

    Size n = ECB::nextDates(Date::minDate()).size();
    if (n != knownDates.size())
        FAIL("\nnextDates(minDate) returns "  << n <<
                   " instead of " << knownDates.size() << " dates");

    std::set<Date>::const_iterator i;
    Date previousEcbDate = Date::minDate(),
         currentEcbDate, ecbDateMinusOne;
    for (i=knownDates.begin(); i!=knownDates.end(); ++i) {

        currentEcbDate = *i;
        if (!ECB::isECBdate(currentEcbDate))
            FAIL("\n" << currentEcbDate << " fails isECBdate check");

        ecbDateMinusOne = currentEcbDate-1;
        if (ECB::isECBdate(ecbDateMinusOne))
            FAIL("\n" << ecbDateMinusOne << " fails isECBdate check");

        if (ECB::nextDate(ecbDateMinusOne)!=currentEcbDate)
            FAIL("\n next EBC date following " << ecbDateMinusOne <<
                       " must be " << currentEcbDate);

        if (ECB::nextDate(previousEcbDate)!=currentEcbDate)
            FAIL("\n next EBC date following " << previousEcbDate <<
                       " must be " << currentEcbDate);

        previousEcbDate = currentEcbDate;
    }

    Date knownDate = *knownDates.begin();
    ECB::removeDate(knownDate);
    if (ECB::isECBdate(knownDate))
        FAIL("\neunable to remove an EBC date");
    ECB::addDate(knownDate);
    if (!ECB::isECBdate(knownDate))
        FAIL("\neunable to add an EBC date");

}

TEST_CASE( "Date_immDates", "[Date]" ) {
    INFO("Testing IMM dates...");

    const std::string IMMcodes[] = {
        "F0", "G0", "H0", "J0", "K0", "M0", "N0", "Q0", "U0", "V0", "X0", "Z0",
        "F1", "G1", "H1", "J1", "K1", "M1", "N1", "Q1", "U1", "V1", "X1", "Z1",
        "F2", "G2", "H2", "J2", "K2", "M2", "N2", "Q2", "U2", "V2", "X2", "Z2",
        "F3", "G3", "H3", "J3", "K3", "M3", "N3", "Q3", "U3", "V3", "X3", "Z3",
        "F4", "G4", "H4", "J4", "K4", "M4", "N4", "Q4", "U4", "V4", "X4", "Z4",
        "F5", "G5", "H5", "J5", "K5", "M5", "N5", "Q5", "U5", "V5", "X5", "Z5",
        "F6", "G6", "H6", "J6", "K6", "M6", "N6", "Q6", "U6", "V6", "X6", "Z6",
        "F7", "G7", "H7", "J7", "K7", "M7", "N7", "Q7", "U7", "V7", "X7", "Z7",
        "F8", "G8", "H8", "J8", "K8", "M8", "N8", "Q8", "U8", "V8", "X8", "Z8",
        "F9", "G9", "H9", "J9", "K9", "M9", "N9", "Q9", "U9", "V9", "X9", "Z9"
    };

    Date counter = Date::minDate();
    // 10 years of futures must not exceed Date::maxDate
    Date last = Date::maxDate() - 121*Months;
    Date imm;

    while (counter<=last) {
        imm = IMM::nextDate(counter, false);

        // check that imm is greater than counter
        if (imm<=counter)
            FAIL("\n  "
                       << imm.weekday() << " " << imm
                       << " is not greater than "
                       << counter.weekday() << " " << counter);

        // check that imm is an IMM date
        if (!IMM::isIMMdate(imm, false))
            FAIL("\n  "
                       << imm.weekday() << " " << imm
                       << " is not an IMM date (calculated from "
                       << counter.weekday() << " " << counter << ")");

        // check that imm is <= to the next IMM date in the main cycle
        if (imm>IMM::nextDate(counter, true))
            FAIL("\n  "
                       << imm.weekday() << " " << imm
                       << " is not less than or equal to the next future in the main cycle "
                       << IMM::nextDate(counter, true));

        //// check that if counter is an IMM date, then imm==counter
        //if (IMM::isIMMdate(counter, false) && (imm!=counter))
        //    FAIL("\n  "
        //               << counter.weekday() << " " << counter
        //               << " is already an IMM date, while nextIMM() returns "
        //               << imm.weekday() << " " << imm);

        // check that for every date IMMdate is the inverse of IMMcode
        if (IMM::date(IMM::code(imm), counter) != imm)
            FAIL("\n  "
                       << IMM::code(imm)
                       << " at calendar day " << counter
                       << " is not the IMM code matching " << imm);

        // check that for every date the 120 IMM codes refer to future dates
        for (int i=0; i<40; ++i) {
            if (IMM::date(IMMcodes[i], counter)<counter)
                FAIL("\n  "
                       << IMM::date(IMMcodes[i], counter)
                       << " is wrong for " << IMMcodes[i]
                       << " at reference date " << counter);
        }

        counter = counter + 1;
    }
}

TEST_CASE( "Date_asxDates", "[Date]" ) {
    INFO("Testing ASX dates...");

    const std::string ASXcodes[] = {
        "F0", "G0", "H0", "J0", "K0", "M0", "N0", "Q0", "U0", "V0", "X0", "Z0",
        "F1", "G1", "H1", "J1", "K1", "M1", "N1", "Q1", "U1", "V1", "X1", "Z1",
        "F2", "G2", "H2", "J2", "K2", "M2", "N2", "Q2", "U2", "V2", "X2", "Z2",
        "F3", "G3", "H3", "J3", "K3", "M3", "N3", "Q3", "U3", "V3", "X3", "Z3",
        "F4", "G4", "H4", "J4", "K4", "M4", "N4", "Q4", "U4", "V4", "X4", "Z4",
        "F5", "G5", "H5", "J5", "K5", "M5", "N5", "Q5", "U5", "V5", "X5", "Z5",
        "F6", "G6", "H6", "J6", "K6", "M6", "N6", "Q6", "U6", "V6", "X6", "Z6",
        "F7", "G7", "H7", "J7", "K7", "M7", "N7", "Q7", "U7", "V7", "X7", "Z7",
        "F8", "G8", "H8", "J8", "K8", "M8", "N8", "Q8", "U8", "V8", "X8", "Z8",
        "F9", "G9", "H9", "J9", "K9", "M9", "N9", "Q9", "U9", "V9", "X9", "Z9"
    };

    Date counter = Date::minDate();
    // 10 years of futures must not exceed Date::maxDate
    Date last = Date::maxDate() - 121 * Months;
    Date asx;

    while (counter <= last) {
        asx = ASX::nextDate(counter, false);

        // check that asx is greater than counter
        if (asx <= counter)
            FAIL("\n  "
            << asx.weekday() << " " << asx
            << " is not greater than "
            << counter.weekday() << " " << counter);

        // check that asx is an ASX date
        if (!ASX::isASXdate(asx, false))
            FAIL("\n  "
            << asx.weekday() << " " << asx
            << " is not an ASX date (calculated from "
            << counter.weekday() << " " << counter << ")");

        // check that asx is <= to the next ASX date in the main cycle
        if (asx>ASX::nextDate(counter, true))
            FAIL("\n  "
            << asx.weekday() << " " << asx
            << " is not less than or equal to the next future in the main cycle "
            << ASX::nextDate(counter, true));

        //// check that if counter is an ASX date, then asx==counter
        //if (ASX::isASXdate(counter, false) && (asx!=counter))
        //    FAIL("\n  "
        //               << counter.weekday() << " " << counter
        //               << " is already an ASX date, while nextASX() returns "
        //               << asx.weekday() << " " << asx);

        // check that for every date ASXdate is the inverse of ASXcode
        if (ASX::date(ASX::code(asx), counter) != asx)
            FAIL("\n  "
            << ASX::code(asx)
            << " at calendar day " << counter
            << " is not the ASX code matching " << asx);

        // check that for every date the 120 ASX codes refer to future dates
        for (int i = 0; i<120; ++i) {
            if (ASX::date(ASXcodes[i], counter)<counter)
                FAIL("\n  "
                << ASX::date(ASXcodes[i], counter)
                << " is wrong for " << ASXcodes[i]
                << " at reference date " << counter);
        }

        counter = counter + 1;
    }
}

TEST_CASE( "Date_Consistency", "[Date]" ) {

    INFO("Testing dates...");

    Date::serial_type minDate = Date::minDate().serialNumber()+1,
                      maxDate = Date::maxDate().serialNumber();

    Date::serial_type dyold = Date(minDate-1).dayOfYear(),
                      dold  = Date(minDate-1).dayOfMonth(),
                      mold  = Date(minDate-1).month(),
                      yold  = Date(minDate-1).year(),
                      wdold = Date(minDate-1).weekday();

    for (Date::serial_type i=minDate; i<=maxDate; i++) {
        Date t(i);
        Date::serial_type serial = t.serialNumber();

        // check serial number consistency
        if (serial != i)
            FAIL("inconsistent serial number:\n"
                       << "    original:      " << i << "\n"
                       << "    date:          " << t << "\n"
                       << "    serial number: " << serial);

        Integer dy = t.dayOfYear(),
                d  = t.dayOfMonth(),
                m  = t.month(),
                y  = t.year(),
                wd = t.weekday();

        // check if skipping any date
        if (!((dy == dyold+1) ||
              (dy == 1 && dyold == 365 && !Date::isLeap(yold)) ||
              (dy == 1 && dyold == 366 && Date::isLeap(yold))))
            FAIL("wrong day of year increment: \n"
                       << "    date: " << t << "\n"
                       << "    day of year: " << dy << "\n"
                       << "    previous:    " << dyold);
        dyold = dy;

        if (!((d == dold+1 && m == mold   && y == yold) ||
              (d == 1      && m == mold+1 && y == yold) ||
              (d == 1      && m == 1      && y == yold+1)))
            FAIL("wrong day,month,year increment: \n"
                       << "    date: " << t << "\n"
                       << "    day,month,year: "
                       << d << "," << Integer(m) << "," << y << "\n"
                       << "    previous:       "
                       << dold<< "," << Integer(mold) << "," << yold);
        dold = d; mold = m; yold = y;

        // check month definition
        if (m < 1 || m > 12)
            FAIL("invalid month: \n"
                       << "    date:  " << t << "\n"
                       << "    month: " << Integer(m));

        // check day definition
        if (d < 1)
            FAIL("invalid day of month: \n"
                       << "    date:  " << t << "\n"
                       << "    day: " << d);
        if (!((m == 1  && d <= 31) ||
              (m == 2  && d <= 28) ||
              (m == 2  && d == 29 && Date::isLeap(y)) ||
              (m == 3  && d <= 31) ||
              (m == 4  && d <= 30) ||
              (m == 5  && d <= 31) ||
              (m == 6  && d <= 30) ||
              (m == 7  && d <= 31) ||
              (m == 8  && d <= 31) ||
              (m == 9  && d <= 30) ||
              (m == 10 && d <= 31) ||
              (m == 11 && d <= 30) ||
              (m == 12 && d <= 31)))
            FAIL("invalid day of month: \n"
                       << "    date:  " << t << "\n"
                       << "    day: " << d);

        // check weekday definition
        if (!((Integer(wd) == Integer(wdold+1)) ||
              (Integer(wd) == 1 && Integer(wdold) == 7)))
            FAIL("invalid weekday: \n"
                       << "    date:  " << t << "\n"
                       << "    weekday:  " << Integer(wd) << "\n"
                       << "    previous: " << Integer(wdold));
        wdold = wd;

        // create the same date with a different constructor
        Date s(d,Month(m),y);
        // check serial number consistency
        serial = s.serialNumber();
        if (serial != i)
            FAIL("inconsistent serial number:\n"
                       << "    date:          " << t << "\n"
                       << "    serial number: " << i << "\n"
                       << "    cloned date:   " <<  s << "\n"
                       << "    serial number: " << serial);
    }

}

TEST_CASE( "Date_isoDates", "[Date]" ) {
    INFO("Testing ISO dates...");
    std::string input_date("2006-01-15");
    Date d = DateParser::parseISO(input_date);
    if (d.dayOfMonth() != 15 ||
        d.month() != January ||
        d.year() != 2006) {
        FAIL("Iso date failed\n"
                   << " input date:    " << input_date << "\n"
                   << " day of month:  " << d.dayOfMonth() << "\n"
                   << " month:         " << d.month() << "\n"
                   << " year:          " << d.year());
    }
}

TEST_CASE( "Date_parseDates", "[Date]" ) {
    INFO("Testing parsing of dates...");

    std::string input_date("2006-01-15");
    Date d = DateParser::parseFormatted(input_date, "%Y-%m-%d");
    if (d != Date(15, January, 2006)) {
        FAIL("date parsing failed\n"
                   << " input date:  " << input_date << "\n"
                   << " parsed date: " << d);
    }

    input_date = "12/02/2012";
    d = DateParser::parseFormatted(input_date, "%m/%d/%Y");
    if (d != Date(2, December, 2012)) {
        FAIL("date parsing failed\n"
                   << " input date:  " << input_date << "\n"
                   << " parsed date: " << d);
    }
    d = DateParser::parseFormatted(input_date, "%d/%m/%Y");
    if (d != Date(12, February, 2012)) {
        FAIL("date parsing failed\n"
                   << " input date:  " << input_date << "\n"
                   << " parsed date: " << d);
    }

    input_date = "20011002";
    d = DateParser::parseFormatted(input_date, "%Y%m%d");
    if (d != Date(2, October, 2001)) {
        FAIL("date parsing failed\n"
                   << " input date:  " << input_date << "\n"
                   << " parsed date: " << d);
    }
}

TEST_CASE( "Date_intraday", "[Date]" ) {
#ifdef QL_HIGH_RESOLUTION_DATE

    INFO("Testing intraday information of dates...");

    const Date d1 = Date(12, February, 2015, 10, 45, 12, 1234, 76253);
    if (d1.year() != 2015)
	    FAIL_CHECK("failed to reproduce year");
    if (d1.month() != February)
	    FAIL_CHECK("failed to reproduce month");
    if (d1.dayOfMonth() != 12)
	    FAIL_CHECK("failed to reproduce day");
    if (d1.hours() != 10)
	    FAIL_CHECK("failed to reproduce hour of day");
    if (d1.minutes() != 45)
	    FAIL_CHECK("failed to reproduce minute of hour");
    if (d1.seconds() != 13)
	    FAIL_CHECK("failed to reproduce second of minute");

    if (Date::ticksPerSecond() == 1000)
	    if (d1.fractionOfSecond() != 0.234)
		    FAIL_CHECK("failed to reproduce fraction of second");
    else if (Date::ticksPerSecond() >= 1000000)
	    if (d1.fractionOfSecond() != (234000 + 76253)/1000000.0)
		    FAIL_CHECK("failed to reproduce fraction of second");

    if (Date::ticksPerSecond() >= 1000)
	    if (d1.milliseconds() != 234 + 76)
		    FAIL_CHECK("failed to reproduce number of milliseconds");

    if (Date::ticksPerSecond() >= 1000000)
	    if (d1.microseconds() != 253)
		    FAIL_CHECK("failed to reproduce number of microseconds");

    const Date d2 = Date(28, February, 2015, 50, 165, 476, 1234, 253);
    if (d2.year() != 2015)
	   FAIL_CHECK("failed to reproduce year");
    if (d2.month() != March)
	   FAIL_CHECK("failed to reproduce month");
    if (d2.dayOfMonth() != 2)
	   FAIL_CHECK("failed to reproduce day");
    if (d2.hours() != 4)
	   FAIL_CHECK("failed to reproduce hour of day");
    if (d2.minutes() != 52)
	    FAIL_CHECK("failed to reproduce minute of hour");
    if (d2.seconds() != 57)
	    FAIL_CHECK("failed to reproduce second of minute");

    if (Date::ticksPerSecond() >= 1000)
        if (d2.milliseconds() != 234)
		FAIL_CHECK("failed to reproduce number of milliseconds");
    if (Date::ticksPerSecond() >= 1000000)
        if (d2.microseconds() != 253)
		FAIL_CHECK("failed to reproduce number of microseconds");

    std::ostringstream s;
    s << io::iso_datetime(Date(7, February, 2015, 1, 4, 2, 3, 4));

    if (s.str() != std::string("2015-02-07T01:04:02,003004"))
	    FAIL_CHECK("datetime to string failed to reproduce expected result");

#endif
}

