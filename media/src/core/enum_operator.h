#ifndef src_core_enum_operator_h_
#define src_core_enum_operator_h_

#include <type_traits>

namespace LJMP {
	template <typename E>
	constexpr auto enum_cast(E e) noexcept {
		return static_cast<std::underlying_type_t<E>>(e);
	}

	template <typename E>
	constexpr std::enable_if_t<std::is_enum<E>::value, E> operator| (E lhs, E rhs) noexcept {
		return E(enum_cast(lhs) | enum_cast(rhs));
	}

	template <typename E>
	constexpr std::enable_if_t<std::is_enum<E>::value, E&> operator|= (E& lhs, E rhs) noexcept {
		lhs = E(enum_cast(lhs) | enum_cast(rhs));
		return lhs;
	}

	template <typename E>
	constexpr std::enable_if_t<std::is_enum<E>::value, E> operator& (E lhs, E rhs) noexcept {
		return E(enum_cast(lhs) & enum_cast(rhs));
	}

	template <typename E>
	constexpr std::enable_if_t<std::is_enum<E>::value, E> operator&= (E& lhs, E rhs) noexcept {
		lhs = E(enum_cast(lhs) & enum_cast(rhs));
		return lhs;
	}

	template <typename E>
	constexpr std::enable_if_t<std::is_enum<E>::value, E> operator^ (E lhs, E rhs) noexcept {
		return E(enum_cast(lhs) ^ enum_cast(rhs));
	}

	template <typename E>
	constexpr std::enable_if_t<std::is_enum<E>::value, E> operator^= (E& lhs, E rhs) noexcept {
		lhs = E(enum_cast(lhs) ^ enum_cast(rhs));
		return lhs;
	}

	template <typename E>
	constexpr std::enable_if_t<std::is_enum<E>::value, E> operator~ (E op) noexcept {
		return E(~enum_cast(op));
	}
}

#endif // ! src_core_enum_operator_h_
