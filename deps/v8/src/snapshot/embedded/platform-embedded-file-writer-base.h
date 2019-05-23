// Copyright 2019 the V8 project authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef V8_SNAPSHOT_EMBEDDED_PLATFORM_EMBEDDED_FILE_WRITER_BASE_H_
#define V8_SNAPSHOT_EMBEDDED_PLATFORM_EMBEDDED_FILE_WRITER_BASE_H_

#include <cinttypes>
#include <cstdio>  // For FILE.
#include <memory>

namespace v8 {
namespace internal {

enum DataDirective {
  kByte,
  kLong,
  kQuad,
  kOcta,
};

enum class EmbeddedTargetOs {
  kAIX,
  kChromeOS,
  kFuchsia,
  kMac,
  kWin,
  kGeneric,  // Everything not covered above falls in here.
};

enum class EmbeddedTargetArch {
  kArm,
  kArm64,
  kIA32,
  kX64,
  kGeneric,  // Everything not covered above falls in here.
};

// The platform-dependent logic for emitting assembly code for the generated
// embedded.S file.
class PlatformEmbeddedFileWriterBase {
 public:
  virtual ~PlatformEmbeddedFileWriterBase() = default;

  void SetFile(FILE* fp) { fp_ = fp; }
  FILE* fp() const { return fp_; }

  virtual void SectionText() = 0;
  virtual void SectionData() = 0;
  virtual void SectionRoData() = 0;

  virtual void AlignToCodeAlignment() = 0;
  virtual void AlignToDataAlignment() = 0;

  virtual void DeclareUint32(const char* name, uint32_t value) = 0;
  virtual void DeclarePointerToSymbol(const char* name, const char* target) = 0;

  virtual void DeclareLabel(const char* name) = 0;

  virtual void SourceInfo(int fileid, const char* filename, int line) = 0;
  virtual void DeclareFunctionBegin(const char* name) = 0;
  virtual void DeclareFunctionEnd(const char* name) = 0;

  // Returns the number of printed characters.
  virtual int HexLiteral(uint64_t value) = 0;

  virtual void Comment(const char* string) = 0;
  virtual void Newline() { fprintf(fp_, "\n"); }

  virtual void FilePrologue() = 0;
  virtual void DeclareExternalFilename(int fileid, const char* filename) = 0;
  virtual void FileEpilogue() = 0;

  virtual int IndentedDataDirective(DataDirective directive) = 0;

 protected:
  FILE* fp_ = nullptr;
};

// The factory function. Returns the appropriate platform-specific instance.
std::unique_ptr<PlatformEmbeddedFileWriterBase> NewPlatformEmbeddedFileWriter(
    const char* target_arch, const char* target_os);

}  // namespace internal
}  // namespace v8

#endif  // V8_SNAPSHOT_EMBEDDED_PLATFORM_EMBEDDED_FILE_WRITER_BASE_H_
