#include "store.hh"
#include <iostream>
#include <algorithm>
#include <cmath>
#include <functional>
#include <numeric>


store::store() : decay_(), store_(), loss_(0), max_(-1), decays_(0)
{
    std::fill(decay_.begin(), decay_.end(), 1);
    std::fill(ratio_.begin(), ratio_.end(), 1);
    zero_store();
}


store::store(std::initializer_list<unit> const &decay) : loss_(0), max_(-1), decays_(-1)
{
    size_t m = std::min(maxdecay, decay.size());
    std::array<unit, maxdecay>::iterator end = std::copy_n(decay.begin(), m, decay_.begin());
    // If it's too short, pad with zeros
    std::fill(end, decay_.end(), 0);
    reset_decay();
    zero_store();
}


store::store(unit cap) : decay_(), store_(), loss_(0), max_(cap), decays_(0)
{
    std::fill(decay_.begin(), decay_.end(), 1);
    std::fill(ratio_.begin(), ratio_.end(), 1);
    zero_store();
}


store::store(unit cap, std::initializer_list<unit> const &decay) : decay_(), store_(), loss_(0), max_(cap), decays_(-1)
{
    size_t m = std::min(maxdecay, decay.size());
    std::array<unit, maxdecay>::iterator end = std::copy_n(decay.begin(), m, decay_.begin());
    std::fill(end, decay_.end(), 0);
    reset_decay();
    zero_store();
}


/**  Assuming the decay has just been set, sanity check it and populate the ratios
 */
void
store::reset_decay()
{
    auto const incr = std::adjacent_find(decay_.cbegin(), decay_.cend(),
	std::less<unit>());
    if(incr != decay_.cend()) {
	// Allow for increasing "decay" meaning product grows during storage
	auto const bar = std::adjacent_find(decay_.cbegin(), decay_.cend(), std::greater<unit>());
	if(bar != decay_.cend())
	    throw std::invalid_argument("Non-monotonic growth/decay profile");
	decays_ = +1;		// growth
    }
    if(decay_.back() < 0 || decay_.front() < 0)
	throw std::invalid_argument("Decay profile has negative numbers");
    // adjacent difference won't do since it copies the first element as well
    std::transform(decay_.cbegin()+1, decay_.cend(), decay_.cbegin(), ratio_.begin(),
		   [](float b, float a) { return a>0 ? b/a : 0.0; });
}


void
store::zero_store() noexcept
{
    std::fill(store_.begin(), store_.end(), 0);
    loss_ = 0;
}


unit
store::operator+=(unit delta) noexcept
{
    unit add = delta;
    if(max_ >= 0) {
	unit cap = max_ - std::accumulate(store_.cbegin(), store_.cend(), 0);
	if(delta > cap)
	    add = cap;
    }
    store_[0] += add;
    return delta-add;
}


unit
store::operator-=(unit delta) noexcept
{
    unit stuff = std::accumulate(store_.cbegin(), store_.cend(), 0);
    // can only remove as much as we've got
    if(delta > stuff) delta = stuff;
    stuff = delta;
    // extract older products first
    std::array<unit, maxdecay>::reverse_iterator p = store_.rbegin(), q = store_.rend();
    while( delta > 0 && p != q) {
	unit v = std::min(delta, *p);
	delta -= v;
	*p++ -= v;
    }
    return stuff;
}


void
store::step() noexcept
{
    std::array<unit, maxdecay>::reverse_iterator o, p, q, r;
    p = q = store_.rbegin();
    o = ratio_.rbegin();	// ratio_ is one shorter than store_
    r = store_.rend();
    // Everything is shifted by one time step.
    // Whatever comes off the end is lost
    loss_ += *p++;
    // Now we update, shifting values from p to q, using the ratios as multipliers in each step
    unit total = 0;
    while(p != r) {
	float v = *o++;
	unit tmp = *p++;
	unit add = tmp*v;
	*q++ = add;
	total += add;
	if(v<1.0)
	    loss_ += tmp*(1.0-v);
    }
    // The timestep zero storage becomes empty
    *q = 0;
    // This should only apply to growth: we have got too much to store
    if(max_ >= 0 && total >= max_)
	loss_ += (*this -= total-max_);
}



template<typename T, size_t N>
std::ostream &
operator<<(std::ostream &os, std::array<T, N> const &v)
{
    for(size_t n = 0; n < N-1; ++n)
	os << v[n] << ',';
    os << v[N-1];
    return os;
}


std::ostream &
operator<<(std::ostream &os, store const &s)
{
    os << "STOR(";
   if(s.max_ >= 0) os << s.max_;
   os << "){" << s.decay_ \
       << "}[" << s.store_ << "]\n";
    return os;
}
