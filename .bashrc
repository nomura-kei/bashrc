# vim: ts=4 sw=4 sts=4
################################################################################
## Script    : .bashrc
## Name      : bash 設定ファイル
## Version   : 0.0.2
## Copyright : 2018-2022  Nomura Kei
## License   : BAD-2-Clause
################################################################################

# 対話モードでない場合は、終了する。
# [理由] .bashrc で標準出力/標準エラー出力等した場合、rsync などが失敗するため
case $- in
    *i*) ;;
      *) return;;
esac

# 設定ファイル読込
#
# 次のファイルを読み込みます。
# 1. com-*
# 2. host-{ホスト名}*-*
# 3. sys-{システム名}*-*   (Ex. Linux, MSYS_NT-x.x-xxxxx)
# 4. term-{端末}*-*        (Ex. linux, xterm, xterm-256color)
#
UNAME=`uname`
HOSTNAME=`hostname`
for conf_file in `\ls -1	~/.bashrc.d/com-* \
							~/.bashrc.d/host-${HOSTNAME}*-*	\
							~/.bashrc.d/sys-${UNAME}*-*		\
							~/.bashrc.d/term-${TERM}*-*		\
							2>/dev/null`; do
	. ${conf_file}
done

