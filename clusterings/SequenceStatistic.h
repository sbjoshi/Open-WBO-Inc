#ifndef SequenceStatistic_h
#define SequenceStatistic_h

using NSPACE::vec;

namespace openwbo {

// Assume that sequence is sorted
class SequenceStatistic {
  
public:
  SequenceStatistic(Statistics statistic = Statistics::_MEAN_);
  void setStatistic(Statistics statistic);
  uint64_t getSequenceStatistic(vec<uint64_t> sequence, uint64_t start_index,
                                uint64_t end_index); // check for reference
  uint64_t meanStatistic(vec<uint64_t> sequence, uint64_t start_index,
                                uint64_t end_index); // check for reference
  uint64_t medianStatistic(vec<uint64_t> sequence, uint64_t start_index,
                                uint64_t end_index); // check for reference
  uint64_t maxStatistic(vec<uint64_t> sequence, uint64_t start_index,
                                uint64_t end_index); // check for reference
  uint64_t minStatistic(vec<uint64_t> sequence, uint64_t start_index,
                                uint64_t end_index); // check for reference
                                
protected:
  Statistics statistic;
};

}

#endif
