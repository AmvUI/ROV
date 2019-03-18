#!/bin/sh

if [ -n "$DESTDIR" ] ; then
    case $DESTDIR in
        /*) # ok
            ;;
        *)
            /bin/echo "DESTDIR argument must be absolute... "
            /bin/echo "otherwise python's distutils will bork things."
            exit 1
    esac
    DESTDIR_ARG="--root=$DESTDIR"
fi

echo_and_run() { echo "+ $@" ; "$@" ; }

echo_and_run cd "/home/mfikih15/Documents/ROV/src/rosserial/rosserial_arduino"

# ensure that Python install destination exists
echo_and_run mkdir -p "$DESTDIR/home/mfikih15/Documents/ROV/install/lib/python2.7/dist-packages"

# Note that PYTHONPATH is pulled from the environment to support installing
# into one location when some dependencies were installed in another
# location, #123.
echo_and_run /usr/bin/env \
    PYTHONPATH="/home/mfikih15/Documents/ROV/install/lib/python2.7/dist-packages:/home/mfikih15/Documents/ROV/build/lib/python2.7/dist-packages:$PYTHONPATH" \
    CATKIN_BINARY_DIR="/home/mfikih15/Documents/ROV/build" \
    "/usr/bin/python" \
    "/home/mfikih15/Documents/ROV/src/rosserial/rosserial_arduino/setup.py" \
    build --build-base "/home/mfikih15/Documents/ROV/build/rosserial/rosserial_arduino" \
    install \
    $DESTDIR_ARG \
    --install-layout=deb --prefix="/home/mfikih15/Documents/ROV/install" --install-scripts="/home/mfikih15/Documents/ROV/install/bin"