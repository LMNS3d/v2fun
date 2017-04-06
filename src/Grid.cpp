/*
 * Grid.cpp
 *
 *  Created on: Apr 5, 2017
 *      Author: clarkp
 */

#include "Grid.h"

#define _USE_MATH_DEFINES // Needed for M_PI
#include <cmath>

Grid::Grid(bool isUniform, double delta, double delta_v)
    : remap_param(0.97), a(remap_param*M_PI/2),
      b(std::sin(remap_param*M_PI/2)), isUniform(isUniform) {
  if (isUniform) {
    size = std::ceil(delta/delta_v);
  } else {
    size = std::ceil(a/(std::asin(b*delta_v/delta-b)+a));
  }

  xi = gsl_vector_alloc(size);
  y = gsl_vector_alloc(size);

  for (unsigned int i = 0; i<size; i++) {
    gsl_vector_set(xi, i, double(i+1)/size);
    if (isUniform) {
      gsl_vector_set(y, i, gsl_vector_get(xi, i));
    } else {
      gsl_vector_set(y, i, remap(gsl_vector_get(xi, i)));
    }
  }
}

Grid::~Grid() {
  gsl_vector_free(xi);
  gsl_vector_free(y);
}

double Grid::remap(double xi) const {
  return std::sin((xi-1)*a)/b+1;
}

double Grid::dXidY(double xi) const {
 return b/(a*std::sqrt(1.0 - b*b*std::pow(remap(xi)-1,2)));
}

double Grid::d2XidY2(double xi) const {
 return (b*b*(b*remap(xi)-b))/
     (a*pow(1.0-pow(b-b*remap(xi),2),1.5));
}