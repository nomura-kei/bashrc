" vim: ts=4 sw=4 sts=4
" ======================================================================
"  .vimrc 設定ファイル
"  Copyright (c) 2009-2022  Nomura Kei
" ======================================================================



" ----------------------------------------------------------------------
"  Plugin
" ----------------------------------------------------------------------
"if !has("nvim")
if empty(glob('~/.vim/autoload/plug.vim'))
	silent !curl -fLo ~/.vim/autoload/plugin.vim --create-dirs
				\ https://raw.githubusercontent.com/junegunn/vim-plug/master/plug.vim
	autocmd VimEnter * PlugInstall
endif
call plug#begin('~/.vim/plugged')
Plug 'vim-jp/vimdoc-ja'
Plug 'easymotion/vim-easymotion'
Plug 'machakann/vim-sandwich'
Plug 'posva/vim-vue'
Plug 'lambdalisue/fern.vim'
Plug 'lambdalisue/fern-git-status.vim'
Plug 'lambdalisue/fern-hijack.vim'
Plug 'lambdalisue/nerdfont.vim'
Plug 'lambdalisue/fern-renderer-nerdfont.vim'
Plug 'lambdalisue/glyph-palette.vim'
Plug 'yuki-yano/fern-preview.vim'
Plug 'airblade/vim-gitgutter'
Plug 'junegunn/fzf', { 'dir':'~/.fzf', 'do':'./install --all' }
Plug 'junegunn/fzf.vim'
Plug 'ryanoasis/vim-devicons'
Plug 'ojroques/vim-oscyank'
call plug#end()


" ----------------------------------------------------------------------
"  基本設定
" ----------------------------------------------------------------------
syntax enable
set number
set encoding=utf-8
set fileencoding=utf-8
set fileencodings=utf-8,sjis,iso-2022-jp-3,euc-jisx0213,guess
set fileformat=unix
set ts=4 sw=4 sts=4
set signcolumn=yes
if !has('nvim')
    set clipboard=unnamed,autoselect
	set ttymouse=xterm2
endif
if has('win32')
    set guifont=HackGen\ Console\ NFJ:h11
endif

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
set mouse=a


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
let g:fern#renderer = 'nerdfont'
augroup my-glyph-palette
	autocmd! *
	autocmd FileType fern call glyph_palette#apply()
	autocmd FileType nerdtree,startify call glpyh_palette#apply()
augroup END
" For oscyank
augroup osc52
    autocmd!
    autocmd TextYankPost *
			\ if v:event.operator is 'y' |
			\ execute 'OSCYankRegister' |
			\ endif
augroup END

" For ClearTerminal
if has('nvim')
function! ClearTerminal()
	set scrollback=1
	let &g:scrollback=1
	echo &scrollback
	call feedkeys("\i")
	call feedkeys("clear\<CR>")
	call feedkeys("\<C-\>\<C-n>")
	call feedkeys("\i")
	sleep 3
	let &scrollback=s:scrolll_value
endfunction
endif

