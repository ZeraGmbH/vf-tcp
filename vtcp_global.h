#ifndef VEIN_TCP_GLOBAL_H
#define VEIN_TCP_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(VEINTCP_LIBRARY)
#  define VEIN_TCPSHARED_EXPORT Q_DECL_EXPORT
#else
#  define VEIN_TCPSHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // VEIN_TCP_GLOBAL_H
