" vim: ts=4 sw=4 sts=4
" ==============================================================================
"  .vimrc 設定ファイル
"  Copyright (c)  2009 Nomura Kei
" ==============================================================================

" ------------------------------------------------------------------------------
"   dein.vim 読み込み
" ------------------------------------------------------------------------------
let s:script_dir = expand('<sfile>:p:h')	" 本ファイルの位置を取得
let s:dein_script = s:script_dir . '/.vim/settings/dein.vim'
if filereadable(s:dein_script)
	exe("source" . s:dein_script)
endif


" ------------------------------------------------------------------------------
"   設定
" ------------------------------------------------------------------------------
syntax enable

" ---- NERDTree 設定
let g:NERDTreeShowBookmarks = 1
if !argc()
	autocmd vimenter * Msys2
	autocmd vimenter * NERDTree|normal gg3j
endif
autocmd bufenter * if (winnr("$") == 1 && exists("b:NERDTree") && b:NERDTree.isTabTree()) | q | endif


" --- ブラウザ利用設定
let g:netrw_nogx = 1 " disable netrw's gx mapping
nmap gx <Plug>(openbrowser-smart-search)
vmap gx <Plug>(openbrowser-smart-search)
let g:openbrowser_browser_commands = [
			\	{ 'name' : 'chrome.exe',
			\	'args' : [ 'start', '{browser}', '{uri}' ] }
			\]

" --- Markdown 設定
"  パス区切りの問題で、shellslash を ON にする
set shellslash
autocmd BufRead,BufNewFile *.md set filetype=markdown
nnoremap <silent> <C-p> :PrevimOpen<CR> " Ctrl+P でプレビュー

" --- PlantUML 設定
autocmd BufWritePost *.uml silent !puml <afile>

" command Msys2 terminal /c/Tools/msys2/usr/bin/bash.exe
command Msys2 terminal

set splitbelow
set number
set fileencoding=utf-8
set fileencodings=utf-8,sjis,iso-2202-jp-3,euc-jisx0213,guess,ucs-bom,ucs-2le,ucs-2
set fileformat=unix
set ts=4 sw=4 sts=4
"set backupdir=~/.tmp/vim-backup
"set undodir=~/.tmp/vim-backup
set backspace=indent,eol,start

