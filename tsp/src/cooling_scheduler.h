#ifndef __SA_COOLING_SCHEDULER__
#define __SA_COOLING_SCHEDULER__

#include <algorithm>

namespace tsp_sa {

class CoolingScheduler {
public:
  virtual ~CoolingScheduler() = default;
  virtual double next_temperature() = 0;
};

class GeometricCooling : public CoolingScheduler {

public:
  GeometricCooling(const double decay_rate, const double init_temperature)
      : decay_rate_(decay_rate), init_temperature_(init_temperature),
        curr_temperature_(init_temperature) {}
  double get_init_temperature() const { return init_temperature_; }

  virtual double next_temperature() override final {
    curr_temperature_ = std::max(1.0, curr_temperature_ * decay_rate_);
    return curr_temperature_;
  }

private:
  double decay_rate_;
  double init_temperature_;
  double curr_temperature_;
};
} // namespace tsp_sa

#endif
