/*!
 * \author Sukrut Rao - cs15btech11036@iith.ac.in
 *
 * @section LICENSE
 *
 * Open-WBO, Copyright (c) 2013-2017, Ruben Martins, Vasco Manquinho, Ines Lynce
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 */

#include "SequenceStatistic.h"

using namespace openwbo;

SequenceStatistic::SequenceStatistic(Statistics stat) { statistic = stat; }

void SequenceStatistic::setStatistic(Statistics stat) { statistic = stat; }

uint64_t SequenceStatistic::getSequenceStatistic(vec<uint64_t> &sequence,
                                                 uint64_t start_index,
                                                 uint64_t end_index) {
  if (start_index > end_index) {
    return 0; // check if this is ok, useful for CC
  }
  switch (statistic) {
  case Statistics::_MEAN_:
    return meanStatistic(sequence, start_index, end_index);
    break;
  case Statistics::_MEDIAN_:
    return medianStatistic(sequence, start_index, end_index);
    break;
  case Statistics::_MAX_:
    return maxStatistic(sequence, start_index, end_index);
    break;
  case Statistics::_MIN_:
    return minStatistic(sequence, start_index, end_index);
    break;
  default: // check this
    return meanStatistic(sequence, start_index, end_index);
  }
  return meanStatistic(sequence, start_index, end_index);
}

uint64_t SequenceStatistic::meanStatistic(vec<uint64_t> &sequence,
                                          uint64_t start_index,
                                          uint64_t end_index) {

  uint64_t sum = 0;
  for (int i = start_index; i <= end_index; i++) {
    sum += sequence[i];
  }
  sum /= (end_index - start_index + 1);
  return sum;
}

uint64_t SequenceStatistic::medianStatistic(vec<uint64_t> &sequence,
                                            uint64_t start_index,
                                            uint64_t end_index) {

  uint64_t index = (start_index + end_index + 1) / 2;
  return sequence[index];
}

uint64_t SequenceStatistic::maxStatistic(vec<uint64_t> &sequence,
                                         uint64_t start_index,
                                         uint64_t end_index) {

  return sequence[end_index];
}

uint64_t SequenceStatistic::minStatistic(vec<uint64_t> &sequence,
                                         uint64_t start_index,
                                         uint64_t end_index) {

  return sequence[start_index];
}
