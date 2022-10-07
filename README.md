# YorkCircular

Modelling circular supply chain in York (or elsewhere)

These are very basic models illustrating a simple circular economy with perishable goods.

# Background

The work in this repo was done as a part of a project on circular economy led by Professor Peter Ball, [University of
York](https://www.york.ac.uk/), and funded by [STFC Food Network](https://www.stfcfoodnetwork.org/)

As with many such food projects, the aim was exploratory: identify strengths and weaknesses and opportunities
particularly to reduce food waste; bring people together across different disciplines -- food scientists, food SMEs,
supply chain, farmers, modellers, etc.

# What is here?

This repository explores the question of: if my product is not used immediately, it needs to be stored until it can be
used.  What if the product is perishable?  The classic example is the supermarket where fresher products are put on the
back of the shelf, so customers take the older products first.  This approach minimises waste, or possibly transfers the
waste to the consumer.

The idea is then to introduce a **decay function** $s$ (say) which is a non-negative function of time satisfying the following constraints:

$$s(t)=\begin{cases} 0, & t < 0 \\ 1, & t = 0 \\ \text{...}, & t>0 \end{cases}$$

Here the ... is meant to be a non-increasing (and non-negative) component.

The function $s$ describes the fraction $s(t)$ that remains of a product that is placed into the store at $t=0$; it is
zero for $t<0$ because it will be placed in the store in the future.

## Negative Decay

Considering the part of the decay function for $t>0$, the next thought is then: could it also model growth?  If so, then
the farm also becomes a "storage" facility for the product, but the product is grown and decays by a factor $s(t)>1$ for
$t>0$.  This obviously means the "decay function" is misnamed but the name has stuck.

## Getting stuff out

As in the supermarket, older "generations" of products need to be taken out first in order to minimise loss.  The code
tracks the age of stuff deposited and preferentially tries to match a request for produce from older items.

# Code

There are currently two implementations or work-in-progress implementations.

## Excel Code

An example in Microsoft Excel has been developed; should work nicely in LibreOffice et al.  The purpose of this
implementation is to allow non-programmers to explore how the model works as a standalone unit and combined with others
in a circular economy.

## C++ code

The C++ code is the start of a rewrite, consisting currently only of the storage unit.
