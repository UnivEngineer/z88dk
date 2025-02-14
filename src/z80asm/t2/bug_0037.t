#!/usr/bin/env perl

BEGIN { use lib 't2'; require 'testlib.pl'; }

# BUG_0037 : Symbol already defined error when symbol used in IF expression
path("$test.asm")->spew(<<END);
			IF !NEED_floatpack
				DEFINE	NEED_floatpack
			ENDIF
			defb NEED_floatpack		;; 01
END

run_ok("./z88dk-z80asm -b $test.asm");
check_bin_file("$test.bin", bytes(1));
ok unlink("$test.bin");

run_ok("./z88dk-z80asm -b -DNEED_floatpack $test.asm");
check_bin_file("$test.bin", bytes(1));

unlink_testfiles;
done_testing;
