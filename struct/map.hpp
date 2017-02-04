#ifndef EBA_MAP_HPP
#	define EBA_MAP_HPP
#	include "bst_rb.hpp"
namespace eba {
	template<typename TKey, typename TValue> struct pair {
		TKey first;
		TValue second;
		pair(TKey a, TValue b): first(a), second(b) {}
		pair(TKey a): first(a) {}
		bool operator < (const pair &a) {
			return first < a.first;
		}
	};
	template<typename TKey, typename TValue, typename bst = bst_rb<pair<TKey, TValue> > > class map: bst {
		int count (TKey key) {
			return bst::find(key);
		}
		TValue& operator[] (TKey key) {
			return path(pair<TKey, TValue>(key), true)->val.second;
		}
	};
}
#endif
