vcpkg_from_github(
    OUT_SOURCE_PATH SOURCE_PATH
    REPO LibVNC/libvncserver
    REF "LibVNCServer-${VERSION}"
    SHA512 8a0a58e355154cbe1e5807864dc874b063c5e5f5e20d64ae541d49d51b654a35ca9c625adc5c303b6570fa79c19f82d87a37612f9801b453ccf862bd77c06fcc
    HEAD_REF master
)

vcpkg_check_features(OUT_FEATURE_OPTIONS FEATURE_OPTIONS
    FEATURES
        websockets    WITH_WEBSOCKETS
        openssl    WITH_OPENSSL
    INVERTED_FEATURES
        websockets    WITH_WEBSOCKETS
        openssl    WITH_OPENSSL
)

set(VCPKG_LIBRARY_LINKAGE "dynamic")

vcpkg_cmake_configure(
    SOURCE_PATH "${SOURCE_PATH}"
    OPTIONS
        -DWITH_EXAMPLES=OFF
        -DWITH_TESTS=OFF
        ${FEATURE_OPTIONS}
    MAYBE_UNUSED_VARIABLES
        WITH_OPENSSL
        WITH_WEBSOCKETS
)

vcpkg_cmake_install()
vcpkg_copy_pdbs()
vcpkg_fixup_pkgconfig()

vcpkg_cmake_config_fixup(CONFIG_PATH lib/cmake/LibVNCServer)

file(REMOVE_RECURSE "${CURRENT_PACKAGES_DIR}/debug/include")
file(REMOVE_RECURSE "${CURRENT_PACKAGES_DIR}/debug/share")
file(REMOVE_RECURSE "${CURRENT_PACKAGES_DIR}/share/doc")
file(REMOVE_RECURSE "${CURRENT_PACKAGES_DIR}/share/man")

file(INSTALL "${CURRENT_PORT_DIR}/usage" DESTINATION "${CURRENT_PACKAGES_DIR}/share/${PORT}")
file(INSTALL "${SOURCE_PATH}/COPYING" DESTINATION "${CURRENT_PACKAGES_DIR}/share/${PORT}" RENAME copyright)
