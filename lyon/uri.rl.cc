
#line 1 "uri.rl"
#include "uri.h"
#include <sstream>
namespace lyon {


#line 9 "uri.rl.cc"
static const int uri_parser_start = 453;
static const int uri_parser_first_final = 453;
static const int uri_parser_error = 0;

static const int uri_parser_en_main = 453;


#line 125 "uri.rl"


Uri::ptr Uri::Parser(const std::string &uristr) {
    Uri::ptr uri(new Uri());
    int cs = 0;
    const char *mark = 0;
    
#line 25 "uri.rl.cc"
	{
	cs = uri_parser_start;
	}

#line 132 "uri.rl"
    const char *p = uristr.c_str();
    const char *pe = p + uristr.size();
    const char *eof = pe;
    
#line 35 "uri.rl.cc"
	{
	if ( p == pe )
		goto _test_eof;
	switch ( cs )
	{
case 453:
	switch( (*p) ) {
		case 34: goto st0;
		case 35: goto st455;
		case 37: goto st3;
		case 47: goto st460;
		case 58: goto st0;
		case 60: goto st0;
		case 62: goto st0;
		case 63: goto st458;
		case 96: goto st0;
		case 127: goto st0;
	}
	if ( (*p) < 91 ) {
		if ( (*p) > 32 ) {
			if ( 65 <= (*p) && (*p) <= 90 )
				goto tr461;
		} else if ( (*p) >= 0 )
			goto st0;
	} else if ( (*p) > 94 ) {
		if ( (*p) > 122 ) {
			if ( 123 <= (*p) && (*p) <= 125 )
				goto st0;
		} else if ( (*p) >= 97 )
			goto tr461;
	} else
		goto st0;
	goto st454;
st454:
	if ( ++p == pe )
		goto _test_eof454;
case 454:
	switch( (*p) ) {
		case 34: goto st0;
		case 35: goto st455;
		case 37: goto st3;
		case 47: goto st457;
		case 58: goto st0;
		case 60: goto st0;
		case 62: goto st0;
		case 63: goto st458;
		case 96: goto st0;
		case 127: goto st0;
	}
	if ( (*p) < 91 ) {
		if ( 0 <= (*p) && (*p) <= 32 )
			goto st0;
	} else if ( (*p) > 94 ) {
		if ( 123 <= (*p) && (*p) <= 125 )
			goto st0;
	} else
		goto st0;
	goto st454;
st0:
cs = 0;
	goto _out;
tr467:
#line 17 "uri.rl"
	{ mark = p; }
#line 96 "uri.rl"
	{
        // std::cout << std::string(mark, fpc - mark) << std::endl;
        uri->setQuery(std::string(mark, p - mark));
        mark = NULL;
    }
	goto st455;
tr469:
#line 96 "uri.rl"
	{
        // std::cout << std::string(mark, fpc - mark) << std::endl;
        uri->setQuery(std::string(mark, p - mark));
        mark = NULL;
    }
	goto st455;
tr475:
#line 18 "uri.rl"
	{ mark = p; }
#line 40 "uri.rl"
	{
        if (mark != NULL) {
            // std::cout << std::string(mark, fpc - mark) << std::endl;
            uri->setHost(std::string(mark, p - mark));
        }
    }
	goto st455;
tr486:
#line 40 "uri.rl"
	{
        if (mark != NULL) {
            // std::cout << std::string(mark, fpc - mark) << std::endl;
            uri->setHost(std::string(mark, p - mark));
        }
    }
	goto st455;
tr490:
#line 18 "uri.rl"
	{ mark = p; }
#line 27 "uri.rl"
	{
        if (p != mark) {
            uri->setPort(atoi(mark));
        }
        mark = NULL;
    }
	goto st455;
tr494:
#line 27 "uri.rl"
	{
        if (p != mark) {
            uri->setPort(atoi(mark));
        }
        mark = NULL;
    }
	goto st455;
tr554:
#line 18 "uri.rl"
	{ mark = p; }
#line 40 "uri.rl"
	{
        if (mark != NULL) {
            // std::cout << std::string(mark, fpc - mark) << std::endl;
            uri->setHost(std::string(mark, p - mark));
        }
    }
#line 73 "uri.rl"
	{
        // std::cout << std::string(mark, fpc - mark) << std::endl;
        uri->setPath(std::string(mark, p - mark));
        mark = NULL;
    }
	goto st455;
tr565:
#line 40 "uri.rl"
	{
        if (mark != NULL) {
            // std::cout << std::string(mark, fpc - mark) << std::endl;
            uri->setHost(std::string(mark, p - mark));
        }
    }
#line 18 "uri.rl"
	{ mark = p; }
#line 73 "uri.rl"
	{
        // std::cout << std::string(mark, fpc - mark) << std::endl;
        uri->setPath(std::string(mark, p - mark));
        mark = NULL;
    }
	goto st455;
tr569:
#line 73 "uri.rl"
	{
        // std::cout << std::string(mark, fpc - mark) << std::endl;
        uri->setPath(std::string(mark, p - mark));
        mark = NULL;
    }
	goto st455;
tr572:
#line 18 "uri.rl"
	{ mark = p; }
#line 27 "uri.rl"
	{
        if (p != mark) {
            uri->setPort(atoi(mark));
        }
        mark = NULL;
    }
#line 73 "uri.rl"
	{
        // std::cout << std::string(mark, fpc - mark) << std::endl;
        uri->setPath(std::string(mark, p - mark));
        mark = NULL;
    }
	goto st455;
tr576:
#line 27 "uri.rl"
	{
        if (p != mark) {
            uri->setPort(atoi(mark));
        }
        mark = NULL;
    }
#line 18 "uri.rl"
	{ mark = p; }
#line 73 "uri.rl"
	{
        // std::cout << std::string(mark, fpc - mark) << std::endl;
        uri->setPath(std::string(mark, p - mark));
        mark = NULL;
    }
	goto st455;
st455:
	if ( ++p == pe )
		goto _test_eof455;
case 455:
#line 235 "uri.rl.cc"
	switch( (*p) ) {
		case 37: goto tr463;
		case 60: goto st0;
		case 62: goto st0;
		case 96: goto st0;
		case 127: goto st0;
	}
	if ( (*p) < 34 ) {
		if ( 0 <= (*p) && (*p) <= 32 )
			goto st0;
	} else if ( (*p) > 35 ) {
		if ( (*p) > 94 ) {
			if ( 123 <= (*p) && (*p) <= 125 )
				goto st0;
		} else if ( (*p) >= 91 )
			goto st0;
	} else
		goto st0;
	goto tr462;
tr462:
#line 17 "uri.rl"
	{ mark = p; }
	goto st456;
st456:
	if ( ++p == pe )
		goto _test_eof456;
case 456:
#line 263 "uri.rl.cc"
	switch( (*p) ) {
		case 37: goto st1;
		case 60: goto st0;
		case 62: goto st0;
		case 96: goto st0;
		case 127: goto st0;
	}
	if ( (*p) < 34 ) {
		if ( 0 <= (*p) && (*p) <= 32 )
			goto st0;
	} else if ( (*p) > 35 ) {
		if ( (*p) > 94 ) {
			if ( 123 <= (*p) && (*p) <= 125 )
				goto st0;
		} else if ( (*p) >= 91 )
			goto st0;
	} else
		goto st0;
	goto st456;
tr463:
#line 17 "uri.rl"
	{ mark = p; }
	goto st1;
st1:
	if ( ++p == pe )
		goto _test_eof1;
case 1:
#line 291 "uri.rl.cc"
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st2;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st2;
	} else
		goto st2;
	goto st0;
st2:
	if ( ++p == pe )
		goto _test_eof2;
case 2:
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st456;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st456;
	} else
		goto st456;
	goto st0;
st3:
	if ( ++p == pe )
		goto _test_eof3;
case 3:
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st4;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st4;
	} else
		goto st4;
	goto st0;
st4:
	if ( ++p == pe )
		goto _test_eof4;
case 4:
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st454;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st454;
	} else
		goto st454;
	goto st0;
tr477:
#line 18 "uri.rl"
	{ mark = p; }
#line 40 "uri.rl"
	{
        if (mark != NULL) {
            // std::cout << std::string(mark, fpc - mark) << std::endl;
            uri->setHost(std::string(mark, p - mark));
        }
    }
	goto st457;
tr487:
#line 40 "uri.rl"
	{
        if (mark != NULL) {
            // std::cout << std::string(mark, fpc - mark) << std::endl;
            uri->setHost(std::string(mark, p - mark));
        }
    }
	goto st457;
tr491:
#line 18 "uri.rl"
	{ mark = p; }
#line 27 "uri.rl"
	{
        if (p != mark) {
            uri->setPort(atoi(mark));
        }
        mark = NULL;
    }
	goto st457;
tr495:
#line 27 "uri.rl"
	{
        if (p != mark) {
            uri->setPort(atoi(mark));
        }
        mark = NULL;
    }
	goto st457;
st457:
	if ( ++p == pe )
		goto _test_eof457;
case 457:
#line 384 "uri.rl.cc"
	switch( (*p) ) {
		case 34: goto st0;
		case 35: goto st455;
		case 37: goto st5;
		case 60: goto st0;
		case 62: goto st0;
		case 63: goto st458;
		case 96: goto st0;
		case 127: goto st0;
	}
	if ( (*p) < 91 ) {
		if ( 0 <= (*p) && (*p) <= 32 )
			goto st0;
	} else if ( (*p) > 94 ) {
		if ( 123 <= (*p) && (*p) <= 125 )
			goto st0;
	} else
		goto st0;
	goto st457;
st5:
	if ( ++p == pe )
		goto _test_eof5;
case 5:
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st6;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st6;
	} else
		goto st6;
	goto st0;
st6:
	if ( ++p == pe )
		goto _test_eof6;
case 6:
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st457;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st457;
	} else
		goto st457;
	goto st0;
tr483:
#line 18 "uri.rl"
	{ mark = p; }
#line 40 "uri.rl"
	{
        if (mark != NULL) {
            // std::cout << std::string(mark, fpc - mark) << std::endl;
            uri->setHost(std::string(mark, p - mark));
        }
    }
	goto st458;
tr489:
#line 40 "uri.rl"
	{
        if (mark != NULL) {
            // std::cout << std::string(mark, fpc - mark) << std::endl;
            uri->setHost(std::string(mark, p - mark));
        }
    }
	goto st458;
tr493:
#line 18 "uri.rl"
	{ mark = p; }
#line 27 "uri.rl"
	{
        if (p != mark) {
            uri->setPort(atoi(mark));
        }
        mark = NULL;
    }
	goto st458;
tr497:
#line 27 "uri.rl"
	{
        if (p != mark) {
            uri->setPort(atoi(mark));
        }
        mark = NULL;
    }
	goto st458;
tr562:
#line 18 "uri.rl"
	{ mark = p; }
#line 40 "uri.rl"
	{
        if (mark != NULL) {
            // std::cout << std::string(mark, fpc - mark) << std::endl;
            uri->setHost(std::string(mark, p - mark));
        }
    }
#line 73 "uri.rl"
	{
        // std::cout << std::string(mark, fpc - mark) << std::endl;
        uri->setPath(std::string(mark, p - mark));
        mark = NULL;
    }
	goto st458;
tr568:
#line 40 "uri.rl"
	{
        if (mark != NULL) {
            // std::cout << std::string(mark, fpc - mark) << std::endl;
            uri->setHost(std::string(mark, p - mark));
        }
    }
#line 18 "uri.rl"
	{ mark = p; }
#line 73 "uri.rl"
	{
        // std::cout << std::string(mark, fpc - mark) << std::endl;
        uri->setPath(std::string(mark, p - mark));
        mark = NULL;
    }
	goto st458;
tr571:
#line 73 "uri.rl"
	{
        // std::cout << std::string(mark, fpc - mark) << std::endl;
        uri->setPath(std::string(mark, p - mark));
        mark = NULL;
    }
	goto st458;
tr575:
#line 18 "uri.rl"
	{ mark = p; }
#line 27 "uri.rl"
	{
        if (p != mark) {
            uri->setPort(atoi(mark));
        }
        mark = NULL;
    }
#line 73 "uri.rl"
	{
        // std::cout << std::string(mark, fpc - mark) << std::endl;
        uri->setPath(std::string(mark, p - mark));
        mark = NULL;
    }
	goto st458;
tr579:
#line 27 "uri.rl"
	{
        if (p != mark) {
            uri->setPort(atoi(mark));
        }
        mark = NULL;
    }
#line 18 "uri.rl"
	{ mark = p; }
#line 73 "uri.rl"
	{
        // std::cout << std::string(mark, fpc - mark) << std::endl;
        uri->setPath(std::string(mark, p - mark));
        mark = NULL;
    }
	goto st458;
st458:
	if ( ++p == pe )
		goto _test_eof458;
case 458:
#line 550 "uri.rl.cc"
	switch( (*p) ) {
		case 34: goto st0;
		case 35: goto tr467;
		case 37: goto tr468;
		case 60: goto st0;
		case 62: goto st0;
		case 96: goto st0;
		case 127: goto st0;
	}
	if ( (*p) < 91 ) {
		if ( 0 <= (*p) && (*p) <= 32 )
			goto st0;
	} else if ( (*p) > 94 ) {
		if ( 123 <= (*p) && (*p) <= 125 )
			goto st0;
	} else
		goto st0;
	goto tr466;
tr466:
#line 17 "uri.rl"
	{ mark = p; }
	goto st459;
st459:
	if ( ++p == pe )
		goto _test_eof459;
case 459:
#line 577 "uri.rl.cc"
	switch( (*p) ) {
		case 34: goto st0;
		case 35: goto tr469;
		case 37: goto st7;
		case 60: goto st0;
		case 62: goto st0;
		case 96: goto st0;
		case 127: goto st0;
	}
	if ( (*p) < 91 ) {
		if ( 0 <= (*p) && (*p) <= 32 )
			goto st0;
	} else if ( (*p) > 94 ) {
		if ( 123 <= (*p) && (*p) <= 125 )
			goto st0;
	} else
		goto st0;
	goto st459;
tr468:
#line 17 "uri.rl"
	{ mark = p; }
	goto st7;
st7:
	if ( ++p == pe )
		goto _test_eof7;
case 7:
#line 604 "uri.rl.cc"
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st8;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st8;
	} else
		goto st8;
	goto st0;
st8:
	if ( ++p == pe )
		goto _test_eof8;
case 8:
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st459;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st459;
	} else
		goto st459;
	goto st0;
st460:
	if ( ++p == pe )
		goto _test_eof460;
case 460:
	switch( (*p) ) {
		case 34: goto st0;
		case 35: goto st455;
		case 37: goto st5;
		case 47: goto st461;
		case 60: goto st0;
		case 62: goto st0;
		case 63: goto st458;
		case 96: goto st0;
		case 127: goto st0;
	}
	if ( (*p) < 91 ) {
		if ( 0 <= (*p) && (*p) <= 32 )
			goto st0;
	} else if ( (*p) > 94 ) {
		if ( 123 <= (*p) && (*p) <= 125 )
			goto st0;
	} else
		goto st0;
	goto st457;
st461:
	if ( ++p == pe )
		goto _test_eof461;
case 461:
	switch( (*p) ) {
		case 2: goto tr472;
		case 32: goto tr473;
		case 33: goto tr474;
		case 35: goto tr475;
		case 37: goto tr476;
		case 47: goto tr477;
		case 49: goto tr479;
		case 50: goto tr480;
		case 57: goto tr481;
		case 58: goto tr482;
		case 59: goto tr474;
		case 61: goto tr474;
		case 63: goto tr483;
		case 64: goto tr484;
		case 91: goto tr485;
		case 95: goto tr474;
		case 126: goto tr474;
	}
	if ( (*p) < 48 ) {
		if ( 36 <= (*p) && (*p) <= 46 )
			goto tr474;
	} else if ( (*p) > 56 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr474;
		} else if ( (*p) >= 65 )
			goto tr474;
	} else
		goto tr478;
	goto st0;
tr472:
#line 18 "uri.rl"
	{ mark = p; }
	goto st9;
st9:
	if ( ++p == pe )
		goto _test_eof9;
case 9:
#line 694 "uri.rl.cc"
	switch( (*p) ) {
		case 32: goto st10;
		case 48: goto st10;
		case 52: goto st10;
	}
	goto st0;
tr473:
#line 18 "uri.rl"
	{ mark = p; }
	goto st10;
st10:
	if ( ++p == pe )
		goto _test_eof10;
case 10:
#line 709 "uri.rl.cc"
	if ( 48 <= (*p) && (*p) <= 57 )
		goto st11;
	goto st0;
st11:
	if ( ++p == pe )
		goto _test_eof11;
case 11:
	if ( (*p) == 46 )
		goto st12;
	goto st0;
st12:
	if ( ++p == pe )
		goto _test_eof12;
case 12:
	switch( (*p) ) {
		case 2: goto st13;
		case 32: goto st14;
		case 49: goto st28;
		case 50: goto st30;
		case 57: goto st29;
	}
	if ( 48 <= (*p) && (*p) <= 56 )
		goto st15;
	goto st0;
st13:
	if ( ++p == pe )
		goto _test_eof13;
case 13:
	switch( (*p) ) {
		case 32: goto st14;
		case 48: goto st14;
		case 52: goto st14;
	}
	goto st0;
st14:
	if ( ++p == pe )
		goto _test_eof14;
case 14:
	if ( 48 <= (*p) && (*p) <= 57 )
		goto st15;
	goto st0;
st15:
	if ( ++p == pe )
		goto _test_eof15;
case 15:
	if ( (*p) == 46 )
		goto st16;
	goto st0;
st16:
	if ( ++p == pe )
		goto _test_eof16;
case 16:
	switch( (*p) ) {
		case 2: goto st17;
		case 32: goto st18;
		case 49: goto st24;
		case 50: goto st26;
		case 57: goto st25;
	}
	if ( 48 <= (*p) && (*p) <= 56 )
		goto st19;
	goto st0;
st17:
	if ( ++p == pe )
		goto _test_eof17;
case 17:
	switch( (*p) ) {
		case 32: goto st18;
		case 48: goto st18;
		case 52: goto st18;
	}
	goto st0;
st18:
	if ( ++p == pe )
		goto _test_eof18;
case 18:
	if ( 48 <= (*p) && (*p) <= 57 )
		goto st19;
	goto st0;
st19:
	if ( ++p == pe )
		goto _test_eof19;
case 19:
	if ( (*p) == 46 )
		goto st20;
	goto st0;
st20:
	if ( ++p == pe )
		goto _test_eof20;
case 20:
	switch( (*p) ) {
		case 2: goto st21;
		case 32: goto st22;
		case 49: goto st465;
		case 50: goto st467;
		case 57: goto st466;
	}
	if ( 48 <= (*p) && (*p) <= 56 )
		goto st462;
	goto st0;
st21:
	if ( ++p == pe )
		goto _test_eof21;
case 21:
	switch( (*p) ) {
		case 32: goto st22;
		case 48: goto st22;
		case 52: goto st22;
	}
	goto st0;
st22:
	if ( ++p == pe )
		goto _test_eof22;
case 22:
	if ( 48 <= (*p) && (*p) <= 57 )
		goto st462;
	goto st0;
st462:
	if ( ++p == pe )
		goto _test_eof462;
case 462:
	switch( (*p) ) {
		case 35: goto tr486;
		case 47: goto tr487;
		case 58: goto tr488;
		case 63: goto tr489;
	}
	goto st0;
tr508:
#line 18 "uri.rl"
	{ mark = p; }
#line 40 "uri.rl"
	{
        if (mark != NULL) {
            // std::cout << std::string(mark, fpc - mark) << std::endl;
            uri->setHost(std::string(mark, p - mark));
        }
    }
	goto st463;
tr488:
#line 40 "uri.rl"
	{
        if (mark != NULL) {
            // std::cout << std::string(mark, fpc - mark) << std::endl;
            uri->setHost(std::string(mark, p - mark));
        }
    }
	goto st463;
st463:
	if ( ++p == pe )
		goto _test_eof463;
case 463:
#line 862 "uri.rl.cc"
	switch( (*p) ) {
		case 35: goto tr490;
		case 47: goto tr491;
		case 63: goto tr493;
	}
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr492;
	goto st0;
tr492:
#line 18 "uri.rl"
	{ mark = p; }
	goto st464;
st464:
	if ( ++p == pe )
		goto _test_eof464;
case 464:
#line 879 "uri.rl.cc"
	switch( (*p) ) {
		case 35: goto tr494;
		case 47: goto tr495;
		case 63: goto tr497;
	}
	if ( 48 <= (*p) && (*p) <= 57 )
		goto st464;
	goto st0;
st465:
	if ( ++p == pe )
		goto _test_eof465;
case 465:
	switch( (*p) ) {
		case 35: goto tr486;
		case 47: goto tr487;
		case 58: goto tr488;
		case 63: goto tr489;
	}
	if ( 48 <= (*p) && (*p) <= 57 )
		goto st466;
	goto st0;
st466:
	if ( ++p == pe )
		goto _test_eof466;
case 466:
	switch( (*p) ) {
		case 35: goto tr486;
		case 47: goto tr487;
		case 58: goto tr488;
		case 63: goto tr489;
	}
	if ( 48 <= (*p) && (*p) <= 57 )
		goto st462;
	goto st0;
st467:
	if ( ++p == pe )
		goto _test_eof467;
case 467:
	switch( (*p) ) {
		case 35: goto tr486;
		case 47: goto tr487;
		case 53: goto st23;
		case 58: goto tr488;
		case 63: goto tr489;
	}
	goto st0;
st23:
	if ( ++p == pe )
		goto _test_eof23;
case 23:
	switch( (*p) ) {
		case 32: goto st462;
		case 48: goto st462;
		case 53: goto st462;
	}
	goto st0;
st24:
	if ( ++p == pe )
		goto _test_eof24;
case 24:
	if ( (*p) == 46 )
		goto st20;
	if ( 48 <= (*p) && (*p) <= 57 )
		goto st25;
	goto st0;
st25:
	if ( ++p == pe )
		goto _test_eof25;
case 25:
	if ( (*p) == 46 )
		goto st20;
	if ( 48 <= (*p) && (*p) <= 57 )
		goto st19;
	goto st0;
st26:
	if ( ++p == pe )
		goto _test_eof26;
case 26:
	switch( (*p) ) {
		case 46: goto st20;
		case 53: goto st27;
	}
	goto st0;
st27:
	if ( ++p == pe )
		goto _test_eof27;
case 27:
	switch( (*p) ) {
		case 32: goto st19;
		case 48: goto st19;
		case 53: goto st19;
	}
	goto st0;
st28:
	if ( ++p == pe )
		goto _test_eof28;
case 28:
	if ( (*p) == 46 )
		goto st16;
	if ( 48 <= (*p) && (*p) <= 57 )
		goto st29;
	goto st0;
st29:
	if ( ++p == pe )
		goto _test_eof29;
case 29:
	if ( (*p) == 46 )
		goto st16;
	if ( 48 <= (*p) && (*p) <= 57 )
		goto st15;
	goto st0;
st30:
	if ( ++p == pe )
		goto _test_eof30;
case 30:
	switch( (*p) ) {
		case 46: goto st16;
		case 53: goto st31;
	}
	goto st0;
st31:
	if ( ++p == pe )
		goto _test_eof31;
case 31:
	switch( (*p) ) {
		case 32: goto st15;
		case 48: goto st15;
		case 53: goto st15;
	}
	goto st0;
tr474:
#line 18 "uri.rl"
	{ mark = p; }
	goto st468;
st468:
	if ( ++p == pe )
		goto _test_eof468;
case 468:
#line 1018 "uri.rl.cc"
	switch( (*p) ) {
		case 33: goto st468;
		case 35: goto tr486;
		case 37: goto st32;
		case 47: goto tr487;
		case 58: goto tr500;
		case 61: goto st468;
		case 63: goto tr489;
		case 64: goto tr38;
		case 95: goto st468;
		case 126: goto st468;
	}
	if ( (*p) < 65 ) {
		if ( 36 <= (*p) && (*p) <= 59 )
			goto st468;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto st468;
	} else
		goto st468;
	goto st0;
tr476:
#line 18 "uri.rl"
	{ mark = p; }
	goto st32;
st32:
	if ( ++p == pe )
		goto _test_eof32;
case 32:
#line 1048 "uri.rl.cc"
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st33;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st33;
	} else
		goto st33;
	goto st0;
st33:
	if ( ++p == pe )
		goto _test_eof33;
case 33:
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st468;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st468;
	} else
		goto st468;
	goto st0;
tr482:
#line 18 "uri.rl"
	{ mark = p; }
#line 40 "uri.rl"
	{
        if (mark != NULL) {
            // std::cout << std::string(mark, fpc - mark) << std::endl;
            uri->setHost(std::string(mark, p - mark));
        }
    }
	goto st469;
tr500:
#line 40 "uri.rl"
	{
        if (mark != NULL) {
            // std::cout << std::string(mark, fpc - mark) << std::endl;
            uri->setHost(std::string(mark, p - mark));
        }
    }
	goto st469;
st469:
	if ( ++p == pe )
		goto _test_eof469;
case 469:
#line 1095 "uri.rl.cc"
	switch( (*p) ) {
		case 33: goto st34;
		case 35: goto tr490;
		case 37: goto st35;
		case 47: goto tr491;
		case 61: goto st34;
		case 63: goto tr493;
		case 64: goto tr38;
		case 95: goto st34;
		case 126: goto st34;
	}
	if ( (*p) < 58 ) {
		if ( (*p) > 46 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr501;
		} else if ( (*p) >= 36 )
			goto st34;
	} else if ( (*p) > 59 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto st34;
		} else if ( (*p) >= 65 )
			goto st34;
	} else
		goto st34;
	goto st0;
st34:
	if ( ++p == pe )
		goto _test_eof34;
case 34:
	switch( (*p) ) {
		case 33: goto st34;
		case 37: goto st35;
		case 61: goto st34;
		case 64: goto tr38;
		case 95: goto st34;
		case 126: goto st34;
	}
	if ( (*p) < 48 ) {
		if ( 36 <= (*p) && (*p) <= 46 )
			goto st34;
	} else if ( (*p) > 59 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto st34;
		} else if ( (*p) >= 65 )
			goto st34;
	} else
		goto st34;
	goto st0;
st35:
	if ( ++p == pe )
		goto _test_eof35;
case 35:
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st36;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st36;
	} else
		goto st36;
	goto st0;
st36:
	if ( ++p == pe )
		goto _test_eof36;
case 36:
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st34;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st34;
	} else
		goto st34;
	goto st0;
tr38:
#line 33 "uri.rl"
	{
        if (mark) {
            // std::cout << std::string(mark, fpc - mark) << std::endl;
            uri->setUserinfo(std::string(mark, p - mark));
        }
        mark = NULL;
    }
	goto st470;
tr484:
#line 18 "uri.rl"
	{ mark = p; }
#line 33 "uri.rl"
	{
        if (mark) {
            // std::cout << std::string(mark, fpc - mark) << std::endl;
            uri->setUserinfo(std::string(mark, p - mark));
        }
        mark = NULL;
    }
	goto st470;
st470:
	if ( ++p == pe )
		goto _test_eof470;
case 470:
#line 1198 "uri.rl.cc"
	switch( (*p) ) {
		case 2: goto tr472;
		case 32: goto tr473;
		case 33: goto tr502;
		case 35: goto tr475;
		case 37: goto tr503;
		case 47: goto tr477;
		case 49: goto tr505;
		case 50: goto tr506;
		case 57: goto tr507;
		case 58: goto tr508;
		case 59: goto tr502;
		case 61: goto tr502;
		case 63: goto tr483;
		case 91: goto tr485;
		case 95: goto tr502;
		case 126: goto tr502;
	}
	if ( (*p) < 48 ) {
		if ( 36 <= (*p) && (*p) <= 46 )
			goto tr502;
	} else if ( (*p) > 56 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr502;
		} else if ( (*p) >= 65 )
			goto tr502;
	} else
		goto tr504;
	goto st0;
tr502:
#line 18 "uri.rl"
	{ mark = p; }
	goto st471;
st471:
	if ( ++p == pe )
		goto _test_eof471;
case 471:
#line 1237 "uri.rl.cc"
	switch( (*p) ) {
		case 33: goto st471;
		case 35: goto tr486;
		case 37: goto st37;
		case 47: goto tr487;
		case 58: goto tr488;
		case 61: goto st471;
		case 63: goto tr489;
		case 95: goto st471;
		case 126: goto st471;
	}
	if ( (*p) < 65 ) {
		if ( 36 <= (*p) && (*p) <= 59 )
			goto st471;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto st471;
	} else
		goto st471;
	goto st0;
tr503:
#line 18 "uri.rl"
	{ mark = p; }
	goto st37;
st37:
	if ( ++p == pe )
		goto _test_eof37;
case 37:
#line 1266 "uri.rl.cc"
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st38;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st38;
	} else
		goto st38;
	goto st0;
st38:
	if ( ++p == pe )
		goto _test_eof38;
case 38:
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st471;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st471;
	} else
		goto st471;
	goto st0;
tr504:
#line 18 "uri.rl"
	{ mark = p; }
	goto st472;
st472:
	if ( ++p == pe )
		goto _test_eof472;
case 472:
#line 1297 "uri.rl.cc"
	switch( (*p) ) {
		case 33: goto st471;
		case 35: goto tr486;
		case 37: goto st37;
		case 46: goto st473;
		case 47: goto tr487;
		case 58: goto tr488;
		case 61: goto st471;
		case 63: goto tr489;
		case 95: goto st471;
		case 126: goto st471;
	}
	if ( (*p) < 65 ) {
		if ( 36 <= (*p) && (*p) <= 59 )
			goto st471;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto st471;
	} else
		goto st471;
	goto st0;
st473:
	if ( ++p == pe )
		goto _test_eof473;
case 473:
	switch( (*p) ) {
		case 2: goto st13;
		case 32: goto st14;
		case 33: goto st471;
		case 35: goto tr486;
		case 37: goto st37;
		case 47: goto tr487;
		case 49: goto st484;
		case 50: goto st486;
		case 57: goto st485;
		case 58: goto tr488;
		case 59: goto st471;
		case 61: goto st471;
		case 63: goto tr489;
		case 95: goto st471;
		case 126: goto st471;
	}
	if ( (*p) < 48 ) {
		if ( 36 <= (*p) && (*p) <= 46 )
			goto st471;
	} else if ( (*p) > 56 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto st471;
		} else if ( (*p) >= 65 )
			goto st471;
	} else
		goto st474;
	goto st0;
st474:
	if ( ++p == pe )
		goto _test_eof474;
case 474:
	switch( (*p) ) {
		case 33: goto st471;
		case 35: goto tr486;
		case 37: goto st37;
		case 46: goto st475;
		case 47: goto tr487;
		case 58: goto tr488;
		case 61: goto st471;
		case 63: goto tr489;
		case 95: goto st471;
		case 126: goto st471;
	}
	if ( (*p) < 65 ) {
		if ( 36 <= (*p) && (*p) <= 59 )
			goto st471;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto st471;
	} else
		goto st471;
	goto st0;
st475:
	if ( ++p == pe )
		goto _test_eof475;
case 475:
	switch( (*p) ) {
		case 2: goto st17;
		case 32: goto st18;
		case 33: goto st471;
		case 35: goto tr486;
		case 37: goto st37;
		case 47: goto tr487;
		case 49: goto st480;
		case 50: goto st482;
		case 57: goto st481;
		case 58: goto tr488;
		case 59: goto st471;
		case 61: goto st471;
		case 63: goto tr489;
		case 95: goto st471;
		case 126: goto st471;
	}
	if ( (*p) < 48 ) {
		if ( 36 <= (*p) && (*p) <= 46 )
			goto st471;
	} else if ( (*p) > 56 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto st471;
		} else if ( (*p) >= 65 )
			goto st471;
	} else
		goto st476;
	goto st0;
st476:
	if ( ++p == pe )
		goto _test_eof476;
case 476:
	switch( (*p) ) {
		case 33: goto st471;
		case 35: goto tr486;
		case 37: goto st37;
		case 46: goto st477;
		case 47: goto tr487;
		case 58: goto tr488;
		case 61: goto st471;
		case 63: goto tr489;
		case 95: goto st471;
		case 126: goto st471;
	}
	if ( (*p) < 65 ) {
		if ( 36 <= (*p) && (*p) <= 59 )
			goto st471;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto st471;
	} else
		goto st471;
	goto st0;
st477:
	if ( ++p == pe )
		goto _test_eof477;
case 477:
	switch( (*p) ) {
		case 2: goto st21;
		case 32: goto st22;
		case 33: goto st471;
		case 35: goto tr486;
		case 37: goto st37;
		case 47: goto tr487;
		case 50: goto st478;
		case 58: goto tr488;
		case 61: goto st471;
		case 63: goto tr489;
		case 95: goto st471;
		case 126: goto st471;
	}
	if ( (*p) < 65 ) {
		if ( 36 <= (*p) && (*p) <= 59 )
			goto st471;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto st471;
	} else
		goto st471;
	goto st0;
st478:
	if ( ++p == pe )
		goto _test_eof478;
case 478:
	switch( (*p) ) {
		case 33: goto st471;
		case 35: goto tr486;
		case 37: goto st37;
		case 47: goto tr487;
		case 53: goto st479;
		case 58: goto tr488;
		case 61: goto st471;
		case 63: goto tr489;
		case 95: goto st471;
		case 126: goto st471;
	}
	if ( (*p) < 65 ) {
		if ( 36 <= (*p) && (*p) <= 59 )
			goto st471;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto st471;
	} else
		goto st471;
	goto st0;
st479:
	if ( ++p == pe )
		goto _test_eof479;
case 479:
	switch( (*p) ) {
		case 32: goto st462;
		case 33: goto st471;
		case 35: goto tr486;
		case 37: goto st37;
		case 47: goto tr487;
		case 58: goto tr488;
		case 61: goto st471;
		case 63: goto tr489;
		case 95: goto st471;
		case 126: goto st471;
	}
	if ( (*p) < 65 ) {
		if ( 36 <= (*p) && (*p) <= 59 )
			goto st471;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto st471;
	} else
		goto st471;
	goto st0;
st480:
	if ( ++p == pe )
		goto _test_eof480;
case 480:
	switch( (*p) ) {
		case 33: goto st471;
		case 35: goto tr486;
		case 37: goto st37;
		case 46: goto st477;
		case 47: goto tr487;
		case 58: goto tr488;
		case 59: goto st471;
		case 61: goto st471;
		case 63: goto tr489;
		case 95: goto st471;
		case 126: goto st471;
	}
	if ( (*p) < 48 ) {
		if ( 36 <= (*p) && (*p) <= 45 )
			goto st471;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto st471;
		} else if ( (*p) >= 65 )
			goto st471;
	} else
		goto st481;
	goto st0;
st481:
	if ( ++p == pe )
		goto _test_eof481;
case 481:
	switch( (*p) ) {
		case 33: goto st471;
		case 35: goto tr486;
		case 37: goto st37;
		case 46: goto st477;
		case 47: goto tr487;
		case 58: goto tr488;
		case 59: goto st471;
		case 61: goto st471;
		case 63: goto tr489;
		case 95: goto st471;
		case 126: goto st471;
	}
	if ( (*p) < 48 ) {
		if ( 36 <= (*p) && (*p) <= 45 )
			goto st471;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto st471;
		} else if ( (*p) >= 65 )
			goto st471;
	} else
		goto st476;
	goto st0;
st482:
	if ( ++p == pe )
		goto _test_eof482;
case 482:
	switch( (*p) ) {
		case 33: goto st471;
		case 35: goto tr486;
		case 37: goto st37;
		case 46: goto st477;
		case 47: goto tr487;
		case 53: goto st483;
		case 58: goto tr488;
		case 61: goto st471;
		case 63: goto tr489;
		case 95: goto st471;
		case 126: goto st471;
	}
	if ( (*p) < 65 ) {
		if ( 36 <= (*p) && (*p) <= 59 )
			goto st471;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto st471;
	} else
		goto st471;
	goto st0;
st483:
	if ( ++p == pe )
		goto _test_eof483;
case 483:
	switch( (*p) ) {
		case 32: goto st19;
		case 33: goto st471;
		case 35: goto tr486;
		case 37: goto st37;
		case 47: goto tr487;
		case 48: goto st476;
		case 53: goto st476;
		case 58: goto tr488;
		case 61: goto st471;
		case 63: goto tr489;
		case 95: goto st471;
		case 126: goto st471;
	}
	if ( (*p) < 65 ) {
		if ( 36 <= (*p) && (*p) <= 59 )
			goto st471;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto st471;
	} else
		goto st471;
	goto st0;
st484:
	if ( ++p == pe )
		goto _test_eof484;
case 484:
	switch( (*p) ) {
		case 33: goto st471;
		case 35: goto tr486;
		case 37: goto st37;
		case 46: goto st475;
		case 47: goto tr487;
		case 58: goto tr488;
		case 59: goto st471;
		case 61: goto st471;
		case 63: goto tr489;
		case 95: goto st471;
		case 126: goto st471;
	}
	if ( (*p) < 48 ) {
		if ( 36 <= (*p) && (*p) <= 45 )
			goto st471;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto st471;
		} else if ( (*p) >= 65 )
			goto st471;
	} else
		goto st485;
	goto st0;
st485:
	if ( ++p == pe )
		goto _test_eof485;
case 485:
	switch( (*p) ) {
		case 33: goto st471;
		case 35: goto tr486;
		case 37: goto st37;
		case 46: goto st475;
		case 47: goto tr487;
		case 58: goto tr488;
		case 59: goto st471;
		case 61: goto st471;
		case 63: goto tr489;
		case 95: goto st471;
		case 126: goto st471;
	}
	if ( (*p) < 48 ) {
		if ( 36 <= (*p) && (*p) <= 45 )
			goto st471;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto st471;
		} else if ( (*p) >= 65 )
			goto st471;
	} else
		goto st474;
	goto st0;
st486:
	if ( ++p == pe )
		goto _test_eof486;
case 486:
	switch( (*p) ) {
		case 33: goto st471;
		case 35: goto tr486;
		case 37: goto st37;
		case 46: goto st475;
		case 47: goto tr487;
		case 53: goto st487;
		case 58: goto tr488;
		case 61: goto st471;
		case 63: goto tr489;
		case 95: goto st471;
		case 126: goto st471;
	}
	if ( (*p) < 65 ) {
		if ( 36 <= (*p) && (*p) <= 59 )
			goto st471;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto st471;
	} else
		goto st471;
	goto st0;
st487:
	if ( ++p == pe )
		goto _test_eof487;
case 487:
	switch( (*p) ) {
		case 32: goto st15;
		case 33: goto st471;
		case 35: goto tr486;
		case 37: goto st37;
		case 47: goto tr487;
		case 48: goto st474;
		case 53: goto st474;
		case 58: goto tr488;
		case 61: goto st471;
		case 63: goto tr489;
		case 95: goto st471;
		case 126: goto st471;
	}
	if ( (*p) < 65 ) {
		if ( 36 <= (*p) && (*p) <= 59 )
			goto st471;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto st471;
	} else
		goto st471;
	goto st0;
tr505:
#line 18 "uri.rl"
	{ mark = p; }
	goto st488;
st488:
	if ( ++p == pe )
		goto _test_eof488;
case 488:
#line 1742 "uri.rl.cc"
	switch( (*p) ) {
		case 33: goto st471;
		case 35: goto tr486;
		case 37: goto st37;
		case 46: goto st473;
		case 47: goto tr487;
		case 58: goto tr488;
		case 59: goto st471;
		case 61: goto st471;
		case 63: goto tr489;
		case 95: goto st471;
		case 126: goto st471;
	}
	if ( (*p) < 48 ) {
		if ( 36 <= (*p) && (*p) <= 45 )
			goto st471;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto st471;
		} else if ( (*p) >= 65 )
			goto st471;
	} else
		goto st489;
	goto st0;
tr507:
#line 18 "uri.rl"
	{ mark = p; }
	goto st489;
st489:
	if ( ++p == pe )
		goto _test_eof489;
case 489:
#line 1776 "uri.rl.cc"
	switch( (*p) ) {
		case 33: goto st471;
		case 35: goto tr486;
		case 37: goto st37;
		case 46: goto st473;
		case 47: goto tr487;
		case 58: goto tr488;
		case 59: goto st471;
		case 61: goto st471;
		case 63: goto tr489;
		case 95: goto st471;
		case 126: goto st471;
	}
	if ( (*p) < 48 ) {
		if ( 36 <= (*p) && (*p) <= 45 )
			goto st471;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto st471;
		} else if ( (*p) >= 65 )
			goto st471;
	} else
		goto st472;
	goto st0;
tr506:
#line 18 "uri.rl"
	{ mark = p; }
	goto st490;
st490:
	if ( ++p == pe )
		goto _test_eof490;
case 490:
#line 1810 "uri.rl.cc"
	switch( (*p) ) {
		case 33: goto st471;
		case 35: goto tr486;
		case 37: goto st37;
		case 46: goto st473;
		case 47: goto tr487;
		case 53: goto st491;
		case 58: goto tr488;
		case 61: goto st471;
		case 63: goto tr489;
		case 95: goto st471;
		case 126: goto st471;
	}
	if ( (*p) < 65 ) {
		if ( 36 <= (*p) && (*p) <= 59 )
			goto st471;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto st471;
	} else
		goto st471;
	goto st0;
st491:
	if ( ++p == pe )
		goto _test_eof491;
case 491:
	switch( (*p) ) {
		case 32: goto st11;
		case 33: goto st471;
		case 35: goto tr486;
		case 37: goto st37;
		case 47: goto tr487;
		case 48: goto st472;
		case 53: goto st472;
		case 58: goto tr488;
		case 61: goto st471;
		case 63: goto tr489;
		case 95: goto st471;
		case 126: goto st471;
	}
	if ( (*p) < 65 ) {
		if ( 36 <= (*p) && (*p) <= 59 )
			goto st471;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto st471;
	} else
		goto st471;
	goto st0;
tr485:
#line 18 "uri.rl"
	{ mark = p; }
	goto st39;
st39:
	if ( ++p == pe )
		goto _test_eof39;
case 39:
#line 1868 "uri.rl.cc"
	switch( (*p) ) {
		case 58: goto st147;
		case 118: goto st226;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st40;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st40;
	} else
		goto st40;
	goto st0;
st40:
	if ( ++p == pe )
		goto _test_eof40;
case 40:
	if ( (*p) == 58 )
		goto st44;
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st41;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st41;
	} else
		goto st41;
	goto st0;
st41:
	if ( ++p == pe )
		goto _test_eof41;
case 41:
	if ( (*p) == 58 )
		goto st44;
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st42;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st42;
	} else
		goto st42;
	goto st0;
st42:
	if ( ++p == pe )
		goto _test_eof42;
case 42:
	if ( (*p) == 58 )
		goto st44;
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st43;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st43;
	} else
		goto st43;
	goto st0;
st43:
	if ( ++p == pe )
		goto _test_eof43;
case 43:
	if ( (*p) == 58 )
		goto st44;
	goto st0;
st44:
	if ( ++p == pe )
		goto _test_eof44;
case 44:
	if ( (*p) == 58 )
		goto st142;
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st45;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st45;
	} else
		goto st45;
	goto st0;
st45:
	if ( ++p == pe )
		goto _test_eof45;
case 45:
	if ( (*p) == 58 )
		goto st49;
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st46;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st46;
	} else
		goto st46;
	goto st0;
st46:
	if ( ++p == pe )
		goto _test_eof46;
case 46:
	if ( (*p) == 58 )
		goto st49;
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st47;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st47;
	} else
		goto st47;
	goto st0;
st47:
	if ( ++p == pe )
		goto _test_eof47;
case 47:
	if ( (*p) == 58 )
		goto st49;
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st48;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st48;
	} else
		goto st48;
	goto st0;
st48:
	if ( ++p == pe )
		goto _test_eof48;
case 48:
	if ( (*p) == 58 )
		goto st49;
	goto st0;
st49:
	if ( ++p == pe )
		goto _test_eof49;
case 49:
	if ( (*p) == 58 )
		goto st137;
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st50;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st50;
	} else
		goto st50;
	goto st0;
st50:
	if ( ++p == pe )
		goto _test_eof50;
case 50:
	if ( (*p) == 58 )
		goto st54;
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st51;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st51;
	} else
		goto st51;
	goto st0;
st51:
	if ( ++p == pe )
		goto _test_eof51;
case 51:
	if ( (*p) == 58 )
		goto st54;
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st52;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st52;
	} else
		goto st52;
	goto st0;
st52:
	if ( ++p == pe )
		goto _test_eof52;
case 52:
	if ( (*p) == 58 )
		goto st54;
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st53;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st53;
	} else
		goto st53;
	goto st0;
st53:
	if ( ++p == pe )
		goto _test_eof53;
case 53:
	if ( (*p) == 58 )
		goto st54;
	goto st0;
st54:
	if ( ++p == pe )
		goto _test_eof54;
case 54:
	if ( (*p) == 58 )
		goto st132;
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st55;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st55;
	} else
		goto st55;
	goto st0;
st55:
	if ( ++p == pe )
		goto _test_eof55;
case 55:
	if ( (*p) == 58 )
		goto st59;
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st56;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st56;
	} else
		goto st56;
	goto st0;
st56:
	if ( ++p == pe )
		goto _test_eof56;
case 56:
	if ( (*p) == 58 )
		goto st59;
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st57;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st57;
	} else
		goto st57;
	goto st0;
st57:
	if ( ++p == pe )
		goto _test_eof57;
case 57:
	if ( (*p) == 58 )
		goto st59;
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st58;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st58;
	} else
		goto st58;
	goto st0;
st58:
	if ( ++p == pe )
		goto _test_eof58;
case 58:
	if ( (*p) == 58 )
		goto st59;
	goto st0;
st59:
	if ( ++p == pe )
		goto _test_eof59;
case 59:
	if ( (*p) == 58 )
		goto st127;
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st60;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st60;
	} else
		goto st60;
	goto st0;
st60:
	if ( ++p == pe )
		goto _test_eof60;
case 60:
	if ( (*p) == 58 )
		goto st64;
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st61;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st61;
	} else
		goto st61;
	goto st0;
st61:
	if ( ++p == pe )
		goto _test_eof61;
case 61:
	if ( (*p) == 58 )
		goto st64;
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st62;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st62;
	} else
		goto st62;
	goto st0;
st62:
	if ( ++p == pe )
		goto _test_eof62;
case 62:
	if ( (*p) == 58 )
		goto st64;
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st63;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st63;
	} else
		goto st63;
	goto st0;
st63:
	if ( ++p == pe )
		goto _test_eof63;
case 63:
	if ( (*p) == 58 )
		goto st64;
	goto st0;
st64:
	if ( ++p == pe )
		goto _test_eof64;
case 64:
	if ( (*p) == 58 )
		goto st114;
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st65;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st65;
	} else
		goto st65;
	goto st0;
st65:
	if ( ++p == pe )
		goto _test_eof65;
case 65:
	if ( (*p) == 58 )
		goto st69;
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st66;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st66;
	} else
		goto st66;
	goto st0;
st66:
	if ( ++p == pe )
		goto _test_eof66;
case 66:
	if ( (*p) == 58 )
		goto st69;
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st67;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st67;
	} else
		goto st67;
	goto st0;
st67:
	if ( ++p == pe )
		goto _test_eof67;
case 67:
	if ( (*p) == 58 )
		goto st69;
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st68;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st68;
	} else
		goto st68;
	goto st0;
st68:
	if ( ++p == pe )
		goto _test_eof68;
case 68:
	if ( (*p) == 58 )
		goto st69;
	goto st0;
st69:
	if ( ++p == pe )
		goto _test_eof69;
case 69:
	switch( (*p) ) {
		case 2: goto st70;
		case 32: goto st71;
		case 49: goto st105;
		case 50: goto st108;
		case 57: goto st110;
		case 58: goto st112;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 56 )
			goto st97;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st113;
	} else
		goto st113;
	goto st0;
st70:
	if ( ++p == pe )
		goto _test_eof70;
case 70:
	switch( (*p) ) {
		case 32: goto st71;
		case 48: goto st71;
		case 52: goto st71;
	}
	goto st0;
st71:
	if ( ++p == pe )
		goto _test_eof71;
case 71:
	if ( 48 <= (*p) && (*p) <= 57 )
		goto st72;
	goto st0;
st72:
	if ( ++p == pe )
		goto _test_eof72;
case 72:
	if ( (*p) == 46 )
		goto st73;
	goto st0;
st73:
	if ( ++p == pe )
		goto _test_eof73;
case 73:
	switch( (*p) ) {
		case 2: goto st74;
		case 32: goto st75;
		case 49: goto st93;
		case 50: goto st95;
		case 57: goto st94;
	}
	if ( 48 <= (*p) && (*p) <= 56 )
		goto st76;
	goto st0;
st74:
	if ( ++p == pe )
		goto _test_eof74;
case 74:
	switch( (*p) ) {
		case 32: goto st75;
		case 48: goto st75;
		case 52: goto st75;
	}
	goto st0;
st75:
	if ( ++p == pe )
		goto _test_eof75;
case 75:
	if ( 48 <= (*p) && (*p) <= 57 )
		goto st76;
	goto st0;
st76:
	if ( ++p == pe )
		goto _test_eof76;
case 76:
	if ( (*p) == 46 )
		goto st77;
	goto st0;
st77:
	if ( ++p == pe )
		goto _test_eof77;
case 77:
	switch( (*p) ) {
		case 2: goto st78;
		case 32: goto st79;
		case 49: goto st89;
		case 50: goto st91;
		case 57: goto st90;
	}
	if ( 48 <= (*p) && (*p) <= 56 )
		goto st80;
	goto st0;
st78:
	if ( ++p == pe )
		goto _test_eof78;
case 78:
	switch( (*p) ) {
		case 32: goto st79;
		case 48: goto st79;
		case 52: goto st79;
	}
	goto st0;
st79:
	if ( ++p == pe )
		goto _test_eof79;
case 79:
	if ( 48 <= (*p) && (*p) <= 57 )
		goto st80;
	goto st0;
st80:
	if ( ++p == pe )
		goto _test_eof80;
case 80:
	if ( (*p) == 46 )
		goto st81;
	goto st0;
st81:
	if ( ++p == pe )
		goto _test_eof81;
case 81:
	switch( (*p) ) {
		case 2: goto st82;
		case 32: goto st83;
		case 49: goto st85;
		case 50: goto st87;
		case 57: goto st86;
	}
	if ( 48 <= (*p) && (*p) <= 56 )
		goto st84;
	goto st0;
st82:
	if ( ++p == pe )
		goto _test_eof82;
case 82:
	switch( (*p) ) {
		case 32: goto st83;
		case 48: goto st83;
		case 52: goto st83;
	}
	goto st0;
st83:
	if ( ++p == pe )
		goto _test_eof83;
case 83:
	if ( 48 <= (*p) && (*p) <= 57 )
		goto st84;
	goto st0;
st84:
	if ( ++p == pe )
		goto _test_eof84;
case 84:
	if ( (*p) == 93 )
		goto st462;
	goto st0;
st85:
	if ( ++p == pe )
		goto _test_eof85;
case 85:
	if ( (*p) == 93 )
		goto st462;
	if ( 48 <= (*p) && (*p) <= 57 )
		goto st86;
	goto st0;
st86:
	if ( ++p == pe )
		goto _test_eof86;
case 86:
	if ( (*p) == 93 )
		goto st462;
	if ( 48 <= (*p) && (*p) <= 57 )
		goto st84;
	goto st0;
st87:
	if ( ++p == pe )
		goto _test_eof87;
case 87:
	switch( (*p) ) {
		case 53: goto st88;
		case 93: goto st462;
	}
	goto st0;
st88:
	if ( ++p == pe )
		goto _test_eof88;
case 88:
	switch( (*p) ) {
		case 32: goto st84;
		case 48: goto st84;
		case 53: goto st84;
	}
	goto st0;
st89:
	if ( ++p == pe )
		goto _test_eof89;
case 89:
	if ( (*p) == 46 )
		goto st81;
	if ( 48 <= (*p) && (*p) <= 57 )
		goto st90;
	goto st0;
st90:
	if ( ++p == pe )
		goto _test_eof90;
case 90:
	if ( (*p) == 46 )
		goto st81;
	if ( 48 <= (*p) && (*p) <= 57 )
		goto st80;
	goto st0;
st91:
	if ( ++p == pe )
		goto _test_eof91;
case 91:
	switch( (*p) ) {
		case 46: goto st81;
		case 53: goto st92;
	}
	goto st0;
st92:
	if ( ++p == pe )
		goto _test_eof92;
case 92:
	switch( (*p) ) {
		case 32: goto st80;
		case 48: goto st80;
		case 53: goto st80;
	}
	goto st0;
st93:
	if ( ++p == pe )
		goto _test_eof93;
case 93:
	if ( (*p) == 46 )
		goto st77;
	if ( 48 <= (*p) && (*p) <= 57 )
		goto st94;
	goto st0;
st94:
	if ( ++p == pe )
		goto _test_eof94;
case 94:
	if ( (*p) == 46 )
		goto st77;
	if ( 48 <= (*p) && (*p) <= 57 )
		goto st76;
	goto st0;
st95:
	if ( ++p == pe )
		goto _test_eof95;
case 95:
	switch( (*p) ) {
		case 46: goto st77;
		case 53: goto st96;
	}
	goto st0;
st96:
	if ( ++p == pe )
		goto _test_eof96;
case 96:
	switch( (*p) ) {
		case 32: goto st76;
		case 48: goto st76;
		case 53: goto st76;
	}
	goto st0;
st97:
	if ( ++p == pe )
		goto _test_eof97;
case 97:
	switch( (*p) ) {
		case 46: goto st73;
		case 58: goto st101;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st98;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st98;
	} else
		goto st98;
	goto st0;
st98:
	if ( ++p == pe )
		goto _test_eof98;
case 98:
	if ( (*p) == 58 )
		goto st101;
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st99;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st99;
	} else
		goto st99;
	goto st0;
st99:
	if ( ++p == pe )
		goto _test_eof99;
case 99:
	if ( (*p) == 58 )
		goto st101;
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st100;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st100;
	} else
		goto st100;
	goto st0;
st100:
	if ( ++p == pe )
		goto _test_eof100;
case 100:
	if ( (*p) == 58 )
		goto st101;
	goto st0;
st101:
	if ( ++p == pe )
		goto _test_eof101;
case 101:
	if ( (*p) == 58 )
		goto st84;
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st102;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st102;
	} else
		goto st102;
	goto st0;
st102:
	if ( ++p == pe )
		goto _test_eof102;
case 102:
	if ( (*p) == 93 )
		goto st462;
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st103;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st103;
	} else
		goto st103;
	goto st0;
st103:
	if ( ++p == pe )
		goto _test_eof103;
case 103:
	if ( (*p) == 93 )
		goto st462;
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st104;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st104;
	} else
		goto st104;
	goto st0;
st104:
	if ( ++p == pe )
		goto _test_eof104;
case 104:
	if ( (*p) == 93 )
		goto st462;
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st84;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st84;
	} else
		goto st84;
	goto st0;
st105:
	if ( ++p == pe )
		goto _test_eof105;
case 105:
	switch( (*p) ) {
		case 46: goto st73;
		case 58: goto st101;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st106;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st98;
	} else
		goto st98;
	goto st0;
st106:
	if ( ++p == pe )
		goto _test_eof106;
case 106:
	switch( (*p) ) {
		case 46: goto st73;
		case 58: goto st101;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st107;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st99;
	} else
		goto st99;
	goto st0;
st107:
	if ( ++p == pe )
		goto _test_eof107;
case 107:
	switch( (*p) ) {
		case 46: goto st73;
		case 58: goto st101;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st100;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st100;
	} else
		goto st100;
	goto st0;
st108:
	if ( ++p == pe )
		goto _test_eof108;
case 108:
	switch( (*p) ) {
		case 46: goto st73;
		case 53: goto st109;
		case 58: goto st101;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st98;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st98;
	} else
		goto st98;
	goto st0;
st109:
	if ( ++p == pe )
		goto _test_eof109;
case 109:
	switch( (*p) ) {
		case 32: goto st72;
		case 48: goto st107;
		case 53: goto st107;
		case 58: goto st101;
	}
	if ( (*p) < 65 ) {
		if ( 49 <= (*p) && (*p) <= 57 )
			goto st99;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st99;
	} else
		goto st99;
	goto st0;
st110:
	if ( ++p == pe )
		goto _test_eof110;
case 110:
	switch( (*p) ) {
		case 46: goto st73;
		case 58: goto st101;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st111;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st98;
	} else
		goto st98;
	goto st0;
st111:
	if ( ++p == pe )
		goto _test_eof111;
case 111:
	switch( (*p) ) {
		case 46: goto st73;
		case 58: goto st101;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st99;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st99;
	} else
		goto st99;
	goto st0;
st112:
	if ( ++p == pe )
		goto _test_eof112;
case 112:
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st102;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st102;
	} else
		goto st102;
	goto st0;
st113:
	if ( ++p == pe )
		goto _test_eof113;
case 113:
	if ( (*p) == 58 )
		goto st101;
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st98;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st98;
	} else
		goto st98;
	goto st0;
st114:
	if ( ++p == pe )
		goto _test_eof114;
case 114:
	switch( (*p) ) {
		case 2: goto st70;
		case 32: goto st71;
		case 49: goto st119;
		case 50: goto st122;
		case 57: goto st124;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 56 )
			goto st115;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st126;
	} else
		goto st126;
	goto st0;
st115:
	if ( ++p == pe )
		goto _test_eof115;
case 115:
	switch( (*p) ) {
		case 46: goto st73;
		case 58: goto st112;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st116;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st116;
	} else
		goto st116;
	goto st0;
st116:
	if ( ++p == pe )
		goto _test_eof116;
case 116:
	if ( (*p) == 58 )
		goto st112;
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st117;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st117;
	} else
		goto st117;
	goto st0;
st117:
	if ( ++p == pe )
		goto _test_eof117;
case 117:
	if ( (*p) == 58 )
		goto st112;
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st118;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st118;
	} else
		goto st118;
	goto st0;
st118:
	if ( ++p == pe )
		goto _test_eof118;
case 118:
	if ( (*p) == 58 )
		goto st112;
	goto st0;
st119:
	if ( ++p == pe )
		goto _test_eof119;
case 119:
	switch( (*p) ) {
		case 46: goto st73;
		case 58: goto st112;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st120;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st116;
	} else
		goto st116;
	goto st0;
st120:
	if ( ++p == pe )
		goto _test_eof120;
case 120:
	switch( (*p) ) {
		case 46: goto st73;
		case 58: goto st112;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st121;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st117;
	} else
		goto st117;
	goto st0;
st121:
	if ( ++p == pe )
		goto _test_eof121;
case 121:
	switch( (*p) ) {
		case 46: goto st73;
		case 58: goto st112;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st118;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st118;
	} else
		goto st118;
	goto st0;
st122:
	if ( ++p == pe )
		goto _test_eof122;
case 122:
	switch( (*p) ) {
		case 46: goto st73;
		case 53: goto st123;
		case 58: goto st112;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st116;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st116;
	} else
		goto st116;
	goto st0;
st123:
	if ( ++p == pe )
		goto _test_eof123;
case 123:
	switch( (*p) ) {
		case 32: goto st72;
		case 48: goto st121;
		case 53: goto st121;
		case 58: goto st112;
	}
	if ( (*p) < 65 ) {
		if ( 49 <= (*p) && (*p) <= 57 )
			goto st117;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st117;
	} else
		goto st117;
	goto st0;
st124:
	if ( ++p == pe )
		goto _test_eof124;
case 124:
	switch( (*p) ) {
		case 46: goto st73;
		case 58: goto st112;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st125;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st116;
	} else
		goto st116;
	goto st0;
st125:
	if ( ++p == pe )
		goto _test_eof125;
case 125:
	switch( (*p) ) {
		case 46: goto st73;
		case 58: goto st112;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st117;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st117;
	} else
		goto st117;
	goto st0;
st126:
	if ( ++p == pe )
		goto _test_eof126;
case 126:
	if ( (*p) == 58 )
		goto st112;
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st116;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st116;
	} else
		goto st116;
	goto st0;
st127:
	if ( ++p == pe )
		goto _test_eof127;
case 127:
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st128;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st128;
	} else
		goto st128;
	goto st0;
st128:
	if ( ++p == pe )
		goto _test_eof128;
case 128:
	if ( (*p) == 58 )
		goto st114;
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st129;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st129;
	} else
		goto st129;
	goto st0;
st129:
	if ( ++p == pe )
		goto _test_eof129;
case 129:
	if ( (*p) == 58 )
		goto st114;
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st130;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st130;
	} else
		goto st130;
	goto st0;
st130:
	if ( ++p == pe )
		goto _test_eof130;
case 130:
	if ( (*p) == 58 )
		goto st114;
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st131;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st131;
	} else
		goto st131;
	goto st0;
st131:
	if ( ++p == pe )
		goto _test_eof131;
case 131:
	if ( (*p) == 58 )
		goto st114;
	goto st0;
st132:
	if ( ++p == pe )
		goto _test_eof132;
case 132:
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st133;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st133;
	} else
		goto st133;
	goto st0;
st133:
	if ( ++p == pe )
		goto _test_eof133;
case 133:
	if ( (*p) == 58 )
		goto st127;
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st134;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st134;
	} else
		goto st134;
	goto st0;
st134:
	if ( ++p == pe )
		goto _test_eof134;
case 134:
	if ( (*p) == 58 )
		goto st127;
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st135;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st135;
	} else
		goto st135;
	goto st0;
st135:
	if ( ++p == pe )
		goto _test_eof135;
case 135:
	if ( (*p) == 58 )
		goto st127;
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st136;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st136;
	} else
		goto st136;
	goto st0;
st136:
	if ( ++p == pe )
		goto _test_eof136;
case 136:
	if ( (*p) == 58 )
		goto st127;
	goto st0;
st137:
	if ( ++p == pe )
		goto _test_eof137;
case 137:
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st138;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st138;
	} else
		goto st138;
	goto st0;
st138:
	if ( ++p == pe )
		goto _test_eof138;
case 138:
	if ( (*p) == 58 )
		goto st132;
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st139;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st139;
	} else
		goto st139;
	goto st0;
st139:
	if ( ++p == pe )
		goto _test_eof139;
case 139:
	if ( (*p) == 58 )
		goto st132;
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st140;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st140;
	} else
		goto st140;
	goto st0;
st140:
	if ( ++p == pe )
		goto _test_eof140;
case 140:
	if ( (*p) == 58 )
		goto st132;
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st141;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st141;
	} else
		goto st141;
	goto st0;
st141:
	if ( ++p == pe )
		goto _test_eof141;
case 141:
	if ( (*p) == 58 )
		goto st132;
	goto st0;
st142:
	if ( ++p == pe )
		goto _test_eof142;
case 142:
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st143;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st143;
	} else
		goto st143;
	goto st0;
st143:
	if ( ++p == pe )
		goto _test_eof143;
case 143:
	if ( (*p) == 58 )
		goto st137;
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st144;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st144;
	} else
		goto st144;
	goto st0;
st144:
	if ( ++p == pe )
		goto _test_eof144;
case 144:
	if ( (*p) == 58 )
		goto st137;
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st145;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st145;
	} else
		goto st145;
	goto st0;
st145:
	if ( ++p == pe )
		goto _test_eof145;
case 145:
	if ( (*p) == 58 )
		goto st137;
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st146;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st146;
	} else
		goto st146;
	goto st0;
st146:
	if ( ++p == pe )
		goto _test_eof146;
case 146:
	if ( (*p) == 58 )
		goto st137;
	goto st0;
st147:
	if ( ++p == pe )
		goto _test_eof147;
case 147:
	if ( (*p) == 58 )
		goto st148;
	goto st0;
st148:
	if ( ++p == pe )
		goto _test_eof148;
case 148:
	switch( (*p) ) {
		case 2: goto st70;
		case 32: goto st71;
		case 49: goto st218;
		case 50: goto st221;
		case 57: goto st223;
		case 93: goto st462;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 56 )
			goto st149;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st225;
	} else
		goto st225;
	goto st0;
st149:
	if ( ++p == pe )
		goto _test_eof149;
case 149:
	switch( (*p) ) {
		case 46: goto st73;
		case 58: goto st153;
		case 93: goto st462;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st150;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st150;
	} else
		goto st150;
	goto st0;
st150:
	if ( ++p == pe )
		goto _test_eof150;
case 150:
	switch( (*p) ) {
		case 58: goto st153;
		case 93: goto st462;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st151;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st151;
	} else
		goto st151;
	goto st0;
st151:
	if ( ++p == pe )
		goto _test_eof151;
case 151:
	switch( (*p) ) {
		case 58: goto st153;
		case 93: goto st462;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st152;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st152;
	} else
		goto st152;
	goto st0;
st152:
	if ( ++p == pe )
		goto _test_eof152;
case 152:
	switch( (*p) ) {
		case 58: goto st153;
		case 93: goto st462;
	}
	goto st0;
st153:
	if ( ++p == pe )
		goto _test_eof153;
case 153:
	switch( (*p) ) {
		case 2: goto st70;
		case 32: goto st71;
		case 49: goto st210;
		case 50: goto st213;
		case 57: goto st215;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 56 )
			goto st154;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st217;
	} else
		goto st217;
	goto st0;
st154:
	if ( ++p == pe )
		goto _test_eof154;
case 154:
	switch( (*p) ) {
		case 46: goto st73;
		case 58: goto st158;
		case 93: goto st462;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st155;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st155;
	} else
		goto st155;
	goto st0;
st155:
	if ( ++p == pe )
		goto _test_eof155;
case 155:
	switch( (*p) ) {
		case 58: goto st158;
		case 93: goto st462;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st156;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st156;
	} else
		goto st156;
	goto st0;
st156:
	if ( ++p == pe )
		goto _test_eof156;
case 156:
	switch( (*p) ) {
		case 58: goto st158;
		case 93: goto st462;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st157;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st157;
	} else
		goto st157;
	goto st0;
st157:
	if ( ++p == pe )
		goto _test_eof157;
case 157:
	switch( (*p) ) {
		case 58: goto st158;
		case 93: goto st462;
	}
	goto st0;
st158:
	if ( ++p == pe )
		goto _test_eof158;
case 158:
	switch( (*p) ) {
		case 2: goto st70;
		case 32: goto st71;
		case 49: goto st202;
		case 50: goto st205;
		case 57: goto st207;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 56 )
			goto st159;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st209;
	} else
		goto st209;
	goto st0;
st159:
	if ( ++p == pe )
		goto _test_eof159;
case 159:
	switch( (*p) ) {
		case 46: goto st73;
		case 58: goto st163;
		case 93: goto st462;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st160;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st160;
	} else
		goto st160;
	goto st0;
st160:
	if ( ++p == pe )
		goto _test_eof160;
case 160:
	switch( (*p) ) {
		case 58: goto st163;
		case 93: goto st462;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st161;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st161;
	} else
		goto st161;
	goto st0;
st161:
	if ( ++p == pe )
		goto _test_eof161;
case 161:
	switch( (*p) ) {
		case 58: goto st163;
		case 93: goto st462;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st162;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st162;
	} else
		goto st162;
	goto st0;
st162:
	if ( ++p == pe )
		goto _test_eof162;
case 162:
	switch( (*p) ) {
		case 58: goto st163;
		case 93: goto st462;
	}
	goto st0;
st163:
	if ( ++p == pe )
		goto _test_eof163;
case 163:
	switch( (*p) ) {
		case 2: goto st70;
		case 32: goto st71;
		case 49: goto st194;
		case 50: goto st197;
		case 57: goto st199;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 56 )
			goto st164;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st201;
	} else
		goto st201;
	goto st0;
st164:
	if ( ++p == pe )
		goto _test_eof164;
case 164:
	switch( (*p) ) {
		case 46: goto st73;
		case 58: goto st168;
		case 93: goto st462;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st165;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st165;
	} else
		goto st165;
	goto st0;
st165:
	if ( ++p == pe )
		goto _test_eof165;
case 165:
	switch( (*p) ) {
		case 58: goto st168;
		case 93: goto st462;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st166;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st166;
	} else
		goto st166;
	goto st0;
st166:
	if ( ++p == pe )
		goto _test_eof166;
case 166:
	switch( (*p) ) {
		case 58: goto st168;
		case 93: goto st462;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st167;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st167;
	} else
		goto st167;
	goto st0;
st167:
	if ( ++p == pe )
		goto _test_eof167;
case 167:
	switch( (*p) ) {
		case 58: goto st168;
		case 93: goto st462;
	}
	goto st0;
st168:
	if ( ++p == pe )
		goto _test_eof168;
case 168:
	switch( (*p) ) {
		case 2: goto st70;
		case 32: goto st71;
		case 49: goto st186;
		case 50: goto st189;
		case 57: goto st191;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 56 )
			goto st169;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st193;
	} else
		goto st193;
	goto st0;
st169:
	if ( ++p == pe )
		goto _test_eof169;
case 169:
	switch( (*p) ) {
		case 46: goto st73;
		case 58: goto st173;
		case 93: goto st462;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st170;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st170;
	} else
		goto st170;
	goto st0;
st170:
	if ( ++p == pe )
		goto _test_eof170;
case 170:
	switch( (*p) ) {
		case 58: goto st173;
		case 93: goto st462;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st171;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st171;
	} else
		goto st171;
	goto st0;
st171:
	if ( ++p == pe )
		goto _test_eof171;
case 171:
	switch( (*p) ) {
		case 58: goto st173;
		case 93: goto st462;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st172;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st172;
	} else
		goto st172;
	goto st0;
st172:
	if ( ++p == pe )
		goto _test_eof172;
case 172:
	switch( (*p) ) {
		case 58: goto st173;
		case 93: goto st462;
	}
	goto st0;
st173:
	if ( ++p == pe )
		goto _test_eof173;
case 173:
	switch( (*p) ) {
		case 2: goto st70;
		case 32: goto st71;
		case 49: goto st178;
		case 50: goto st181;
		case 57: goto st183;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 56 )
			goto st174;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st185;
	} else
		goto st185;
	goto st0;
st174:
	if ( ++p == pe )
		goto _test_eof174;
case 174:
	switch( (*p) ) {
		case 46: goto st73;
		case 58: goto st112;
		case 93: goto st462;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st175;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st175;
	} else
		goto st175;
	goto st0;
st175:
	if ( ++p == pe )
		goto _test_eof175;
case 175:
	switch( (*p) ) {
		case 58: goto st112;
		case 93: goto st462;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st176;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st176;
	} else
		goto st176;
	goto st0;
st176:
	if ( ++p == pe )
		goto _test_eof176;
case 176:
	switch( (*p) ) {
		case 58: goto st112;
		case 93: goto st462;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st177;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st177;
	} else
		goto st177;
	goto st0;
st177:
	if ( ++p == pe )
		goto _test_eof177;
case 177:
	switch( (*p) ) {
		case 58: goto st112;
		case 93: goto st462;
	}
	goto st0;
st178:
	if ( ++p == pe )
		goto _test_eof178;
case 178:
	switch( (*p) ) {
		case 46: goto st73;
		case 58: goto st112;
		case 93: goto st462;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st179;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st175;
	} else
		goto st175;
	goto st0;
st179:
	if ( ++p == pe )
		goto _test_eof179;
case 179:
	switch( (*p) ) {
		case 46: goto st73;
		case 58: goto st112;
		case 93: goto st462;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st180;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st176;
	} else
		goto st176;
	goto st0;
st180:
	if ( ++p == pe )
		goto _test_eof180;
case 180:
	switch( (*p) ) {
		case 46: goto st73;
		case 58: goto st112;
		case 93: goto st462;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st177;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st177;
	} else
		goto st177;
	goto st0;
st181:
	if ( ++p == pe )
		goto _test_eof181;
case 181:
	switch( (*p) ) {
		case 46: goto st73;
		case 53: goto st182;
		case 58: goto st112;
		case 93: goto st462;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st175;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st175;
	} else
		goto st175;
	goto st0;
st182:
	if ( ++p == pe )
		goto _test_eof182;
case 182:
	switch( (*p) ) {
		case 32: goto st72;
		case 48: goto st180;
		case 53: goto st180;
		case 58: goto st112;
		case 93: goto st462;
	}
	if ( (*p) < 65 ) {
		if ( 49 <= (*p) && (*p) <= 57 )
			goto st176;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st176;
	} else
		goto st176;
	goto st0;
st183:
	if ( ++p == pe )
		goto _test_eof183;
case 183:
	switch( (*p) ) {
		case 46: goto st73;
		case 58: goto st112;
		case 93: goto st462;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st184;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st175;
	} else
		goto st175;
	goto st0;
st184:
	if ( ++p == pe )
		goto _test_eof184;
case 184:
	switch( (*p) ) {
		case 46: goto st73;
		case 58: goto st112;
		case 93: goto st462;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st176;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st176;
	} else
		goto st176;
	goto st0;
st185:
	if ( ++p == pe )
		goto _test_eof185;
case 185:
	switch( (*p) ) {
		case 58: goto st112;
		case 93: goto st462;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st175;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st175;
	} else
		goto st175;
	goto st0;
st186:
	if ( ++p == pe )
		goto _test_eof186;
case 186:
	switch( (*p) ) {
		case 46: goto st73;
		case 58: goto st173;
		case 93: goto st462;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st187;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st170;
	} else
		goto st170;
	goto st0;
st187:
	if ( ++p == pe )
		goto _test_eof187;
case 187:
	switch( (*p) ) {
		case 46: goto st73;
		case 58: goto st173;
		case 93: goto st462;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st188;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st171;
	} else
		goto st171;
	goto st0;
st188:
	if ( ++p == pe )
		goto _test_eof188;
case 188:
	switch( (*p) ) {
		case 46: goto st73;
		case 58: goto st173;
		case 93: goto st462;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st172;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st172;
	} else
		goto st172;
	goto st0;
st189:
	if ( ++p == pe )
		goto _test_eof189;
case 189:
	switch( (*p) ) {
		case 46: goto st73;
		case 53: goto st190;
		case 58: goto st173;
		case 93: goto st462;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st170;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st170;
	} else
		goto st170;
	goto st0;
st190:
	if ( ++p == pe )
		goto _test_eof190;
case 190:
	switch( (*p) ) {
		case 32: goto st72;
		case 48: goto st188;
		case 53: goto st188;
		case 58: goto st173;
		case 93: goto st462;
	}
	if ( (*p) < 65 ) {
		if ( 49 <= (*p) && (*p) <= 57 )
			goto st171;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st171;
	} else
		goto st171;
	goto st0;
st191:
	if ( ++p == pe )
		goto _test_eof191;
case 191:
	switch( (*p) ) {
		case 46: goto st73;
		case 58: goto st173;
		case 93: goto st462;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st192;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st170;
	} else
		goto st170;
	goto st0;
st192:
	if ( ++p == pe )
		goto _test_eof192;
case 192:
	switch( (*p) ) {
		case 46: goto st73;
		case 58: goto st173;
		case 93: goto st462;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st171;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st171;
	} else
		goto st171;
	goto st0;
st193:
	if ( ++p == pe )
		goto _test_eof193;
case 193:
	switch( (*p) ) {
		case 58: goto st173;
		case 93: goto st462;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st170;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st170;
	} else
		goto st170;
	goto st0;
st194:
	if ( ++p == pe )
		goto _test_eof194;
case 194:
	switch( (*p) ) {
		case 46: goto st73;
		case 58: goto st168;
		case 93: goto st462;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st195;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st165;
	} else
		goto st165;
	goto st0;
st195:
	if ( ++p == pe )
		goto _test_eof195;
case 195:
	switch( (*p) ) {
		case 46: goto st73;
		case 58: goto st168;
		case 93: goto st462;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st196;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st166;
	} else
		goto st166;
	goto st0;
st196:
	if ( ++p == pe )
		goto _test_eof196;
case 196:
	switch( (*p) ) {
		case 46: goto st73;
		case 58: goto st168;
		case 93: goto st462;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st167;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st167;
	} else
		goto st167;
	goto st0;
st197:
	if ( ++p == pe )
		goto _test_eof197;
case 197:
	switch( (*p) ) {
		case 46: goto st73;
		case 53: goto st198;
		case 58: goto st168;
		case 93: goto st462;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st165;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st165;
	} else
		goto st165;
	goto st0;
st198:
	if ( ++p == pe )
		goto _test_eof198;
case 198:
	switch( (*p) ) {
		case 32: goto st72;
		case 48: goto st196;
		case 53: goto st196;
		case 58: goto st168;
		case 93: goto st462;
	}
	if ( (*p) < 65 ) {
		if ( 49 <= (*p) && (*p) <= 57 )
			goto st166;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st166;
	} else
		goto st166;
	goto st0;
st199:
	if ( ++p == pe )
		goto _test_eof199;
case 199:
	switch( (*p) ) {
		case 46: goto st73;
		case 58: goto st168;
		case 93: goto st462;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st200;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st165;
	} else
		goto st165;
	goto st0;
st200:
	if ( ++p == pe )
		goto _test_eof200;
case 200:
	switch( (*p) ) {
		case 46: goto st73;
		case 58: goto st168;
		case 93: goto st462;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st166;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st166;
	} else
		goto st166;
	goto st0;
st201:
	if ( ++p == pe )
		goto _test_eof201;
case 201:
	switch( (*p) ) {
		case 58: goto st168;
		case 93: goto st462;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st165;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st165;
	} else
		goto st165;
	goto st0;
st202:
	if ( ++p == pe )
		goto _test_eof202;
case 202:
	switch( (*p) ) {
		case 46: goto st73;
		case 58: goto st163;
		case 93: goto st462;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st203;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st160;
	} else
		goto st160;
	goto st0;
st203:
	if ( ++p == pe )
		goto _test_eof203;
case 203:
	switch( (*p) ) {
		case 46: goto st73;
		case 58: goto st163;
		case 93: goto st462;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st204;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st161;
	} else
		goto st161;
	goto st0;
st204:
	if ( ++p == pe )
		goto _test_eof204;
case 204:
	switch( (*p) ) {
		case 46: goto st73;
		case 58: goto st163;
		case 93: goto st462;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st162;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st162;
	} else
		goto st162;
	goto st0;
st205:
	if ( ++p == pe )
		goto _test_eof205;
case 205:
	switch( (*p) ) {
		case 46: goto st73;
		case 53: goto st206;
		case 58: goto st163;
		case 93: goto st462;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st160;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st160;
	} else
		goto st160;
	goto st0;
st206:
	if ( ++p == pe )
		goto _test_eof206;
case 206:
	switch( (*p) ) {
		case 32: goto st72;
		case 48: goto st204;
		case 53: goto st204;
		case 58: goto st163;
		case 93: goto st462;
	}
	if ( (*p) < 65 ) {
		if ( 49 <= (*p) && (*p) <= 57 )
			goto st161;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st161;
	} else
		goto st161;
	goto st0;
st207:
	if ( ++p == pe )
		goto _test_eof207;
case 207:
	switch( (*p) ) {
		case 46: goto st73;
		case 58: goto st163;
		case 93: goto st462;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st208;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st160;
	} else
		goto st160;
	goto st0;
st208:
	if ( ++p == pe )
		goto _test_eof208;
case 208:
	switch( (*p) ) {
		case 46: goto st73;
		case 58: goto st163;
		case 93: goto st462;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st161;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st161;
	} else
		goto st161;
	goto st0;
st209:
	if ( ++p == pe )
		goto _test_eof209;
case 209:
	switch( (*p) ) {
		case 58: goto st163;
		case 93: goto st462;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st160;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st160;
	} else
		goto st160;
	goto st0;
st210:
	if ( ++p == pe )
		goto _test_eof210;
case 210:
	switch( (*p) ) {
		case 46: goto st73;
		case 58: goto st158;
		case 93: goto st462;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st211;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st155;
	} else
		goto st155;
	goto st0;
st211:
	if ( ++p == pe )
		goto _test_eof211;
case 211:
	switch( (*p) ) {
		case 46: goto st73;
		case 58: goto st158;
		case 93: goto st462;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st212;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st156;
	} else
		goto st156;
	goto st0;
st212:
	if ( ++p == pe )
		goto _test_eof212;
case 212:
	switch( (*p) ) {
		case 46: goto st73;
		case 58: goto st158;
		case 93: goto st462;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st157;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st157;
	} else
		goto st157;
	goto st0;
st213:
	if ( ++p == pe )
		goto _test_eof213;
case 213:
	switch( (*p) ) {
		case 46: goto st73;
		case 53: goto st214;
		case 58: goto st158;
		case 93: goto st462;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st155;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st155;
	} else
		goto st155;
	goto st0;
st214:
	if ( ++p == pe )
		goto _test_eof214;
case 214:
	switch( (*p) ) {
		case 32: goto st72;
		case 48: goto st212;
		case 53: goto st212;
		case 58: goto st158;
		case 93: goto st462;
	}
	if ( (*p) < 65 ) {
		if ( 49 <= (*p) && (*p) <= 57 )
			goto st156;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st156;
	} else
		goto st156;
	goto st0;
st215:
	if ( ++p == pe )
		goto _test_eof215;
case 215:
	switch( (*p) ) {
		case 46: goto st73;
		case 58: goto st158;
		case 93: goto st462;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st216;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st155;
	} else
		goto st155;
	goto st0;
st216:
	if ( ++p == pe )
		goto _test_eof216;
case 216:
	switch( (*p) ) {
		case 46: goto st73;
		case 58: goto st158;
		case 93: goto st462;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st156;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st156;
	} else
		goto st156;
	goto st0;
st217:
	if ( ++p == pe )
		goto _test_eof217;
case 217:
	switch( (*p) ) {
		case 58: goto st158;
		case 93: goto st462;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st155;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st155;
	} else
		goto st155;
	goto st0;
st218:
	if ( ++p == pe )
		goto _test_eof218;
case 218:
	switch( (*p) ) {
		case 46: goto st73;
		case 58: goto st153;
		case 93: goto st462;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st219;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st150;
	} else
		goto st150;
	goto st0;
st219:
	if ( ++p == pe )
		goto _test_eof219;
case 219:
	switch( (*p) ) {
		case 46: goto st73;
		case 58: goto st153;
		case 93: goto st462;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st220;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st151;
	} else
		goto st151;
	goto st0;
st220:
	if ( ++p == pe )
		goto _test_eof220;
case 220:
	switch( (*p) ) {
		case 46: goto st73;
		case 58: goto st153;
		case 93: goto st462;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st152;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st152;
	} else
		goto st152;
	goto st0;
st221:
	if ( ++p == pe )
		goto _test_eof221;
case 221:
	switch( (*p) ) {
		case 46: goto st73;
		case 53: goto st222;
		case 58: goto st153;
		case 93: goto st462;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st150;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st150;
	} else
		goto st150;
	goto st0;
st222:
	if ( ++p == pe )
		goto _test_eof222;
case 222:
	switch( (*p) ) {
		case 32: goto st72;
		case 48: goto st220;
		case 53: goto st220;
		case 58: goto st153;
		case 93: goto st462;
	}
	if ( (*p) < 65 ) {
		if ( 49 <= (*p) && (*p) <= 57 )
			goto st151;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st151;
	} else
		goto st151;
	goto st0;
st223:
	if ( ++p == pe )
		goto _test_eof223;
case 223:
	switch( (*p) ) {
		case 46: goto st73;
		case 58: goto st153;
		case 93: goto st462;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st224;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st150;
	} else
		goto st150;
	goto st0;
st224:
	if ( ++p == pe )
		goto _test_eof224;
case 224:
	switch( (*p) ) {
		case 46: goto st73;
		case 58: goto st153;
		case 93: goto st462;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st151;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st151;
	} else
		goto st151;
	goto st0;
st225:
	if ( ++p == pe )
		goto _test_eof225;
case 225:
	switch( (*p) ) {
		case 58: goto st153;
		case 93: goto st462;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st150;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st150;
	} else
		goto st150;
	goto st0;
st226:
	if ( ++p == pe )
		goto _test_eof226;
case 226:
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st227;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st227;
	} else
		goto st227;
	goto st0;
st227:
	if ( ++p == pe )
		goto _test_eof227;
case 227:
	if ( (*p) == 46 )
		goto st228;
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st227;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st227;
	} else
		goto st227;
	goto st0;
st228:
	if ( ++p == pe )
		goto _test_eof228;
case 228:
	switch( (*p) ) {
		case 33: goto st229;
		case 36: goto st229;
		case 61: goto st229;
		case 95: goto st229;
		case 126: goto st229;
	}
	if ( (*p) < 48 ) {
		if ( 38 <= (*p) && (*p) <= 46 )
			goto st229;
	} else if ( (*p) > 59 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto st229;
		} else if ( (*p) >= 65 )
			goto st229;
	} else
		goto st229;
	goto st0;
st229:
	if ( ++p == pe )
		goto _test_eof229;
case 229:
	switch( (*p) ) {
		case 33: goto st229;
		case 36: goto st229;
		case 61: goto st229;
		case 93: goto st462;
		case 95: goto st229;
		case 126: goto st229;
	}
	if ( (*p) < 48 ) {
		if ( 38 <= (*p) && (*p) <= 46 )
			goto st229;
	} else if ( (*p) > 59 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto st229;
		} else if ( (*p) >= 65 )
			goto st229;
	} else
		goto st229;
	goto st0;
tr501:
#line 18 "uri.rl"
	{ mark = p; }
	goto st492;
st492:
	if ( ++p == pe )
		goto _test_eof492;
case 492:
#line 4727 "uri.rl.cc"
	switch( (*p) ) {
		case 33: goto st34;
		case 35: goto tr494;
		case 37: goto st35;
		case 47: goto tr495;
		case 61: goto st34;
		case 63: goto tr497;
		case 64: goto tr38;
		case 95: goto st34;
		case 126: goto st34;
	}
	if ( (*p) < 58 ) {
		if ( (*p) > 46 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto st492;
		} else if ( (*p) >= 36 )
			goto st34;
	} else if ( (*p) > 59 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto st34;
		} else if ( (*p) >= 65 )
			goto st34;
	} else
		goto st34;
	goto st0;
tr478:
#line 18 "uri.rl"
	{ mark = p; }
	goto st493;
st493:
	if ( ++p == pe )
		goto _test_eof493;
case 493:
#line 4762 "uri.rl.cc"
	switch( (*p) ) {
		case 33: goto st468;
		case 35: goto tr486;
		case 37: goto st32;
		case 46: goto st494;
		case 47: goto tr487;
		case 58: goto tr500;
		case 61: goto st468;
		case 63: goto tr489;
		case 64: goto tr38;
		case 95: goto st468;
		case 126: goto st468;
	}
	if ( (*p) < 65 ) {
		if ( 36 <= (*p) && (*p) <= 59 )
			goto st468;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto st468;
	} else
		goto st468;
	goto st0;
st494:
	if ( ++p == pe )
		goto _test_eof494;
case 494:
	switch( (*p) ) {
		case 2: goto st13;
		case 32: goto st14;
		case 33: goto st468;
		case 35: goto tr486;
		case 37: goto st32;
		case 47: goto tr487;
		case 49: goto st505;
		case 50: goto st507;
		case 57: goto st506;
		case 58: goto tr500;
		case 59: goto st468;
		case 61: goto st468;
		case 63: goto tr489;
		case 64: goto tr38;
		case 95: goto st468;
		case 126: goto st468;
	}
	if ( (*p) < 48 ) {
		if ( 36 <= (*p) && (*p) <= 46 )
			goto st468;
	} else if ( (*p) > 56 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto st468;
		} else if ( (*p) >= 65 )
			goto st468;
	} else
		goto st495;
	goto st0;
st495:
	if ( ++p == pe )
		goto _test_eof495;
case 495:
	switch( (*p) ) {
		case 33: goto st468;
		case 35: goto tr486;
		case 37: goto st32;
		case 46: goto st496;
		case 47: goto tr487;
		case 58: goto tr500;
		case 61: goto st468;
		case 63: goto tr489;
		case 64: goto tr38;
		case 95: goto st468;
		case 126: goto st468;
	}
	if ( (*p) < 65 ) {
		if ( 36 <= (*p) && (*p) <= 59 )
			goto st468;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto st468;
	} else
		goto st468;
	goto st0;
st496:
	if ( ++p == pe )
		goto _test_eof496;
case 496:
	switch( (*p) ) {
		case 2: goto st17;
		case 32: goto st18;
		case 33: goto st468;
		case 35: goto tr486;
		case 37: goto st32;
		case 47: goto tr487;
		case 49: goto st501;
		case 50: goto st503;
		case 57: goto st502;
		case 58: goto tr500;
		case 59: goto st468;
		case 61: goto st468;
		case 63: goto tr489;
		case 64: goto tr38;
		case 95: goto st468;
		case 126: goto st468;
	}
	if ( (*p) < 48 ) {
		if ( 36 <= (*p) && (*p) <= 46 )
			goto st468;
	} else if ( (*p) > 56 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto st468;
		} else if ( (*p) >= 65 )
			goto st468;
	} else
		goto st497;
	goto st0;
st497:
	if ( ++p == pe )
		goto _test_eof497;
case 497:
	switch( (*p) ) {
		case 33: goto st468;
		case 35: goto tr486;
		case 37: goto st32;
		case 46: goto st498;
		case 47: goto tr487;
		case 58: goto tr500;
		case 61: goto st468;
		case 63: goto tr489;
		case 64: goto tr38;
		case 95: goto st468;
		case 126: goto st468;
	}
	if ( (*p) < 65 ) {
		if ( 36 <= (*p) && (*p) <= 59 )
			goto st468;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto st468;
	} else
		goto st468;
	goto st0;
st498:
	if ( ++p == pe )
		goto _test_eof498;
case 498:
	switch( (*p) ) {
		case 2: goto st21;
		case 32: goto st22;
		case 33: goto st468;
		case 35: goto tr486;
		case 37: goto st32;
		case 47: goto tr487;
		case 50: goto st499;
		case 58: goto tr500;
		case 61: goto st468;
		case 63: goto tr489;
		case 64: goto tr38;
		case 95: goto st468;
		case 126: goto st468;
	}
	if ( (*p) < 65 ) {
		if ( 36 <= (*p) && (*p) <= 59 )
			goto st468;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto st468;
	} else
		goto st468;
	goto st0;
st499:
	if ( ++p == pe )
		goto _test_eof499;
case 499:
	switch( (*p) ) {
		case 33: goto st468;
		case 35: goto tr486;
		case 37: goto st32;
		case 47: goto tr487;
		case 53: goto st500;
		case 58: goto tr500;
		case 61: goto st468;
		case 63: goto tr489;
		case 64: goto tr38;
		case 95: goto st468;
		case 126: goto st468;
	}
	if ( (*p) < 65 ) {
		if ( 36 <= (*p) && (*p) <= 59 )
			goto st468;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto st468;
	} else
		goto st468;
	goto st0;
st500:
	if ( ++p == pe )
		goto _test_eof500;
case 500:
	switch( (*p) ) {
		case 32: goto st462;
		case 33: goto st468;
		case 35: goto tr486;
		case 37: goto st32;
		case 47: goto tr487;
		case 58: goto tr500;
		case 61: goto st468;
		case 63: goto tr489;
		case 64: goto tr38;
		case 95: goto st468;
		case 126: goto st468;
	}
	if ( (*p) < 65 ) {
		if ( 36 <= (*p) && (*p) <= 59 )
			goto st468;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto st468;
	} else
		goto st468;
	goto st0;
st501:
	if ( ++p == pe )
		goto _test_eof501;
case 501:
	switch( (*p) ) {
		case 33: goto st468;
		case 35: goto tr486;
		case 37: goto st32;
		case 46: goto st498;
		case 47: goto tr487;
		case 58: goto tr500;
		case 59: goto st468;
		case 61: goto st468;
		case 63: goto tr489;
		case 64: goto tr38;
		case 95: goto st468;
		case 126: goto st468;
	}
	if ( (*p) < 48 ) {
		if ( 36 <= (*p) && (*p) <= 45 )
			goto st468;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto st468;
		} else if ( (*p) >= 65 )
			goto st468;
	} else
		goto st502;
	goto st0;
st502:
	if ( ++p == pe )
		goto _test_eof502;
case 502:
	switch( (*p) ) {
		case 33: goto st468;
		case 35: goto tr486;
		case 37: goto st32;
		case 46: goto st498;
		case 47: goto tr487;
		case 58: goto tr500;
		case 59: goto st468;
		case 61: goto st468;
		case 63: goto tr489;
		case 64: goto tr38;
		case 95: goto st468;
		case 126: goto st468;
	}
	if ( (*p) < 48 ) {
		if ( 36 <= (*p) && (*p) <= 45 )
			goto st468;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto st468;
		} else if ( (*p) >= 65 )
			goto st468;
	} else
		goto st497;
	goto st0;
st503:
	if ( ++p == pe )
		goto _test_eof503;
case 503:
	switch( (*p) ) {
		case 33: goto st468;
		case 35: goto tr486;
		case 37: goto st32;
		case 46: goto st498;
		case 47: goto tr487;
		case 53: goto st504;
		case 58: goto tr500;
		case 61: goto st468;
		case 63: goto tr489;
		case 64: goto tr38;
		case 95: goto st468;
		case 126: goto st468;
	}
	if ( (*p) < 65 ) {
		if ( 36 <= (*p) && (*p) <= 59 )
			goto st468;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto st468;
	} else
		goto st468;
	goto st0;
st504:
	if ( ++p == pe )
		goto _test_eof504;
case 504:
	switch( (*p) ) {
		case 32: goto st19;
		case 33: goto st468;
		case 35: goto tr486;
		case 37: goto st32;
		case 47: goto tr487;
		case 48: goto st497;
		case 53: goto st497;
		case 58: goto tr500;
		case 61: goto st468;
		case 63: goto tr489;
		case 64: goto tr38;
		case 95: goto st468;
		case 126: goto st468;
	}
	if ( (*p) < 65 ) {
		if ( 36 <= (*p) && (*p) <= 59 )
			goto st468;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto st468;
	} else
		goto st468;
	goto st0;
st505:
	if ( ++p == pe )
		goto _test_eof505;
case 505:
	switch( (*p) ) {
		case 33: goto st468;
		case 35: goto tr486;
		case 37: goto st32;
		case 46: goto st496;
		case 47: goto tr487;
		case 58: goto tr500;
		case 59: goto st468;
		case 61: goto st468;
		case 63: goto tr489;
		case 64: goto tr38;
		case 95: goto st468;
		case 126: goto st468;
	}
	if ( (*p) < 48 ) {
		if ( 36 <= (*p) && (*p) <= 45 )
			goto st468;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto st468;
		} else if ( (*p) >= 65 )
			goto st468;
	} else
		goto st506;
	goto st0;
st506:
	if ( ++p == pe )
		goto _test_eof506;
case 506:
	switch( (*p) ) {
		case 33: goto st468;
		case 35: goto tr486;
		case 37: goto st32;
		case 46: goto st496;
		case 47: goto tr487;
		case 58: goto tr500;
		case 59: goto st468;
		case 61: goto st468;
		case 63: goto tr489;
		case 64: goto tr38;
		case 95: goto st468;
		case 126: goto st468;
	}
	if ( (*p) < 48 ) {
		if ( 36 <= (*p) && (*p) <= 45 )
			goto st468;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto st468;
		} else if ( (*p) >= 65 )
			goto st468;
	} else
		goto st495;
	goto st0;
st507:
	if ( ++p == pe )
		goto _test_eof507;
case 507:
	switch( (*p) ) {
		case 33: goto st468;
		case 35: goto tr486;
		case 37: goto st32;
		case 46: goto st496;
		case 47: goto tr487;
		case 53: goto st508;
		case 58: goto tr500;
		case 61: goto st468;
		case 63: goto tr489;
		case 64: goto tr38;
		case 95: goto st468;
		case 126: goto st468;
	}
	if ( (*p) < 65 ) {
		if ( 36 <= (*p) && (*p) <= 59 )
			goto st468;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto st468;
	} else
		goto st468;
	goto st0;
st508:
	if ( ++p == pe )
		goto _test_eof508;
case 508:
	switch( (*p) ) {
		case 32: goto st15;
		case 33: goto st468;
		case 35: goto tr486;
		case 37: goto st32;
		case 47: goto tr487;
		case 48: goto st495;
		case 53: goto st495;
		case 58: goto tr500;
		case 61: goto st468;
		case 63: goto tr489;
		case 64: goto tr38;
		case 95: goto st468;
		case 126: goto st468;
	}
	if ( (*p) < 65 ) {
		if ( 36 <= (*p) && (*p) <= 59 )
			goto st468;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto st468;
	} else
		goto st468;
	goto st0;
tr479:
#line 18 "uri.rl"
	{ mark = p; }
	goto st509;
st509:
	if ( ++p == pe )
		goto _test_eof509;
case 509:
#line 5223 "uri.rl.cc"
	switch( (*p) ) {
		case 33: goto st468;
		case 35: goto tr486;
		case 37: goto st32;
		case 46: goto st494;
		case 47: goto tr487;
		case 58: goto tr500;
		case 59: goto st468;
		case 61: goto st468;
		case 63: goto tr489;
		case 64: goto tr38;
		case 95: goto st468;
		case 126: goto st468;
	}
	if ( (*p) < 48 ) {
		if ( 36 <= (*p) && (*p) <= 45 )
			goto st468;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto st468;
		} else if ( (*p) >= 65 )
			goto st468;
	} else
		goto st510;
	goto st0;
tr481:
#line 18 "uri.rl"
	{ mark = p; }
	goto st510;
st510:
	if ( ++p == pe )
		goto _test_eof510;
case 510:
#line 5258 "uri.rl.cc"
	switch( (*p) ) {
		case 33: goto st468;
		case 35: goto tr486;
		case 37: goto st32;
		case 46: goto st494;
		case 47: goto tr487;
		case 58: goto tr500;
		case 59: goto st468;
		case 61: goto st468;
		case 63: goto tr489;
		case 64: goto tr38;
		case 95: goto st468;
		case 126: goto st468;
	}
	if ( (*p) < 48 ) {
		if ( 36 <= (*p) && (*p) <= 45 )
			goto st468;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto st468;
		} else if ( (*p) >= 65 )
			goto st468;
	} else
		goto st493;
	goto st0;
tr480:
#line 18 "uri.rl"
	{ mark = p; }
	goto st511;
st511:
	if ( ++p == pe )
		goto _test_eof511;
case 511:
#line 5293 "uri.rl.cc"
	switch( (*p) ) {
		case 33: goto st468;
		case 35: goto tr486;
		case 37: goto st32;
		case 46: goto st494;
		case 47: goto tr487;
		case 53: goto st512;
		case 58: goto tr500;
		case 61: goto st468;
		case 63: goto tr489;
		case 64: goto tr38;
		case 95: goto st468;
		case 126: goto st468;
	}
	if ( (*p) < 65 ) {
		if ( 36 <= (*p) && (*p) <= 59 )
			goto st468;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto st468;
	} else
		goto st468;
	goto st0;
st512:
	if ( ++p == pe )
		goto _test_eof512;
case 512:
	switch( (*p) ) {
		case 32: goto st11;
		case 33: goto st468;
		case 35: goto tr486;
		case 37: goto st32;
		case 47: goto tr487;
		case 48: goto st493;
		case 53: goto st493;
		case 58: goto tr500;
		case 61: goto st468;
		case 63: goto tr489;
		case 64: goto tr38;
		case 95: goto st468;
		case 126: goto st468;
	}
	if ( (*p) < 65 ) {
		if ( 36 <= (*p) && (*p) <= 59 )
			goto st468;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto st468;
	} else
		goto st468;
	goto st0;
tr461:
#line 17 "uri.rl"
	{ mark = p; }
	goto st513;
st513:
	if ( ++p == pe )
		goto _test_eof513;
case 513:
#line 5353 "uri.rl.cc"
	switch( (*p) ) {
		case 34: goto st0;
		case 35: goto st455;
		case 37: goto st3;
		case 43: goto st513;
		case 47: goto st457;
		case 58: goto tr548;
		case 60: goto st0;
		case 62: goto st0;
		case 63: goto st458;
		case 96: goto st0;
		case 127: goto st0;
	}
	if ( (*p) < 65 ) {
		if ( (*p) > 32 ) {
			if ( 45 <= (*p) && (*p) <= 57 )
				goto st513;
		} else if ( (*p) >= 0 )
			goto st0;
	} else if ( (*p) > 90 ) {
		if ( (*p) < 97 ) {
			if ( 91 <= (*p) && (*p) <= 94 )
				goto st0;
		} else if ( (*p) > 122 ) {
			if ( 123 <= (*p) && (*p) <= 125 )
				goto st0;
		} else
			goto st513;
	} else
		goto st513;
	goto st454;
tr548:
#line 20 "uri.rl"
	{
        uri->setScheme(std::string(mark, p - mark));
        mark = NULL;
    }
	goto st514;
st514:
	if ( ++p == pe )
		goto _test_eof514;
case 514:
#line 5396 "uri.rl.cc"
	switch( (*p) ) {
		case 34: goto st0;
		case 35: goto st455;
		case 37: goto st5;
		case 47: goto st515;
		case 60: goto st0;
		case 62: goto st0;
		case 63: goto st458;
		case 96: goto st0;
		case 127: goto st0;
	}
	if ( (*p) < 91 ) {
		if ( 0 <= (*p) && (*p) <= 32 )
			goto st0;
	} else if ( (*p) > 94 ) {
		if ( 123 <= (*p) && (*p) <= 125 )
			goto st0;
	} else
		goto st0;
	goto st457;
st515:
	if ( ++p == pe )
		goto _test_eof515;
case 515:
	switch( (*p) ) {
		case 34: goto st0;
		case 35: goto st455;
		case 37: goto st5;
		case 47: goto st516;
		case 60: goto st0;
		case 62: goto st0;
		case 63: goto st458;
		case 96: goto st0;
		case 127: goto st0;
	}
	if ( (*p) < 91 ) {
		if ( 0 <= (*p) && (*p) <= 32 )
			goto st0;
	} else if ( (*p) > 94 ) {
		if ( 123 <= (*p) && (*p) <= 125 )
			goto st0;
	} else
		goto st0;
	goto st457;
st516:
	if ( ++p == pe )
		goto _test_eof516;
case 516:
	switch( (*p) ) {
		case 2: goto tr551;
		case 32: goto tr552;
		case 33: goto tr553;
		case 35: goto tr554;
		case 37: goto tr555;
		case 47: goto tr556;
		case 49: goto tr558;
		case 50: goto tr559;
		case 57: goto tr560;
		case 58: goto tr561;
		case 59: goto tr553;
		case 61: goto tr553;
		case 63: goto tr562;
		case 64: goto tr563;
		case 91: goto tr564;
		case 95: goto tr553;
		case 126: goto tr553;
	}
	if ( (*p) < 48 ) {
		if ( 36 <= (*p) && (*p) <= 46 )
			goto tr553;
	} else if ( (*p) > 56 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr553;
		} else if ( (*p) >= 65 )
			goto tr553;
	} else
		goto tr557;
	goto st0;
tr551:
#line 18 "uri.rl"
	{ mark = p; }
	goto st230;
st230:
	if ( ++p == pe )
		goto _test_eof230;
case 230:
#line 5484 "uri.rl.cc"
	switch( (*p) ) {
		case 32: goto st231;
		case 48: goto st231;
		case 52: goto st231;
	}
	goto st0;
tr552:
#line 18 "uri.rl"
	{ mark = p; }
	goto st231;
st231:
	if ( ++p == pe )
		goto _test_eof231;
case 231:
#line 5499 "uri.rl.cc"
	if ( 48 <= (*p) && (*p) <= 57 )
		goto st232;
	goto st0;
st232:
	if ( ++p == pe )
		goto _test_eof232;
case 232:
	if ( (*p) == 46 )
		goto st233;
	goto st0;
st233:
	if ( ++p == pe )
		goto _test_eof233;
case 233:
	switch( (*p) ) {
		case 2: goto st234;
		case 32: goto st235;
		case 49: goto st251;
		case 50: goto st253;
		case 57: goto st252;
	}
	if ( 48 <= (*p) && (*p) <= 56 )
		goto st236;
	goto st0;
st234:
	if ( ++p == pe )
		goto _test_eof234;
case 234:
	switch( (*p) ) {
		case 32: goto st235;
		case 48: goto st235;
		case 52: goto st235;
	}
	goto st0;
st235:
	if ( ++p == pe )
		goto _test_eof235;
case 235:
	if ( 48 <= (*p) && (*p) <= 57 )
		goto st236;
	goto st0;
st236:
	if ( ++p == pe )
		goto _test_eof236;
case 236:
	if ( (*p) == 46 )
		goto st237;
	goto st0;
st237:
	if ( ++p == pe )
		goto _test_eof237;
case 237:
	switch( (*p) ) {
		case 2: goto st238;
		case 32: goto st239;
		case 49: goto st247;
		case 50: goto st249;
		case 57: goto st248;
	}
	if ( 48 <= (*p) && (*p) <= 56 )
		goto st240;
	goto st0;
st238:
	if ( ++p == pe )
		goto _test_eof238;
case 238:
	switch( (*p) ) {
		case 32: goto st239;
		case 48: goto st239;
		case 52: goto st239;
	}
	goto st0;
st239:
	if ( ++p == pe )
		goto _test_eof239;
case 239:
	if ( 48 <= (*p) && (*p) <= 57 )
		goto st240;
	goto st0;
st240:
	if ( ++p == pe )
		goto _test_eof240;
case 240:
	if ( (*p) == 46 )
		goto st241;
	goto st0;
st241:
	if ( ++p == pe )
		goto _test_eof241;
case 241:
	switch( (*p) ) {
		case 2: goto st242;
		case 32: goto st243;
		case 49: goto st521;
		case 50: goto st523;
		case 57: goto st522;
	}
	if ( 48 <= (*p) && (*p) <= 56 )
		goto st517;
	goto st0;
st242:
	if ( ++p == pe )
		goto _test_eof242;
case 242:
	switch( (*p) ) {
		case 32: goto st243;
		case 48: goto st243;
		case 52: goto st243;
	}
	goto st0;
st243:
	if ( ++p == pe )
		goto _test_eof243;
case 243:
	if ( 48 <= (*p) && (*p) <= 57 )
		goto st517;
	goto st0;
st517:
	if ( ++p == pe )
		goto _test_eof517;
case 517:
	switch( (*p) ) {
		case 35: goto tr565;
		case 47: goto tr566;
		case 58: goto tr567;
		case 63: goto tr568;
	}
	goto st0;
tr556:
#line 18 "uri.rl"
	{ mark = p; }
#line 40 "uri.rl"
	{
        if (mark != NULL) {
            // std::cout << std::string(mark, fpc - mark) << std::endl;
            uri->setHost(std::string(mark, p - mark));
        }
    }
	goto st518;
tr573:
#line 18 "uri.rl"
	{ mark = p; }
#line 27 "uri.rl"
	{
        if (p != mark) {
            uri->setPort(atoi(mark));
        }
        mark = NULL;
    }
	goto st518;
tr566:
#line 40 "uri.rl"
	{
        if (mark != NULL) {
            // std::cout << std::string(mark, fpc - mark) << std::endl;
            uri->setHost(std::string(mark, p - mark));
        }
    }
#line 18 "uri.rl"
	{ mark = p; }
	goto st518;
tr577:
#line 27 "uri.rl"
	{
        if (p != mark) {
            uri->setPort(atoi(mark));
        }
        mark = NULL;
    }
#line 18 "uri.rl"
	{ mark = p; }
	goto st518;
st518:
	if ( ++p == pe )
		goto _test_eof518;
case 518:
#line 5676 "uri.rl.cc"
	switch( (*p) ) {
		case 34: goto st0;
		case 35: goto tr569;
		case 37: goto st244;
		case 60: goto st0;
		case 62: goto st0;
		case 63: goto tr571;
		case 96: goto st0;
		case 127: goto st0;
	}
	if ( (*p) < 91 ) {
		if ( 0 <= (*p) && (*p) <= 32 )
			goto st0;
	} else if ( (*p) > 94 ) {
		if ( 123 <= (*p) && (*p) <= 125 )
			goto st0;
	} else
		goto st0;
	goto st518;
st244:
	if ( ++p == pe )
		goto _test_eof244;
case 244:
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st245;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st245;
	} else
		goto st245;
	goto st0;
st245:
	if ( ++p == pe )
		goto _test_eof245;
case 245:
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st518;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st518;
	} else
		goto st518;
	goto st0;
tr590:
#line 18 "uri.rl"
	{ mark = p; }
#line 40 "uri.rl"
	{
        if (mark != NULL) {
            // std::cout << std::string(mark, fpc - mark) << std::endl;
            uri->setHost(std::string(mark, p - mark));
        }
    }
	goto st519;
tr567:
#line 40 "uri.rl"
	{
        if (mark != NULL) {
            // std::cout << std::string(mark, fpc - mark) << std::endl;
            uri->setHost(std::string(mark, p - mark));
        }
    }
	goto st519;
st519:
	if ( ++p == pe )
		goto _test_eof519;
case 519:
#line 5746 "uri.rl.cc"
	switch( (*p) ) {
		case 35: goto tr572;
		case 47: goto tr573;
		case 63: goto tr575;
	}
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr574;
	goto st0;
tr574:
#line 18 "uri.rl"
	{ mark = p; }
	goto st520;
st520:
	if ( ++p == pe )
		goto _test_eof520;
case 520:
#line 5763 "uri.rl.cc"
	switch( (*p) ) {
		case 35: goto tr576;
		case 47: goto tr577;
		case 63: goto tr579;
	}
	if ( 48 <= (*p) && (*p) <= 57 )
		goto st520;
	goto st0;
st521:
	if ( ++p == pe )
		goto _test_eof521;
case 521:
	switch( (*p) ) {
		case 35: goto tr565;
		case 47: goto tr566;
		case 58: goto tr567;
		case 63: goto tr568;
	}
	if ( 48 <= (*p) && (*p) <= 57 )
		goto st522;
	goto st0;
st522:
	if ( ++p == pe )
		goto _test_eof522;
case 522:
	switch( (*p) ) {
		case 35: goto tr565;
		case 47: goto tr566;
		case 58: goto tr567;
		case 63: goto tr568;
	}
	if ( 48 <= (*p) && (*p) <= 57 )
		goto st517;
	goto st0;
st523:
	if ( ++p == pe )
		goto _test_eof523;
case 523:
	switch( (*p) ) {
		case 35: goto tr565;
		case 47: goto tr566;
		case 53: goto st246;
		case 58: goto tr567;
		case 63: goto tr568;
	}
	goto st0;
st246:
	if ( ++p == pe )
		goto _test_eof246;
case 246:
	switch( (*p) ) {
		case 32: goto st517;
		case 48: goto st517;
		case 53: goto st517;
	}
	goto st0;
st247:
	if ( ++p == pe )
		goto _test_eof247;
case 247:
	if ( (*p) == 46 )
		goto st241;
	if ( 48 <= (*p) && (*p) <= 57 )
		goto st248;
	goto st0;
st248:
	if ( ++p == pe )
		goto _test_eof248;
case 248:
	if ( (*p) == 46 )
		goto st241;
	if ( 48 <= (*p) && (*p) <= 57 )
		goto st240;
	goto st0;
st249:
	if ( ++p == pe )
		goto _test_eof249;
case 249:
	switch( (*p) ) {
		case 46: goto st241;
		case 53: goto st250;
	}
	goto st0;
st250:
	if ( ++p == pe )
		goto _test_eof250;
case 250:
	switch( (*p) ) {
		case 32: goto st240;
		case 48: goto st240;
		case 53: goto st240;
	}
	goto st0;
st251:
	if ( ++p == pe )
		goto _test_eof251;
case 251:
	if ( (*p) == 46 )
		goto st237;
	if ( 48 <= (*p) && (*p) <= 57 )
		goto st252;
	goto st0;
st252:
	if ( ++p == pe )
		goto _test_eof252;
case 252:
	if ( (*p) == 46 )
		goto st237;
	if ( 48 <= (*p) && (*p) <= 57 )
		goto st236;
	goto st0;
st253:
	if ( ++p == pe )
		goto _test_eof253;
case 253:
	switch( (*p) ) {
		case 46: goto st237;
		case 53: goto st254;
	}
	goto st0;
st254:
	if ( ++p == pe )
		goto _test_eof254;
case 254:
	switch( (*p) ) {
		case 32: goto st236;
		case 48: goto st236;
		case 53: goto st236;
	}
	goto st0;
tr553:
#line 18 "uri.rl"
	{ mark = p; }
	goto st524;
st524:
	if ( ++p == pe )
		goto _test_eof524;
case 524:
#line 5902 "uri.rl.cc"
	switch( (*p) ) {
		case 33: goto st524;
		case 35: goto tr565;
		case 37: goto st255;
		case 47: goto tr566;
		case 58: goto tr582;
		case 61: goto st524;
		case 63: goto tr568;
		case 64: goto tr263;
		case 95: goto st524;
		case 126: goto st524;
	}
	if ( (*p) < 65 ) {
		if ( 36 <= (*p) && (*p) <= 59 )
			goto st524;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto st524;
	} else
		goto st524;
	goto st0;
tr555:
#line 18 "uri.rl"
	{ mark = p; }
	goto st255;
st255:
	if ( ++p == pe )
		goto _test_eof255;
case 255:
#line 5932 "uri.rl.cc"
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st256;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st256;
	} else
		goto st256;
	goto st0;
st256:
	if ( ++p == pe )
		goto _test_eof256;
case 256:
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st524;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st524;
	} else
		goto st524;
	goto st0;
tr561:
#line 18 "uri.rl"
	{ mark = p; }
#line 40 "uri.rl"
	{
        if (mark != NULL) {
            // std::cout << std::string(mark, fpc - mark) << std::endl;
            uri->setHost(std::string(mark, p - mark));
        }
    }
	goto st525;
tr582:
#line 40 "uri.rl"
	{
        if (mark != NULL) {
            // std::cout << std::string(mark, fpc - mark) << std::endl;
            uri->setHost(std::string(mark, p - mark));
        }
    }
	goto st525;
st525:
	if ( ++p == pe )
		goto _test_eof525;
case 525:
#line 5979 "uri.rl.cc"
	switch( (*p) ) {
		case 33: goto st257;
		case 35: goto tr572;
		case 37: goto st258;
		case 47: goto tr573;
		case 61: goto st257;
		case 63: goto tr575;
		case 64: goto tr263;
		case 95: goto st257;
		case 126: goto st257;
	}
	if ( (*p) < 58 ) {
		if ( (*p) > 46 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr583;
		} else if ( (*p) >= 36 )
			goto st257;
	} else if ( (*p) > 59 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto st257;
		} else if ( (*p) >= 65 )
			goto st257;
	} else
		goto st257;
	goto st0;
st257:
	if ( ++p == pe )
		goto _test_eof257;
case 257:
	switch( (*p) ) {
		case 33: goto st257;
		case 37: goto st258;
		case 61: goto st257;
		case 64: goto tr263;
		case 95: goto st257;
		case 126: goto st257;
	}
	if ( (*p) < 48 ) {
		if ( 36 <= (*p) && (*p) <= 46 )
			goto st257;
	} else if ( (*p) > 59 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto st257;
		} else if ( (*p) >= 65 )
			goto st257;
	} else
		goto st257;
	goto st0;
st258:
	if ( ++p == pe )
		goto _test_eof258;
case 258:
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st259;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st259;
	} else
		goto st259;
	goto st0;
st259:
	if ( ++p == pe )
		goto _test_eof259;
case 259:
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st257;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st257;
	} else
		goto st257;
	goto st0;
tr263:
#line 33 "uri.rl"
	{
        if (mark) {
            // std::cout << std::string(mark, fpc - mark) << std::endl;
            uri->setUserinfo(std::string(mark, p - mark));
        }
        mark = NULL;
    }
	goto st526;
tr563:
#line 18 "uri.rl"
	{ mark = p; }
#line 33 "uri.rl"
	{
        if (mark) {
            // std::cout << std::string(mark, fpc - mark) << std::endl;
            uri->setUserinfo(std::string(mark, p - mark));
        }
        mark = NULL;
    }
	goto st526;
st526:
	if ( ++p == pe )
		goto _test_eof526;
case 526:
#line 6082 "uri.rl.cc"
	switch( (*p) ) {
		case 2: goto tr551;
		case 32: goto tr552;
		case 33: goto tr584;
		case 35: goto tr554;
		case 37: goto tr585;
		case 47: goto tr556;
		case 49: goto tr587;
		case 50: goto tr588;
		case 57: goto tr589;
		case 58: goto tr590;
		case 59: goto tr584;
		case 61: goto tr584;
		case 63: goto tr562;
		case 91: goto tr564;
		case 95: goto tr584;
		case 126: goto tr584;
	}
	if ( (*p) < 48 ) {
		if ( 36 <= (*p) && (*p) <= 46 )
			goto tr584;
	} else if ( (*p) > 56 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr584;
		} else if ( (*p) >= 65 )
			goto tr584;
	} else
		goto tr586;
	goto st0;
tr584:
#line 18 "uri.rl"
	{ mark = p; }
	goto st527;
st527:
	if ( ++p == pe )
		goto _test_eof527;
case 527:
#line 6121 "uri.rl.cc"
	switch( (*p) ) {
		case 33: goto st527;
		case 35: goto tr565;
		case 37: goto st260;
		case 47: goto tr566;
		case 58: goto tr567;
		case 61: goto st527;
		case 63: goto tr568;
		case 95: goto st527;
		case 126: goto st527;
	}
	if ( (*p) < 65 ) {
		if ( 36 <= (*p) && (*p) <= 59 )
			goto st527;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto st527;
	} else
		goto st527;
	goto st0;
tr585:
#line 18 "uri.rl"
	{ mark = p; }
	goto st260;
st260:
	if ( ++p == pe )
		goto _test_eof260;
case 260:
#line 6150 "uri.rl.cc"
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st261;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st261;
	} else
		goto st261;
	goto st0;
st261:
	if ( ++p == pe )
		goto _test_eof261;
case 261:
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st527;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st527;
	} else
		goto st527;
	goto st0;
tr586:
#line 18 "uri.rl"
	{ mark = p; }
	goto st528;
st528:
	if ( ++p == pe )
		goto _test_eof528;
case 528:
#line 6181 "uri.rl.cc"
	switch( (*p) ) {
		case 33: goto st527;
		case 35: goto tr565;
		case 37: goto st260;
		case 46: goto st529;
		case 47: goto tr566;
		case 58: goto tr567;
		case 61: goto st527;
		case 63: goto tr568;
		case 95: goto st527;
		case 126: goto st527;
	}
	if ( (*p) < 65 ) {
		if ( 36 <= (*p) && (*p) <= 59 )
			goto st527;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto st527;
	} else
		goto st527;
	goto st0;
st529:
	if ( ++p == pe )
		goto _test_eof529;
case 529:
	switch( (*p) ) {
		case 2: goto st234;
		case 32: goto st235;
		case 33: goto st527;
		case 35: goto tr565;
		case 37: goto st260;
		case 47: goto tr566;
		case 49: goto st540;
		case 50: goto st542;
		case 57: goto st541;
		case 58: goto tr567;
		case 59: goto st527;
		case 61: goto st527;
		case 63: goto tr568;
		case 95: goto st527;
		case 126: goto st527;
	}
	if ( (*p) < 48 ) {
		if ( 36 <= (*p) && (*p) <= 46 )
			goto st527;
	} else if ( (*p) > 56 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto st527;
		} else if ( (*p) >= 65 )
			goto st527;
	} else
		goto st530;
	goto st0;
st530:
	if ( ++p == pe )
		goto _test_eof530;
case 530:
	switch( (*p) ) {
		case 33: goto st527;
		case 35: goto tr565;
		case 37: goto st260;
		case 46: goto st531;
		case 47: goto tr566;
		case 58: goto tr567;
		case 61: goto st527;
		case 63: goto tr568;
		case 95: goto st527;
		case 126: goto st527;
	}
	if ( (*p) < 65 ) {
		if ( 36 <= (*p) && (*p) <= 59 )
			goto st527;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto st527;
	} else
		goto st527;
	goto st0;
st531:
	if ( ++p == pe )
		goto _test_eof531;
case 531:
	switch( (*p) ) {
		case 2: goto st238;
		case 32: goto st239;
		case 33: goto st527;
		case 35: goto tr565;
		case 37: goto st260;
		case 47: goto tr566;
		case 49: goto st536;
		case 50: goto st538;
		case 57: goto st537;
		case 58: goto tr567;
		case 59: goto st527;
		case 61: goto st527;
		case 63: goto tr568;
		case 95: goto st527;
		case 126: goto st527;
	}
	if ( (*p) < 48 ) {
		if ( 36 <= (*p) && (*p) <= 46 )
			goto st527;
	} else if ( (*p) > 56 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto st527;
		} else if ( (*p) >= 65 )
			goto st527;
	} else
		goto st532;
	goto st0;
st532:
	if ( ++p == pe )
		goto _test_eof532;
case 532:
	switch( (*p) ) {
		case 33: goto st527;
		case 35: goto tr565;
		case 37: goto st260;
		case 46: goto st533;
		case 47: goto tr566;
		case 58: goto tr567;
		case 61: goto st527;
		case 63: goto tr568;
		case 95: goto st527;
		case 126: goto st527;
	}
	if ( (*p) < 65 ) {
		if ( 36 <= (*p) && (*p) <= 59 )
			goto st527;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto st527;
	} else
		goto st527;
	goto st0;
st533:
	if ( ++p == pe )
		goto _test_eof533;
case 533:
	switch( (*p) ) {
		case 2: goto st242;
		case 32: goto st243;
		case 33: goto st527;
		case 35: goto tr565;
		case 37: goto st260;
		case 47: goto tr566;
		case 50: goto st534;
		case 58: goto tr567;
		case 61: goto st527;
		case 63: goto tr568;
		case 95: goto st527;
		case 126: goto st527;
	}
	if ( (*p) < 65 ) {
		if ( 36 <= (*p) && (*p) <= 59 )
			goto st527;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto st527;
	} else
		goto st527;
	goto st0;
st534:
	if ( ++p == pe )
		goto _test_eof534;
case 534:
	switch( (*p) ) {
		case 33: goto st527;
		case 35: goto tr565;
		case 37: goto st260;
		case 47: goto tr566;
		case 53: goto st535;
		case 58: goto tr567;
		case 61: goto st527;
		case 63: goto tr568;
		case 95: goto st527;
		case 126: goto st527;
	}
	if ( (*p) < 65 ) {
		if ( 36 <= (*p) && (*p) <= 59 )
			goto st527;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto st527;
	} else
		goto st527;
	goto st0;
st535:
	if ( ++p == pe )
		goto _test_eof535;
case 535:
	switch( (*p) ) {
		case 32: goto st517;
		case 33: goto st527;
		case 35: goto tr565;
		case 37: goto st260;
		case 47: goto tr566;
		case 58: goto tr567;
		case 61: goto st527;
		case 63: goto tr568;
		case 95: goto st527;
		case 126: goto st527;
	}
	if ( (*p) < 65 ) {
		if ( 36 <= (*p) && (*p) <= 59 )
			goto st527;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto st527;
	} else
		goto st527;
	goto st0;
st536:
	if ( ++p == pe )
		goto _test_eof536;
case 536:
	switch( (*p) ) {
		case 33: goto st527;
		case 35: goto tr565;
		case 37: goto st260;
		case 46: goto st533;
		case 47: goto tr566;
		case 58: goto tr567;
		case 59: goto st527;
		case 61: goto st527;
		case 63: goto tr568;
		case 95: goto st527;
		case 126: goto st527;
	}
	if ( (*p) < 48 ) {
		if ( 36 <= (*p) && (*p) <= 45 )
			goto st527;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto st527;
		} else if ( (*p) >= 65 )
			goto st527;
	} else
		goto st537;
	goto st0;
st537:
	if ( ++p == pe )
		goto _test_eof537;
case 537:
	switch( (*p) ) {
		case 33: goto st527;
		case 35: goto tr565;
		case 37: goto st260;
		case 46: goto st533;
		case 47: goto tr566;
		case 58: goto tr567;
		case 59: goto st527;
		case 61: goto st527;
		case 63: goto tr568;
		case 95: goto st527;
		case 126: goto st527;
	}
	if ( (*p) < 48 ) {
		if ( 36 <= (*p) && (*p) <= 45 )
			goto st527;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto st527;
		} else if ( (*p) >= 65 )
			goto st527;
	} else
		goto st532;
	goto st0;
st538:
	if ( ++p == pe )
		goto _test_eof538;
case 538:
	switch( (*p) ) {
		case 33: goto st527;
		case 35: goto tr565;
		case 37: goto st260;
		case 46: goto st533;
		case 47: goto tr566;
		case 53: goto st539;
		case 58: goto tr567;
		case 61: goto st527;
		case 63: goto tr568;
		case 95: goto st527;
		case 126: goto st527;
	}
	if ( (*p) < 65 ) {
		if ( 36 <= (*p) && (*p) <= 59 )
			goto st527;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto st527;
	} else
		goto st527;
	goto st0;
st539:
	if ( ++p == pe )
		goto _test_eof539;
case 539:
	switch( (*p) ) {
		case 32: goto st240;
		case 33: goto st527;
		case 35: goto tr565;
		case 37: goto st260;
		case 47: goto tr566;
		case 48: goto st532;
		case 53: goto st532;
		case 58: goto tr567;
		case 61: goto st527;
		case 63: goto tr568;
		case 95: goto st527;
		case 126: goto st527;
	}
	if ( (*p) < 65 ) {
		if ( 36 <= (*p) && (*p) <= 59 )
			goto st527;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto st527;
	} else
		goto st527;
	goto st0;
st540:
	if ( ++p == pe )
		goto _test_eof540;
case 540:
	switch( (*p) ) {
		case 33: goto st527;
		case 35: goto tr565;
		case 37: goto st260;
		case 46: goto st531;
		case 47: goto tr566;
		case 58: goto tr567;
		case 59: goto st527;
		case 61: goto st527;
		case 63: goto tr568;
		case 95: goto st527;
		case 126: goto st527;
	}
	if ( (*p) < 48 ) {
		if ( 36 <= (*p) && (*p) <= 45 )
			goto st527;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto st527;
		} else if ( (*p) >= 65 )
			goto st527;
	} else
		goto st541;
	goto st0;
st541:
	if ( ++p == pe )
		goto _test_eof541;
case 541:
	switch( (*p) ) {
		case 33: goto st527;
		case 35: goto tr565;
		case 37: goto st260;
		case 46: goto st531;
		case 47: goto tr566;
		case 58: goto tr567;
		case 59: goto st527;
		case 61: goto st527;
		case 63: goto tr568;
		case 95: goto st527;
		case 126: goto st527;
	}
	if ( (*p) < 48 ) {
		if ( 36 <= (*p) && (*p) <= 45 )
			goto st527;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto st527;
		} else if ( (*p) >= 65 )
			goto st527;
	} else
		goto st530;
	goto st0;
st542:
	if ( ++p == pe )
		goto _test_eof542;
case 542:
	switch( (*p) ) {
		case 33: goto st527;
		case 35: goto tr565;
		case 37: goto st260;
		case 46: goto st531;
		case 47: goto tr566;
		case 53: goto st543;
		case 58: goto tr567;
		case 61: goto st527;
		case 63: goto tr568;
		case 95: goto st527;
		case 126: goto st527;
	}
	if ( (*p) < 65 ) {
		if ( 36 <= (*p) && (*p) <= 59 )
			goto st527;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto st527;
	} else
		goto st527;
	goto st0;
st543:
	if ( ++p == pe )
		goto _test_eof543;
case 543:
	switch( (*p) ) {
		case 32: goto st236;
		case 33: goto st527;
		case 35: goto tr565;
		case 37: goto st260;
		case 47: goto tr566;
		case 48: goto st530;
		case 53: goto st530;
		case 58: goto tr567;
		case 61: goto st527;
		case 63: goto tr568;
		case 95: goto st527;
		case 126: goto st527;
	}
	if ( (*p) < 65 ) {
		if ( 36 <= (*p) && (*p) <= 59 )
			goto st527;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto st527;
	} else
		goto st527;
	goto st0;
tr587:
#line 18 "uri.rl"
	{ mark = p; }
	goto st544;
st544:
	if ( ++p == pe )
		goto _test_eof544;
case 544:
#line 6626 "uri.rl.cc"
	switch( (*p) ) {
		case 33: goto st527;
		case 35: goto tr565;
		case 37: goto st260;
		case 46: goto st529;
		case 47: goto tr566;
		case 58: goto tr567;
		case 59: goto st527;
		case 61: goto st527;
		case 63: goto tr568;
		case 95: goto st527;
		case 126: goto st527;
	}
	if ( (*p) < 48 ) {
		if ( 36 <= (*p) && (*p) <= 45 )
			goto st527;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto st527;
		} else if ( (*p) >= 65 )
			goto st527;
	} else
		goto st545;
	goto st0;
tr589:
#line 18 "uri.rl"
	{ mark = p; }
	goto st545;
st545:
	if ( ++p == pe )
		goto _test_eof545;
case 545:
#line 6660 "uri.rl.cc"
	switch( (*p) ) {
		case 33: goto st527;
		case 35: goto tr565;
		case 37: goto st260;
		case 46: goto st529;
		case 47: goto tr566;
		case 58: goto tr567;
		case 59: goto st527;
		case 61: goto st527;
		case 63: goto tr568;
		case 95: goto st527;
		case 126: goto st527;
	}
	if ( (*p) < 48 ) {
		if ( 36 <= (*p) && (*p) <= 45 )
			goto st527;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto st527;
		} else if ( (*p) >= 65 )
			goto st527;
	} else
		goto st528;
	goto st0;
tr588:
#line 18 "uri.rl"
	{ mark = p; }
	goto st546;
st546:
	if ( ++p == pe )
		goto _test_eof546;
case 546:
#line 6694 "uri.rl.cc"
	switch( (*p) ) {
		case 33: goto st527;
		case 35: goto tr565;
		case 37: goto st260;
		case 46: goto st529;
		case 47: goto tr566;
		case 53: goto st547;
		case 58: goto tr567;
		case 61: goto st527;
		case 63: goto tr568;
		case 95: goto st527;
		case 126: goto st527;
	}
	if ( (*p) < 65 ) {
		if ( 36 <= (*p) && (*p) <= 59 )
			goto st527;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto st527;
	} else
		goto st527;
	goto st0;
st547:
	if ( ++p == pe )
		goto _test_eof547;
case 547:
	switch( (*p) ) {
		case 32: goto st232;
		case 33: goto st527;
		case 35: goto tr565;
		case 37: goto st260;
		case 47: goto tr566;
		case 48: goto st528;
		case 53: goto st528;
		case 58: goto tr567;
		case 61: goto st527;
		case 63: goto tr568;
		case 95: goto st527;
		case 126: goto st527;
	}
	if ( (*p) < 65 ) {
		if ( 36 <= (*p) && (*p) <= 59 )
			goto st527;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto st527;
	} else
		goto st527;
	goto st0;
tr564:
#line 18 "uri.rl"
	{ mark = p; }
	goto st262;
st262:
	if ( ++p == pe )
		goto _test_eof262;
case 262:
#line 6752 "uri.rl.cc"
	switch( (*p) ) {
		case 58: goto st370;
		case 118: goto st449;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st263;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st263;
	} else
		goto st263;
	goto st0;
st263:
	if ( ++p == pe )
		goto _test_eof263;
case 263:
	if ( (*p) == 58 )
		goto st267;
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st264;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st264;
	} else
		goto st264;
	goto st0;
st264:
	if ( ++p == pe )
		goto _test_eof264;
case 264:
	if ( (*p) == 58 )
		goto st267;
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st265;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st265;
	} else
		goto st265;
	goto st0;
st265:
	if ( ++p == pe )
		goto _test_eof265;
case 265:
	if ( (*p) == 58 )
		goto st267;
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st266;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st266;
	} else
		goto st266;
	goto st0;
st266:
	if ( ++p == pe )
		goto _test_eof266;
case 266:
	if ( (*p) == 58 )
		goto st267;
	goto st0;
st267:
	if ( ++p == pe )
		goto _test_eof267;
case 267:
	if ( (*p) == 58 )
		goto st365;
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st268;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st268;
	} else
		goto st268;
	goto st0;
st268:
	if ( ++p == pe )
		goto _test_eof268;
case 268:
	if ( (*p) == 58 )
		goto st272;
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st269;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st269;
	} else
		goto st269;
	goto st0;
st269:
	if ( ++p == pe )
		goto _test_eof269;
case 269:
	if ( (*p) == 58 )
		goto st272;
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st270;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st270;
	} else
		goto st270;
	goto st0;
st270:
	if ( ++p == pe )
		goto _test_eof270;
case 270:
	if ( (*p) == 58 )
		goto st272;
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st271;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st271;
	} else
		goto st271;
	goto st0;
st271:
	if ( ++p == pe )
		goto _test_eof271;
case 271:
	if ( (*p) == 58 )
		goto st272;
	goto st0;
st272:
	if ( ++p == pe )
		goto _test_eof272;
case 272:
	if ( (*p) == 58 )
		goto st360;
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st273;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st273;
	} else
		goto st273;
	goto st0;
st273:
	if ( ++p == pe )
		goto _test_eof273;
case 273:
	if ( (*p) == 58 )
		goto st277;
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st274;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st274;
	} else
		goto st274;
	goto st0;
st274:
	if ( ++p == pe )
		goto _test_eof274;
case 274:
	if ( (*p) == 58 )
		goto st277;
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st275;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st275;
	} else
		goto st275;
	goto st0;
st275:
	if ( ++p == pe )
		goto _test_eof275;
case 275:
	if ( (*p) == 58 )
		goto st277;
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st276;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st276;
	} else
		goto st276;
	goto st0;
st276:
	if ( ++p == pe )
		goto _test_eof276;
case 276:
	if ( (*p) == 58 )
		goto st277;
	goto st0;
st277:
	if ( ++p == pe )
		goto _test_eof277;
case 277:
	if ( (*p) == 58 )
		goto st355;
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st278;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st278;
	} else
		goto st278;
	goto st0;
st278:
	if ( ++p == pe )
		goto _test_eof278;
case 278:
	if ( (*p) == 58 )
		goto st282;
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st279;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st279;
	} else
		goto st279;
	goto st0;
st279:
	if ( ++p == pe )
		goto _test_eof279;
case 279:
	if ( (*p) == 58 )
		goto st282;
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st280;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st280;
	} else
		goto st280;
	goto st0;
st280:
	if ( ++p == pe )
		goto _test_eof280;
case 280:
	if ( (*p) == 58 )
		goto st282;
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st281;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st281;
	} else
		goto st281;
	goto st0;
st281:
	if ( ++p == pe )
		goto _test_eof281;
case 281:
	if ( (*p) == 58 )
		goto st282;
	goto st0;
st282:
	if ( ++p == pe )
		goto _test_eof282;
case 282:
	if ( (*p) == 58 )
		goto st350;
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st283;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st283;
	} else
		goto st283;
	goto st0;
st283:
	if ( ++p == pe )
		goto _test_eof283;
case 283:
	if ( (*p) == 58 )
		goto st287;
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st284;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st284;
	} else
		goto st284;
	goto st0;
st284:
	if ( ++p == pe )
		goto _test_eof284;
case 284:
	if ( (*p) == 58 )
		goto st287;
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st285;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st285;
	} else
		goto st285;
	goto st0;
st285:
	if ( ++p == pe )
		goto _test_eof285;
case 285:
	if ( (*p) == 58 )
		goto st287;
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st286;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st286;
	} else
		goto st286;
	goto st0;
st286:
	if ( ++p == pe )
		goto _test_eof286;
case 286:
	if ( (*p) == 58 )
		goto st287;
	goto st0;
st287:
	if ( ++p == pe )
		goto _test_eof287;
case 287:
	if ( (*p) == 58 )
		goto st337;
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st288;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st288;
	} else
		goto st288;
	goto st0;
st288:
	if ( ++p == pe )
		goto _test_eof288;
case 288:
	if ( (*p) == 58 )
		goto st292;
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st289;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st289;
	} else
		goto st289;
	goto st0;
st289:
	if ( ++p == pe )
		goto _test_eof289;
case 289:
	if ( (*p) == 58 )
		goto st292;
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st290;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st290;
	} else
		goto st290;
	goto st0;
st290:
	if ( ++p == pe )
		goto _test_eof290;
case 290:
	if ( (*p) == 58 )
		goto st292;
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st291;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st291;
	} else
		goto st291;
	goto st0;
st291:
	if ( ++p == pe )
		goto _test_eof291;
case 291:
	if ( (*p) == 58 )
		goto st292;
	goto st0;
st292:
	if ( ++p == pe )
		goto _test_eof292;
case 292:
	switch( (*p) ) {
		case 2: goto st293;
		case 32: goto st294;
		case 49: goto st328;
		case 50: goto st331;
		case 57: goto st333;
		case 58: goto st335;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 56 )
			goto st320;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st336;
	} else
		goto st336;
	goto st0;
st293:
	if ( ++p == pe )
		goto _test_eof293;
case 293:
	switch( (*p) ) {
		case 32: goto st294;
		case 48: goto st294;
		case 52: goto st294;
	}
	goto st0;
st294:
	if ( ++p == pe )
		goto _test_eof294;
case 294:
	if ( 48 <= (*p) && (*p) <= 57 )
		goto st295;
	goto st0;
st295:
	if ( ++p == pe )
		goto _test_eof295;
case 295:
	if ( (*p) == 46 )
		goto st296;
	goto st0;
st296:
	if ( ++p == pe )
		goto _test_eof296;
case 296:
	switch( (*p) ) {
		case 2: goto st297;
		case 32: goto st298;
		case 49: goto st316;
		case 50: goto st318;
		case 57: goto st317;
	}
	if ( 48 <= (*p) && (*p) <= 56 )
		goto st299;
	goto st0;
st297:
	if ( ++p == pe )
		goto _test_eof297;
case 297:
	switch( (*p) ) {
		case 32: goto st298;
		case 48: goto st298;
		case 52: goto st298;
	}
	goto st0;
st298:
	if ( ++p == pe )
		goto _test_eof298;
case 298:
	if ( 48 <= (*p) && (*p) <= 57 )
		goto st299;
	goto st0;
st299:
	if ( ++p == pe )
		goto _test_eof299;
case 299:
	if ( (*p) == 46 )
		goto st300;
	goto st0;
st300:
	if ( ++p == pe )
		goto _test_eof300;
case 300:
	switch( (*p) ) {
		case 2: goto st301;
		case 32: goto st302;
		case 49: goto st312;
		case 50: goto st314;
		case 57: goto st313;
	}
	if ( 48 <= (*p) && (*p) <= 56 )
		goto st303;
	goto st0;
st301:
	if ( ++p == pe )
		goto _test_eof301;
case 301:
	switch( (*p) ) {
		case 32: goto st302;
		case 48: goto st302;
		case 52: goto st302;
	}
	goto st0;
st302:
	if ( ++p == pe )
		goto _test_eof302;
case 302:
	if ( 48 <= (*p) && (*p) <= 57 )
		goto st303;
	goto st0;
st303:
	if ( ++p == pe )
		goto _test_eof303;
case 303:
	if ( (*p) == 46 )
		goto st304;
	goto st0;
st304:
	if ( ++p == pe )
		goto _test_eof304;
case 304:
	switch( (*p) ) {
		case 2: goto st305;
		case 32: goto st306;
		case 49: goto st308;
		case 50: goto st310;
		case 57: goto st309;
	}
	if ( 48 <= (*p) && (*p) <= 56 )
		goto st307;
	goto st0;
st305:
	if ( ++p == pe )
		goto _test_eof305;
case 305:
	switch( (*p) ) {
		case 32: goto st306;
		case 48: goto st306;
		case 52: goto st306;
	}
	goto st0;
st306:
	if ( ++p == pe )
		goto _test_eof306;
case 306:
	if ( 48 <= (*p) && (*p) <= 57 )
		goto st307;
	goto st0;
st307:
	if ( ++p == pe )
		goto _test_eof307;
case 307:
	if ( (*p) == 93 )
		goto st517;
	goto st0;
st308:
	if ( ++p == pe )
		goto _test_eof308;
case 308:
	if ( (*p) == 93 )
		goto st517;
	if ( 48 <= (*p) && (*p) <= 57 )
		goto st309;
	goto st0;
st309:
	if ( ++p == pe )
		goto _test_eof309;
case 309:
	if ( (*p) == 93 )
		goto st517;
	if ( 48 <= (*p) && (*p) <= 57 )
		goto st307;
	goto st0;
st310:
	if ( ++p == pe )
		goto _test_eof310;
case 310:
	switch( (*p) ) {
		case 53: goto st311;
		case 93: goto st517;
	}
	goto st0;
st311:
	if ( ++p == pe )
		goto _test_eof311;
case 311:
	switch( (*p) ) {
		case 32: goto st307;
		case 48: goto st307;
		case 53: goto st307;
	}
	goto st0;
st312:
	if ( ++p == pe )
		goto _test_eof312;
case 312:
	if ( (*p) == 46 )
		goto st304;
	if ( 48 <= (*p) && (*p) <= 57 )
		goto st313;
	goto st0;
st313:
	if ( ++p == pe )
		goto _test_eof313;
case 313:
	if ( (*p) == 46 )
		goto st304;
	if ( 48 <= (*p) && (*p) <= 57 )
		goto st303;
	goto st0;
st314:
	if ( ++p == pe )
		goto _test_eof314;
case 314:
	switch( (*p) ) {
		case 46: goto st304;
		case 53: goto st315;
	}
	goto st0;
st315:
	if ( ++p == pe )
		goto _test_eof315;
case 315:
	switch( (*p) ) {
		case 32: goto st303;
		case 48: goto st303;
		case 53: goto st303;
	}
	goto st0;
st316:
	if ( ++p == pe )
		goto _test_eof316;
case 316:
	if ( (*p) == 46 )
		goto st300;
	if ( 48 <= (*p) && (*p) <= 57 )
		goto st317;
	goto st0;
st317:
	if ( ++p == pe )
		goto _test_eof317;
case 317:
	if ( (*p) == 46 )
		goto st300;
	if ( 48 <= (*p) && (*p) <= 57 )
		goto st299;
	goto st0;
st318:
	if ( ++p == pe )
		goto _test_eof318;
case 318:
	switch( (*p) ) {
		case 46: goto st300;
		case 53: goto st319;
	}
	goto st0;
st319:
	if ( ++p == pe )
		goto _test_eof319;
case 319:
	switch( (*p) ) {
		case 32: goto st299;
		case 48: goto st299;
		case 53: goto st299;
	}
	goto st0;
st320:
	if ( ++p == pe )
		goto _test_eof320;
case 320:
	switch( (*p) ) {
		case 46: goto st296;
		case 58: goto st324;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st321;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st321;
	} else
		goto st321;
	goto st0;
st321:
	if ( ++p == pe )
		goto _test_eof321;
case 321:
	if ( (*p) == 58 )
		goto st324;
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st322;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st322;
	} else
		goto st322;
	goto st0;
st322:
	if ( ++p == pe )
		goto _test_eof322;
case 322:
	if ( (*p) == 58 )
		goto st324;
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st323;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st323;
	} else
		goto st323;
	goto st0;
st323:
	if ( ++p == pe )
		goto _test_eof323;
case 323:
	if ( (*p) == 58 )
		goto st324;
	goto st0;
st324:
	if ( ++p == pe )
		goto _test_eof324;
case 324:
	if ( (*p) == 58 )
		goto st307;
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st325;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st325;
	} else
		goto st325;
	goto st0;
st325:
	if ( ++p == pe )
		goto _test_eof325;
case 325:
	if ( (*p) == 93 )
		goto st517;
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st326;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st326;
	} else
		goto st326;
	goto st0;
st326:
	if ( ++p == pe )
		goto _test_eof326;
case 326:
	if ( (*p) == 93 )
		goto st517;
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st327;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st327;
	} else
		goto st327;
	goto st0;
st327:
	if ( ++p == pe )
		goto _test_eof327;
case 327:
	if ( (*p) == 93 )
		goto st517;
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st307;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st307;
	} else
		goto st307;
	goto st0;
st328:
	if ( ++p == pe )
		goto _test_eof328;
case 328:
	switch( (*p) ) {
		case 46: goto st296;
		case 58: goto st324;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st329;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st321;
	} else
		goto st321;
	goto st0;
st329:
	if ( ++p == pe )
		goto _test_eof329;
case 329:
	switch( (*p) ) {
		case 46: goto st296;
		case 58: goto st324;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st330;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st322;
	} else
		goto st322;
	goto st0;
st330:
	if ( ++p == pe )
		goto _test_eof330;
case 330:
	switch( (*p) ) {
		case 46: goto st296;
		case 58: goto st324;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st323;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st323;
	} else
		goto st323;
	goto st0;
st331:
	if ( ++p == pe )
		goto _test_eof331;
case 331:
	switch( (*p) ) {
		case 46: goto st296;
		case 53: goto st332;
		case 58: goto st324;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st321;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st321;
	} else
		goto st321;
	goto st0;
st332:
	if ( ++p == pe )
		goto _test_eof332;
case 332:
	switch( (*p) ) {
		case 32: goto st295;
		case 48: goto st330;
		case 53: goto st330;
		case 58: goto st324;
	}
	if ( (*p) < 65 ) {
		if ( 49 <= (*p) && (*p) <= 57 )
			goto st322;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st322;
	} else
		goto st322;
	goto st0;
st333:
	if ( ++p == pe )
		goto _test_eof333;
case 333:
	switch( (*p) ) {
		case 46: goto st296;
		case 58: goto st324;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st334;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st321;
	} else
		goto st321;
	goto st0;
st334:
	if ( ++p == pe )
		goto _test_eof334;
case 334:
	switch( (*p) ) {
		case 46: goto st296;
		case 58: goto st324;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st322;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st322;
	} else
		goto st322;
	goto st0;
st335:
	if ( ++p == pe )
		goto _test_eof335;
case 335:
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st325;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st325;
	} else
		goto st325;
	goto st0;
st336:
	if ( ++p == pe )
		goto _test_eof336;
case 336:
	if ( (*p) == 58 )
		goto st324;
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st321;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st321;
	} else
		goto st321;
	goto st0;
st337:
	if ( ++p == pe )
		goto _test_eof337;
case 337:
	switch( (*p) ) {
		case 2: goto st293;
		case 32: goto st294;
		case 49: goto st342;
		case 50: goto st345;
		case 57: goto st347;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 56 )
			goto st338;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st349;
	} else
		goto st349;
	goto st0;
st338:
	if ( ++p == pe )
		goto _test_eof338;
case 338:
	switch( (*p) ) {
		case 46: goto st296;
		case 58: goto st335;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st339;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st339;
	} else
		goto st339;
	goto st0;
st339:
	if ( ++p == pe )
		goto _test_eof339;
case 339:
	if ( (*p) == 58 )
		goto st335;
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st340;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st340;
	} else
		goto st340;
	goto st0;
st340:
	if ( ++p == pe )
		goto _test_eof340;
case 340:
	if ( (*p) == 58 )
		goto st335;
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st341;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st341;
	} else
		goto st341;
	goto st0;
st341:
	if ( ++p == pe )
		goto _test_eof341;
case 341:
	if ( (*p) == 58 )
		goto st335;
	goto st0;
st342:
	if ( ++p == pe )
		goto _test_eof342;
case 342:
	switch( (*p) ) {
		case 46: goto st296;
		case 58: goto st335;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st343;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st339;
	} else
		goto st339;
	goto st0;
st343:
	if ( ++p == pe )
		goto _test_eof343;
case 343:
	switch( (*p) ) {
		case 46: goto st296;
		case 58: goto st335;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st344;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st340;
	} else
		goto st340;
	goto st0;
st344:
	if ( ++p == pe )
		goto _test_eof344;
case 344:
	switch( (*p) ) {
		case 46: goto st296;
		case 58: goto st335;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st341;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st341;
	} else
		goto st341;
	goto st0;
st345:
	if ( ++p == pe )
		goto _test_eof345;
case 345:
	switch( (*p) ) {
		case 46: goto st296;
		case 53: goto st346;
		case 58: goto st335;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st339;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st339;
	} else
		goto st339;
	goto st0;
st346:
	if ( ++p == pe )
		goto _test_eof346;
case 346:
	switch( (*p) ) {
		case 32: goto st295;
		case 48: goto st344;
		case 53: goto st344;
		case 58: goto st335;
	}
	if ( (*p) < 65 ) {
		if ( 49 <= (*p) && (*p) <= 57 )
			goto st340;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st340;
	} else
		goto st340;
	goto st0;
st347:
	if ( ++p == pe )
		goto _test_eof347;
case 347:
	switch( (*p) ) {
		case 46: goto st296;
		case 58: goto st335;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st348;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st339;
	} else
		goto st339;
	goto st0;
st348:
	if ( ++p == pe )
		goto _test_eof348;
case 348:
	switch( (*p) ) {
		case 46: goto st296;
		case 58: goto st335;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st340;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st340;
	} else
		goto st340;
	goto st0;
st349:
	if ( ++p == pe )
		goto _test_eof349;
case 349:
	if ( (*p) == 58 )
		goto st335;
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st339;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st339;
	} else
		goto st339;
	goto st0;
st350:
	if ( ++p == pe )
		goto _test_eof350;
case 350:
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st351;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st351;
	} else
		goto st351;
	goto st0;
st351:
	if ( ++p == pe )
		goto _test_eof351;
case 351:
	if ( (*p) == 58 )
		goto st337;
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st352;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st352;
	} else
		goto st352;
	goto st0;
st352:
	if ( ++p == pe )
		goto _test_eof352;
case 352:
	if ( (*p) == 58 )
		goto st337;
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st353;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st353;
	} else
		goto st353;
	goto st0;
st353:
	if ( ++p == pe )
		goto _test_eof353;
case 353:
	if ( (*p) == 58 )
		goto st337;
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st354;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st354;
	} else
		goto st354;
	goto st0;
st354:
	if ( ++p == pe )
		goto _test_eof354;
case 354:
	if ( (*p) == 58 )
		goto st337;
	goto st0;
st355:
	if ( ++p == pe )
		goto _test_eof355;
case 355:
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st356;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st356;
	} else
		goto st356;
	goto st0;
st356:
	if ( ++p == pe )
		goto _test_eof356;
case 356:
	if ( (*p) == 58 )
		goto st350;
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st357;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st357;
	} else
		goto st357;
	goto st0;
st357:
	if ( ++p == pe )
		goto _test_eof357;
case 357:
	if ( (*p) == 58 )
		goto st350;
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st358;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st358;
	} else
		goto st358;
	goto st0;
st358:
	if ( ++p == pe )
		goto _test_eof358;
case 358:
	if ( (*p) == 58 )
		goto st350;
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st359;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st359;
	} else
		goto st359;
	goto st0;
st359:
	if ( ++p == pe )
		goto _test_eof359;
case 359:
	if ( (*p) == 58 )
		goto st350;
	goto st0;
st360:
	if ( ++p == pe )
		goto _test_eof360;
case 360:
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st361;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st361;
	} else
		goto st361;
	goto st0;
st361:
	if ( ++p == pe )
		goto _test_eof361;
case 361:
	if ( (*p) == 58 )
		goto st355;
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st362;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st362;
	} else
		goto st362;
	goto st0;
st362:
	if ( ++p == pe )
		goto _test_eof362;
case 362:
	if ( (*p) == 58 )
		goto st355;
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st363;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st363;
	} else
		goto st363;
	goto st0;
st363:
	if ( ++p == pe )
		goto _test_eof363;
case 363:
	if ( (*p) == 58 )
		goto st355;
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st364;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st364;
	} else
		goto st364;
	goto st0;
st364:
	if ( ++p == pe )
		goto _test_eof364;
case 364:
	if ( (*p) == 58 )
		goto st355;
	goto st0;
st365:
	if ( ++p == pe )
		goto _test_eof365;
case 365:
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st366;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st366;
	} else
		goto st366;
	goto st0;
st366:
	if ( ++p == pe )
		goto _test_eof366;
case 366:
	if ( (*p) == 58 )
		goto st360;
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st367;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st367;
	} else
		goto st367;
	goto st0;
st367:
	if ( ++p == pe )
		goto _test_eof367;
case 367:
	if ( (*p) == 58 )
		goto st360;
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st368;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st368;
	} else
		goto st368;
	goto st0;
st368:
	if ( ++p == pe )
		goto _test_eof368;
case 368:
	if ( (*p) == 58 )
		goto st360;
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st369;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st369;
	} else
		goto st369;
	goto st0;
st369:
	if ( ++p == pe )
		goto _test_eof369;
case 369:
	if ( (*p) == 58 )
		goto st360;
	goto st0;
st370:
	if ( ++p == pe )
		goto _test_eof370;
case 370:
	if ( (*p) == 58 )
		goto st371;
	goto st0;
st371:
	if ( ++p == pe )
		goto _test_eof371;
case 371:
	switch( (*p) ) {
		case 2: goto st293;
		case 32: goto st294;
		case 49: goto st441;
		case 50: goto st444;
		case 57: goto st446;
		case 93: goto st517;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 56 )
			goto st372;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st448;
	} else
		goto st448;
	goto st0;
st372:
	if ( ++p == pe )
		goto _test_eof372;
case 372:
	switch( (*p) ) {
		case 46: goto st296;
		case 58: goto st376;
		case 93: goto st517;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st373;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st373;
	} else
		goto st373;
	goto st0;
st373:
	if ( ++p == pe )
		goto _test_eof373;
case 373:
	switch( (*p) ) {
		case 58: goto st376;
		case 93: goto st517;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st374;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st374;
	} else
		goto st374;
	goto st0;
st374:
	if ( ++p == pe )
		goto _test_eof374;
case 374:
	switch( (*p) ) {
		case 58: goto st376;
		case 93: goto st517;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st375;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st375;
	} else
		goto st375;
	goto st0;
st375:
	if ( ++p == pe )
		goto _test_eof375;
case 375:
	switch( (*p) ) {
		case 58: goto st376;
		case 93: goto st517;
	}
	goto st0;
st376:
	if ( ++p == pe )
		goto _test_eof376;
case 376:
	switch( (*p) ) {
		case 2: goto st293;
		case 32: goto st294;
		case 49: goto st433;
		case 50: goto st436;
		case 57: goto st438;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 56 )
			goto st377;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st440;
	} else
		goto st440;
	goto st0;
st377:
	if ( ++p == pe )
		goto _test_eof377;
case 377:
	switch( (*p) ) {
		case 46: goto st296;
		case 58: goto st381;
		case 93: goto st517;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st378;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st378;
	} else
		goto st378;
	goto st0;
st378:
	if ( ++p == pe )
		goto _test_eof378;
case 378:
	switch( (*p) ) {
		case 58: goto st381;
		case 93: goto st517;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st379;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st379;
	} else
		goto st379;
	goto st0;
st379:
	if ( ++p == pe )
		goto _test_eof379;
case 379:
	switch( (*p) ) {
		case 58: goto st381;
		case 93: goto st517;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st380;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st380;
	} else
		goto st380;
	goto st0;
st380:
	if ( ++p == pe )
		goto _test_eof380;
case 380:
	switch( (*p) ) {
		case 58: goto st381;
		case 93: goto st517;
	}
	goto st0;
st381:
	if ( ++p == pe )
		goto _test_eof381;
case 381:
	switch( (*p) ) {
		case 2: goto st293;
		case 32: goto st294;
		case 49: goto st425;
		case 50: goto st428;
		case 57: goto st430;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 56 )
			goto st382;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st432;
	} else
		goto st432;
	goto st0;
st382:
	if ( ++p == pe )
		goto _test_eof382;
case 382:
	switch( (*p) ) {
		case 46: goto st296;
		case 58: goto st386;
		case 93: goto st517;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st383;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st383;
	} else
		goto st383;
	goto st0;
st383:
	if ( ++p == pe )
		goto _test_eof383;
case 383:
	switch( (*p) ) {
		case 58: goto st386;
		case 93: goto st517;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st384;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st384;
	} else
		goto st384;
	goto st0;
st384:
	if ( ++p == pe )
		goto _test_eof384;
case 384:
	switch( (*p) ) {
		case 58: goto st386;
		case 93: goto st517;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st385;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st385;
	} else
		goto st385;
	goto st0;
st385:
	if ( ++p == pe )
		goto _test_eof385;
case 385:
	switch( (*p) ) {
		case 58: goto st386;
		case 93: goto st517;
	}
	goto st0;
st386:
	if ( ++p == pe )
		goto _test_eof386;
case 386:
	switch( (*p) ) {
		case 2: goto st293;
		case 32: goto st294;
		case 49: goto st417;
		case 50: goto st420;
		case 57: goto st422;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 56 )
			goto st387;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st424;
	} else
		goto st424;
	goto st0;
st387:
	if ( ++p == pe )
		goto _test_eof387;
case 387:
	switch( (*p) ) {
		case 46: goto st296;
		case 58: goto st391;
		case 93: goto st517;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st388;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st388;
	} else
		goto st388;
	goto st0;
st388:
	if ( ++p == pe )
		goto _test_eof388;
case 388:
	switch( (*p) ) {
		case 58: goto st391;
		case 93: goto st517;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st389;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st389;
	} else
		goto st389;
	goto st0;
st389:
	if ( ++p == pe )
		goto _test_eof389;
case 389:
	switch( (*p) ) {
		case 58: goto st391;
		case 93: goto st517;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st390;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st390;
	} else
		goto st390;
	goto st0;
st390:
	if ( ++p == pe )
		goto _test_eof390;
case 390:
	switch( (*p) ) {
		case 58: goto st391;
		case 93: goto st517;
	}
	goto st0;
st391:
	if ( ++p == pe )
		goto _test_eof391;
case 391:
	switch( (*p) ) {
		case 2: goto st293;
		case 32: goto st294;
		case 49: goto st409;
		case 50: goto st412;
		case 57: goto st414;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 56 )
			goto st392;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st416;
	} else
		goto st416;
	goto st0;
st392:
	if ( ++p == pe )
		goto _test_eof392;
case 392:
	switch( (*p) ) {
		case 46: goto st296;
		case 58: goto st396;
		case 93: goto st517;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st393;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st393;
	} else
		goto st393;
	goto st0;
st393:
	if ( ++p == pe )
		goto _test_eof393;
case 393:
	switch( (*p) ) {
		case 58: goto st396;
		case 93: goto st517;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st394;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st394;
	} else
		goto st394;
	goto st0;
st394:
	if ( ++p == pe )
		goto _test_eof394;
case 394:
	switch( (*p) ) {
		case 58: goto st396;
		case 93: goto st517;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st395;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st395;
	} else
		goto st395;
	goto st0;
st395:
	if ( ++p == pe )
		goto _test_eof395;
case 395:
	switch( (*p) ) {
		case 58: goto st396;
		case 93: goto st517;
	}
	goto st0;
st396:
	if ( ++p == pe )
		goto _test_eof396;
case 396:
	switch( (*p) ) {
		case 2: goto st293;
		case 32: goto st294;
		case 49: goto st401;
		case 50: goto st404;
		case 57: goto st406;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 56 )
			goto st397;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st408;
	} else
		goto st408;
	goto st0;
st397:
	if ( ++p == pe )
		goto _test_eof397;
case 397:
	switch( (*p) ) {
		case 46: goto st296;
		case 58: goto st335;
		case 93: goto st517;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st398;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st398;
	} else
		goto st398;
	goto st0;
st398:
	if ( ++p == pe )
		goto _test_eof398;
case 398:
	switch( (*p) ) {
		case 58: goto st335;
		case 93: goto st517;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st399;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st399;
	} else
		goto st399;
	goto st0;
st399:
	if ( ++p == pe )
		goto _test_eof399;
case 399:
	switch( (*p) ) {
		case 58: goto st335;
		case 93: goto st517;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st400;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st400;
	} else
		goto st400;
	goto st0;
st400:
	if ( ++p == pe )
		goto _test_eof400;
case 400:
	switch( (*p) ) {
		case 58: goto st335;
		case 93: goto st517;
	}
	goto st0;
st401:
	if ( ++p == pe )
		goto _test_eof401;
case 401:
	switch( (*p) ) {
		case 46: goto st296;
		case 58: goto st335;
		case 93: goto st517;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st402;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st398;
	} else
		goto st398;
	goto st0;
st402:
	if ( ++p == pe )
		goto _test_eof402;
case 402:
	switch( (*p) ) {
		case 46: goto st296;
		case 58: goto st335;
		case 93: goto st517;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st403;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st399;
	} else
		goto st399;
	goto st0;
st403:
	if ( ++p == pe )
		goto _test_eof403;
case 403:
	switch( (*p) ) {
		case 46: goto st296;
		case 58: goto st335;
		case 93: goto st517;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st400;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st400;
	} else
		goto st400;
	goto st0;
st404:
	if ( ++p == pe )
		goto _test_eof404;
case 404:
	switch( (*p) ) {
		case 46: goto st296;
		case 53: goto st405;
		case 58: goto st335;
		case 93: goto st517;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st398;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st398;
	} else
		goto st398;
	goto st0;
st405:
	if ( ++p == pe )
		goto _test_eof405;
case 405:
	switch( (*p) ) {
		case 32: goto st295;
		case 48: goto st403;
		case 53: goto st403;
		case 58: goto st335;
		case 93: goto st517;
	}
	if ( (*p) < 65 ) {
		if ( 49 <= (*p) && (*p) <= 57 )
			goto st399;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st399;
	} else
		goto st399;
	goto st0;
st406:
	if ( ++p == pe )
		goto _test_eof406;
case 406:
	switch( (*p) ) {
		case 46: goto st296;
		case 58: goto st335;
		case 93: goto st517;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st407;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st398;
	} else
		goto st398;
	goto st0;
st407:
	if ( ++p == pe )
		goto _test_eof407;
case 407:
	switch( (*p) ) {
		case 46: goto st296;
		case 58: goto st335;
		case 93: goto st517;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st399;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st399;
	} else
		goto st399;
	goto st0;
st408:
	if ( ++p == pe )
		goto _test_eof408;
case 408:
	switch( (*p) ) {
		case 58: goto st335;
		case 93: goto st517;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st398;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st398;
	} else
		goto st398;
	goto st0;
st409:
	if ( ++p == pe )
		goto _test_eof409;
case 409:
	switch( (*p) ) {
		case 46: goto st296;
		case 58: goto st396;
		case 93: goto st517;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st410;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st393;
	} else
		goto st393;
	goto st0;
st410:
	if ( ++p == pe )
		goto _test_eof410;
case 410:
	switch( (*p) ) {
		case 46: goto st296;
		case 58: goto st396;
		case 93: goto st517;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st411;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st394;
	} else
		goto st394;
	goto st0;
st411:
	if ( ++p == pe )
		goto _test_eof411;
case 411:
	switch( (*p) ) {
		case 46: goto st296;
		case 58: goto st396;
		case 93: goto st517;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st395;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st395;
	} else
		goto st395;
	goto st0;
st412:
	if ( ++p == pe )
		goto _test_eof412;
case 412:
	switch( (*p) ) {
		case 46: goto st296;
		case 53: goto st413;
		case 58: goto st396;
		case 93: goto st517;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st393;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st393;
	} else
		goto st393;
	goto st0;
st413:
	if ( ++p == pe )
		goto _test_eof413;
case 413:
	switch( (*p) ) {
		case 32: goto st295;
		case 48: goto st411;
		case 53: goto st411;
		case 58: goto st396;
		case 93: goto st517;
	}
	if ( (*p) < 65 ) {
		if ( 49 <= (*p) && (*p) <= 57 )
			goto st394;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st394;
	} else
		goto st394;
	goto st0;
st414:
	if ( ++p == pe )
		goto _test_eof414;
case 414:
	switch( (*p) ) {
		case 46: goto st296;
		case 58: goto st396;
		case 93: goto st517;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st415;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st393;
	} else
		goto st393;
	goto st0;
st415:
	if ( ++p == pe )
		goto _test_eof415;
case 415:
	switch( (*p) ) {
		case 46: goto st296;
		case 58: goto st396;
		case 93: goto st517;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st394;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st394;
	} else
		goto st394;
	goto st0;
st416:
	if ( ++p == pe )
		goto _test_eof416;
case 416:
	switch( (*p) ) {
		case 58: goto st396;
		case 93: goto st517;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st393;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st393;
	} else
		goto st393;
	goto st0;
st417:
	if ( ++p == pe )
		goto _test_eof417;
case 417:
	switch( (*p) ) {
		case 46: goto st296;
		case 58: goto st391;
		case 93: goto st517;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st418;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st388;
	} else
		goto st388;
	goto st0;
st418:
	if ( ++p == pe )
		goto _test_eof418;
case 418:
	switch( (*p) ) {
		case 46: goto st296;
		case 58: goto st391;
		case 93: goto st517;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st419;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st389;
	} else
		goto st389;
	goto st0;
st419:
	if ( ++p == pe )
		goto _test_eof419;
case 419:
	switch( (*p) ) {
		case 46: goto st296;
		case 58: goto st391;
		case 93: goto st517;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st390;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st390;
	} else
		goto st390;
	goto st0;
st420:
	if ( ++p == pe )
		goto _test_eof420;
case 420:
	switch( (*p) ) {
		case 46: goto st296;
		case 53: goto st421;
		case 58: goto st391;
		case 93: goto st517;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st388;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st388;
	} else
		goto st388;
	goto st0;
st421:
	if ( ++p == pe )
		goto _test_eof421;
case 421:
	switch( (*p) ) {
		case 32: goto st295;
		case 48: goto st419;
		case 53: goto st419;
		case 58: goto st391;
		case 93: goto st517;
	}
	if ( (*p) < 65 ) {
		if ( 49 <= (*p) && (*p) <= 57 )
			goto st389;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st389;
	} else
		goto st389;
	goto st0;
st422:
	if ( ++p == pe )
		goto _test_eof422;
case 422:
	switch( (*p) ) {
		case 46: goto st296;
		case 58: goto st391;
		case 93: goto st517;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st423;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st388;
	} else
		goto st388;
	goto st0;
st423:
	if ( ++p == pe )
		goto _test_eof423;
case 423:
	switch( (*p) ) {
		case 46: goto st296;
		case 58: goto st391;
		case 93: goto st517;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st389;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st389;
	} else
		goto st389;
	goto st0;
st424:
	if ( ++p == pe )
		goto _test_eof424;
case 424:
	switch( (*p) ) {
		case 58: goto st391;
		case 93: goto st517;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st388;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st388;
	} else
		goto st388;
	goto st0;
st425:
	if ( ++p == pe )
		goto _test_eof425;
case 425:
	switch( (*p) ) {
		case 46: goto st296;
		case 58: goto st386;
		case 93: goto st517;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st426;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st383;
	} else
		goto st383;
	goto st0;
st426:
	if ( ++p == pe )
		goto _test_eof426;
case 426:
	switch( (*p) ) {
		case 46: goto st296;
		case 58: goto st386;
		case 93: goto st517;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st427;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st384;
	} else
		goto st384;
	goto st0;
st427:
	if ( ++p == pe )
		goto _test_eof427;
case 427:
	switch( (*p) ) {
		case 46: goto st296;
		case 58: goto st386;
		case 93: goto st517;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st385;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st385;
	} else
		goto st385;
	goto st0;
st428:
	if ( ++p == pe )
		goto _test_eof428;
case 428:
	switch( (*p) ) {
		case 46: goto st296;
		case 53: goto st429;
		case 58: goto st386;
		case 93: goto st517;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st383;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st383;
	} else
		goto st383;
	goto st0;
st429:
	if ( ++p == pe )
		goto _test_eof429;
case 429:
	switch( (*p) ) {
		case 32: goto st295;
		case 48: goto st427;
		case 53: goto st427;
		case 58: goto st386;
		case 93: goto st517;
	}
	if ( (*p) < 65 ) {
		if ( 49 <= (*p) && (*p) <= 57 )
			goto st384;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st384;
	} else
		goto st384;
	goto st0;
st430:
	if ( ++p == pe )
		goto _test_eof430;
case 430:
	switch( (*p) ) {
		case 46: goto st296;
		case 58: goto st386;
		case 93: goto st517;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st431;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st383;
	} else
		goto st383;
	goto st0;
st431:
	if ( ++p == pe )
		goto _test_eof431;
case 431:
	switch( (*p) ) {
		case 46: goto st296;
		case 58: goto st386;
		case 93: goto st517;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st384;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st384;
	} else
		goto st384;
	goto st0;
st432:
	if ( ++p == pe )
		goto _test_eof432;
case 432:
	switch( (*p) ) {
		case 58: goto st386;
		case 93: goto st517;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st383;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st383;
	} else
		goto st383;
	goto st0;
st433:
	if ( ++p == pe )
		goto _test_eof433;
case 433:
	switch( (*p) ) {
		case 46: goto st296;
		case 58: goto st381;
		case 93: goto st517;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st434;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st378;
	} else
		goto st378;
	goto st0;
st434:
	if ( ++p == pe )
		goto _test_eof434;
case 434:
	switch( (*p) ) {
		case 46: goto st296;
		case 58: goto st381;
		case 93: goto st517;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st435;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st379;
	} else
		goto st379;
	goto st0;
st435:
	if ( ++p == pe )
		goto _test_eof435;
case 435:
	switch( (*p) ) {
		case 46: goto st296;
		case 58: goto st381;
		case 93: goto st517;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st380;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st380;
	} else
		goto st380;
	goto st0;
st436:
	if ( ++p == pe )
		goto _test_eof436;
case 436:
	switch( (*p) ) {
		case 46: goto st296;
		case 53: goto st437;
		case 58: goto st381;
		case 93: goto st517;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st378;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st378;
	} else
		goto st378;
	goto st0;
st437:
	if ( ++p == pe )
		goto _test_eof437;
case 437:
	switch( (*p) ) {
		case 32: goto st295;
		case 48: goto st435;
		case 53: goto st435;
		case 58: goto st381;
		case 93: goto st517;
	}
	if ( (*p) < 65 ) {
		if ( 49 <= (*p) && (*p) <= 57 )
			goto st379;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st379;
	} else
		goto st379;
	goto st0;
st438:
	if ( ++p == pe )
		goto _test_eof438;
case 438:
	switch( (*p) ) {
		case 46: goto st296;
		case 58: goto st381;
		case 93: goto st517;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st439;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st378;
	} else
		goto st378;
	goto st0;
st439:
	if ( ++p == pe )
		goto _test_eof439;
case 439:
	switch( (*p) ) {
		case 46: goto st296;
		case 58: goto st381;
		case 93: goto st517;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st379;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st379;
	} else
		goto st379;
	goto st0;
st440:
	if ( ++p == pe )
		goto _test_eof440;
case 440:
	switch( (*p) ) {
		case 58: goto st381;
		case 93: goto st517;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st378;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st378;
	} else
		goto st378;
	goto st0;
st441:
	if ( ++p == pe )
		goto _test_eof441;
case 441:
	switch( (*p) ) {
		case 46: goto st296;
		case 58: goto st376;
		case 93: goto st517;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st442;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st373;
	} else
		goto st373;
	goto st0;
st442:
	if ( ++p == pe )
		goto _test_eof442;
case 442:
	switch( (*p) ) {
		case 46: goto st296;
		case 58: goto st376;
		case 93: goto st517;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st443;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st374;
	} else
		goto st374;
	goto st0;
st443:
	if ( ++p == pe )
		goto _test_eof443;
case 443:
	switch( (*p) ) {
		case 46: goto st296;
		case 58: goto st376;
		case 93: goto st517;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st375;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st375;
	} else
		goto st375;
	goto st0;
st444:
	if ( ++p == pe )
		goto _test_eof444;
case 444:
	switch( (*p) ) {
		case 46: goto st296;
		case 53: goto st445;
		case 58: goto st376;
		case 93: goto st517;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st373;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st373;
	} else
		goto st373;
	goto st0;
st445:
	if ( ++p == pe )
		goto _test_eof445;
case 445:
	switch( (*p) ) {
		case 32: goto st295;
		case 48: goto st443;
		case 53: goto st443;
		case 58: goto st376;
		case 93: goto st517;
	}
	if ( (*p) < 65 ) {
		if ( 49 <= (*p) && (*p) <= 57 )
			goto st374;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st374;
	} else
		goto st374;
	goto st0;
st446:
	if ( ++p == pe )
		goto _test_eof446;
case 446:
	switch( (*p) ) {
		case 46: goto st296;
		case 58: goto st376;
		case 93: goto st517;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st447;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st373;
	} else
		goto st373;
	goto st0;
st447:
	if ( ++p == pe )
		goto _test_eof447;
case 447:
	switch( (*p) ) {
		case 46: goto st296;
		case 58: goto st376;
		case 93: goto st517;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st374;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st374;
	} else
		goto st374;
	goto st0;
st448:
	if ( ++p == pe )
		goto _test_eof448;
case 448:
	switch( (*p) ) {
		case 58: goto st376;
		case 93: goto st517;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st373;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st373;
	} else
		goto st373;
	goto st0;
st449:
	if ( ++p == pe )
		goto _test_eof449;
case 449:
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st450;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st450;
	} else
		goto st450;
	goto st0;
st450:
	if ( ++p == pe )
		goto _test_eof450;
case 450:
	if ( (*p) == 46 )
		goto st451;
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st450;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st450;
	} else
		goto st450;
	goto st0;
st451:
	if ( ++p == pe )
		goto _test_eof451;
case 451:
	switch( (*p) ) {
		case 33: goto st452;
		case 36: goto st452;
		case 61: goto st452;
		case 95: goto st452;
		case 126: goto st452;
	}
	if ( (*p) < 48 ) {
		if ( 38 <= (*p) && (*p) <= 46 )
			goto st452;
	} else if ( (*p) > 59 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto st452;
		} else if ( (*p) >= 65 )
			goto st452;
	} else
		goto st452;
	goto st0;
st452:
	if ( ++p == pe )
		goto _test_eof452;
case 452:
	switch( (*p) ) {
		case 33: goto st452;
		case 36: goto st452;
		case 61: goto st452;
		case 93: goto st517;
		case 95: goto st452;
		case 126: goto st452;
	}
	if ( (*p) < 48 ) {
		if ( 38 <= (*p) && (*p) <= 46 )
			goto st452;
	} else if ( (*p) > 59 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto st452;
		} else if ( (*p) >= 65 )
			goto st452;
	} else
		goto st452;
	goto st0;
tr583:
#line 18 "uri.rl"
	{ mark = p; }
	goto st548;
st548:
	if ( ++p == pe )
		goto _test_eof548;
case 548:
#line 9611 "uri.rl.cc"
	switch( (*p) ) {
		case 33: goto st257;
		case 35: goto tr576;
		case 37: goto st258;
		case 47: goto tr577;
		case 61: goto st257;
		case 63: goto tr579;
		case 64: goto tr263;
		case 95: goto st257;
		case 126: goto st257;
	}
	if ( (*p) < 58 ) {
		if ( (*p) > 46 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto st548;
		} else if ( (*p) >= 36 )
			goto st257;
	} else if ( (*p) > 59 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto st257;
		} else if ( (*p) >= 65 )
			goto st257;
	} else
		goto st257;
	goto st0;
tr557:
#line 18 "uri.rl"
	{ mark = p; }
	goto st549;
st549:
	if ( ++p == pe )
		goto _test_eof549;
case 549:
#line 9646 "uri.rl.cc"
	switch( (*p) ) {
		case 33: goto st524;
		case 35: goto tr565;
		case 37: goto st255;
		case 46: goto st550;
		case 47: goto tr566;
		case 58: goto tr582;
		case 61: goto st524;
		case 63: goto tr568;
		case 64: goto tr263;
		case 95: goto st524;
		case 126: goto st524;
	}
	if ( (*p) < 65 ) {
		if ( 36 <= (*p) && (*p) <= 59 )
			goto st524;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto st524;
	} else
		goto st524;
	goto st0;
st550:
	if ( ++p == pe )
		goto _test_eof550;
case 550:
	switch( (*p) ) {
		case 2: goto st234;
		case 32: goto st235;
		case 33: goto st524;
		case 35: goto tr565;
		case 37: goto st255;
		case 47: goto tr566;
		case 49: goto st561;
		case 50: goto st563;
		case 57: goto st562;
		case 58: goto tr582;
		case 59: goto st524;
		case 61: goto st524;
		case 63: goto tr568;
		case 64: goto tr263;
		case 95: goto st524;
		case 126: goto st524;
	}
	if ( (*p) < 48 ) {
		if ( 36 <= (*p) && (*p) <= 46 )
			goto st524;
	} else if ( (*p) > 56 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto st524;
		} else if ( (*p) >= 65 )
			goto st524;
	} else
		goto st551;
	goto st0;
st551:
	if ( ++p == pe )
		goto _test_eof551;
case 551:
	switch( (*p) ) {
		case 33: goto st524;
		case 35: goto tr565;
		case 37: goto st255;
		case 46: goto st552;
		case 47: goto tr566;
		case 58: goto tr582;
		case 61: goto st524;
		case 63: goto tr568;
		case 64: goto tr263;
		case 95: goto st524;
		case 126: goto st524;
	}
	if ( (*p) < 65 ) {
		if ( 36 <= (*p) && (*p) <= 59 )
			goto st524;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto st524;
	} else
		goto st524;
	goto st0;
st552:
	if ( ++p == pe )
		goto _test_eof552;
case 552:
	switch( (*p) ) {
		case 2: goto st238;
		case 32: goto st239;
		case 33: goto st524;
		case 35: goto tr565;
		case 37: goto st255;
		case 47: goto tr566;
		case 49: goto st557;
		case 50: goto st559;
		case 57: goto st558;
		case 58: goto tr582;
		case 59: goto st524;
		case 61: goto st524;
		case 63: goto tr568;
		case 64: goto tr263;
		case 95: goto st524;
		case 126: goto st524;
	}
	if ( (*p) < 48 ) {
		if ( 36 <= (*p) && (*p) <= 46 )
			goto st524;
	} else if ( (*p) > 56 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto st524;
		} else if ( (*p) >= 65 )
			goto st524;
	} else
		goto st553;
	goto st0;
st553:
	if ( ++p == pe )
		goto _test_eof553;
case 553:
	switch( (*p) ) {
		case 33: goto st524;
		case 35: goto tr565;
		case 37: goto st255;
		case 46: goto st554;
		case 47: goto tr566;
		case 58: goto tr582;
		case 61: goto st524;
		case 63: goto tr568;
		case 64: goto tr263;
		case 95: goto st524;
		case 126: goto st524;
	}
	if ( (*p) < 65 ) {
		if ( 36 <= (*p) && (*p) <= 59 )
			goto st524;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto st524;
	} else
		goto st524;
	goto st0;
st554:
	if ( ++p == pe )
		goto _test_eof554;
case 554:
	switch( (*p) ) {
		case 2: goto st242;
		case 32: goto st243;
		case 33: goto st524;
		case 35: goto tr565;
		case 37: goto st255;
		case 47: goto tr566;
		case 50: goto st555;
		case 58: goto tr582;
		case 61: goto st524;
		case 63: goto tr568;
		case 64: goto tr263;
		case 95: goto st524;
		case 126: goto st524;
	}
	if ( (*p) < 65 ) {
		if ( 36 <= (*p) && (*p) <= 59 )
			goto st524;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto st524;
	} else
		goto st524;
	goto st0;
st555:
	if ( ++p == pe )
		goto _test_eof555;
case 555:
	switch( (*p) ) {
		case 33: goto st524;
		case 35: goto tr565;
		case 37: goto st255;
		case 47: goto tr566;
		case 53: goto st556;
		case 58: goto tr582;
		case 61: goto st524;
		case 63: goto tr568;
		case 64: goto tr263;
		case 95: goto st524;
		case 126: goto st524;
	}
	if ( (*p) < 65 ) {
		if ( 36 <= (*p) && (*p) <= 59 )
			goto st524;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto st524;
	} else
		goto st524;
	goto st0;
st556:
	if ( ++p == pe )
		goto _test_eof556;
case 556:
	switch( (*p) ) {
		case 32: goto st517;
		case 33: goto st524;
		case 35: goto tr565;
		case 37: goto st255;
		case 47: goto tr566;
		case 58: goto tr582;
		case 61: goto st524;
		case 63: goto tr568;
		case 64: goto tr263;
		case 95: goto st524;
		case 126: goto st524;
	}
	if ( (*p) < 65 ) {
		if ( 36 <= (*p) && (*p) <= 59 )
			goto st524;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto st524;
	} else
		goto st524;
	goto st0;
st557:
	if ( ++p == pe )
		goto _test_eof557;
case 557:
	switch( (*p) ) {
		case 33: goto st524;
		case 35: goto tr565;
		case 37: goto st255;
		case 46: goto st554;
		case 47: goto tr566;
		case 58: goto tr582;
		case 59: goto st524;
		case 61: goto st524;
		case 63: goto tr568;
		case 64: goto tr263;
		case 95: goto st524;
		case 126: goto st524;
	}
	if ( (*p) < 48 ) {
		if ( 36 <= (*p) && (*p) <= 45 )
			goto st524;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto st524;
		} else if ( (*p) >= 65 )
			goto st524;
	} else
		goto st558;
	goto st0;
st558:
	if ( ++p == pe )
		goto _test_eof558;
case 558:
	switch( (*p) ) {
		case 33: goto st524;
		case 35: goto tr565;
		case 37: goto st255;
		case 46: goto st554;
		case 47: goto tr566;
		case 58: goto tr582;
		case 59: goto st524;
		case 61: goto st524;
		case 63: goto tr568;
		case 64: goto tr263;
		case 95: goto st524;
		case 126: goto st524;
	}
	if ( (*p) < 48 ) {
		if ( 36 <= (*p) && (*p) <= 45 )
			goto st524;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto st524;
		} else if ( (*p) >= 65 )
			goto st524;
	} else
		goto st553;
	goto st0;
st559:
	if ( ++p == pe )
		goto _test_eof559;
case 559:
	switch( (*p) ) {
		case 33: goto st524;
		case 35: goto tr565;
		case 37: goto st255;
		case 46: goto st554;
		case 47: goto tr566;
		case 53: goto st560;
		case 58: goto tr582;
		case 61: goto st524;
		case 63: goto tr568;
		case 64: goto tr263;
		case 95: goto st524;
		case 126: goto st524;
	}
	if ( (*p) < 65 ) {
		if ( 36 <= (*p) && (*p) <= 59 )
			goto st524;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto st524;
	} else
		goto st524;
	goto st0;
st560:
	if ( ++p == pe )
		goto _test_eof560;
case 560:
	switch( (*p) ) {
		case 32: goto st240;
		case 33: goto st524;
		case 35: goto tr565;
		case 37: goto st255;
		case 47: goto tr566;
		case 48: goto st553;
		case 53: goto st553;
		case 58: goto tr582;
		case 61: goto st524;
		case 63: goto tr568;
		case 64: goto tr263;
		case 95: goto st524;
		case 126: goto st524;
	}
	if ( (*p) < 65 ) {
		if ( 36 <= (*p) && (*p) <= 59 )
			goto st524;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto st524;
	} else
		goto st524;
	goto st0;
st561:
	if ( ++p == pe )
		goto _test_eof561;
case 561:
	switch( (*p) ) {
		case 33: goto st524;
		case 35: goto tr565;
		case 37: goto st255;
		case 46: goto st552;
		case 47: goto tr566;
		case 58: goto tr582;
		case 59: goto st524;
		case 61: goto st524;
		case 63: goto tr568;
		case 64: goto tr263;
		case 95: goto st524;
		case 126: goto st524;
	}
	if ( (*p) < 48 ) {
		if ( 36 <= (*p) && (*p) <= 45 )
			goto st524;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto st524;
		} else if ( (*p) >= 65 )
			goto st524;
	} else
		goto st562;
	goto st0;
st562:
	if ( ++p == pe )
		goto _test_eof562;
case 562:
	switch( (*p) ) {
		case 33: goto st524;
		case 35: goto tr565;
		case 37: goto st255;
		case 46: goto st552;
		case 47: goto tr566;
		case 58: goto tr582;
		case 59: goto st524;
		case 61: goto st524;
		case 63: goto tr568;
		case 64: goto tr263;
		case 95: goto st524;
		case 126: goto st524;
	}
	if ( (*p) < 48 ) {
		if ( 36 <= (*p) && (*p) <= 45 )
			goto st524;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto st524;
		} else if ( (*p) >= 65 )
			goto st524;
	} else
		goto st551;
	goto st0;
st563:
	if ( ++p == pe )
		goto _test_eof563;
case 563:
	switch( (*p) ) {
		case 33: goto st524;
		case 35: goto tr565;
		case 37: goto st255;
		case 46: goto st552;
		case 47: goto tr566;
		case 53: goto st564;
		case 58: goto tr582;
		case 61: goto st524;
		case 63: goto tr568;
		case 64: goto tr263;
		case 95: goto st524;
		case 126: goto st524;
	}
	if ( (*p) < 65 ) {
		if ( 36 <= (*p) && (*p) <= 59 )
			goto st524;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto st524;
	} else
		goto st524;
	goto st0;
st564:
	if ( ++p == pe )
		goto _test_eof564;
case 564:
	switch( (*p) ) {
		case 32: goto st236;
		case 33: goto st524;
		case 35: goto tr565;
		case 37: goto st255;
		case 47: goto tr566;
		case 48: goto st551;
		case 53: goto st551;
		case 58: goto tr582;
		case 61: goto st524;
		case 63: goto tr568;
		case 64: goto tr263;
		case 95: goto st524;
		case 126: goto st524;
	}
	if ( (*p) < 65 ) {
		if ( 36 <= (*p) && (*p) <= 59 )
			goto st524;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto st524;
	} else
		goto st524;
	goto st0;
tr558:
#line 18 "uri.rl"
	{ mark = p; }
	goto st565;
st565:
	if ( ++p == pe )
		goto _test_eof565;
case 565:
#line 10107 "uri.rl.cc"
	switch( (*p) ) {
		case 33: goto st524;
		case 35: goto tr565;
		case 37: goto st255;
		case 46: goto st550;
		case 47: goto tr566;
		case 58: goto tr582;
		case 59: goto st524;
		case 61: goto st524;
		case 63: goto tr568;
		case 64: goto tr263;
		case 95: goto st524;
		case 126: goto st524;
	}
	if ( (*p) < 48 ) {
		if ( 36 <= (*p) && (*p) <= 45 )
			goto st524;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto st524;
		} else if ( (*p) >= 65 )
			goto st524;
	} else
		goto st566;
	goto st0;
tr560:
#line 18 "uri.rl"
	{ mark = p; }
	goto st566;
st566:
	if ( ++p == pe )
		goto _test_eof566;
case 566:
#line 10142 "uri.rl.cc"
	switch( (*p) ) {
		case 33: goto st524;
		case 35: goto tr565;
		case 37: goto st255;
		case 46: goto st550;
		case 47: goto tr566;
		case 58: goto tr582;
		case 59: goto st524;
		case 61: goto st524;
		case 63: goto tr568;
		case 64: goto tr263;
		case 95: goto st524;
		case 126: goto st524;
	}
	if ( (*p) < 48 ) {
		if ( 36 <= (*p) && (*p) <= 45 )
			goto st524;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto st524;
		} else if ( (*p) >= 65 )
			goto st524;
	} else
		goto st549;
	goto st0;
tr559:
#line 18 "uri.rl"
	{ mark = p; }
	goto st567;
st567:
	if ( ++p == pe )
		goto _test_eof567;
case 567:
#line 10177 "uri.rl.cc"
	switch( (*p) ) {
		case 33: goto st524;
		case 35: goto tr565;
		case 37: goto st255;
		case 46: goto st550;
		case 47: goto tr566;
		case 53: goto st568;
		case 58: goto tr582;
		case 61: goto st524;
		case 63: goto tr568;
		case 64: goto tr263;
		case 95: goto st524;
		case 126: goto st524;
	}
	if ( (*p) < 65 ) {
		if ( 36 <= (*p) && (*p) <= 59 )
			goto st524;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto st524;
	} else
		goto st524;
	goto st0;
st568:
	if ( ++p == pe )
		goto _test_eof568;
case 568:
	switch( (*p) ) {
		case 32: goto st232;
		case 33: goto st524;
		case 35: goto tr565;
		case 37: goto st255;
		case 47: goto tr566;
		case 48: goto st549;
		case 53: goto st549;
		case 58: goto tr582;
		case 61: goto st524;
		case 63: goto tr568;
		case 64: goto tr263;
		case 95: goto st524;
		case 126: goto st524;
	}
	if ( (*p) < 65 ) {
		if ( 36 <= (*p) && (*p) <= 59 )
			goto st524;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto st524;
	} else
		goto st524;
	goto st0;
	}
	_test_eof454: cs = 454; goto _test_eof; 
	_test_eof455: cs = 455; goto _test_eof; 
	_test_eof456: cs = 456; goto _test_eof; 
	_test_eof1: cs = 1; goto _test_eof; 
	_test_eof2: cs = 2; goto _test_eof; 
	_test_eof3: cs = 3; goto _test_eof; 
	_test_eof4: cs = 4; goto _test_eof; 
	_test_eof457: cs = 457; goto _test_eof; 
	_test_eof5: cs = 5; goto _test_eof; 
	_test_eof6: cs = 6; goto _test_eof; 
	_test_eof458: cs = 458; goto _test_eof; 
	_test_eof459: cs = 459; goto _test_eof; 
	_test_eof7: cs = 7; goto _test_eof; 
	_test_eof8: cs = 8; goto _test_eof; 
	_test_eof460: cs = 460; goto _test_eof; 
	_test_eof461: cs = 461; goto _test_eof; 
	_test_eof9: cs = 9; goto _test_eof; 
	_test_eof10: cs = 10; goto _test_eof; 
	_test_eof11: cs = 11; goto _test_eof; 
	_test_eof12: cs = 12; goto _test_eof; 
	_test_eof13: cs = 13; goto _test_eof; 
	_test_eof14: cs = 14; goto _test_eof; 
	_test_eof15: cs = 15; goto _test_eof; 
	_test_eof16: cs = 16; goto _test_eof; 
	_test_eof17: cs = 17; goto _test_eof; 
	_test_eof18: cs = 18; goto _test_eof; 
	_test_eof19: cs = 19; goto _test_eof; 
	_test_eof20: cs = 20; goto _test_eof; 
	_test_eof21: cs = 21; goto _test_eof; 
	_test_eof22: cs = 22; goto _test_eof; 
	_test_eof462: cs = 462; goto _test_eof; 
	_test_eof463: cs = 463; goto _test_eof; 
	_test_eof464: cs = 464; goto _test_eof; 
	_test_eof465: cs = 465; goto _test_eof; 
	_test_eof466: cs = 466; goto _test_eof; 
	_test_eof467: cs = 467; goto _test_eof; 
	_test_eof23: cs = 23; goto _test_eof; 
	_test_eof24: cs = 24; goto _test_eof; 
	_test_eof25: cs = 25; goto _test_eof; 
	_test_eof26: cs = 26; goto _test_eof; 
	_test_eof27: cs = 27; goto _test_eof; 
	_test_eof28: cs = 28; goto _test_eof; 
	_test_eof29: cs = 29; goto _test_eof; 
	_test_eof30: cs = 30; goto _test_eof; 
	_test_eof31: cs = 31; goto _test_eof; 
	_test_eof468: cs = 468; goto _test_eof; 
	_test_eof32: cs = 32; goto _test_eof; 
	_test_eof33: cs = 33; goto _test_eof; 
	_test_eof469: cs = 469; goto _test_eof; 
	_test_eof34: cs = 34; goto _test_eof; 
	_test_eof35: cs = 35; goto _test_eof; 
	_test_eof36: cs = 36; goto _test_eof; 
	_test_eof470: cs = 470; goto _test_eof; 
	_test_eof471: cs = 471; goto _test_eof; 
	_test_eof37: cs = 37; goto _test_eof; 
	_test_eof38: cs = 38; goto _test_eof; 
	_test_eof472: cs = 472; goto _test_eof; 
	_test_eof473: cs = 473; goto _test_eof; 
	_test_eof474: cs = 474; goto _test_eof; 
	_test_eof475: cs = 475; goto _test_eof; 
	_test_eof476: cs = 476; goto _test_eof; 
	_test_eof477: cs = 477; goto _test_eof; 
	_test_eof478: cs = 478; goto _test_eof; 
	_test_eof479: cs = 479; goto _test_eof; 
	_test_eof480: cs = 480; goto _test_eof; 
	_test_eof481: cs = 481; goto _test_eof; 
	_test_eof482: cs = 482; goto _test_eof; 
	_test_eof483: cs = 483; goto _test_eof; 
	_test_eof484: cs = 484; goto _test_eof; 
	_test_eof485: cs = 485; goto _test_eof; 
	_test_eof486: cs = 486; goto _test_eof; 
	_test_eof487: cs = 487; goto _test_eof; 
	_test_eof488: cs = 488; goto _test_eof; 
	_test_eof489: cs = 489; goto _test_eof; 
	_test_eof490: cs = 490; goto _test_eof; 
	_test_eof491: cs = 491; goto _test_eof; 
	_test_eof39: cs = 39; goto _test_eof; 
	_test_eof40: cs = 40; goto _test_eof; 
	_test_eof41: cs = 41; goto _test_eof; 
	_test_eof42: cs = 42; goto _test_eof; 
	_test_eof43: cs = 43; goto _test_eof; 
	_test_eof44: cs = 44; goto _test_eof; 
	_test_eof45: cs = 45; goto _test_eof; 
	_test_eof46: cs = 46; goto _test_eof; 
	_test_eof47: cs = 47; goto _test_eof; 
	_test_eof48: cs = 48; goto _test_eof; 
	_test_eof49: cs = 49; goto _test_eof; 
	_test_eof50: cs = 50; goto _test_eof; 
	_test_eof51: cs = 51; goto _test_eof; 
	_test_eof52: cs = 52; goto _test_eof; 
	_test_eof53: cs = 53; goto _test_eof; 
	_test_eof54: cs = 54; goto _test_eof; 
	_test_eof55: cs = 55; goto _test_eof; 
	_test_eof56: cs = 56; goto _test_eof; 
	_test_eof57: cs = 57; goto _test_eof; 
	_test_eof58: cs = 58; goto _test_eof; 
	_test_eof59: cs = 59; goto _test_eof; 
	_test_eof60: cs = 60; goto _test_eof; 
	_test_eof61: cs = 61; goto _test_eof; 
	_test_eof62: cs = 62; goto _test_eof; 
	_test_eof63: cs = 63; goto _test_eof; 
	_test_eof64: cs = 64; goto _test_eof; 
	_test_eof65: cs = 65; goto _test_eof; 
	_test_eof66: cs = 66; goto _test_eof; 
	_test_eof67: cs = 67; goto _test_eof; 
	_test_eof68: cs = 68; goto _test_eof; 
	_test_eof69: cs = 69; goto _test_eof; 
	_test_eof70: cs = 70; goto _test_eof; 
	_test_eof71: cs = 71; goto _test_eof; 
	_test_eof72: cs = 72; goto _test_eof; 
	_test_eof73: cs = 73; goto _test_eof; 
	_test_eof74: cs = 74; goto _test_eof; 
	_test_eof75: cs = 75; goto _test_eof; 
	_test_eof76: cs = 76; goto _test_eof; 
	_test_eof77: cs = 77; goto _test_eof; 
	_test_eof78: cs = 78; goto _test_eof; 
	_test_eof79: cs = 79; goto _test_eof; 
	_test_eof80: cs = 80; goto _test_eof; 
	_test_eof81: cs = 81; goto _test_eof; 
	_test_eof82: cs = 82; goto _test_eof; 
	_test_eof83: cs = 83; goto _test_eof; 
	_test_eof84: cs = 84; goto _test_eof; 
	_test_eof85: cs = 85; goto _test_eof; 
	_test_eof86: cs = 86; goto _test_eof; 
	_test_eof87: cs = 87; goto _test_eof; 
	_test_eof88: cs = 88; goto _test_eof; 
	_test_eof89: cs = 89; goto _test_eof; 
	_test_eof90: cs = 90; goto _test_eof; 
	_test_eof91: cs = 91; goto _test_eof; 
	_test_eof92: cs = 92; goto _test_eof; 
	_test_eof93: cs = 93; goto _test_eof; 
	_test_eof94: cs = 94; goto _test_eof; 
	_test_eof95: cs = 95; goto _test_eof; 
	_test_eof96: cs = 96; goto _test_eof; 
	_test_eof97: cs = 97; goto _test_eof; 
	_test_eof98: cs = 98; goto _test_eof; 
	_test_eof99: cs = 99; goto _test_eof; 
	_test_eof100: cs = 100; goto _test_eof; 
	_test_eof101: cs = 101; goto _test_eof; 
	_test_eof102: cs = 102; goto _test_eof; 
	_test_eof103: cs = 103; goto _test_eof; 
	_test_eof104: cs = 104; goto _test_eof; 
	_test_eof105: cs = 105; goto _test_eof; 
	_test_eof106: cs = 106; goto _test_eof; 
	_test_eof107: cs = 107; goto _test_eof; 
	_test_eof108: cs = 108; goto _test_eof; 
	_test_eof109: cs = 109; goto _test_eof; 
	_test_eof110: cs = 110; goto _test_eof; 
	_test_eof111: cs = 111; goto _test_eof; 
	_test_eof112: cs = 112; goto _test_eof; 
	_test_eof113: cs = 113; goto _test_eof; 
	_test_eof114: cs = 114; goto _test_eof; 
	_test_eof115: cs = 115; goto _test_eof; 
	_test_eof116: cs = 116; goto _test_eof; 
	_test_eof117: cs = 117; goto _test_eof; 
	_test_eof118: cs = 118; goto _test_eof; 
	_test_eof119: cs = 119; goto _test_eof; 
	_test_eof120: cs = 120; goto _test_eof; 
	_test_eof121: cs = 121; goto _test_eof; 
	_test_eof122: cs = 122; goto _test_eof; 
	_test_eof123: cs = 123; goto _test_eof; 
	_test_eof124: cs = 124; goto _test_eof; 
	_test_eof125: cs = 125; goto _test_eof; 
	_test_eof126: cs = 126; goto _test_eof; 
	_test_eof127: cs = 127; goto _test_eof; 
	_test_eof128: cs = 128; goto _test_eof; 
	_test_eof129: cs = 129; goto _test_eof; 
	_test_eof130: cs = 130; goto _test_eof; 
	_test_eof131: cs = 131; goto _test_eof; 
	_test_eof132: cs = 132; goto _test_eof; 
	_test_eof133: cs = 133; goto _test_eof; 
	_test_eof134: cs = 134; goto _test_eof; 
	_test_eof135: cs = 135; goto _test_eof; 
	_test_eof136: cs = 136; goto _test_eof; 
	_test_eof137: cs = 137; goto _test_eof; 
	_test_eof138: cs = 138; goto _test_eof; 
	_test_eof139: cs = 139; goto _test_eof; 
	_test_eof140: cs = 140; goto _test_eof; 
	_test_eof141: cs = 141; goto _test_eof; 
	_test_eof142: cs = 142; goto _test_eof; 
	_test_eof143: cs = 143; goto _test_eof; 
	_test_eof144: cs = 144; goto _test_eof; 
	_test_eof145: cs = 145; goto _test_eof; 
	_test_eof146: cs = 146; goto _test_eof; 
	_test_eof147: cs = 147; goto _test_eof; 
	_test_eof148: cs = 148; goto _test_eof; 
	_test_eof149: cs = 149; goto _test_eof; 
	_test_eof150: cs = 150; goto _test_eof; 
	_test_eof151: cs = 151; goto _test_eof; 
	_test_eof152: cs = 152; goto _test_eof; 
	_test_eof153: cs = 153; goto _test_eof; 
	_test_eof154: cs = 154; goto _test_eof; 
	_test_eof155: cs = 155; goto _test_eof; 
	_test_eof156: cs = 156; goto _test_eof; 
	_test_eof157: cs = 157; goto _test_eof; 
	_test_eof158: cs = 158; goto _test_eof; 
	_test_eof159: cs = 159; goto _test_eof; 
	_test_eof160: cs = 160; goto _test_eof; 
	_test_eof161: cs = 161; goto _test_eof; 
	_test_eof162: cs = 162; goto _test_eof; 
	_test_eof163: cs = 163; goto _test_eof; 
	_test_eof164: cs = 164; goto _test_eof; 
	_test_eof165: cs = 165; goto _test_eof; 
	_test_eof166: cs = 166; goto _test_eof; 
	_test_eof167: cs = 167; goto _test_eof; 
	_test_eof168: cs = 168; goto _test_eof; 
	_test_eof169: cs = 169; goto _test_eof; 
	_test_eof170: cs = 170; goto _test_eof; 
	_test_eof171: cs = 171; goto _test_eof; 
	_test_eof172: cs = 172; goto _test_eof; 
	_test_eof173: cs = 173; goto _test_eof; 
	_test_eof174: cs = 174; goto _test_eof; 
	_test_eof175: cs = 175; goto _test_eof; 
	_test_eof176: cs = 176; goto _test_eof; 
	_test_eof177: cs = 177; goto _test_eof; 
	_test_eof178: cs = 178; goto _test_eof; 
	_test_eof179: cs = 179; goto _test_eof; 
	_test_eof180: cs = 180; goto _test_eof; 
	_test_eof181: cs = 181; goto _test_eof; 
	_test_eof182: cs = 182; goto _test_eof; 
	_test_eof183: cs = 183; goto _test_eof; 
	_test_eof184: cs = 184; goto _test_eof; 
	_test_eof185: cs = 185; goto _test_eof; 
	_test_eof186: cs = 186; goto _test_eof; 
	_test_eof187: cs = 187; goto _test_eof; 
	_test_eof188: cs = 188; goto _test_eof; 
	_test_eof189: cs = 189; goto _test_eof; 
	_test_eof190: cs = 190; goto _test_eof; 
	_test_eof191: cs = 191; goto _test_eof; 
	_test_eof192: cs = 192; goto _test_eof; 
	_test_eof193: cs = 193; goto _test_eof; 
	_test_eof194: cs = 194; goto _test_eof; 
	_test_eof195: cs = 195; goto _test_eof; 
	_test_eof196: cs = 196; goto _test_eof; 
	_test_eof197: cs = 197; goto _test_eof; 
	_test_eof198: cs = 198; goto _test_eof; 
	_test_eof199: cs = 199; goto _test_eof; 
	_test_eof200: cs = 200; goto _test_eof; 
	_test_eof201: cs = 201; goto _test_eof; 
	_test_eof202: cs = 202; goto _test_eof; 
	_test_eof203: cs = 203; goto _test_eof; 
	_test_eof204: cs = 204; goto _test_eof; 
	_test_eof205: cs = 205; goto _test_eof; 
	_test_eof206: cs = 206; goto _test_eof; 
	_test_eof207: cs = 207; goto _test_eof; 
	_test_eof208: cs = 208; goto _test_eof; 
	_test_eof209: cs = 209; goto _test_eof; 
	_test_eof210: cs = 210; goto _test_eof; 
	_test_eof211: cs = 211; goto _test_eof; 
	_test_eof212: cs = 212; goto _test_eof; 
	_test_eof213: cs = 213; goto _test_eof; 
	_test_eof214: cs = 214; goto _test_eof; 
	_test_eof215: cs = 215; goto _test_eof; 
	_test_eof216: cs = 216; goto _test_eof; 
	_test_eof217: cs = 217; goto _test_eof; 
	_test_eof218: cs = 218; goto _test_eof; 
	_test_eof219: cs = 219; goto _test_eof; 
	_test_eof220: cs = 220; goto _test_eof; 
	_test_eof221: cs = 221; goto _test_eof; 
	_test_eof222: cs = 222; goto _test_eof; 
	_test_eof223: cs = 223; goto _test_eof; 
	_test_eof224: cs = 224; goto _test_eof; 
	_test_eof225: cs = 225; goto _test_eof; 
	_test_eof226: cs = 226; goto _test_eof; 
	_test_eof227: cs = 227; goto _test_eof; 
	_test_eof228: cs = 228; goto _test_eof; 
	_test_eof229: cs = 229; goto _test_eof; 
	_test_eof492: cs = 492; goto _test_eof; 
	_test_eof493: cs = 493; goto _test_eof; 
	_test_eof494: cs = 494; goto _test_eof; 
	_test_eof495: cs = 495; goto _test_eof; 
	_test_eof496: cs = 496; goto _test_eof; 
	_test_eof497: cs = 497; goto _test_eof; 
	_test_eof498: cs = 498; goto _test_eof; 
	_test_eof499: cs = 499; goto _test_eof; 
	_test_eof500: cs = 500; goto _test_eof; 
	_test_eof501: cs = 501; goto _test_eof; 
	_test_eof502: cs = 502; goto _test_eof; 
	_test_eof503: cs = 503; goto _test_eof; 
	_test_eof504: cs = 504; goto _test_eof; 
	_test_eof505: cs = 505; goto _test_eof; 
	_test_eof506: cs = 506; goto _test_eof; 
	_test_eof507: cs = 507; goto _test_eof; 
	_test_eof508: cs = 508; goto _test_eof; 
	_test_eof509: cs = 509; goto _test_eof; 
	_test_eof510: cs = 510; goto _test_eof; 
	_test_eof511: cs = 511; goto _test_eof; 
	_test_eof512: cs = 512; goto _test_eof; 
	_test_eof513: cs = 513; goto _test_eof; 
	_test_eof514: cs = 514; goto _test_eof; 
	_test_eof515: cs = 515; goto _test_eof; 
	_test_eof516: cs = 516; goto _test_eof; 
	_test_eof230: cs = 230; goto _test_eof; 
	_test_eof231: cs = 231; goto _test_eof; 
	_test_eof232: cs = 232; goto _test_eof; 
	_test_eof233: cs = 233; goto _test_eof; 
	_test_eof234: cs = 234; goto _test_eof; 
	_test_eof235: cs = 235; goto _test_eof; 
	_test_eof236: cs = 236; goto _test_eof; 
	_test_eof237: cs = 237; goto _test_eof; 
	_test_eof238: cs = 238; goto _test_eof; 
	_test_eof239: cs = 239; goto _test_eof; 
	_test_eof240: cs = 240; goto _test_eof; 
	_test_eof241: cs = 241; goto _test_eof; 
	_test_eof242: cs = 242; goto _test_eof; 
	_test_eof243: cs = 243; goto _test_eof; 
	_test_eof517: cs = 517; goto _test_eof; 
	_test_eof518: cs = 518; goto _test_eof; 
	_test_eof244: cs = 244; goto _test_eof; 
	_test_eof245: cs = 245; goto _test_eof; 
	_test_eof519: cs = 519; goto _test_eof; 
	_test_eof520: cs = 520; goto _test_eof; 
	_test_eof521: cs = 521; goto _test_eof; 
	_test_eof522: cs = 522; goto _test_eof; 
	_test_eof523: cs = 523; goto _test_eof; 
	_test_eof246: cs = 246; goto _test_eof; 
	_test_eof247: cs = 247; goto _test_eof; 
	_test_eof248: cs = 248; goto _test_eof; 
	_test_eof249: cs = 249; goto _test_eof; 
	_test_eof250: cs = 250; goto _test_eof; 
	_test_eof251: cs = 251; goto _test_eof; 
	_test_eof252: cs = 252; goto _test_eof; 
	_test_eof253: cs = 253; goto _test_eof; 
	_test_eof254: cs = 254; goto _test_eof; 
	_test_eof524: cs = 524; goto _test_eof; 
	_test_eof255: cs = 255; goto _test_eof; 
	_test_eof256: cs = 256; goto _test_eof; 
	_test_eof525: cs = 525; goto _test_eof; 
	_test_eof257: cs = 257; goto _test_eof; 
	_test_eof258: cs = 258; goto _test_eof; 
	_test_eof259: cs = 259; goto _test_eof; 
	_test_eof526: cs = 526; goto _test_eof; 
	_test_eof527: cs = 527; goto _test_eof; 
	_test_eof260: cs = 260; goto _test_eof; 
	_test_eof261: cs = 261; goto _test_eof; 
	_test_eof528: cs = 528; goto _test_eof; 
	_test_eof529: cs = 529; goto _test_eof; 
	_test_eof530: cs = 530; goto _test_eof; 
	_test_eof531: cs = 531; goto _test_eof; 
	_test_eof532: cs = 532; goto _test_eof; 
	_test_eof533: cs = 533; goto _test_eof; 
	_test_eof534: cs = 534; goto _test_eof; 
	_test_eof535: cs = 535; goto _test_eof; 
	_test_eof536: cs = 536; goto _test_eof; 
	_test_eof537: cs = 537; goto _test_eof; 
	_test_eof538: cs = 538; goto _test_eof; 
	_test_eof539: cs = 539; goto _test_eof; 
	_test_eof540: cs = 540; goto _test_eof; 
	_test_eof541: cs = 541; goto _test_eof; 
	_test_eof542: cs = 542; goto _test_eof; 
	_test_eof543: cs = 543; goto _test_eof; 
	_test_eof544: cs = 544; goto _test_eof; 
	_test_eof545: cs = 545; goto _test_eof; 
	_test_eof546: cs = 546; goto _test_eof; 
	_test_eof547: cs = 547; goto _test_eof; 
	_test_eof262: cs = 262; goto _test_eof; 
	_test_eof263: cs = 263; goto _test_eof; 
	_test_eof264: cs = 264; goto _test_eof; 
	_test_eof265: cs = 265; goto _test_eof; 
	_test_eof266: cs = 266; goto _test_eof; 
	_test_eof267: cs = 267; goto _test_eof; 
	_test_eof268: cs = 268; goto _test_eof; 
	_test_eof269: cs = 269; goto _test_eof; 
	_test_eof270: cs = 270; goto _test_eof; 
	_test_eof271: cs = 271; goto _test_eof; 
	_test_eof272: cs = 272; goto _test_eof; 
	_test_eof273: cs = 273; goto _test_eof; 
	_test_eof274: cs = 274; goto _test_eof; 
	_test_eof275: cs = 275; goto _test_eof; 
	_test_eof276: cs = 276; goto _test_eof; 
	_test_eof277: cs = 277; goto _test_eof; 
	_test_eof278: cs = 278; goto _test_eof; 
	_test_eof279: cs = 279; goto _test_eof; 
	_test_eof280: cs = 280; goto _test_eof; 
	_test_eof281: cs = 281; goto _test_eof; 
	_test_eof282: cs = 282; goto _test_eof; 
	_test_eof283: cs = 283; goto _test_eof; 
	_test_eof284: cs = 284; goto _test_eof; 
	_test_eof285: cs = 285; goto _test_eof; 
	_test_eof286: cs = 286; goto _test_eof; 
	_test_eof287: cs = 287; goto _test_eof; 
	_test_eof288: cs = 288; goto _test_eof; 
	_test_eof289: cs = 289; goto _test_eof; 
	_test_eof290: cs = 290; goto _test_eof; 
	_test_eof291: cs = 291; goto _test_eof; 
	_test_eof292: cs = 292; goto _test_eof; 
	_test_eof293: cs = 293; goto _test_eof; 
	_test_eof294: cs = 294; goto _test_eof; 
	_test_eof295: cs = 295; goto _test_eof; 
	_test_eof296: cs = 296; goto _test_eof; 
	_test_eof297: cs = 297; goto _test_eof; 
	_test_eof298: cs = 298; goto _test_eof; 
	_test_eof299: cs = 299; goto _test_eof; 
	_test_eof300: cs = 300; goto _test_eof; 
	_test_eof301: cs = 301; goto _test_eof; 
	_test_eof302: cs = 302; goto _test_eof; 
	_test_eof303: cs = 303; goto _test_eof; 
	_test_eof304: cs = 304; goto _test_eof; 
	_test_eof305: cs = 305; goto _test_eof; 
	_test_eof306: cs = 306; goto _test_eof; 
	_test_eof307: cs = 307; goto _test_eof; 
	_test_eof308: cs = 308; goto _test_eof; 
	_test_eof309: cs = 309; goto _test_eof; 
	_test_eof310: cs = 310; goto _test_eof; 
	_test_eof311: cs = 311; goto _test_eof; 
	_test_eof312: cs = 312; goto _test_eof; 
	_test_eof313: cs = 313; goto _test_eof; 
	_test_eof314: cs = 314; goto _test_eof; 
	_test_eof315: cs = 315; goto _test_eof; 
	_test_eof316: cs = 316; goto _test_eof; 
	_test_eof317: cs = 317; goto _test_eof; 
	_test_eof318: cs = 318; goto _test_eof; 
	_test_eof319: cs = 319; goto _test_eof; 
	_test_eof320: cs = 320; goto _test_eof; 
	_test_eof321: cs = 321; goto _test_eof; 
	_test_eof322: cs = 322; goto _test_eof; 
	_test_eof323: cs = 323; goto _test_eof; 
	_test_eof324: cs = 324; goto _test_eof; 
	_test_eof325: cs = 325; goto _test_eof; 
	_test_eof326: cs = 326; goto _test_eof; 
	_test_eof327: cs = 327; goto _test_eof; 
	_test_eof328: cs = 328; goto _test_eof; 
	_test_eof329: cs = 329; goto _test_eof; 
	_test_eof330: cs = 330; goto _test_eof; 
	_test_eof331: cs = 331; goto _test_eof; 
	_test_eof332: cs = 332; goto _test_eof; 
	_test_eof333: cs = 333; goto _test_eof; 
	_test_eof334: cs = 334; goto _test_eof; 
	_test_eof335: cs = 335; goto _test_eof; 
	_test_eof336: cs = 336; goto _test_eof; 
	_test_eof337: cs = 337; goto _test_eof; 
	_test_eof338: cs = 338; goto _test_eof; 
	_test_eof339: cs = 339; goto _test_eof; 
	_test_eof340: cs = 340; goto _test_eof; 
	_test_eof341: cs = 341; goto _test_eof; 
	_test_eof342: cs = 342; goto _test_eof; 
	_test_eof343: cs = 343; goto _test_eof; 
	_test_eof344: cs = 344; goto _test_eof; 
	_test_eof345: cs = 345; goto _test_eof; 
	_test_eof346: cs = 346; goto _test_eof; 
	_test_eof347: cs = 347; goto _test_eof; 
	_test_eof348: cs = 348; goto _test_eof; 
	_test_eof349: cs = 349; goto _test_eof; 
	_test_eof350: cs = 350; goto _test_eof; 
	_test_eof351: cs = 351; goto _test_eof; 
	_test_eof352: cs = 352; goto _test_eof; 
	_test_eof353: cs = 353; goto _test_eof; 
	_test_eof354: cs = 354; goto _test_eof; 
	_test_eof355: cs = 355; goto _test_eof; 
	_test_eof356: cs = 356; goto _test_eof; 
	_test_eof357: cs = 357; goto _test_eof; 
	_test_eof358: cs = 358; goto _test_eof; 
	_test_eof359: cs = 359; goto _test_eof; 
	_test_eof360: cs = 360; goto _test_eof; 
	_test_eof361: cs = 361; goto _test_eof; 
	_test_eof362: cs = 362; goto _test_eof; 
	_test_eof363: cs = 363; goto _test_eof; 
	_test_eof364: cs = 364; goto _test_eof; 
	_test_eof365: cs = 365; goto _test_eof; 
	_test_eof366: cs = 366; goto _test_eof; 
	_test_eof367: cs = 367; goto _test_eof; 
	_test_eof368: cs = 368; goto _test_eof; 
	_test_eof369: cs = 369; goto _test_eof; 
	_test_eof370: cs = 370; goto _test_eof; 
	_test_eof371: cs = 371; goto _test_eof; 
	_test_eof372: cs = 372; goto _test_eof; 
	_test_eof373: cs = 373; goto _test_eof; 
	_test_eof374: cs = 374; goto _test_eof; 
	_test_eof375: cs = 375; goto _test_eof; 
	_test_eof376: cs = 376; goto _test_eof; 
	_test_eof377: cs = 377; goto _test_eof; 
	_test_eof378: cs = 378; goto _test_eof; 
	_test_eof379: cs = 379; goto _test_eof; 
	_test_eof380: cs = 380; goto _test_eof; 
	_test_eof381: cs = 381; goto _test_eof; 
	_test_eof382: cs = 382; goto _test_eof; 
	_test_eof383: cs = 383; goto _test_eof; 
	_test_eof384: cs = 384; goto _test_eof; 
	_test_eof385: cs = 385; goto _test_eof; 
	_test_eof386: cs = 386; goto _test_eof; 
	_test_eof387: cs = 387; goto _test_eof; 
	_test_eof388: cs = 388; goto _test_eof; 
	_test_eof389: cs = 389; goto _test_eof; 
	_test_eof390: cs = 390; goto _test_eof; 
	_test_eof391: cs = 391; goto _test_eof; 
	_test_eof392: cs = 392; goto _test_eof; 
	_test_eof393: cs = 393; goto _test_eof; 
	_test_eof394: cs = 394; goto _test_eof; 
	_test_eof395: cs = 395; goto _test_eof; 
	_test_eof396: cs = 396; goto _test_eof; 
	_test_eof397: cs = 397; goto _test_eof; 
	_test_eof398: cs = 398; goto _test_eof; 
	_test_eof399: cs = 399; goto _test_eof; 
	_test_eof400: cs = 400; goto _test_eof; 
	_test_eof401: cs = 401; goto _test_eof; 
	_test_eof402: cs = 402; goto _test_eof; 
	_test_eof403: cs = 403; goto _test_eof; 
	_test_eof404: cs = 404; goto _test_eof; 
	_test_eof405: cs = 405; goto _test_eof; 
	_test_eof406: cs = 406; goto _test_eof; 
	_test_eof407: cs = 407; goto _test_eof; 
	_test_eof408: cs = 408; goto _test_eof; 
	_test_eof409: cs = 409; goto _test_eof; 
	_test_eof410: cs = 410; goto _test_eof; 
	_test_eof411: cs = 411; goto _test_eof; 
	_test_eof412: cs = 412; goto _test_eof; 
	_test_eof413: cs = 413; goto _test_eof; 
	_test_eof414: cs = 414; goto _test_eof; 
	_test_eof415: cs = 415; goto _test_eof; 
	_test_eof416: cs = 416; goto _test_eof; 
	_test_eof417: cs = 417; goto _test_eof; 
	_test_eof418: cs = 418; goto _test_eof; 
	_test_eof419: cs = 419; goto _test_eof; 
	_test_eof420: cs = 420; goto _test_eof; 
	_test_eof421: cs = 421; goto _test_eof; 
	_test_eof422: cs = 422; goto _test_eof; 
	_test_eof423: cs = 423; goto _test_eof; 
	_test_eof424: cs = 424; goto _test_eof; 
	_test_eof425: cs = 425; goto _test_eof; 
	_test_eof426: cs = 426; goto _test_eof; 
	_test_eof427: cs = 427; goto _test_eof; 
	_test_eof428: cs = 428; goto _test_eof; 
	_test_eof429: cs = 429; goto _test_eof; 
	_test_eof430: cs = 430; goto _test_eof; 
	_test_eof431: cs = 431; goto _test_eof; 
	_test_eof432: cs = 432; goto _test_eof; 
	_test_eof433: cs = 433; goto _test_eof; 
	_test_eof434: cs = 434; goto _test_eof; 
	_test_eof435: cs = 435; goto _test_eof; 
	_test_eof436: cs = 436; goto _test_eof; 
	_test_eof437: cs = 437; goto _test_eof; 
	_test_eof438: cs = 438; goto _test_eof; 
	_test_eof439: cs = 439; goto _test_eof; 
	_test_eof440: cs = 440; goto _test_eof; 
	_test_eof441: cs = 441; goto _test_eof; 
	_test_eof442: cs = 442; goto _test_eof; 
	_test_eof443: cs = 443; goto _test_eof; 
	_test_eof444: cs = 444; goto _test_eof; 
	_test_eof445: cs = 445; goto _test_eof; 
	_test_eof446: cs = 446; goto _test_eof; 
	_test_eof447: cs = 447; goto _test_eof; 
	_test_eof448: cs = 448; goto _test_eof; 
	_test_eof449: cs = 449; goto _test_eof; 
	_test_eof450: cs = 450; goto _test_eof; 
	_test_eof451: cs = 451; goto _test_eof; 
	_test_eof452: cs = 452; goto _test_eof; 
	_test_eof548: cs = 548; goto _test_eof; 
	_test_eof549: cs = 549; goto _test_eof; 
	_test_eof550: cs = 550; goto _test_eof; 
	_test_eof551: cs = 551; goto _test_eof; 
	_test_eof552: cs = 552; goto _test_eof; 
	_test_eof553: cs = 553; goto _test_eof; 
	_test_eof554: cs = 554; goto _test_eof; 
	_test_eof555: cs = 555; goto _test_eof; 
	_test_eof556: cs = 556; goto _test_eof; 
	_test_eof557: cs = 557; goto _test_eof; 
	_test_eof558: cs = 558; goto _test_eof; 
	_test_eof559: cs = 559; goto _test_eof; 
	_test_eof560: cs = 560; goto _test_eof; 
	_test_eof561: cs = 561; goto _test_eof; 
	_test_eof562: cs = 562; goto _test_eof; 
	_test_eof563: cs = 563; goto _test_eof; 
	_test_eof564: cs = 564; goto _test_eof; 
	_test_eof565: cs = 565; goto _test_eof; 
	_test_eof566: cs = 566; goto _test_eof; 
	_test_eof567: cs = 567; goto _test_eof; 
	_test_eof568: cs = 568; goto _test_eof; 

	_test_eof: {}
	if ( p == eof )
	{
	switch ( cs ) {
	case 464: 
	case 492: 
#line 27 "uri.rl"
	{
        if (p != mark) {
            uri->setPort(atoi(mark));
        }
        mark = NULL;
    }
	break;
	case 462: 
	case 465: 
	case 466: 
	case 467: 
	case 468: 
	case 471: 
	case 472: 
	case 473: 
	case 474: 
	case 475: 
	case 476: 
	case 477: 
	case 478: 
	case 479: 
	case 480: 
	case 481: 
	case 482: 
	case 483: 
	case 484: 
	case 485: 
	case 486: 
	case 487: 
	case 488: 
	case 489: 
	case 490: 
	case 491: 
	case 493: 
	case 494: 
	case 495: 
	case 496: 
	case 497: 
	case 498: 
	case 499: 
	case 500: 
	case 501: 
	case 502: 
	case 503: 
	case 504: 
	case 505: 
	case 506: 
	case 507: 
	case 508: 
	case 509: 
	case 510: 
	case 511: 
	case 512: 
#line 40 "uri.rl"
	{
        if (mark != NULL) {
            // std::cout << std::string(mark, fpc - mark) << std::endl;
            uri->setHost(std::string(mark, p - mark));
        }
    }
	break;
	case 518: 
#line 73 "uri.rl"
	{
        // std::cout << std::string(mark, fpc - mark) << std::endl;
        uri->setPath(std::string(mark, p - mark));
        mark = NULL;
    }
	break;
	case 459: 
#line 96 "uri.rl"
	{
        // std::cout << std::string(mark, fpc - mark) << std::endl;
        uri->setQuery(std::string(mark, p - mark));
        mark = NULL;
    }
	break;
	case 456: 
#line 101 "uri.rl"
	{
        // std::cout << std::string(mark, fpc - mark) << std::endl;
        uri->setFragment(std::string(mark, p - mark));
        mark = NULL;
    }
	break;
	case 458: 
#line 17 "uri.rl"
	{ mark = p; }
#line 96 "uri.rl"
	{
        // std::cout << std::string(mark, fpc - mark) << std::endl;
        uri->setQuery(std::string(mark, p - mark));
        mark = NULL;
    }
	break;
	case 455: 
#line 17 "uri.rl"
	{ mark = p; }
#line 101 "uri.rl"
	{
        // std::cout << std::string(mark, fpc - mark) << std::endl;
        uri->setFragment(std::string(mark, p - mark));
        mark = NULL;
    }
	break;
	case 463: 
	case 469: 
#line 18 "uri.rl"
	{ mark = p; }
#line 27 "uri.rl"
	{
        if (p != mark) {
            uri->setPort(atoi(mark));
        }
        mark = NULL;
    }
	break;
	case 461: 
	case 470: 
#line 18 "uri.rl"
	{ mark = p; }
#line 40 "uri.rl"
	{
        if (mark != NULL) {
            // std::cout << std::string(mark, fpc - mark) << std::endl;
            uri->setHost(std::string(mark, p - mark));
        }
    }
	break;
	case 519: 
	case 525: 
#line 18 "uri.rl"
	{ mark = p; }
#line 27 "uri.rl"
	{
        if (p != mark) {
            uri->setPort(atoi(mark));
        }
        mark = NULL;
    }
#line 73 "uri.rl"
	{
        // std::cout << std::string(mark, fpc - mark) << std::endl;
        uri->setPath(std::string(mark, p - mark));
        mark = NULL;
    }
	break;
	case 516: 
	case 526: 
#line 18 "uri.rl"
	{ mark = p; }
#line 40 "uri.rl"
	{
        if (mark != NULL) {
            // std::cout << std::string(mark, fpc - mark) << std::endl;
            uri->setHost(std::string(mark, p - mark));
        }
    }
#line 73 "uri.rl"
	{
        // std::cout << std::string(mark, fpc - mark) << std::endl;
        uri->setPath(std::string(mark, p - mark));
        mark = NULL;
    }
	break;
	case 520: 
	case 548: 
#line 27 "uri.rl"
	{
        if (p != mark) {
            uri->setPort(atoi(mark));
        }
        mark = NULL;
    }
#line 18 "uri.rl"
	{ mark = p; }
#line 73 "uri.rl"
	{
        // std::cout << std::string(mark, fpc - mark) << std::endl;
        uri->setPath(std::string(mark, p - mark));
        mark = NULL;
    }
	break;
	case 517: 
	case 521: 
	case 522: 
	case 523: 
	case 524: 
	case 527: 
	case 528: 
	case 529: 
	case 530: 
	case 531: 
	case 532: 
	case 533: 
	case 534: 
	case 535: 
	case 536: 
	case 537: 
	case 538: 
	case 539: 
	case 540: 
	case 541: 
	case 542: 
	case 543: 
	case 544: 
	case 545: 
	case 546: 
	case 547: 
	case 549: 
	case 550: 
	case 551: 
	case 552: 
	case 553: 
	case 554: 
	case 555: 
	case 556: 
	case 557: 
	case 558: 
	case 559: 
	case 560: 
	case 561: 
	case 562: 
	case 563: 
	case 564: 
	case 565: 
	case 566: 
	case 567: 
	case 568: 
#line 40 "uri.rl"
	{
        if (mark != NULL) {
            // std::cout << std::string(mark, fpc - mark) << std::endl;
            uri->setHost(std::string(mark, p - mark));
        }
    }
#line 18 "uri.rl"
	{ mark = p; }
#line 73 "uri.rl"
	{
        // std::cout << std::string(mark, fpc - mark) << std::endl;
        uri->setPath(std::string(mark, p - mark));
        mark = NULL;
    }
	break;
#line 11050 "uri.rl.cc"
	}
	}

	_out: {}
	}

#line 136 "uri.rl"

    if (cs == uri_parser_error) {
        return nullptr;
    } else if (cs >= uri_parser_first_final) {
        return uri;
    }
    return uri;
}

const std::string &Uri::getPath() const {
    static std::string s_default_path = "/";
    return (m_path.empty() ? s_default_path : m_path);
}

uint32_t Uri::getPort() const {
    if (m_port != 0) {
        return m_port;
    }
    if (m_scheme == "http") {
        return 80;
    } else if (m_scheme == "https") {
        return 443;
    }
    return 0;
}

bool Uri::isDefaultPort() const {
    if (m_port == 0) {
        return true;
    }
    if (m_scheme == "http") {
        return m_port == 80;
    } else if (m_scheme == "https") {
        return m_port == 443;
    }
    return false;
}

std::ostream &Uri::dump(std::ostream &os) const {
    os << m_scheme << "://" << m_userinfo << (m_userinfo.empty() ? "" : "@")
       << m_host << (isDefaultPort() ? "" : ":" + std::to_string(m_port))
       << getPath() << (m_query.empty() ? "" : "?" + m_query)
       << (m_fragment.empty() ? "" : "#" + m_fragment);
    return os;
}

std::string Uri::toString() const {
    std::stringstream ss;
    dump(ss);
    return ss.str();
}
} // namespace lyon
