#ifndef PROTO_NET_GLOBAL_H
#define PROTO_NET_GLOBAL_H

#include <QtCore/qglobal.h>

/*! \mainpage protobuf networking class-library
 *
 * \section Introduction
 *
 * This protobuf network library provides a standardised general purpose network stack.
 *
 * \section Usage
 *
 * \subsection Examples:
 *
 */

#if defined(XIQNET_LIBRARY)
#  define XIQNET_QTSHARED_EXPORT Q_DECL_EXPORT
#else
#  define XIQNET_QTSHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // PROTO_NET_GLOBAL_H
