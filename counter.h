#pragma once

#include <vector>
#include <utility>

#include <QtGui/QImage>

namespace ccnt {

struct Colony {
	size_t x;
	size_t y;
	size_t r;
};

enum class ConfidenceLevel {
	CERTAIN, UNCERTAIN, ALL
};

class Counter {
public:
	Counter(QImage& img);
	void findColonies(); // this is time consuming operation
	std::vector<Colony> getColonies(ConfidenceLevel level);
private:
	QImage& img;
	std::vector<Colony> certainColonies;
	std::vector<Colony> uncertainColonies;

	typedef std::vector<std::vector<int>> ComponentsMatrix;
	std::pair<size_t, ComponentsMatrix> findConnectedComponents();
	typedef Colony Circle;
	std::vector<Circle> findBoundingCircles(const std::pair<size_t, ComponentsMatrix>& matrix);
};

}
