#ifndef __SA_TSP_MOVE_H__
#define __SA_TSP_MOVE_H__

#include "datatype.h"
#include <random>

namespace tsp_sa {
class TSPMove {
public:
  virtual ~TSPMove() = default;
  virtual double propose(const city_order_type &, const CityMap &) = 0;
  virtual void apply_move(city_order_type &) = 0;
  virtual void set_city_number(const size_t) = 0;
};

class TwoMove : public TSPMove {
public:
  virtual void set_city_number(const size_t n_city) override final {
    idx_dist_ = std::uniform_int_distribution<size_t>(0, n_city - 1);
    std::random_device r;
    rd_gen_ = std::mt19937_64(r());
  }

protected:
  void rand_city() {
    i_ = idx_dist_(rd_gen_);
    j_ = idx_dist_(rd_gen_);
    while (j_ == i_) {
      j_ = idx_dist_(rd_gen_);
    }

    if (j_ < i_) {
      std::swap(i_, j_);
    }
  }

  size_t i_;
  size_t j_;

  std::uniform_int_distribution<size_t> idx_dist_;
  std::mt19937_64 rd_gen_;
};

class TwoOPTMove : public TwoMove {
public:
  virtual double propose(const city_order_type &,
                         const CityMap &) override final;
  virtual void apply_move(city_order_type &) override final;
};

class SwapTwoCity : public TwoMove {
public:
  virtual double propose(const city_order_type &,
                         const CityMap &) override final;
  virtual void apply_move(city_order_type &) override final;
};

} // namespace tsp_sa
#endif
