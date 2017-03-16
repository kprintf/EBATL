#ifndef EBA_MAP_HPP
#	define EBA_MAP_HPP
#	include "bst_rb.hpp"
namespace eba {
	template<typename TKey, typename TValue> struct map_pair {
		TKey first;
		TValue second;
		map_pair(TKey a, TValue b): first(a), second(b) {}
		map_pair(TKey a): first(a), second() {}
		bool operator < (const map_pair &a) const{
			return first < a.first;
		}
	};
	template<typename TKey, typename TValue, typename bst = bst_rb<map_pair<TKey, TValue> > > class map: public bst {
		public:
			map() {
				beep();
			}
		int count (TKey key) {
			return bst::find(key);
		}
		TValue& operator[] (TKey key) {
			return bst::path(map_pair<TKey, TValue>(key), true)->val.second;
		}
		void erase(TKey key) {
			bst::remove(map_pair<TKey, TValue>(key));
		}
	};
}
#endif
