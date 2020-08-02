#ifndef TSP_K_OPT
#define TSP_K_OPT

#include <random>

#include "solver.h"

namespace tsp {
typedef std::vector<size_t> order_type;

class KOpt : public Solver {

public:
  KOpt(const std::vector<Point> &cities_) : Solver(cities_) {}
  virtual Solution solve() override;

private:
  order_type random_init(std::default_random_engine &) const;
  order_type warm_start() const;
  order_type search(const order_type &order, const size_t start_idx) const;
  void reverse_order(const size_t idx0, const size_t idx1,
                     order_type &order) const;
};

} // namespace tsp

#endif
