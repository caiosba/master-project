/* ANTLR Translator Generator
 * Project led by Terence Parr at http://www.jGuru.com
 * Software rights: http://www.antlr.org/license.html
 *
 * $Id: TokenStreamBasicFilter.cpp,v 1.1.1.2 2004/11/08 20:45:24 braden Exp $
 */
#include "antlr/TokenStreamBasicFilter.hpp"

#ifdef ANTLR_CXX_SUPPORTS_NAMESPACE
namespace antlr {
#endif

/** This object is a TokenStream that passes through all
 *  tokens except for those that you tell it to discard.
 *  There is no buffering of the tokens.
 */
TokenStreamBasicFilter::TokenStreamBasicFilter(TokenStream& input_)
: input(&input_)
{
}

void TokenStreamBasicFilter::discard(int ttype)
{
	discardMask.add(ttype);
}

void TokenStreamBasicFilter::discard(const BitSet& mask)
{
	discardMask = mask;
}

RefToken TokenStreamBasicFilter::nextToken()
{
	RefToken tok = input->nextToken();
	while ( tok && discardMask.member(tok->getType()) ) {
		tok = input->nextToken();
	}
	return tok;
}

#ifdef ANTLR_CXX_SUPPORTS_NAMESPACE
}
#endif

