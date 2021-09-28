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
//nullable

//derivative
	template <char symb, typename regex> 
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

	template <char symb, typename regex1, typename regex2>
	struct derivative <symb, alt_regex<regex1, regex2>> {
		using result_type = alt_regex<derivative_v<symb, regex1>, derivative_v<symb, regex2>>;
	};

	template<char symb, typename regex>
	struct derivative <symb, star_regex<regex>> {
		using result_type = concat_regex<derivative_v<symb, regex>, star_regex<regex>>;
	};

	template<char symb, typename regex1, typename regex2> 
	struct derivative <symb, concat_regex<regex1, regex2>> {
		using result_type = alt_regex< concat_regex<derivative_v<symb, regex1>, regex2>, 
		                               concat_regex<nullable_v<regex1>, derivative_v<symb, regex2>>>;
	};
//derivative

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
		static constexpr bool result = std::is_same_v<nullable_v<long_derivative_v<regex, first, word...>>, epsilon_regex>;
	};

	template <typename regex, char first, char ...word>
	constexpr bool match_v = match<regex, first, word...>::result;
//match

} //hw03

#endif //REGULAR_PARSER_H_