#ifndef EBA_ARRAY_HPP
#	define EBA_ARRAY_HPP
#	include "common/common.hpp"
#	include <memory>
#	include <utility>
#	include <cstring>
/* this is HAT. Hashed Array Tree
 * differ from vector by memory compsumtion on reallocation. O(sqrt{N}) vs O(N).
 * cost is double pointer jump per every access.
 * */
namespace eba {
	template<typename T> class array {
		size_t sz, pow;
		T** data;
		T*  new_page(size_t pow) {
			char *page = new char[sizeof(T) * (1<<pow)];
			::bzero(page, sizeof(T) * (1<<pow));
			return reinterpret_cast<T*>(page);
		}
		T*  dup_page(const T* src, size_t count) {
			char *page = new char[sizeof(T) * (1<<pow)];
			if(count> 1<<pow) count = 1<<pow;
			for(size_t ind = 0; ind < count; ind++)
				new(&page[sizeof(T)*ind]) T(src[ind]);
			::bzero(&page[sizeof(T)*count], sizeof(T) * ((1<<pow)-count));
			return reinterpret_cast<T*>(page);
		}
		void del_page(size_t page) { //calls destructor if object exist
			if(sz > page * (size_t(1)<<pow))
				for(size_t ind = 0; ind < min(size_t(1)<<pow, sz - page*(1<<pow)); ind++)
					data[page][ind].~T();
			delete[] reinterpret_cast<char*>(data[page]);
		}
		void reallocate() {
			T **new_data = new T*[1<<(pow+1)];
			::bzero(new_data, sizeof(T*)*(1<<(pow+1)));
			if(pow)
			for(size_t ind = 0; ind < size_t(1)<<(pow-1); ind++)
				new_data[ind] = new_page(pow+1);
			if(data) {
				for(size_t page = 0; page < 1<<(pow-1); page++) {
					if(!data[page*2]) break;
					for(size_t ind = 0; ind < 1<<pow; ind++)
						new(&new_data[page][ind]) T(::std::move(data[page*2][ind]));
					del_page(page*2);	
					if(data[page*2 + 1]) {
						for(size_t ind = 0; ind < 1<<pow; ind++)
							new(&new_data[page][ind + (1<<pow)]) 
							  T(::std::move(data[page*2+1][ind]));
						del_page(page*2+1);
					}
				}
				delete[] data;
			}
			pow++;
			data = new_data;
		}
		public:
		array(): sz(0), pow(0), data(nullptr) {};
		array(size_t sz) {
			while(sz --> 0) push_back(T());
		}
		array(size_t sz, T& fill) {
			while(sz --> 0) push_back(fill);
		}
		array(const array& copy) {
			pow = copy.pow;
			sz  = copy.sz;
			if(sz) {
				data = new T*[1<<(pow)];
				::bzero(data, sizeof(T*)*(1<<(pow)));
				for(size_t page = 0; page <= (sz-1) >> pow; page++)
					data[page] = dup_page(copy.data[page], sz - (page<<pow));
			}
			else data = nullptr;
		}
		~array() {
			if(data && sz) {
				for(size_t i = 0; i <= (sz-1)>>pow; i++)
					del_page(i);
				delete[] data;
			}
		}
		size_t size() const {
			return sz;
		}
		bool empty() const {
			return !sz;
		}
		T& at(size_t ind) {
			return data[ind >> pow][ind & ((1<<pow)-1)];
		}
		void push_back(const T& el) {
			if(!pow || sz == size_t(1)<<(2*pow))
				reallocate();
			if(!data[sz >> pow]) data[sz >> pow] = new_page(pow);
			at(sz++) = el;
		}
		T& operator[] (size_t ind) {
			return at(ind);
		}
		class iterator {
			size_t ind;
			array& src;
			public:
			iterator(size_t x, array& y): ind(x), src(y) {}
			bool operator != (const iterator& it) {
				return ind != it.ind;
			}
			T& operator*() {
				return src[ind];
			}
			T& operator->() {
				return src[ind];
			}
			void operator++() {
				ind++;
			}
			void operator--() {
				ind--;
			}
		};
		iterator begin() {
			return iterator(0, *this);
		}
		iterator end() {
			return iterator(sz, *this);
		}
	};
}
#endif
