/*
 * Cut.cpp
 *
 *  Created on: 6 août 2012
 *      Author: manuel
 */

#include "Cut.hpp"
#include "IExtendedPartition.hpp"
#include "IGraph.hpp"

void Cut::getCut(IExtendedPartition const & data, DoubleVector & cut) const {
	cut.assign(data.nbNodes(), 0);
	for (size_t n(0); n < data.nbNodes(); ++n) {
		size_t const & l(data.label(n));
		FOR_EACH_CONST(e , data.graph().adjacentList(n)) {
			if (data.label(e.first) != l)
				cut[l] += e.second;
		}
	}
}

double Cut::getCut(IExtendedPartition const & data, size_t const & l) const {
	double cut(0);
	FOR_EACH_CONST (n , data.list(l)) {
		size_t const & l(data.label(n));
		FOR_EACH_CONST( e , data.graph().adjacentList(n)) {
			if (data.label(e.first) != l)
				cut += e.second;
		}
	}
	return cut;
}