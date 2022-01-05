#!/usr/bin/env perl

BEGIN { use lib 't2'; require 'testlib.pl'; }

# BUG_0003 : Illegal options are ignored, although ReportError 9 (Illegal Option) exists
capture_nok("./z88dk-z80asm -h=x", <<END);
error: illegal option: -h=x
END

unlink_testfiles;
done_testing;
