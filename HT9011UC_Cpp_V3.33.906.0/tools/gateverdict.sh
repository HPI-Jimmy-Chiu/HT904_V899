#!/bin/bash
# AI(W906-FW-GATEVERDICT) 20260827: 把「逐項比對失敗集合」從紀律變成工具。
#
# 為什麼需要這支：20260827 凌晨查出 **W34 是在紅燈上 commit 的，而記錄寫成綠燈**。
# 機制不是粗心，是 marker 檔本身沒有鑑別力——
#
#   ctest 對「有任何測試失敗」一律回 exit 8，不論失敗的是 5 個還是 7 個。
#   所以 _<tag>_gate_{g,done}.txt 裡的 `G_EXIT=8` / `R_EXIT=8`
#   在 w23/w26/w28/w29/w30/w31/w32（真的 5 個失敗）
#   與 w34（Debug 7 個、Release 6 個）**逐位元組相同**。
#
# 政策裡「比對失敗集合逐項，不看數字」那條規則存在的理由就是為了擋這件事，
# 而 exit code 恰好長得像一個確認訊號。**把判定寫進工具，不要靠人每次記得做。**
#
# 用法：
#   bash tools/gateverdict.sh <tag>        # 判定 build_<tag>g 與 build_<tag>r
#   bash tools/gateverdict.sh <tag> g      # 只判定 Debug 側
#
# 輸出（每側一段）：
#   <side>_TOTAL=<n 個失敗>
#   <side>_EXTRA=<超出常駐清單的項目>      ← 非空就是 RED
#   <side>_ABSENT=<常駐清單裡這次沒失敗的>  ← 非空不代表壞，但要知道（可能是被跳過）
#   <side>_VERDICT=GREEN|RED|NO-LOG
#
# 離開碼：0=兩側都 GREEN；1=任一側 RED；2=用法錯或缺 log。

ROOT=/d/HT9045/HT9011UC_Cpp_V3.33.906.0

# 常駐失敗清單（docs/PT_CAMPAIGN_PLAN.md §7）。
# dfm2rc_idempotent 歷史上曾是第六項，目前實測為通過，故不列入基準——
# 若它再度失敗，會以 EXTRA 出現，那正是我們要看到的。
RESIDENT="GA1_ReadGeneralIni
IniFiles
config_db
config_loaders
ini_helpers"

TAG="$1"
[ -z "$TAG" ] && { echo "用法: bash tools/gateverdict.sh <tag> [g|r]" >&2; exit 2; }
SIDES="${2:-g r}"

rc=0
for s in $SIDES; do
  log="$ROOT/build_${TAG}${s}/ctest.log"
  up=$(echo "$s" | tr 'gr' 'GR')
  if [ ! -f "$log" ]; then
    echo "${up}_VERDICT=NO-LOG"
    rc=2
    continue
  fi

  # **這個守衛是承重的。** 一個還在寫的 ctest.log 沒有 "The following tests FAILED"
  # 區段，失敗集合會抽成空的 → EXTRA 空 → 判成 GREEN。
  # 那正是這支工具存在要擋的那類假綠燈，所以先要求整輪跑完的摘要行存在。
  if ! grep -q 'tests passed' "$log"; then
    echo "${up}_VERDICT=INCOMPLETE (ctest.log 尚無 'tests passed' 摘要行，該輪還在跑或被中斷)"
    rc=2
    continue
  fi

  # 失敗清單長這樣（前導 tab/空白）：	 14 - config_db (Failed)
  # 剝成純名字。註：ctest 的 log 是 CRLF，正則用 $ 搭 re.M 會靜默失配，
  # 這裡先 tr -d '\r'（與 scratchpad/link_closure.py 同一個坑）。
  names=$(sed -n '/The following tests FAILED/,$p' "$log" \
          | tr -d '\r' \
          | sed -nE 's/^[[:space:]]*[0-9]+ - (.+) \([A-Za-z]+\)$/\1/p' \
          | sort)

  n=$(printf '%s' "$names" | grep -c . )
  extra=$(comm -13 <(printf '%s\n' "$RESIDENT") <(printf '%s\n' "$names") | grep . | tr '\n' ' ')
  absent=$(comm -23 <(printf '%s\n' "$RESIDENT") <(printf '%s\n' "$names") | grep . | tr '\n' ' ')

  echo "${up}_TOTAL=$n"
  echo "${up}_FAILSET=$(printf '%s' "$names" | tr '\n' ' ')"
  echo "${up}_EXTRA=$extra"
  echo "${up}_ABSENT=$absent"
  if [ -n "$extra" ]; then
    echo "${up}_VERDICT=RED"
    rc=1
  else
    echo "${up}_VERDICT=GREEN"
  fi
done
exit $rc
