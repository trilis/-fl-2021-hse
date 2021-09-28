#include "regular_parser.h"
#include <iostream>

using namespace hw03;

namespace hw_tests {
	using REGEX1 = concat_regex<star_regex<concat_regex<char_regex<'a'>, char_regex<'b'>>>, char_regex<'a'>>; 
	//(ab)*a

	static_assert(match_v<REGEX1, 'a', 'b', 'a', 'b', 'a'>); //ababa
	static_assert(!match_v<REGEX1, 'a', 'b', 'a', 'b', 'a', 'b'>); //ababab
	static_assert(match_v<REGEX1, 'a'>); //a

	using REGEX2 = alt_regex< concat_regex<concat_regex< char_regex<'a'>, char_regex<'b'>>, char_regex<'c'> >, 
	                                    star_regex<char_regex<'z'>> >;
	// (abc|z*)

	static_assert(match_v<REGEX2, 
		                  'z', 'z', 'z'>);
	static_assert(match_v<REGEX2, 
		                  'a', 'b', 'c'>);
	static_assert(nullable_bool_v<REGEX2>);
	static_assert(nullable_bool_v<star_regex<REGEX2>>);
	static_assert(match_v<star_regex<REGEX2>, 
		                  'z', 'z', 'a', 'b', 'c', 'z', 'z', 'a', 'b', 'c', 'z', 'z'>);
	static_assert(!match_v<star_regex<REGEX2>, 'a', 'b'>);
	static_assert(!match_v<star_regex<REGEX2>, 'f'>);
	static_assert(match_v<star_regex<star_regex<char_regex<'z'>>>, 'z', 'z', 'z'>);

	using REGEX3 = alt_regex <
	                          star_regex<alt_regex<
	                                     concat_regex<
	                                                  char_regex<'a'>, 
	                                                  char_regex<'b'>>, 
	                                     concat_regex<
	                                                  char_regex<'c'>, 
	                                                  char_regex<'d'>>>>, 
	                          star_regex<alt_regex<
	                                     concat_regex<
	                                                  char_regex<'a'>, 
	                                                  char_regex<'c'>>, 
	                                     concat_regex<
	                                                  char_regex<'b'>, 
	                                                  char_regex<'d'>>>>>;
	// ((ab|cd)*)|((ac|bd)*)

	static_assert(match_v<REGEX3, 'a', 'b'>);
	static_assert(match_v<REGEX3, 'c', 'd'>);
	static_assert(match_v<REGEX3, 'a', 'c'>);
	static_assert(match_v<REGEX3, 'b', 'd'>);
	static_assert(!match_v<REGEX3, 'a', 'b', 'a', 'c'>);
	static_assert(!match_v<REGEX3, 'a', 'b', 'b', 'd'>);
	static_assert(!match_v<REGEX3, 'a', 'b', 'b', 'd'>);
	static_assert(match_v<REGEX3, 'a', 'b', 'c', 'd', 'a', 'b', 'c', 'd', 'c', 'd', 'c', 'd', 'a', 'b', 'a', 'b'>);
	static_assert(!match_v<REGEX3, 'a', 'b', 'c', 'd', 'a', 'b', 'c', 'd', 'c', 'd', 'c', 'd', 'a', 'b', 'a', 'c'>);

	using REGEX4 = star_regex<REGEX3>;
	static_assert(match_v<REGEX4, 'a', 'b', 'c', 'd'>);
	static_assert(match_v<REGEX4, 'a', 'b', 'c', 'd', 'a', 'c'>);
	static_assert(!match_v<REGEX4, 'a', 'b', 'c', 'd', 'a', 'd'>);
	static_assert(match_v<REGEX4, 'a', 'b', 'c', 'd', 'a', 'b', 'c', 'd', 'c', 'd', 'c', 'd', 'a', 'b', 'a', 'c'>);

	using REGEX5 = concat_regex < 
		                star_regex<
		                          star_regex<
		                                     star_regex<
		                                                star_regex<
		                                                           char_regex<'z'>
		                                                >
		                                      >
		                          >
		                >,
		                star_regex<
		                          star_regex<
		                                     star_regex<
		                                                star_regex<
		                                                           char_regex<'z'>
		                                                >
		                                      >
		                          >
		                >
	                >;

	static_assert(match_v<REGEX5, 'z', 'z', 'z', 'z', 'z', 'z', 'z'>);


	static_assert(!match_v<REGEX3, 'a', 'b', 'c', 'd', 'a', 'b', 'c', 'd', 'c', 'd', 'c', 'd', 'a', 'b',
	                               'a', 'b', 'c', 'd', 'a', 'b', 'c', 'd', 'c', 'd', 'c', 'd', 'a', 'b',
	                               'a', 'c', 'b', 'd', 'a', 'c', 'b', 'd', 'a', 'c', 'b', 'd', 'b', 'd',
	                               'a', 'c', 'b', 'd', 'a', 'c', 'b', 'd', 'a', 'c', 'b', 'd', 'b', 'd',
	                               'a', 'c', 'b', 'd', 'a', 'c', 'b', 'd', 'a', 'c', 'b', 'd', 'b', 'd'>);


	//big test
	using AB = concat_regex <char_regex<'a'>, char_regex<'b'>>;
	using BC = concat_regex <char_regex<'b'>, char_regex<'c'>>;
	using CD = concat_regex <char_regex<'c'>, char_regex<'d'>>;
	using DE = concat_regex <char_regex<'d'>, char_regex<'e'>>;
	using EF = concat_regex <char_regex<'e'>, char_regex<'f'>>;
	using FG = concat_regex <char_regex<'f'>, char_regex<'g'>>;
	using GH = concat_regex <char_regex<'g'>, char_regex<'h'>>;
	using big_typ =  concat_regex<concat_regex<concat_regex<star_regex<concat_regex<concat_regex< star_regex<alt_regex<AB, BC>>, star_regex<alt_regex<BC, CD>>>, star_regex<alt_regex<CD, DE>>>>, star_regex<alt_regex<DE, EF>>>, star_regex<alt_regex<EF, FG>>>, star_regex<alt_regex<FG, GH>>>;
	static_assert(match_v<big_typ, 'a', 'b', 'b', 'c', 'a', 'b', 'b', 'c', 'a', 'b', 'b', 'c', 'a', 'b', 'b', 'c', 'a', 'b', 'b', 'c', 'a', 'b', 'b', 'c', 'a', 'b', 'b', 'c', 'a', 'b', 'b', 'c',
		                            'b', 'c', 'c', 'd', 'b', 'c', 'c', 'd', 'b', 'c', 'c', 'd', 'b', 'c', 'c', 'd', 'b', 'c', 'c', 'd', 'b', 'c', 'c', 'd',
		                            'c', 'd', 'd', 'e', 'c', 'd', 'd', 'e', 'c', 'd', 'd', 'e', 'c', 'd', 'd', 'e', 'c', 'd', 'd', 'e', 'c', 'd', 'd', 'e', 'c', 'd', 'd', 'e', 'c', 'd', 'd', 'e',
		                            'd', 'e', 'e', 'f', 'd', 'e', 'e', 'f', 'd', 'e', 'e', 'f', 'd', 'e', 'e', 'f', 'd', 'e', 'e', 'f', 'd', 'e', 'e', 'f', 'd', 'e', 'e', 'f', 'd', 'e', 'e', 'f',
		                            'e', 'f', 'f', 'g', 'e', 'f', 'f', 'g', 'e', 'f', 'f', 'g', 'e', 'f', 'f', 'g',
		                            'f', 'g', 'g', 'h', 'f', 'g', 'g', 'h', 'f', 'g', 'g', 'h', 'f', 'g', 'g', 'h', 'f', 'g', 'g', 'h'>);
} //tests

int main() {

}