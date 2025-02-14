#!/usr/bin/perl

# Z88DK Z80 Macro Assembler
#
# Copyright (C) Gunther Strube, InterLogic 1993-99
# Copyright (C) Paulo Custodio, 2011-2022
# License: The Artistic License 2.0, http://www.perlfoundation.org/artistic_license_2_0
# Repository: https://github.com/z88dk/z88dk

use Modern::Perl;
use Test::More;
require './t/test_utils.pl';

# -DVAR
t_z80asm(
	asm		=> "define VAR",
	err		=> "test.asm:1: error: duplicate definition: VAR\n  ^---- define VAR",
	options	=> "-DVAR"
);

t_z80asm(
	asm		=> "defb VAR",
	bin		=> "\1",
	options	=> "-DVAR"
);

t_z80asm(
	asm		=> "defc VAR=2",
	err		=> "test.asm:1: error: duplicate definition: VAR\n  ^---- defc VAR=2",
	options	=> "-DVAR"
);

t_z80asm(
	asm		=> "VAR: nop",
	err		=> "test.asm:1: error: duplicate definition: VAR\n  ^---- VAR: nop",
	options	=> "-DVAR"
);

t_z80asm(
	asm		=> "PUBLIC VAR : defb VAR",
	bin		=> "\1",
	options	=> "-DVAR"
);

t_z80asm(
	asm		=> "EXTERN VAR",
	err		=> "test.asm:1: error: symbol redeclaration: VAR\n  ^---- EXTERN VAR",
	options	=> "-DVAR"
);

# define VAR
t_z80asm(
	asm		=> "define VAR : defb VAR",
	bin		=> "\1",
);

t_z80asm(
	asm		=> "undefine VAR : defb VAR",
	err		=> "test.asm:2: error: undefined symbol: VAR\n  ^---- VAR",
);

t_z80asm(
	asm		=> "define VAR : undefine VAR : define VAR : defb VAR",
	bin		=> "\1",
);

t_z80asm(
	asm		=> "define VAR : defc VAR=2",
	err		=> "test.asm:2: error: duplicate definition: VAR\n  ^---- defc VAR=2",
);

t_z80asm(
	asm		=> "define VAR : VAR: nop",
	err		=> "test.asm:2: error: duplicate definition: VAR\n  ^---- VAR: nop",
);

t_z80asm(
	asm		=> "define VAR : PUBLIC VAR : defb VAR",
	bin		=> "\1",
);

t_z80asm(
	asm		=> "define VAR : EXTERN VAR",
	err		=> "test.asm:2: error: symbol redeclaration: VAR\n  ^---- EXTERN VAR",
);

# defc VAR
t_z80asm(
	asm		=> "defc VAR=1 : defb VAR",
	bin		=> "\1",
);

t_z80asm(
	asm		=> "defc VAR=1 : define VAR",
	err		=> "test.asm:2: error: duplicate definition: VAR\n  ^---- define VAR",
);

t_z80asm(
	asm		=> "defc VAR=1 : defc VAR=1",
	err		=> "test.asm:2: error: duplicate definition: VAR\n  ^---- defc VAR=1",
);

t_z80asm(
	asm		=> "defc VAR=1 : VAR: nop",
	err		=> "test.asm:2: error: duplicate definition: VAR\n  ^---- VAR: nop",
);

t_z80asm(
	asm		=> "defc VAR=1 : PUBLIC VAR : defb VAR",
	bin		=> "\1",
);

t_z80asm(
	asm		=> "defc VAR=1 : EXTERN VAR",
	err		=> "test.asm:2: error: symbol redeclaration: VAR\n  ^---- EXTERN VAR",
);

t_z80asm(
	asm		=> "PUBLIC am48_dcmp : EXTERN mm48_cmp : defc am48_dcmp = mm48_cmp",
	asm1	=> "PUBLIC mm48_cmp : mm48_cmp: ret",
	bin		=> "\xC9",
);

# VAR:
t_z80asm(
	asm		=> "VAR: : defb VAR",
	bin		=> "\0",
);

t_z80asm(
	asm		=> "defb VAR : VAR:",
	bin		=> "\1",
);

t_z80asm(
	asm		=> "VAR: : define VAR",
	err		=> "test.asm:2: error: duplicate definition: VAR\n  ^---- define VAR",
);

t_z80asm(
	asm		=> "VAR: : defc VAR=1",
	err		=> "test.asm:2: error: duplicate definition: VAR\n  ^---- defc VAR=1",
);

t_z80asm(
	asm		=> "VAR: : VAR: nop",
	err		=> "test.asm:2: error: duplicate definition: VAR\n  ^---- VAR: nop",
);

t_z80asm(
	asm		=> "VAR: : PUBLIC VAR : defb VAR",
	bin		=> "\0",
);

t_z80asm(
	asm		=> "PUBLIC VAR : VAR: : defb VAR",
	bin		=> "\0",
);

t_z80asm(
	asm		=> "VAR: : EXTERN VAR",
	err		=> "test.asm:2: error: symbol redeclaration: VAR\n  ^---- EXTERN VAR",
);

# PUBLIC
t_z80asm(
	asm		=> "PUBLIC VAR : PUBLIC VAR : DEFC VAR = 3 : defb VAR",
	asm1	=> "EXTERN VAR : defb VAR",
	bin		=> "\3\3",
);

t_z80asm(
	asm		=> "PUBLIC VAR : EXTERN VAR : DEFC VAR = 3 : defb VAR",
	asm1	=> "EXTERN VAR : defb VAR",
	bin		=> "\3\3",
);

t_z80asm(
	asm		=> "PUBLIC VAR : GLOBAL VAR : DEFC VAR = 3 : defb VAR",
	asm1	=> "EXTERN VAR : defb VAR",
	bin		=> "\3\3",
);

# EXTERN
t_z80asm(
	asm		=> "EXTERN VAR : PUBLIC VAR : DEFC VAR = 3 : defb VAR",
	asm1	=> "EXTERN VAR : defb VAR",
	bin		=> "\3\3",
);

t_z80asm(
	asm		=> "EXTERN VAR : EXTERN VAR : VAR: defb VAR",	# local hides global
	bin		=> "\0",
);

t_z80asm(
	asm		=> "EXTERN VAR : GLOBAL VAR : DEFC VAR = 3 : defb VAR",
	asm1	=> "EXTERN VAR : defb VAR",
	bin		=> "\3\3",
);

# GLOBAL
t_z80asm(
	asm		=> "GLOBAL VAR : PUBLIC VAR : DEFC VAR = 3 : defb VAR",
	asm1	=> "EXTERN VAR : defb VAR",
	bin		=> "\3\3",
);

t_z80asm(
	asm		=> "GLOBAL VAR : EXTERN VAR : DEFC VAR = 3 : defb VAR",
	asm1	=> "EXTERN VAR : defb VAR",
	bin		=> "\3\3",
);

t_z80asm(
	asm		=> "GLOBAL VAR : GLOBAL VAR : DEFC VAR = 3 : defb VAR",
	asm1	=> "EXTERN VAR : defb VAR",
	bin		=> "\3\3",
);

# Symbol redefined
t_z80asm(
	asm		=> "PUBLIC VAR : defc VAR=3 : defb VAR",
	asm1	=> "PUBLIC VAR : defc VAR=3 : defb VAR",
	linkerr	=> "test1.asm: error: duplicate definition: test::VAR",
);

# Symbol declared global in another module
t_z80asm(
	asm		=> "PUBLIC VAR : defc VAR=2",
	asm1	=> "PUBLIC VAR : defc VAR=3",
	linkerr	=> "test1.asm: error: duplicate definition: test::VAR",
);

# Case-sensitive symbols
unlink_testfiles();
write_file(asm_file(), " Defc Loc = 1 \n DEFC LOC = 2 \n ".
					   " Public Loc, LOC \n".
					   " ld a, Loc \n ld a, LOC");
write_file(asm1_file(), "EXTERN Loc, LOC \n ld a, Loc \n ld a, LOC");
t_z80asm_capture("-l -b ".asm_file()." ".asm1_file(), "", "", 0);
t_binary(read_binfile(bin_file()), "\x3E\x01\x3E\x02\x3E\x01\x3E\x02");

# delete directories and files
unlink_testfiles();
done_testing;
