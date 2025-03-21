/*
Suppose you work as a truck driver, and you have been hired to carry a load of
potatoes from Boise, Idaho, to Minneapolis, Minnesota, a total distance of 2,500
kilometers. You estimate you can average 100 km/hr driving within the speed
limits, requiring a total of 25 hours for the trip.

(a) You hear on the news that Montana has just abolished its speed limit, which
constitutes 1,500 km of the trip. Your truck can travel at 150 km/hr. What will
be your speedup for the trip?”

(b) You can buy a new turbocharger for your truck at www.fasttrucks.com. They
stock a variety of models, but the faster you want to go, the more it will cost.
How fast must you travel through Montana to get an overall speedup for your trip
of 1.67×?
*/

#include <stdio.h>

int main(void) {
  double boise_to_minneapolis_km = 2500;
  double km_per_hour_limit = 100;

  // (a)
  double montanna_km = 1500;
  double montanna_km_per_hour_limit = 150;

  double alpha = montanna_km / boise_to_minneapolis_km;
  double performance_improvement =
      montanna_km_per_hour_limit / km_per_hour_limit;
  double speedup = 1 / ((1 - alpha) + (alpha / performance_improvement));

  printf("(a): %gx\n", speedup);

  // (b)
  // Speedup = 1 / ((1 - a) + (a / k))
  // S((1 - a) + (a / k)) = 1
  // S(1 - a) + S(a / k) = 1
  // S - Sa + (Sa / k) = 1
  // Sa / k = 1 - S + Sa
  // k = Sa / (1 - S + Sa)
  double required_speedup = 1.67;

  double k = (required_speedup * alpha) /
             (1 - required_speedup + (required_speedup * alpha));

  printf("(b): %gkm/h\n", km_per_hour_limit * k);

  return 0;
}
