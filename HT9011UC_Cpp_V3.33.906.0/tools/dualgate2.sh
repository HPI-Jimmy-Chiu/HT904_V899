#!/bin/bash
# AI(W906-FW-GATE2) 20260826: dualgate.sh 的管線化版本。
#
# 為什麼另開一支而不是改 dualgate.sh：那一支的檔頭明說它是**刻意凍結**的模板
# （手打重覆導致 missing-absolute-cd 的 bug 犯過四次）。凍結的東西就不要動它，
# 新做法先並排跑、驗證過再說。
#
# 改了什麼：只有一件事——**Debug ctest 與 Release build 重疊**。
#   原本：Dcfg → Dbuild → Dctest → Rcfg → Rbuild → Rctest   （純序列）
#   現在：Dcfg → Dbuild → { Dctest ∥ Rcfg+Rbuild } → Rctest
#
# 為什麼這樣是安全的（三個條件，缺一不可）：
#   1. **任何時刻只有一個 ctest 在跑**。這是硬條件：ctest 會綁 WB_TcpLink 那一族埠，
#      兩個 ctest 併行會互相假失敗（tools/webprobe/README.md 的硬規則同源）。
#      本腳本的 Dctest 與 Rctest 之間有 wait，不重疊。
#   2. 兩條腿寫**不同的 build 目錄**（build_<tag>g / build_<tag>r），無檔案衝突。
#      （記憶中的「併發 build 弄壞 build dir」是共用目錄造成的，不是這種。）
#   3. CPU 夠：本機 16 邏輯核心，build 用 -j8、ctest 實測多為單執行緒，
#      重疊時峰值約 10/16，不會互相餓死。
#
# 為什麼**不**順便把 -j8 調高：一次只改一件事。若同時改重疊與並行度而 gate 紅了，
# 就無從歸因。而且 -j 提高會拉高峰值記憶體，而本樹有一顆已知地雷
# （uHGemHT9045_EC.cpp 在 -O3 讓 32-bit cc1plus OOM，已釘 -O1）——那要單獨實測。
#
# 驗證方式：對一棵**已知綠**的樹同時跑 dualgate.sh 與本腳本，
# 兩者的 ctest 數字與失敗集合必須逐項相同。沒驗過之前不要拿它當驗收工具。
#
# 用法（與 dualgate.sh 相同）：
#   nohup bash tools/dualgate2.sh <tag> > /dev/null 2>&1 &
ROOT=/d/HT9045/HT9011UC_Cpp_V3.33.906.0
TAG="$1"
[ -z "$TAG" ] && exit 2

CXX=C:/MinGW/bin/g++.exe
CC=C:/MinGW/bin/gcc.exe

# ---- Debug：configure + build（前景，因為 Release 要等它嗎？不用，但先讓它起跑）----
cd "$ROOT" && mkdir -p "build_${TAG}g" && cd "build_${TAG}g" \
  && cmake -G "MinGW Makefiles" -DCMAKE_CXX_COMPILER=$CXX -DCMAKE_C_COMPILER=$CC .. > cfg.log 2>&1 \
  && cmake --build . -j8 > build.log 2>&1
DBUILD=$?
echo "G_BUILD_EXIT=$DBUILD" > "$ROOT/_${TAG}_gate_gbuild.txt"

if [ $DBUILD -ne 0 ]; then
  echo "G_EXIT=$DBUILD" > "$ROOT/_${TAG}_gate_g.txt"
  echo "R_EXIT=skipped(debug build failed)" > "$ROOT/_${TAG}_gate_done.txt"
  exit $DBUILD
fi

# ---- 重疊段：Release 的 configure+build 放背景，Debug ctest 同時在前景跑 ----
(
  cd "$ROOT" && mkdir -p "build_${TAG}r" && cd "build_${TAG}r" \
    && cmake -G "MinGW Makefiles" -DCMAKE_BUILD_TYPE=Release -DCMAKE_CXX_COMPILER=$CXX -DCMAKE_C_COMPILER=$CC .. > cfg.log 2>&1 \
    && cmake --build . -j8 > build.log 2>&1
  echo "R_BUILD_EXIT=$?" > "$ROOT/_${TAG}_gate_rbuild.txt"
) &
RBUILD_PID=$!

cd "$ROOT/build_${TAG}g" && ctest --timeout 600 > ctest.log 2>&1
echo "G_EXIT=$?" > "$ROOT/_${TAG}_gate_g.txt"

# ---- 等 Release build 結束，才開始 Release ctest（兩個 ctest 永不重疊）----
wait $RBUILD_PID
RB=$(cat "$ROOT/_${TAG}_gate_rbuild.txt" 2>/dev/null | sed 's/.*=//')
if [ "$RB" != "0" ]; then
  echo "R_EXIT=build-failed($RB)" > "$ROOT/_${TAG}_gate_done.txt"
  exit 1
fi

cd "$ROOT/build_${TAG}r" && ctest --timeout 600 > ctest.log 2>&1
echo "R_EXIT=$?" > "$ROOT/_${TAG}_gate_done.txt"
