#ifndef __SA_SOLVER_H__
#define __SA_SOLVER_H__

#include "cooling_scheduler.h"
#include "datatype.h"
#include "tsp_move.h"

namespace tsp_sa {

struct SolverConfig {
  int max_inner_iter_;
  int max_outer_iter_;
  SolverConfig(const int max_inner_iter_, const int max_outer_iter_)
      : max_inner_iter_(max_inner_iter_), max_outer_iter_(max_outer_iter_) {}
};

class Solver {
public:
  Solver(const SolverConfig &config) : config_(config){};
  void register_tsp_move(TSPMove *);
  void register_cooling_schedular(CoolingScheduler *);
  city_order_type solve(const CityMap &);
  city_order_type solve(const CityMap &, const city_order_type &);

private:
  city_order_type warm_start(const CityMap &) const;
  bool is_acceptable(const double energy_diff, const double temperature,
                     const double acceptable_cutoff) const;

  std::vector<TSPMove *> tsp_moves_;
  CoolingScheduler *cooling_scheduler_;
  const SolverConfig &config_;
};

} // namespace tsp_sa
#endif
