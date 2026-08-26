#!/bin/sh
# MG 戰役：V910 樹單檔隔離語法編譯 gate（obj 進 scratchpad，不碰共用 Obj）
# 用法: sh tools/port_tools/bcc_syntax.sh 'Automation\automation.cpp' [更多檔...]
#
# 驗收準則＝錯誤集合與波前基準（<file>.mgbak）一致，不是零錯誤：
# 本 gate 的旗標組（-D_VER6，無 IDE 專案全套 define）下，V910 樹部分檔案有
# 既有錯誤（例 main.cpp Timer7Timer HWND ×2 + edSetupFileNameKeyUp E2096）。
# 零錯誤 → PASS；有錯但與 .mgbak 基準的正規化錯誤集合相同 → PASS(no-regression)；
# 錯誤集合不同或無基準可比 → FAIL。
set -e
TREE='D:/HT9045/HT9011UC_Code_V3.33.910.0_20260716_Jimmy'
BCC='D:/ProgramFiles/Borland/CBuilder6/Bin/bcc32.exe'
OBJTMP="${TMP:-/tmp}/mg_objtmp"
mkdir -p "$OBJTMP"
INC='Common\PickPlanner;VacuumUnit;MyPLC;EtherCAT;AutoAlignment;Mes;LoadCCD;GroundMan;TempCtrl;CanBus;ProductionInfo;ARMS;D:\HT9045\elec\myvcl;D:\HT9045\elec\Component;Monitor;SECSGEM;Public;PMAlarm;SortingBinTray;Motor;OmronLaser;AutoTeach;BarCode;RotateKit;AutoClean;ASE_K Socket;FingerprintRead;KYECFTP;FTPUpload;Automation;CCLink;BinDisplay;Interface;ATC;EJ1N;HThreadCtrl;D:\HT9045\Obj;D:\ProgramFiles\Borland\CBuilder6\include;D:\ProgramFiles\Borland\CBuilder6\include\vcl;.'
cd "$TREE"

norm_errors() {
  # 去掉檔名與行號，留錯誤碼+訊息，排序；stdin=編譯輸出
  grep -a '^Error ' | sed -E 's/^(Error [A-Z][0-9]+) [^ ]+ [0-9]+:/\1:/' | sort
}

compile() {
  # $1=檔案（可為 .mgbak，自動加 -P 強制 C++）
  case "$1" in
    *.mgbak) "$BCC" -P -c -H- -w- -D_VER6 -I"$INC" -n"$OBJTMP" "$1" 2>&1 || true ;;
    *)       "$BCC"    -c -H- -w- -D_VER6 -I"$INC" -n"$OBJTMP" "$1" 2>&1 || true ;;
  esac
}

fail=0
for f in "$@"; do
  echo "== bcc32 -c $f =="
  out=$(compile "$f")
  errs=$(printf '%s\n' "$out" | norm_errors)
  if [ -z "$errs" ]; then
    echo "   PASS (0 errors)"
    continue
  fi
  if [ -f "$f.mgbak" ]; then
    base=$(compile "$f.mgbak" | norm_errors)
    if [ "$errs" = "$base" ]; then
      n=$(printf '%s\n' "$errs" | grep -ac . )
      echo "   PASS (no-regression: $n 個既有錯誤與基準一致)"
      printf '%s\n' "$errs" | sed 's/^/     baseline: /'
      continue
    fi
    echo "   FAIL: 錯誤集合與基準不同"
    echo "--- baseline ---"; printf '%s\n' "$base"
    echo "--- now ---"; printf '%s\n' "$errs"
    fail=1
  else
    echo "   FAIL: 有錯誤且無 .mgbak 基準可比"
    printf '%s\n' "$out" | grep -a '^Error ' | head -10
    fail=1
  fi
done
[ $fail -eq 0 ] && echo "== bcc_syntax PASS ==" || { echo "== bcc_syntax FAIL =="; exit 1; }
