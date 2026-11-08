#pragma once

#include <QString>

namespace quadm {

// The application version, taken from project(QuadM VERSION ...) in the top-level CMakeLists.txt.
// It is the single source of truth: installers, the About box and release tags all follow it.
QString versionString();

} // namespace quadm
