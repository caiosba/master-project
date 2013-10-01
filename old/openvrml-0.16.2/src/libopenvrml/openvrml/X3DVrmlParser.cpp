/* $ANTLR 2.7.4: "expandedX3DVrmlParser.g" -> "X3DVrmlParser.cpp"$ */
#line 46 "expandedX3DVrmlParser.g"

# include "X3DVrmlParser.hpp"
ANTLR_RBRACE

#line 8 "X3DVrmlParser.cpp"
#include "X3DVrmlParser.hpp"
#include <antlr/NoViableAltException.hpp>
#include <antlr/SemanticException.hpp>
#include <antlr/ASTFactory.hpp>
#line 50 "expandedX3DVrmlParser.g"

# include <cctype>
# include <antlr/CommonToken.hpp>
# include <boost/lexical_cast.hpp>
# include <private.h>
# include "scope.h"
# include "script.h"

namespace {
const int X3DVrmlScanner::COLON                  (48);
const int X3DVrmlScanner::KEYWORD_AS             (49);
const int X3DVrmlScanner::KEYWORD_COMPONENT      (50);
const int X3DVrmlScanner::KEYWORD_EXPORT         (51);
const int X3DVrmlScanner::KEYWORD_IMPORT         (52);
const int X3DVrmlScanner::KEYWORD_META           (53);
const int X3DVrmlScanner::KEYWORD_PROFILE        (54);
const int X3DVrmlScanner::KEYWORD_INPUTONLY      (55);
const int X3DVrmlScanner::KEYWORD_OUTPUTONLY     (56);
const int X3DVrmlScanner::KEYWORD_INPUTOUTPUT    (57);
const int X3DVrmlScanner::KEYWORD_INITIALIZEONLY (58);
const int X3DVrmlScanner::FIELDTYPE_SFCOLORRGBA  (59);
const int X3DVrmlScanner::FIELDTYPE_SFDOUBLE     (60);
const int X3DVrmlScanner::FIELDTYPE_SFVEC2D      (61);
const int X3DVrmlScanner::FIELDTYPE_SFVEC3D      (62);
const int X3DVrmlScanner::FIELDTYPE_MFBOOL       (63);
const int X3DVrmlScanner::FIELDTYPE_MFCOLORRGBA  (64);
const int X3DVrmlScanner::FIELDTYPE_MFDOUBLE     (65);
const int X3DVrmlScanner::FIELDTYPE_MFIMAGE      (66);
const int X3DVrmlScanner::FIELDTYPE_MFVEC2D      (67);
const int X3DVrmlScanner::FIELDTYPE_MFVEC3D      (68);

X3DVrmlScanner::X3DVrmlScanner(std::istream & in):
    Vrml97Scanner(in)
{}

//
// The X3D Classic VRML encoding specification doesn't list colon (':') as
// an invalid character in an identifier; however, in light of the fact that
// colon is used as a terminal symbol following an identifier in this format,
// this must be an error.
//
inline bool X3DVrmlScanner::isValidIdRestChars(const char c)
{
    return this->Vrml97Scanner::isValidIdRestChars(c) && (c != 0x3a);
}

inline bool X3DVrmlScanner::identifyKeyword(antlr::Token & token)
{
    if (!this->Vrml97Scanner::identifyKeyword(token)) {
        assert(token.getType() == antlr::Token::INVALID_TYPE);
        const std::string token_text(token.getText());
        if (token_text == "AS") {
            token.setType(KEYWORD_AS);
        } else if (token_text == "COMPONENT") {
            token.setType(KEYWORD_COMPONENT);
        } else if (token_text == "EXPORT") {
            token.setType(KEYWORD_EXPORT);
        } else if (token_text == "IMPORT") {
            token.setType(KEYWORD_IMPORT);
        } else if (token_text == "META") {
            token.setType(KEYWORD_META);
        } else if (token_text == "PROFILE") {
            token.setType(KEYWORD_PROFILE);
        } else if (token_text == "inputOnly") {
            this->expectFieldType();
            token.setType(KEYWORD_EVENTIN);
        } else if (token_text == "outputOnly") {
            this->expectFieldType();
            token.setType(KEYWORD_EVENTOUT);
        } else if (token_text == "inputOutput") {
            this->expectFieldType();
            token.setType(KEYWORD_EXPOSEDFIELD);
        } else if (token_text == "initializeOnly") {
            this->expectFieldType();
            token.setType(KEYWORD_FIELD);
        } else {
            return false;
        }
    }
    return true;
}

inline bool X3DVrmlScanner::identifyFieldType(antlr::Token & token)
{
    if (!this->Vrml97Scanner::identifyFieldType(token)) {
        assert(token.getType() == antlr::Token::INVALID_TYPE);
        const std::string token_text(token.getText());
        if (token_text == "SFColorRGBA") {
            token.setType(FIELDTYPE_SFCOLORRGBA);
        } else if (token_text == "SFDouble") {
            token.setType(FIELDTYPE_SFDOUBLE);
        } else if (token_text == "SFVec2d") {
            token.setType(FIELDTYPE_SFVEC2D);
        } else if (token_text == "SFVec3d") {
            token.setType(FIELDTYPE_SFVEC3D);
        } else if (token_text == "MFBool") {
            token.setType(FIELDTYPE_MFBOOL);
        } else if (token_text == "MFColorRGBA") {
            token.setType(FIELDTYPE_MFCOLORRGBA);
        } else if (token_text == "MFDouble") {
            token.setType(FIELDTYPE_MFDOUBLE);
        } else if (token_text == "MFImage") {
            token.setType(FIELDTYPE_MFIMAGE);
        } else if (token_text == "MFVec2d") {
            token.setType(FIELDTYPE_MFVEC2D);
        } else if (token_text == "MFVec3d") {
            token.setType(FIELDTYPE_MFVEC3D);
        } else {
            return false;
        }
    }
    return true;
}

inline bool X3DVrmlScanner::identifyTerminalSymbol(antlr::Token & token)
{
    if (!this->Vrml97Scanner::identifyTerminalSymbol(token)) {
        assert(token.getType() == antlr::Token::INVALID_TYPE);
        const std::string token_text(token.getText());
        if (token_text == ":") {
            token.setType(COLON);
        } else {
            return false;
        }
    }
    return true;
}

} // namespace

namespace openvrml ANTLR_LBRACE


#line 147 "X3DVrmlParser.cpp"
#line 1 "expandedX3DVrmlParser.g"
#line 149 "X3DVrmlParser.cpp"
X3DVrmlParser::X3DVrmlParser(ANTLR_USE_NAMESPACE(antlr)TokenBuffer& tokenBuf, int k)
: ANTLR_USE_NAMESPACE(antlr)LLkParser(tokenBuf,k)
{
}

X3DVrmlParser::X3DVrmlParser(ANTLR_USE_NAMESPACE(antlr)TokenBuffer& tokenBuf)
: ANTLR_USE_NAMESPACE(antlr)LLkParser(tokenBuf,1)
{
}

X3DVrmlParser::X3DVrmlParser(ANTLR_USE_NAMESPACE(antlr)TokenStream& lexer, int k)
: ANTLR_USE_NAMESPACE(antlr)LLkParser(lexer,k)
{
}

X3DVrmlParser::X3DVrmlParser(ANTLR_USE_NAMESPACE(antlr)TokenStream& lexer)
: ANTLR_USE_NAMESPACE(antlr)LLkParser(lexer,1)
{
}

X3DVrmlParser::X3DVrmlParser(const ANTLR_USE_NAMESPACE(antlr)ParserSharedInputState& state)
: ANTLR_USE_NAMESPACE(antlr)LLkParser(state,1)
{
}

void X3DVrmlParser::vrmlScene(
	const openvrml::scene & scene,
          std::vector<boost::intrusive_ptr<openvrml::node> > & nodes,
          std::map<std::string, std::string> & meta_data
) {
#line 239 "expandedX3DVrmlParser.g"
	
	std::string profile_id;
	
#line 184 "X3DVrmlParser.cpp"
	
	try {      // for error handling
		profile_id=profileStatement();
#line 248 "expandedX3DVrmlParser.g"
		
		const profile & p = ::profile_registry_.at(profile_id);
		std::auto_ptr<scope> root_scope_auto_ptr =
		p.create_root_scope(scene.browser(), this->getFilename());
		const boost::shared_ptr<scope> root_scope(root_scope_auto_ptr);
		
#line 195 "X3DVrmlParser.cpp"
		{ // ( ... )*
		for (;;) {
			if ((LA(1) == KEYWORD_COMPONENT)) {
				componentStatement(scene.browser(), *root_scope);
			}
			else {
				goto _loop3;
			}
			
		}
		_loop3:;
		} // ( ... )*
		{ // ( ... )*
		for (;;) {
			if ((LA(1) == KEYWORD_META)) {
				metaStatement(meta_data);
			}
			else {
				goto _loop5;
			}
			
		}
		_loop5:;
		} // ( ... )*
		{ // ( ... )*
		for (;;) {
			if ((_tokenSet_0.member(LA(1)))) {
				statement(scene, nodes, root_scope);
			}
			else {
				goto _loop7;
			}
			
		}
		_loop7:;
		} // ( ... )*
	}
	catch (boost::bad_ptr_container_operation &) {
#line 259 "expandedX3DVrmlParser.g"
		
		throw antlr::SemanticException("unrecognized profile \""
		+ profile_id + "\"",
		this->getFilename(),
		LT(1)->getLine(),
		LT(1)->getColumn());
		
#line 242 "X3DVrmlParser.cpp"
	}
}

std::string  X3DVrmlParser::profileStatement() {
#line 267 "expandedX3DVrmlParser.g"
	std::string profile_id;
#line 249 "X3DVrmlParser.cpp"
	ANTLR_USE_NAMESPACE(antlr)RefToken  id = ANTLR_USE_NAMESPACE(antlr)nullToken;
	
	match(KEYWORD_PROFILE);
	id = LT(1);
	match(ID);
#line 271 "expandedX3DVrmlParser.g"
	profile_id = id->getText();
#line 257 "X3DVrmlParser.cpp"
	return profile_id;
}

void X3DVrmlParser::componentStatement(
	openvrml::browser & browser, openvrml::scope & scope
) {
	ANTLR_USE_NAMESPACE(antlr)RefToken  id = ANTLR_USE_NAMESPACE(antlr)nullToken;
#line 274 "expandedX3DVrmlParser.g"
	
	int level;
	
#line 269 "X3DVrmlParser.cpp"
	
	try {      // for error handling
		match(KEYWORD_COMPONENT);
		id = LT(1);
		match(ID);
		match(COLON);
		level=intValue();
#line 281 "expandedX3DVrmlParser.g"
		
		const component & c = component_registry_.at(id->getText());
		c.add_to_scope(browser, scope, level);
		
#line 282 "X3DVrmlParser.cpp"
	}
	catch (boost::bad_ptr_container_operation &) {
#line 287 "expandedX3DVrmlParser.g"
		
		throw antlr::SemanticException("unrecognized component \""
		+ id->getText() + "\"",
		this->getFilename(),
		id->getLine(),
		id->getColumn());
		
#line 293 "X3DVrmlParser.cpp"
	}
	catch (std::invalid_argument & ex) {
#line 294 "expandedX3DVrmlParser.g"
		
		throw antlr::SemanticException(ex.what(),
		this->getFilename(),
		id->getLine(),
		id->getColumn());
		
#line 303 "X3DVrmlParser.cpp"
	}
}

void X3DVrmlParser::metaStatement(
	std::map<std::string, std::string> & meta_data
) {
#line 301 "expandedX3DVrmlParser.g"
	
	std::string key, value;
	
#line 314 "X3DVrmlParser.cpp"
	
	try {      // for error handling
		match(KEYWORD_META);
		key=stringValue();
		value=stringValue();
#line 304 "expandedX3DVrmlParser.g"
		
		meta_data[key] = value;
		
#line 324 "X3DVrmlParser.cpp"
	}
	catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& ex) {
		reportError(ex);
		consume();
		consumeUntil(_tokenSet_1);
	}
}

void X3DVrmlParser::statement(
	const openvrml::scene & scene,
          std::vector<boost::intrusive_ptr<openvrml::node> > & nodes,
          const boost::shared_ptr<openvrml::scope> & scope
) {
#line 309 "expandedX3DVrmlParser.g"
	
	boost::intrusive_ptr<openvrml::node> node;
	
#line 342 "X3DVrmlParser.cpp"
	
	switch ( LA(1)) {
	case ID:
	case KEYWORD_DEF:
	case KEYWORD_USE:
	{
		node=nodeStatement(scene, scope, std::string());
#line 318 "expandedX3DVrmlParser.g"
		
		//
		// If we are unable to parse a node, node will be null.
		//
		if (node) { nodes.push_back(node); }
		
#line 357 "X3DVrmlParser.cpp"
		break;
	}
	case KEYWORD_EXTERNPROTO:
	case KEYWORD_PROTO:
	{
		protoStatement(scene, scope);
		break;
	}
	case KEYWORD_ROUTE:
	{
		routeStatement(*scope);
		break;
	}
	case KEYWORD_IMPORT:
	{
		importStatement();
		break;
	}
	case KEYWORD_EXPORT:
	{
		exportStatement();
		break;
	}
	default:
	{
		throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
	}
	}
}

int32  X3DVrmlParser::intValue() {
#line 1927 "expandedX3DVrmlParser.g"
	int32 val;
#line 391 "X3DVrmlParser.cpp"
	ANTLR_USE_NAMESPACE(antlr)RefToken  i0 = ANTLR_USE_NAMESPACE(antlr)nullToken;
	ANTLR_USE_NAMESPACE(antlr)RefToken  i1 = ANTLR_USE_NAMESPACE(antlr)nullToken;
	
	switch ( LA(1)) {
	case INTEGER:
	{
		i0 = LT(1);
		match(INTEGER);
#line 1931 "expandedX3DVrmlParser.g"
		
		val = atol(i0->getText().c_str());
		
#line 404 "X3DVrmlParser.cpp"
		break;
	}
	case HEX_INTEGER:
	{
		i1 = LT(1);
		match(HEX_INTEGER);
#line 1934 "expandedX3DVrmlParser.g"
		
		val = int32(strtoul(i1->getText().c_str() + 2, 0, 16));
		
#line 415 "X3DVrmlParser.cpp"
		break;
	}
	default:
	{
		throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
	}
	}
	return val;
}

std::string  X3DVrmlParser::stringValue() {
#line 2140 "expandedX3DVrmlParser.g"
	std::string str;
#line 429 "X3DVrmlParser.cpp"
	ANTLR_USE_NAMESPACE(antlr)RefToken  s = ANTLR_USE_NAMESPACE(antlr)nullToken;
	
	s = LT(1);
	match(STRING);
#line 2144 "expandedX3DVrmlParser.g"
	
	using std::string;
	const string & token_text(s->getText());
	//
	// "Unescape" the string.
	//
	bool escape = false;
	for (string::const_iterator pos = token_text.begin() + 1;
	pos != token_text.end() - 1;
	++pos) {
	if (*pos == '\\' && !escape) {
	escape = true;
	} else {
	str.push_back(*pos);
	escape = false;
	}
	}
	
#line 453 "X3DVrmlParser.cpp"
	return str;
}

boost::intrusive_ptr<openvrml::node>  X3DVrmlParser::nodeStatement(
	const openvrml::scene & scene,
              const boost::shared_ptr<openvrml::scope> & scope,
              const std::string & script_node_id
) {
#line 616 "expandedX3DVrmlParser.g"
	boost::intrusive_ptr<openvrml::node> n;
#line 464 "X3DVrmlParser.cpp"
	ANTLR_USE_NAMESPACE(antlr)RefToken  id0 = ANTLR_USE_NAMESPACE(antlr)nullToken;
	ANTLR_USE_NAMESPACE(antlr)RefToken  id1 = ANTLR_USE_NAMESPACE(antlr)nullToken;
	
	switch ( LA(1)) {
	case KEYWORD_DEF:
	{
		match(KEYWORD_DEF);
		id0 = LT(1);
		match(ID);
		n=node(scene, scope, id0->getText());
		break;
	}
	case KEYWORD_USE:
	{
		match(KEYWORD_USE);
		id1 = LT(1);
		match(ID);
#line 623 "expandedX3DVrmlParser.g"
		
		if (id1->getText() == script_node_id) {
		//
		// Script node self-reference.
		//
		n = openvrml::node::self_tag;
		} else {
		using antlr::SemanticException;
		assert(scope);
		n = boost::intrusive_ptr<openvrml::node>(
		scope->find_node(id1->getText()));
		if (!n) {
		throw SemanticException("node \"" + id1->getText()
		+ "\" has not been defined in "
		+ "this scope",
		this->getFilename(),
		id1->getLine(),
		id1->getColumn());
		}
		}
		
#line 504 "X3DVrmlParser.cpp"
		break;
	}
	case ID:
	{
		n=node(scene, scope, std::string());
		break;
	}
	default:
	{
		throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
	}
	}
	return n;
}

void X3DVrmlParser::protoStatement(
	const openvrml::scene & scene,
               const boost::shared_ptr<openvrml::scope> & scope
) {
	
	switch ( LA(1)) {
	case KEYWORD_EXTERNPROTO:
	{
		externproto(scene, scope);
		break;
	}
	case KEYWORD_PROTO:
	{
		proto(scene, scope);
		break;
	}
	default:
	{
		throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
	}
	}
}

void X3DVrmlParser::routeStatement(
	const openvrml::scope & scope
) {
	ANTLR_USE_NAMESPACE(antlr)RefToken  from_node_id = ANTLR_USE_NAMESPACE(antlr)nullToken;
	ANTLR_USE_NAMESPACE(antlr)RefToken  eventout_id = ANTLR_USE_NAMESPACE(antlr)nullToken;
	ANTLR_USE_NAMESPACE(antlr)RefToken  to_node_id = ANTLR_USE_NAMESPACE(antlr)nullToken;
	ANTLR_USE_NAMESPACE(antlr)RefToken  eventin_id = ANTLR_USE_NAMESPACE(antlr)nullToken;
	
	match(KEYWORD_ROUTE);
	from_node_id = LT(1);
	match(ID);
	match(PERIOD);
	eventout_id = LT(1);
	match(ID);
	match(KEYWORD_TO);
	to_node_id = LT(1);
	match(ID);
	match(PERIOD);
	eventin_id = LT(1);
	match(ID);
#line 1076 "expandedX3DVrmlParser.g"
	
	using std::runtime_error;
	using antlr::SemanticException;
	
	openvrml::node * const from_node =
	scope.find_node(from_node_id->getText());
	if (!from_node) {
	throw SemanticException("node \"" + from_node_id->getText()
	+ "\" has not been defined in this "
	"scope",
	this->getFilename(),
	from_node_id->getLine(),
	from_node_id->getColumn());
	}
	
	openvrml::node * const to_node =
	scope.find_node(to_node_id->getText());
	if (!to_node) {
	throw SemanticException("node \"" + to_node_id->getText()
	+ "\" has not been defined in this "
	"scope",
	this->getFilename(),
	to_node_id->getLine(),
	to_node_id->getColumn());
	}
	
	try {
	//
	// Redundant routing is ignored (4.10.2), so we don't care
	// if add_route returns false.
	//
	add_route(*from_node, eventout_id->getText(),
	*to_node, eventin_id->getText());
	} catch (runtime_error & ex) {
	throw SemanticException(ex.what(),
	this->getFilename(),
	from_node_id->getLine(),
	from_node_id->getColumn());
	}
	
#line 604 "X3DVrmlParser.cpp"
}

void X3DVrmlParser::importStatement() {
	
	match(KEYWORD_IMPORT);
	match(ID);
	match(PERIOD);
	match(ID);
	match(KEYWORD_AS);
	match(ID);
}

void X3DVrmlParser::exportStatement() {
	
	match(KEYWORD_EXPORT);
	match(ID);
	match(KEYWORD_AS);
	match(ID);
}

openvrml::field_value::type_id  X3DVrmlParser::fieldType() {
#line 344 "expandedX3DVrmlParser.g"
	openvrml::field_value::type_id ft =
         openvrml::field_value::invalid_type_id;
#line 629 "X3DVrmlParser.cpp"
#line 344 "expandedX3DVrmlParser.g"
	
	using openvrml::field_value;
	
#line 634 "X3DVrmlParser.cpp"
	
	switch ( LA(1)) {
	case FIELDTYPE_MFBOOL:
	{
		match(FIELDTYPE_MFBOOL);
#line 352 "expandedX3DVrmlParser.g"
		ft = field_value::mfbool_id;
#line 642 "X3DVrmlParser.cpp"
		break;
	}
	case FIELDTYPE_MFCOLOR:
	{
		match(FIELDTYPE_MFCOLOR);
#line 353 "expandedX3DVrmlParser.g"
		ft = field_value::mfcolor_id;
#line 650 "X3DVrmlParser.cpp"
		break;
	}
	case FIELDTYPE_MFCOLORRGBA:
	{
		match(FIELDTYPE_MFCOLORRGBA);
#line 354 "expandedX3DVrmlParser.g"
		ft = field_value::mfcolorrgba_id;
#line 658 "X3DVrmlParser.cpp"
		break;
	}
	case FIELDTYPE_MFFLOAT:
	{
		match(FIELDTYPE_MFFLOAT);
#line 355 "expandedX3DVrmlParser.g"
		ft = field_value::mffloat_id;
#line 666 "X3DVrmlParser.cpp"
		break;
	}
	case FIELDTYPE_MFDOUBLE:
	{
		match(FIELDTYPE_MFDOUBLE);
#line 356 "expandedX3DVrmlParser.g"
		ft = field_value::mfdouble_id;
#line 674 "X3DVrmlParser.cpp"
		break;
	}
	case FIELDTYPE_MFIMAGE:
	{
		match(FIELDTYPE_MFIMAGE);
#line 357 "expandedX3DVrmlParser.g"
		ft = field_value::mfimage_id;
#line 682 "X3DVrmlParser.cpp"
		break;
	}
	case FIELDTYPE_MFINT32:
	{
		match(FIELDTYPE_MFINT32);
#line 358 "expandedX3DVrmlParser.g"
		ft = field_value::mfint32_id;
#line 690 "X3DVrmlParser.cpp"
		break;
	}
	case FIELDTYPE_MFNODE:
	{
		match(FIELDTYPE_MFNODE);
#line 359 "expandedX3DVrmlParser.g"
		ft = field_value::mfnode_id;
#line 698 "X3DVrmlParser.cpp"
		break;
	}
	case FIELDTYPE_MFROTATION:
	{
		match(FIELDTYPE_MFROTATION);
#line 360 "expandedX3DVrmlParser.g"
		ft = field_value::mfrotation_id;
#line 706 "X3DVrmlParser.cpp"
		break;
	}
	case FIELDTYPE_MFSTRING:
	{
		match(FIELDTYPE_MFSTRING);
#line 361 "expandedX3DVrmlParser.g"
		ft = field_value::mfstring_id;
#line 714 "X3DVrmlParser.cpp"
		break;
	}
	case FIELDTYPE_MFTIME:
	{
		match(FIELDTYPE_MFTIME);
#line 362 "expandedX3DVrmlParser.g"
		ft = field_value::mftime_id;
#line 722 "X3DVrmlParser.cpp"
		break;
	}
	case FIELDTYPE_MFVEC2F:
	{
		match(FIELDTYPE_MFVEC2F);
#line 363 "expandedX3DVrmlParser.g"
		ft = field_value::mfvec2f_id;
#line 730 "X3DVrmlParser.cpp"
		break;
	}
	case FIELDTYPE_MFVEC2D:
	{
		match(FIELDTYPE_MFVEC2D);
#line 364 "expandedX3DVrmlParser.g"
		ft = field_value::mfvec2d_id;
#line 738 "X3DVrmlParser.cpp"
		break;
	}
	case FIELDTYPE_MFVEC3F:
	{
		match(FIELDTYPE_MFVEC3F);
#line 365 "expandedX3DVrmlParser.g"
		ft = field_value::mfvec3f_id;
#line 746 "X3DVrmlParser.cpp"
		break;
	}
	case FIELDTYPE_MFVEC3D:
	{
		match(FIELDTYPE_MFVEC3D);
#line 366 "expandedX3DVrmlParser.g"
		ft = field_value::mfvec3d_id;
#line 754 "X3DVrmlParser.cpp"
		break;
	}
	case FIELDTYPE_SFBOOL:
	{
		match(FIELDTYPE_SFBOOL);
#line 367 "expandedX3DVrmlParser.g"
		ft = field_value::sfbool_id;
#line 762 "X3DVrmlParser.cpp"
		break;
	}
	case FIELDTYPE_SFCOLOR:
	{
		match(FIELDTYPE_SFCOLOR);
#line 368 "expandedX3DVrmlParser.g"
		ft = field_value::sfcolor_id;
#line 770 "X3DVrmlParser.cpp"
		break;
	}
	case FIELDTYPE_SFCOLORRGBA:
	{
		match(FIELDTYPE_SFCOLORRGBA);
#line 369 "expandedX3DVrmlParser.g"
		ft = field_value::sfcolorrgba_id;
#line 778 "X3DVrmlParser.cpp"
		break;
	}
	case FIELDTYPE_SFFLOAT:
	{
		match(FIELDTYPE_SFFLOAT);
#line 370 "expandedX3DVrmlParser.g"
		ft = field_value::sffloat_id;
#line 786 "X3DVrmlParser.cpp"
		break;
	}
	case FIELDTYPE_SFDOUBLE:
	{
		match(FIELDTYPE_SFDOUBLE);
#line 371 "expandedX3DVrmlParser.g"
		ft = field_value::sfdouble_id;
#line 794 "X3DVrmlParser.cpp"
		break;
	}
	case FIELDTYPE_SFIMAGE:
	{
		match(FIELDTYPE_SFIMAGE);
#line 372 "expandedX3DVrmlParser.g"
		ft = field_value::sfimage_id;
#line 802 "X3DVrmlParser.cpp"
		break;
	}
	case FIELDTYPE_SFINT32:
	{
		match(FIELDTYPE_SFINT32);
#line 373 "expandedX3DVrmlParser.g"
		ft = field_value::sfint32_id;
#line 810 "X3DVrmlParser.cpp"
		break;
	}
	case FIELDTYPE_SFNODE:
	{
		match(FIELDTYPE_SFNODE);
#line 374 "expandedX3DVrmlParser.g"
		ft = field_value::sfnode_id;
#line 818 "X3DVrmlParser.cpp"
		break;
	}
	case FIELDTYPE_SFROTATION:
	{
		match(FIELDTYPE_SFROTATION);
#line 375 "expandedX3DVrmlParser.g"
		ft = field_value::sfrotation_id;
#line 826 "X3DVrmlParser.cpp"
		break;
	}
	case FIELDTYPE_SFSTRING:
	{
		match(FIELDTYPE_SFSTRING);
#line 376 "expandedX3DVrmlParser.g"
		ft = field_value::sfstring_id;
#line 834 "X3DVrmlParser.cpp"
		break;
	}
	case FIELDTYPE_SFTIME:
	{
		match(FIELDTYPE_SFTIME);
#line 377 "expandedX3DVrmlParser.g"
		ft = field_value::sftime_id;
#line 842 "X3DVrmlParser.cpp"
		break;
	}
	case FIELDTYPE_SFVEC2F:
	{
		match(FIELDTYPE_SFVEC2F);
#line 378 "expandedX3DVrmlParser.g"
		ft = field_value::sfvec2f_id;
#line 850 "X3DVrmlParser.cpp"
		break;
	}
	case FIELDTYPE_SFVEC2D:
	{
		match(FIELDTYPE_SFVEC2D);
#line 379 "expandedX3DVrmlParser.g"
		ft = field_value::sfvec2d_id;
#line 858 "X3DVrmlParser.cpp"
		break;
	}
	case FIELDTYPE_SFVEC3F:
	{
		match(FIELDTYPE_SFVEC3F);
#line 380 "expandedX3DVrmlParser.g"
		ft = field_value::sfvec3f_id;
#line 866 "X3DVrmlParser.cpp"
		break;
	}
	case FIELDTYPE_SFVEC3D:
	{
		match(FIELDTYPE_SFVEC3D);
#line 381 "expandedX3DVrmlParser.g"
		ft = field_value::sfvec3d_id;
#line 874 "X3DVrmlParser.cpp"
		break;
	}
	default:
	{
		throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
	}
	}
	return ft;
}

boost::shared_ptr<field_value>  X3DVrmlParser::nonNodeFieldValue(
	openvrml::field_value::type_id ft
) {
#line 384 "expandedX3DVrmlParser.g"
	boost::shared_ptr<field_value> fv;
#line 890 "X3DVrmlParser.cpp"
	
	if (((LA(1) == KEYWORD_FALSE || LA(1) == KEYWORD_TRUE))&&( ft == field_value::sfbool_id )) {
		fv=sfBoolValue();
	}
	else if (((LA(1) == INTEGER || LA(1) == REAL))&&( ft == field_value::sfcolor_id )) {
		fv=sfColorValue();
	}
	else if (((LA(1) == INTEGER || LA(1) == REAL))&&( ft == field_value::sfcolorrgba_id )) {
		fv=sfColorRgbaValue();
	}
	else if (((LA(1) == INTEGER || LA(1) == REAL))&&( ft == field_value::sffloat_id )) {
		fv=sfFloatValue();
	}
	else if (((LA(1) == INTEGER || LA(1) == REAL))&&( ft == field_value::sfdouble_id )) {
		fv=sfDoubleValue();
	}
	else if (((LA(1) == INTEGER || LA(1) == HEX_INTEGER))&&( ft == field_value::sfimage_id )) {
		fv=sfImageValue();
	}
	else if (((LA(1) == INTEGER || LA(1) == HEX_INTEGER))&&( ft == field_value::sfint32_id )) {
		fv=sfInt32Value();
	}
	else if (((LA(1) == INTEGER || LA(1) == REAL))&&( ft == field_value::sfrotation_id )) {
		fv=sfRotationValue();
	}
	else if (((LA(1) == STRING))&&( ft == field_value::sfstring_id )) {
		fv=sfStringValue();
	}
	else if (((LA(1) == INTEGER || LA(1) == REAL))&&( ft == field_value::sftime_id )) {
		fv=sfTimeValue();
	}
	else if (((LA(1) == INTEGER || LA(1) == REAL))&&( ft == field_value::sfvec2f_id )) {
		fv=sfVec2fValue();
	}
	else if (((LA(1) == INTEGER || LA(1) == REAL))&&( ft == field_value::sfvec2d_id )) {
		fv=sfVec2dValue();
	}
	else if (((LA(1) == INTEGER || LA(1) == REAL))&&( ft == field_value::sfvec3f_id )) {
		fv=sfVec3fValue();
	}
	else if (((LA(1) == INTEGER || LA(1) == REAL))&&( ft == field_value::sfvec3d_id )) {
		fv=sfVec3dValue();
	}
	else if (((LA(1) == LBRACKET || LA(1) == KEYWORD_FALSE || LA(1) == KEYWORD_TRUE))&&( ft == field_value::mfbool_id )) {
		fv=mfBoolValue();
	}
	else if (((LA(1) == LBRACKET || LA(1) == INTEGER || LA(1) == REAL))&&( ft == field_value::mfcolor_id )) {
		fv=mfColorValue();
	}
	else if (((LA(1) == LBRACKET || LA(1) == INTEGER || LA(1) == REAL))&&( ft == field_value::mfcolorrgba_id )) {
		fv=mfColorRgbaValue();
	}
	else if (((LA(1) == LBRACKET || LA(1) == INTEGER || LA(1) == REAL))&&( ft == field_value::mffloat_id )) {
		fv=mfFloatValue();
	}
	else if (((LA(1) == LBRACKET || LA(1) == INTEGER || LA(1) == REAL))&&( ft == field_value::mfdouble_id )) {
		fv=mfDoubleValue();
	}
	else if (((LA(1) == LBRACKET || LA(1) == INTEGER || LA(1) == HEX_INTEGER))&&( ft == field_value::mfimage_id )) {
		fv=mfImageValue();
	}
	else if (((LA(1) == LBRACKET || LA(1) == INTEGER || LA(1) == HEX_INTEGER))&&( ft == field_value::mfint32_id )) {
		fv=mfInt32Value();
	}
	else if (((LA(1) == LBRACKET || LA(1) == INTEGER || LA(1) == REAL))&&( ft == field_value::mfrotation_id )) {
		fv=mfRotationValue();
	}
	else if (((LA(1) == LBRACKET || LA(1) == STRING))&&( ft == field_value::mfstring_id )) {
		fv=mfStringValue();
	}
	else if (((LA(1) == LBRACKET || LA(1) == INTEGER || LA(1) == REAL))&&( ft == field_value::mftime_id )) {
		fv=mfTimeValue();
	}
	else if (((LA(1) == LBRACKET || LA(1) == INTEGER || LA(1) == REAL))&&( ft == field_value::mfvec2f_id )) {
		fv=mfVec2fValue();
	}
	else if (((LA(1) == LBRACKET || LA(1) == INTEGER || LA(1) == REAL))&&( ft == field_value::mfvec2d_id )) {
		fv=mfVec2dValue();
	}
	else if (((LA(1) == LBRACKET || LA(1) == INTEGER || LA(1) == REAL))&&( ft == field_value::mfvec3f_id )) {
		fv=mfVec3fValue();
	}
	else if (((LA(1) == LBRACKET || LA(1) == INTEGER || LA(1) == REAL))&&( ft == field_value::mfvec3d_id )) {
		fv=mfVec3dValue();
	}
	else {
		throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
	}
	
	return fv;
}

boost::shared_ptr<field_value>  X3DVrmlParser::sfBoolValue() {
#line 1736 "expandedX3DVrmlParser.g"
	boost::shared_ptr<field_value> sbv;
#line 986 "X3DVrmlParser.cpp"
#line 1736 "expandedX3DVrmlParser.g"
	
	bool val(false);
	
#line 991 "X3DVrmlParser.cpp"
	
	val=boolValue();
#line 1743 "expandedX3DVrmlParser.g"
	sbv.reset(new sfbool(val));
#line 996 "X3DVrmlParser.cpp"
	return sbv;
}

boost::shared_ptr<field_value>  X3DVrmlParser::sfColorValue() {
#line 1756 "expandedX3DVrmlParser.g"
	boost::shared_ptr<field_value> scv;
#line 1003 "X3DVrmlParser.cpp"
#line 1756 "expandedX3DVrmlParser.g"
	
	color c;
	
#line 1008 "X3DVrmlParser.cpp"
	
	colorValue(c);
#line 1763 "expandedX3DVrmlParser.g"
	scv.reset(new sfcolor(c));
#line 1013 "X3DVrmlParser.cpp"
	return scv;
}

boost::shared_ptr<field_value>  X3DVrmlParser::sfColorRgbaValue() {
#line 437 "expandedX3DVrmlParser.g"
	boost::shared_ptr<field_value> scv;
#line 1020 "X3DVrmlParser.cpp"
#line 437 "expandedX3DVrmlParser.g"
	
	color_rgba c;
	
#line 1025 "X3DVrmlParser.cpp"
	
	colorRgbaValue(c);
#line 444 "expandedX3DVrmlParser.g"
	scv.reset(new sfcolorrgba(c));
#line 1030 "X3DVrmlParser.cpp"
	return scv;
}

boost::shared_ptr<field_value>  X3DVrmlParser::sfFloatValue() {
#line 1818 "expandedX3DVrmlParser.g"
	boost::shared_ptr<field_value> sfv;
#line 1037 "X3DVrmlParser.cpp"
#line 1818 "expandedX3DVrmlParser.g"
	
	float f;
	
#line 1042 "X3DVrmlParser.cpp"
	
	f=floatValue();
#line 1825 "expandedX3DVrmlParser.g"
	sfv.reset(new sffloat(f));
#line 1047 "X3DVrmlParser.cpp"
	return sfv;
}

boost::shared_ptr<field_value>  X3DVrmlParser::sfDoubleValue() {
#line 501 "expandedX3DVrmlParser.g"
	boost::shared_ptr<field_value> sfv;
#line 1054 "X3DVrmlParser.cpp"
#line 501 "expandedX3DVrmlParser.g"
	
	double d;
	
#line 1059 "X3DVrmlParser.cpp"
	
	d=doubleValue();
#line 508 "expandedX3DVrmlParser.g"
	sfv.reset(new sfdouble(d));
#line 1064 "X3DVrmlParser.cpp"
	return sfv;
}

boost::shared_ptr<field_value>  X3DVrmlParser::sfImageValue() {
#line 1858 "expandedX3DVrmlParser.g"
	boost::shared_ptr<field_value> siv;
#line 1071 "X3DVrmlParser.cpp"
#line 1858 "expandedX3DVrmlParser.g"
	
	image img;
	
#line 1076 "X3DVrmlParser.cpp"
	
	imageValue(img);
#line 1865 "expandedX3DVrmlParser.g"
	
	siv.reset(new sfimage(img));
	
#line 1083 "X3DVrmlParser.cpp"
	return siv;
}

boost::shared_ptr<field_value>  X3DVrmlParser::sfInt32Value() {
#line 1896 "expandedX3DVrmlParser.g"
	boost::shared_ptr<field_value> siv;
#line 1090 "X3DVrmlParser.cpp"
#line 1896 "expandedX3DVrmlParser.g"
	
	long i;
	
#line 1095 "X3DVrmlParser.cpp"
	
	i=intValue();
#line 1903 "expandedX3DVrmlParser.g"
	siv.reset(new sfint32(i));
#line 1100 "X3DVrmlParser.cpp"
	return siv;
}

boost::shared_ptr<field_value>  X3DVrmlParser::sfRotationValue() {
#line 2051 "expandedX3DVrmlParser.g"
	boost::shared_ptr<field_value> srv;
#line 1107 "X3DVrmlParser.cpp"
#line 2051 "expandedX3DVrmlParser.g"
	
	rotation r;
	
#line 1112 "X3DVrmlParser.cpp"
	
	rotationValue(r);
#line 2058 "expandedX3DVrmlParser.g"
	srv.reset(new sfrotation(r));
#line 1117 "X3DVrmlParser.cpp"
	return srv;
}

boost::shared_ptr<field_value>  X3DVrmlParser::sfStringValue() {
#line 2108 "expandedX3DVrmlParser.g"
	boost::shared_ptr<field_value> ssv;
#line 1124 "X3DVrmlParser.cpp"
#line 2108 "expandedX3DVrmlParser.g"
	
	std::string s;
	
#line 1129 "X3DVrmlParser.cpp"
	
	s=stringValue();
#line 2115 "expandedX3DVrmlParser.g"
	ssv.reset(new sfstring(s));
#line 1134 "X3DVrmlParser.cpp"
	return ssv;
}

boost::shared_ptr<field_value>  X3DVrmlParser::sfTimeValue() {
#line 2165 "expandedX3DVrmlParser.g"
	boost::shared_ptr<field_value> stv;
#line 1141 "X3DVrmlParser.cpp"
#line 2165 "expandedX3DVrmlParser.g"
	
	double t(0.0);
	
#line 1146 "X3DVrmlParser.cpp"
	
	t=doubleValue();
#line 2172 "expandedX3DVrmlParser.g"
	stv.reset(new sftime(t));
#line 1151 "X3DVrmlParser.cpp"
	return stv;
}

boost::shared_ptr<field_value>  X3DVrmlParser::sfVec2fValue() {
#line 2205 "expandedX3DVrmlParser.g"
	boost::shared_ptr<field_value> svv;
#line 1158 "X3DVrmlParser.cpp"
#line 2205 "expandedX3DVrmlParser.g"
	
	vec2f v;
	
#line 1163 "X3DVrmlParser.cpp"
	
	vec2fValue(v);
#line 2212 "expandedX3DVrmlParser.g"
	svv.reset(new sfvec2f(v));
#line 1168 "X3DVrmlParser.cpp"
	return svv;
}

boost::shared_ptr<field_value>  X3DVrmlParser::sfVec2dValue() {
#line 530 "expandedX3DVrmlParser.g"
	boost::shared_ptr<field_value> svv;
#line 1175 "X3DVrmlParser.cpp"
#line 530 "expandedX3DVrmlParser.g"
	
	vec2d v;
	
#line 1180 "X3DVrmlParser.cpp"
	
	vec2dValue(v);
#line 537 "expandedX3DVrmlParser.g"
	svv.reset(new sfvec2d(v));
#line 1185 "X3DVrmlParser.cpp"
	return svv;
}

boost::shared_ptr<field_value>  X3DVrmlParser::sfVec3fValue() {
#line 2248 "expandedX3DVrmlParser.g"
	boost::shared_ptr<field_value> svv;
#line 1192 "X3DVrmlParser.cpp"
#line 2248 "expandedX3DVrmlParser.g"
	
	vec3f v;
	
#line 1197 "X3DVrmlParser.cpp"
	
	vec3fValue(v);
#line 2255 "expandedX3DVrmlParser.g"
	svv.reset(new sfvec3f(v));
#line 1202 "X3DVrmlParser.cpp"
	return svv;
}

boost::shared_ptr<field_value>  X3DVrmlParser::sfVec3dValue() {
#line 572 "expandedX3DVrmlParser.g"
	boost::shared_ptr<field_value> svv;
#line 1209 "X3DVrmlParser.cpp"
#line 572 "expandedX3DVrmlParser.g"
	
	vec3d v;
	
#line 1214 "X3DVrmlParser.cpp"
	
	vec3dValue(v);
#line 579 "expandedX3DVrmlParser.g"
	svv.reset(new sfvec3d(v));
#line 1219 "X3DVrmlParser.cpp"
	return svv;
}

boost::shared_ptr<field_value>  X3DVrmlParser::mfBoolValue() {
#line 418 "expandedX3DVrmlParser.g"
	boost::shared_ptr<field_value> mfv =
         boost::shared_ptr<field_value>(new mfbool);
#line 1227 "X3DVrmlParser.cpp"
#line 418 "expandedX3DVrmlParser.g"
	
	using std::vector;
	
	bool b;
	mfbool & bools = static_cast<mfbool &>(*mfv);
	
#line 1235 "X3DVrmlParser.cpp"
	
	switch ( LA(1)) {
	case KEYWORD_FALSE:
	case KEYWORD_TRUE:
	{
		b=boolValue();
#line 429 "expandedX3DVrmlParser.g"
		bools.value(vector<bool>(1, b));
#line 1244 "X3DVrmlParser.cpp"
		break;
	}
	case LBRACKET:
	{
		match(LBRACKET);
#line 430 "expandedX3DVrmlParser.g"
		
		vector<bool> value;
		
#line 1254 "X3DVrmlParser.cpp"
		{ // ( ... )*
		for (;;) {
			if ((LA(1) == KEYWORD_FALSE || LA(1) == KEYWORD_TRUE)) {
				b=boolValue();
#line 432 "expandedX3DVrmlParser.g"
				value.push_back(b);
#line 1261 "X3DVrmlParser.cpp"
			}
			else {
				goto _loop18;
			}
			
		}
		_loop18:;
		} // ( ... )*
		match(RBRACKET);
#line 432 "expandedX3DVrmlParser.g"
		
		bools.value(value);
		
#line 1275 "X3DVrmlParser.cpp"
		break;
	}
	default:
	{
		throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
	}
	}
	return mfv;
}

boost::shared_ptr<field_value>  X3DVrmlParser::mfColorValue() {
#line 1767 "expandedX3DVrmlParser.g"
	boost::shared_ptr<field_value> mcv =
            boost::shared_ptr<field_value>(new mfcolor);
#line 1290 "X3DVrmlParser.cpp"
#line 1767 "expandedX3DVrmlParser.g"
	
	using std::vector;
	
	color c;
	mfcolor & colors = static_cast<mfcolor &>(*mcv);
	
#line 1298 "X3DVrmlParser.cpp"
	
	switch ( LA(1)) {
	case INTEGER:
	case REAL:
	{
		colorValue(c);
#line 1778 "expandedX3DVrmlParser.g"
		colors.value(vector<color>(1, c));
#line 1307 "X3DVrmlParser.cpp"
		break;
	}
	case LBRACKET:
	{
		match(LBRACKET);
#line 1779 "expandedX3DVrmlParser.g"
		
		vector<color> value;
		
#line 1317 "X3DVrmlParser.cpp"
		{ // ( ... )*
		for (;;) {
			if ((LA(1) == INTEGER || LA(1) == REAL)) {
				colorValue(c);
#line 1781 "expandedX3DVrmlParser.g"
				value.push_back(c);
#line 1324 "X3DVrmlParser.cpp"
			}
			else {
				goto _loop97;
			}
			
		}
		_loop97:;
		} // ( ... )*
		match(RBRACKET);
#line 1781 "expandedX3DVrmlParser.g"
		
		colors.value(value);
		
#line 1338 "X3DVrmlParser.cpp"
		break;
	}
	default:
	{
		throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
	}
	}
	return mcv;
}

boost::shared_ptr<field_value>  X3DVrmlParser::mfColorRgbaValue() {
#line 447 "expandedX3DVrmlParser.g"
	boost::shared_ptr<field_value> mcv =
            boost::shared_ptr<field_value>(new mfcolorrgba);
#line 1353 "X3DVrmlParser.cpp"
#line 447 "expandedX3DVrmlParser.g"
	
	using std::vector;
	
	color_rgba c;
	mfcolorrgba & colors = static_cast<mfcolorrgba &>(*mcv);
	
#line 1361 "X3DVrmlParser.cpp"
	
	switch ( LA(1)) {
	case INTEGER:
	case REAL:
	{
		colorRgbaValue(c);
#line 458 "expandedX3DVrmlParser.g"
		colors.value(vector<color_rgba>(1, c));
#line 1370 "X3DVrmlParser.cpp"
		break;
	}
	case LBRACKET:
	{
		match(LBRACKET);
#line 459 "expandedX3DVrmlParser.g"
		
		vector<color_rgba> value;
		
#line 1380 "X3DVrmlParser.cpp"
		{ // ( ... )*
		for (;;) {
			if ((LA(1) == INTEGER || LA(1) == REAL)) {
				colorRgbaValue(c);
#line 461 "expandedX3DVrmlParser.g"
				value.push_back(c);
#line 1387 "X3DVrmlParser.cpp"
			}
			else {
				goto _loop22;
			}
			
		}
		_loop22:;
		} // ( ... )*
		match(RBRACKET);
#line 461 "expandedX3DVrmlParser.g"
		
		colors.value(value);
		
#line 1401 "X3DVrmlParser.cpp"
		break;
	}
	default:
	{
		throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
	}
	}
	return mcv;
}

boost::shared_ptr<field_value>  X3DVrmlParser::mfFloatValue() {
#line 1829 "expandedX3DVrmlParser.g"
	boost::shared_ptr<field_value> mfv =
            boost::shared_ptr<field_value>(new mffloat);
#line 1416 "X3DVrmlParser.cpp"
#line 1829 "expandedX3DVrmlParser.g"
	
	using std::vector;
	
	float f;
	mffloat & floats = static_cast<mffloat &>(*mfv);
	
#line 1424 "X3DVrmlParser.cpp"
	
	switch ( LA(1)) {
	case INTEGER:
	case REAL:
	{
		f=floatValue();
#line 1840 "expandedX3DVrmlParser.g"
		floats.value(vector<float>(1, f));
#line 1433 "X3DVrmlParser.cpp"
		break;
	}
	case LBRACKET:
	{
		match(LBRACKET);
#line 1841 "expandedX3DVrmlParser.g"
		
		vector<float> value;
		
#line 1443 "X3DVrmlParser.cpp"
		{ // ( ... )*
		for (;;) {
			if ((LA(1) == INTEGER || LA(1) == REAL)) {
				f=floatValue();
#line 1843 "expandedX3DVrmlParser.g"
				value.push_back(f);
#line 1450 "X3DVrmlParser.cpp"
			}
			else {
				goto _loop103;
			}
			
		}
		_loop103:;
		} // ( ... )*
		match(RBRACKET);
#line 1843 "expandedX3DVrmlParser.g"
		
		floats.value(value);
		
#line 1464 "X3DVrmlParser.cpp"
		break;
	}
	default:
	{
		throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
	}
	}
	return mfv;
}

boost::shared_ptr<field_value>  X3DVrmlParser::mfDoubleValue() {
#line 511 "expandedX3DVrmlParser.g"
	boost::shared_ptr<field_value> mfv =
         boost::shared_ptr<field_value>(new mfdouble);
#line 1479 "X3DVrmlParser.cpp"
#line 511 "expandedX3DVrmlParser.g"
	
	using std::vector;
	
	double d;
	mfdouble & doubles = static_cast<mfdouble &>(*mfv);
	
#line 1487 "X3DVrmlParser.cpp"
	
	switch ( LA(1)) {
	case INTEGER:
	case REAL:
	{
		d=doubleValue();
#line 522 "expandedX3DVrmlParser.g"
		doubles.value(vector<double>(1, d));
#line 1496 "X3DVrmlParser.cpp"
		break;
	}
	case LBRACKET:
	{
		match(LBRACKET);
#line 523 "expandedX3DVrmlParser.g"
		
		vector<double> value;
		
#line 1506 "X3DVrmlParser.cpp"
		{ // ( ... )*
		for (;;) {
			if ((LA(1) == INTEGER || LA(1) == REAL)) {
				d=doubleValue();
#line 525 "expandedX3DVrmlParser.g"
				value.push_back(d);
#line 1513 "X3DVrmlParser.cpp"
			}
			else {
				goto _loop30;
			}
			
		}
		_loop30:;
		} // ( ... )*
		match(RBRACKET);
#line 525 "expandedX3DVrmlParser.g"
		
		doubles.value(value);
		
#line 1527 "X3DVrmlParser.cpp"
		break;
	}
	default:
	{
		throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
	}
	}
	return mfv;
}

boost::shared_ptr<field_value>  X3DVrmlParser::mfImageValue() {
#line 481 "expandedX3DVrmlParser.g"
	boost::shared_ptr<field_value> mcv =
            boost::shared_ptr<field_value>(new mfimage);
#line 1542 "X3DVrmlParser.cpp"
#line 481 "expandedX3DVrmlParser.g"
	
	using std::vector;
	
	image img;
	mfimage & imgs = static_cast<mfimage &>(*mcv);
	
#line 1550 "X3DVrmlParser.cpp"
	
	switch ( LA(1)) {
	case INTEGER:
	case HEX_INTEGER:
	{
		imageValue(img);
#line 492 "expandedX3DVrmlParser.g"
		imgs.value(vector<image>(1, img));
#line 1559 "X3DVrmlParser.cpp"
		break;
	}
	case LBRACKET:
	{
		match(LBRACKET);
#line 493 "expandedX3DVrmlParser.g"
		
		vector<image> value;
		
#line 1569 "X3DVrmlParser.cpp"
		{ // ( ... )*
		for (;;) {
			if ((LA(1) == INTEGER || LA(1) == HEX_INTEGER)) {
				imageValue(img);
#line 495 "expandedX3DVrmlParser.g"
				value.push_back(img);
#line 1576 "X3DVrmlParser.cpp"
			}
			else {
				goto _loop26;
			}
			
		}
		_loop26:;
		} // ( ... )*
		match(RBRACKET);
#line 496 "expandedX3DVrmlParser.g"
		
		imgs.value(value);
		
#line 1590 "X3DVrmlParser.cpp"
		break;
	}
	default:
	{
		throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
	}
	}
	return mcv;
}

boost::shared_ptr<field_value>  X3DVrmlParser::mfInt32Value() {
#line 1907 "expandedX3DVrmlParser.g"
	boost::shared_ptr<field_value> miv =
            boost::shared_ptr<field_value>(new mfint32);
#line 1605 "X3DVrmlParser.cpp"
#line 1907 "expandedX3DVrmlParser.g"
	
	using std::vector;
	
	long i;
	mfint32 & int32s = static_cast<mfint32 &>(*miv);
	
#line 1613 "X3DVrmlParser.cpp"
	
	switch ( LA(1)) {
	case INTEGER:
	case HEX_INTEGER:
	{
		i=intValue();
#line 1918 "expandedX3DVrmlParser.g"
		int32s.value(vector<int32>(1, i));
#line 1622 "X3DVrmlParser.cpp"
		break;
	}
	case LBRACKET:
	{
		match(LBRACKET);
#line 1919 "expandedX3DVrmlParser.g"
		
		vector<int32> value;
		
#line 1632 "X3DVrmlParser.cpp"
		{ // ( ... )*
		for (;;) {
			if ((LA(1) == INTEGER || LA(1) == HEX_INTEGER)) {
				i=intValue();
#line 1921 "expandedX3DVrmlParser.g"
				value.push_back(i);
#line 1639 "X3DVrmlParser.cpp"
			}
			else {
				goto _loop111;
			}
			
		}
		_loop111:;
		} // ( ... )*
		match(RBRACKET);
#line 1921 "expandedX3DVrmlParser.g"
		
		int32s.value(value);
		
#line 1653 "X3DVrmlParser.cpp"
		break;
	}
	default:
	{
		throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
	}
	}
	return miv;
}

boost::shared_ptr<field_value>  X3DVrmlParser::mfRotationValue() {
#line 2062 "expandedX3DVrmlParser.g"
	boost::shared_ptr<field_value> mrv =
         boost::shared_ptr<field_value>(new mfrotation);
#line 1668 "X3DVrmlParser.cpp"
#line 2062 "expandedX3DVrmlParser.g"
	
	using std::vector;
	
	rotation r;
	mfrotation & rotations = static_cast<mfrotation &>(*mrv);
	
#line 1676 "X3DVrmlParser.cpp"
	
	switch ( LA(1)) {
	case INTEGER:
	case REAL:
	{
		rotationValue(r);
#line 2073 "expandedX3DVrmlParser.g"
		rotations.value(vector<rotation>(1, r));
#line 1685 "X3DVrmlParser.cpp"
		break;
	}
	case LBRACKET:
	{
		match(LBRACKET);
#line 2074 "expandedX3DVrmlParser.g"
		
		vector<rotation> value;
		
#line 1695 "X3DVrmlParser.cpp"
		{ // ( ... )*
		for (;;) {
			if ((LA(1) == INTEGER || LA(1) == REAL)) {
				rotationValue(r);
#line 2076 "expandedX3DVrmlParser.g"
				value.push_back(r);
#line 1702 "X3DVrmlParser.cpp"
			}
			else {
				goto _loop124;
			}
			
		}
		_loop124:;
		} // ( ... )*
		match(RBRACKET);
#line 2076 "expandedX3DVrmlParser.g"
		
		rotations.value(value);
		
#line 1716 "X3DVrmlParser.cpp"
		break;
	}
	default:
	{
		throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
	}
	}
	return mrv;
}

boost::shared_ptr<field_value>  X3DVrmlParser::mfStringValue() {
#line 2119 "expandedX3DVrmlParser.g"
	boost::shared_ptr<field_value> msv =
         boost::shared_ptr<field_value>(new mfstring);
#line 1731 "X3DVrmlParser.cpp"
#line 2119 "expandedX3DVrmlParser.g"
	
	using std::string;
	using std::vector;
	
	string s;
	mfstring & strings = static_cast<mfstring &>(*msv);
	
#line 1740 "X3DVrmlParser.cpp"
	
	switch ( LA(1)) {
	case STRING:
	{
		s=stringValue();
#line 2131 "expandedX3DVrmlParser.g"
		strings.value(vector<string>(1, s));
#line 1748 "X3DVrmlParser.cpp"
		break;
	}
	case LBRACKET:
	{
		match(LBRACKET);
#line 2132 "expandedX3DVrmlParser.g"
		
		vector<string> value;
		
#line 1758 "X3DVrmlParser.cpp"
		{ // ( ... )*
		for (;;) {
			if ((LA(1) == STRING)) {
				s=stringValue();
#line 2134 "expandedX3DVrmlParser.g"
				value.push_back(s);
#line 1765 "X3DVrmlParser.cpp"
			}
			else {
				goto _loop129;
			}
			
		}
		_loop129:;
		} // ( ... )*
		match(RBRACKET);
#line 2134 "expandedX3DVrmlParser.g"
		
		strings.value(value);
		
#line 1779 "X3DVrmlParser.cpp"
		break;
	}
	default:
	{
		throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
	}
	}
	return msv;
}

boost::shared_ptr<field_value>  X3DVrmlParser::mfTimeValue() {
#line 2176 "expandedX3DVrmlParser.g"
	boost::shared_ptr<field_value> mtv =
         boost::shared_ptr<field_value>(new mftime);
#line 1794 "X3DVrmlParser.cpp"
#line 2176 "expandedX3DVrmlParser.g"
	
	using std::vector;
	
	double t;
	mftime & times = static_cast<mftime &>(*mtv);
	
#line 1802 "X3DVrmlParser.cpp"
	
	switch ( LA(1)) {
	case INTEGER:
	case REAL:
	{
		t=doubleValue();
#line 2187 "expandedX3DVrmlParser.g"
		times.value(vector<double>(1, t));
#line 1811 "X3DVrmlParser.cpp"
		break;
	}
	case LBRACKET:
	{
		match(LBRACKET);
#line 2188 "expandedX3DVrmlParser.g"
		
		vector<double> value;
		
#line 1821 "X3DVrmlParser.cpp"
		{ // ( ... )*
		for (;;) {
			if ((LA(1) == INTEGER || LA(1) == REAL)) {
				t=doubleValue();
#line 2190 "expandedX3DVrmlParser.g"
				value.push_back(t);
#line 1828 "X3DVrmlParser.cpp"
			}
			else {
				goto _loop134;
			}
			
		}
		_loop134:;
		} // ( ... )*
		match(RBRACKET);
#line 2190 "expandedX3DVrmlParser.g"
		
		times.value(value);
		
#line 1842 "X3DVrmlParser.cpp"
		break;
	}
	default:
	{
		throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
	}
	}
	return mtv;
}

boost::shared_ptr<field_value>  X3DVrmlParser::mfVec2fValue() {
#line 2216 "expandedX3DVrmlParser.g"
	boost::shared_ptr<field_value> mvv =
         boost::shared_ptr<field_value>(new mfvec2f);
#line 1857 "X3DVrmlParser.cpp"
#line 2216 "expandedX3DVrmlParser.g"
	
	using std::vector;
	
	vec2f v;
	mfvec2f & vec2fs = static_cast<mfvec2f &>(*mvv);
	
#line 1865 "X3DVrmlParser.cpp"
	
	switch ( LA(1)) {
	case INTEGER:
	case REAL:
	{
		vec2fValue(v);
#line 2227 "expandedX3DVrmlParser.g"
		vec2fs.value(vector<vec2f>(1, v));
#line 1874 "X3DVrmlParser.cpp"
		break;
	}
	case LBRACKET:
	{
		match(LBRACKET);
#line 2228 "expandedX3DVrmlParser.g"
		
		vector<vec2f> value;
		
#line 1884 "X3DVrmlParser.cpp"
		{ // ( ... )*
		for (;;) {
			if ((LA(1) == INTEGER || LA(1) == REAL)) {
				vec2fValue(v);
#line 2230 "expandedX3DVrmlParser.g"
				value.push_back(v);
#line 1891 "X3DVrmlParser.cpp"
			}
			else {
				goto _loop139;
			}
			
		}
		_loop139:;
		} // ( ... )*
		match(RBRACKET);
#line 2230 "expandedX3DVrmlParser.g"
		
		vec2fs.value(value);
		
#line 1905 "X3DVrmlParser.cpp"
		break;
	}
	default:
	{
		throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
	}
	}
	return mvv;
}

boost::shared_ptr<field_value>  X3DVrmlParser::mfVec2dValue() {
#line 540 "expandedX3DVrmlParser.g"
	boost::shared_ptr<field_value> mvv =
         boost::shared_ptr<field_value>(new mfvec2d);
#line 1920 "X3DVrmlParser.cpp"
#line 540 "expandedX3DVrmlParser.g"
	
	using std::vector;
	
	vec2d v;
	mfvec2d & vec2ds = static_cast<mfvec2d &>(*mvv);
	
#line 1928 "X3DVrmlParser.cpp"
	
	switch ( LA(1)) {
	case INTEGER:
	case REAL:
	{
		vec2dValue(v);
#line 551 "expandedX3DVrmlParser.g"
		vec2ds.value(vector<vec2d>(1, v));
#line 1937 "X3DVrmlParser.cpp"
		break;
	}
	case LBRACKET:
	{
		match(LBRACKET);
#line 552 "expandedX3DVrmlParser.g"
		
		vector<vec2d> value;
		
#line 1947 "X3DVrmlParser.cpp"
		{ // ( ... )*
		for (;;) {
			if ((LA(1) == INTEGER || LA(1) == REAL)) {
				vec2dValue(v);
#line 554 "expandedX3DVrmlParser.g"
				value.push_back(v);
#line 1954 "X3DVrmlParser.cpp"
			}
			else {
				goto _loop34;
			}
			
		}
		_loop34:;
		} // ( ... )*
		match(RBRACKET);
#line 554 "expandedX3DVrmlParser.g"
		
		vec2ds.value(value);
		
#line 1968 "X3DVrmlParser.cpp"
		break;
	}
	default:
	{
		throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
	}
	}
	return mvv;
}

boost::shared_ptr<field_value>  X3DVrmlParser::mfVec3fValue() {
#line 2259 "expandedX3DVrmlParser.g"
	boost::shared_ptr<field_value> mvv =
         boost::shared_ptr<field_value>(new mfvec3f);
#line 1983 "X3DVrmlParser.cpp"
#line 2259 "expandedX3DVrmlParser.g"
	
	using std::vector;
	
	vec3f v;
	mfvec3f & vec3fs = static_cast<mfvec3f &>(*mvv);
	
#line 1991 "X3DVrmlParser.cpp"
	
	switch ( LA(1)) {
	case INTEGER:
	case REAL:
	{
		vec3fValue(v);
#line 2270 "expandedX3DVrmlParser.g"
		vec3fs.value(vector<vec3f>(1, v));
#line 2000 "X3DVrmlParser.cpp"
		break;
	}
	case LBRACKET:
	{
		match(LBRACKET);
#line 2271 "expandedX3DVrmlParser.g"
		
		vector<vec3f> value;
		
#line 2010 "X3DVrmlParser.cpp"
		{ // ( ... )*
		for (;;) {
			if ((LA(1) == INTEGER || LA(1) == REAL)) {
				vec3fValue(v);
#line 2273 "expandedX3DVrmlParser.g"
				value.push_back(v);
#line 2017 "X3DVrmlParser.cpp"
			}
			else {
				goto _loop144;
			}
			
		}
		_loop144:;
		} // ( ... )*
		match(RBRACKET);
#line 2273 "expandedX3DVrmlParser.g"
		
		vec3fs.value(value);
		
#line 2031 "X3DVrmlParser.cpp"
		break;
	}
	default:
	{
		throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
	}
	}
	return mvv;
}

boost::shared_ptr<field_value>  X3DVrmlParser::mfVec3dValue() {
#line 582 "expandedX3DVrmlParser.g"
	boost::shared_ptr<field_value> mvv =
         boost::shared_ptr<field_value>(new mfvec3d);
#line 2046 "X3DVrmlParser.cpp"
#line 582 "expandedX3DVrmlParser.g"
	
	using std::vector;
	
	vec3d v;
	mfvec3d & vec3ds = static_cast<mfvec3d &>(*mvv);
	
#line 2054 "X3DVrmlParser.cpp"
	
	switch ( LA(1)) {
	case INTEGER:
	case REAL:
	{
		vec3dValue(v);
#line 593 "expandedX3DVrmlParser.g"
		vec3ds.value(vector<vec3d>(1, v));
#line 2063 "X3DVrmlParser.cpp"
		break;
	}
	case LBRACKET:
	{
		match(LBRACKET);
#line 594 "expandedX3DVrmlParser.g"
		
		vector<vec3d> value;
		
#line 2073 "X3DVrmlParser.cpp"
		{ // ( ... )*
		for (;;) {
			if ((LA(1) == INTEGER || LA(1) == REAL)) {
				vec3dValue(v);
#line 596 "expandedX3DVrmlParser.g"
				value.push_back(v);
#line 2080 "X3DVrmlParser.cpp"
			}
			else {
				goto _loop39;
			}
			
		}
		_loop39:;
		} // ( ... )*
		match(RBRACKET);
#line 596 "expandedX3DVrmlParser.g"
		
		vec3ds.value(value);
		
#line 2094 "X3DVrmlParser.cpp"
		break;
	}
	default:
	{
		throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
	}
	}
	return mvv;
}

bool  X3DVrmlParser::boolValue() {
#line 1747 "expandedX3DVrmlParser.g"
	bool val = false;
#line 2108 "X3DVrmlParser.cpp"
	
	switch ( LA(1)) {
	case KEYWORD_TRUE:
	{
		match(KEYWORD_TRUE);
#line 1751 "expandedX3DVrmlParser.g"
		val = true;
#line 2116 "X3DVrmlParser.cpp"
		break;
	}
	case KEYWORD_FALSE:
	{
		match(KEYWORD_FALSE);
#line 1752 "expandedX3DVrmlParser.g"
		val = false;
#line 2124 "X3DVrmlParser.cpp"
		break;
	}
	default:
	{
		throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
	}
	}
	return val;
}

void X3DVrmlParser::colorRgbaValue(
	color_rgba & c
) {
#line 466 "expandedX3DVrmlParser.g"
	
	float r, g, b, a;
	
#line 2142 "X3DVrmlParser.cpp"
	
	r=colorComponent();
	g=colorComponent();
	b=colorComponent();
	a=colorComponent();
#line 473 "expandedX3DVrmlParser.g"
	
	c.r(r);
	c.g(g);
	c.b(b);
	c.a(a);
	
#line 2155 "X3DVrmlParser.cpp"
}

float  X3DVrmlParser::colorComponent() {
#line 1800 "expandedX3DVrmlParser.g"
	float val = 0.0f;
#line 2161 "X3DVrmlParser.cpp"
	
	val=floatValue();
#line 1804 "expandedX3DVrmlParser.g"
	
	if (val < 0.0 || val > 1.0) {
	this->reportWarning(
	"color component values must be from 0 to 1");
	if (val < 0.0) {
	val = 0.0;
	} else if (val > 1.0) {
	val = 1.0;
	}
	}
	
#line 2176 "X3DVrmlParser.cpp"
	return val;
}

void X3DVrmlParser::imageValue(
	image & img
) {
#line 1871 "expandedX3DVrmlParser.g"
	
	using antlr::SemanticException;
	size_t x, y, comp;
	int32 pixel;
	
#line 2189 "X3DVrmlParser.cpp"
	
	x=intValue();
	y=intValue();
	comp=intValue();
#line 1880 "expandedX3DVrmlParser.g"
	
	img.comp(comp);
	img.resize(x, y);
	const size_t img_size = x * y;
	for (size_t pixel_index = 0; pixel_index < img_size; ++pixel_index)
	ANTLR_LBRACE
	
#line 2202 "X3DVrmlParser.cpp"
	{
	pixel=intValue();
#line 1887 "expandedX3DVrmlParser.g"
	
	img.pixel(pixel_index, pixel);
	
#line 2209 "X3DVrmlParser.cpp"
	}
#line 1890 "expandedX3DVrmlParser.g"
	
	ANTLR_RBRACE
	
#line 2215 "X3DVrmlParser.cpp"
}

double  X3DVrmlParser::doubleValue() {
#line 2196 "expandedX3DVrmlParser.g"
	double val = 0.0;
#line 2221 "X3DVrmlParser.cpp"
	ANTLR_USE_NAMESPACE(antlr)RefToken  d0 = ANTLR_USE_NAMESPACE(antlr)nullToken;
	ANTLR_USE_NAMESPACE(antlr)RefToken  d1 = ANTLR_USE_NAMESPACE(antlr)nullToken;
	
	switch ( LA(1)) {
	case REAL:
	{
		d0 = LT(1);
		match(REAL);
#line 2200 "expandedX3DVrmlParser.g"
		val = atof(d0->getText().c_str());
#line 2232 "X3DVrmlParser.cpp"
		break;
	}
	case INTEGER:
	{
		d1 = LT(1);
		match(INTEGER);
#line 2201 "expandedX3DVrmlParser.g"
		val = double(atol(d1->getText().c_str()));
#line 2241 "X3DVrmlParser.cpp"
		break;
	}
	default:
	{
		throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
	}
	}
	return val;
}

void X3DVrmlParser::vec2dValue(
	vec2d & v
) {
#line 559 "expandedX3DVrmlParser.g"
	
	double x, y;
	
#line 2259 "X3DVrmlParser.cpp"
	
	x=doubleValue();
	y=doubleValue();
#line 566 "expandedX3DVrmlParser.g"
	
	v.x(x);
	v.y(y);
	
#line 2268 "X3DVrmlParser.cpp"
}

void X3DVrmlParser::vec3dValue(
	vec3d & v
) {
#line 601 "expandedX3DVrmlParser.g"
	
	double x, y, z;
	
#line 2278 "X3DVrmlParser.cpp"
	
	x=doubleValue();
	y=doubleValue();
	z=doubleValue();
#line 608 "expandedX3DVrmlParser.g"
	
	v.x(x);
	v.y(y);
	v.z(z);
	
#line 2289 "X3DVrmlParser.cpp"
}

boost::intrusive_ptr<openvrml::node>  X3DVrmlParser::node(
	const openvrml::scene & scene,
     const boost::shared_ptr<openvrml::scope> & scope,
     const std::string & node_id
) {
#line 1205 "expandedX3DVrmlParser.g"
	boost::intrusive_ptr<openvrml::node> n;
#line 2299 "X3DVrmlParser.cpp"
	ANTLR_USE_NAMESPACE(antlr)RefToken  scriptId = ANTLR_USE_NAMESPACE(antlr)nullToken;
	ANTLR_USE_NAMESPACE(antlr)RefToken  nodeTypeId = ANTLR_USE_NAMESPACE(antlr)nullToken;
#line 1205 "expandedX3DVrmlParser.g"
	
	using antlr::SemanticException;
	using boost::intrusive_ptr;
	
	initial_value_map initial_values;
	node_interface_set interfaces;
	boost::shared_ptr<openvrml::node_type> node_type;
	
#line 2311 "X3DVrmlParser.cpp"
	
	try {      // for error handling
		if (((LA(1) == ID))&&( !LT(1)->getText().compare("Script") )) {
			scriptId = LT(1);
			match(ID);
			match(LBRACE);
			{ // ( ... )*
			for (;;) {
				switch ( LA(1)) {
				case ID:
				case KEYWORD_EXTERNPROTO:
				case KEYWORD_PROTO:
				case KEYWORD_ROUTE:
				{
					nodeBodyElement(scene,
                                scope,
                                script_node_interface_set,
                                initial_values);
					break;
				}
				case KEYWORD_EVENTIN:
				case KEYWORD_EVENTOUT:
				case KEYWORD_FIELD:
				{
					scriptInterfaceDeclaration(scene,
                                           scope,
                                           interfaces,
                                           initial_values,
                                           node_id);
					break;
				}
				default:
				{
					goto _loop68;
				}
				}
			}
			_loop68:;
			} // ( ... )*
			match(RBRACE);
#line 1229 "expandedX3DVrmlParser.g"
			
			n = intrusive_ptr<openvrml::node>(
			new script_node(scene.browser().script_node_metatype_,
			scope,
			interfaces,
			initial_values));
			if (!node_id.empty()) { n->id(node_id); }
			
#line 2361 "X3DVrmlParser.cpp"
		}
		else if ((LA(1) == ID)) {
			nodeTypeId = LT(1);
			match(ID);
#line 1237 "expandedX3DVrmlParser.g"
			
			node_type = scope->find_type(nodeTypeId->getText());
			if (!node_type) {
			throw SemanticException("unknown node type \""
			+ nodeTypeId->getText() + "\"",
			this->getFilename(),
			nodeTypeId->getLine(),
			nodeTypeId->getColumn());
			}
			
			
#line 2378 "X3DVrmlParser.cpp"
			match(LBRACE);
			{ // ( ... )*
			for (;;) {
				if ((_tokenSet_2.member(LA(1)))) {
					nodeBodyElement(scene,
                                  scope,
                                  node_type->interfaces(),
                                  initial_values);
				}
				else {
					goto _loop70;
				}
				
			}
			_loop70:;
			} // ( ... )*
			match(RBRACE);
#line 1251 "expandedX3DVrmlParser.g"
			
			n = node_type->create_node(scope, initial_values);
			
			if (!node_id.empty()) { n->id(node_id); }
			
#line 2402 "X3DVrmlParser.cpp"
		}
		else {
			throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
		}
		
	}
	catch (std::invalid_argument & ex) {
#line 1258 "expandedX3DVrmlParser.g"
		
		throw SemanticException(ex.what(),
		this->getFilename(),
		LT(1)->getLine(),
		LT(1)->getColumn());
		
#line 2417 "X3DVrmlParser.cpp"
	}
	catch (unsupported_interface & ex) {
#line 1264 "expandedX3DVrmlParser.g"
		
		throw SemanticException(ex.what(),
		this->getFilename(),
		LT(1)->getLine(),
		LT(1)->getColumn());
		
#line 2427 "X3DVrmlParser.cpp"
	}
	catch (std::bad_cast &) {
#line 1270 "expandedX3DVrmlParser.g"
		
		throw SemanticException("incorrect value type for field or "
		"exposedField",
		this->getFilename(),
		LT(1)->getLine(),
		LT(1)->getColumn());
		
#line 2438 "X3DVrmlParser.cpp"
	}
	return n;
}

void X3DVrmlParser::externproto(
	const openvrml::scene & scene,
            const boost::shared_ptr<openvrml::scope> & scope
) {
	ANTLR_USE_NAMESPACE(antlr)RefToken  id = ANTLR_USE_NAMESPACE(antlr)nullToken;
#line 925 "expandedX3DVrmlParser.g"
	
	using std::string;
	using std::vector;
	using boost::shared_ptr;
	
	openvrml::node_interface_set interfaces;
	openvrml::mfstring uri_list;
	shared_ptr<node_type> node_type;
	
	//
	// If we are parsing an "anonymous" stream--that is, one passed to
	// browser::create_vrml_from_stream--we cannot use the URI associated with
	// the stream to resolve relative URIs.  So in this instance, we get the
	// base URI from browser::world_url.  If browser::world_url is an empty
	// string, we call create_file_url with an empty (relative) uri.
	//
	const ::uri base_uri = anonymous_stream_id(::uri(this->getFilename()))
	? scene.browser().world_url().empty()
	? create_file_url(::uri())
	: ::uri(scene.browser().world_url())
	: ::uri(this->getFilename());
	
#line 2471 "X3DVrmlParser.cpp"
	
	match(KEYWORD_EXTERNPROTO);
	id = LT(1);
	match(ID);
	match(LBRACKET);
	{ // ( ... )*
	for (;;) {
		if ((_tokenSet_3.member(LA(1)))) {
			externInterfaceDeclaration(interfaces);
		}
		else {
			goto _loop58;
		}
		
	}
	_loop58:;
	} // ( ... )*
	match(RBRACKET);
	uri_list=externprotoUrlList();
#line 954 "expandedX3DVrmlParser.g"
	
	const vector<string> & alt_uris = uri_list.value();
	for (vector<string>::const_iterator resource_id = alt_uris.begin();
	resource_id != alt_uris.end();
	++resource_id) {
	const ::uri absolute_uri = relative(::uri(*resource_id))
	? ::uri(*resource_id).resolve_against(base_uri)
	: ::uri(*resource_id);
	const shared_ptr<openvrml::node_metatype> node_metatype =
	scene.browser().node_metatype(node_metatype_id(absolute_uri));
	if (node_metatype) {
	node_type = node_metatype->create_type(id->getText(),
	interfaces);
	break;
	}
	}
	
	if (!node_type) {
	const shared_ptr<node_metatype> externproto_class(
	new externproto_node_metatype(
	path(*scope) + '#' + id->getText(),
	scene,
	alt_uris,
	scene.browser().load_proto_thread_group_));
	
	scene.browser().add_node_metatype(externproto_class->id(),
	externproto_class);
	for (vector<string>::const_iterator resource_id =
	alt_uris.begin();
	resource_id != alt_uris.end();
	++resource_id) {
	const ::uri absolute_uri = relative(::uri(*resource_id))
	? ::uri(*resource_id).resolve_against(base_uri)
	: ::uri(*resource_id);
	scene.browser()
	.add_node_metatype(node_metatype_id(absolute_uri),
	externproto_class);
	}
	
	node_type = externproto_class->create_type(id->getText(),
	interfaces);
	}
	
	assert(node_type);
	
	if (!scope->add_type(node_type).second) {
	using antlr::SemanticException;
	throw SemanticException("node type \"" + node_type->id()
	+ "\" has already been defined in "
	+ "this scope",
	this->getFilename(),
	id->getLine(),
	id->getColumn());
	}
	
#line 2547 "X3DVrmlParser.cpp"
}

void X3DVrmlParser::proto(
	const openvrml::scene & scene,
      const boost::shared_ptr<openvrml::scope> & scope
) {
	ANTLR_USE_NAMESPACE(antlr)RefToken  id = ANTLR_USE_NAMESPACE(antlr)nullToken;
#line 658 "expandedX3DVrmlParser.g"
	
	using std::string;
	using std::vector;
	using boost::shared_ptr;
	using boost::dynamic_pointer_cast;
	
	node_interface_set interfaces;
	proto_node_metatype::default_value_map_t default_value_map;
	vector<boost::intrusive_ptr<openvrml::node> > impl_nodes;
	proto_node_metatype::is_map_t is_map;
	proto_node_metatype::routes_t routes;
	
#line 2568 "X3DVrmlParser.cpp"
	
	match(KEYWORD_PROTO);
	id = LT(1);
	match(ID);
#line 675 "expandedX3DVrmlParser.g"
	
	assert(scope);
	const boost::shared_ptr<openvrml::scope>
	proto_scope(new openvrml::scope(id->getText(), scope));
	
#line 2579 "X3DVrmlParser.cpp"
	match(LBRACKET);
	{ // ( ... )*
	for (;;) {
		if ((_tokenSet_3.member(LA(1)))) {
			protoInterfaceDeclaration(scene,
                                              scope,
                                              id->getText(),
                                              interfaces,
                                              default_value_map);
		}
		else {
			goto _loop45;
		}
		
	}
	_loop45:;
	} // ( ... )*
	match(RBRACKET);
	match(LBRACE);
	protoBody(scene,
                         proto_scope,
                         interfaces,
                         impl_nodes,
                         is_map,
                         routes);
	match(RBRACE);
#line 689 "expandedX3DVrmlParser.g"
	
	const shared_ptr<openvrml::node_metatype>
	node_metatype(
	new proto_node_metatype(path(*proto_scope),
	scene.browser(),
	interfaces,
	default_value_map,
	impl_nodes,
	is_map,
	routes));
	//
	// Add the new node_metatype (prototype definition) to the browser's
	// node_metatype_map.
	//
	scene.browser().add_node_metatype(node_metatype->id(), node_metatype);
	
	if (!dynamic_pointer_cast<proto_node_metatype>(
	scene.browser().node_metatype(
	node_metatype_id(this->getFilename())))) {
	scene.browser()
	.add_node_metatype(node_metatype_id(this->getFilename()),
	node_metatype);
	}
	
	//
	// PROTOs implicitly introduce a new node type as well.
	//
	const shared_ptr<node_type> node_type =
	node_metatype->create_type(id->getText(), interfaces);
	assert(node_type);
	if (!scope->add_type(node_type).second) {
	using antlr::SemanticException;
	throw SemanticException("node type \"" + node_type->id()
	+ "\" has already been defined in "
	"this scope",
	this->getFilename(),
	id->getLine(),
	id->getColumn());
	}
	
#line 2647 "X3DVrmlParser.cpp"
}

void X3DVrmlParser::protoInterfaceDeclaration(
	
    const openvrml::scene & scene,
    const boost::shared_ptr<const openvrml::scope> & outer_scope,
    const std::string & proto_id,
    node_interface_set & interfaces,
    proto_node_metatype::default_value_map_t & default_value_map
) {
	ANTLR_USE_NAMESPACE(antlr)RefToken  id0 = ANTLR_USE_NAMESPACE(antlr)nullToken;
	ANTLR_USE_NAMESPACE(antlr)RefToken  id1 = ANTLR_USE_NAMESPACE(antlr)nullToken;
#line 732 "expandedX3DVrmlParser.g"
	
	using antlr::SemanticException;
	
	node_interface::type_id it;
	field_value::type_id ft;
	boost::shared_ptr<field_value> fv;
	
#line 2668 "X3DVrmlParser.cpp"
	
	switch ( LA(1)) {
	case KEYWORD_EVENTIN:
	case KEYWORD_EVENTOUT:
	{
		it=eventInterfaceType();
		ft=fieldType();
		id0 = LT(1);
		match(ID);
#line 748 "expandedX3DVrmlParser.g"
		
		const node_interface interface(it, ft, id0->getText());
		const bool succeeded = interfaces.insert(interface).second;
		if (!succeeded) {
		using std::string;
		using boost::lexical_cast;
		throw SemanticException("Interface \""
		+ lexical_cast<string>(interface)
		+ "\" conflicts with previous "
		"declaration",
		this->getFilename(),
		id0->getLine(),
		id0->getColumn());
		}
		
#line 2694 "X3DVrmlParser.cpp"
		break;
	}
	case KEYWORD_EXPOSEDFIELD:
	case KEYWORD_FIELD:
	{
		it=fieldInterfaceType();
		ft=fieldType();
		id1 = LT(1);
		match(ID);
#line 764 "expandedX3DVrmlParser.g"
		
		//
		// The field value declaration should have access to the node
		// types in the outer scope.
		//
		const boost::shared_ptr<openvrml::scope> field_decl_scope(
		new scope(proto_id + '.' + id1->getText(), outer_scope));
		
#line 2713 "X3DVrmlParser.cpp"
		fv=fieldValue(scene, field_decl_scope, ft, std::string());
#line 772 "expandedX3DVrmlParser.g"
		
		assert(fv);
		
		const node_interface interface(it, ft, id1->getText());
		bool succeeded = interfaces.insert(interface).second;
		if (!succeeded) {
		using std::string;
		using boost::lexical_cast;
		throw SemanticException("Interface \""
		+ lexical_cast<string>(interface)
		+ "\" conflicts with previous "
		"declaration",
		this->getFilename(),
		id1->getLine(),
		id1->getColumn());
		}
		succeeded =
		default_value_map.insert(make_pair(id1->getText(), fv)).second;
		assert(succeeded);
		
#line 2736 "X3DVrmlParser.cpp"
		break;
	}
	default:
	{
		throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
	}
	}
}

void X3DVrmlParser::protoBody(
	const openvrml::scene & scene,
          const boost::shared_ptr<openvrml::scope> & scope,
          const node_interface_set & interfaces,
          std::vector<boost::intrusive_ptr<openvrml::node> > & impl_nodes,
          proto_node_metatype::is_map_t & is_map,
          proto_node_metatype::routes_t & routes
) {
#line 813 "expandedX3DVrmlParser.g"
	
	assert(scope);
	assert(impl_nodes.empty());
	assert(is_map.empty());
	assert(routes.empty());
	
	boost::intrusive_ptr<openvrml::node> n;
	
#line 2763 "X3DVrmlParser.cpp"
	
	{ // ( ... )*
	for (;;) {
		if ((LA(1) == KEYWORD_EXTERNPROTO || LA(1) == KEYWORD_PROTO)) {
			protoStatement(scene, scope);
		}
		else {
			goto _loop51;
		}
		
	}
	_loop51:;
	} // ( ... )*
	n=protoNodeStatement(scene,
                                                             scope,
                                                             interfaces,
                                                             is_map,
                                                             routes,
                                                             std::string());
#line 836 "expandedX3DVrmlParser.g"
	
	assert(n);
	impl_nodes.push_back(n);
	
#line 2788 "X3DVrmlParser.cpp"
	{ // ( ... )*
	for (;;) {
		if ((_tokenSet_4.member(LA(1)))) {
			protoBodyStatement(scene,
                            scope,
                            interfaces,
                            impl_nodes,
                            is_map,
                            routes);
		}
		else {
			goto _loop53;
		}
		
	}
	_loop53:;
	} // ( ... )*
}

node_interface::type_id  X3DVrmlParser::eventInterfaceType() {
#line 795 "expandedX3DVrmlParser.g"
	node_interface::type_id it;
#line 2811 "X3DVrmlParser.cpp"
	
	switch ( LA(1)) {
	case KEYWORD_EVENTIN:
	{
		match(KEYWORD_EVENTIN);
#line 799 "expandedX3DVrmlParser.g"
		it = node_interface::eventin_id;
#line 2819 "X3DVrmlParser.cpp"
		break;
	}
	case KEYWORD_EVENTOUT:
	{
		match(KEYWORD_EVENTOUT);
#line 800 "expandedX3DVrmlParser.g"
		it = node_interface::eventout_id;
#line 2827 "X3DVrmlParser.cpp"
		break;
	}
	default:
	{
		throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
	}
	}
	return it;
}

node_interface::type_id  X3DVrmlParser::fieldInterfaceType() {
#line 804 "expandedX3DVrmlParser.g"
	node_interface::type_id it;
#line 2841 "X3DVrmlParser.cpp"
	
	switch ( LA(1)) {
	case KEYWORD_FIELD:
	{
		match(KEYWORD_FIELD);
#line 808 "expandedX3DVrmlParser.g"
		it = node_interface::field_id;
#line 2849 "X3DVrmlParser.cpp"
		break;
	}
	case KEYWORD_EXPOSEDFIELD:
	{
		match(KEYWORD_EXPOSEDFIELD);
#line 809 "expandedX3DVrmlParser.g"
		it = node_interface::exposedfield_id;
#line 2857 "X3DVrmlParser.cpp"
		break;
	}
	default:
	{
		throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
	}
	}
	return it;
}

boost::shared_ptr<field_value>  X3DVrmlParser::fieldValue(
	const openvrml::scene & scene,
           const boost::shared_ptr<openvrml::scope> & scope,
           const openvrml::field_value::type_id ft,
           const std::string & node_id
) {
#line 1657 "expandedX3DVrmlParser.g"
	boost::shared_ptr<field_value> fv;
#line 2876 "X3DVrmlParser.cpp"
	
	if (((_tokenSet_5.member(LA(1))))&&( (ft == field_value::sfnode_id) || (ft == field_value::mfnode_id) )) {
		fv=nodeFieldValue(scene, scope, ft, node_id);
	}
	else if ((_tokenSet_6.member(LA(1)))) {
		fv=nonNodeFieldValue(ft);
	}
	else {
		throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
	}
	
	return fv;
}

boost::intrusive_ptr<openvrml::node>  X3DVrmlParser::protoNodeStatement(
	const openvrml::scene & scene,
                   const boost::shared_ptr<openvrml::scope> & scope,
                   const node_interface_set & proto_interfaces,
                   proto_node_metatype::is_map_t & is_map,
                   proto_node_metatype::routes_t & routes,
                   const std::string & script_node_id
) {
#line 879 "expandedX3DVrmlParser.g"
	boost::intrusive_ptr<openvrml::node> n;
#line 2901 "X3DVrmlParser.cpp"
	ANTLR_USE_NAMESPACE(antlr)RefToken  id0 = ANTLR_USE_NAMESPACE(antlr)nullToken;
	ANTLR_USE_NAMESPACE(antlr)RefToken  id1 = ANTLR_USE_NAMESPACE(antlr)nullToken;
#line 879 "expandedX3DVrmlParser.g"
	
	using antlr::SemanticException;
	
#line 2908 "X3DVrmlParser.cpp"
	
	switch ( LA(1)) {
	case KEYWORD_DEF:
	{
		match(KEYWORD_DEF);
		id0 = LT(1);
		match(ID);
		n=protoNode(scene,
                                       scope,
                                       proto_interfaces,
                                       is_map,
                                       routes,
                                       id0->getText());
		break;
	}
	case KEYWORD_USE:
	{
		match(KEYWORD_USE);
		id1 = LT(1);
		match(ID);
#line 897 "expandedX3DVrmlParser.g"
		
		if (id1->getText() == script_node_id) {
		//
		// Script node self-reference.
		//
		n = node::self_tag;
		} else {
		n = boost::intrusive_ptr<openvrml::node>(
		scope->find_node(id1->getText()));
		if (!n) {
		throw SemanticException("node \"" + id1->getText()
		+ "\" has not been defined in "
		+ "this scope",
		this->getFilename(),
		id1->getLine(),
		id1->getColumn());
		}
		}
		
#line 2949 "X3DVrmlParser.cpp"
		break;
	}
	case ID:
	{
		n=protoNode(scene,
                    scope,
                    proto_interfaces,
                    is_map,
                    routes,
                    std::string());
		break;
	}
	default:
	{
		throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
	}
	}
	return n;
}

void X3DVrmlParser::protoBodyStatement(
	
    const openvrml::scene & scene,
    const boost::shared_ptr<openvrml::scope> & scope,
    const node_interface_set & interfaces,
    std::vector<boost::intrusive_ptr<openvrml::node> > & impl_nodes,
    proto_node_metatype::is_map_t & is_map,
    proto_node_metatype::routes_t & routes
) {
#line 849 "expandedX3DVrmlParser.g"
	
	assert(scope);
	assert(!impl_nodes.empty());
	
	boost::intrusive_ptr<openvrml::node> n;
	
#line 2986 "X3DVrmlParser.cpp"
	
	switch ( LA(1)) {
	case ID:
	case KEYWORD_DEF:
	case KEYWORD_USE:
	{
		n=protoNodeStatement(scene,
                             scope,
                             interfaces,
                             is_map,
                             routes,
                             std::string());
#line 870 "expandedX3DVrmlParser.g"
		
		assert(n);
		impl_nodes.push_back(n);
		
#line 3004 "X3DVrmlParser.cpp"
		break;
	}
	case KEYWORD_EXTERNPROTO:
	case KEYWORD_PROTO:
	{
		protoStatement(scene, scope);
		break;
	}
	case KEYWORD_ROUTE:
	{
		protoRouteStatement(*scope, routes);
		break;
	}
	default:
	{
		throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
	}
	}
}

void X3DVrmlParser::protoRouteStatement(
	const openvrml::scope & scope,
                    proto_node_metatype::routes_t & routes
) {
	ANTLR_USE_NAMESPACE(antlr)RefToken  from_node_id = ANTLR_USE_NAMESPACE(antlr)nullToken;
	ANTLR_USE_NAMESPACE(antlr)RefToken  eventout_id = ANTLR_USE_NAMESPACE(antlr)nullToken;
	ANTLR_USE_NAMESPACE(antlr)RefToken  to_node_id = ANTLR_USE_NAMESPACE(antlr)nullToken;
	ANTLR_USE_NAMESPACE(antlr)RefToken  eventin_id = ANTLR_USE_NAMESPACE(antlr)nullToken;
	
	match(KEYWORD_ROUTE);
	from_node_id = LT(1);
	match(ID);
	match(PERIOD);
	eventout_id = LT(1);
	match(ID);
	match(KEYWORD_TO);
	to_node_id = LT(1);
	match(ID);
	match(PERIOD);
	eventin_id = LT(1);
	match(ID);
#line 1126 "expandedX3DVrmlParser.g"
	
	using std::runtime_error;
	using antlr::SemanticException;
	
	openvrml::node * const from_node =
	scope.find_node(from_node_id->getText());
	if (!from_node) {
	throw SemanticException("node \"" + from_node_id->getText()
	+ "\" has not been defined in this "
	"scope",
	this->getFilename(),
	from_node_id->getLine(),
	from_node_id->getColumn());
	}
	
	openvrml::node * const to_node =
	scope.find_node(to_node_id->getText());
	if (!to_node) {
	throw SemanticException("node \"" + to_node_id->getText()
	+ "\" has not been defined in this "
	"scope",
	this->getFilename(),
	to_node_id->getLine(),
	to_node_id->getColumn());
	}
	
	//
	// Routes outside PROTO definitions get added with add_route, which
	// does the appropriate validation.  However, in a PROTO
	// definition, we don't actually establish a route; the actual
	// routes (event_listeners) are established when the PROTO is
	// instantiated.  So, we need to do validation here.
	//
	const node_interface_set & from_node_interfaces =
	from_node->type().interfaces();
	const node_interface_set::const_iterator from_interface =
	find_if(from_node_interfaces.begin(),
	from_node_interfaces.end(),
	bind2nd(node_interface_matches_eventout(),
	eventout_id->getText()));
	if (from_interface == from_node_interfaces.end()) {
	throw SemanticException(from_node->type().id() + " node has "
	"no eventOut \""
	+ eventout_id->getText() + "\"",
	this->getFilename(),
	eventout_id->getLine(),
	eventout_id->getColumn());
	}
	
	const node_interface_set & to_node_interfaces =
	to_node->type().interfaces();
	const node_interface_set::const_iterator to_interface =
	find_if(to_node_interfaces.begin(), to_node_interfaces.end(),
	bind2nd(node_interface_matches_eventin(),
	eventin_id->getText()));
	if (to_interface == to_node_interfaces.end()) {
	throw SemanticException(to_node->type().id() + " node has no "
	"eventIn \"" + eventin_id->getText()
	+ "\"",
	this->getFilename(),
	eventin_id->getLine(),
	eventin_id->getColumn());
	}
	
	if (to_interface->field_type != from_interface->field_type) {
	throw SemanticException("mismatch between eventOut and "
	"eventIn types",
	this->getFilename(),
	eventin_id->getLine(),
	eventin_id->getColumn());
	}
	
	const proto_node_metatype::route r(*from_node, eventout_id->getText(),
	*to_node, eventin_id->getText());
	routes.push_back(r);
	
#line 3123 "X3DVrmlParser.cpp"
}

boost::intrusive_ptr<openvrml::node>  X3DVrmlParser::protoNode(
	const openvrml::scene & scene,
          const boost::shared_ptr<openvrml::scope> & scope,
          const node_interface_set & proto_interfaces,
          proto_node_metatype::is_map_t & is_map,
          proto_node_metatype::routes_t & routes,
          const std::string & node_id
) {
#line 1406 "expandedX3DVrmlParser.g"
	boost::intrusive_ptr<openvrml::node> n;
#line 3136 "X3DVrmlParser.cpp"
	ANTLR_USE_NAMESPACE(antlr)RefToken  scriptId = ANTLR_USE_NAMESPACE(antlr)nullToken;
	ANTLR_USE_NAMESPACE(antlr)RefToken  nodeTypeId = ANTLR_USE_NAMESPACE(antlr)nullToken;
#line 1406 "expandedX3DVrmlParser.g"
	
	using antlr::SemanticException;
	
	initial_value_map initial_values;
	node_interface_set interfaces;
	is_list is_mappings;
	boost::shared_ptr<openvrml::node_type> node_type;
	
#line 3148 "X3DVrmlParser.cpp"
	
	{
	if (((LA(1) == ID))&&( !LT(1)->getText().compare("Script") )) {
		scriptId = LT(1);
		match(ID);
		match(LBRACE);
		{ // ( ... )*
		for (;;) {
			switch ( LA(1)) {
			case ID:
			case KEYWORD_EXTERNPROTO:
			case KEYWORD_PROTO:
			case KEYWORD_ROUTE:
			{
				protoNodeBodyElement(scene,
                                     scope,
                                     proto_interfaces,
                                     is_map,
                                     routes,
                                     script_node_interface_set,
                                     initial_values,
                                     is_mappings);
				break;
			}
			case KEYWORD_EVENTIN:
			case KEYWORD_EVENTOUT:
			case KEYWORD_FIELD:
			{
				protoScriptInterfaceDeclaration(scene,
                                                scope,
                                                proto_interfaces,
                                                is_map,
                                                routes,
                                                node_id,
                                                interfaces,
                                                initial_values,
                                                is_mappings);
				break;
			}
			default:
			{
				goto _loop77;
			}
			}
		}
		_loop77:;
		} // ( ... )*
		match(RBRACE);
#line 1442 "expandedX3DVrmlParser.g"
		
		n = boost::intrusive_ptr<openvrml::node>(
		new script_node(scene.browser().script_node_metatype_,
		scope,
		interfaces,
		initial_values));
		if (!node_id.empty()) { n->id(node_id); }
		
#line 3206 "X3DVrmlParser.cpp"
	}
	else if ((LA(1) == ID)) {
		nodeTypeId = LT(1);
		match(ID);
#line 1450 "expandedX3DVrmlParser.g"
		
		node_type = scope->find_type(nodeTypeId->getText());
		if (!node_type) {
		throw SemanticException("unknown node type \""
		+ nodeTypeId->getText() + "\"",
		this->getFilename(),
		nodeTypeId->getLine(),
		nodeTypeId->getColumn());
		}
		
#line 3222 "X3DVrmlParser.cpp"
		match(LBRACE);
		{ // ( ... )*
		for (;;) {
			if ((_tokenSet_2.member(LA(1)))) {
				protoNodeBodyElement(scene,
                                         scope,
                                         proto_interfaces,
                                         is_map,
                                         routes,
                                         node_type->interfaces(),
                                         initial_values,
                                         is_mappings);
			}
			else {
				goto _loop79;
			}
			
		}
		_loop79:;
		} // ( ... )*
		match(RBRACE);
#line 1467 "expandedX3DVrmlParser.g"
		
		n = node_type->create_node(scope, initial_values);
		if (!node_id.empty()) { n->id(node_id); }
		
#line 3249 "X3DVrmlParser.cpp"
	}
	else {
		throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
	}
	
	}
#line 1471 "expandedX3DVrmlParser.g"
	
	for (is_list::const_iterator is_mapping = is_mappings.begin();
	is_mapping != is_mappings.end();
	++is_mapping) {
	typedef proto_node_metatype::is_target is_target;
	is_map.insert(make_pair(is_mapping->second,
	is_target(*n, is_mapping->first)));
	}
	
#line 3266 "X3DVrmlParser.cpp"
	return n;
}

void X3DVrmlParser::externInterfaceDeclaration(
	openvrml::node_interface_set & interfaces
) {
	ANTLR_USE_NAMESPACE(antlr)RefToken  id = ANTLR_USE_NAMESPACE(antlr)nullToken;
#line 1012 "expandedX3DVrmlParser.g"
	
	using openvrml::node_interface;
	using openvrml::field_value;
	using antlr::SemanticException;
	node_interface::type_id it(node_interface::invalid_type_id);
	field_value::type_id ft(field_value::invalid_type_id);
	
#line 3282 "X3DVrmlParser.cpp"
	
	it=interfaceType();
	ft=fieldType();
	id = LT(1);
	match(ID);
#line 1023 "expandedX3DVrmlParser.g"
	
	const node_interface interface(it, ft, id->getText());
	bool succeeded = interfaces.insert(interface).second;
	if (!succeeded) {
	using std::string;
	using boost::lexical_cast;
	throw SemanticException("Interface \""
	+ lexical_cast<string>(interface)
	+ "\" conflicts with previous "
	"declaration",
	this->getFilename(),
	id->getLine(),
	id->getColumn());
	}
	
#line 3304 "X3DVrmlParser.cpp"
}

mfstring  X3DVrmlParser::externprotoUrlList() {
#line 1050 "expandedX3DVrmlParser.g"
	mfstring urlList;
#line 3310 "X3DVrmlParser.cpp"
#line 1050 "expandedX3DVrmlParser.g"
	
	using std::string;
	using std::vector;
	using openvrml::mfstring;
	
	string s;
	
#line 3319 "X3DVrmlParser.cpp"
	
	switch ( LA(1)) {
	case STRING:
	{
		s=stringValue();
#line 1061 "expandedX3DVrmlParser.g"
		urlList.value(vector<string>(1, s));
#line 3327 "X3DVrmlParser.cpp"
		break;
	}
	case LBRACKET:
	{
		match(LBRACKET);
#line 1062 "expandedX3DVrmlParser.g"
		
		vector<string> value;
		
#line 3337 "X3DVrmlParser.cpp"
		{ // ( ... )*
		for (;;) {
			if ((LA(1) == STRING)) {
				s=stringValue();
#line 1064 "expandedX3DVrmlParser.g"
				value.push_back(s);
#line 3344 "X3DVrmlParser.cpp"
			}
			else {
				goto _loop63;
			}
			
		}
		_loop63:;
		} // ( ... )*
		match(RBRACKET);
#line 1064 "expandedX3DVrmlParser.g"
		
		urlList.value(value);
		
#line 3358 "X3DVrmlParser.cpp"
		break;
	}
	default:
	{
		throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
	}
	}
	return urlList;
}

node_interface::type_id  X3DVrmlParser::interfaceType() {
#line 1041 "expandedX3DVrmlParser.g"
	node_interface::type_id interface_type;
#line 3372 "X3DVrmlParser.cpp"
	
	switch ( LA(1)) {
	case KEYWORD_EVENTIN:
	case KEYWORD_EVENTOUT:
	{
		interface_type=eventInterfaceType();
		break;
	}
	case KEYWORD_EXPOSEDFIELD:
	case KEYWORD_FIELD:
	{
		interface_type=fieldInterfaceType();
		break;
	}
	default:
	{
		throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
	}
	}
	return interface_type;
}

void X3DVrmlParser::nodeBodyElement(
	const openvrml::scene & scene,
                const boost::shared_ptr<openvrml::scope> & scope,
                const node_interface_set & interfaces,
                initial_value_map & initial_values
) {
	ANTLR_USE_NAMESPACE(antlr)RefToken  id = ANTLR_USE_NAMESPACE(antlr)nullToken;
#line 1279 "expandedX3DVrmlParser.g"
	
	using std::find_if;
	using std::bind2nd;
	using antlr::SemanticException;
	field_value::type_id ft = field_value::invalid_type_id;
	boost::shared_ptr<field_value> fv;
	
#line 3410 "X3DVrmlParser.cpp"
	
	switch ( LA(1)) {
	case ID:
	{
		id = LT(1);
		match(ID);
#line 1293 "expandedX3DVrmlParser.g"
		
		node_interface_set::const_iterator interface =
		find_if(interfaces.begin(), interfaces.end(),
		bind2nd(node_interface_matches_field(),
		id->getText()));
		if (interface == interfaces.end()) {
		interface =
		find_if(interfaces.begin(), interfaces.end(),
		bind2nd(node_interface_matches_exposedfield(),
		id->getText()));
		}
		if (interface == interfaces.end()) {
		throw SemanticException("Node has no field or exposedField \""
		+ id->getText() + "\"",
		this->getFilename(),
		id->getLine(),
		id->getColumn());
		}
		ft = interface->field_type;
		
#line 3438 "X3DVrmlParser.cpp"
		fv=fieldValue(scene, scope, ft, std::string());
#line 1312 "expandedX3DVrmlParser.g"
		
		assert(fv);
		const bool succeeded =
		initial_values.insert(make_pair(id->getText(), fv)).second;
		if (!succeeded) {
		throw SemanticException("value for " + id->getText()
		+ " already declared",
		this->getFilename(),
		id->getLine(),
		id->getColumn());
		}
		
#line 3453 "X3DVrmlParser.cpp"
		break;
	}
	case KEYWORD_ROUTE:
	{
		routeStatement(*scope);
		break;
	}
	case KEYWORD_EXTERNPROTO:
	case KEYWORD_PROTO:
	{
		protoStatement(scene, scope);
		break;
	}
	default:
	{
		throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
	}
	}
}

void X3DVrmlParser::scriptInterfaceDeclaration(
	const openvrml::scene & scene,
                           const boost::shared_ptr<openvrml::scope> & scope,
                           node_interface_set & interfaces,
                           initial_value_map & initial_values,
                           const std::string & node_id
) {
	ANTLR_USE_NAMESPACE(antlr)RefToken  id = ANTLR_USE_NAMESPACE(antlr)nullToken;
#line 1329 "expandedX3DVrmlParser.g"
	
	using antlr::SemanticException;
	node_interface::type_id it(node_interface::invalid_type_id);
	field_value::type_id ft(field_value::invalid_type_id);
	
#line 3488 "X3DVrmlParser.cpp"
	
	switch ( LA(1)) {
	case KEYWORD_EVENTIN:
	case KEYWORD_EVENTOUT:
	{
		it=eventInterfaceType();
		ft=fieldType();
		id = LT(1);
		match(ID);
#line 1342 "expandedX3DVrmlParser.g"
		
		const node_interface interface(it, ft, id->getText());
		const bool succeeded = interfaces.insert(interface).second;
		if (!succeeded) {
		using std::string;
		using boost::lexical_cast;
		throw SemanticException("Interface \""
		+ lexical_cast<string>(interface)
		+ "\" conflicts with previous "
		"declaration",
		this->getFilename(),
		id->getLine(),
		id->getColumn());
		}
		
#line 3514 "X3DVrmlParser.cpp"
		break;
	}
	case KEYWORD_FIELD:
	{
		scriptFieldInterfaceDeclaration(scene,
                                        scope,
                                        interfaces,
                                        initial_values,
                                        node_id);
		break;
	}
	default:
	{
		throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
	}
	}
}

void X3DVrmlParser::scriptFieldInterfaceDeclaration(
	
    const openvrml::scene & scene,
    const boost::shared_ptr<openvrml::scope> & scope,
    node_interface_set & interfaces,
    initial_value_map & initial_values,
    const std::string & script_node_id
) {
	ANTLR_USE_NAMESPACE(antlr)RefToken  id = ANTLR_USE_NAMESPACE(antlr)nullToken;
#line 1365 "expandedX3DVrmlParser.g"
	
	using std::find_if;
	using antlr::SemanticException;
	
	field_value::type_id ft = field_value::invalid_type_id;
	boost::shared_ptr<field_value> fv;
	
#line 3550 "X3DVrmlParser.cpp"
	
	match(KEYWORD_FIELD);
	ft=fieldType();
	id = LT(1);
	match(ID);
	fv=fieldValue(scene,
                                                       scope,
                                                       ft,
                                                       script_node_id);
#line 1384 "expandedX3DVrmlParser.g"
	
	assert(fv);
	bool succeeded =
	interfaces.insert(node_interface(node_interface::field_id,
	ft,
	id->getText()))
	.second;
	if (!succeeded) {
	throw SemanticException("interface \"" + id->getText()
	+ "\" already declared for Script "
	"node",
	this->getFilename(),
	id->getLine(),
	id->getColumn());
	}
	succeeded = initial_values.insert(make_pair(id->getText(), fv))
	.second;
	assert(succeeded);
	
#line 3580 "X3DVrmlParser.cpp"
}

void X3DVrmlParser::protoNodeBodyElement(
	const openvrml::scene & scene,
                     const boost::shared_ptr<openvrml::scope> & scope,
                     const node_interface_set & proto_interfaces,
                     proto_node_metatype::is_map_t & is_map,
                     proto_node_metatype::routes_t & routes,
                     const node_interface_set & node_interfaces,
                     initial_value_map & initial_values,
                     is_list & is_mappings
) {
	ANTLR_USE_NAMESPACE(antlr)RefToken  interface_id = ANTLR_USE_NAMESPACE(antlr)nullToken;
#line 1483 "expandedX3DVrmlParser.g"
	
	using std::string;
	using antlr::SemanticException;
	
	boost::shared_ptr<field_value> fv;
	
#line 3601 "X3DVrmlParser.cpp"
	
	switch ( LA(1)) {
	case ID:
	{
		interface_id = LT(1);
		match(ID);
#line 1500 "expandedX3DVrmlParser.g"
		
		const node_interface_set::const_iterator impl_node_interface =
		find_interface(node_interfaces, interface_id->getText());
		if (impl_node_interface == node_interfaces.end()) {
		throw SemanticException("node has no interface \""
		+ interface_id->getText() + "\"",
		this->getFilename(),
		interface_id->getLine(),
		interface_id->getColumn());
		}
		
#line 3620 "X3DVrmlParser.cpp"
		{
		if (((_tokenSet_7.member(LA(1))))&&(impl_node_interface->type == node_interface::field_id
            || impl_node_interface->type == node_interface::exposedfield_id)) {
			{
			switch ( LA(1)) {
			case LBRACKET:
			case ID:
			case INTEGER:
			case HEX_INTEGER:
			case REAL:
			case STRING:
			case KEYWORD_DEF:
			case KEYWORD_FALSE:
			case KEYWORD_NULL:
			case KEYWORD_TRUE:
			case KEYWORD_USE:
			{
				fv=protoFieldValue(scene,
                                   scope,
                                   proto_interfaces,
                                   is_map,
                                   routes,
                                   impl_node_interface->field_type,
                                   std::string());
#line 1518 "expandedX3DVrmlParser.g"
				
				assert(fv);
				bool succeeded =
				initial_values.insert(
				make_pair(interface_id->getText(), fv)).second;
				assert(succeeded);
				
#line 3653 "X3DVrmlParser.cpp"
				break;
			}
			case KEYWORD_IS:
			{
				isStatement(impl_node_interface->id, is_mappings);
				break;
			}
			default:
			{
				throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
			}
			}
			}
		}
		else if ((LA(1) == KEYWORD_IS)) {
			isStatement(impl_node_interface->id, is_mappings);
		}
		else {
			throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
		}
		
		}
		break;
	}
	case KEYWORD_ROUTE:
	{
		protoRouteStatement(*scope, routes);
		break;
	}
	case KEYWORD_EXTERNPROTO:
	case KEYWORD_PROTO:
	{
		protoStatement(scene, scope);
		break;
	}
	default:
	{
		throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
	}
	}
}

void X3DVrmlParser::protoScriptInterfaceDeclaration(
	
    const openvrml::scene & scene,
    const boost::shared_ptr<openvrml::scope> & scope,
    const node_interface_set & proto_interfaces,
    proto_node_metatype::is_map_t & is_map,
    proto_node_metatype::routes_t & routes,
    const std::string & script_node_id,
    node_interface_set & interfaces,
    initial_value_map & initial_values,
    is_list & is_mappings
) {
	ANTLR_USE_NAMESPACE(antlr)RefToken  id = ANTLR_USE_NAMESPACE(antlr)nullToken;
#line 1546 "expandedX3DVrmlParser.g"
	
	using antlr::SemanticException;
	node_interface::type_id it;
	field_value::type_id ft;
	
#line 3715 "X3DVrmlParser.cpp"
	
	switch ( LA(1)) {
	case KEYWORD_EVENTIN:
	case KEYWORD_EVENTOUT:
	{
		it=eventInterfaceType();
		ft=fieldType();
		id = LT(1);
		match(ID);
#line 1564 "expandedX3DVrmlParser.g"
		
		const node_interface interface(it, ft, id->getText());
		const bool succeeded = interfaces.insert(interface).second;
		if (!succeeded) {
		using std::string;
		using boost::lexical_cast;
		throw SemanticException("Interface  \""
		+ lexical_cast<string>(interface)
		+ "\" conflicts with previous "
		"declaration",
		this->getFilename(),
		id->getLine(),
		id->getColumn());
		}
		
#line 3741 "X3DVrmlParser.cpp"
		{
		switch ( LA(1)) {
		case KEYWORD_IS:
		{
			isStatement(id->getText(), is_mappings);
			break;
		}
		case RBRACE:
		case ID:
		case KEYWORD_EVENTIN:
		case KEYWORD_EVENTOUT:
		case KEYWORD_EXTERNPROTO:
		case KEYWORD_FIELD:
		case KEYWORD_PROTO:
		case KEYWORD_ROUTE:
		{
			break;
		}
		default:
		{
			throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
		}
		}
		}
		break;
	}
	case KEYWORD_FIELD:
	{
		protoScriptFieldInterfaceDeclaration(scene,
                                             scope,
                                             proto_interfaces,
                                             is_map,
                                             routes,
                                             interfaces,
                                             initial_values,
                                             script_node_id,
                                             is_mappings);
		break;
	}
	default:
	{
		throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
	}
	}
}

boost::shared_ptr<field_value>  X3DVrmlParser::protoFieldValue(
	const openvrml::scene & scene,
                const boost::shared_ptr<openvrml::scope> & scope,
                const node_interface_set & proto_interfaces,
                proto_node_metatype::is_map_t & is_map,
                proto_node_metatype::routes_t & routes,
                const field_value::type_id ft,
                const std::string & script_node_id
) {
#line 1670 "expandedX3DVrmlParser.g"
	boost::shared_ptr<field_value> fv;
#line 3799 "X3DVrmlParser.cpp"
	
	if (((_tokenSet_5.member(LA(1))))&&( (ft == field_value::sfnode_id) || (ft == field_value::mfnode_id) )) {
		fv=protoNodeFieldValue(scene,
                               scope,
                               proto_interfaces,
                               is_map,
                               routes,
                               ft,
                               script_node_id);
#line 1687 "expandedX3DVrmlParser.g"
		
		assert(fv);
		
#line 3813 "X3DVrmlParser.cpp"
	}
	else if ((_tokenSet_6.member(LA(1)))) {
		fv=nonNodeFieldValue(ft);
#line 1690 "expandedX3DVrmlParser.g"
		assert(fv);
#line 3819 "X3DVrmlParser.cpp"
	}
	else {
		throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
	}
	
	return fv;
}

void X3DVrmlParser::isStatement(
	const std::string & impl_node_interface_id,
            is_list & is_mappings
) {
	ANTLR_USE_NAMESPACE(antlr)RefToken  id = ANTLR_USE_NAMESPACE(antlr)nullToken;
	
	match(KEYWORD_IS);
	id = LT(1);
	match(ID);
#line 1539 "expandedX3DVrmlParser.g"
	
	is_mappings.insert(make_pair(impl_node_interface_id,
	id->getText()));
	
#line 3842 "X3DVrmlParser.cpp"
}

void X3DVrmlParser::protoScriptFieldInterfaceDeclaration(
	
    const openvrml::scene & scene,
    const boost::shared_ptr<openvrml::scope> & scope,
    const node_interface_set & proto_interfaces,
    proto_node_metatype::is_map_t & is_map,
    proto_node_metatype::routes_t & routes,
    node_interface_set & interfaces,
    initial_value_map & initial_values,
    const std::string & script_node_id,
    is_list & is_mappings
) {
	ANTLR_USE_NAMESPACE(antlr)RefToken  id = ANTLR_USE_NAMESPACE(antlr)nullToken;
#line 1591 "expandedX3DVrmlParser.g"
	
	field_value::type_id ft;
	boost::shared_ptr<field_value> fv;
	bool succeeded;
	
#line 3864 "X3DVrmlParser.cpp"
	
	match(KEYWORD_FIELD);
	ft=fieldType();
	id = LT(1);
	match(ID);
#line 1609 "expandedX3DVrmlParser.g"
	
	succeeded =
	interfaces.insert(node_interface(node_interface::field_id,
	ft,
	id->getText()))
	.second;
	if (!succeeded) {
	using antlr::SemanticException;
	throw SemanticException("interface \"" + id->getText()
	+ "\" already declared for Script "
	"node",
	this->getFilename(),
	id->getLine(),
	id->getColumn());
	}
	
#line 3887 "X3DVrmlParser.cpp"
	{
	switch ( LA(1)) {
	case LBRACKET:
	case ID:
	case INTEGER:
	case HEX_INTEGER:
	case REAL:
	case STRING:
	case KEYWORD_DEF:
	case KEYWORD_FALSE:
	case KEYWORD_NULL:
	case KEYWORD_TRUE:
	case KEYWORD_USE:
	{
		fv=protoFieldValue(scene,
                               scope,
                               proto_interfaces,
                               is_map,
                               routes,
                               ft,
                               script_node_id);
#line 1631 "expandedX3DVrmlParser.g"
		
		assert(fv);
		succeeded = initial_values.insert(make_pair(id->getText(), fv))
		.second;
		assert(succeeded);
		
#line 3916 "X3DVrmlParser.cpp"
		break;
	}
	case KEYWORD_IS:
	{
		isStatement(id->getText(), is_mappings);
#line 1637 "expandedX3DVrmlParser.g"
		
		using std::auto_ptr;
		using boost::shared_ptr;
		
		//
		// The field needs some default value as a placeholder. This
		// is never actually used in a PROTO instance.
		//
		auto_ptr<field_value> value = field_value::create(ft);
		succeeded =
		initial_values.insert(
		make_pair(id->getText(),
		shared_ptr<field_value>(value)))
		.second;
		assert(succeeded);
		
#line 3939 "X3DVrmlParser.cpp"
		break;
	}
	default:
	{
		throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
	}
	}
	}
}

boost::shared_ptr<field_value>  X3DVrmlParser::nodeFieldValue(
	const openvrml::scene & scene,
               const boost::shared_ptr<openvrml::scope> & scope,
               openvrml::field_value::type_id ft,
               const std::string & script_node_id
) {
#line 1694 "expandedX3DVrmlParser.g"
	boost::shared_ptr<field_value> fv;
#line 3958 "X3DVrmlParser.cpp"
#line 1694 "expandedX3DVrmlParser.g"
	
	using openvrml::field_value;
	
#line 3963 "X3DVrmlParser.cpp"
	
	if (((_tokenSet_8.member(LA(1))))&&( ft == field_value::sfnode_id )) {
		fv=sfNodeValue(scene,
                                                         scope,
                                                         script_node_id);
	}
	else if ((_tokenSet_9.member(LA(1)))) {
		fv=mfNodeValue(scene, scope, script_node_id);
	}
	else {
		throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
	}
	
	return fv;
}

boost::shared_ptr<field_value>  X3DVrmlParser::protoNodeFieldValue(
	const openvrml::scene & scene,
                    const boost::shared_ptr<openvrml::scope> & scope,
                    const node_interface_set & proto_interfaces,
                    proto_node_metatype::is_map_t & is_map,
                    proto_node_metatype::routes_t & routes,
                    field_value::type_id ft,
                    const std::string & script_node_id
) {
#line 1711 "expandedX3DVrmlParser.g"
	boost::shared_ptr<field_value> fv;
#line 3991 "X3DVrmlParser.cpp"
	
	if (((_tokenSet_8.member(LA(1))))&&( ft == field_value::sfnode_id )) {
		fv=protoSfNodeValue(scene,
                                                              scope,
                                                              proto_interfaces,
                                                              is_map,
                                                              routes,
                                                              script_node_id);
	}
	else if ((_tokenSet_9.member(LA(1)))) {
		fv=protoMfNodeValue(scene,
                            scope,
                            proto_interfaces,
                            is_map,
                            routes,
                            script_node_id);
	}
	else {
		throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
	}
	
	return fv;
}

boost::shared_ptr<field_value>  X3DVrmlParser::sfNodeValue(
	const openvrml::scene & scene,
            const boost::shared_ptr<openvrml::scope> & scope,
            const std::string & script_node_id
) {
#line 1940 "expandedX3DVrmlParser.g"
	boost::shared_ptr<field_value> snv;
#line 4023 "X3DVrmlParser.cpp"
#line 1940 "expandedX3DVrmlParser.g"
	
	boost::intrusive_ptr<openvrml::node> n;
	
#line 4028 "X3DVrmlParser.cpp"
	
	switch ( LA(1)) {
	case ID:
	case KEYWORD_DEF:
	case KEYWORD_USE:
	{
		n=nodeStatement(scene, scope, script_node_id);
#line 1949 "expandedX3DVrmlParser.g"
		
		snv.reset(new sfnode(n));
		
#line 4040 "X3DVrmlParser.cpp"
		break;
	}
	case KEYWORD_NULL:
	{
		match(KEYWORD_NULL);
#line 1952 "expandedX3DVrmlParser.g"
		snv.reset(new sfnode);
#line 4048 "X3DVrmlParser.cpp"
		break;
	}
	default:
	{
		throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
	}
	}
	return snv;
}

boost::shared_ptr<field_value>  X3DVrmlParser::mfNodeValue(
	const openvrml::scene & scene,
            const boost::shared_ptr<openvrml::scope> & scope,
            const std::string & script_node_id
) {
#line 1982 "expandedX3DVrmlParser.g"
	boost::shared_ptr<field_value> mnv =
         boost::shared_ptr<field_value>(new mfnode);
#line 4067 "X3DVrmlParser.cpp"
#line 1982 "expandedX3DVrmlParser.g"
	
	using std::vector;
	
	boost::intrusive_ptr<openvrml::node> n;
	mfnode & nodes = static_cast<mfnode &>(*mnv);
	
#line 4075 "X3DVrmlParser.cpp"
	
	switch ( LA(1)) {
	case ID:
	case KEYWORD_DEF:
	case KEYWORD_USE:
	{
		n=nodeStatement(scene, scope, script_node_id);
#line 1995 "expandedX3DVrmlParser.g"
		
		if (n) { nodes.value(mfnode::value_type(1, n)); }
		
#line 4087 "X3DVrmlParser.cpp"
		break;
	}
	case LBRACKET:
	{
		match(LBRACKET);
#line 1998 "expandedX3DVrmlParser.g"
		
		mfnode::value_type value;
		
#line 4097 "X3DVrmlParser.cpp"
		{ // ( ... )*
		for (;;) {
			if ((LA(1) == ID || LA(1) == KEYWORD_DEF || LA(1) == KEYWORD_USE)) {
				n=nodeStatement(scene, scope, script_node_id);
#line 2001 "expandedX3DVrmlParser.g"
				
				if (n) { value.push_back(n); }
				
#line 4106 "X3DVrmlParser.cpp"
			}
			else {
				goto _loop117;
			}
			
		}
		_loop117:;
		} // ( ... )*
		match(RBRACKET);
#line 2004 "expandedX3DVrmlParser.g"
		
		nodes.value(value);
		
#line 4120 "X3DVrmlParser.cpp"
		break;
	}
	default:
	{
		throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
	}
	}
	return mnv;
}

boost::shared_ptr<field_value>  X3DVrmlParser::protoSfNodeValue(
	const openvrml::scene & scene,
                 const boost::shared_ptr<openvrml::scope> & scope,
                 const node_interface_set & proto_interfaces,
                 proto_node_metatype::is_map_t & is_map,
                 proto_node_metatype::routes_t & routes,
                 const std::string & script_node_id
) {
#line 1956 "expandedX3DVrmlParser.g"
	boost::shared_ptr<field_value> snv;
#line 4141 "X3DVrmlParser.cpp"
#line 1956 "expandedX3DVrmlParser.g"
	
	boost::intrusive_ptr<openvrml::node> n;
	
#line 4146 "X3DVrmlParser.cpp"
	
	switch ( LA(1)) {
	case ID:
	case KEYWORD_DEF:
	case KEYWORD_USE:
	{
		n=protoNodeStatement(scene,
                             scope,
                             proto_interfaces,
                             is_map,
                             routes,
                             script_node_id);
#line 1973 "expandedX3DVrmlParser.g"
		
		snv.reset(new sfnode(n));
		
#line 4163 "X3DVrmlParser.cpp"
		break;
	}
	case KEYWORD_NULL:
	{
		match(KEYWORD_NULL);
#line 1976 "expandedX3DVrmlParser.g"
		
		snv.reset(new sfnode);
		
#line 4173 "X3DVrmlParser.cpp"
		break;
	}
	default:
	{
		throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
	}
	}
	return snv;
}

boost::shared_ptr<field_value>  X3DVrmlParser::protoMfNodeValue(
	const openvrml::scene & scene,
                 const boost::shared_ptr<openvrml::scope> & scope,
                 const node_interface_set & proto_interfaces,
                 proto_node_metatype::is_map_t & is_map,
                 proto_node_metatype::routes_t & routes,
                 const std::string & script_node_id
) {
#line 2010 "expandedX3DVrmlParser.g"
	boost::shared_ptr<field_value> mnv =
         boost::shared_ptr<field_value>(new mfnode);
#line 4195 "X3DVrmlParser.cpp"
#line 2010 "expandedX3DVrmlParser.g"
	
	using std::vector;
	
	boost::intrusive_ptr<openvrml::node> n;
	mfnode & nodes = static_cast<mfnode &>(*mnv);
	
#line 4203 "X3DVrmlParser.cpp"
	
	switch ( LA(1)) {
	case ID:
	case KEYWORD_DEF:
	case KEYWORD_USE:
	{
		n=protoNodeStatement(scene,
                             scope,
                             proto_interfaces,
                             is_map,
                             routes,
                             script_node_id);
#line 2031 "expandedX3DVrmlParser.g"
		
		if (n) { nodes.value(mfnode::value_type(1, n)); }
		
#line 4220 "X3DVrmlParser.cpp"
		break;
	}
	case LBRACKET:
	{
		match(LBRACKET);
#line 2034 "expandedX3DVrmlParser.g"
		
		mfnode::value_type value;
		
#line 4230 "X3DVrmlParser.cpp"
		{ // ( ... )*
		for (;;) {
			if ((LA(1) == ID || LA(1) == KEYWORD_DEF || LA(1) == KEYWORD_USE)) {
				n=protoNodeStatement(scene,
                                 scope,
                                 proto_interfaces,
                                 is_map,
                                 routes,
                                 script_node_id);
#line 2042 "expandedX3DVrmlParser.g"
				
				if (n) { value.push_back(n); }
				
#line 4244 "X3DVrmlParser.cpp"
			}
			else {
				goto _loop120;
			}
			
		}
		_loop120:;
		} // ( ... )*
		match(RBRACKET);
#line 2045 "expandedX3DVrmlParser.g"
		
		nodes.value(value);
		
#line 4258 "X3DVrmlParser.cpp"
		break;
	}
	default:
	{
		throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
	}
	}
	return mnv;
}

void X3DVrmlParser::colorValue(
	color & c
) {
#line 1787 "expandedX3DVrmlParser.g"
	
	float r, g, b;
	
#line 4276 "X3DVrmlParser.cpp"
	
	r=colorComponent();
	g=colorComponent();
	b=colorComponent();
#line 1794 "expandedX3DVrmlParser.g"
	c.r(r);
	c.g(g);
	c.b(b);
#line 4285 "X3DVrmlParser.cpp"
}

float  X3DVrmlParser::floatValue() {
#line 1849 "expandedX3DVrmlParser.g"
	float val;
#line 4291 "X3DVrmlParser.cpp"
	ANTLR_USE_NAMESPACE(antlr)RefToken  f0 = ANTLR_USE_NAMESPACE(antlr)nullToken;
	ANTLR_USE_NAMESPACE(antlr)RefToken  f1 = ANTLR_USE_NAMESPACE(antlr)nullToken;
	
	switch ( LA(1)) {
	case REAL:
	{
		f0 = LT(1);
		match(REAL);
#line 1853 "expandedX3DVrmlParser.g"
		val = float(atof(f0->getText().c_str()));
#line 4302 "X3DVrmlParser.cpp"
		break;
	}
	case INTEGER:
	{
		f1 = LT(1);
		match(INTEGER);
#line 1854 "expandedX3DVrmlParser.g"
		val = float(atol(f1->getText().c_str()));
#line 4311 "X3DVrmlParser.cpp"
		break;
	}
	default:
	{
		throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
	}
	}
	return val;
}

void X3DVrmlParser::rotationValue(
	rotation & r
) {
#line 2082 "expandedX3DVrmlParser.g"
	
	using openvrml_::fequal;
	float x, y, z, angle;
	
#line 4330 "X3DVrmlParser.cpp"
	
	x=floatValue();
	y=floatValue();
	z=floatValue();
	angle=floatValue();
#line 2090 "expandedX3DVrmlParser.g"
	
	vec3f axis(x, y, z);
	
	const float axisLength = axis.length();
	if (!fequal(axisLength, 1.0f)) {
	this->reportWarning("axis component of a rotation must be "
	"a normalized vector");
	if (fequal(axisLength, 0.0f)) {
	axis.z(1.0);
	} else {
	axis = axis.normalize();
	}
	}
	r = rotation(axis, angle);
	
#line 4352 "X3DVrmlParser.cpp"
}

void X3DVrmlParser::vec2fValue(
	vec2f & v
) {
#line 2236 "expandedX3DVrmlParser.g"
	
	float x, y;
	
#line 4362 "X3DVrmlParser.cpp"
	
	x=floatValue();
	y=floatValue();
#line 2243 "expandedX3DVrmlParser.g"
	v.x(x);
	v.y(y);
#line 4369 "X3DVrmlParser.cpp"
}

void X3DVrmlParser::vec3fValue(
	vec3f & v
) {
#line 2279 "expandedX3DVrmlParser.g"
	
	float x, y, z;
	
#line 4379 "X3DVrmlParser.cpp"
	
	x=floatValue();
	y=floatValue();
	z=floatValue();
#line 2286 "expandedX3DVrmlParser.g"
	v.x(x);
	v.y(y);
	v.z(z);
#line 4388 "X3DVrmlParser.cpp"
}

void X3DVrmlParser::initializeASTFactory( ANTLR_USE_NAMESPACE(antlr)ASTFactory& )
{
}
const char* X3DVrmlParser::tokenNames[] = {
	"<0>",
	"EOF",
	"<2>",
	"NULL_TREE_LOOKAHEAD",
	".",
	"[",
	"]",
	"{",
	"}",
	"an identifier",
	"an integer value",
	"an integer value",
	"a floating point value",
	"a string",
	"\"DEF\"",
	"\"eventIn\"",
	"\"eventOut\"",
	"\"exposedField\"",
	"\"EXTERNPROTO\"",
	"\"FALSE\"",
	"\"field\"",
	"\"IS\"",
	"\"NULL\"",
	"\"PROTO\"",
	"\"ROUTE\"",
	"\"TO\"",
	"\"TRUE\"",
	"\"USE\"",
	"FIELDTYPE_SFBOOL",
	"FIELDTYPE_SFCOLOR",
	"FIELDTYPE_SFFLOAT",
	"FIELDTYPE_SFIMAGE",
	"FIELDTYPE_SFINT32",
	"FIELDTYPE_SFNODE",
	"FIELDTYPE_SFROTATION",
	"FIELDTYPE_SFSTRING",
	"FIELDTYPE_SFTIME",
	"FIELDTYPE_SFVEC2F",
	"FIELDTYPE_SFVEC3F",
	"FIELDTYPE_MFCOLOR",
	"FIELDTYPE_MFFLOAT",
	"FIELDTYPE_MFINT32",
	"FIELDTYPE_MFNODE",
	"FIELDTYPE_MFROTATION",
	"FIELDTYPE_MFSTRING",
	"FIELDTYPE_MFTIME",
	"FIELDTYPE_MFVEC2F",
	"FIELDTYPE_MFVEC3F",
	":",
	"\"AS\"",
	"\"COMPONENT\"",
	"\"EXPORT\"",
	"\"IMPORT\"",
	"\"META\"",
	"\"PROFILE\"",
	"\"inputOnly\"",
	"\"outputOnly\"",
	"\"inputOutput\"",
	"\"initializeOnly\"",
	"FIELDTYPE_SFCOLORRGBA",
	"FIELDTYPE_SFDOUBLE",
	"FIELDTYPE_SFVEC2D",
	"FIELDTYPE_SFVEC3D",
	"FIELDTYPE_MFBOOL",
	"FIELDTYPE_MFCOLORRGBA",
	"FIELDTYPE_MFDOUBLE",
	"FIELDTYPE_MFIMAGE",
	"FIELDTYPE_MFVEC3D",
	"FIELDTYPE_MFVEC2D",
	0
};

const unsigned long X3DVrmlParser::_tokenSet_0_data_[] = { 159662592UL, 1572864UL, 0UL, 0UL };
// ID "DEF" "EXTERNPROTO" "PROTO" "ROUTE" "USE" "EXPORT" "IMPORT" 
const ANTLR_USE_NAMESPACE(antlr)BitSet X3DVrmlParser::_tokenSet_0(_tokenSet_0_data_,4);
const unsigned long X3DVrmlParser::_tokenSet_1_data_[] = { 159662594UL, 3670016UL, 0UL, 0UL };
// EOF ID "DEF" "EXTERNPROTO" "PROTO" "ROUTE" "USE" "EXPORT" "IMPORT" "META" 
const ANTLR_USE_NAMESPACE(antlr)BitSet X3DVrmlParser::_tokenSet_1(_tokenSet_1_data_,4);
const unsigned long X3DVrmlParser::_tokenSet_2_data_[] = { 25428480UL, 0UL, 0UL, 0UL };
// ID "EXTERNPROTO" "PROTO" "ROUTE" 
const ANTLR_USE_NAMESPACE(antlr)BitSet X3DVrmlParser::_tokenSet_2(_tokenSet_2_data_,4);
const unsigned long X3DVrmlParser::_tokenSet_3_data_[] = { 1277952UL, 0UL, 0UL, 0UL };
// "eventIn" "eventOut" "exposedField" "field" 
const ANTLR_USE_NAMESPACE(antlr)BitSet X3DVrmlParser::_tokenSet_3(_tokenSet_3_data_,4);
const unsigned long X3DVrmlParser::_tokenSet_4_data_[] = { 159662592UL, 0UL, 0UL, 0UL };
// ID "DEF" "EXTERNPROTO" "PROTO" "ROUTE" "USE" 
const ANTLR_USE_NAMESPACE(antlr)BitSet X3DVrmlParser::_tokenSet_4(_tokenSet_4_data_,4);
const unsigned long X3DVrmlParser::_tokenSet_5_data_[] = { 138428960UL, 0UL, 0UL, 0UL };
// LBRACKET ID "DEF" "NULL" "USE" 
const ANTLR_USE_NAMESPACE(antlr)BitSet X3DVrmlParser::_tokenSet_5(_tokenSet_5_data_,4);
const unsigned long X3DVrmlParser::_tokenSet_6_data_[] = { 67648544UL, 0UL, 0UL, 0UL };
// LBRACKET INTEGER HEX_INTEGER REAL STRING "FALSE" "TRUE" 
const ANTLR_USE_NAMESPACE(antlr)BitSet X3DVrmlParser::_tokenSet_6(_tokenSet_6_data_,4);
const unsigned long X3DVrmlParser::_tokenSet_7_data_[] = { 208174624UL, 0UL, 0UL, 0UL };
// LBRACKET ID INTEGER HEX_INTEGER REAL STRING "DEF" "FALSE" "IS" "NULL" 
// "TRUE" "USE" 
const ANTLR_USE_NAMESPACE(antlr)BitSet X3DVrmlParser::_tokenSet_7(_tokenSet_7_data_,4);
const unsigned long X3DVrmlParser::_tokenSet_8_data_[] = { 138428928UL, 0UL, 0UL, 0UL };
// ID "DEF" "NULL" "USE" 
const ANTLR_USE_NAMESPACE(antlr)BitSet X3DVrmlParser::_tokenSet_8(_tokenSet_8_data_,4);
const unsigned long X3DVrmlParser::_tokenSet_9_data_[] = { 134234656UL, 0UL, 0UL, 0UL };
// LBRACKET ID "DEF" "USE" 
const ANTLR_USE_NAMESPACE(antlr)BitSet X3DVrmlParser::_tokenSet_9(_tokenSet_9_data_,4);


