#include "store.hh"
#include <iostream>
#include <utility>		// pair
#include <vector>
#include <cstdlib>		// abs



int
main()
{
    // A store is defined by its capacity and decay profile; here capacity is unlimited
    store s{1.0, 0.98, 0.95, 0.9, 0.5, 0.1};

    // test data - deposit and expected return value at each time step;
    // return is the same as deposited 
    std::vector<unit> deposit{10.0, 0, 2.0, 5.0};
    deposit.resize(10, 0.0);
    std::vector<std::pair<unit,unit>> withdraw{{0.5, 0.5}, {0, 0}, {8.0, 8.0}, {0, 0}, {5, 5}, {4, 2.034924}, {3, 0}};
    withdraw.resize(10, std::make_pair<unit,unit>(0, 0));

    for( size_t ts = 0; ts < 10; ++ts) {
	// step in parallel
	if( s+= deposit[ts] != 0.0 )
	    std::cerr << "Expected 0 from deposit step " << ts << "\n";
	if( std::abs((s -= withdraw[ts].first) - withdraw[ts].second) > 1.0E-5 )
	    std::cerr << "Withdraw mismatch at step " << ts << "\n";
    }
    return 0;
}
