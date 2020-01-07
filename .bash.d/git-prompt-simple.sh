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

GIT_PS1_DIR=~/.tmp/git_ps1/`tty`
GIT_PS1_ENV=${GIT_PS1_DIR}/git_ps1.env
mkdir -p ${GIT_PS1_DIR}
touch    ${GIT_PS1_ENV}

__git_ps1() {
	. ${GIT_PS1_ENV}
	if [ "${LAST_PWD}" != "${PWD}" ]; then
		BRANCH=$(git rev-parse --abbrev-ref HEAD 2> /dev/null)
		STATUS=
		if [ ! -z $BRANCH ] && [ ! -z "$(git status --short)" ]; then
			STATUS=!
		fi
cat <<EOF > ${GIT_PS1_ENV}
BRANCH=${BRANCH}
STATUS=${STATUS}
LAST_PWD=${PWD}
EOF
	fi
	if [ ! -z $BRANCH ]; then
		echo -n -e "\e[33m(${BRANCH}\e[31m${STATUS})"
	fi
}


