#!/usr/bin/env perl

BEGIN { use lib 't2'; require 'testlib.pl'; }

# BUG_0032 : DEFGROUP ignores name after assignment
z80asm_ok("", "", "", <<END, bytes(10,11));
		defgroup
		{
			f10 = 10, f11
		}
		defb f10, f11
END

unlink_testfiles;
done_testing;
