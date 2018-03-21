#!/bin/sh -u
# tools/update-upstream.sh -- files to their latest version
# Copyright (C) 2017  Olaf Meeuwissen
#
# License: GPL-3.0+

fetch () {
    if type curl 2>/dev/null >/dev/null ; then
        curl --silent --remote-name $1
        return
    fi
    if type wget 2>/dev/null >/dev/null ; then
        wget --quiet --output-document $(echo $1 | sed 's,.*/,,') $1
    fi
}

CONFIG_BASE_URL=http://git.savannah.gnu.org/cgit/config.git/plain

for file in config.guess config.sub; do
    fetch $CONFIG_BASE_URL/$file
done
