#pragma once
#include <string>

namespace Document {

struct Index {
	int col = 0,
		row = 0;

	constexpr bool operator==(const Index &rhs) {
		return col == rhs.col && row == rhs.row;
	}
};

enum DiffType {
	Insert = 0,
	Delete,
	Remove
};

struct Delta {
	Index start, end;
	std::string content;
	DiffType type;
};

class Builder {
public:
	void apply(const Delta &delta);

	bool save(const std::string &path) const;

	const std::string &str() const;
private:
	std::string _str;
};

};
