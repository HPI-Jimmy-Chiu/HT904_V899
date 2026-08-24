#!/bin/bash
# AI(W906-FW-BARCODE3) 20260825: the frozen dual-gate template as a SCRIPT,
# because retyping it inline produced the missing-absolute-cd bug four times
# (qw5, qw8, qw11 -- see DEVLOG XVI/XVIII). Usage:
#   nohup bash tools/dualgate.sh <tag> > /dev/null 2>&1 &
# Produces build_<tag>g / build_<tag>r, sentinels _<tag>_gate_g.txt /
# _<tag>_gate_done.txt in the tree root. Both legs cd absolutely.
ROOT=/d/HT9045/HT9011UC_Cpp_V3.33.906.0
TAG="$1"
[ -z "$TAG" ] && exit 2

cd "$ROOT" && mkdir -p "build_${TAG}g" && cd "build_${TAG}g" \
  && cmake -G "MinGW Makefiles" -DCMAKE_CXX_COMPILER=C:/MinGW/bin/g++.exe -DCMAKE_C_COMPILER=C:/MinGW/bin/gcc.exe .. > cfg.log 2>&1 \
  && cmake --build . -j8 > build.log 2>&1 \
  && ctest --timeout 600 > ctest.log 2>&1
echo "G_EXIT=$?" > "$ROOT/_${TAG}_gate_g.txt"

cd "$ROOT" && mkdir -p "build_${TAG}r" && cd "build_${TAG}r" \
  && cmake -G "MinGW Makefiles" -DCMAKE_BUILD_TYPE=Release -DCMAKE_CXX_COMPILER=C:/MinGW/bin/g++.exe -DCMAKE_C_COMPILER=C:/MinGW/bin/gcc.exe .. > cfg.log 2>&1 \
  && cmake --build . -j8 > build.log 2>&1 \
  && ctest --timeout 600 > ctest.log 2>&1
echo "R_EXIT=$?" > "$ROOT/_${TAG}_gate_done.txt"
