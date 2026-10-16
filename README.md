# SpiceLite

SpiceLite is a terminal-based linear circuit simulator using Modified Nodal Analysis (MNA) and Eigen.

## Build and run

```powershell
cmake --preset default
cmake --build --preset release
.\build\Release\SpiceLite.exe netlists\smoke.cir output\results.txt
ctest --test-dir build -C Release --output-on-failure
```

Node `0` is ground. Lines beginning with `*`, `#`, or `;` are comments.

## Supported linear netlist elements

| Element | Format |
| --- | --- |
| Resistor | `Rname p n resistance` |
| Capacitor | `Cname p n capacitance` |
| Inductor | `Lname p n inductance` |
| Current source | `Iname p n [DC] value [AC magnitude phaseDegrees]` |
| Voltage source | `Vname p n [DC] value [AC magnitude phaseDegrees]` |
| VCCS | `Gname p n controlP controlN transconductance` |
| VCVS | `Ename p n controlP controlN gain` |
| CCCS | `Fname p n controllingVoltageSource gain` |
| CCVS | `Hname p n controllingVoltageSource transresistance` |

Engineering suffixes are supported: `t`, `g`, `meg`, `k`, `m`, `u`, `n`, `p`, and `f`; `m` means milli.

## Analyses

Without an analysis directive SpiceLite computes a DC operating point. Add one of:

```text
.tran 1m 10m
.ac dec 20 10 100k
```

Transient analysis uses Backward Euler companion models. AC analysis reports real and imaginary node-voltage components at logarithmically spaced frequency points. Sources contribute to AC analysis only when an `AC` magnitude is supplied.
