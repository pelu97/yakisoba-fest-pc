# Yakisoba Festival
## Concurrent Programming with C example _(Yakisoba Festival 🍝)_

* __N Clients__:
    * __wait__ in line to be serviced;
* __1 Worker__:
    * take dishes from the kitchen counter to the clients, up to 2 at a time;
    * if there are no clients waiting and/or no dishes ready on the counter, __waits__;
* __3 Chefs__:
    * cook dishes and place them on the kitchen counter;
    * if kitchen counter is full, __waits__;

* __Obs__:
    * __delays__ are added to simulate the time that takes to complete certain activities;
    * due to the __delays__, it is very easy to see how having only one kitchen counter becomes a huge __bottleneck__, specially when increasing the amount of chefs and workers;
