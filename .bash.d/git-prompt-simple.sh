# vim: ts=4 sw=4 sts=4
################################################################################
## Script    : git-prompt-simple.sh
## Name      : bash 設定ファイル [git 状態表示]
## Version   : 0.0.1
## Copyright : 2018-2019  Nomura Kei
## License   : BSD-2-Clause
################################################################################
[[ "$-" != *i* ]] && return
echo  "[load] ${BASH_SOURCE:-$0}"

__git_ps1() {
BRANCH=$(git rev-parse --abbrev-ref HEAD 2> /dev/null)
if [ ! -z $BRANCH ]; then
	STATUS=
	if [ ! -z "$(git status --short)" ]; then
		STATUS=!
	fi
	echo -n -e "\e[33m(${BRANCH}\e[31m${STATUS}\e[33m)"
fi
}
