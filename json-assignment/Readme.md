# Embedded JSON Serializer Assignment

## Overview
This project implements a small embedded-friendly C library that serializes
structured meter data into a fixed JSON format for smart-meter / w-M-Bus systems.

## Platform & Language
- Language: C
- Platform: PC (portable to STM32 / ESP32)
- Reason: deterministic memory usage, no dependencies.

## How to build
gcc src/json_serializer.c examples/demo.c -Iinclude -o demo

## How to run
./demo

## Public API
serialize_to_json()

## Design
- No dynamic memory allocation
- No external JSON libraries
- Safe snprintf-based serialization
- Transport agnostic

## Possible Extensions
- Streaming output
- Unit tests
- Support for multiple devices
