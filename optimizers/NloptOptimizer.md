# NloptOptimizer Configuration Reference

This document describes the configuration parameters for the `NloptOptimizer` class, which uses the [NLopt library](https://nlopt.readthedocs.io/) for nonlinear optimization.

## Configuration File Format

Configuration is loaded from an `.ini` file with the following parameters:

---

## Required Parameters

### `direction`
**Type:** `string`  
**Possible Values:** `MIN`, `MAX`  
**Default:** None (required)

Specifies the optimization direction:
- `MIN` - Minimize the objective function
- `MAX` - Maximize the objective function

**Example:**
```ini
direction = MAX
```

---

### `algo`
**Type:** `string`
**Possible Values:** See [NLopt Algorithms](#nlopt-algorithms) below
**Default:** None (required)

The NLopt optimization algorithm to use. This is passed directly to the NLopt library.

> **IMPORTANT:** Algorithm names should **NOT** include the `NLOPT_` prefix. Use `LD_MMA` instead of `NLOPT_LD_MMA`.

**Example:**
```ini
algo = LD_MMA
```

---

### `relativeToleranceX`
**Type:** `double`  
**Possible Values:** Any positive number (typically 1e-2 to 1e-12)  
**Recommended:** `1e-4` (0.0001)

**Relative tolerance on optimization parameters (x-values).**  
The optimizer stops when the relative change in the optimization parameters between iterations falls below this threshold. The stop reason will be `XTOL_REACHED`.

**Guidelines:**
| Precision Level | Value | Description |
|-----------------|-------|-------------|
| Loose | 0.01 - 0.1 | Quick results, less precision |
| Moderate | 1e-4 | Good balance of speed and accuracy |
| High | 1e-6 to 1e-8 | Precise results, slower convergence |

**Example:**
```ini
relativeToleranceX = 0.01
```

---

### `relativeToleranceF`
**Type:** `double`  
**Possible Values:** Any positive number (typically 1e-2 to 1e-12)  
**Recommended:** `1e-5` (0.00001)

**Relative tolerance on the objective function value.**  
The optimizer stops when the relative change in the objective function value between iterations falls below this threshold. The stop reason will be `FTOL_REACHED`.

**Guidelines:**
| Precision Level | Value | Description |
|-----------------|-------|-------------|
| Loose | 1e-2 to 1e-3 | Quick results, less precision |
| Moderate | 1e-4 to 1e-6 | Good balance of speed and accuracy |
| High | 1e-8 to 1e-12 | Precise results, slower convergence |

**Example:**
```ini
relativeToleranceF = 1e-5
```

---

### `maxEval`
**Type:** `int`  
**Possible Values:** Any integer (negative = infinite)  
**Recommended:** `-1` (infinite) or `100000`

**Maximum number of objective function evaluations.**  
The optimizer stops after this many evaluations. Set to any negative value to use `INT_MAX` (effectively infinite). The stop reason will be `MAXEVAL_REACHED`.

**Guidelines:**
| Use Case | Value | Description |
|----------|-------|-------------|
| Quick testing | 100 - 1000 | Fast iterations for debugging |
| Standard optimization | 10000 - 100000 | Typical for trading strategies |
| Complex problems | 1000000+ | For large parameter spaces |
| Unlimited | -1 | Relies on tolerance criteria only |

**Example:**
```ini
maxEval = -1
```

---

### `maxTime`
**Type:** `double`  
**Possible Values:** Any number in seconds (negative = infinite)  
**Recommended:** `-1` (infinite)

**Maximum optimization time in seconds.**  
The optimizer stops after this many seconds. Set to any negative value to use `INFINITY` (effectively unlimited). The stop reason will be `MAXTIME_REACHED`.

**Guidelines:**
| Use Case | Value | Description |
|----------|-------|-------------|
| Quick testing | 10 - 60 | Fast iterations |
| Standard optimization | 300 - 3600 | 5 minutes to 1 hour |
| Unlimited | -1 | Relies on tolerance criteria only |

**Example:**
```ini
maxTime = -1
```

---

## NLopt Algorithms

The `algo` parameter accepts any valid NLopt algorithm string. Common algorithms include:

> **NOTE:** Algorithm names should **NOT** include the `NLOPT_` prefix. For example, use `LD_MMA` instead of `NLOPT_LD_MMA`.

### Gradient-Based (Derivative Required)
| Algorithm | Description | Best For |
|-----------|-------------|----------|
| `LD_MMA` | Method of Moving Asymptotes | General constrained optimization |
| `LD_SLSQP` | Sequential Quadratic Programming | Small to medium problems |
| `LD_LBFGS` | Limited-memory BFGS | Unconstrained optimization |
| `LD_TNEWTON` | Truncated Newton | Large-scale problems |

### Derivative-Free
| Algorithm | Description | Best For |
|-----------|-------------|----------|
| `LN_NELDERMEAD` | Nelder-Mead simplex | Small problems, no gradient |
| `LN_SBPLX` | Subplex | Improved Nelder-Mead |
| `LN_BOBYQA` | Bound Optimization BY Quadratic Approximation | Bound-constrained problems |
| `LN_COBYLA` | Constrained Optimization BY Linear Approximations | Constrained problems |

### Global Optimization
| Algorithm | Description | Best For |
|-----------|-------------|----------|
| `GN_DIRECT` | Dividing Rectangles | Global optimization |
| `GN_DIRECT_L` | DIRECT-L (more local) | Global with local refinement |
| `GN_CRS2_LM` | Controlled Random Search | Global, derivative-free |
| `GD_STOGO` | Stochastic Global Optimization | Global, differentiable |
| `G_MLSL` | Multi-Level Single-Linkage | Global, multi-start |

### References
- [NLopt Algorithms Reference](https://nlopt.readthedocs.io/en/latest/NLopt_Algorithms/) - Official documentation
- [NLopt Source Code](https://github.com/stevengj/nlopt) - For complete algorithm list, see `src/api/general.c` function `nlopt_algorithm_to_string()`

---

## Stop Reasons

The optimization can stop for the following reasons (returned in `OptimizedResult`):

| Stop Reason | Description |
|-------------|-------------|
| `SUCCESS` | Generic success |
| `STOPVAL_REACHED` | Stop value reached (not currently used) |
| `FTOL_REACHED` | Function value tolerance reached |
| `XTOL_REACHED` | Parameter tolerance reached |
| `MAXEVAL_REACHED` | Maximum evaluations reached |
| `MAXTIME_REACHED` | Maximum time reached |

---

## Example Configuration Files

### Quick Exploration
```ini
direction = MAX
algo = LN_NELDERMEAD
relativeToleranceX = 0.1
relativeToleranceF = 1e-3
maxEval = 1000
maxTime = 60
```

### Standard Trading Strategy Optimization
```ini
direction = MAX
algo = LD_MMA
relativeToleranceX = 0.01
relativeToleranceF = 1e-5
maxEval = -1
maxTime = -1
```

### High-Precision Final Tuning
```ini
direction = MAX
algo = LD_LBFGS
relativeToleranceX = 1e-6
relativeToleranceF = 1e-8
maxEval = 1000000
maxTime = 3600
```

---

## Code References

- Implementation: [`NloptOptimizer.cpp`](NloptOptimizer.cpp)
- Base class: [`Optimizer.hpp`](../Optimizer.hpp)
- Direction enum: [`OptimizerDirection.hpp`](../OptimizerDirection.hpp)
