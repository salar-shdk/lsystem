#include "Lsystem.h"

Lsystem sierpinski_triangle("F-G-G", {rule{"F", "F-G+F+G-F"}, rule{"G", "GG"}},
                            120);
Lsystem sierpinski_arrowhead("A", {rule{"A", "B-A-B"}, rule{"B", "A+B+A"}}, 60);

Lsystem dragon_curve("FX", {rule{"X", "X+YF+"}, rule{"Y", "-FX-Y"}}, 90);

Lsystem barnsley_fern("X", {rule{"X", "F+[[X]-X]-F[-FX]+X"}, rule{"F", "FF"}},
                      25);

Lsystem sticks("X", {rule{"X", "F[+X]F[-X]+X"}, rule{"F", "FF"}}, 20);

Lsystem hilbert("L", {rule{"L", "+RF-LFL-FR+"}, rule{"R", "-LF+RFR+FL-"}}, 90);

Lsystem pentaplexity("F++F++F++F++F", {rule{"F", "F++F++F+++++F-F++F"}}, 36);
