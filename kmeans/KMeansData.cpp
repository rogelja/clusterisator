/*
 * KMeansData.cpp
 *
 *  Created on: 9 août 2012
 *      Author: manuel
 */

#include "KMeansData.hpp"
#include "IndexedList.hpp"
KMeansData::KMeansData(size_t const & n, size_t const & m, size_t const & k) :
		RawData(n, m), _clusters(k) {
	buildDiag();
}

KMeansData::~KMeansData() {
}

void KMeansData::buildDiag() {
	_kii.assign(getN(), 0);
	for (size_t i(0); i < getN(); ++i)
		_kii[i] += get(i);
}
void KMeansData::init(IntVector const & x) {
	_x = x;
	_clusters.assign(_clusters.size(), ClusterData());
	for (size_t i(0); i < getN(); ++i) {
		size_t const c(_x[i]);
		_clusters[c].weight += _weights[i];
		_clusters[c].list.push_front(i);
		_position[i] = _clusters[c].list.begin();
	}
}
void KMeansData::setWeights(DoubleVector const & weights) {
	_weights = weights;
}
size_t KMeansData::getK() const {
	return _clusters.size();
}

double KMeansData::getIntra(size_t const & c) const {
	double result(0);
	for (auto const & i : _clusters[c].list) {
		for (auto const & j : _clusters[c].list) {
			if (i < j) {
				result += 2 * _weights[i] * _weights[j] * get(i, j);
			}
		}
	}
	return result / (_clusters[c].weight * _clusters[c].weight);
}
void KMeansData::getIntra() {
	for (size_t k(0); k < getK(); ++k)
		_clusters[k].intra = getIntra(k);
}
double KMeansData::getLinearPart(size_t const & i, size_t const & c) const {
	double result(0);
	for (auto const & j : _clusters[c].list) {
		result -= (2 * _weights[j] * get(i, j));
	}
	return result / _clusters[c].weight;

}
double KMeansData::getD(size_t const & i, size_t const & c) const {
	double result(0);
	result += _kii[i];
	result += getLinearPart(i, c);
	result += getIntra(c);

	return result;
}
void KMeansData::initLoop() {
	getIntra();
//	_buffer.assign(getK(), 0);
//	for (size_t k(0); k < getK(); ++k) {
//
//	}
}
void KMeansData::move(size_t const & i, size_t const & c) {
	if (_x[i] != c) {
		_clusters[_x[i]].weight -= _weights[i];
		_clusters[_x[i]].list.erase(_position[i]);

		_clusters[c].weight += _weights[i];
		_clusters[c].list.push_back(i);

		_position[i] = _clusters[c].list.begin();
		_x[i] = c;
	}
}

size_t KMeansData::getClosest(size_t const & i) {
	size_t bestC(getK());
	double minD(std::numeric_limits<double>::max());
	for (size_t c(0); c < getK(); ++c) {
		double const d(getD(i, c));
		if (d < minD) {
			bestC = c;
			minD = d;
		}
	}
	return bestC;
}
bool KMeansData::loop(size_t const & maxIte) {
	IndexedList all(getK(), true);
	return loop(all, maxIte);

}
bool KMeansData::loop(IndexedList const & sub, size_t const & maxIte) {
	bool improvement(false);
	std::vector<std::pair<size_t, size_t> > moves;
	moves.reserve(getN());
	size_t ite(0);
	do {
		++ite;
		moves.clear();
		initLoop();
		for (auto const & c : sub) {
			for (auto const & i : _clusters[c].list) {
				size_t const newC(getClosest(i));
				if (newC != _x[i])
					moves.push_back(std::make_pair(i, newC));
			}
		}
		for (auto const & m : moves)
			move(m.first, m.second);
		if (!moves.empty())
			improvement = true;
	} while (!moves.empty() && ite < maxIte);
	return improvement;
}