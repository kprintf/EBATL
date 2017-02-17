#ifndef EBA_VECTOR_HPP
#       define EBA_VECTOR_HPP
#       ifndef EBA_NOSTL
#               include <vector>
#       endif
#	include "common.hpp"
namespace eba {
#	ifndef EBA_NOSTL
	template<typename T> using vector = std::vector<T> ;
#	else
	template<typename T> class vector {
		int size, capacity;
		T *content;
		static const double constexpr GROWTH_COEFF = 2.0;
		void realloc(int new_cap) {
			capacity = new_cap;
			T *new_content = new T[capacity];
			for(int i = 0; i < size; i++)
				new_content[i] = content[i];
			delete content;
			content = new_content;
		}
		public:
			T& operator[](int i) {
				return content[i];
			}
			vector() {
				content = new T[4];
				capacity = 4;
				size = 0;
			}
			vector(unsigned int n, T fill=T()) {
				content = new T[n];
				capacity = n;
				size = n;
				for(int i = 0; i < size; i++)
					content[i] = fill;
			}
			vector(const vector &v): capacity(v.capacity), size(v.size) {
				content = new T[capacity];
				for(int i = 0; i < size; i++)
					content[i] = v.content[i];
			}
			~vector() {
				delete[] content;
				content = 0;
			}
			void push_back(T t) {
				if(size == capacity) {
					realloc(capacity*GROWTH_COEFF);
				}
				content[size++] = t;
			}
			void shrink_to_fit() {
				realloc(size);
			}
		      	class iterator {
				T *ptr;
				public:
				iterator(T* n): ptr(n) {}
				void operator++() {
					ptr++;
				}
				void operator--() {
					ptr--;
				}
				T& operator*() {
					return *ptr;
				}
				bool operator ==(const iterator &a) {
					return ptr == a.ptr;
				}
				bool operator !=(const iterator &a) {
					return ptr != a.ptr;
				}
			};
			iterator begin() {
				return iterator(&content[0]);
			}
			iterator end() {
				return iterator(&content[size]);
			}
			vector& operator = (const vector& v) {
				capacity = v.capacity;
				size = v.size;
				content = new T[capacity];
				for(int i = 0; i < size; i++)
					content[i] = v.content[i];
			}
	};
#	endif
};
#endif
