// kate: replace-tabs off; indent-width 4; indent-mode normal
// vim: ts=4:sw=4:noexpandtab
/*

Copyright (c) 2010--2012,
François Pomerleau and Stephane Magnenat, ASL, ETHZ, Switzerland
You can contact the authors at <f dot pomerleau at gmail dot com> and
<stephane at magnenat dot net>

All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:
    * Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above copyright
      notice, this list of conditions and the following disclaimer in the
      documentation and/or other materials provided with the distribution.
    * Neither the name of the <organization> nor the
      names of its contributors may be used to endorse or promote products
      derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL ETH-ASL BE LIABLE FOR ANY
DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

*/

#ifndef __POINTMATCHER_ERRORMINIMIZERS_H
#define __POINTMATCHER_ERRORMINIMIZERS_H

#include "PointMatcher.h"
#include "ErrorMinimizers/PointToPlane.h"
#include "ErrorMinimizers/PointToPlaneWithCov.h"
#include "ErrorMinimizers/PointToPoint.h"
#include "ErrorMinimizers/PointToPointSimilarity.h"

template<typename T>
struct ErrorMinimizersImpl
{
	typedef PointMatcherSupport::Parametrizable Parametrizable;
	typedef PointMatcherSupport::Parametrizable P;
	typedef Parametrizable::Parameters Parameters;
	typedef Parametrizable::ParameterDoc ParameterDoc;
	typedef Parametrizable::ParametersDoc ParametersDoc;

	typedef typename PointMatcher<T>::DataPoints DataPoints;
	typedef typename PointMatcher<T>::Matches Matches;
	typedef typename PointMatcher<T>::OutlierWeights OutlierWeights;
	typedef typename PointMatcher<T>::ErrorMinimizer ErrorMinimizer;
	typedef typename PointMatcher<T>::ErrorMinimizer::ErrorElements ErrorElements;
	typedef typename PointMatcher<T>::TransformationParameters TransformationParameters;
	typedef typename PointMatcher<T>::Vector Vector;
	typedef typename PointMatcher<T>::Matrix Matrix;

	struct IdentityErrorMinimizer: ErrorMinimizer
	{
		inline static const std::string description()
		{
			return "Does nothing.";
		}

		//virtual TransformationParameters compute(const DataPoints& filteredReading, const DataPoints& filteredReference, const OutlierWeights& outlierWeights, const Matches& matches);
		virtual TransformationParameters compute(const ErrorElements& mPts);
	};

	struct PointToPointWithCovErrorMinimizer: ErrorMinimizer
	{
		inline static const std::string description()
		{
			return "Point-to-point error. Based on SVD decomposition. Based on \\cite{Besl1992Point2Point}. Covariance estimation based on \\cite{Censi2007ICPCovariance}.";
		}

		inline static const ParametersDoc availableParameters()
		{
			return {
				{"sensorStdDev", "sensor standard deviation", "0.01", "0.", "inf", &P::Comp<T>}
			};
		}

		const T sensorStdDev;
		Matrix covMatrix;

		PointToPointWithCovErrorMinimizer(const Parameters& params = Parameters());
		//virtual TransformationParameters compute(const DataPoints& filteredReading, const DataPoints& filteredReference, const OutlierWeights& outlierWeights, const Matches& matches);
		virtual TransformationParameters compute(const ErrorElements& mPts);
		virtual T getResidualError(const DataPoints& filteredReading, const DataPoints& filteredReference, const OutlierWeights& outlierWeights, const Matches& matches) const;
		virtual T getOverlap() const;
		virtual Matrix getCovariance() const;
		Matrix estimateCovariance(const ErrorElements& mPts, const TransformationParameters& transformation);
	};
}; // ErrorMinimizersImpl

#endif // __POINTMATCHER_ERRORMINIMIZER_H
