" ==============================================================================
"  dein.vim
"  Copyright 2019  Nomura Kei
" ==============================================================================
"
if &compatible
	set nocompatible
endif

let s:dein_dir = expand('~/.vim/cache/dein')
let s:dein_repo_dir = s:dein_dir . '/repos/github.com/Shougo/dein.vim'

if &runtimepath !~# '/dein.vim'
	if !isdirectory(s:dein_repo_dir)
		execute '!git clone https://github.com/Shougo/dein.vim' fnameescape(s:dein_repo_dir)
	endif
	execute 'set runtimepath^=' . fnameescape(s:dein_repo_dir)
endif

if dein#load_state(s:dein_dir)
	call dein#begin(s:dein_dir)

	" For Uninstall
	" call map(dein#check_clean(), "delete (v:val, 'rF')")

	let g:rc_dir    = expand('~/.vim/rc')
	let s:toml      = g:rc_dir . '/dein.toml'
	let s:lazy_toml = g:rc_dir . '/dein_lazy.toml'
	call dein#load_toml(s:toml,      {'lazy': 0})
	call dein#load_toml(s:lazy_toml, {'lazy': 1})
	call dein#end()
	call dein#save_state()
endif
filetype plugin  indent on
if dein#check_install()
	call dein#install()
endif

