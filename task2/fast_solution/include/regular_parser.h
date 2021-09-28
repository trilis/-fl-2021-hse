#ifndef REGULAR_PARSER_H_
#define REGULAR_PARSER_H_
#include <type_traits>

namespace hw03 {

//regex definition
	struct base_regex {
		static constexpr bool is_regex_type = true;
	}; //to check that type is a regex

	template <typename T, typename = void>
	struct is_regex : std::false_type {};

	template <typename T>
	struct is_regex <T, std::enable_if_t<T::is_regex_type>> : std::true_type {};

	#define CHECK_IF_REGEX(regex) static_assert(is_regex<regex>::value)

	struct empty_regex : base_regex {};
	struct epsilon_regex : base_regex {};

	template <char c>
	struct char_regex : base_regex {};

	template <typename regex>
	struct star_regex : base_regex {
		CHECK_IF_REGEX(regex);
	};

	template <typename regex1, typename regex2>
	struct alt_regex : base_regex {
		CHECK_IF_REGEX(regex1);
		CHECK_IF_REGEX(regex2);
	};

	template <typename regex1, typename regex2>
	struct concat_regex : base_regex {
		CHECK_IF_REGEX(regex1);
		CHECK_IF_REGEX(regex2);
	};
	static_assert(!is_regex<int>::value);

	template <typename regex>
	struct is_usual_regex : std::true_type {};
	template <>
	struct is_usual_regex<empty_regex> : std::false_type {};
	template <>
	struct is_usual_regex<epsilon_regex> : std::false_type {};
	template <typename regex>
	static constexpr bool is_usual_regex_v = is_usual_regex<regex>::value;

	template <typename regex1, typename regex2>
	struct get_usual {
		using result_type = regex1;
	};

	template <typename regex>
	struct get_usual<empty_regex, regex> {
		using result_type = regex;
	};

	template <typename regex>
	struct get_usual<epsilon_regex, regex> {
		using result_type = regex;
	};

	template <typename regex1, typename regex2>
	using get_usual_t = typename get_usual<regex1, regex2>::result_type;

//regex definition
	
//intersect
	template <typename regex1, typename regex2>
	struct intersect {
		using result_type = std::enable_if_t<std::is_same_v<regex1, empty_regex> ||
		                                     std::is_same_v<regex2, empty_regex>, empty_regex>;
	};

	template<>
	struct intersect<epsilon_regex, epsilon_regex> {
		using result_type = epsilon_regex;
	};

	template <typename regex1, typename regex2>
	using intersect_v = typename intersect<regex1, regex2>::result_type;
//intersect

//union
	template <typename regex1, typename regex2>
	struct union_r {
		using result_type = std::enable_if_t<std::is_same_v<regex1, epsilon_regex> ||
		                                     std::is_same_v<regex2, epsilon_regex>, epsilon_regex>;
	};

	template <>
	struct union_r <empty_regex, empty_regex> {
		using result_type = empty_regex;
	};

	template <typename regex1, typename regex2>
	using union_v =  typename union_r<regex1, regex2>::result_type;
//union

//nullable
	template <typename regex>
	struct nullable {
		using result_type = void;
	};

	template <typename regex>
	using nullable_v = typename nullable<regex>::result_type;

	template<>
	struct nullable <empty_regex> {
		using result_type = empty_regex;
	};

	template<>
	struct nullable <epsilon_regex> {
		using result_type = epsilon_regex;
	};

	template<char symb>
	struct nullable <char_regex<symb>> {
		using result_type = empty_regex;
	};


	template<typename regex1, typename regex2> 
	struct nullable <concat_regex<regex1, regex2>> {
		using result_type = intersect_v<nullable_v<regex1>, nullable_v<regex2>>;
	};

	template<typename regex1, typename regex2> 
	struct nullable <alt_regex<regex1, regex2>> {
		using result_type = union_v<nullable_v<regex1>, nullable_v<regex2>>;
	};

	template<typename regex> 
	struct nullable <star_regex<regex>> {
		using result_type = epsilon_regex;
	};

	template <typename regex>
	struct nullable_bool : std::false_type {};

	template <typename regex>
	static constexpr bool nullable_bool_v = nullable_bool<regex>::value;

	template<>
	struct nullable_bool <empty_regex> : std::false_type {};

	template<>
	struct nullable_bool <epsilon_regex> : std::true_type {};

	template<char symb>
	struct nullable_bool <char_regex<symb>> : std::false_type {};


	template<typename regex1, typename regex2> 
	struct nullable_bool<concat_regex<regex1, regex2>> : std::bool_constant<
	                                                     nullable_bool_v<regex1> && 
	                                                     nullable_bool_v<regex2>> {};

	template<typename regex1, typename regex2> 
	struct nullable_bool <alt_regex<regex1, regex2>> : std::bool_constant<
	                                                     nullable_bool_v<regex1> || 
	                                                     nullable_bool_v<regex2>> {};

	template<typename regex> 
	struct nullable_bool <star_regex<regex>> : std::true_type {};
//nullable

//derivative
	template <char symb, typename regex, typename = void> 
	struct derivative { 
		using result_type = void; 
	};

	template <char symb, typename regex>
	using derivative_v = typename derivative<symb, regex>::result_type;

	template <char symb> 
	struct derivative <symb, empty_regex> { 
		using result_type = empty_regex; 
	};

	template <char symb> 
	struct derivative <symb, epsilon_regex> { 
		using result_type = empty_regex; 
	};

	template <char symb1, char symb2> 
	struct derivative <symb1, char_regex<symb2>> { 
		using result_type = empty_regex; 
	};

	template <char symb> 
	struct derivative <symb, char_regex<symb>> { 
		using result_type = epsilon_regex; 
	};

//alt_regex

	template <char symb, typename regex1, typename regex2>
	struct derivative <symb, alt_regex<regex1, regex2>, std::enable_if_t<!std::is_same_v<regex1, empty_regex> &&
	                                                                     !std::is_same_v<regex2, empty_regex> &&
	                                                                     !std::is_same_v<regex1, regex2>>> {
		using result_type = alt_regex<derivative_v<symb, regex1>, derivative_v<symb, regex2>>;
	};

	template <char symb, typename regex>
	struct derivative <symb, alt_regex<regex, empty_regex>> {
		using result_type = derivative_v<symb, regex>;
	};

	template <char symb, typename regex>
	struct derivative <symb, alt_regex<empty_regex, regex>> {
		using result_type = derivative_v<symb, regex>;
	};

	template <char symb>
	struct derivative <symb, alt_regex<empty_regex, empty_regex>> {
		using result_type = empty_regex;
	};

	template <char symb, typename regex>
	struct derivative <symb, alt_regex<regex, regex>, std::enable_if_t<!std::is_same_v<regex, empty_regex>>> {
		using result_type = derivative_v<symb, regex>;
	};
//alt_regex

//star_regex
	template<char symb, typename regex>
	struct derivative <symb, star_regex<regex>> {
		using result_type = concat_regex<derivative_v<symb, regex>, star_regex<regex>>;
	};

	template <char symb>
	struct derivative <symb, star_regex<epsilon_regex>> {
		using result_type = epsilon_regex;
	};

	template <char symb>
	struct derivative <symb, star_regex<empty_regex>> {
		using result_type = empty_regex;
	};

	template<char symb, typename regex>
	struct derivative <symb, star_regex<star_regex<regex>>> {
		using result_type = derivative_v<symb, star_regex<regex>>;
	};
//star_regex

//concat_regex

	template<char symb, typename regex1, typename regex2> 
	struct derivative <symb, concat_regex<regex1, regex2>, 
	                   std::enable_if_t<is_usual_regex_v<regex1> && is_usual_regex_v<regex2>>> {
		using result_type = alt_regex< concat_regex<derivative_v<symb, regex1>, regex2>, 
		                               concat_regex<nullable_v<regex1>, derivative_v<symb, regex2>>>;
	};

	template <char symb, typename regex1, typename regex2>
	struct derivative<symb, concat_regex<regex1, regex2>,
							std::enable_if_t <
	                        std::is_same_v<regex1, empty_regex> ||
	                        std::is_same_v<regex2, empty_regex>
	                        >> {
		using result_type = empty_regex;
	};

	template <char symb, typename regex> 
	struct derivative <symb, concat_regex <epsilon_regex, regex>,
	                                       std::enable_if_t<!std::is_same_v<regex, empty_regex>> > {
		using result_type = derivative_v<symb, regex>;
	};

	template <char symb, typename regex> 
	struct derivative <symb, concat_regex <regex, epsilon_regex>,
	                                       std::enable_if_t<!std::is_same_v<regex, empty_regex> &&
	                                                        !std::is_same_v<regex, epsilon_regex>> > {
		using result_type = derivative_v<symb, regex>;
	};
//concat_regex

//derivative

namespace all_instantations {
	derivative_v <'a', alt_regex<empty_regex, epsilon_regex>> kek1_;
	derivative_v <'a', alt_regex<empty_regex, empty_regex>> kek2_;
	derivative_v <'a', alt_regex<epsilon_regex, empty_regex>> kek3_;
	derivative_v <'a', alt_regex<empty_regex, star_regex<epsilon_regex>>> kek4_;
	derivative_v <'a', alt_regex<star_regex<epsilon_regex>, empty_regex>> kek5_;
	derivative_v <'a', alt_regex<epsilon_regex, star_regex<epsilon_regex>>> kek6_;
	derivative_v <'a', alt_regex<star_regex<epsilon_regex>, epsilon_regex>> kek7_;
	derivative_v <'a', alt_regex<epsilon_regex, epsilon_regex>> kek8_;
}

namespace all_instantations {
	derivative_v <'a', concat_regex<empty_regex, epsilon_regex>> kek1;
	derivative_v <'a', concat_regex<empty_regex, empty_regex>> kek2;
	derivative_v <'a', concat_regex<epsilon_regex, empty_regex>> kek3;
	derivative_v <'a', concat_regex<empty_regex, star_regex<epsilon_regex>>> kek4;
	derivative_v <'a', concat_regex<star_regex<epsilon_regex>, empty_regex>> kek5;
	derivative_v <'a', concat_regex<epsilon_regex, star_regex<epsilon_regex>>> kek6;
	derivative_v <'a', concat_regex<star_regex<epsilon_regex>, epsilon_regex>> kek7;
	derivative_v <'a', concat_regex<epsilon_regex, epsilon_regex>> kek8;
} //check that all specializations are correct

//long_derivative
	template <typename regex, char first, char ...tail>
	struct long_derivative {
		using result_type = typename long_derivative<derivative_v<first, regex>, tail...>::result_type;
	};

	template <typename regex, char first>
	struct long_derivative<regex, first> {
		using result_type = derivative_v<first, regex>;
	};

	template <typename regex, char first, char ...tail>
	using long_derivative_v = typename long_derivative<regex, first, tail...>::result_type;
//long_derivative

//match
	template <typename regex, char first, char ...word>
	struct match {
		CHECK_IF_REGEX(regex);
		static constexpr bool result = nullable_bool_v<long_derivative_v<regex, first, word...>>;
	};

	template <typename regex, char first, char ...word>
	constexpr bool match_v = match<regex, first, word...>::result;
//match

} //hw03

#endif //REGULAR_PARSER_H_