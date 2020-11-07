#!/bin/bash
set -eo pipefail

# Should be in a folder called 'lovr-fork'
pwd | grep 'lovr-fork$' >/dev/null || echo 'Wrong dir' >/dev/stderr
pwd | grep 'lovr-fork$' >/dev/null || exit 1

# Clean build
rm -rf build

echo ===
echo Desktop build
echo ===
mkdir -p build/desktop
pushd build/desktop >/dev/null
cmake ../.. \
    -DLOVR_ENABLE_MOUSE=ON \
    -DLOVR_ENABLE_HEADSET=OFF \
    -DLOVR_ENABLE_ENET=OFF \
    -DLOVR_ENABLE_XXHASH=ON \
    -DLOVR_SYSTEM_OPENAL=ON \
    -DLOVR_SYSTEM_GLFW=ON \
    -DOpenGL_GL_PREFERENCE=GLVND \
    $@
make -j8
popd >/dev/null

echo ===
echo Mini-web build
echo ===
mkdir -p build/miniweb
pushd build/miniweb >/dev/null
emcmake cmake ../.. \
    -DLOVR_ENABLE_MOUSE=ON \
    -DLOVR_ENABLE_HEADSET=OFF \
    -DLOVR_ENABLE_ENET=OFF \
    -DLOVR_ENABLE_XXHASH=ON \
    -DLOVR_ENABLE_PHYSICS=OFF \
    -DLOVR_ENABLE_THREAD=OFF \
    -DLOVR_ENABLE_JSON=OFF \
    $@
emmake make -j8
popd >/dev/null

echo ===
echo Web build
echo ===
mkdir -p build/web
pushd build/web >/dev/null
emcmake cmake ../.. \
    -DLOVR_ENABLE_MOUSE=ON \
    -DLOVR_ENABLE_HEADSET=OFF \
    -DLOVR_ENABLE_ENET=OFF \
    -DLOVR_ENABLE_XXHASH=ON \
    $@
emmake make -j8
popd >/dev/null

echo OK
