" vim: ts=4 sw=4 sts=4
" ======================================================================
"  .vimrc 設定ファイル
"  Copyright (c) 2009-2022  Nomura Kei
" ======================================================================

" ----------------------------------------------------------------------
"  dein.vim 読み込み
" ----------------------------------------------------------------------
let s:script_dir = expand('<sfile>:p:h')	" 本ファイル位置取得
let s:dein_script = s:script_dir . '/.vim/settings/dein.vim'
if filereadable(s:dein_script)
	exe("source" . s:dein_script)
endif

" ----------------------------------------------------------------------
"  基本設定
" ----------------------------------------------------------------------
syntax enable
set number
set fileencoding=utf-8
set fileencodings=utf-8,sjis,iso-2022-jp-3,euc-jisx0213,guess
set fileformat=unix
set ts=4 sw=4 sts=4
set clipboard=unnamed,autoselect

" ■ ステータスライン
"   %<                          : 最低限ここまで表示
"   %=                          : 右寄せ/左寄せ区切り
"   %F                          : ファイルフルパス表示 (%f:ファイルパス, %t:ファイル名)
"   %m                          : 修正フラグ[+]        (%M:+)
"   %h                          : ヘルプ表示中[Help]   (%H:HLP)
"   %r                          : 読取専用[RO]         (%R:RO)
"   %w                          : [PREVIEW]
"   [%{&fenc!=''?&fenc:&enc}]   : 文字コード表示
"   [%{&ff}]                    : ファイルフォーマット [doc/unix/mac]
"   [%Y]                        : ファイルタイプ       (%y:小文字表記)
"   [CODE=0x%02B]               : カーソル位置の文字コード
"   [%l,%v]                     : 行番号、桁番号
"   [%L]                        : ファイル行数
"  
set statusline=%F%m%h%w%<\ [%{&fenc!=''?&fenc:&enc}][%{&ff}][%Y]%=%l,%c%V\ [0x%02B]
set laststatus=2

" TermDebug
packadd termdebug

