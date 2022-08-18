" vim: ts=4 sw=4 sts=4
" ======================================================================
"  .vimrc 設定ファイル
"  Copyright (c) 2009-2022  Nomura Kei
" ======================================================================



" ----------------------------------------------------------------------
"  Plugin
" ----------------------------------------------------------------------
call plug#begin('~/.vim/plugged')
Plug 'vim-jp/vimdoc-ja'
Plug 'easymotion/vim-easymotion'
Plug 'machakann/vim-sandwich'
Plug 'posva/vim-vue'
Plug 'lambdalisue/fern.vim'
Plug 'lambdalisue/fern-git-status.vim'
Plug 'lambdalisue/fern-hijack.vim'
Plug 'yuki-yano/fern-preview.vim'
Plug 'airblade/vim-gitgutter'
Plug 'junegunn/fzf.vim'
call plug#end()


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

" ------------------------------------------------------------------------------
"  Plugin 設定
" ------------------------------------------------------------------------------
" For fern, fern-preview
nnoremap <C-n> :Fern . -reveal=% -drawer -toggle -width=40<CR>
function! s:fern_settings() abort
	nmap <silent> <buffer> p     <Plug>(fern-action-preview:toggle)
	nmap <silent> <buffer> <C-p> <Plug>(fern-action-preview:auto:toggle)
	nmap <silent> <buffer> <C-d> <Plug>(fern-action-preview:scroll:down:half)
	nmap <silent> <buffer> <C-u> <Plug>(fern-action-preview:scroll:up:half)
endfunction
augroup fern-settings
	autocmd!
	autocmd FileType fern call s:fern_settings()
augroup END

