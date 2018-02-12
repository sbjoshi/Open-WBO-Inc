#include "SequenceStatistic.h"

using namespace openwbo;

SequenceStatistic::SequenceStatistic(Statistics stat = Statistics::_MEAN_) {
  statistic = stat;
}

void SequenceStatistic::setStatistic(Statistics stat) {
  statistic = stat;
}
  
uint64_t SequenceStatistic::getSequenceStatistic(vec<uint64_t> sequence, 
    uint64_t start_index, uint64_t end_index) {
  if(start_index > end_index) {
    return 0; // check if this is ok, useful for CC
  }
  switch(statistic) {
    case Statistics::_MEAN_:
      return meanStatistic(sequence,start_index,end_index);
      break;
    case Statistics::_MEDIAN_:
      return medianStatistic(sequence,start_index,end_index);
      break;
    case Statistics::_MAX_:
      return maxStatistic(sequence,start_index,end_index);
      break;
    case Statistics::_MIN_:
      return minStatistic(sequence,start_index,end_index);
      break;
    default: // check this
      return meanStatistic(sequence,start_index,end_index);
  }
  return meanStatistic(sequence,start_index,end_index);    
}
  
uint64_t SequenceStatistic::meanStatistic(vec<uint64_t> sequence, 
    uint64_t start_index, uint64_t end_index) {
  
  uint64_t sum = 0;
  for(int i = start_index; i <= end_index; i++) {
    sum += sequence[i];
  }
  sum /= (end_index-start_index+1);
  return sum;      
}
  
uint64_t SequenceStatistic::medianStatistic(vec<uint64_t> sequence,
    uint64_t start_index, uint64_t end_index) {
  
  uint64_t index = (start_index+end_index+1)/2;
  return sequence[index];       
}

uint64_t SequenceStatistic::maxStatistic(vec<uint64_t> sequence,
    uint64_t start_index, uint64_t end_index) {
  
  return sequence[end_index];     
}

uint64_t SequenceStatistic::minStatistic(vec<uint64_t> sequence,
    uint64_t start_index, uint64_t end_index) {
  
  return sequence[start_index];     
}
