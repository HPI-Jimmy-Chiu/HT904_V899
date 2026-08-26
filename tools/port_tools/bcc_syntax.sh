#!/bin/sh
# MG 戰役：V910 樹單檔隔離語法編譯 gate（obj 進 scratchpad，不碰共用 Obj）
# 用法: sh tools/port_tools/bcc_syntax.sh 'Automation\automation.cpp' [更多檔...]
set -e
TREE='D:/HT9045/HT9011UC_Code_V3.33.910.0_20260716_Jimmy'
BCC='D:/ProgramFiles/Borland/CBuilder6/Bin/bcc32.exe'
OBJTMP="${TMP:-/tmp}/mg_objtmp"
mkdir -p "$OBJTMP"
INC='Common\PickPlanner;VacuumUnit;MyPLC;EtherCAT;AutoAlignment;Mes;LoadCCD;GroundMan;TempCtrl;CanBus;ProductionInfo;ARMS;D:\HT9045\elec\myvcl;D:\HT9045\elec\Component;Monitor;SECSGEM;Public;PMAlarm;SortingBinTray;Motor;OmronLaser;AutoTeach;BarCode;RotateKit;AutoClean;ASE_K Socket;FingerprintRead;KYECFTP;Automation;CCLink;BinDisplay;Interface;ATC;EJ1N;HThreadCtrl;D:\HT9045\Obj;D:\ProgramFiles\Borland\CBuilder6\include;D:\ProgramFiles\Borland\CBuilder6\include\vcl;.'
cd "$TREE"
fail=0
for f in "$@"; do
  echo "== bcc32 -c $f =="
  if ! "$BCC" -c -H- -w- -D_VER6 -I"$INC" -n"$OBJTMP" "$f"; then
    fail=1
  fi
done
[ $fail -eq 0 ] && echo "== bcc_syntax PASS ==" || { echo "== bcc_syntax FAIL =="; exit 1; }
