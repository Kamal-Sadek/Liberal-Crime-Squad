/**
 * @file test-no-dice.cpp
 * @brief Test driver for the LCS unit test suite.
 *
 * Copyright 2017 Stephen M. Webb  <stephen.webb@bregmasoft.ca>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of Version 2 of the GNU General Public License as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */
#define CATCH_CONFIG_RUNNER
#include "catch/catch.hpp"
#include "catch/catch_reporter_tap.hpp"


int
main(int argc, char* argv[])
{
  Catch::Session session;

  int return_code = session.applyCommandLine(argc, argv);
  if (return_code != 0)
    return return_code;

  session.configData().reporterNames.push_back("tap");

  int num_failed = session.run();
  return num_failed < 0xff ? num_failed : 0xff;
}
