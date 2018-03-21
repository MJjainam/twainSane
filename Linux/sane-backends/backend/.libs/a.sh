# @echo "Libraries exporting 'illegal' symbols:"
	for lib in ./*.so; do \
	lines=`nm -g $lib|grep '\( T \)\|\( D \)'|egrep -v ' _fini| _init'|egrep -v ' sane_| sanei_'`; \
	if test -n "$lines" ; then \
	  echo -e "*** $lib:\n$lines"; \
	  found_errors="yes"; \
	fi \
	done; \
	echo $found_errors