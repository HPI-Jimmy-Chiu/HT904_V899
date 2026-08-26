#!/bin/bash
# AI(W906-FW-BARCODE3) 20260825: the frozen dual-gate template as a SCRIPT,
# because retyping it inline produced the missing-absolute-cd bug four times
# (qw5, qw8, qw11 -- see DEVLOG XVI/XVIII). Usage:
#   nohup bash tools/dualgate.sh <tag> > /dev/null 2>&1 &
# Produces build_<tag>g / build_<tag>r, sentinels _<tag>_gate_g.txt /
# _<tag>_gate_done.txt in the tree root. Both legs cd absolutely.
#
# AI(W906-FW-GATEVERDICT) 20260827: sentinel 現在除了 exit code 還帶**失敗集合判定**。
#   起因：W34 是在紅燈上 commit 的而被記成綠燈。ctest 對「有任何測試失敗」
#   一律回 exit 8，所以舊 sentinel 的 `G_EXIT=8` 在真的只有常駐五項失敗時、
#   與多了 dfm2rc_fidelity + ShowBinSelectCore 時**逐位元組相同**。
#   exit code 長得像確認訊號，其實零鑑別力。判定改由 tools/gateverdict.sh 給。
#
# AI(W906-FW-GATEVERDICT) 20260827: **這支（序列）是預設，不要用 dualgate2.sh。**
#   dualgate2 讓 Debug ctest 疊在 Release build 上，實測把 dfm2rc_fidelity 從
#   565.94s（單獨跑，通過）推過 600s 逾時 → 假紅。而它換來的加速只有
#   2m25s / 37m43（6%）。用正確性換 6% 是虧的。
ROOT=/d/HT9045/HT9011UC_Cpp_V3.33.906.0
TAG="$1"
[ -z "$TAG" ] && exit 2

cd "$ROOT" && mkdir -p "build_${TAG}g" && cd "build_${TAG}g" \
  && cmake -G "MinGW Makefiles" -DCMAKE_CXX_COMPILER=C:/MinGW/bin/g++.exe -DCMAKE_C_COMPILER=C:/MinGW/bin/gcc.exe .. > cfg.log 2>&1 \
  && cmake --build . -j8 > build.log 2>&1 \
  && ctest --timeout 600 > ctest.log 2>&1
GEX=$?
{ echo "G_EXIT=$GEX"; bash "$ROOT/tools/gateverdict.sh" "$TAG" g; } > "$ROOT/_${TAG}_gate_g.txt" 2>&1

cd "$ROOT" && mkdir -p "build_${TAG}r" && cd "build_${TAG}r" \
  && cmake -G "MinGW Makefiles" -DCMAKE_BUILD_TYPE=Release -DCMAKE_CXX_COMPILER=C:/MinGW/bin/g++.exe -DCMAKE_C_COMPILER=C:/MinGW/bin/gcc.exe .. > cfg.log 2>&1 \
  && cmake --build . -j8 > build.log 2>&1 \
  && ctest --timeout 600 > ctest.log 2>&1
REX=$?
{ echo "R_EXIT=$REX"; bash "$ROOT/tools/gateverdict.sh" "$TAG" r; } > "$ROOT/_${TAG}_gate_done.txt" 2>&1
