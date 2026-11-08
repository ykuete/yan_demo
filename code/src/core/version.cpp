#include "core/version.h"

namespace quadm { 

QString versionString()
{
    return QStringLiteral(QUADM_VERSION);
}    
} // namespace quadm