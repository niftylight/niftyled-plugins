#!/bin/bash

# generate version string

######################
# release version
MAJOR=0
MINOR=1
MICRO=1



# ----------------------------------------------

# function to print helptext
function help()
{
    echo "$0 --long | --short | --major | --minor | --micro | --git"
}

# output micro version
function micro()
{
    echo -n $MICRO
}

# output minor version
function minor()
{
    echo -n $MINOR
}

# output major version
function major()
{
    echo -n $MAJOR
}

# output git version
function output_git()
{
    T=$(git describe --always --dirty)
    echo -n ${T%%"\n"*}
}

# output short version string
function output_short()
{
    echo -n $(major).$(minor).$(micro)
}

# output long version string
function output_long()
{
    echo -n $(major).$(minor).$(micro)-$(output_git)
}

# parse cmdline arguments
TEMP=`getopt -o lsMmug --long long,short,major,minor,micro,git -n 'version.sh' -- "$@"`
if [ $? != 0 ] ; then echo "Terminating..." >&2 ; exit 1 ; fi

# Note the quotes around `$TEMP': they are essential!
eval set -- "$TEMP"

while true ; do
    case "$1" in
        # long
        -l|--long) output_long ; shift 1 ;;

        # short
        -s|--short) output_short ; shift 1 ;;

        # git
        -g|--git) output_git ; shift 1 ;;

        # major
        -M|--major) major ; shift 1 ;;

        # minor
        -m|--minor) minor ; shift 1 ;;

        # micro
        -u|--micro) micro ; shift 1 ;;

        # ?!
        --) shift ; break ;;
        *) echo "Argument parsing error!" ; exit 1 ;;
    esac
done
