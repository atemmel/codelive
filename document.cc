#include "document.h"
#include <iostream>

using namespace Document;

void Builder::apply(const Delta &delta) {

	Index current;

	//auto step = [&](

	auto ins = [&](auto it){
		if(delta.content.empty() ) {
			if(delta.type == DiffType::Insert) {
				_str.insert(it, '\n');
			} else if(delta.type == DiffType::Delete) {
				auto it2 = it;
				Index next = current;
				for(; it2 != _str.end(); it2++) {
					if(next == delta.end) {
						break;
					}

					++next.col;
					if(*it2 == '\n') {
						next.col = 0;
						++next.row;
					}
				}

				std::cout << "Erasing from: " << std::string(it, it2) << '\n';
				_str.erase(it, it2);
			} else {
				return;
			}
		} else {
			_str.insert(it, delta.content.front() );
		}
		std::cout << "Document:\n" << _str << '\n';
	};

	for(auto it = _str.begin(); it != _str.end(); it++) {
		if(current == delta.start) {
			ins(it);
			return;
		}

		++current.col;
		if(*it == '\n') {
			current.col = 0;
			++current.row;
		}
	}

	ins(_str.end() );

	std::cout << "Row: " << delta.start.row << " Col:" << delta.start.col << '\n';
	std::cout << "CRow: " << current.row << " CCol:" << current.col << '\n';
}

const std::string &Builder::str() const {
	return _str;
}
