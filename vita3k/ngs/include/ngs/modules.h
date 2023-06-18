// Vita3K emulator project
// Copyright (C) 2023 Vita3K team
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License along
// with this program; if not, write to the Free Software Foundation, Inc.,
// 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.

#pragma once

#include <ngs/system.h>
#include <ngs/types.h>

namespace ngs {
struct Atrac9Module : public Module {
private:
    std::unique_ptr<Atrac9DecoderState> decoder;
    uint32_t last_config;
    std::vector<uint8_t> temp_buffer;

    static SwrContext *swr_mono_to_stereo;
    static SwrContext *swr_stereo;

    // return false if data could not be decoded (error or no more data available)
    bool decode_more_data(KernelState &kern, const MemState &mem, const SceUID thread_id, ModuleData &data, const SceNgsAT9Params *params, SceNgsAT9States *state, std::unique_lock<std::recursive_mutex> &scheduler_lock, std::unique_lock<std::mutex> &voice_lock);

public:
    explicit Atrac9Module();

    bool process(KernelState &kern, const MemState &mem, const SceUID thread_id, ModuleData &data, std::unique_lock<std::recursive_mutex> &scheduler_lock, std::unique_lock<std::mutex> &voice_lock) override;
    uint32_t module_id() const override { return 0x5CAA; }
    uint32_t get_buffer_parameter_size() const override;
    void on_state_change(ModuleData &v, const VoiceState previous) override;
    void on_param_change(const MemState &mem, ModuleData &data) override;
};

void atrac9_get_buffer_parameter(uint32_t start_sample, uint32_t num_samples, uint32_t info, SceNgsAT9SkipBufferInfo &parameter);

struct PlayerModule : public ngs::Module {
private:
    std::unique_ptr<PCMDecoderState> decoder;

public:
    explicit PlayerModule();
    bool process(KernelState &kern, const MemState &mem, const SceUID thread_id, ModuleData &data, std::unique_lock<std::recursive_mutex> &scheduler_lock, std::unique_lock<std::mutex> &voice_lock) override;
    uint32_t module_id() const override { return 0x5CE6; }
    uint32_t get_buffer_parameter_size() const override;
    void on_state_change(ModuleData &v, const VoiceState previous) override;
    void on_param_change(const MemState &mem, ModuleData &data) override;
};

struct EqualizerModule : public Module {
public:
    explicit EqualizerModule();

    bool process(KernelState &kern, const MemState &mem, const SceUID thread_id, ModuleData &data, std::unique_lock<std::recursive_mutex> &scheduler_lock, std::unique_lock<std::mutex> &voice_lock) override;
    uint32_t module_id() const override { return 0x5CEC; }
    uint32_t get_buffer_parameter_size() const override;
};

struct MasterModule : public Module {
public:
    explicit MasterModule();
    bool process(KernelState &kern, const MemState &mem, const SceUID thread_id, ModuleData &data, std::unique_lock<std::recursive_mutex> &scheduler_lock, std::unique_lock<std::mutex> &voice_lock) override;
    uint32_t get_buffer_parameter_size() const override {
        return 0;
    }
};

struct NullModule : public Module {
public:
    explicit NullModule();

    bool process(KernelState &kern, const MemState &mem, const SceUID thread_id, ModuleData &data, std::unique_lock<std::recursive_mutex> &scheduler_lock, std::unique_lock<std::mutex> &voice_lock) override;
    uint32_t get_buffer_parameter_size() const override {
        return 0;
    }
};

struct PassthroughModule : public ngs::Module {
    PassthroughModule();

    bool process(KernelState &kern, const MemState &mem, const SceUID thread_id, ModuleData &data, std::unique_lock<std::recursive_mutex> &scheduler_lock, std::unique_lock<std::mutex> &voice_lock) override;
    uint32_t get_buffer_parameter_size() const override;
};

} // namespace ngs
