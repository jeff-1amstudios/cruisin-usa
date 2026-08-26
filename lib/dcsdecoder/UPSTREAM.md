# DCSDecoder

This directory contains the native decoder subset from Michael J. Roberts'
DCSExplorer, licensed under the BSD 3-Clause license in `LICENSE`.

The imported files are `DCSDecoder.{h,cpp}`, `DCSDecoderNative.{h,cpp}`, and
`PlatformSpecific.h`.  They provide only the ROM catalog/track interpreter and
native DCS-to-PCM decoder used by this port at startup.  The explorer UI,
encoder, audio output, ADSP emulator, and other utilities are not included.

The import is based on the local DCSExplorer source used to add Cruis'n USA's
catalog at U2 offset `0x24000` and identify its ROM signature.
