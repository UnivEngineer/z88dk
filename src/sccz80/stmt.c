/*
 *      Small C+ Compiler
 *      Split into parts djm 3/3/99
 *
 *      This part deals with statements
 *
 *      $Id: stmt.c,v 1.5 2001-04-27 14:45:28 dom Exp $
 */

#include "ccdefs.h"

/*
 *      Some variables for goto and cleaning up after compound 
 *      statements
 */

extern void    dogoto(void);
extern int     dolabel(void);
int     stkstor[MAX_LEVELS];    /* ZSp for each compound level */
int	lastline=0;




/*
 *      Statement parser
 *
 * called whenever syntax requires a statement.
 * this routine performs that statement
 * and returns a number telling which one
 */
int statement()
{
        TAG_SYMBOL *otag ;
        int st ;
        struct varid var;
        char locstatic;  /* have we had the static keyword */


        blanks() ;
	if (lineno != lastline ) {
		lastline=lineno;
		EmitLine(lineno);
	}
        if ( ch()==0 && eof )
                return (lastst=STEXP) ;
        else {
		char regit=NO;
/* Ignore the register and auto keywords! */
                regit=locstatic= ( (swallow("register")) | swallow("auto") );

/* Check to see if specified as static, and also for far and near */
                if (amatch("static") ) {
                        if (locstatic) {
                                warning(W_BADSTC);
                                locstatic=NO;
                        } else  locstatic=YES;
                } else locstatic=NO;
/*
 * Now get the identity, STATIK is for struct definitions
 */
                otag=GetVarID(&var,STATIK);

                if (var.type==STRUCT) {
                           declloc(STRUCT, otag,var.sign,locstatic,var.zfar) ;
                           return(lastst);
                } else if (var.type || regit) {
			  if (regit && var.type == NO ) var.type=CINT;
                          declloc(var.type, NULL_TAG,var.sign,locstatic,var.zfar);
                          return (lastst) ;
                }

                /* not a definition */
                if ( declared >= 0 ) {
                        if (lstdecl) postlabel(lstlab);
                        lstdecl=0;
                        Zsp = modstk(Zsp-declared, NO,NO) ;
                        declared = -1 ;
                }
                st = -1 ;
                switch ( ch() ) {
                case '{' :
                        inbyte() ;
                        compound() ;
                        st = lastst ;
                        break ;
                case 'i' :
			if ( amatch("iferror") ) {
				doiferror();
				st=STIF;
                        } else if ( amatch("if") ) {
                                doif() ;
                                st = STIF ;
                        }
                        break ;
                case 'w' :
                        if ( amatch("while") ) {
                                dowhile() ;
                                st = STWHILE;
                        }
                        break ;
                case 'd' :
                        if ( amatch("do") ) {
                                dodo() ;
                                st = STDO ;
                        }
                        else if ( amatch("default") ) {
                                dodefault() ;
                                st = STDEF ;
                        }
                        break ;
                case 'f' :
                        if ( amatch("for") ) {
                                dofor() ;
                                st = STFOR ;
                        }
                        break ;
                case 's' :
                        if ( amatch("switch") ) {
                                doswitch() ;
                                st = STSWITCH ;
                        }
                        break ;
                case 'c' :
                        if ( amatch("case") ) {
                                docase() ;
                                st = STCASE;
                        }
                        else if ( amatch("continue") ) {
                                docont() ;
                                ns() ;
                                st = STCONT ;
                        }
                        break ;
                case 'r' :
                        if ( amatch("return") ) {
                                doreturn(0) ;
                                ns() ;
                                st = STRETURN ;
                        } else if ( amatch("return_c") ) {
				doreturn(1);
				ns();
				st= STRETURN;
			} else if ( amatch("return_nc") ) {
				doreturn(2);
				ns();
				st=STRETURN;
			}
                        break ;
                case 'b' :
                        if ( amatch("break") ) {
                                dobreak() ;
                                ns() ;
                                st = STBREAK ;
                        }
                        break ;
                case ';' :
                        inbyte() ;
                        st = lastst ;
                        break ;
                case 'a' :
                        if ( amatch("asm") ) {
                                doasmfunc(YES);
                                ns();
                                st=STASM;
                        }
                        break;
                case 'g' :
                        if ( amatch("goto") ) {
                                dogoto();
                                ns();
                                st=STGOTO;
                        }
                        break;
                case '#' :
                        if ( match("#asm") ) {
                                doasm() ;
                                st = STASM;
                        }
                        break ;
                }
                if ( st == -1 ) {
                        /* if nothing else, assume it's an expression */
                        if (dolabel() == 0 ) {
                                doexpr() ;
                                ns() ;
                        }
                        st = STEXP ;
                }
        }
        return (lastst = st) ;
}

/*
 *      Semicolon enforcer
 *
 * called whenever syntax requires a semicolon
 */
void ns()
{
        if ( cmatch(';') == 0 )
                warning(W_EXPSEMI);
}

/*
 *      Compound statement
 *
 * allow any number of statements to fall between "{}"
 */
void compound()
{
        SYMBOL *savloc ;

        if      (ncmp == MAX_LEVELS ) 
                error(E_MAXLEVELS,ncmp);
        
        stkstor[ncmp]=Zsp;
        savloc = locptr ;
        declared = 0 ;          /* may declare local variables */
        ++ncmp;                         /* new level open */
        while (cmatch('}')==0) statement(); /* do one */
        --ncmp;                         /* close current level */
        if ( lastst != STRETURN ) {
                modstk(stkstor[ncmp],NO,NO) ;             /* delete local variable space */
        }
        Zsp = stkstor[ncmp];
        locptr = savloc ;       /* delete local symbols */
        declared = -1 ;
}

/*
 *	"iferror" statement
 *	This is z88dk specific and is used to check for
 *	an error from a package call..highly non standard!
 *
 *	I sense getting into trouble with purists but trying
 *	to combine C and asm compactly and efficiently requires
 * 	this sort of extension (much like return_c/_nc
 */

void doiferror()
{
	int	flab1,flab2;
	flab1=getlabel();	/* Get label for false branch */
	jumpnc(flab1);
	statement();
        if ( amatch("else") == 0 ) {
                /* no else, print false label and exit  */
                postlabel(flab1);
                return;
        }
        /* an "if...else" statement. */
        flab2 = getlabel() ;
        if ( lastst != STRETURN ) {
                /* if last statement of 'if' was 'return' we needn't skip 'else' code */
                jump(flab2);
        }
        postlabel(flab1);                               /* print false label */
        statement();                                    /* and do 'else' clause */
	postlabel(flab2);
}

/*
 *              "if" statement
 */
void doif()
{
        int flab1,flab2;

        flab1=getlabel();       /* get label for false branch */
        test(flab1,YES);        /* get expression, and branch false */
        statement();            /* if true, do a statement */
        if ( amatch("else") == 0 ) {
                /* no else, print false label and exit  */
                postlabel(flab1);
                return;
        }
        /* an "if...else" statement. */
        flab2 = getlabel() ;
        if ( lastst != STRETURN ) {
                /* if last statement of 'if' was 'return' we needn't skip 'else' code */
                jump(flab2);
        }
        postlabel(flab1);                               /* print false label */
        statement();                                    /* and do 'else' clause */
        postlabel(flab2);                               /* print true label */
}




/*
 * perform expression (including commas)
 */
int doexpr()
{
        char *before, *start ;
        int type, vconst, val ;

        while (1) {
                setstage(&before, &start) ;
                type = expression(&vconst, &val) ;
                clearstage( before, start ) ;
                if ( ch() != ',' ) return type ;
                inbyte() ;
        }
}

/*
 *      "while" statement
 */
void dowhile()
{
        WHILE_TAB wq ;          /* allocate local queue */

        addwhile(&wq) ;                 /* add entry to queue */
                                                        /* (for "break" statement) */
        postlabel(wq.loop) ;    /* loop label */
        test(wq.exit, YES) ;    /* see if true */
        statement() ;                   /* if so, do a statement */
        jump(wq.loop) ;                 /* loop to label */
        postlabel(wq.exit) ;    /* exit label */
        delwhile() ;                    /* delete queue entry */
}

/*
 * "do - while" statement
 */
void dodo()
{
        WHILE_TAB wq ;
        int top ;

        addwhile(&wq) ;
        postlabel(top=getlabel()) ;
        statement() ;
        needtoken("while") ;
        postlabel(wq.loop) ;
        test(wq.exit, YES) ;
        jump(top);
        postlabel(wq.exit) ;
        delwhile() ;
        ns() ;
}

/*
 * "for" statement
 */
void dofor()
{
        WHILE_TAB wq ;
        int lab1, lab2 ;

        addwhile(&wq) ;
        lab1 = getlabel() ;
        lab2 = getlabel() ;
        needchar('(') ;
        if (cmatch(';') == 0 ) {
                doexpr() ;                              /* expr 1 */
                ns() ;
        }
        postlabel(lab1) ;
        if ( cmatch(';') == 0 ) {
                test(wq.exit, NO ) ;    /* expr 2 */
                ns() ;
        }
        jump(lab2) ;
        postlabel(wq.loop) ;
        if ( cmatch(')') == 0 ) {
                doexpr() ;                              /* expr 3 */
                needchar(')') ;
        }
        jump(lab1) ;
        postlabel(lab2) ;
        statement() ;
        jump(wq.loop) ;
        postlabel(wq.exit) ;
        delwhile() ;
}

/*
 * "switch" statement
 */
void doswitch()
{
        WHILE_TAB wq ;
        int endlab, swact, swdef ;
        SW_TAB *swnex, *swptr ;
        char    swtype;   /* type of switch statement - CINT/LONG */

        swact = swactive ;
        swdef = swdefault ;
        swnex = swptr = swnext ;
        addwhile(&wq) ;
        (wqptr-1)->loop = 0 ;
        needchar('(') ;
        swtype=doexpr() ;                      /* evaluate switch expression */
        needchar(')') ;
        swdefault = 0 ;
        swactive = 1 ;
        jump(endlab=getlabel()) ;
        statement() ;           /* cases, etc. */
        jump(wq.exit) ;
        postlabel(endlab) ;
        if (swtype==CCHAR) {
                LoadAccum();
                while (swptr < swnext) {
                        CpCharVal(swptr->value);
                        opjump("z,",swptr->label);
                        ++swptr;
                }
        } else {
                sw(swtype) ;                          /* insert code to match cases */
                while ( swptr < swnext ) {
                        defword() ;
                        printlabel(swptr->label) ;              /* case label */
                        if (swtype==LONG) {
                                outbyte('\n');
                                deflong();
                        }else
                                outbyte(',') ;
                        outdec(swptr->value) ;                  /* case value */
                        nl() ;
                        ++swptr ;
                }
                defword() ;
                outdec(0) ;
                nl() ;
        }
        if (swdefault) jump(swdefault) ;
        postlabel(wq.exit) ;
        delwhile() ;
        swnext = swnex ;
        swdefault = swdef ;
        swactive = swact ;
}

/*
 * "case" statement
 */
void docase()
{
        if (swactive == 0) error(E_SWITCH) ;
        if (swnext > swend ) {
                error(E_CASE) ;
                return ;
        }
        postlabel(swnext->label = getlabel()) ;
        constexpr(&swnext->value,1) ;
        needchar(':') ;
        ++swnext ;
}

void dodefault()
{
        if (swactive) {
                if (swdefault) error(E_DEFAULT) ;
        }
        else error(E_SWITCH) ;
        needchar(':') ;
        postlabel(swdefault=getlabel()) ;
}
        

/*
 *      "return" statement
 */
void doreturn(char type)
{
        /* if not end of statement, get an expression */
        if ( endst() == 0 ) {
                if ( currfn->more ) {
                        /* return pointer to value */
                        force(CINT, doexpr(),YES,dosigned,0) ;
                }
                else {
                        /* return actual value */
                        force(currfn->type, doexpr(), currfn->flags&UNSIGNED, dosigned,0) ;
                }
                leave(YES,type);
        }
        else leave(NO,type) ;
}

/*
 * leave a function
 * preserve primary register if save is TRUE
 */
void leave(save,type)
int save ;
char type;	/* 1=c, 2=nc 0=don't bother */
{
        int savesp;
        modstk(0,save,NO); /* clean up stk */
        if ( (compactcode || currfn->flags&CALLEE)  && (stackargs>2) ) {
/* 
 * We're exiting a function and we want to clean up after ourselves
 * (so calling function doesn't have to do this) (first of all we
 * have to grab the return address - easy just to exchange
 */
                savesp=Zsp;
                doexx();
                zpop();         /* Return address in de */
                Zsp-=stackargs-2;
                modstk(0,NO,NO);
                zpushde();
                doexx();
                Zsp=savesp;
        }
#ifdef USEFRAME
	popframe();		/* Restore previous frame pointer */
#endif
	if (type) setcond(type);
        zret();         /* and exit function */
}

/*
 *      "break" statement
 */
void dobreak()
{
        WHILE_TAB *ptr ;

        /* see if any "whiles" are open */
        if ((ptr=readwhile(wqptr))==0) return;  /* no */
        modstk(ptr->sp, NO,NO);    /* else clean up stk ptr */
        jump(ptr->exit) ;               /* jump to exit label */
}

/*
 *      "continue" statement
 */
void docont()
{
        WHILE_TAB *ptr;

        /* see if any "whiles" are open */
        ptr = wqptr ;
        while (1) {
                if ((ptr=readwhile(ptr))==0) return ;
                /* try again if loop is zero (that's a switch statement) */
                if ( ptr->loop ) break ;
        }
        modstk(ptr->sp, NO,NO) ;   /* else clean up stk ptr */
        jump(ptr->loop) ;               /* jump to loop label */
}

/*
 *      asm()   statement
 *
 *      This doesn't do any label expansions - just chucks it out to
 *      the file, it might be useful for setting a value to machine
 *      code thing
 *      
 *      If wantbr==YES then we need the opening bracket (called by
 *      itself)
 *
 *      Actually, this caution may be unneccesary because it is also
 *      dealt with as a function, we'll just have to see - i.e. maybe
 *      it doesn't have to be statement as well!
 *
 *      New: 3/3/99 djm
 */

void doasmfunc(char wantbr)
{
        char c;
        if (wantbr) needchar('(');

        outbyte('\t');
        needchar('"');
        do {
                while (!cmatch('"')) {
                        c=litchar();
                        if (c == 0)
                                break;
                        outbyte(c);
                        if ( c == 10 || c == 13 ) outstr("\n\t");
                }
        } while (cmatch ('"'));
        needchar (')');
        outbyte('\n');
}



/*
 *      "asm" pseudo-statement (for #asm/#endasm)
 *      enters mode where assembly language statement are
 *      passed intact through parser
 */


void doasm()
{
        char label[NAMEMAX];
        int     k;
	char	lab=0;		/* Got an good asm label */
        SYMBOL *myptr;
        endasm=cmode=0;         /* mark mode as "asm" */
#ifdef INBUILT_OPTIMIZER
        generate();             /* Dump queued stuff to be opt'd */
#endif

        while (1) {
                preprocess();   /* get and print lines */
                if ( endasm || match("#endasm") || eof ) {
                        break ;
		}
/*
 * This is only relevent for z80asm since asxxx doesn't differentiate
 * between different types of .globl and assemble time
 *
 * If the first column contains a "." then it's a lebel and we 
 * should check for the magic prefix ("smc_" or "_") and compare
 * to symbol table. If match then the symbol will be XDEF'd instead
 * of XREF'd
 */
                if (line[0] == '.')
                {
                        k=1;
                        while ((line[k] != ' ') && (line[k] != '\n')  && (line[k] != '\0') && (line[k] != '\t') )
                               {
                                label[k-1]=line[k];
                                k++;
                                }
                        label[k-1]='\0';
/*
 * Snagged a label, we check form smc_ or _ prefix if so remove
 * it and check for symbol then switch to local defn
 *
 * Lab is where the actual label name starts from
 */
			lab=0;
			if (label[0]=='_') {
				strcpy(label,label+1);
				lab=2; 
                        } else if ( strncmp(label,"smc_",4)==0 ) {
                         	strcpy(label,label+4);
				lab=5;
			}
			if (lab) {
/* ATP Got assembler label, now check to see if defined as extern.. */
                          if ( (myptr=findglb(label)) ) {
/* Have matched it to an extern, so now change type... */
                                if (myptr->storage == EXTERNAL)
                                        myptr->storage = DECLEXTN;
                          }
			  line[1]='_';
			  strcpy(line+2,&line[(int)lab]);
			}
                }
        	if ( output != NULL_FD ) {
        		if ( fputs(line, output) == -1 ) {
               			 fabort() ;
                	}
                	fputc('\n',output);
        	} else 
			puts(line);

        }
/* Print the line out, with the appropriate prefix */


	clear() ;               /* invalidate line */
        if (eof) warning(W_UNTERM);
        cmode=1 ;               /* then back to parse level */
}

/* #pragma statement */


void dopragma()
{
        if ( amatch("proto") ) addmac() ;
        else if ( amatch("set") ) addmac();
        else if ( amatch("unproto") ) delmac() ;
        else if ( amatch("unset") ) delmac();
        else if ( amatch("asm") ) doasm();
        else if ( amatch("endasm") ) endasm=1; 
	else if ( amatch("output") ) WriteDefined(line+lptr+1,0);
	else if ( amatch("string") ) PragmaOutput(line+lptr+1);
	else if ( amatch("data") ) PragmaBytes(1);
	else if ( amatch("byte") ) PragmaBytes(0);
        else if ( rcmatch('-') ) ParseArgs(line+lptr+1);
        else {
               warning(W_PRAGMA);
               junk();
               vinline();
        }
}

