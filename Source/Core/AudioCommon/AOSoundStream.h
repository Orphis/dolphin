// Copyright 2008 Dolphin Emulator Project
// Licensed under GPLv2+
// Refer to the license.txt file included.

#pragma once

#include <mutex>
#include <thread>

#include "AudioCommon/SoundStream.h"
#include "Common/DynamicLoader.h"
#include "Common/Event.h"
#include "Common/Thread.h"

#if defined(HAVE_AO) && HAVE_AO
#include <ao/ao.h>
#endif

class AOSound final : public SoundStream
{
#if defined(HAVE_AO) && HAVE_AO
  class DynamicAO : public DynamicLoaderBase {
   public:
    DynamicAO() : DynamicLoaderBase("ao") {}

    DL_IMPORT_FUNC(ao_close);
    DL_IMPORT_FUNC(ao_default_driver_id);
    DL_IMPORT_FUNC(ao_initialize);
    DL_IMPORT_FUNC(ao_open_live);
    DL_IMPORT_FUNC(ao_play);
    DL_IMPORT_FUNC(ao_shutdown);
  };

  DynamicAO ao;
  std::thread thread;
  Common::Flag m_run_thread;
  std::mutex soundCriticalSection;
  Common::Event soundSyncEvent;

  int buf_size;

  ao_device* device;
  ao_sample_format format;
  int default_driver;

  short realtimeBuffer[1024 * 1024];

public:
  bool Start() override;
  void SoundLoop() override;
  void Stop() override;
  void Update() override;

  static bool isValid() { return DynamicAO().IsLoaded(); }
#endif
};
