#include "types.h"
#undef Success
#include <Eigen/Dense>
#include <limits>
#include <map>
#include <math.h>
#include <stdlib.h>
#include <vector>

using Eigen::VectorXf;
using std::map;
using std::vector;

VectorXf SampleArbitraryDistribution(const VectorXf& distribution, const int& num_samples) {
  const int size = distribution.size();
  
  const VectorXf normalized = distribution / distribution.sum();
  
      VectorXf cumulative(size);
      cumulative(0) = normalized(0);
      for (int i = 1; i < size;  i++) {
	cumulative(i) = cumulative(i - 1) + normalized(i);
      }

      vector<float> steps(size);
      for (int i = 0; i < size; i++) {
	steps[i] = ((float) i) / ((float) size - 1);
      }
      steps[size-1] = 1.0f;

      VectorXf cumulative_inverse(size);
      int index = 0;
      for (int i = 0; i < size; i++) {
	if (steps[i] < cumulative(index)) {
	  cumulative_inverse(i) = (float) index;
	} else {
	  while (index < size - 1
		 && steps[i] > cumulative(index) + std::numeric_limits<float>::epsilon()) {
	    index++;
	  }
	  cumulative_inverse(i) = (float) index;
	}
	// Have to stop early in some cases.
	if (index >= size) {
	  for (int j = i+1; j < size; j++) {
	    cumulative_inverse(j) = (float) size - 1;
	  }
	  break;
	}
      }
      
      VectorXf samples(num_samples);
      for (int i = 0; i < samples.size(); i++) {
	const int index = rand() % (distribution.size() - 1);
	samples(i) = cumulative_inverse(index);
      }
      
      return samples;
    }

FloatMatrix SampleArbitraryDistribution(const FloatMatrix& distribution, const int& num_samples) {
      // Compute marginal probability distribution over one of the dimensions (cols).
      const VectorXf marginal = distribution.rowwise().sum() / ((float) distribution.cols());
      
      FloatMatrix samples(num_samples, 2);
      // Sample the marginal PDF.
      const VectorXf row_samples = SampleArbitraryDistribution(marginal, num_samples);
      for (int i = 0; i < num_samples; i++) {
	const int row = row_samples(i);
	// Sample the column PDF.
	const VectorXf row_data = distribution.row(row);
	const VectorXf column_sample = SampleArbitraryDistribution(row_data, 1);
	samples(i, 0) = column_sample(0);
	samples(i, 1) = row;
      }
      
      return samples;
    }

int main(int argc, char** argv) {
  FloatMatrix map(10, 10);

  for (int i = 0; i < 10; ++i) {
    for (int j = 0; j < 10; ++j) {
      map(i, j) = atoi(argv[1 + i + j * 10]);
    }
  }

  FloatMatrix sample = SampleArbitraryDistribution(map, 1);
  printf("%d %d\n", sample(0), sample(1));

  return 0;
}
