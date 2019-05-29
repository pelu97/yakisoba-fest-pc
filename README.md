# Yakisoba Festival
## Concurrent Programming with C example _(Yakisoba Festival 🍝)_

* __N Clients__:
    * wait in line to be serviced;
* __1 Worker__:
    * take dishes from the kitchen counter to the clients, up to 2 at a time;
    * if there are no clients waiting and/or no dishes ready on the counter, waits;
* __3 chefs__:
    * cook dishes and place them on the kitchen counter;
    * if kitchen counter is full, waits;
