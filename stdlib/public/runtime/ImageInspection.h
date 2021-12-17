//===--- ImageInspection.h - Image inspection routines ----------*- C++ -*-===//
//
// This source file is part of the Swift.org open source project
//
// Copyright (c) 2014 - 2017 Apple Inc. and the Swift project authors
// Licensed under Apache License v2.0 with Runtime Library Exception
//
// See https://swift.org/LICENSE.txt for license information
// See https://swift.org/CONTRIBUTORS.txt for the list of Swift project authors
//
//===----------------------------------------------------------------------===//
///
/// \file
///
/// This file includes routines that extract metadata from executable and
/// dynamic library image files generated by the Swift compiler. The concrete
/// implementations vary greatly by platform.
///
//===----------------------------------------------------------------------===//

#ifndef SWIFT_RUNTIME_IMAGEINSPECTION_H
#define SWIFT_RUNTIME_IMAGEINSPECTION_H

#include <cstdint>
#include <cstddef>
#if defined(__cplusplus)
#include <memory>
#endif

namespace swift {

/// This is a platform independent version of Dl_info from dlfcn.h
#if defined(__cplusplus)

template <typename T>
struct null_deleter {
  void operator()(T *) const {}
  void operator()(typename std::remove_cv<T>::type *value) const {}
};

template <typename T>
struct free_deleter {
  void operator()(T *value) const {
    free(const_cast<typename std::remove_cv<T>::type *>(value));
  }
  void operator()(typename std::remove_cv<T>::type *value) const {
    free(value);
  }
};

struct SymbolInfo {
  const char *fileName;
  void *baseAddress;
#if defined(_WIN32)
  std::unique_ptr<const char, free_deleter<const char>> symbolName;
#else
  std::unique_ptr<const char, null_deleter<const char>> symbolName;
#endif
  void *symbolAddress;
};
#endif

/// Load the metadata from the image necessary to find protocols by name.
void initializeProtocolLookup();

/// Load the metadata from the image necessary to find a type's
/// protocol conformance.
void initializeProtocolConformanceLookup();

/// Load the metadata from the image necessary to find a type by name.
void initializeTypeMetadataRecordLookup();

/// Load the metadata from the image necessary to perform dynamic replacements.
void initializeDynamicReplacementLookup();

// Callbacks to register metadata from an image to the runtime.
void addImageProtocolsBlockCallback(const void *baseAddress,
                                    const void *start, uintptr_t size);
void addImageProtocolsBlockCallbackUnsafe(const void *baseAddress,
                                          const void *start, uintptr_t size);
void addImageProtocolConformanceBlockCallback(const void *baseAddress,
                                              const void *start,
                                              uintptr_t size);
void addImageProtocolConformanceBlockCallbackUnsafe(const void *baseAddress,
                                                    const void *start,
                                                    uintptr_t size);
void addImageTypeMetadataRecordBlockCallback(const void *baseAddress,
                                             const void *start,
                                             uintptr_t size);
void addImageTypeMetadataRecordBlockCallbackUnsafe(const void *baseAddress,
                                                   const void *start,
                                                   uintptr_t size);
void addImageDynamicReplacementBlockCallback(const void *baseAddress,
                                             const void *start, uintptr_t size,
                                             const void *start2,
                                             uintptr_t size2);

int lookupSymbol(const void *address, SymbolInfo *info);

} // end namespace swift

#endif
