	.386p
	ifdef ??version
	if    ??version GT 500H
	.mmx
	endif
	endif
	model flat
	ifndef	??version
	?debug	macro
	endm
	endif
	?debug	S "D:\HT9045\HT9046LS_Code_V3.24P_305_20150724_Siguard\SECSGEM\TasmInfo.cpp"
	?debug	T "D:\HT9045\HT9046LS_Code_V3.24P_305_20150724_Siguard\SECSGEM\TasmInfo.cpp"
_TEXT	segment dword public use32 'CODE'
_TEXT	ends
_DATA	segment dword public use32 'DATA'
_DATA	ends
_BSS	segment dword public use32 'BSS'
_BSS	ends
$$BSYMS	segment byte public use32 'DEBSYM'
$$BSYMS	ends
$$BTYPES	segment byte public use32 'DEBTYP'
$$BTYPES	ends
$$BNAMES	segment byte public use32 'DEBNAM'
$$BNAMES	ends
$$BROWSE	segment byte public use32 'DEBSYM'
$$BROWSE	ends
$$BROWFILE	segment byte public use32 'DEBSYM'
$$BROWFILE	ends
 ?debug  C FB0C0100600000
DGROUP	group	_BSS,_DATA
_BSS	segment dword public use32 'BSS'
	align	4
_deax	label	dword
	db	4	dup(?)
	align	4
_debx	label	dword
	db	4	dup(?)
	align	4
_decx	label	dword
	db	4	dup(?)
	align	4
_dedx	label	dword
	db	4	dup(?)
	align	4
_startFreq	label	dword
	db	4	dup(?)
	align	4
_overFreq	label	dword
	db	4	dup(?)
_BSS	ends
_TEXT	segment dword public use32 'CODE'
@ExeCPUID$qul	segment virtual
@@ExeCPUID$qul	proc	near
?live16385@0:
	?debug L 17
	push      ebp
	mov       ebp,esp
	push      ebx
	?debug L 21
@1:
	mov	 eax,dword ptr [ebp+8]
	?debug L 22
	dw	 0a20fH
	?debug L 23
	mov	 _deax,eax
	?debug L 24
	mov	 _debx,ebx
	?debug L 25
	mov	 _decx,ecx
	?debug L 26
	mov	 _dedx,edx
	?debug L 28
@2:
	pop       ebx
	pop       ebp
	ret 
	?debug L 0
@@ExeCPUID$qul	endp
@ExeCPUID$qul	ends
_TEXT	ends
$$BSYMS	segment byte public use32 'DEBSYM'
	db	2
	db	0
	db	0
	db	0
	dw	60
	dw	517
	dw	0
	dw	0
	dw	0
	dw	0
	dw	0
	dw	0
	dd	?patch1
	dd	?patch2
	dd	?patch3
	df	@@ExeCPUID$qul
	dw	0
	dw	4096
	dw	0
	dw	1
	dw	0
	dw	0
	dw	0
	db	13
	db	64
	db	69
	db	120
	db	101
	db	67
	db	80
	db	85
	db	73
	db	68
	db	36
	db	113
	db	117
	db	108
	dw	18
	dw	512
	dw	8
	dw	0
	dw	34
	dw	0
	dw	2
	dw	0
	dw	0
	dw	0
?patch1	equ	@2-@@ExeCPUID$qul+3
?patch2	equ	0
?patch3	equ	@2-@@ExeCPUID$qul
	dw	2
	dw	6
	dw	8
	dw	531
	dw	1
	dw	65532
	dw	65535
$$BSYMS	ends
_TEXT	segment dword public use32 'CODE'
@GetCPUFreq$qv	segment virtual
@@GetCPUFreq$qv	proc	near
?live16386@0:
	?debug L 30
	push      ebp
	mov       ebp,esp
	?debug L 34
@3:
	dw	 310Fh
	?debug L 35
	mov	 _startFreq,eax                
	?debug L 37
	push      50
	call      @Sleep
	?debug L 40
	dw	 310Fh
	?debug L 41
	mov	 _overFreq,eax                 
	?debug L 45
	mov       eax,dword ptr [_overFreq]
	sub       eax,dword ptr [_startFreq]
	mov       ecx,50000
	cdq
	idiv      ecx
	?debug L 46
@5:
@4:
	pop       ebp
	ret 
	?debug L 0
@@GetCPUFreq$qv	endp
@GetCPUFreq$qv	ends
_TEXT	ends
$$BSYMS	segment byte public use32 'DEBSYM'
	dw	61
	dw	517
	dw	0
	dw	0
	dw	0
	dw	0
	dw	0
	dw	0
	dd	?patch4
	dd	?patch5
	dd	?patch6
	df	@@GetCPUFreq$qv
	dw	0
	dw	4098
	dw	0
	dw	3
	dw	0
	dw	0
	dw	0
	db	14
	db	64
	db	71
	db	101
	db	116
	db	67
	db	80
	db	85
	db	70
	db	114
	db	101
	db	113
	db	36
	db	113
	db	118
?patch4	equ	@5-@@GetCPUFreq$qv+2
?patch5	equ	0
?patch6	equ	@5-@@GetCPUFreq$qv
	dw	2
	dw	6
$$BSYMS	ends
_DATA	segment dword public use32 'DATA'
@_$DCDE$@GetManID$qv	segment virtual
	align	2
@@_$DCDE$@GetManID$qv	label	dword
	dd	@@$xt$17System@AnsiString
	dd	4
	dd	-4
	dd	@@$xt$p17System@AnsiString
	dd	20
	dd	8
	dd	0
	dd	@@$xt$p17System@AnsiString
	dd	52
	dd	8
	dd	0
@_$DCDE$@GetManID$qv	ends
_DATA	ends
_DATA	segment dword public use32 'DATA'
@_$ECTEE$@GetManID$qv	segment virtual
	align	2
@@_$ECTEE$@GetManID$qv	label	dword
	dd	0
	dd	-40
	dw	0
	dw	5
	dd	0
	dd	@@_$DCDE$@GetManID$qv
	dw	8
	dw	5
	dd	1
	dd	@@_$DCDE$@GetManID$qv+28
@_$ECTEE$@GetManID$qv	ends
_DATA	ends
_TEXT	segment dword public use32 'CODE'
@GetManID$qv	segment virtual
@@GetManID$qv	proc	near
?live16387@0:
	?debug L 48
	push      ebp
	mov       ebp,esp
	add       esp,-68
@6:
	mov       eax,offset @@_$ECTEE$@GetManID$qv
	call      @__InitExceptBlockLDTC
	?debug L 51
	push      25
	push      0
	lea       edx,dword ptr [ebp-68]
	push      edx
	call      @_memset
	add       esp,12
	?debug L 53
	push      0
	call      @@ExeCPUID$qul
	pop       ecx
	?debug L 54
	push      4
	push      offset _debx
	lea       ecx,dword ptr [ebp-68]
	push      ecx
	call      @_memcpy
	add       esp,12
	?debug L 55
	push      4
	push      offset _dedx
	lea       eax,dword ptr [ebp-64]
	push      eax
	call      @_memcpy
	add       esp,12
	?debug L 56
	push      4
	push      offset _decx
	lea       edx,dword ptr [ebp-60]
	push      edx
	call      @_memcpy
	add       esp,12
	?debug L 58
	mov       word ptr [ebp-24],8
	lea       edx,dword ptr [ebp-68]
	lea       eax,dword ptr [ebp-4]
	call      @@System@AnsiString@$bctr$qqrpxc
	mov       edx,eax
	inc       dword ptr [ebp-12]
	mov       eax,dword ptr [ebp+8]
	call      @@System@AnsiString@$basg$qqrrx17System@AnsiString
	mov       eax,dword ptr [ebp+8]
	mov       word ptr [ebp-24],20
	push      eax
	dec       dword ptr [ebp-12]
	lea       eax,dword ptr [ebp-4]
	mov       edx,2
	call      @@System@AnsiString@$bdtr$qqrv
	pop       eax
	mov       word ptr [ebp-24],8
	inc       dword ptr [ebp-12]
	mov       edx,dword ptr [ebp-40]
	mov       dword ptr fs:[0],edx
	?debug L 59
@8:
@7:
	mov       esp,ebp
	pop       ebp
	ret 
	?debug L 0
@@GetManID$qv	endp
@GetManID$qv	ends
_TEXT	ends
$$BSYMS	segment byte public use32 'DEBSYM'
	dw	59
	dw	517
	dw	0
	dw	0
	dw	0
	dw	0
	dw	0
	dw	0
	dd	?patch7
	dd	?patch8
	dd	?patch9
	df	@@GetManID$qv
	dw	0
	dw	4100
	dw	0
	dw	4
	dw	0
	dw	0
	dw	0
	db	12
	db	64
	db	71
	db	101
	db	116
	db	77
	db	97
	db	110
	db	73
	db	68
	db	36
	db	113
	db	118
	dw	18
	dw	512
	dw	8
	dw	0
	dw	65534
	dw	65535
	dw	5
	dw	0
	dw	0
	dw	0
	dw	18
	dw	512
	dw	65468
	dw	65535
	dw	4102
	dw	0
	dw	6
	dw	0
	dw	0
	dw	0
?patch7	equ	@8-@@GetManID$qv+4
?patch8	equ	0
?patch9	equ	@8-@@GetManID$qv
	dw	2
	dw	6
	dw	4
	dw	531
	dw	0
$$BSYMS	ends
_DATA	segment dword public use32 'DATA'
@_$DCFE$@GetCPUType$qv	segment virtual
	align	2
@@_$DCFE$@GetCPUType$qv	label	dword
	dd	@@$xt$17System@AnsiString
	dd	4
	dd	-4
	dd	@@$xt$p17System@AnsiString
	dd	20
	dd	8
	dd	0
	dd	@@$xt$p17System@AnsiString
	dd	52
	dd	8
	dd	0
@_$DCFE$@GetCPUType$qv	ends
_DATA	ends
_DATA	segment dword public use32 'DATA'
@_$ECTGE$@GetCPUType$qv	segment virtual
	align	2
@@_$ECTGE$@GetCPUType$qv	label	dword
	dd	0
	dd	-40
	dw	0
	dw	5
	dd	0
	dd	@@_$DCFE$@GetCPUType$qv
	dw	8
	dw	5
	dd	1
	dd	@@_$DCFE$@GetCPUType$qv+28
@_$ECTGE$@GetCPUType$qv	ends
_DATA	ends
_TEXT	segment dword public use32 'CODE'
@GetCPUType$qv	segment virtual
@@GetCPUType$qv	proc	near
?live16392@0:
	?debug L 61
	push      ebp
	mov       ebp,esp
	add       esp,-100
@9:
	mov       eax,offset @@_$ECTGE$@GetCPUType$qv
	call      @__InitExceptBlockLDTC
	?debug L 63
	mov       dword ptr [ebp-44],-2147483646
	?debug L 65
	push      49
	push      0
	lea       edx,dword ptr [ebp-100]
	push      edx
	call      @_memset
	add       esp,12
	?debug L 67
@10:
	xor       ecx,ecx
	mov       dword ptr [ebp-48],ecx
	?debug L 69
@11:
	mov       eax,dword ptr [ebp-48]
	add       eax,-2147483646
	push      eax
	call      @@ExeCPUID$qul
	pop       ecx
	?debug L 71
	push      4
	push      offset _deax
	mov       edx,dword ptr [ebp-48]
	shl       edx,4
	lea       ecx,dword ptr [ebp-100]
	add       edx,ecx
	push      edx
	call      @_memcpy
	add       esp,12
	?debug L 72
	push      4
	push      offset _debx
	mov       eax,dword ptr [ebp-48]
	shl       eax,4
	lea       edx,dword ptr [ebp-96]
	add       eax,edx
	push      eax
	call      @_memcpy
	add       esp,12
	?debug L 73
	push      4
	push      offset _decx
	mov       ecx,dword ptr [ebp-48]
	shl       ecx,4
	lea       eax,dword ptr [ebp-92]
	add       ecx,eax
	push      ecx
	call      @_memcpy
	add       esp,12
	?debug L 74
	push      4
	push      offset _dedx
	mov       edx,dword ptr [ebp-48]
	shl       edx,4
	lea       ecx,dword ptr [ebp-88]
	add       edx,ecx
	push      edx
	call      @_memcpy
	add       esp,12
	?debug L 67
@13:
	inc       dword ptr [ebp-48]
	cmp       dword ptr [ebp-48],3
	jb        @11
	?debug L 77
	mov       word ptr [ebp-24],8
	lea       edx,dword ptr [ebp-100]
	lea       eax,dword ptr [ebp-4]
	call      @@System@AnsiString@$bctr$qqrpxc
	mov       edx,eax
	inc       dword ptr [ebp-12]
	mov       eax,dword ptr [ebp+8]
	call      @@System@AnsiString@$basg$qqrrx17System@AnsiString
	mov       eax,dword ptr [ebp+8]
	mov       word ptr [ebp-24],20
	push      eax
	dec       dword ptr [ebp-12]
	lea       eax,dword ptr [ebp-4]
	mov       edx,2
	call      @@System@AnsiString@$bdtr$qqrv
	pop       eax
	mov       word ptr [ebp-24],8
	inc       dword ptr [ebp-12]
	mov       edx,dword ptr [ebp-40]
	mov       dword ptr fs:[0],edx
	?debug L 78
@16:
@15:
	mov       esp,ebp
	pop       ebp
	ret 
	?debug L 0
@@GetCPUType$qv	endp
@GetCPUType$qv	ends
_TEXT	ends
$$BSYMS	segment byte public use32 'DEBSYM'
	dw	61
	dw	517
	dw	0
	dw	0
	dw	0
	dw	0
	dw	0
	dw	0
	dd	?patch10
	dd	?patch11
	dd	?patch12
	df	@@GetCPUType$qv
	dw	0
	dw	4103
	dw	0
	dw	7
	dw	0
	dw	0
	dw	0
	db	14
	db	64
	db	71
	db	101
	db	116
	db	67
	db	80
	db	85
	db	84
	db	121
	db	112
	db	101
	db	36
	db	113
	db	118
	dw	18
	dw	512
	dw	8
	dw	0
	dw	65534
	dw	65535
	dw	8
	dw	0
	dw	0
	dw	0
	dw	18
	dw	512
	dw	65436
	dw	65535
	dw	4105
	dw	0
	dw	9
	dw	0
	dw	0
	dw	0
	dw	?patch13
	dw	3
	dw	4106
	dw	0
	dw	10
	dw	0
	dw	0
	dw	0
	dw	32772
	dw	2
	dw	32768
?patch13	equ	20
	dw	24
	dw	519
	dw	0
	dw	0
	dw	0
	dw	0
	dd	?patch14
	df	@10
	dw	0
	dw	0
	dw	18
	dw	512
	dw	65488
	dw	65535
	dw	34
	dw	0
	dw	11
	dw	0
	dw	0
	dw	0
?patch14	equ	@13-@10
	dw	2
	dw	6
?patch10	equ	@16-@@GetCPUType$qv+4
?patch11	equ	0
?patch12	equ	@16-@@GetCPUType$qv
	dw	2
	dw	6
	dw	4
	dw	531
	dw	0
$$BSYMS	ends
_TEXT	segment dword public use32 'CODE'
@GetDiskFreeSpaceMB$qcrit2	segment virtual
@@GetDiskFreeSpaceMB$qcrit2	proc	near
?live16395@0:
	?debug L 82
	push      ebp
	mov       ebp,esp
	add       esp,-292
	?debug L 88
@17:
	mov       al,byte ptr [ebp+8]
	mov       byte ptr [ebp-12],al
	?debug L 89
	mov       byte ptr [ebp-11],58
	?debug L 90
	mov       byte ptr [ebp-10],0
	?debug L 92
	lea       edx,dword ptr [ebp-36]
	push      edx
	lea       ecx,dword ptr [ebp-28]
	push      ecx
	lea       eax,dword ptr [ebp-20]
	push      eax
	lea       edx,dword ptr [ebp-12]
	push      edx
	mov       ecx,dword ptr [@Sysutils@GetDiskFreeSpaceEx]
	call      dword ptr [ecx]
	test      eax,eax
	setne     al
	and       eax,1
	mov       byte ptr [ebp-1],al
	?debug L 93
	push      0
	push      1048576
	mov       eax,dword ptr [ebp-28]
	mov       edx,dword ptr [ebp-24]
	call      __lldiv
	mov       edx,dword ptr [ebp+12]
	mov       dword ptr [edx],eax
	?debug L 94
	push      0
	push      1048576
	mov       eax,dword ptr [ebp-36]
	mov       edx,dword ptr [ebp-32]
	call      __lldiv
	mov       edx,dword ptr [ebp+16]
	mov       dword ptr [edx],eax
	?debug L 95
	mov       al,byte ptr [ebp-1]
	?debug L 96
@19:
@18:
	mov       esp,ebp
	pop       ebp
	ret 
	?debug L 0
@@GetDiskFreeSpaceMB$qcrit2	endp
@GetDiskFreeSpaceMB$qcrit2	ends
_TEXT	ends
$$BSYMS	segment byte public use32 'DEBSYM'
	dw	73
	dw	517
	dw	0
	dw	0
	dw	0
	dw	0
	dw	0
	dw	0
	dd	?patch15
	dd	?patch16
	dd	?patch17
	df	@@GetDiskFreeSpaceMB$qcrit2
	dw	0
	dw	4107
	dw	0
	dw	12
	dw	0
	dw	0
	dw	0
	db	26
	db	64
	db	71
	db	101
	db	116
	db	68
	db	105
	db	115
	db	107
	db	70
	db	114
	db	101
	db	101
	db	83
	db	112
	db	97
	db	99
	db	101
	db	77
	db	66
	db	36
	db	113
	db	99
	db	114
	db	105
	db	116
	db	50
	dw	18
	dw	512
	dw	8
	dw	0
	dw	112
	dw	0
	dw	13
	dw	0
	dw	0
	dw	0
	dw	18
	dw	512
	dw	12
	dw	0
	dw	4108
	dw	0
	dw	14
	dw	0
	dw	0
	dw	0
	dw	18
	dw	512
	dw	16
	dw	0
	dw	4108
	dw	0
	dw	15
	dw	0
	dw	0
	dw	0
	dw	18
	dw	512
	dw	65500
	dw	65535
	dw	118
	dw	0
	dw	16
	dw	0
	dw	0
	dw	0
	dw	18
	dw	512
	dw	65508
	dw	65535
	dw	118
	dw	0
	dw	17
	dw	0
	dw	0
	dw	0
	dw	18
	dw	512
	dw	65516
	dw	65535
	dw	118
	dw	0
	dw	18
	dw	0
	dw	0
	dw	0
	dw	18
	dw	512
	dw	65244
	dw	65535
	dw	4110
	dw	0
	dw	19
	dw	0
	dw	0
	dw	0
	dw	18
	dw	512
	dw	65524
	dw	65535
	dw	4111
	dw	0
	dw	20
	dw	0
	dw	0
	dw	0
	dw	18
	dw	512
	dw	65535
	dw	65535
	dw	48
	dw	0
	dw	21
	dw	0
	dw	0
	dw	0
?patch15	equ	@19-@@GetDiskFreeSpaceMB$qcrit2+4
?patch16	equ	0
?patch17	equ	@19-@@GetDiskFreeSpaceMB$qcrit2
	dw	2
	dw	6
	dw	4
	dw	531
	dw	0
$$BSYMS	ends
_TEXT	segment dword public use32 'CODE'
@GetGlobalMemoryStatusKB$qrult1t1	segment virtual
@@GetGlobalMemoryStatusKB$qrult1t1	proc	near
?live16396@0:
	?debug L 98
	push      ebp
	mov       ebp,esp
	add       esp,-64
	?debug L 101
@20:
	mov       dword ptr [ebp-64],64
	?debug L 102
	lea       eax,dword ptr [ebp-64]
	push      eax
	call      @GlobalMemoryStatusEx
	?debug L 103
	mov       edx,dword ptr [ebp-60]
	mov       ecx,dword ptr [ebp+8]
	mov       dword ptr [ecx],edx
	?debug L 104
	push      0
	push      1024
	mov       eax,dword ptr [ebp-56]
	mov       edx,dword ptr [ebp-52]
	call      __lludiv
	mov       edx,dword ptr [ebp+12]
	mov       dword ptr [edx],eax
	?debug L 105
	push      0
	push      1024
	mov       eax,dword ptr [ebp-48]
	mov       edx,dword ptr [ebp-44]
	call      __lludiv
	mov       edx,dword ptr [ebp+16]
	mov       dword ptr [edx],eax
	?debug L 106
@21:
	mov       esp,ebp
	pop       ebp
	ret 
	?debug L 0
@@GetGlobalMemoryStatusKB$qrult1t1	endp
@GetGlobalMemoryStatusKB$qrult1t1	ends
_TEXT	ends
$$BSYMS	segment byte public use32 'DEBSYM'
	dw	80
	dw	517
	dw	0
	dw	0
	dw	0
	dw	0
	dw	0
	dw	0
	dd	?patch18
	dd	?patch19
	dd	?patch20
	df	@@GetGlobalMemoryStatusKB$qrult1t1
	dw	0
	dw	4112
	dw	0
	dw	22
	dw	0
	dw	0
	dw	0
	db	33
	db	64
	db	71
	db	101
	db	116
	db	71
	db	108
	db	111
	db	98
	db	97
	db	108
	db	77
	db	101
	db	109
	db	111
	db	114
	db	121
	db	83
	db	116
	db	97
	db	116
	db	117
	db	115
	db	75
	db	66
	db	36
	db	113
	db	114
	db	117
	db	108
	db	116
	db	49
	db	116
	db	49
	dw	18
	dw	512
	dw	8
	dw	0
	dw	4113
	dw	0
	dw	23
	dw	0
	dw	0
	dw	0
	dw	18
	dw	512
	dw	12
	dw	0
	dw	4113
	dw	0
	dw	24
	dw	0
	dw	0
	dw	0
	dw	18
	dw	512
	dw	16
	dw	0
	dw	4113
	dw	0
	dw	25
	dw	0
	dw	0
	dw	0
	dw	18
	dw	512
	dw	65472
	dw	65535
	dw	65533
	dw	65535
	dw	26
	dw	0
	dw	0
	dw	0
?patch18	equ	@21-@@GetGlobalMemoryStatusKB$qrult1t1+4
?patch19	equ	0
?patch20	equ	@21-@@GetGlobalMemoryStatusKB$qrult1t1
	dw	2
	dw	6
	dw	4
	dw	531
	dw	0
$$BSYMS	ends
_TEXT	segment dword public use32 'CODE'
@$xt$p17System@AnsiString	segment virtual
	align	2
@@$xt$p17System@AnsiString	label	dword
	dd	4
	dw	16
	dw	12
	dd	@@$xt$17System@AnsiString
	db	65
	db	110
	db	115
	db	105
	db	83
	db	116
	db	114
	db	105
	db	110
	db	103
	db	32
	db	42
	db	0
@$xt$p17System@AnsiString	ends
_TEXT	ends
_TEXT	segment dword public use32 'CODE'
@$xt$17System@AnsiString	segment virtual
	align	2
@@$xt$17System@AnsiString	label	byte
	dd	4
	dw	3
	dw	48
	dd	-1
	dd	3
	dw	68
	dw	72
	dd	0
	dw	0
	dw	0
	dd	0
	dd	1
	dd	1
	dd	@@System@AnsiString@$bdtr$qqrv
	dw	3
	dw	76
	db	83
	db	121
	db	115
	db	116
	db	101
	db	109
	db	58
	db	58
	db	65
	db	110
	db	115
	db	105
	db	83
	db	116
	db	114
	db	105
	db	110
	db	103
	db	0
	db	0
	dd	0
	dd	0
	dd	0
@$xt$17System@AnsiString	ends
_TEXT	ends
_TEXT	segment dword public use32 'CODE'
_TEXT	ends
 ?debug  C FB041C405379737574696C73404765744469736B467265655370616365457801000000
	public	_deax
	public	_debx
	public	_decx
	public	_dedx
	public	_startFreq
	public	_overFreq
@Sleep equ Sleep
 extrn   Sleep:near
@_memset equ _memset
 extrn   _memset:near
@_memcpy equ _memcpy
 extrn   _memcpy:near
@@System@AnsiString@$basg$qqrrx17System@AnsiString equ @System@AnsiString@$basg$qqrrx17System@AnsiString
 extrn   @System@AnsiString@$basg$qqrrx17System@AnsiString:near
@@System@AnsiString@$bctr$qqrpxc equ @System@AnsiString@$bctr$qqrpxc
 extrn   @System@AnsiString@$bctr$qqrpxc:near
@@System@AnsiString@$bdtr$qqrv equ @System@AnsiString@$bdtr$qqrv
 extrn   @System@AnsiString@$bdtr$qqrv:near
 extrn   __Exception_list:dword
 extrn   @__InitExceptBlockLDTC:near
 extrn   @Sysutils@GetDiskFreeSpaceEx:dword
 extrn   __lldiv:near
@GlobalMemoryStatusEx equ GlobalMemoryStatusEx
 extrn   GlobalMemoryStatusEx:near
 extrn   __lludiv:near
 ?debug  C FB0A5461736D496E666F00
 ?debug  C 9F76636C6535302E6C6962
$$BSYMS	segment byte public use32 'DEBSYM'
	dw	22
	dw	514
	df	_deax
	dw	0
	dw	34
	dw	0
	dw	27
	dw	0
	dw	0
	dw	0
	dw	22
	dw	514
	df	_debx
	dw	0
	dw	34
	dw	0
	dw	28
	dw	0
	dw	0
	dw	0
	dw	22
	dw	514
	df	_decx
	dw	0
	dw	34
	dw	0
	dw	29
	dw	0
	dw	0
	dw	0
	dw	22
	dw	514
	df	_dedx
	dw	0
	dw	34
	dw	0
	dw	30
	dw	0
	dw	0
	dw	0
	dw	22
	dw	514
	df	_startFreq
	dw	0
	dw	116
	dw	0
	dw	31
	dw	0
	dw	0
	dw	0
	dw	22
	dw	514
	df	_overFreq
	dw	0
	dw	116
	dw	0
	dw	32
	dw	0
	dw	0
	dw	0
	dw	?patch21
	dw	37
	dw	33
	dw	0
	dw	0
	dw	0
	dw	0
?patch21	equ	12
	dw	?patch22
	dw	37
	dw	34
	dw	0
	dw	0
	dw	0
	dw	0
?patch22	equ	12
	dw	?patch23
	dw	3
	dw	4115
	dw	0
	dw	35
	dw	0
	dw	0
	dw	0
	dw	2
?patch23	equ	16
	dw	?patch24
	dw	3
	dw	4115
	dw	0
	dw	36
	dw	0
	dw	0
	dw	0
	dw	3
?patch24	equ	16
	dw	?patch25
	dw	3
	dw	4115
	dw	0
	dw	37
	dw	0
	dw	0
	dw	0
	dw	4
?patch25	equ	16
	dw	?patch26
	dw	3
	dw	4115
	dw	0
	dw	38
	dw	0
	dw	0
	dw	0
	dw	5
?patch26	equ	16
	dw	?patch27
	dw	3
	dw	4115
	dw	0
	dw	39
	dw	0
	dw	0
	dw	0
	dw	6
?patch27	equ	16
	dw	?patch28
	dw	3
	dw	4115
	dw	0
	dw	40
	dw	0
	dw	0
	dw	0
	dw	7
?patch28	equ	16
	dw	?patch29
	dw	3
	dw	4115
	dw	0
	dw	41
	dw	0
	dw	0
	dw	0
	dw	8
?patch29	equ	16
	dw	?patch30
	dw	3
	dw	4115
	dw	0
	dw	42
	dw	0
	dw	0
	dw	0
	dw	9
?patch30	equ	16
	dw	?patch31
	dw	3
	dw	4115
	dw	0
	dw	43
	dw	0
	dw	0
	dw	0
	dw	11
?patch31	equ	16
	dw	?patch32
	dw	3
	dw	4115
	dw	0
	dw	44
	dw	0
	dw	0
	dw	0
	dw	12
?patch32	equ	16
	dw	?patch33
	dw	3
	dw	4115
	dw	0
	dw	45
	dw	0
	dw	0
	dw	0
	dw	13
?patch33	equ	16
	dw	?patch34
	dw	3
	dw	4115
	dw	0
	dw	46
	dw	0
	dw	0
	dw	0
	dw	17
?patch34	equ	16
	dw	?patch35
	dw	3
	dw	4116
	dw	0
	dw	47
	dw	0
	dw	0
	dw	0
	dw	16384
?patch35	equ	16
	dw	?patch36
	dw	3
	dw	4115
	dw	0
	dw	48
	dw	0
	dw	0
	dw	0
	dw	0
?patch36	equ	16
	dw	?patch37
	dw	3
	dw	4115
	dw	0
	dw	49
	dw	0
	dw	0
	dw	0
	dw	1
?patch37	equ	16
	dw	?patch38
	dw	3
	dw	4115
	dw	0
	dw	50
	dw	0
	dw	0
	dw	0
	dw	2
?patch38	equ	16
	dw	?patch39
	dw	3
	dw	4115
	dw	0
	dw	51
	dw	0
	dw	0
	dw	0
	dw	3
?patch39	equ	16
	dw	?patch40
	dw	3
	dw	4115
	dw	0
	dw	52
	dw	0
	dw	0
	dw	0
	dw	4
?patch40	equ	16
	dw	?patch41
	dw	3
	dw	4115
	dw	0
	dw	53
	dw	0
	dw	0
	dw	0
	dw	5
?patch41	equ	16
	dw	?patch42
	dw	3
	dw	4115
	dw	0
	dw	54
	dw	0
	dw	0
	dw	0
	dw	6
?patch42	equ	16
	dw	?patch43
	dw	3
	dw	4115
	dw	0
	dw	55
	dw	0
	dw	0
	dw	0
	dw	7
?patch43	equ	16
	dw	?patch44
	dw	3
	dw	4115
	dw	0
	dw	56
	dw	0
	dw	0
	dw	0
	dw	8
?patch44	equ	16
	dw	?patch45
	dw	3
	dw	4115
	dw	0
	dw	57
	dw	0
	dw	0
	dw	0
	dw	9
?patch45	equ	16
	dw	?patch46
	dw	3
	dw	4115
	dw	0
	dw	58
	dw	0
	dw	0
	dw	0
	dw	10
?patch46	equ	16
	dw	?patch47
	dw	3
	dw	4115
	dw	0
	dw	59
	dw	0
	dw	0
	dw	0
	dw	11
?patch47	equ	16
	dw	?patch48
	dw	3
	dw	4115
	dw	0
	dw	60
	dw	0
	dw	0
	dw	0
	dw	12
?patch48	equ	16
	dw	?patch49
	dw	3
	dw	4115
	dw	0
	dw	61
	dw	0
	dw	0
	dw	0
	dw	13
?patch49	equ	16
	dw	?patch50
	dw	37
	dw	62
	dw	0
	dw	0
	dw	0
	dw	0
?patch50	equ	12
	dw	?patch51
	dw	37
	dw	63
	dw	0
	dw	0
	dw	0
	dw	0
?patch51	equ	12
	dw	?patch52
	dw	37
	dw	64
	dw	0
	dw	0
	dw	0
	dw	0
?patch52	equ	12
	dw	?patch53
	dw	37
	dw	65
	dw	0
	dw	0
	dw	0
	dw	0
?patch53	equ	12
	dw	?patch54
	dw	37
	dw	66
	dw	0
	dw	0
	dw	0
	dw	0
?patch54	equ	12
	dw	?patch55
	dw	37
	dw	67
	dw	0
	dw	0
	dw	0
	dw	0
?patch55	equ	12
	dw	?patch56
	dw	37
	dw	68
	dw	0
	dw	0
	dw	0
	dw	0
?patch56	equ	12
	dw	?patch57
	dw	37
	dw	69
	dw	0
	dw	0
	dw	0
	dw	0
?patch57	equ	12
	dw	?patch58
	dw	37
	dw	70
	dw	0
	dw	0
	dw	0
	dw	0
?patch58	equ	12
	dw	?patch59
	dw	37
	dw	71
	dw	0
	dw	0
	dw	0
	dw	0
?patch59	equ	12
	dw	?patch60
	dw	37
	dw	72
	dw	0
	dw	0
	dw	0
	dw	0
?patch60	equ	12
	dw	?patch61
	dw	37
	dw	73
	dw	0
	dw	0
	dw	0
	dw	0
?patch61	equ	12
	dw	?patch62
	dw	37
	dw	74
	dw	0
	dw	0
	dw	0
	dw	0
?patch62	equ	12
	dw	?patch63
	dw	37
	dw	75
	dw	0
	dw	0
	dw	0
	dw	0
?patch63	equ	12
	dw	?patch64
	dw	37
	dw	76
	dw	0
	dw	0
	dw	0
	dw	0
?patch64	equ	12
	dw	?patch65
	dw	37
	dw	77
	dw	0
	dw	0
	dw	0
	dw	0
?patch65	equ	12
	dw	?patch66
	dw	37
	dw	78
	dw	0
	dw	0
	dw	0
	dw	0
?patch66	equ	12
	dw	?patch67
	dw	37
	dw	79
	dw	0
	dw	0
	dw	0
	dw	0
?patch67	equ	12
	dw	?patch68
	dw	37
	dw	80
	dw	0
	dw	0
	dw	0
	dw	0
?patch68	equ	12
	dw	?patch69
	dw	37
	dw	81
	dw	0
	dw	0
	dw	0
	dw	0
?patch69	equ	12
	dw	?patch70
	dw	37
	dw	82
	dw	0
	dw	0
	dw	0
	dw	0
?patch70	equ	12
	dw	?patch71
	dw	37
	dw	83
	dw	0
	dw	0
	dw	0
	dw	0
?patch71	equ	12
	dw	?patch72
	dw	37
	dw	84
	dw	0
	dw	0
	dw	0
	dw	0
?patch72	equ	12
	dw	?patch73
	dw	37
	dw	85
	dw	0
	dw	0
	dw	0
	dw	0
?patch73	equ	12
	dw	?patch74
	dw	37
	dw	86
	dw	0
	dw	0
	dw	0
	dw	0
?patch74	equ	12
	dw	?patch75
	dw	37
	dw	87
	dw	0
	dw	0
	dw	0
	dw	0
?patch75	equ	12
	dw	?patch76
	dw	38
	dw	267
	dw	88
	dw	0
	dw	89
	dw	0
	dw	90
	dw	0
	dw	91
	dw	0
	dw	92
	dw	0
	dw	93
	dw	0
	dw	94
	dw	0
	dw	95
	dw	0
	dw	96
	dw	0
	dw	97
	dw	0
	dw	98
	dw	0
	dw	99
	dw	0
	dw	100
	dw	0
	dw	101
	dw	0
	dw	102
	dw	0
	dw	103
	dw	0
	dw	104
	dw	0
	dw	105
	dw	0
	dw	106
	dw	0
	dw	107
	dw	0
	dw	108
	dw	0
	dw	109
	dw	0
	dw	110
	dw	0
	dw	111
	dw	0
	dw	112
	dw	0
	dw	113
	dw	0
	dw	114
	dw	0
	dw	115
	dw	0
	dw	116
	dw	0
	dw	117
	dw	0
	dw	118
	dw	0
	dw	119
	dw	0
	dw	120
	dw	0
	dw	121
	dw	0
	dw	122
	dw	0
	dw	123
	dw	0
	dw	124
	dw	0
	dw	125
	dw	0
	dw	126
	dw	0
	dw	127
	dw	0
	dw	128
	dw	0
	dw	129
	dw	0
	dw	130
	dw	0
	dw	131
	dw	0
	dw	132
	dw	0
	dw	133
	dw	0
	dw	134
	dw	0
	dw	135
	dw	0
	dw	136
	dw	0
	dw	137
	dw	0
	dw	138
	dw	0
	dw	139
	dw	0
	dw	140
	dw	0
	dw	141
	dw	0
	dw	142
	dw	0
	dw	143
	dw	0
	dw	144
	dw	0
	dw	145
	dw	0
	dw	146
	dw	0
	dw	147
	dw	0
	dw	148
	dw	0
	dw	149
	dw	0
	dw	150
	dw	0
	dw	151
	dw	0
	dw	152
	dw	0
	dw	153
	dw	0
	dw	154
	dw	0
	dw	155
	dw	0
	dw	156
	dw	0
	dw	157
	dw	0
	dw	158
	dw	0
	dw	159
	dw	0
	dw	160
	dw	0
	dw	161
	dw	0
	dw	162
	dw	0
	dw	163
	dw	0
	dw	164
	dw	0
	dw	165
	dw	0
	dw	166
	dw	0
	dw	167
	dw	0
	dw	168
	dw	0
	dw	169
	dw	0
	dw	170
	dw	0
	dw	171
	dw	0
	dw	172
	dw	0
	dw	173
	dw	0
	dw	174
	dw	0
	dw	175
	dw	0
	dw	176
	dw	0
	dw	177
	dw	0
	dw	178
	dw	0
	dw	179
	dw	0
	dw	180
	dw	0
	dw	181
	dw	0
	dw	182
	dw	0
	dw	183
	dw	0
	dw	184
	dw	0
	dw	185
	dw	0
	dw	186
	dw	0
	dw	187
	dw	0
	dw	188
	dw	0
	dw	189
	dw	0
	dw	190
	dw	0
	dw	191
	dw	0
	dw	192
	dw	0
	dw	193
	dw	0
	dw	194
	dw	0
	dw	195
	dw	0
	dw	196
	dw	0
	dw	197
	dw	0
	dw	198
	dw	0
	dw	199
	dw	0
	dw	200
	dw	0
	dw	201
	dw	0
	dw	202
	dw	0
	dw	203
	dw	0
	dw	204
	dw	0
	dw	205
	dw	0
	dw	206
	dw	0
	dw	207
	dw	0
	dw	208
	dw	0
	dw	209
	dw	0
	dw	210
	dw	0
	dw	211
	dw	0
	dw	212
	dw	0
	dw	213
	dw	0
	dw	214
	dw	0
	dw	215
	dw	0
	dw	216
	dw	0
	dw	217
	dw	0
	dw	218
	dw	0
	dw	219
	dw	0
	dw	220
	dw	0
	dw	221
	dw	0
	dw	222
	dw	0
	dw	223
	dw	0
	dw	224
	dw	0
	dw	225
	dw	0
	dw	226
	dw	0
	dw	227
	dw	0
	dw	228
	dw	0
	dw	229
	dw	0
	dw	230
	dw	0
	dw	231
	dw	0
	dw	232
	dw	0
	dw	233
	dw	0
	dw	234
	dw	0
	dw	235
	dw	0
	dw	236
	dw	0
	dw	237
	dw	0
	dw	238
	dw	0
	dw	239
	dw	0
	dw	240
	dw	0
	dw	241
	dw	0
	dw	242
	dw	0
	dw	243
	dw	0
	dw	244
	dw	0
	dw	245
	dw	0
	dw	246
	dw	0
	dw	247
	dw	0
	dw	248
	dw	0
	dw	249
	dw	0
	dw	250
	dw	0
	dw	251
	dw	0
	dw	252
	dw	0
	dw	253
	dw	0
	dw	254
	dw	0
	dw	255
	dw	0
	dw	256
	dw	0
	dw	257
	dw	0
	dw	258
	dw	0
	dw	259
	dw	0
	dw	260
	dw	0
	dw	261
	dw	0
	dw	262
	dw	0
	dw	263
	dw	0
	dw	264
	dw	0
	dw	265
	dw	0
	dw	266
	dw	0
	dw	267
	dw	0
	dw	268
	dw	0
	dw	269
	dw	0
	dw	270
	dw	0
	dw	271
	dw	0
	dw	272
	dw	0
	dw	273
	dw	0
	dw	274
	dw	0
	dw	275
	dw	0
	dw	276
	dw	0
	dw	277
	dw	0
	dw	278
	dw	0
	dw	279
	dw	0
	dw	280
	dw	0
	dw	281
	dw	0
	dw	282
	dw	0
	dw	283
	dw	0
	dw	284
	dw	0
	dw	285
	dw	0
	dw	286
	dw	0
	dw	287
	dw	0
	dw	288
	dw	0
	dw	289
	dw	0
	dw	290
	dw	0
	dw	291
	dw	0
	dw	292
	dw	0
	dw	293
	dw	0
	dw	294
	dw	0
	dw	295
	dw	0
	dw	296
	dw	0
	dw	297
	dw	0
	dw	298
	dw	0
	dw	299
	dw	0
	dw	300
	dw	0
	dw	301
	dw	0
	dw	302
	dw	0
	dw	303
	dw	0
	dw	304
	dw	0
	dw	305
	dw	0
	dw	306
	dw	0
	dw	307
	dw	0
	dw	308
	dw	0
	dw	309
	dw	0
	dw	310
	dw	0
	dw	311
	dw	0
	dw	312
	dw	0
	dw	313
	dw	0
	dw	314
	dw	0
	dw	315
	dw	0
	dw	316
	dw	0
	dw	317
	dw	0
	dw	318
	dw	0
	dw	319
	dw	0
	dw	320
	dw	0
	dw	321
	dw	0
	dw	322
	dw	0
	dw	323
	dw	0
	dw	324
	dw	0
	dw	325
	dw	0
	dw	326
	dw	0
	dw	327
	dw	0
	dw	328
	dw	0
	dw	329
	dw	0
	dw	330
	dw	0
	dw	331
	dw	0
	dw	332
	dw	0
	dw	333
	dw	0
	dw	334
	dw	0
	dw	335
	dw	0
	dw	336
	dw	0
	dw	337
	dw	0
	dw	338
	dw	0
	dw	339
	dw	0
	dw	340
	dw	0
	dw	341
	dw	0
	dw	342
	dw	0
	dw	343
	dw	0
	dw	344
	dw	0
	dw	345
	dw	0
	dw	346
	dw	0
	dw	347
	dw	0
	dw	348
	dw	0
	dw	349
	dw	0
	dw	350
	dw	0
	dw	351
	dw	0
	dw	352
	dw	0
	dw	353
	dw	0
	dw	354
	dw	0
?patch76	equ	1072
	dw	?patch77
	dw	1
	db	3
	db	1
	db	0
	db	24
	db	9
	db	66
	db	67
	db	67
	db	51
	db	50
	db	32
	db	53
	db	46
	db	53
?patch77	equ	16
$$BSYMS	ends
$$BTYPES	segment byte public use32 'DEBTYP'
	db        2,0,0,0,14,0,8,0,3,0,0,0,0,0,1,0
	db        1,16,0,0,8,0,1,2,1,0,34,0,0,0,14,0
	db        8,0,18,0,0,0,0,0,0,0,3,16,0,0,4,0
	db        1,2,0,0,14,0,8,0,255,255,255,255,0,0,0,0
	db        5,16,0,0,4,0,1,2,0,0,18,0,3,0,112,0
	db        0,0,17,0,0,0,0,0,0,0,25,0,25,0,14,0
	db        8,0,255,255,255,255,0,0,0,0,8,16,0,0,4,0
	db        1,2,0,0,18,0,3,0,112,0,0,0,17,0,0,0
	db        0,0,0,0,49,0,49,0,8,0,1,0,1,0,34,0
	db        0,0,14,0,8,0,48,0,0,0,0,0,3,0,13,16
	db        0,0,8,0,2,0,42,0,116,0,0,0,16,0,1,2
	db        3,0,112,0,0,0,12,16,0,0,12,16,0,0,18,0
	db        3,0,112,0,0,0,17,0,0,0,0,0,0,0,0,1
	db        0,1,18,0,3,0,112,0,0,0,17,0,0,0,0,0
	db        0,0,10,0,10,0,14,0,8,0,3,0,0,0,0,0
	db        3,0,18,16,0,0,8,0,2,0,42,0,34,0,0,0
	db        16,0,1,2,3,0,17,16,0,0,17,16,0,0,17,16
	db        0,0,8,0,1,0,1,0,16,0,0,0,8,0,1,0
	db        1,0,33,0,0,0,14,0,8,0,3,0,0,0,7,0
	db        1,0,22,16,0,0,8,0,1,2,1,0,34,0,0,0
	db        14,0,8,0,3,4,0,0,0,0,3,0,24,16,0,0
	db        16,0,1,2,3,0,3,4,0,0,116,0,0,0,117,0
	db        0,0,14,0,8,0,3,4,0,0,0,0,3,0,28,16
	db        0,0,8,0,2,0,10,0,27,16,0,0,8,0,1,0
	db        1,0,3,0,0,0,16,0,1,2,3,0,3,4,0,0
	db        26,16,0,0,117,0,0,0,14,0,8,0,252,255,255,255
	db        4,0,2,0,31,16,0,0,8,0,2,0,10,4,255,255
	db        255,255,12,0,1,2,2,0,30,16,0,0,252,255,255,255
	db        14,0,8,0,254,255,255,255,4,0,2,0,35,16,0,0
	db        8,0,2,0,10,0,34,16,0,0,8,0,1,0,1,0
	db        112,0,0,0,12,0,1,2,2,0,30,16,0,0,33,16
	db        0,0,14,0,8,0,3,0,0,0,4,0,2,0,37,16
	db        0,0,12,0,1,2,2,0,30,16,0,0,116,0,0,0
	db        8,0,2,0,10,2,3,0,0,0,14,0,8,0,3,0
	db        0,0,4,0,1,0,40,16,0,0,8,0,1,2,1,0
	db        3,4,0,0,8,0,2,0,10,0,42,16,0,0,14,0
	db        8,0,116,0,0,0,7,0,4,0,44,16,0,0,8,0
	db        2,0,42,0,118,0,0,0,20,0,1,2,4,0,112,4
	db        0,0,43,16,0,0,43,16,0,0,118,4,0,0,14,0
	db        8,0,116,0,0,0,2,0,0,0,46,16,0,0,4,0
	db        1,2,0,0,14,0,8,0,116,0,0,0,7,0,1,0
	db        48,16,0,0,8,0,1,2,1,0,251,255,255,255
$$BTYPES	ends
$$BNAMES	segment byte public use32 'DEBNAM'
	db	8,'ExeCPUID'
	db	4,'veax'
	db	10,'GetCPUFreq'
	db	8,'GetManID'
	db	6,'return'
	db	2,'ID'
	db	10,'GetCPUType'
	db	6,'return'
	db	7,'CPUType'
	db	2,'id'
	db	1,'t'
	db	18,'GetDiskFreeSpaceMB'
	db	4,'Disk'
	db	9,'iTotSpace'
	db	8,'iTotFree'
	db	14,'totalFreeSpace'
	db	10,'totalSpace'
	db	9,'freeSpace'
	db	11,'tempFreeSpc'
	db	6,'szDisk'
	db	3,'ret'
	db	23,'GetGlobalMemoryStatusKB'
	db	12,'ulMemoryLoad'
	db	11,'ulTotalPhys'
	db	11,'ulAvailPhys'
	db	6,'statex'
	db	4,'deax'
	db	4,'debx'
	db	4,'decx'
	db	4,'dedx'
	db	9,'startFreq'
	db	8,'overFreq'
	db	3,'std'
	db	6,'System'
	db	19,'@System@varSmallint'
	db	18,'@System@varInteger'
	db	17,'@System@varSingle'
	db	17,'@System@varDouble'
	db	19,'@System@varCurrency'
	db	15,'@System@varDate'
	db	17,'@System@varOleStr'
	db	19,'@System@varDispatch'
	db	18,'@System@varBoolean'
	db	18,'@System@varVariant'
	db	18,'@System@varUnknown'
	db	15,'@System@varByte'
	db	16,'@System@varByRef'
	db	17,'@System@vtInteger'
	db	17,'@System@vtBoolean'
	db	14,'@System@vtChar'
	db	18,'@System@vtExtended'
	db	16,'@System@vtString'
	db	17,'@System@vtPointer'
	db	15,'@System@vtPChar'
	db	16,'@System@vtObject'
	db	15,'@System@vtClass'
	db	18,'@System@vtWideChar'
	db	19,'@System@vtPWideChar'
	db	20,'@System@vtAnsiString'
	db	18,'@System@vtCurrency'
	db	17,'@System@vtVariant'
	db	8,'Sysutils'
	db	7,'Sysinit'
	db	7,'Windows'
	db	8,'Messages'
	db	8,'Sysconst'
	db	7,'Classes'
	db	8,'Graphics'
	db	8,'Commctrl'
	db	7,'Imglist'
	db	8,'Actnlist'
	db	3,'Imm'
	db	7,'Contnrs'
	db	5,'Menus'
	db	8,'Multimon'
	db	8,'Controls'
	db	5,'Forms'
	db	7,'Activex'
	db	6,'Urlmon'
	db	7,'Wininet'
	db	6,'Regstr'
	db	8,'Shellapi'
	db	6,'Shlobj'
	db	8,'Stdctrls'
	db	7,'Commdlg'
	db	7,'Dialogs'
	db	8,'Extctrls'
	db	8,'Extctrls'
	db	7,'Dialogs'
	db	7,'Commdlg'
	db	8,'Stdctrls'
	db	6,'Shlobj'
	db	8,'Shellapi'
	db	6,'Regstr'
	db	7,'Wininet'
	db	6,'Urlmon'
	db	7,'Activex'
	db	5,'Forms'
	db	8,'Controls'
	db	8,'Multimon'
	db	5,'Menus'
	db	7,'Contnrs'
	db	3,'Imm'
	db	8,'Actnlist'
	db	7,'Imglist'
	db	8,'Commctrl'
	db	8,'Graphics'
	db	7,'Classes'
	db	8,'Sysutils'
	db	8,'Sysconst'
	db	8,'Messages'
	db	7,'Windows'
	db	7,'Sysinit'
	db	6,'System'
	db	5,'_rotr'
	db	5,'_rotl'
	db	6,'_crotr'
	db	6,'_lrotr'
	db	6,'_lrotl'
	db	6,'_crotl'
	db	3,'abs'
	db	6,'wctomb'
	db	7,'wcstoul'
	db	8,'wcstombs'
	db	6,'wcstol'
	db	6,'wcstod'
	db	5,'ultoa'
	db	4,'time'
	db	6,'system'
	db	4,'swab'
	db	7,'strtoul'
	db	6,'strtol'
	db	6,'strtod'
	db	5,'srand'
	db	7,'realloc'
	db	4,'rand'
	db	6,'putenv'
	db	6,'perror'
	db	6,'mbtowc'
	db	8,'mbstowcs'
	db	5,'mblen'
	db	13,'_seterrormode'
	db	26,'@std@%max$T1%$qrxT1t1$rxT1'
	db	26,'@std@%min$T1%$qrxT1t1$rxT1'
	db	17,'@std@randomize$qv'
	db	14,'@std@random$qi'
	db	4,'ltoa'
	db	4,'gcvt'
	db	4,'fcvt'
	db	4,'ecvt'
	db	6,'malloc'
	db	4,'ldiv'
	db	4,'labs'
	db	4,'itoa'
	db	6,'getenv'
	db	4,'free'
	db	4,'exit'
	db	3,'div'
	db	6,'calloc'
	db	4,'atol'
	db	4,'atoi'
	db	4,'atof'
	db	6,'atexit'
	db	5,'abort'
	db	6,'_wtold'
	db	5,'_wtol'
	db	5,'_wtoi'
	db	5,'_wtof'
	db	8,'_wsystem'
	db	11,'_wsplitpath'
	db	11,'_wsearchstr'
	db	11,'_wsearchenv'
	db	8,'_wputenv'
	db	8,'_wperror'
	db	10,'_wmakepath'
	db	8,'_wgetenv'
	db	10,'_wfullpath'
	db	8,'_wcstold'
	db	8,'_strtold'
	db	10,'_splitpath'
	db	10,'_searchstr'
	db	10,'_searchenv'
	db	9,'_makepath'
	db	7,'_wtoi64'
	db	8,'_ui64tow'
	db	8,'_ui64toa'
	db	7,'_i64tow'
	db	7,'_i64toa'
	db	7,'_atoi64'
	db	5,'_ltow'
	db	5,'_ltoa'
	db	6,'_lrand'
	db	5,'_itow'
	db	6,'_ultow'
	db	5,'_gcvt'
	db	5,'_fcvt'
	db	9,'_fullpath'
	db	5,'_exit'
	db	6,'_atold'
	db	5,'_ecvt'
	db	7,'__errno'
	db	10,'__doserrno'
	db	5,'qsort'
	db	7,'lsearch'
	db	5,'lfind'
	db	7,'bsearch'
	db	7,'wcsxfrm'
	db	6,'wcstok'
	db	6,'wcsstr'
	db	6,'wcsspn'
	db	7,'wcsrchr'
	db	7,'wcspcpy'
	db	7,'wcspbrk'
	db	7,'wcsncpy'
	db	7,'wcsncmp'
	db	7,'wcsncat'
	db	6,'wcslen'
	db	7,'wcscspn'
	db	6,'wcscpy'
	db	7,'wcscoll'
	db	6,'wcscmp'
	db	6,'wcschr'
	db	6,'wcscat'
	db	6,'strtok'
	db	18,'@std@strstr$qpxct1'
	db	18,'@std@strstr$qpcpxc'
	db	6,'strspn'
	db	19,'@std@strpbrk$qpxct1'
	db	19,'@std@strpbrk$qpcpxc'
	db	8,'strerror'
	db	7,'strcspn'
	db	18,'@std@strrchr$qpxci'
	db	17,'@std@strrchr$qpci'
	db	7,'strncpy'
	db	7,'strncmp'
	db	7,'strncat'
	db	6,'strlen'
	db	6,'strcpy'
	db	6,'strcmp'
	db	17,'@std@strchr$qpxci'
	db	16,'@std@strchr$qpci'
	db	6,'strcat'
	db	6,'strset'
	db	6,'stpcpy'
	db	7,'strnset'
	db	7,'_wcsset'
	db	7,'_wcsrev'
	db	8,'_wcspcpy'
	db	8,'_wcsnset'
	db	10,'_wcsnicoll'
	db	9,'_wcsnicmp'
	db	9,'_wcsncoll'
	db	9,'_wcsicoll'
	db	8,'_wcsicmp'
	db	7,'_wcsdup'
	db	10,'_strnicoll'
	db	9,'_strncoll'
	db	9,'_stricoll'
	db	9,'_strerror'
	db	9,'_lwcsxfrm'
	db	8,'_lwcsupr'
	db	11,'_lwcsnicoll'
	db	10,'_lwcsncoll'
	db	8,'_lwcslwr'
	db	10,'_lwcsicoll'
	db	9,'_lwcscoll'
	db	9,'_lstrxfrm'
	db	11,'_lstrnicoll'
	db	10,'_lstrncoll'
	db	10,'_lstricoll'
	db	9,'_lstrcoll'
	db	6,'strrev'
	db	8,'strnicmp'
	db	7,'stricmp'
	db	6,'strdup'
	db	7,'_stpcpy'
	db	8,'_lstrupr'
	db	8,'_lstrlwr'
	db	12,'_lwsetlocale'
	db	11,'_wsetlocale'
	db	12,'_llocaleconv'
	db	10,'localeconv'
	db	11,'_lsetlocale'
	db	10,'_ltowlower'
	db	10,'_ltowupper'
	db	8,'towupper'
	db	8,'towlower'
	db	9,'_ltoupper'
	db	7,'toupper'
	db	9,'_ltolower'
	db	7,'tolower'
	db	8,'iswascii'
	db	9,'iswxdigit'
	db	8,'iswupper'
	db	8,'iswspace'
	db	8,'iswpunct'
	db	8,'iswprint'
	db	8,'iswlower'
	db	8,'iswgraph'
	db	8,'iswdigit'
	db	8,'iswcntrl'
	db	8,'iswalpha'
	db	8,'iswalnum'
	db	7,'isascii'
	db	8,'isxdigit'
	db	7,'isupper'
	db	7,'isspace'
	db	7,'ispunct'
	db	7,'isprint'
	db	7,'islower'
	db	7,'isgraph'
	db	7,'isdigit'
	db	7,'iscntrl'
	db	7,'isalpha'
	db	7,'isalnum'
	db	6,'memset'
	db	6,'memcpy'
	db	6,'memcmp'
	db	19,'@std@memchr$qpxviui'
	db	18,'@std@memchr$qpviui'
	db	7,'memmove'
	db	7,'memicmp'
	db	7,'memccpy'
	db	8,'_wmemset'
	db	8,'_wmemcpy'
	db	21,'@std@_wmemchr$qpxviui'
	db	20,'@std@_wmemchr$qpviui'
	db	10,'__threadid'
	db	14,'@std@ptrdiff_t'
	db	11,'@std@size_t'
	db	11,'@std@wint_t'
	db	13,'@std@wctype_t'
	db	12,'@std@va_list'
	db	9,'_chartype'
	db	6,'_lower'
	db	6,'_upper'
	db	5,'lconv'
	db	5,'_argc'
	db	5,'_argv'
	db	12,'__mb_cur_max'
	db	16,'_cmdline_escapes'
	db	8,'_environ'
	db	9,'_fileinfo'
	db	6,'_fmode'
	db	8,'_osmajor'
	db	8,'_osminor'
	db	7,'_osmode'
	db	10,'_osversion'
	db	12,'_sys_errlist'
	db	9,'_sys_nerr'
	db	9,'_wenviron'
	db	13,'@std@atexit_t'
	db	10,'@std@div_t'
	db	11,'@std@ldiv_t'
$$BNAMES	ends
	?debug	D "D:\HT9045\HT9046LS_Code_V3.24P_305_20150724_Siguard\SECSGEM\TasmInfo.h" 18160 48911
	?debug	D "C:\PROGRAM FILES\BORLAND\CBUILDER5\INCLUDE\VCL\extctrls.hpp" 10303 10240
	?debug	D "C:\PROGRAM FILES\BORLAND\CBUILDER5\INCLUDE\commdlg.h" 10303 10240
	?debug	D "C:\PROGRAM FILES\BORLAND\CBUILDER5\INCLUDE\VCL\CommDlg.hpp" 10303 10240
	?debug	D "C:\PROGRAM FILES\BORLAND\CBUILDER5\INCLUDE\VCL\StdCtrls.hpp" 10303 10240
	?debug	D "C:\PROGRAM FILES\BORLAND\CBUILDER5\INCLUDE\shellapi.h" 10303 10240
	?debug	D "C:\PROGRAM FILES\BORLAND\CBUILDER5\INCLUDE\VCL\ShellAPI.hpp" 10303 10240
	?debug	D "C:\PROGRAM FILES\BORLAND\CBUILDER5\INCLUDE\regstr.h" 10303 10240
	?debug	D "C:\PROGRAM FILES\BORLAND\CBUILDER5\INCLUDE\VCL\RegStr.hpp" 10303 10240
	?debug	D "C:\PROGRAM FILES\BORLAND\CBUILDER5\INCLUDE\wininet.h" 10303 10240
	?debug	D "C:\PROGRAM FILES\BORLAND\CBUILDER5\INCLUDE\VCL\WinInet.hpp" 10303 10240
	?debug	D "C:\PROGRAM FILES\BORLAND\CBUILDER5\INCLUDE\objidl.h" 10303 10240
	?debug	D "C:\PROGRAM FILES\BORLAND\CBUILDER5\INCLUDE\oaidl.h" 10303 10240
	?debug	D "C:\PROGRAM FILES\BORLAND\CBUILDER5\INCLUDE\oleauto.h" 10303 10240
	?debug	D "C:\PROGRAM FILES\BORLAND\CBUILDER5\INCLUDE\VCL\ActiveX.hpp" 10303 10240
	?debug	D "C:\PROGRAM FILES\BORLAND\CBUILDER5\INCLUDE\VCL\UrlMon.hpp" 10303 10240
	?debug	D "C:\PROGRAM FILES\BORLAND\CBUILDER5\INCLUDE\VCL\ShlObj.hpp" 10303 10240
	?debug	D "C:\PROGRAM FILES\BORLAND\CBUILDER5\INCLUDE\VCL\dialogs.hpp" 10303 10240
	?debug	D "C:\PROGRAM FILES\BORLAND\CBUILDER5\INCLUDE\VCL\forms.hpp" 10303 10240
	?debug	D "C:\PROGRAM FILES\BORLAND\CBUILDER5\INCLUDE\multimon.h" 10303 10240
	?debug	D "C:\PROGRAM FILES\BORLAND\CBUILDER5\INCLUDE\VCL\MultiMon.hpp" 10303 10240
	?debug	D "C:\PROGRAM FILES\BORLAND\CBUILDER5\INCLUDE\VCL\Contnrs.hpp" 10303 10240
	?debug	D "C:\PROGRAM FILES\BORLAND\CBUILDER5\INCLUDE\VCL\Menus.hpp" 10303 10240
	?debug	D "C:\PROGRAM FILES\BORLAND\CBUILDER5\INCLUDE\VCL\Imm.hpp" 10303 10240
	?debug	D "C:\PROGRAM FILES\BORLAND\CBUILDER5\INCLUDE\CommCtrl.h" 10303 10240
	?debug	D "C:\PROGRAM FILES\BORLAND\CBUILDER5\INCLUDE\VCL\Commctrl.hpp" 10303 10240
	?debug	D "C:\PROGRAM FILES\BORLAND\CBUILDER5\INCLUDE\VCL\ImgList.hpp" 10303 10240
	?debug	D "C:\PROGRAM FILES\BORLAND\CBUILDER5\INCLUDE\VCL\ActnList.hpp" 10303 10240
	?debug	D "C:\PROGRAM FILES\BORLAND\CBUILDER5\INCLUDE\VCL\controls.hpp" 10303 10240
	?debug	D "C:\PROGRAM FILES\BORLAND\CBUILDER5\INCLUDE\VCL\graphics.hpp" 10303 10240
	?debug	D "C:\PROGRAM FILES\BORLAND\CBUILDER5\INCLUDE\VCL\classes.hpp" 10303 10240
	?debug	D "C:\PROGRAM FILES\BORLAND\CBUILDER5\INCLUDE\VCL\SysConst.hpp" 10303 10240
	?debug	D "C:\PROGRAM FILES\BORLAND\CBUILDER5\INCLUDE\VCL\sysutils.hpp" 10303 10240
	?debug	D "C:\PROGRAM FILES\BORLAND\CBUILDER5\INCLUDE\VCL\messages.hpp" 10303 10240
	?debug	D "C:\PROGRAM FILES\BORLAND\CBUILDER5\INCLUDE\dde.h" 10303 10240
	?debug	D "C:\PROGRAM FILES\BORLAND\CBUILDER5\INCLUDE\wincrypt.h" 10303 10240
	?debug	D "C:\PROGRAM FILES\BORLAND\CBUILDER5\INCLUDE\wintrust.h" 10303 10240
	?debug	D "C:\PROGRAM FILES\BORLAND\CBUILDER5\INCLUDE\VCL\SysInit.hpp" 10303 10240
	?debug	D "C:\PROGRAM FILES\BORLAND\CBUILDER5\INCLUDE\VCL\windows.hpp" 10303 10240
	?debug	D "C:\PROGRAM FILES\BORLAND\CBUILDER5\INCLUDE\unknwn.h" 10303 10240
	?debug	D "C:\PROGRAM FILES\BORLAND\CBUILDER5\INCLUDE\VCL\sysvari.h" 10303 10240
	?debug	D "C:\PROGRAM FILES\BORLAND\CBUILDER5\INCLUDE\VCL\sysopen.h" 10303 10240
	?debug	D "C:\PROGRAM FILES\BORLAND\CBUILDER5\INCLUDE\search.h" 10303 10240
	?debug	D "C:\PROGRAM FILES\BORLAND\CBUILDER5\INCLUDE\stdlib.h" 10303 10240
	?debug	D "C:\PROGRAM FILES\BORLAND\CBUILDER5\INCLUDE\VCL\sysdyn.h" 10303 10240
	?debug	D "C:\PROGRAM FILES\BORLAND\CBUILDER5\INCLUDE\VCL\systvar.h" 10303 10240
	?debug	D "C:\PROGRAM FILES\BORLAND\CBUILDER5\INCLUDE\VCL\sysset.h" 10303 10240
	?debug	D "C:\PROGRAM FILES\BORLAND\CBUILDER5\INCLUDE\VCL\syscomp.h" 10303 10240
	?debug	D "C:\PROGRAM FILES\BORLAND\CBUILDER5\INCLUDE\VCL\syscurr.h" 10303 10240
	?debug	D "C:\PROGRAM FILES\BORLAND\CBUILDER5\INCLUDE\VCL\systdate.h" 10303 10240
	?debug	D "C:\PROGRAM FILES\BORLAND\CBUILDER5\INCLUDE\VCL\systobj.h" 10303 10240
	?debug	D "C:\PROGRAM FILES\BORLAND\CBUILDER5\INCLUDE\rpcnsip.h" 10303 10240
	?debug	D "C:\PROGRAM FILES\BORLAND\CBUILDER5\INCLUDE\rpcndr.h" 10303 10240
	?debug	D "C:\PROGRAM FILES\BORLAND\CBUILDER5\INCLUDE\rpcnterr.h" 10303 10240
	?debug	D "C:\PROGRAM FILES\BORLAND\CBUILDER5\INCLUDE\rpcnsi.h" 10303 10240
	?debug	D "C:\PROGRAM FILES\BORLAND\CBUILDER5\INCLUDE\rpcdcep.h" 10303 10240
	?debug	D "C:\PROGRAM FILES\BORLAND\CBUILDER5\INCLUDE\rpcdce.h" 10303 10240
	?debug	D "C:\PROGRAM FILES\BORLAND\CBUILDER5\INCLUDE\rpc.h" 10303 10240
	?debug	D "C:\PROGRAM FILES\BORLAND\CBUILDER5\INCLUDE\wtypes.h" 10303 10240
	?debug	D "C:\PROGRAM FILES\BORLAND\CBUILDER5\INCLUDE\VCL\wstring.h" 10303 10240
	?debug	D "C:\PROGRAM FILES\BORLAND\CBUILDER5\INCLUDE\VCL\sysmac.h" 10303 10240
	?debug	D "C:\PROGRAM FILES\BORLAND\CBUILDER5\INCLUDE\VCL\dstring.h" 10303 10240
	?debug	D "C:\PROGRAM FILES\BORLAND\CBUILDER5\INCLUDE\VCL\sysclass.H" 10303 10240
	?debug	D "C:\PROGRAM FILES\BORLAND\CBUILDER5\INCLUDE\basetyps.h" 10303 10240
	?debug	D "C:\PROGRAM FILES\BORLAND\CBUILDER5\INCLUDE\prsht.h" 10303 10240
	?debug	D "C:\PROGRAM FILES\BORLAND\CBUILDER5\INCLUDE\winspool.h" 10303 10240
	?debug	D "C:\PROGRAM FILES\BORLAND\CBUILDER5\INCLUDE\imm.h" 10303 10240
	?debug	D "C:\PROGRAM FILES\BORLAND\CBUILDER5\INCLUDE\mcx.h" 10303 10240
	?debug	D "C:\PROGRAM FILES\BORLAND\CBUILDER5\INCLUDE\winsvc.h" 10303 10240
	?debug	D "C:\PROGRAM FILES\BORLAND\CBUILDER5\INCLUDE\winnetwk.h" 10303 10240
	?debug	D "C:\PROGRAM FILES\BORLAND\CBUILDER5\INCLUDE\winreg.h" 10303 10240
	?debug	D "C:\PROGRAM FILES\BORLAND\CBUILDER5\INCLUDE\winver.h" 10303 10240
	?debug	D "C:\PROGRAM FILES\BORLAND\CBUILDER5\INCLUDE\wincon.h" 10303 10240
	?debug	D "C:\PROGRAM FILES\BORLAND\CBUILDER5\INCLUDE\winnls.h" 10303 10240
	?debug	D "C:\PROGRAM FILES\BORLAND\CBUILDER5\INCLUDE\tvout.h" 10303 10240
	?debug	D "C:\PROGRAM FILES\BORLAND\CBUILDER5\INCLUDE\winuser.h" 10303 10240
	?debug	D "C:\PROGRAM FILES\BORLAND\CBUILDER5\INCLUDE\pshpack1.h" 10303 10240
	?debug	D "C:\PROGRAM FILES\BORLAND\CBUILDER5\INCLUDE\wingdi.h" 10303 10240
	?debug	D "C:\PROGRAM FILES\BORLAND\CBUILDER5\INCLUDE\winerror.h" 10303 10240
	?debug	D "C:\PROGRAM FILES\BORLAND\CBUILDER5\INCLUDE\winbase.h" 10303 10240
	?debug	D "C:\PROGRAM FILES\BORLAND\CBUILDER5\INCLUDE\pshpack8.h" 10303 10240
	?debug	D "C:\PROGRAM FILES\BORLAND\CBUILDER5\INCLUDE\pshpack2.h" 10303 10240
	?debug	D "C:\PROGRAM FILES\BORLAND\CBUILDER5\INCLUDE\poppack.h" 10303 10240
	?debug	D "C:\PROGRAM FILES\BORLAND\CBUILDER5\INCLUDE\pshpack4.h" 10303 10240
	?debug	D "C:\PROGRAM FILES\BORLAND\CBUILDER5\INCLUDE\_loc.h" 10303 10240
	?debug	D "C:\PROGRAM FILES\BORLAND\CBUILDER5\INCLUDE\locale.h" 10303 10240
	?debug	D "C:\PROGRAM FILES\BORLAND\CBUILDER5\INCLUDE\_str.h" 10303 10240
	?debug	D "C:\PROGRAM FILES\BORLAND\CBUILDER5\INCLUDE\string.h" 10303 10240
	?debug	D "C:\PROGRAM FILES\BORLAND\CBUILDER5\INCLUDE\guiddef.h" 10303 10240
	?debug	D "C:\PROGRAM FILES\BORLAND\CBUILDER5\INCLUDE\basetsd.h" 10303 10240
	?debug	D "C:\PROGRAM FILES\BORLAND\CBUILDER5\INCLUDE\mbctype.h" 10303 10240
	?debug	D "C:\PROGRAM FILES\BORLAND\CBUILDER5\INCLUDE\ctype.h" 10303 10240
	?debug	D "C:\PROGRAM FILES\BORLAND\CBUILDER5\INCLUDE\winnt.h" 10303 10240
	?debug	D "C:\PROGRAM FILES\BORLAND\CBUILDER5\INCLUDE\windef.h" 10303 10240
	?debug	D "C:\PROGRAM FILES\BORLAND\CBUILDER5\INCLUDE\stdarg.h" 10303 10240
	?debug	D "C:\PROGRAM FILES\BORLAND\CBUILDER5\INCLUDE\excpt.h" 10303 10240
	?debug	D "C:\PROGRAM FILES\BORLAND\CBUILDER5\INCLUDE\_null.h" 10303 10240
	?debug	D "C:\PROGRAM FILES\BORLAND\CBUILDER5\INCLUDE\_defs.h" 10303 10240
	?debug	D "C:\PROGRAM FILES\BORLAND\CBUILDER5\INCLUDE\_stddef.h" 10303 10240
	?debug	D "C:\PROGRAM FILES\BORLAND\CBUILDER5\INCLUDE\mem.h" 10303 10240
	?debug	D "C:\PROGRAM FILES\BORLAND\CBUILDER5\INCLUDE\windows.h" 10303 10240
	?debug	D "C:\PROGRAM FILES\BORLAND\CBUILDER5\INCLUDE\VCL\sysmac.H" 10303 10240
	?debug	D "C:\PROGRAM FILES\BORLAND\CBUILDER5\INCLUDE\VCL\system.hpp" 10303 10240
	?debug	D "C:\PROGRAM FILES\BORLAND\CBUILDER5\INCLUDE\VCL\vcl0.h" 10303 10240
	?debug	D "C:\PROGRAM FILES\BORLAND\CBUILDER5\INCLUDE\VCL\VCL.H" 10303 10240
	?debug	D "D:\HT9045\HT9046LS_Code_V3.24P_305_20150724_Siguard\SECSGEM\TasmInfo.cpp" 18169 28491
 ?debug  C FB062A633A5C50524F4752417E315C626F726C616E645C434255494C447E315C6C69625C76636C35302E233030DFF419002370F946
 ?debug  C FB050B23416E7369537472696E67
 ?debug  C FB050C232A416E7369537472696E67
 ?debug  C FB0510235F4D454D4F52595354415455534558
 ?debug  C FB050C2326416E7369537472696E67
 ?debug  C FB0511232A5F4D454D4F52595354415455534558
	end
