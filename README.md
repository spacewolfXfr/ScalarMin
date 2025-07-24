# ScalarMin

By Maël FEURGARD (<mael.feurgard@enac.fr>)

----

A global minimization solver library for univariate scalar functions on an interval.
Two main methods are implemented: Interval algebra based and interpolation based (so-called Brent).

## Interval algebra based solver (`IntervalSolver`)

Some reading about interval algebra based solvers: [https://arnold-neumaier.at/glopt/techniques.html#interval](https://arnold-neumaier.at/glopt/techniques.html#interval)

And a paper describing the basics, and how to improve them: [A Parallel Software Package for
Nonlinear Global Optimization](https://arnold-neumaier.at/glopt/mss/HuKX00.pdf)

The reference for what we are doing [Global Optimization Using Interval
Analysis: The One-Dimensional Case](https://doi.org/10.1007/BF00933139)

## Polynomial interpolated based method (`BrentSolver`)

## The underlying problem we are interested in

These methods are useful for one specific problem: looking for collisions (or conflicts)
between two trajectories. They must be reliable, since collisions are unacceptable; and fast, as
they may be called often during path planning.

This problem is formalized as follows: given two trajectory functions $f_1,f_2 \colon \mathbb{R}\to\mathbb{R}^n$
describing with respect to time the movement of two vehicles is some $n$-dimensional space (usually 2 or 3), find the closest they get to one another during
some time period $I$ a finite interval of $\mathbb{R}$. This means solving the optimization problem:
$$
\min_{t\in I} \left\lVert f_1(t) - f_2(t) \right\rVert
$$
It can be equivalently solved by using the squared norm (assuming it is the Euclidean norm). Meaning we want to minimize
the function
$$
d(t) := \left\lVert f_1(t) - f_2(t) \right\rVert^2
$$

Because most of the path planning techniques use well-known functions (lines and circles for geometric planner
such as [Dubins](https://en.wikipedia.org/wiki/Dubins_path); [splines](https://en.wikipedia.org/wiki/Spline_(mathematics)#External_links) for continuous optimization planner), it is reasonable to rely on the derivatives of the objective function $d$. They can be easily computed from the trajectories definitions:
$$
\begin{matrix}
    d'(t) = 2 \left[ f_1'(t) - f_2'(t) \right]^T \left[ f_1(t) - f_2(t) \right]\\
    d''(t) = 2 \left[ f_1''(t) - f_2''(t) \right]^T \left[ f_1(t) - f_2(t) \right] + 2 \left\lVert f_1'(t) - f_2'(t) \right\rVert^2
\end{matrix}
$$
Using Cauchy inequality and the triangle inequality, we get these bounds on $d''$
$$
-2 \left(\left\lVert f_1''(t)\right\rVert + \left\lVert f_2''(t) \right\rVert\right) \left\lVert f_1(t) - f_2(t) \right\rVert
\leq d''(t) \leq
2 \left(\left\lVert f_1''(t) \right\rVert + \left\lVert f_2''(t) \right\rVert\right) \left\lVert f_1(t) - f_2(t) \right\rVert
+ 2 \left( \left\lVert f_1'(t) \right\rVert + \left\lVert f_2'(t) \right\rVert\right)^2
$$
They mostly depend on the individual maximal values of $\left\lVert f_i'(t) \right\rVert, \left\lVert f_i''(t) \right\rVert$
which should be known as part of the vehicles limitations. Regarding the *maximum* distance between the two vehicles, it may be bounded by using a bounding box on the planning.
