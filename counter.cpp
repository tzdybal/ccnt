#include "counter.h"

#include <iostream>
#include <algorithm>

#include <QtGui/QColor>
#include <QtGui/QPainter>


namespace ccnt {

Counter::Counter(QImage& img) : img(img) {}

void Counter::findColonies() {
	// find connected components
	auto components = findConnectedComponents();

	auto circles = findBoundingCircles(components);

	//uncertainColonies = circles;
}

std::vector<Colony> Counter::getColonies(ConfidenceLevel level) {
	std::vector<Colony> colonies;
	switch (level) {
	case ConfidenceLevel::CERTAIN:
		colonies = certainColonies;
		break;
	case ConfidenceLevel::UNCERTAIN:
		colonies = uncertainColonies;
		break;
	case ConfidenceLevel::ALL:
	default:
		colonies = certainColonies;
		colonies.insert(colonies.end(), uncertainColonies.begin(), uncertainColonies.end());
	}

	return colonies;
}

std::pair<size_t, Counter::ComponentsMatrix> Counter::findConnectedComponents()
{
	size_t w = img.width();
	size_t h = img.height();

	std::vector<std::vector<int>> components(h);
	for (auto& row : components) row.resize(w, 0);


	int threshold = 5;// TODO extract parameter
	auto value = [this](size_t x, size_t y) { return qGray(img.pixel(y, x)); };
	auto sameComponent = [this,&value,threshold](int v, size_t x, size_t y) {
		return img.valid(y, x) && abs(v - value(x, y)) < threshold;

	};

	int n = 1;
	components[0][0] = n;

	std::map<int, int> equivalence;

	for (size_t r = 0; r < h; ++r) {
		for (size_t c = 0; c < w; ++c) {
			int v = value(r, c);
			int cmp = -1;
			if (sameComponent(v, r, c-1)) {
				cmp = components[r][c-1];
				if (sameComponent(v, r-1, c)) {
					auto current = equivalence.find(cmp);
					auto previous = equivalence.find(components[r-1][c]);
					if (previous->second < current->second) {
						current->second = previous->second;
					}
				}
			} else if (sameComponent(v, r-1, c)) {
				cmp = components[r-1][c];
				if (sameComponent(v, r, c-1)) {
					auto current = equivalence.find(cmp);
					auto previous = equivalence.find(components[r][c-1]);
					if (previous->second < current->second) {
						current->second = previous->second;
					}
				}
			} else {
				cmp = ++n;
				equivalence[cmp] = cmp;
			}


			components[r][c] = cmp;
		}
	}

	for (size_t r = 0; r < h; ++r) {
		for (size_t c = 0; c < w; ++c) {
			components[r][c] = equivalence[components[r][c]];
		}
	}

	std::cout << n << std::endl;
	return std::make_pair(n, components);
}

std::vector<Counter::Circle> Counter::findBoundingCircles(const std::pair<size_t, ComponentsMatrix>& matrix)
{
	size_t w = img.width();
	size_t h = img.height();

	std::vector<size_t> cnt(matrix.first);
	std::vector<Circle> circles(matrix.first);

	QPainter painter(&img);

	// calculate weight center
	for (size_t r = 0; r < h; ++r) {
		for (size_t c = 0; c < w; ++c) {
			size_t n = matrix.second[r][c]-1;
			++cnt[n];
			circles[n].x += c;
			circles[n].y += r;
			painter.setPen(QPen(QColor(n*10%255, 0, 0)));
			painter.drawPoint(c, r);
		}
	}

	for (size_t i = 0; i < circles.size(); ++i) {
		if (cnt[i] != 0) {
			circles[i].x /= cnt[i];
			circles[i].y /= cnt[i];
			circles[i].r = 10;
		}
	}

	return circles;
}

}
