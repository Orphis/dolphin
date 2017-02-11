// Copyright 2008 Dolphin Emulator Project
// Licensed under GPLv2+
// Refer to the license.txt file included.

#include <cstring>

#include "AudioCommon/AOSoundStream.h"
#include "AudioCommon/Mixer.h"
#include "Common/Logging/Log.h"
#include "Common/MsgHandler.h"

#if defined(HAVE_AO) && HAVE_AO

void AOSound::SoundLoop()
{
  Common::SetCurrentThreadName("Audio thread - ao");

  uint_32 numBytesToRender = 256;
  ao.ao_initialize();
  default_driver = ao.ao_default_driver_id();
  format.bits = 16;
  format.channels = 2;
  format.rate = m_mixer->GetSampleRate();
  format.byte_format = AO_FMT_LITTLE;

  device = ao.ao_open_live(default_driver, &format, nullptr /* no options */);
  if (!device)
  {
    PanicAlertT("AudioCommon: Error opening AO device.\n");
    ao.ao_shutdown();
    Stop();
    return;
  }

  buf_size = format.bits / 8 * format.channels * format.rate;

  while (m_run_thread.IsSet())
  {
    m_mixer->Mix(realtimeBuffer, numBytesToRender >> 2);

    {
      std::lock_guard<std::mutex> lk(soundCriticalSection);
      ao.ao_play(device, (char*)realtimeBuffer, numBytesToRender);
    }

    soundSyncEvent.Wait();
  }
}

bool AOSound::Start()
{
  m_run_thread.Set();
  memset(realtimeBuffer, 0, sizeof(realtimeBuffer));

  thread = std::thread(&AOSound::SoundLoop, this);
  return true;
}

void AOSound::Update()
{
  soundSyncEvent.Set();
}

void AOSound::Stop()
{
  m_run_thread.Clear();
  soundSyncEvent.Set();

  {
    std::lock_guard<std::mutex> lk(soundCriticalSection);
    thread.join();

    if (device)
      ao.ao_close(device);

    ao.ao_shutdown();

    device = nullptr;
  }
}

#endif
