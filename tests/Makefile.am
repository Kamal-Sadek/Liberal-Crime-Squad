#
# @file tests/Makefile.am
# @brief automake recipe for LCS unit tests
#
# Copyright 2017 Stephen M. Webb  <stephen.webb@bregmasoft.ca>
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.
#

check_PROGRAMS = test-lcs

test_lcs_SOURCES = \
  test-lcs.cpp

test_lcs_CPPFLAGS = \
  -I$(top_srcdir) \
  -I$(top_srcdir)/tests/include

LOG_DRIVER = env AM_TAP_AWK='$(AWK)' $(SHELL) $(top_srcdir)/config.aux/tap-driver.sh

TESTS = $(check_PROGRAMS)

EXTRA_DIST = include
