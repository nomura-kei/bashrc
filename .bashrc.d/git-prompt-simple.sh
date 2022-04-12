# vim: ts=4 sw=4 sts=4 ff=unix fenc=utf-8:
[[ "$-" != *i* ]] && return
echo "[load] ${BASH_SOURCE:-$0}"

__git_ps1_branch() {
BRANCH=$(git rev-parse --abbrev-ref HEAD 2> /dev/null)
	if [ ! -z $BRANCH ]; then
		echo -n "${BRANCH}"
	fi
}
__git_ps1_status() {
	if [ ! -z "$(git status --short 2> /dev/null)" ]; then
		STATUS=!
	fi
	echo -n "${STATUS}"
}

