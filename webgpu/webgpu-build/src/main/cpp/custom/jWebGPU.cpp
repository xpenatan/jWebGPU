#include "jWebGPU.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#ifdef __ANDROID__
// Custom streambuf to redirect std::cout to Logcat
int LogcatStreamBuf::overflow(int c) {
    if (c == EOF) {
        return !EOF;
    }
    buffer += static_cast<char>(c);
    if (c == '\n') {
        sync();
    }
    return c;
}

int LogcatStreamBuf::sync() {
    if (!buffer.empty()) {
        if (buffer.back() == '\n') {
            buffer.pop_back();
        }
        __android_log_print(ANDROID_LOG_INFO, LOG_TAG, "%s", buffer.c_str());
        buffer.clear();
    }
    return 0;
}

// Singleton to manage Logcat redirection
void LogcatRedirector::initialize() {
    static LogcatRedirector instance; // Lazy initialization
    if (!instance.isInitialized) {
        instance.original_buffer = std::cout.rdbuf();
        std::cout.rdbuf(&instance.logcat_buffer);
        instance.isInitialized = true;
    }
}

LogcatRedirector::LogcatRedirector() : original_buffer(nullptr), isInitialized(false) {}

LogcatRedirector::~LogcatRedirector() {
    if (isInitialized) {
        std::cout.rdbuf(original_buffer);
    }
}
#endif // __ANDROID__

// Template instantiations
template void WGPUByteBuffer::putNumeric<float>(int index, float value);
template void WGPUByteBuffer::putNumeric<int16_t>(int index, int16_t value);
template float WGPUByteBuffer::getNumeric<float>(int index);
template int16_t WGPUByteBuffer::getNumeric<int16_t>(int index);

template class WGPUObjectBase<JGPU::WGPUCommandBuffer, WGPUCommandBuffer>;
template class WGPUObjectBase<JGPU::WGPUBuffer, WGPUBuffer>;
template class WGPUObjectBase<JGPU::WGPUQueue, WGPUQueue>;
template class WGPUObjectBase<JGPU::WGPUBindGroupLayout, WGPUBindGroupLayout>;
template class WGPUObjectBase<JGPU::WGPUComputePipeline, WGPUComputePipeline>;
template class WGPUObjectBase<JGPU::WGPUStringView, WGPUStringView>;
template class WGPUObjectBase<JGPU::WGPUChainedStruct, WGPUChainedStruct*>;
template class WGPUObjectBase<JGPU::WGPUBindGroup, WGPUBindGroup>;
template class WGPUObjectBase<JGPU::WGPURenderBundle, WGPURenderBundle>;
template class WGPUObjectBase<JGPU::WGPUShaderModule, WGPUShaderModule>;
template class WGPUObjectBase<JGPU::WGPUPipelineLayout, WGPUPipelineLayout>;
template class WGPUObjectBase<JGPU::WGPURenderPipeline, WGPURenderPipeline>;
template class WGPUObjectBase<JGPU::WGPURenderPassEncoder, WGPURenderPassEncoder>;
template class WGPUObjectBase<JGPU::WGPUComputePassEncoder, WGPUComputePassEncoder>;
template class WGPUObjectBase<JGPU::WGPUQuerySet, WGPUQuerySet>;
template class WGPUObjectBase<JGPU::WGPUTextureView, WGPUTextureView>;
template class WGPUObjectBase<JGPU::WGPUTexture, WGPUTexture>;
template class WGPUObjectBase<JGPU::WGPUCommandEncoder, WGPUCommandEncoder>;
template class WGPUObjectBase<JGPU::WGPUDevice, WGPUDevice>;
template class WGPUObjectBase<JGPU::WGPUAdapter, WGPUAdapter>;
template class WGPUObjectBase<JGPU::WGPUSurface, WGPUSurface>;
template class WGPUObjectBase<JGPU::WGPUInstance, WGPUInstance>;

// WGPUVectorFutureWaitInfo
WGPUVectorFutureWaitInfo* WGPUVectorFutureWaitInfo::Obtain() {
    static WGPUVectorFutureWaitInfo obj;
    obj = WGPUVectorFutureWaitInfo();
    return &obj;
}

int WGPUVectorFutureWaitInfo::size() { return vector.size(); }

void WGPUVectorFutureWaitInfo::push_back(const JGPU::WGPUFutureWaitInfo& entry) { vector.push_back(entry); }

const JGPU::WGPUFutureWaitInfo* WGPUVectorFutureWaitInfo::data() { return vector.data(); }


// WGPUVectorBindGroupEntry
WGPUVectorBindGroupEntry* WGPUVectorBindGroupEntry::Obtain() {
    static WGPUVectorBindGroupEntry obj;
    obj = WGPUVectorBindGroupEntry();
    return &obj;
}

int WGPUVectorBindGroupEntry::size() { return vector.size(); }

void WGPUVectorBindGroupEntry::push_back(const JGPU::WGPUBindGroupEntry& entry) { vector.push_back(entry); }

const JGPU::WGPUBindGroupEntry* WGPUVectorBindGroupEntry::data() { return vector.data(); }

// WGPUVectorColorTargetState
WGPUVectorColorTargetState* WGPUVectorColorTargetState::Obtain() {
    static WGPUVectorColorTargetState obj;
    obj = WGPUVectorColorTargetState();
    return &obj;
}

int WGPUVectorColorTargetState::size() { return vector.size(); }

void WGPUVectorColorTargetState::push_back(const JGPU::WGPUColorTargetState& attachment) { vector.push_back(attachment); }

const JGPU::WGPUColorTargetState* WGPUVectorColorTargetState::data() { return vector.data(); }

// WGPUVectorFeatureName
WGPUVectorFeatureName* WGPUVectorFeatureName::Obtain() {
    static WGPUVectorFeatureName obj;
    obj = WGPUVectorFeatureName();
    return &obj;
}

int WGPUVectorFeatureName::size() { return vector.size(); }

void WGPUVectorFeatureName::push_back(const WGPUFeatureName& attachment) { vector.push_back(attachment); }

const WGPUFeatureName* WGPUVectorFeatureName::data() { return vector.data(); }

// WGPUVectorConstantEntry
WGPUVectorConstantEntry* WGPUVectorConstantEntry::Obtain() {
    static WGPUVectorConstantEntry obj;
    obj = WGPUVectorConstantEntry();
    return &obj;
}

int WGPUVectorConstantEntry::size() { return vector.size(); }

void WGPUVectorConstantEntry::push_back(const JGPU::WGPUConstantEntry& attachment) { vector.push_back(attachment); }

const JGPU::WGPUConstantEntry* WGPUVectorConstantEntry::data() { return vector.data(); }

// WGPUVectorVertexBufferLayout
WGPUVectorVertexBufferLayout* WGPUVectorVertexBufferLayout::Obtain() {
    static WGPUVectorVertexBufferLayout obj;
    obj = WGPUVectorVertexBufferLayout();
    return &obj;
}

int WGPUVectorVertexBufferLayout::size() { return vector.size(); }

void WGPUVectorVertexBufferLayout::push_back(const JGPU::WGPUVertexBufferLayout& attachment) { vector.push_back(attachment); }

const JGPU::WGPUVertexBufferLayout* WGPUVectorVertexBufferLayout::data() { return vector.data(); }

// WGPUVectorTextureFormat
WGPUVectorTextureFormat* WGPUVectorTextureFormat::Obtain() {
    static WGPUVectorTextureFormat obj;
    obj = WGPUVectorTextureFormat();
    return &obj;
}

int WGPUVectorTextureFormat::size() { return vector.size(); }

WGPUTextureFormat WGPUVectorTextureFormat::get(int index) { return vector[index]; }

void WGPUVectorTextureFormat::push_back(const WGPUTextureFormat& attachment) { vector.push_back(attachment); }

const WGPUTextureFormat* WGPUVectorTextureFormat::data() { return vector.data(); }

// WGPUVectorRenderBundle
WGPUVectorRenderBundle* WGPUVectorRenderBundle::Obtain() {
    static WGPUVectorRenderBundle obj;
    obj = WGPUVectorRenderBundle();
    return &obj;
}

int WGPUVectorRenderBundle::size() { return vector.size(); }

void WGPUVectorRenderBundle::push_back(const JGPU::WGPURenderBundle& attachment) { vector.push_back(attachment); }

const JGPU::WGPURenderBundle* WGPUVectorRenderBundle::data() { return vector.data(); }

// WGPUVectorRenderPassColorAttachment
WGPUVectorRenderPassColorAttachment* WGPUVectorRenderPassColorAttachment::Obtain() {
    static WGPUVectorRenderPassColorAttachment obj;
    obj = WGPUVectorRenderPassColorAttachment();
    return &obj;
}

int WGPUVectorRenderPassColorAttachment::size() { return vector.size(); }

void WGPUVectorRenderPassColorAttachment::clear() { return vector.clear(); }

void WGPUVectorRenderPassColorAttachment::push_back(const JGPU::WGPURenderPassColorAttachment& attachment) { vector.push_back(attachment); }

const JGPU::WGPURenderPassColorAttachment* WGPUVectorRenderPassColorAttachment::data() { return vector.data(); }

// WGPUVectorVertexAttribute
WGPUVectorVertexAttribute* WGPUVectorVertexAttribute::Obtain() {
    static WGPUVectorVertexAttribute obj;
    obj = WGPUVectorVertexAttribute();
    return &obj;
}

int WGPUVectorVertexAttribute::size() { return vector.size(); }

void WGPUVectorVertexAttribute::push_back(const JGPU::WGPUVertexAttribute& attribute) { vector.push_back(attribute); }

const JGPU::WGPUVertexAttribute* WGPUVectorVertexAttribute::data() { return vector.data(); }

// WGPUVectorBindGroupLayout
WGPUVectorBindGroupLayout* WGPUVectorBindGroupLayout::Obtain() {
    static WGPUVectorBindGroupLayout obj;
    obj = WGPUVectorBindGroupLayout();
    return &obj;
}

int WGPUVectorBindGroupLayout::size() { return vector.size(); }

void WGPUVectorBindGroupLayout::push_back(const JGPU::WGPUBindGroupLayout& groupLayout) { vector.push_back(groupLayout); }

const JGPU::WGPUBindGroupLayout* WGPUVectorBindGroupLayout::data() { return vector.data(); }

// WGPUVectorBindGroupLayoutEntry
WGPUVectorBindGroupLayoutEntry* WGPUVectorBindGroupLayoutEntry::Obtain() {
    static WGPUVectorBindGroupLayoutEntry obj;
    obj = WGPUVectorBindGroupLayoutEntry();
    return &obj;
}

int WGPUVectorBindGroupLayoutEntry::size() { return vector.size(); }

void WGPUVectorBindGroupLayoutEntry::push_back(const JGPU::WGPUBindGroupLayoutEntry& entry) { vector.push_back(entry); }

const JGPU::WGPUBindGroupLayoutEntry* WGPUVectorBindGroupLayoutEntry::data() { return vector.data(); }

// WGPUVectorInt
WGPUVectorInt* WGPUVectorInt::Obtain() {
    static WGPUVectorInt obj;
    obj = WGPUVectorInt();
    return &obj;
}

int WGPUVectorInt::size() { return vector.size(); }

void WGPUVectorInt::push_back(int attachment) { vector.push_back(attachment); }

int WGPUVectorInt::get(int index) { return vector[index]; }

const uint32_t* WGPUVectorInt::data() { return vector.data(); }

// WGPUByteBuffer
bool WGPUByteBuffer::isLittleEndianHost() {
    uint16_t test = 0x0001;
    return reinterpret_cast<const uint8_t*>(&test)[0] == 0x01;
}

uint16_t WGPUByteBuffer::swapBytes(uint16_t value) {
    return (value >> 8) | (value << 8);
}

uint32_t WGPUByteBuffer::swapBytes(uint32_t value) {
    return ((value >> 24) & 0x000000FF) |
           ((value >> 8) & 0x0000FF00) |
           ((value << 8) & 0x00FF0000) |
           ((value << 24) & 0xFF000000);
}

uint64_t WGPUByteBuffer::swapBytes(uint64_t value) {
    return ((value >> 56) & 0x00000000000000FFULL) |
           ((value >> 40) & 0x000000000000FF00ULL) |
           ((value >> 24) & 0x0000000000FF0000ULL) |
           ((value >> 8) & 0x00000000FF000000ULL) |
           ((value << 8) & 0x000000FF00000000ULL) |
           ((value << 24) & 0x0000FF0000000000ULL) |
           ((value << 40) & 0x00FF000000000000ULL) |
           ((value << 56) & 0xFF00000000000000ULL);
}

WGPUByteBuffer* WGPUByteBuffer::Obtain(int capacity) {
    static WGPUByteBuffer obj(capacity);
    obj = WGPUByteBuffer(capacity);
    return &obj;
}

int WGPUByteBuffer::getCapacity() { return buffer.size(); }

void WGPUByteBuffer::push_back(char value) { buffer.push_back(value); }

const uint8_t* WGPUByteBuffer::data() { return buffer.data(); }

void WGPUByteBuffer::order(WGPUByteOrder order) {
    byteOrder = order;
    needsSwap = (order == LittleEndian) != isLittleEndianHost();
}

void WGPUByteBuffer::put(int index, char value) {
    if (index >= _limit) {
        throw std::out_of_range("Buffer overflow");
    }
    buffer[index] = value;
}

char WGPUByteBuffer::get(int index) {
    if (index < 0 || index >= _limit) {
        throw std::out_of_range("ByteBuffer index out of bounds");
    }
    return buffer[index];
}

void WGPUByteBuffer::put(char value) {
    if (_position >= _limit) {
        throw std::out_of_range("Buffer overflow");
    }
    buffer[_position++] = value;
}

char WGPUByteBuffer::get() {
    auto value = get(_position);
    _position++;
    return value;
}

void WGPUByteBuffer::put(const uint8_t* values, int index, int size) {
    if (index < 0 || size < 0 || index + size > _limit) {
        throw std::out_of_range("Invalid index or size for ByteBuffer");
    }
    if (size == 0) return;
    std::memcpy(&buffer[index], values, size);
}

void WGPUByteBuffer::putInt(int index, int value) {
    if (index < 0 || index >= getLimit()) {
        throw std::out_of_range("ByteBuffer overflow");
    }
    putNumeric(index, value);
}

int WGPUByteBuffer::getInt(int index) {
    if (index < 0 || index >= getLimit()) {
        throw std::out_of_range("ByteBuffer index out of bounds");
    }
    return getNumeric<int>(index);
}

void WGPUByteBuffer::putFloat(int index, float value) {
    if (index < 0 || index >= getLimit()) {
        throw std::out_of_range("ByteBuffer overflow");
    }
    putNumeric(index, value);
}

float WGPUByteBuffer::getFloat(int index) {
    if (index < 0 || index >= getLimit()) {
        throw std::out_of_range("ByteBuffer index out of bounds");
    }
    return getNumeric<float>(index);
}

int WGPUByteBuffer::remaining() const {
    return _limit - _position;
}

void WGPUByteBuffer::position(int newPosition) {
    if (newPosition > _limit) {
        throw std::out_of_range("Invalid position");
    }
    _position = newPosition;
}

int WGPUByteBuffer::getPosition() {
    return _position;
}

void WGPUByteBuffer::limit(int newLimit) {
    if (newLimit > buffer.size()) {
        throw std::out_of_range("Invalid limit");
    }
    _limit = newLimit;
}

size_t WGPUByteBuffer::getLimit() const {
    return _limit;
}

void WGPUByteBuffer::clear() {
    _position = 0;
    _limit = buffer.size();
}

void WGPUByteBuffer::flip() {
    _limit = _position;
    _position = 0;
}

template<typename T>
void WGPUByteBuffer::putNumeric(int index, T value) {
    if (index + sizeof(T) > _limit) {
        throw std::out_of_range("Buffer overflow");
    }
    if constexpr (sizeof(T) == 2) {
        uint16_t intValue;
        std::memcpy(&intValue, &value, sizeof(T));
        if (needsSwap) {
            intValue = swapBytes(intValue);
        }
        std::memcpy(&buffer[index], &intValue, sizeof(T));
    } else if constexpr (sizeof(T) == 4) {
        uint32_t intValue;
        std::memcpy(&intValue, &value, sizeof(T));
        if (needsSwap) {
            intValue = swapBytes(intValue);
        }
        std::memcpy(&buffer[index], &intValue, sizeof(T));
    } else if constexpr (sizeof(T) == 8) {
        uint64_t intValue;
        std::memcpy(&intValue, &value, sizeof(T));
        if (needsSwap) {
            intValue = swapBytes(intValue);
        }
        std::memcpy(&buffer[index], &intValue, sizeof(T));
    } else {
        std::memcpy(&buffer[index], &value, sizeof(T));
    }
}

template<typename T>
T WGPUByteBuffer::getNumeric(int index) {
    if (index + sizeof(T) > _limit) {
        throw std::out_of_range("Buffer underflow");
    }
    if constexpr (sizeof(T) == 2) {
        uint16_t intValue;
        std::memcpy(&intValue, &buffer[index], sizeof(T));
        if (needsSwap) {
            intValue = swapBytes(intValue);
        }
        T value;
        std::memcpy(&value, &intValue, sizeof(T));
        return value;
    } else if constexpr (sizeof(T) == 4) {
        uint32_t intValue;
        std::memcpy(&intValue, &buffer[index], sizeof(T));
        if (needsSwap) {
            intValue = swapBytes(intValue);
        }
        T value;
        std::memcpy(&value, &intValue, sizeof(T));
        return value;
    } else if constexpr (sizeof(T) == 8) {
        uint64_t intValue;
        std::memcpy(&intValue, &buffer[index], sizeof(T));
        if (needsSwap) {
            intValue = swapBytes(intValue);
        }
        T value;
        std::memcpy(&value, &intValue, sizeof(T));
        return value;
    } else {
        T value;
        std::memcpy(&value, &buffer[index], sizeof(T));
        return value;
    }
}

WGPUFloatBuffer& WGPUByteBuffer::asFloatBuffer() {
    _position = 0;
    floatBuffer.parent = this;
    return floatBuffer;
}

WGPUIntBuffer& WGPUByteBuffer::asIntBuffer() {
    _position = 0;
    intBuffer.parent = this;
    return intBuffer;
}

WGPULongBuffer& WGPUByteBuffer::asLongBuffer() {
    _position = 0;
    longBuffer.parent = this;
    return longBuffer;
}

WGPUShortBuffer& WGPUByteBuffer::asShortBuffer() {
    _position = 0;
    shortBuffer.parent = this;
    return shortBuffer;
}

// WGPUFloatBuffer
WGPUByteBuffer& WGPUFloatBuffer::getByteBuffer() {
    return *parent;
}

void WGPUFloatBuffer::put(int index, float value) {
    if (index < 0 || index >= getLimit()) {
        throw std::out_of_range("FloatBuffer overflow");
    }
    parent->putNumeric(index * sizeof(float), value);
}

float WGPUFloatBuffer::get(int index) {
    if (index < 0 || index >= getLimit()) {
        throw std::out_of_range("FloatBuffer index out of bounds");
    }
    return parent->getNumeric<float>(index * sizeof(float));
}

void WGPUFloatBuffer::put(float value) {
    int floatPosition = getPosition();
    put(floatPosition, value);
    floatPosition++;
    position(floatPosition);
}

float WGPUFloatBuffer::get() {
    int floatPosition = getPosition();
    auto value = get(floatPosition);
    floatPosition++;
    position(floatPosition);
    return value;
}

void WGPUFloatBuffer::put(const float* values, int offset, int size) {
    if (offset < 0 || size < 0 || offset + size > parent->getCapacity() / sizeof(float)) {
        throw std::out_of_range("Invalid offset or size for FloatBuffer");
    }
    if (size == 0) return;

    int floatPosition = getPosition();
    if (floatPosition + size > getLimit()) {
        throw std::out_of_range("FloatBuffer overflow");
    }

    size_t byteOffset = floatPosition * sizeof(float);
    size_t byteSize = size * sizeof(float);
    if (byteOffset + byteSize > parent->getLimit()) {
        throw std::out_of_range("Buffer overflow");
    }

    if (!parent->needsSwap) {
        std::memcpy(&parent->buffer[byteOffset], values + offset, byteSize);
    } else {
        std::vector<float> temp(values + offset, values + offset + size);
        for (int i = 0; i < size; ++i) {
            uint32_t* ptr = reinterpret_cast<uint32_t*>(&temp[i]);
            *ptr = parent->swapBytes(*ptr);
        }
        std::memcpy(&parent->buffer[byteOffset], temp.data(), byteSize);
    }

    position(floatPosition + size);
}

int WGPUFloatBuffer::remaining() const {
    return getLimit() - getPosition();
}

int WGPUFloatBuffer::getCapacity() { return parent->getCapacity() / sizeof(float); }

void WGPUFloatBuffer::position(int newPosition) {
    if (newPosition < 0 || newPosition > getLimit()) {
        throw std::out_of_range("Invalid position for FloatBuffer");
    }
    parent->position(newPosition * sizeof(float));
}

int WGPUFloatBuffer::getPosition() const {
    return parent->getPosition() / sizeof(float);
}

void WGPUFloatBuffer::clear() {
    parent->clear();
}

void WGPUFloatBuffer::limit(int newLimit) {
    parent->limit(newLimit * sizeof(float));
}

int WGPUFloatBuffer::getLimit() const {
    return parent->getLimit() / sizeof(float);
}

void WGPUFloatBuffer::flip() {
    int pos = getPosition();
    limit(pos);
    position(0);
}

// WGPUIntBuffer
WGPUByteBuffer& WGPUIntBuffer::getByteBuffer() {
    return *parent;
}

void WGPUIntBuffer::put(int index, int value) {
    if (index < 0 || index >= getLimit()) {
        throw std::out_of_range("IntBuffer overflow");
    }
    parent->putNumeric(index * sizeof(int), value);
}

int WGPUIntBuffer::get(int index) {
    if (index < 0 || index >= getLimit()) {
        throw std::out_of_range("IntBuffer index out of bounds");
    }
    return parent->getNumeric<int>(index * sizeof(int));
}

void WGPUIntBuffer::put(int value) {
    int intPosition = getPosition();
    put(intPosition, value);
    intPosition++;
    position(intPosition);
}

int WGPUIntBuffer::get() {
    int intPosition = getPosition();
    auto value = get(intPosition);
    intPosition++;
    position(intPosition);
    return value;
}

void WGPUIntBuffer::put(const int* values, int offset, int size) {
    if (offset < 0 || size < 0 || offset + size > getLimit()) {
        throw std::out_of_range("Invalid offset or size for FloatBuffer");
    }
    if (size == 0) return;

    size_t byteOffset = offset * sizeof(int);
    size_t byteSize = size * sizeof(int);
    if (byteOffset + byteSize > parent->getLimit()) {
        throw std::out_of_range("Buffer overflow");
    }

    bool needsSwap = (parent->byteOrder == LittleEndian) != parent->isLittleEndianHost();
    if (!needsSwap) {
        // Direct copy if no byte-swapping is needed
        std::memcpy(&parent->buffer[byteOffset], values, byteSize);
    }
    else {
        // Copy and swap bytes for each float
        std::vector<int> temp(values, values + size);
        for (int i = 0; i < size; ++i) {
            uint32_t* ptr = reinterpret_cast<uint32_t*>(&temp[i]);
            *ptr = parent->swapBytes(*ptr);
        }
        std::memcpy(&parent->buffer[byteOffset], temp.data(), byteSize);
    }
}

int WGPUIntBuffer::remaining() const {
    return getLimit() - getPosition();
}

int WGPUIntBuffer::getCapacity() { return parent->getCapacity() / sizeof(int); }

void WGPUIntBuffer::position(int newPosition) {
    if (newPosition < 0 || newPosition > getLimit()) {
        throw std::out_of_range("Invalid position for IntBuffer");
    }
    parent->position(newPosition * sizeof(int));
}

int WGPUIntBuffer::getPosition() const {
    return parent->getPosition() / sizeof(int);
}

void WGPUIntBuffer::clear() {
    parent->clear();
}

void WGPUIntBuffer::limit(int newLimit) {
    parent->limit(newLimit * sizeof(int));
}

int WGPUIntBuffer::getLimit() const {
    return parent->getLimit() / sizeof(int);
}

void WGPUIntBuffer::flip() {
    int pos = getPosition();
    limit(pos);
    position(0);
}

// WGPULongBuffer
WGPUByteBuffer& WGPULongBuffer::getByteBuffer() {
    return *parent;
}

void WGPULongBuffer::put(int index, long long value) {
    if (index < 0 || index >= getLimit()) {
        throw std::out_of_range("IntBuffer overflow");
    }
    parent->putNumeric(index * sizeof(long long), value);
}

long long WGPULongBuffer::get(int index) {
    if (index < 0 || index >= getLimit()) {
        throw std::out_of_range("IntBuffer index out of bounds");
    }
    return parent->getNumeric<long long>(index * sizeof(long long));
}

void WGPULongBuffer::put(long long value) {
    int intPosition = getPosition();
    put(intPosition, value);
    intPosition++;
    position(intPosition);
}

long long WGPULongBuffer::get() {
    int intPosition = getPosition();
    auto value = get(intPosition);
    intPosition++;
    position(intPosition);
    return value;
}

void WGPULongBuffer::put(const long long* values, int offset, int size) {
    if (offset < 0 || size < 0 || offset + size > getLimit()) {
        throw std::out_of_range("Invalid offset or size for FloatBuffer");
    }
    if (size == 0) return;

    size_t byteOffset = offset * sizeof(long long);
    size_t byteSize = size * sizeof(long long);
    if (byteOffset + byteSize > parent->getLimit()) {
        throw std::out_of_range("Buffer overflow");
    }

    bool needsSwap = (parent->byteOrder == LittleEndian) != parent->isLittleEndianHost();
    if (!needsSwap) {
        // Direct copy if no byte-swapping is needed
        std::memcpy(&parent->buffer[byteOffset], values, byteSize);
    }
    else {
        // Copy and swap bytes for each float
        std::vector<long long> temp(values, values + size);
        for (int i = 0; i < size; ++i) {
            uint32_t* ptr = reinterpret_cast<uint32_t*>(&temp[i]);
            *ptr = parent->swapBytes(*ptr);
        }
        std::memcpy(&parent->buffer[byteOffset], temp.data(), byteSize);
    }
}

int WGPULongBuffer::remaining() const {
    return getLimit() - getPosition();
}

int WGPULongBuffer::getCapacity() { return parent->getCapacity() / sizeof(long long); }

void WGPULongBuffer::position(int newPosition) {
    if (newPosition < 0 || newPosition > getLimit()) {
        throw std::out_of_range("Invalid position for IntBuffer");
    }
    parent->position(newPosition * sizeof(long long));
}

int WGPULongBuffer::getPosition() const {
    return parent->getPosition() / sizeof(long long);
}

void WGPULongBuffer::clear() {
    parent->clear();
}

void WGPULongBuffer::limit(int newLimit) {
    parent->limit(newLimit * sizeof(long long));
}

int WGPULongBuffer::getLimit() const {
    return parent->getLimit() / sizeof(long long);
}

void WGPULongBuffer::flip() {
    int pos = getPosition();
    limit(pos);
    position(0);
}

// WGPUShortBuffer
WGPUByteBuffer& WGPUShortBuffer::getByteBuffer() {
    return *parent;
}

void WGPUShortBuffer::put(int index, int16_t value) {
    if (index < 0 || index >= getLimit()) {
        throw std::out_of_range("ShortBuffer overflow");
    }
    parent->putNumeric(index * sizeof(int16_t), value);
}

int16_t WGPUShortBuffer::get(int index) {
    if (index < 0 || index >= getLimit()) {
        throw std::out_of_range("ShortBuffer index out of bounds");
    }
    return parent->getNumeric<int16_t>(index * sizeof(int16_t));
}

void WGPUShortBuffer::put(int16_t value) {
    int shortPosition = getPosition();
    put(shortPosition, value);
    shortPosition++;
    position(shortPosition);
}

int16_t WGPUShortBuffer::get() {
    int shortPosition = getPosition();
    auto value = get(shortPosition);
    shortPosition++;
    position(shortPosition);
    return value;
}

void WGPUShortBuffer::put(const int16_t* values, int offset, int size) {
    if (offset < 0 || size < 0 || offset + size > getLimit()) {
        throw std::out_of_range("Invalid offset or size for ShortBuffer");
    }
    if (size == 0) return;

    size_t byteOffset = offset * sizeof(int16_t);
    size_t byteSize = size * sizeof(int16_t);
    if (byteOffset + byteSize > parent->getLimit()) {
        throw std::out_of_range("Buffer overflow");
    }

    bool needsSwap = (parent->byteOrder == LittleEndian) != parent->isLittleEndianHost();
    if (!needsSwap) {
        // Direct copy if no byte-swapping is needed
        std::memcpy(&parent->buffer[byteOffset], values, byteSize);
    }
    else {
        // Copy and swap bytes for each short
        std::vector<int16_t> temp(values, values + size);
        for (int i = 0; i < size; ++i) {
            temp[i] = parent->swapBytes(static_cast<uint16_t>(temp[i]));
        }
        std::memcpy(&parent->buffer[byteOffset], temp.data(), byteSize);
    }
}

void WGPUShortBuffer::clear() {
    parent->clear();
}

void WGPUShortBuffer::limit(int newLimit) {
    parent->limit(newLimit * sizeof(int16_t));
}

int WGPUShortBuffer::getLimit() const {
    return parent->getLimit() / sizeof(int16_t);
}

int WGPUShortBuffer::getCapacity() { return parent->getCapacity() / sizeof(int16_t); }

void WGPUShortBuffer::position(int newPosition) {
    if (newPosition < 0 || newPosition > getLimit()) {
        throw std::out_of_range("Invalid position for ShortBuffer");
    }
    parent->position(newPosition * sizeof(int16_t));
}

int WGPUShortBuffer::getPosition() const {
    return parent->getPosition() / sizeof(int16_t);
}

int WGPUShortBuffer::remaining() const {
    return getLimit() - getPosition();
}

void WGPUShortBuffer::flip() {
    int pos = getPosition();
    limit(pos);
    position(0);
}

// WGPUAndroidWindow
WGPUAndroidWindow::~WGPUAndroidWindow() {
    #ifdef __ANDROID__
        if(g_window != nullptr) {
            ANativeWindow_release(g_window);
        }
    #endif
}

void WGPUAndroidWindow::SetWindow(void * window) {
    #ifdef __ANDROID__
        g_window = reinterpret_cast<ANativeWindow * >(window);
    #endif
}

void* WGPUAndroidWindow::GetWindow() {
    #ifdef __ANDROID__
        return g_window;
    #else
        return NULL;
    #endif
}

void WGPUAndroidWindow::InitLogcat() {
    #ifdef __ANDROID__
        __android_log_print(ANDROID_LOG_INFO, LOG_TAG, "Initializing Logcat redirection");
        LogcatRedirector::initialize();
    #endif
}

// WGPUStringView
const std::string JGPU::WGPUStringView::GetString() {
    return std::string(Get().data, Get().length);
}

// WGPUChainedStruct
void JGPU::WGPUChainedStruct::SetNext(JGPU::WGPUChainedStruct* value) {
    Get()->next = value != NULL ? value->Get() : NULL;
}

void JGPU::WGPUChainedStruct::SetSType(WGPUSType type) {
    Get()->sType = type;
}

// WGPULimits
JGPU::WGPULimits* JGPU::WGPULimits::Obtain() {
    static JGPU::WGPULimits obj;
    obj = JGPU::WGPULimits();
    return &obj;
}

void JGPU::WGPULimits::SetMaxTextureDimension1D(int value) {
    Get().maxTextureDimension1D = value;
}

int JGPU::WGPULimits::GetMaxTextureDimension1D() {
    return static_cast<int>(Get().maxTextureDimension1D);
}

void JGPU::WGPULimits::SetMaxTextureDimension2D(int value) {
    Get().maxTextureDimension2D = value;
}

int JGPU::WGPULimits::GetMaxTextureDimension2D() {
    return static_cast<int>(Get().maxTextureDimension2D);
}

void JGPU::WGPULimits::SetMaxTextureDimension3D(int value) {
    Get().maxTextureDimension3D = value;
}

int JGPU::WGPULimits::GetMaxTextureDimension3D() {
    return static_cast<int>(Get().maxTextureDimension3D);
}

void JGPU::WGPULimits::SetMaxTextureArrayLayers(int value) {
    Get().maxTextureArrayLayers = value;
}

int JGPU::WGPULimits::GetMaxTextureArrayLayers() {
    return static_cast<int>(Get().maxTextureArrayLayers);
}

void JGPU::WGPULimits::SetMaxBindGroups(int value) {
    Get().maxBindGroups = value;
}

int JGPU::WGPULimits::GetMaxBindGroups() {
    return static_cast<int>(Get().maxBindGroups);
}

void JGPU::WGPULimits::SetMaxBindGroupsPlusVertexBuffers(int value) {
    Get().maxBindGroupsPlusVertexBuffers = value;
}

int JGPU::WGPULimits::GetMaxBindGroupsPlusVertexBuffers() {
    return static_cast<int>(Get().maxBindGroupsPlusVertexBuffers);
}

void JGPU::WGPULimits::SetMaxBindingsPerBindGroup(int value) {
    Get().maxBindingsPerBindGroup = value;
}

int JGPU::WGPULimits::GetMaxBindingsPerBindGroup() {
    return static_cast<int>(Get().maxBindingsPerBindGroup);
}

void JGPU::WGPULimits::SetMaxDynamicUniformBuffersPerPipelineLayout(int value) {
    Get().maxDynamicUniformBuffersPerPipelineLayout = value;
}

int JGPU::WGPULimits::GetMaxDynamicUniformBuffersPerPipelineLayout() {
    return static_cast<int>(Get().maxDynamicUniformBuffersPerPipelineLayout);
}

void JGPU::WGPULimits::SetMaxDynamicStorageBuffersPerPipelineLayout(int value) {
    Get().maxDynamicStorageBuffersPerPipelineLayout = value;
}

int JGPU::WGPULimits::GetMaxDynamicStorageBuffersPerPipelineLayout() {
    return static_cast<int>(Get().maxDynamicStorageBuffersPerPipelineLayout);
}

void JGPU::WGPULimits::SetMaxSampledTexturesPerShaderStage(int value) {
    Get().maxSampledTexturesPerShaderStage = value;
}

int JGPU::WGPULimits::GetMaxSampledTexturesPerShaderStage() {
    return static_cast<int>(Get().maxSampledTexturesPerShaderStage);
}

void JGPU::WGPULimits::SetMaxSamplersPerShaderStage(int value) {
    Get().maxSamplersPerShaderStage = value;
}

int JGPU::WGPULimits::GetMaxSamplersPerShaderStage() {
    return static_cast<int>(Get().maxSamplersPerShaderStage);
}

void JGPU::WGPULimits::SetMaxStorageBuffersPerShaderStage(int value) {
    Get().maxStorageBuffersPerShaderStage = value;
}

int JGPU::WGPULimits::GetMaxStorageBuffersPerShaderStage() {
    return static_cast<int>(Get().maxStorageBuffersPerShaderStage);
}

void JGPU::WGPULimits::SetMaxStorageTexturesPerShaderStage(int value) {
    Get().maxStorageTexturesPerShaderStage = value;
}

int JGPU::WGPULimits::GetMaxStorageTexturesPerShaderStage() {
    return static_cast<int>(Get().maxStorageTexturesPerShaderStage);
}

void JGPU::WGPULimits::SetMaxUniformBuffersPerShaderStage(int value) {
    Get().maxUniformBuffersPerShaderStage = value;
}

int JGPU::WGPULimits::GetMaxUniformBuffersPerShaderStage() {
    return static_cast<int>(Get().maxUniformBuffersPerShaderStage);
}

void JGPU::WGPULimits::SetMaxUniformBufferBindingSize(int value) {
    Get().maxUniformBufferBindingSize = value;
}

int JGPU::WGPULimits::GetMaxUniformBufferBindingSize() {
    return static_cast<int>(Get().maxUniformBufferBindingSize);
}

void JGPU::WGPULimits::SetMaxStorageBufferBindingSize(int value) {
    Get().maxStorageBufferBindingSize = value;
}

int JGPU::WGPULimits::GetMaxStorageBufferBindingSize() {
    return static_cast<int>(Get().maxStorageBufferBindingSize);
}

void JGPU::WGPULimits::SetMinUniformBufferOffsetAlignment(int value) {
    Get().minUniformBufferOffsetAlignment = value;
}

int JGPU::WGPULimits::GetMinUniformBufferOffsetAlignment() {
    return static_cast<int>(Get().minUniformBufferOffsetAlignment);
}

void JGPU::WGPULimits::SetMinStorageBufferOffsetAlignment(int value) {
    Get().minStorageBufferOffsetAlignment = value;
}

int JGPU::WGPULimits::GetMinStorageBufferOffsetAlignment() {
    return static_cast<int>(Get().minStorageBufferOffsetAlignment);
}

void JGPU::WGPULimits::SetMaxVertexBuffers(int value) {
    Get().maxVertexBuffers = value;
}

int JGPU::WGPULimits::GetMaxVertexBuffers() {
    return static_cast<int>(Get().maxVertexBuffers);
}

void JGPU::WGPULimits::SetMaxBufferSize(int value) {
    Get().maxBufferSize = value;
}

int JGPU::WGPULimits::GetMaxBufferSize() {
    return static_cast<int>(Get().maxBufferSize);
}

void JGPU::WGPULimits::SetMaxVertexAttributes(int value) {
    Get().maxVertexAttributes = value;
}

int JGPU::WGPULimits::GetMaxVertexAttributes() {
    return static_cast<int>(Get().maxVertexAttributes);
}

void JGPU::WGPULimits::SetMaxVertexBufferArrayStride(int value) {
    Get().maxVertexBufferArrayStride = value;
}

int JGPU::WGPULimits::GetMaxVertexBufferArrayStride() {
    return static_cast<int>(Get().maxVertexBufferArrayStride);
}

void JGPU::WGPULimits::SetMaxInterStageShaderVariables(int value) {
    Get().maxInterStageShaderVariables = value;
}

int JGPU::WGPULimits::GetMaxInterStageShaderVariables() {
    return static_cast<int>(Get().maxInterStageShaderVariables);
}

void JGPU::WGPULimits::SetMaxColorAttachments(int value) {
    Get().maxColorAttachments = value;
}

int JGPU::WGPULimits::GetMaxColorAttachments() {
    return static_cast<int>(Get().maxColorAttachments);
}

void JGPU::WGPULimits::SetMaxColorAttachmentBytesPerSample(int value) {
    Get().maxColorAttachmentBytesPerSample = value;
}

int JGPU::WGPULimits::GetMaxColorAttachmentBytesPerSample() {
    return static_cast<int>(Get().maxColorAttachmentBytesPerSample);
}

void JGPU::WGPULimits::SetMaxComputeWorkgroupStorageSize(int value) {
    Get().maxComputeWorkgroupStorageSize = value;
}

int JGPU::WGPULimits::GetMaxComputeWorkgroupStorageSize() {
    return static_cast<int>(Get().maxComputeWorkgroupStorageSize);
}

void JGPU::WGPULimits::SetMaxComputeInvocationsPerWorkgroup(int value) {
    Get().maxComputeInvocationsPerWorkgroup = value;
}

int JGPU::WGPULimits::GetMaxComputeInvocationsPerWorkgroup() {
    return static_cast<int>(Get().maxComputeInvocationsPerWorkgroup);
}

void JGPU::WGPULimits::SetMaxComputeWorkgroupSizeX(int value) {
    Get().maxComputeWorkgroupSizeX = value;
}

int JGPU::WGPULimits::GetMaxComputeWorkgroupSizeX() {
    return static_cast<int>(Get().maxComputeWorkgroupSizeX);
}

void JGPU::WGPULimits::SetMaxComputeWorkgroupSizeY(int value) {
    Get().maxComputeWorkgroupSizeY = value;
}

int JGPU::WGPULimits::GetMaxComputeWorkgroupSizeY() {
    return static_cast<int>(Get().maxComputeWorkgroupSizeY);
}

void JGPU::WGPULimits::SetMaxComputeWorkgroupSizeZ(int value) {
    Get().maxComputeWorkgroupSizeZ = value;
}

int JGPU::WGPULimits::GetMaxComputeWorkgroupSizeZ() {
    return static_cast<int>(Get().maxComputeWorkgroupSizeZ);
}

void JGPU::WGPULimits::SetMaxComputeWorkgroupsPerDimension(int value) {
    Get().maxComputeWorkgroupsPerDimension = value;
}

int JGPU::WGPULimits::GetMaxComputeWorkgroupsPerDimension() {
    return static_cast<int>(Get().maxComputeWorkgroupsPerDimension);
}

// JGPU::WGPUBufferBindingLayout
JGPU::WGPUBufferBindingLayout* JGPU::WGPUBufferBindingLayout::Obtain() {
    static JGPU::WGPUBufferBindingLayout obj;
    obj = JGPU::WGPUBufferBindingLayout();
    return &obj;
}

void JGPU::WGPUBufferBindingLayout::SetNextInChain(JGPU::WGPUChainedStruct* chainedStruct) {
    Get()->nextInChain = chainedStruct != NULL ? chainedStruct->Get() : NULL;
}

void JGPU::WGPUBufferBindingLayout::SetType(WGPUBufferBindingType type) {
    Get()->type = type;
}

void JGPU::WGPUBufferBindingLayout::SetHasDynamicOffset(int hasDynamicOffset) {
    Get()->hasDynamicOffset = hasDynamicOffset;
}

void JGPU::WGPUBufferBindingLayout::SetMinBindingSize(int minBindingSize) {
    Get()->minBindingSize = minBindingSize;
}

// JGPU::WGPUSamplerBindingLayout
JGPU::WGPUSamplerBindingLayout* JGPU::WGPUSamplerBindingLayout::Obtain() {
    static JGPU::WGPUSamplerBindingLayout obj;
    obj = JGPU::WGPUSamplerBindingLayout();
    return &obj;
}

void JGPU::WGPUSamplerBindingLayout::SetNextInChain(JGPU::WGPUChainedStruct* chainedStruct) {
    Get()->nextInChain = chainedStruct != NULL ? chainedStruct->Get() : NULL;
}

void JGPU::WGPUSamplerBindingLayout::SetType(WGPUSamplerBindingType type) {
    Get()->type = type;
}

// JGPU::WGPUTextureBindingLayout
JGPU::WGPUTextureBindingLayout* JGPU::WGPUTextureBindingLayout::Obtain() {
    static JGPU::WGPUTextureBindingLayout obj;
    obj = JGPU::WGPUTextureBindingLayout();
    return &obj;
}

void JGPU::WGPUTextureBindingLayout::SetNextInChain(JGPU::WGPUChainedStruct* chainedStruct) {
    Get()->nextInChain = chainedStruct != NULL ? chainedStruct->Get() : NULL;
}

void JGPU::WGPUTextureBindingLayout::SetSampleType(WGPUTextureSampleType sampleType) {
    Get()->sampleType = sampleType;
}

void JGPU::WGPUTextureBindingLayout::SetViewDimension(WGPUTextureViewDimension viewDimension) {
    Get()->viewDimension = viewDimension;
}

void JGPU::WGPUTextureBindingLayout::SetMultisampled(int multisampled) {
    Get()->multisampled = multisampled;
}

// JGPU::WGPUStorageTextureBindingLayout
JGPU::WGPUStorageTextureBindingLayout* JGPU::WGPUStorageTextureBindingLayout::Obtain() {
    static JGPU::WGPUStorageTextureBindingLayout obj;
    obj = JGPU::WGPUStorageTextureBindingLayout();
    return &obj;
}

void JGPU::WGPUStorageTextureBindingLayout::SetNextInChain(JGPU::WGPUChainedStruct* chainedStruct) {
    Get()->nextInChain = chainedStruct != NULL ? chainedStruct->Get() : NULL;
}

void JGPU::WGPUStorageTextureBindingLayout::SetAccess(WGPUStorageTextureAccess access) {
    Get()->access = access;
}

void JGPU::WGPUStorageTextureBindingLayout::SetFormat(WGPUTextureFormat format) {
    Get()->format = format;
}

void JGPU::WGPUStorageTextureBindingLayout::SetViewDimension(WGPUTextureViewDimension viewDimension) {
    Get()->viewDimension = viewDimension;
}

// JGPU::WGPUBindGroupLayoutEntry
JGPU::WGPUBindGroupLayoutEntry* JGPU::WGPUBindGroupLayoutEntry::Obtain() {
    static JGPU::WGPUBindGroupLayoutEntry obj;
    obj = JGPU::WGPUBindGroupLayoutEntry();
    return &obj;
}

void JGPU::WGPUBindGroupLayoutEntry::SetNextInChain(JGPU::WGPUChainedStruct* chainedStruct) {
    Get().nextInChain = chainedStruct != NULL ? chainedStruct->Get() : NULL;
}

void JGPU::WGPUBindGroupLayoutEntry::SetBinding(int binding) {
    Get().binding = binding;
}

void JGPU::WGPUBindGroupLayoutEntry::SetVisibility(WGPUShaderStage visibility) {
    Get().visibility = visibility;
}

void JGPU::WGPUBindGroupLayoutEntry::SetBuffer(JGPU::WGPUBufferBindingLayout* buffer) {
    Get().buffer = *buffer->Get();
}

void JGPU::WGPUBindGroupLayoutEntry::SetSampler(JGPU::WGPUSamplerBindingLayout* sampler) {
    Get().sampler = *sampler->Get();
}

void JGPU::WGPUBindGroupLayoutEntry::SetTexture(JGPU::WGPUTextureBindingLayout* texture) {
    Get().texture = *texture->Get();
}

void JGPU::WGPUBindGroupLayoutEntry::SetStorageTexture(JGPU::WGPUStorageTextureBindingLayout* storageTexture) {
    Get().storageTexture = *storageTexture->Get();
}

JGPU::WGPUBufferBindingLayout JGPU::WGPUBindGroupLayoutEntry::GetBuffer() {
    JGPU::WGPUBufferBindingLayout temp(&Get().buffer);
    return temp;
}

JGPU::WGPUSamplerBindingLayout JGPU::WGPUBindGroupLayoutEntry::GetSampler() {
    JGPU::WGPUSamplerBindingLayout temp(&Get().sampler);
    return temp;
}

JGPU::WGPUStorageTextureBindingLayout JGPU::WGPUBindGroupLayoutEntry::GetStorageTexture() {
    JGPU::WGPUStorageTextureBindingLayout temp(&Get().storageTexture);
    return temp;
}

JGPU::WGPUTextureBindingLayout JGPU::WGPUBindGroupLayoutEntry::GetTexture() {
    JGPU::WGPUTextureBindingLayout temp(&Get().texture);
    return temp;
}

// JGPU::WGPURequestAdapterOptions
JGPU::WGPURequestAdapterOptions* JGPU::WGPURequestAdapterOptions::Obtain() {
    static JGPU::WGPURequestAdapterOptions obj;
    obj = JGPU::WGPURequestAdapterOptions();
    return &obj;
}

void JGPU::WGPURequestAdapterOptions::SetNextInChain(JGPU::WGPUChainedStruct* chainedStruct) {
    Get().nextInChain = chainedStruct != NULL ? chainedStruct->Get() : NULL;
}

void JGPU::WGPURequestAdapterOptions::SetFeatureLevel(WGPUFeatureLevel featureLevel) {
    Get().featureLevel = featureLevel;
}

void JGPU::WGPURequestAdapterOptions::SetPowerPreference(WGPUPowerPreference powerPreference) {
    Get().powerPreference = powerPreference;
}

void JGPU::WGPURequestAdapterOptions::SetBackendType(WGPUBackendType backendType) {
    Get().backendType = backendType;
}

void JGPU::WGPURequestAdapterOptions::SetCompatibleSurface(JGPU::WGPUSurface* compatibleSurface) {
    Get().compatibleSurface = compatibleSurface->Get();
}

// JGPU::WGPUAdapterInfo
JGPU::WGPUAdapterInfo* JGPU::WGPUAdapterInfo::Obtain() {
    static JGPU::WGPUAdapterInfo obj;
    obj = JGPU::WGPUAdapterInfo();
    return &obj;
}

std::string JGPU::WGPUAdapterInfo::GetVendor() {
    JGPU::WGPUStringView stringView(Get().vendor);
    return stringView.GetString();
}

int JGPU::WGPUAdapterInfo::GetVendorID() {
    return Get().vendorID;
}

std::string JGPU::WGPUAdapterInfo::GetArchitecture() {
    JGPU::WGPUStringView stringView(Get().architecture);
    return stringView.GetString();
}

std::string JGPU::WGPUAdapterInfo::GetDevice() {
    JGPU::WGPUStringView stringView(Get().device);
    return stringView.GetString();
}

int JGPU::WGPUAdapterInfo::GetDeviceID() {
    return Get().deviceID;
}

std::string JGPU::WGPUAdapterInfo::GetDescription() {
    JGPU::WGPUStringView stringView(Get().description);
    return stringView.GetString();
}

WGPUBackendType JGPU::WGPUAdapterInfo::GetBackendType() {
    return Get().backendType;
}

WGPUAdapterType JGPU::WGPUAdapterInfo::GetAdapterType() {
    return Get().adapterType;
}

// JGPU::WGPUFutureWaitInfo
JGPU::WGPUFutureWaitInfo* JGPU::WGPUFutureWaitInfo::Obtain() {
    static JGPU::WGPUFutureWaitInfo obj;
    obj = JGPU::WGPUFutureWaitInfo();
    return &obj;
}

void JGPU::WGPUFutureWaitInfo::SetFuture(JGPU::WGPUFuture* future) {
    Get().future = future != NULL ? future->Get() : ::WGPUFuture();
}

// JGPU::WGPUVertexAttribute
JGPU::WGPUVertexAttribute* JGPU::WGPUVertexAttribute::Obtain() {
    static JGPU::WGPUVertexAttribute obj;
    obj = JGPU::WGPUVertexAttribute();
    return &obj;
}

void JGPU::WGPUVertexAttribute::SetFormat(WGPUVertexFormat format) {
    Get().format = format;
}

void JGPU::WGPUVertexAttribute::SetOffset(int offset) {
    Get().offset = offset;
}

void JGPU::WGPUVertexAttribute::SetShaderLocation(int shaderLocation) {
    Get().shaderLocation = shaderLocation;
}

// JGPU::WGPUVertexBufferLayout
JGPU::WGPUVertexBufferLayout* JGPU::WGPUVertexBufferLayout::Obtain() {
    static JGPU::WGPUVertexBufferLayout obj;
    obj = JGPU::WGPUVertexBufferLayout();
    return &obj;
}

void JGPU::WGPUVertexBufferLayout::SetAttributes(WGPUVectorVertexAttribute* values) {
    if(values != NULL) {
        Get().attributeCount = values->size();
        Get().attributes = reinterpret_cast<const ::WGPUVertexAttribute*>(values->data());
    }
    else {
        Get().attributeCount = 0;
        Get().attributes = NULL;
    }
}

void JGPU::WGPUVertexBufferLayout::SetArrayStride(int offset) {
    Get().arrayStride = offset;
}

void JGPU::WGPUVertexBufferLayout::SetStepMode(WGPUVertexStepMode stepMode) {
    Get().stepMode = stepMode;
}

// JGPU::WGPUVertexState
void JGPU::WGPUVertexState::SetNextInChain(JGPU::WGPUChainedStruct* chainedStruct) {
    Get()->nextInChain = chainedStruct != NULL ? chainedStruct->Get() : NULL;
}

void JGPU::WGPUVertexState::SetModule(JGPU::WGPUShaderModule* shaderModule) {
    Get()->module = shaderModule != NULL ? shaderModule->Get() : NULL;
}

void JGPU::WGPUVertexState::SetEntryPoint(const char* value) {
    JGPU::WGPUStringView stringView(value);
    Get()->entryPoint = stringView.Get();
}

void JGPU::WGPUVertexState::SetConstants(WGPUVectorConstantEntry* values) {
    if(values != NULL) {
        Get()->constantCount = values->size();
        Get()->constants = reinterpret_cast<const ::WGPUConstantEntry*>(values->data());
    }
    else {
        Get()->constantCount = 0;
        Get()->constants = NULL;
    }
}

void JGPU::WGPUVertexState::SetBuffers(WGPUVectorVertexBufferLayout* values) {
    if(values != NULL) {
        Get()->bufferCount = values->size();
        Get()->buffers = reinterpret_cast<const ::WGPUVertexBufferLayout*>(values->data());
    }
    else {
        Get()->bufferCount = 0;
        Get()->buffers = NULL;
    }
}

// JGPU::WGPUShaderSourceWGSL
JGPU::WGPUShaderSourceWGSL* JGPU::WGPUShaderSourceWGSL::Obtain() {
    static JGPU::WGPUShaderSourceWGSL obj;
    obj = JGPU::WGPUShaderSourceWGSL();
    return &obj;
}

void JGPU::WGPUShaderSourceWGSL::SetCode(const char* value) {
    JGPU::WGPUStringView stringView(value);
    Get().code = stringView.Get();
}

JGPU::WGPUChainedStruct JGPU::WGPUShaderSourceWGSL::GetChain() {
    ::WGPUChainedStruct* wgpuChain = &Get().chain;
    JGPU::WGPUChainedStruct chain;
    chain.Set(wgpuChain);
    return chain;
}

// JGPU::WGPUBlendComponent
void JGPU::WGPUBlendComponent::SetOperation(WGPUBlendOperation operation) {
    Get()->operation = operation;
}

WGPUBlendOperation JGPU::WGPUBlendComponent::GetOperation() {
    return Get()->operation;
}

void JGPU::WGPUBlendComponent::SetSrcFactor(WGPUBlendFactor factor) {
    Get()->srcFactor = factor;
}

WGPUBlendFactor JGPU::WGPUBlendComponent::GetSrcFactor() {
    return Get()->srcFactor;
}

void JGPU::WGPUBlendComponent::SetDstFactor(WGPUBlendFactor factor) {
    Get()->dstFactor = factor;
}

WGPUBlendFactor JGPU::WGPUBlendComponent::GetDstFactor() {
    return Get()->dstFactor;
}

// JGPU::WGPUBlendState
JGPU::WGPUBlendState* JGPU::WGPUBlendState::Obtain() {
    static JGPU::WGPUBlendState obj;
    obj = JGPU::WGPUBlendState();
    return &obj;
}

JGPU::WGPUBlendComponent JGPU::WGPUBlendState::GetColor() {
    JGPU::WGPUBlendComponent temp;
    temp.Set(&Get().color);
    return temp;
}

JGPU::WGPUBlendComponent JGPU::WGPUBlendState::GetAlpha() {
    JGPU::WGPUBlendComponent temp;
    temp.Set(&Get().alpha);
    return temp;
}

// JGPU::WGPUColorTargetState
JGPU::WGPUColorTargetState* JGPU::WGPUColorTargetState::Obtain() {
    static JGPU::WGPUColorTargetState obj;
    obj = JGPU::WGPUColorTargetState();
    return &obj;
}

void JGPU::WGPUColorTargetState::SetNextInChain(JGPU::WGPUChainedStruct* chainedStruct) {
    Get().nextInChain = chainedStruct != NULL ? chainedStruct->Get() : NULL;
}

void JGPU::WGPUColorTargetState::SetFormat(WGPUTextureFormat format) {
    Get().format = format;
}

void JGPU::WGPUColorTargetState::SetBlend(JGPU::WGPUBlendState* blendState) {
    Get().blend = reinterpret_cast<const ::WGPUBlendState*>(blendState);
}

void JGPU::WGPUColorTargetState::SetWriteMask(WGPUColorWriteMask writeMask) {
    Get().writeMask = writeMask;
}

// JGPU::WGPUFragmentState
JGPU::WGPUFragmentState* JGPU::WGPUFragmentState::Obtain() {
    static JGPU::WGPUFragmentState obj;
    obj = JGPU::WGPUFragmentState();
    return &obj;
}

void JGPU::WGPUFragmentState::SetNextInChain(JGPU::WGPUChainedStruct* chainedStruct) {
    Get().nextInChain = chainedStruct != NULL ? chainedStruct->Get() : NULL;
}

void JGPU::WGPUFragmentState::SetEntryPoint(const char* value) {
    JGPU::WGPUStringView stringView(value);
    Get().entryPoint = stringView.Get();
}

void JGPU::WGPUFragmentState::SetTargets(WGPUVectorColorTargetState* values) {
    if(values != NULL) {
        Get().targetCount = values->size();
        Get().targets = reinterpret_cast<const ::WGPUColorTargetState*>(values->data());
    }
    else {
        Get().targetCount = 0;
        Get().targets = NULL;
    }
}

void JGPU::WGPUFragmentState::SetModule(JGPU::WGPUShaderModule* shaderModule) {
    Get().module = shaderModule != NULL ? shaderModule->Get() : NULL;
}

void JGPU::WGPUFragmentState::SetConstants(WGPUVectorConstantEntry* values) {
    if(values != NULL) {
        Get().constantCount = values->size();
        Get().constants = reinterpret_cast<const ::WGPUConstantEntry*>(values->data());
    }
    else {
        Get().constantCount = 0;
        Get().constants = NULL;
    }
}

// JGPU::WGPUPrimitiveState
void JGPU::WGPUPrimitiveState::SetNextInChain(JGPU::WGPUChainedStruct* chainedStruct) {
    Get()->nextInChain = chainedStruct != NULL ? chainedStruct->Get() : NULL;
}

void JGPU::WGPUPrimitiveState::SetTopology(WGPUPrimitiveTopology value) {
    Get()->topology = value;
}

void JGPU::WGPUPrimitiveState::SetStripIndexFormat(WGPUIndexFormat value) {
    Get()->stripIndexFormat = value;
}

void JGPU::WGPUPrimitiveState::SetFrontFace(WGPUFrontFace value) {
    Get()->frontFace = value;
}

void JGPU::WGPUPrimitiveState::SetCullMode(WGPUCullMode value) {
    Get()->cullMode = value;
}

// JGPU::WGPUStencilFaceState
void JGPU::WGPUStencilFaceState::SetCompare(WGPUCompareFunction compare) {
    Get()->compare = compare;
}

void JGPU::WGPUStencilFaceState::SetFailOp(WGPUStencilOperation failOp) {
    Get()->failOp = failOp;
}

void JGPU::WGPUStencilFaceState::SetDepthFailOp(WGPUStencilOperation depthFailOp) {
    Get()->depthFailOp = depthFailOp;
}

void JGPU::WGPUStencilFaceState::SetPassOp(WGPUStencilOperation passOp) {
    Get()->passOp = passOp;
}

// JGPU::WGPUDepthStencilState
JGPU::WGPUDepthStencilState* JGPU::WGPUDepthStencilState::Obtain() {
    static JGPU::WGPUDepthStencilState obj;
    obj = JGPU::WGPUDepthStencilState();
    return &obj;
}

void JGPU::WGPUDepthStencilState::SetNextInChain(JGPU::WGPUChainedStruct* chainedStruct) {
    Get().nextInChain = chainedStruct != NULL ? chainedStruct->Get() : NULL;
}

void JGPU::WGPUDepthStencilState::SetFormat(WGPUTextureFormat format) {
    Get().format = format;
}

void JGPU::WGPUDepthStencilState::SetDepthWriteEnabled(WGPUOptionalBool depthWriteEnabled) {
    Get().depthWriteEnabled = depthWriteEnabled;
}

void JGPU::WGPUDepthStencilState::SetDepthCompare(WGPUCompareFunction depthCompare) {
    Get().depthCompare = depthCompare;
}

void JGPU::WGPUDepthStencilState::SetDepthBiasSlopeScale(float depthBiasSlopeScale) {
    Get().depthBiasSlopeScale = depthBiasSlopeScale;
}

void JGPU::WGPUDepthStencilState::SetDepthBiasClamp(float depthBiasClamp) {
    Get().depthBiasClamp = depthBiasClamp;
}

void JGPU::WGPUDepthStencilState::SetStencilReadMask(int stencilReadMask) {
    Get().stencilReadMask = stencilReadMask;
}

void JGPU::WGPUDepthStencilState::SetStencilWriteMask(int stencilWriteMask) {
    Get().stencilWriteMask = stencilWriteMask;
}

void JGPU::WGPUDepthStencilState::SetDepthBias(int depthBias) {
    Get().depthBias = depthBias;
}

JGPU::WGPUStencilFaceState JGPU::WGPUDepthStencilState::GetStencilFront() {
    JGPU::WGPUStencilFaceState temp;
    temp.Set(&Get().stencilFront);
    return temp;
}

JGPU::WGPUStencilFaceState JGPU::WGPUDepthStencilState::GetStencilBack() {
    JGPU::WGPUStencilFaceState temp;
    temp.Set(&Get().stencilBack);
    return temp;
}

// JGPU::WGPUMultisampleState
void JGPU::WGPUMultisampleState::SetNextInChain(JGPU::WGPUChainedStruct* chainedStruct) {
    Get()->nextInChain = chainedStruct != NULL ? chainedStruct->Get() : NULL;
}

void JGPU::WGPUMultisampleState::SetCount(int count) {
    Get()->count = count;
}

void JGPU::WGPUMultisampleState::SetMask(int mask) {
    Get()->mask = mask;
}

void JGPU::WGPUMultisampleState::SetAlphaToCoverageEnabled(bool alphaToCoverageEnabled) {
    Get()->alphaToCoverageEnabled = alphaToCoverageEnabled;
}

// JGPU::WGPUColor
JGPU::WGPUColor* JGPU::WGPUColor::Obtain() {
    static JGPU::WGPUColor obj;
    obj = JGPU::WGPUColor();
    return &obj;
}

void JGPU::WGPUColor::SetColor(float r, float g, float b, float a) {
    Get()->r = r;
    Get()->g = g;
    Get()->b = b;
    Get()->a = a;
}

float JGPU::WGPUColor::GetR() {
    return Get()->r;
}

float JGPU::WGPUColor::GetG() {
    return Get()->g;
}

float JGPU::WGPUColor::GetB() {
    return Get()->b;
}

float JGPU::WGPUColor::GetA() {
    return Get()->a;
}

void JGPU::WGPUColor::SetR(float value) {
    Get()->r = value;
}

void JGPU::WGPUColor::SetG(float value) {
    Get()->g = value;
}

void JGPU::WGPUColor::SetB(float value) {
    Get()->b = value;
}

void JGPU::WGPUColor::SetA(float value) {
    Get()->a = value;
}

// JGPU::WGPUSupportedFeatures
JGPU::WGPUSupportedFeatures* JGPU::WGPUSupportedFeatures::Obtain() {
    static JGPU::WGPUSupportedFeatures obj;
    obj = JGPU::WGPUSupportedFeatures();
    return &obj;
}

int JGPU::WGPUSupportedFeatures::GetFeatureCount() {
    return Get().featureCount;
}

WGPUFeatureName JGPU::WGPUSupportedFeatures::GetFeatureAt(long index) {
    return Get().features[index];
}

// JGPU::WGPURenderPassDepthStencilAttachment
JGPU::WGPURenderPassDepthStencilAttachment* JGPU::WGPURenderPassDepthStencilAttachment::Obtain() {
    static JGPU::WGPURenderPassDepthStencilAttachment obj;
    obj = JGPU::WGPURenderPassDepthStencilAttachment();
    return &obj;
}

void JGPU::WGPURenderPassDepthStencilAttachment::SetNextInChain(JGPU::WGPUChainedStruct* chainedStruct) {
// TODO dont exist in wgpu-native
//    Get().nextInChain = chainedStruct ? chainedStruct->Get() : nullptr;
}

void JGPU::WGPURenderPassDepthStencilAttachment::SetView(JGPU::WGPUTextureView* textureView) {
    Get().view = textureView ? textureView->Get() : nullptr;
}

void JGPU::WGPURenderPassDepthStencilAttachment::SetDepthLoadOp(WGPULoadOp loadOp) {
    Get().depthLoadOp = loadOp;
}

void JGPU::WGPURenderPassDepthStencilAttachment::SetDepthStoreOp(WGPUStoreOp storeOp) {
    Get().depthStoreOp = storeOp;
}

void JGPU::WGPURenderPassDepthStencilAttachment::SetDepthClearValue(float depthClearValue) {
    Get().depthClearValue = depthClearValue;
}

void JGPU::WGPURenderPassDepthStencilAttachment::SetDepthReadOnly(bool depthReadOnly) {
    Get().depthReadOnly = depthReadOnly;
}

void JGPU::WGPURenderPassDepthStencilAttachment::SetStencilLoadOp(WGPULoadOp loadOp) {
    Get().stencilLoadOp = loadOp;
}

void JGPU::WGPURenderPassDepthStencilAttachment::SetStencilStoreOp(WGPUStoreOp storeOp) {
    Get().stencilStoreOp = storeOp;
}

void JGPU::WGPURenderPassDepthStencilAttachment::SetStencilClearValue(int stencilClearValue) {
    Get().stencilClearValue = stencilClearValue;
}

void JGPU::WGPURenderPassDepthStencilAttachment::SetStencilReadOnly(bool stencilReadOnly) {
    Get().stencilReadOnly = stencilReadOnly;
}

// JGPU::WGPURenderPassTimestampWrites
JGPU::WGPURenderPassTimestampWrites* JGPU::WGPURenderPassTimestampWrites::Obtain() {
    static JGPU::WGPURenderPassTimestampWrites obj;
    obj = JGPU::WGPURenderPassTimestampWrites();
    return &obj;
}

void JGPU::WGPURenderPassTimestampWrites::SetQuerySet(JGPU::WGPUQuerySet* value) {
    Get().querySet = value->Get();
}

void JGPU::WGPURenderPassTimestampWrites::SetBeginningOfPassWriteIndex(int value) {
    Get().beginningOfPassWriteIndex = value;
}

void JGPU::WGPURenderPassTimestampWrites::SetEndOfPassWriteIndex(int value) {
    Get().endOfPassWriteIndex = value;
}

// JGPU::WGPUComputePassTimestampWrites
JGPU::WGPUComputePassTimestampWrites* JGPU::WGPUComputePassTimestampWrites::Obtain() {
    static JGPU::WGPUComputePassTimestampWrites obj;
    obj = JGPU::WGPUComputePassTimestampWrites();
    return &obj;
}

void JGPU::WGPUComputePassTimestampWrites::SetQuerySet(JGPU::WGPUQuerySet* value) {
    Get().querySet = value->Get();
}

void JGPU::WGPUComputePassTimestampWrites::SetBeginningOfPassWriteIndex(int value) {
    Get().beginningOfPassWriteIndex = value;
}

void JGPU::WGPUComputePassTimestampWrites::SetEndOfPassWriteIndex(int value) {
    Get().endOfPassWriteIndex = value;
}

// JGPU::WGPUOrigin3D
JGPU::WGPUOrigin3D* JGPU::WGPUOrigin3D::Obtain() {
    static JGPU::WGPUOrigin3D obj;
    obj = JGPU::WGPUOrigin3D();
    return &obj;
}

void JGPU::WGPUOrigin3D::Set(int x, int y, int z) {
    Get()->x = x;
    Get()->y = y;
    Get()->z = z;
}

void JGPU::WGPUOrigin3D::SetX(int value) {
    Get()->x = value;
}

void JGPU::WGPUOrigin3D::SetY(int value) {
    Get()->y = value;
}

void JGPU::WGPUOrigin3D::SetZ(int value) {
    Get()->z = value;
}

// JGPU::WGPUTexelCopyTextureInfo
JGPU::WGPUTexelCopyTextureInfo* JGPU::WGPUTexelCopyTextureInfo::Obtain() {
    static JGPU::WGPUTexelCopyTextureInfo obj;
    obj = JGPU::WGPUTexelCopyTextureInfo();
    return &obj;
}

void JGPU::WGPUTexelCopyTextureInfo::SetTexture(JGPU::WGPUTexture* texture) {
    Get().texture = texture->Get();
}

void JGPU::WGPUTexelCopyTextureInfo::SetMipLevel(int mipLevel) {
    Get().mipLevel = mipLevel;
}

JGPU::WGPUOrigin3D JGPU::WGPUTexelCopyTextureInfo::GetOrigin() {
    JGPU::WGPUOrigin3D temp(&Get().origin);
    return temp;
}

void JGPU::WGPUTexelCopyTextureInfo::SetAspect(WGPUTextureAspect aspect) {
    Get().aspect = aspect;
}

// JGPU::WGPUExtent3D
JGPU::WGPUExtent3D* JGPU::WGPUExtent3D::Obtain() {
    static JGPU::WGPUExtent3D obj;
    obj = JGPU::WGPUExtent3D();
    return &obj;
}

void JGPU::WGPUExtent3D::SetWidth(int width) {
    Get()->width = width;
}

void JGPU::WGPUExtent3D::SetHeight(int height) {
    Get()->height = height;
}

void JGPU::WGPUExtent3D::SetDepthOrArrayLayers(int depthOrArrayLayers) {
    Get()->depthOrArrayLayers = depthOrArrayLayers;
}

// JGPU::WGPUTexelCopyBufferLayout
JGPU::WGPUTexelCopyBufferLayout* JGPU::WGPUTexelCopyBufferLayout::Obtain() {
    static JGPU::WGPUTexelCopyBufferLayout obj;
    obj = JGPU::WGPUTexelCopyBufferLayout();
    return &obj;
}

void JGPU::WGPUTexelCopyBufferLayout::SetOffset(int offset) {
    Get()->offset = offset;
}

void JGPU::WGPUTexelCopyBufferLayout::SetBytesPerRow(int bytesPerRow) {
    Get()->bytesPerRow = bytesPerRow;
}

void JGPU::WGPUTexelCopyBufferLayout::SetRowsPerImage(int rowsPerImage) {
    Get()->rowsPerImage = rowsPerImage;
}

// JGPU::WGPUTexelCopyBufferInfo
JGPU::WGPUTexelCopyBufferInfo* JGPU::WGPUTexelCopyBufferInfo::Obtain() {
    static JGPU::WGPUTexelCopyBufferInfo obj;
    obj = JGPU::WGPUTexelCopyBufferInfo();
    return &obj;
}

JGPU::WGPUTexelCopyBufferLayout JGPU::WGPUTexelCopyBufferInfo::GetLayout() {
    JGPU::WGPUTexelCopyBufferLayout temp(&Get().layout);
    return temp;
}

JGPU::WGPUBuffer JGPU::WGPUTexelCopyBufferInfo::GetBuffer() {
    JGPU::WGPUBuffer temp;
    temp.Set(Get().buffer);
    return temp;
}

void JGPU::WGPUTexelCopyBufferInfo::SetBuffer(JGPU::WGPUBuffer* buffer) {
    Get().buffer = buffer->Get();
}

// JGPU::WGPUSurfaceConfiguration
JGPU::WGPUSurfaceConfiguration* JGPU::WGPUSurfaceConfiguration::Obtain() {
    static JGPU::WGPUSurfaceConfiguration obj;
    obj = JGPU::WGPUSurfaceConfiguration();
    return &obj;
}

void JGPU::WGPUSurfaceConfiguration::SetNextInChain(JGPU::WGPUChainedStruct* chainedStruct) {
    Get().nextInChain = chainedStruct != NULL ? chainedStruct->Get() : NULL;
}

void JGPU::WGPUSurfaceConfiguration::SetWidth(long width) {
    Get().width = width;
}

void JGPU::WGPUSurfaceConfiguration::SetHeight(long height) {
    Get().height = height;
}

void JGPU::WGPUSurfaceConfiguration::SetFormat(WGPUTextureFormat format) {
    Get().format = format;
}

void JGPU::WGPUSurfaceConfiguration::SetViewFormatCount(long value) {
    Get().viewFormatCount = value;
}

void JGPU::WGPUSurfaceConfiguration::SetViewFormats(WGPUVectorTextureFormat* values) {
    if(values != NULL) {
        Get().viewFormatCount = values->size();
        Get().viewFormats = reinterpret_cast<const WGPUTextureFormat*>(values->data());
    }
    else {
        Get().viewFormatCount = 0;
        Get().viewFormats = NULL;
    }
}

void JGPU::WGPUSurfaceConfiguration::SetUsage(WGPUTextureUsage usage) {
    Get().usage = usage;
}

void JGPU::WGPUSurfaceConfiguration::SetDevice(JGPU::WGPUDevice* device) {
    Get().device = device != NULL ? device->Get() : NULL;
}

void JGPU::WGPUSurfaceConfiguration::SetPresentMode(WGPUPresentMode presentMode) {
    Get().presentMode = presentMode;
}

void JGPU::WGPUSurfaceConfiguration::SetAlphaMode(WGPUCompositeAlphaMode alphaMode) {
    Get().alphaMode = alphaMode;
}

// JGPU::WGPUSurfaceCapabilities
JGPU::WGPUSurfaceCapabilities* JGPU::WGPUSurfaceCapabilities::Obtain() {
    static JGPU::WGPUSurfaceCapabilities obj;
    obj = JGPU::WGPUSurfaceCapabilities();
    return &obj;
}

WGPUVectorTextureFormat JGPU::WGPUSurfaceCapabilities::GetFormats() {
    WGPUVectorTextureFormat temp(Get().formats, Get().formatCount);
    return temp;
}

// JGPU::WGPURenderPassColorAttachment
JGPU::WGPURenderPassColorAttachment::WGPURenderPassColorAttachment() {
    SetDepthSlice(WGPU_DEPTH_SLICE_UNDEFINED); // Required for emscripten
}

JGPU::WGPURenderPassColorAttachment* JGPU::WGPURenderPassColorAttachment::Obtain() {
    static JGPU::WGPURenderPassColorAttachment obj;
    obj = JGPU::WGPURenderPassColorAttachment();
    return &obj;
}

void JGPU::WGPURenderPassColorAttachment::Reset() {
    mHandle.nextInChain = NULL;
    mHandle.resolveTarget = NULL;
    mHandle.view = NULL;
    mHandle.loadOp = WGPULoadOp_Undefined;
    mHandle.storeOp = WGPUStoreOp_Undefined;
    mHandle.depthSlice = ~0u;
    mHandle.clearValue.r = 0;
    mHandle.clearValue.g = 0;
    mHandle.clearValue.b = 0;
    mHandle.clearValue.a = 0;
}

void JGPU::WGPURenderPassColorAttachment::SetNextInChain(JGPU::WGPUChainedStruct* chainedStruct) {
    Get().nextInChain = chainedStruct ? chainedStruct->Get() : nullptr;
}

void JGPU::WGPURenderPassColorAttachment::SetResolveTarget(JGPU::WGPUTextureView* textureView) {
    Get().resolveTarget = textureView == NULL ? NULL : textureView->Get();
}

void JGPU::WGPURenderPassColorAttachment::SetView(JGPU::WGPUTextureView* textureView) {
    Get().view = textureView == NULL ? NULL : textureView->Get();
}

void JGPU::WGPURenderPassColorAttachment::SetLoadOp(WGPULoadOp loadOp) {
    Get().loadOp = loadOp;
}

void JGPU::WGPURenderPassColorAttachment::SetStoreOp(WGPUStoreOp storeOp) {
    Get().storeOp = storeOp;
}

void JGPU::WGPURenderPassColorAttachment::SetDepthSlice(int depthSlice) {
    Get().depthSlice = depthSlice;
}

JGPU::WGPUColor JGPU::WGPURenderPassColorAttachment::GetClearValue() {
    JGPU::WGPUColor temp(&Get().clearValue);
    return temp;
}

// JGPU::WGPUSurfaceTexture
JGPU::WGPUSurfaceTexture* JGPU::WGPUSurfaceTexture::Obtain() {
    static JGPU::WGPUSurfaceTexture obj;
    obj = JGPU::WGPUSurfaceTexture();
    return &obj;
}

void JGPU::WGPUSurfaceTexture::GetTexture(JGPU::WGPUTexture* tex) {
    tex->Set(Get().texture);
}

WGPUSurfaceGetCurrentTextureStatus JGPU::WGPUSurfaceTexture::GetStatus() {
    return Get().status;
}

// WGPU
WGPUPlatformType WGPU::GetPlatformType() {
    #if __EMSCRIPTEN__
        return WGPUPlatformType::WGPU_Web;
    #elif __ANDROID__
        return WGPUPlatformType::WGPU_Android;
    #elif TARGET_OS_IPHONE
        return WGPUPlatformType::WGPU_iOS;
    #elif _WIN32
        return WGPUPlatformType::WGPU_Windows;
    #elif __linux__
        return WGPUPlatformType::WGPU_Linux;
    #elif TARGET_OS_MAC
        return WGPUPlatformType::WGPU_Mac;
    #else
        return WGPUPlatformType::WGPU_Unknown;
    #endif
}

JGPU::WGPUInstance* WGPU::CreateInstance(JGPU::WGPUInstanceDescriptor* descriptor) {
    JGPU::WGPUInstance* instance = new JGPU::WGPUInstance();
    instance->Set(wgpuCreateInstance(descriptor != NULL ? &descriptor->Get() : NULL));
    return instance;
}

STBImage* WGPU::loadImage(WGPUByteBuffer* buffer, int desiredChannels) {
    int32_t width, height, format;
    unsigned char* pixels = stbi_load_from_memory(buffer->data(), buffer->getLimit(), &width, &height, &format, desiredChannels);
    if (pixels == NULL)
        return NULL;

    int bufferFormat = format;
    if(desiredChannels > 0) {
        bufferFormat = desiredChannels;
    }

    STBImage* stbImage = new STBImage();
    stbImage->pixel = WGPUByteBuffer(pixels, width * height * bufferFormat);
    stbi_image_free(pixels);
    stbImage->width = width;
    stbImage->height = height;
    stbImage->format = format;
    return stbImage;
}

// JGPU::WGPUBindGroupEntry
JGPU::WGPUBindGroupEntry* JGPU::WGPUBindGroupEntry::Obtain() {
    static JGPU::WGPUBindGroupEntry obj;
    obj = JGPU::WGPUBindGroupEntry();
    return &obj;
}

void JGPU::WGPUBindGroupEntry::Reset() {
    mHandle = ::WGPUBindGroupEntry{};
    mHandle.nextInChain = NULL;
    mHandle.binding = 0;
    mHandle.buffer = NULL;
    mHandle.offset = 0;
    mHandle.size = 0;
    mHandle.sampler = NULL;
    mHandle.textureView = NULL;
}

void JGPU::WGPUBindGroupEntry::SetNextInChain(JGPU::WGPUChainedStruct* chainedStruct) {
    Get().nextInChain = chainedStruct ? chainedStruct->Get() : nullptr;
}

void JGPU::WGPUBindGroupEntry::SetBinding(int binding) {
    Get().binding = binding;
}

void JGPU::WGPUBindGroupEntry::SetBuffer(JGPU::WGPUBuffer* buffer) {
    Get().buffer = buffer ? buffer->Get() : nullptr;
}

void JGPU::WGPUBindGroupEntry::SetOffset(int offset) {
    Get().offset = offset;
}

void JGPU::WGPUBindGroupEntry::SetSize(int size) {
    Get().size = size;
}

void JGPU::WGPUBindGroupEntry::SetSampler(JGPU::WGPUSampler* sampler) {
    Get().sampler = sampler ? sampler->Get() : nullptr;
}

void JGPU::WGPUBindGroupEntry::SetTextureView(JGPU::WGPUTextureView* textureView) {
    Get().textureView = textureView ? textureView->Get() : nullptr;
}

// JGPU::WGPUInstanceCapabilities
//void JGPU::WGPUInstanceCapabilities::SetTimedWaitAnyEnable(bool timedWaitAnyEnable) {
//    Get()->timedWaitAnyEnable = timedWaitAnyEnable;
//}
//
//void JGPU::WGPUInstanceCapabilities::SetTimedWaitAnyMaxCount(int timedWaitAnyMaxCount) {
//    Get()->timedWaitAnyMaxCount = timedWaitAnyMaxCount;
//}

// JGPU::WGPURenderPassMaxDrawCount
JGPU::WGPURenderPassMaxDrawCount* JGPU::WGPURenderPassMaxDrawCount::Obtain() {
    static JGPU::WGPURenderPassMaxDrawCount obj;
    obj = JGPU::WGPURenderPassMaxDrawCount();
    return &obj;
}

void JGPU::WGPURenderPassMaxDrawCount::SetMaxDrawCount(int maxDrawCount) {
    Get().maxDrawCount = maxDrawCount;
}

int JGPU::WGPURenderPassMaxDrawCount::GetMaxDrawCount() {
    return Get().maxDrawCount;
}

// JGPU::WGPUShaderSourceSPIRV
JGPU::WGPUShaderSourceSPIRV* JGPU::WGPUShaderSourceSPIRV::Obtain() {
    static JGPU::WGPUShaderSourceSPIRV obj;
    obj = JGPU::WGPUShaderSourceSPIRV();
    return &obj;
}

//void JGPU::WGPUShaderSourceSPIRV::SetCode(const uint32_t* code, size_t codeSize) {
//    this->code.assign(code, code + codeSize);
//    Get().codeSize = codeSize;
//    Get().code = this->code.data();
//}
//
//JGPU::WGPUChainedStruct JGPU::WGPUShaderSourceSPIRV::GetChain() {
//    JGPU::WGPUChainedStruct chain;
//    chain.Set(&Get().chain);
//    return chain;
//}

// JGPU::WGPUShaderSourceSPIRV
JGPU::WGPUSupportedWGSLLanguageFeatures* JGPU::WGPUSupportedWGSLLanguageFeatures::Obtain() {
    static JGPU::WGPUSupportedWGSLLanguageFeatures obj;
    obj = JGPU::WGPUSupportedWGSLLanguageFeatures();
    return &obj;
}

int JGPU::WGPUSupportedWGSLLanguageFeatures::GetFeatureCount() {
    return Get().featureCount;
}

WGPUWGSLLanguageFeatureName JGPU::WGPUSupportedWGSLLanguageFeatures::GetFeatureAt(int index) {
    return (index < Get().featureCount) ? Get().features[index] : WGPUWGSLLanguageFeatureName(0);
}

// JGPU::WGPUCompilationMessage
JGPU::WGPUCompilationMessage* JGPU::WGPUCompilationMessage::Obtain() {
    static JGPU::WGPUCompilationMessage obj;
    obj = JGPU::WGPUCompilationMessage();
    return &obj;
}

std::string JGPU::WGPUCompilationMessage::GetMessage() {
    return std::string(Get().message.data, Get().message.length);
}

WGPUCompilationMessageType JGPU::WGPUCompilationMessage::GetType() {
    return Get().type;
}

int JGPU::WGPUCompilationMessage::GetLineNum() {
    return Get().lineNum;
}

int JGPU::WGPUCompilationMessage::GetLinePos() {
    return Get().linePos;
}

int JGPU::WGPUCompilationMessage::GetOffset() {
    return Get().offset;
}

int JGPU::WGPUCompilationMessage::GetLength() {
    return Get().length;
}

// JGPU::WGPUCompilationInfo
int JGPU::WGPUCompilationInfo::GetMessageCount() {
    return Get().messageCount;
}

JGPU::WGPUCompilationMessage JGPU::WGPUCompilationInfo::GetMessage(int index) {
    int size = GetMessageCount();
    if(index >= 0 && index < size) {
        JGPU::WGPUCompilationMessage temp;
        temp.Set(Get().messages[index]);
        return temp;
    }
    return JGPU::WGPUCompilationMessage();
}

// ################################### DESCRIPTOR STRUCTS ###################################

// JGPU::WGPUInstanceDescriptor
JGPU::WGPUInstanceDescriptor* JGPU::WGPUInstanceDescriptor::Obtain() {
    static JGPU::WGPUInstanceDescriptor obj;
    obj = JGPU::WGPUInstanceDescriptor();
    return &obj;
}

void JGPU::WGPUInstanceDescriptor::SetNextInChain(JGPU::WGPUChainedStruct* chainedStruct) {
    Get().nextInChain = chainedStruct != nullptr ? chainedStruct->Get() : nullptr;
}

//JGPU::WGPUInstanceCapabilities JGPU::WGPUInstanceDescriptor::GetFeatures() { // TODO dawn have different code
//    #ifdef __EMSCRIPTEN__
//        JGPU::WGPUInstanceCapabilities temp;
//        temp.Set(&Get().capabilities);
//    #else
//        JGPU::WGPUInstanceCapabilities temp;
//        temp.Set(&Get().features);
//    #endif
//    return temp;
//}

// JGPU::WGPURenderBundleDescriptor
JGPU::WGPURenderBundleDescriptor* JGPU::WGPURenderBundleDescriptor::Obtain() {
    static JGPU::WGPURenderBundleDescriptor obj;
    obj = JGPU::WGPURenderBundleDescriptor();
    return &obj;
}

void JGPU::WGPURenderBundleDescriptor::SetNextInChain(JGPU::WGPUChainedStruct* chainedStruct) {
    Get().nextInChain = chainedStruct != nullptr ? chainedStruct->Get() : nullptr;
}

void JGPU::WGPURenderBundleDescriptor::SetLabel(const char* value) {
    JGPU::WGPUStringView stringView(value);
    Get().label = stringView.Get();
}

// JGPU::WGPURenderBundleEncoderDescriptor
JGPU::WGPURenderBundleEncoderDescriptor* JGPU::WGPURenderBundleEncoderDescriptor::Obtain() {
    static JGPU::WGPURenderBundleEncoderDescriptor obj;
    obj = JGPU::WGPURenderBundleEncoderDescriptor();
    return &obj;
}

void JGPU::WGPURenderBundleEncoderDescriptor::SetNextInChain(JGPU::WGPUChainedStruct* chainedStruct) {
    Get().nextInChain = chainedStruct != NULL ? chainedStruct->Get() : NULL;
}

void JGPU::WGPURenderBundleEncoderDescriptor::SetLabel(const char* value) {
    JGPU::WGPUStringView stringView(value);
    Get().label = stringView.Get();
}

void JGPU::WGPURenderBundleEncoderDescriptor::SetColorFormats(WGPUVectorTextureFormat* colorFormats) {
    if(colorFormats != NULL) {
        Get().colorFormatCount = colorFormats->size();
        Get().colorFormats = reinterpret_cast<const WGPUTextureFormat*>(colorFormats->data());
    }
    else {
        Get().colorFormatCount = 0;
        Get().colorFormats = NULL;
    }
}

void JGPU::WGPURenderBundleEncoderDescriptor::SetDepthStencilFormat(WGPUTextureFormat depthStencilFormat) {
    Get().depthStencilFormat = depthStencilFormat;
}

void JGPU::WGPURenderBundleEncoderDescriptor::SetSampleCount(int sampleCount) {
    Get().sampleCount = sampleCount;
}

void JGPU::WGPURenderBundleEncoderDescriptor::SetDepthReadOnly(int depthReadOnly) {
    Get().depthReadOnly = depthReadOnly;
}

void JGPU::WGPURenderBundleEncoderDescriptor::SetStencilReadOnly(int stencilReadOnly) {
    Get().stencilReadOnly = stencilReadOnly;
}

// JGPU::WGPUQuerySetDescriptor
JGPU::WGPUQuerySetDescriptor* JGPU::WGPUQuerySetDescriptor::Obtain() {
    static JGPU::WGPUQuerySetDescriptor obj;
    obj = JGPU::WGPUQuerySetDescriptor();
    return &obj;
}

void JGPU::WGPUQuerySetDescriptor::SetNextInChain(JGPU::WGPUChainedStruct* chainedStruct) {
    Get().nextInChain = chainedStruct != NULL ? chainedStruct->Get() : NULL;
}

void JGPU::WGPUQuerySetDescriptor::SetLabel(const char* value) {
    JGPU::WGPUStringView stringView(value);
    Get().label = stringView.Get();
}

void JGPU::WGPUQuerySetDescriptor::SetType(WGPUQueryType type) {
    Get().type = type;
}

void JGPU::WGPUQuerySetDescriptor::SetCount(int count) {
    Get().count = count;
}

// JGPU::WGPUSamplerDescriptor
JGPU::WGPUSamplerDescriptor* JGPU::WGPUSamplerDescriptor::Obtain() {
    static JGPU::WGPUSamplerDescriptor obj;
    obj = JGPU::WGPUSamplerDescriptor();
    return &obj;
}

void JGPU::WGPUSamplerDescriptor::SetNextInChain(JGPU::WGPUChainedStruct* chainedStruct) {
    Get().nextInChain = chainedStruct != NULL ? chainedStruct->Get() : NULL;
}

void JGPU::WGPUSamplerDescriptor::SetLabel(const char* value) {
    JGPU::WGPUStringView stringView(value);
    Get().label = stringView.Get();
}

void JGPU::WGPUSamplerDescriptor::SetAddressModeU(WGPUAddressMode addressModeU) {
    Get().addressModeU = addressModeU;
}

void JGPU::WGPUSamplerDescriptor::SetAddressModeV(WGPUAddressMode addressModeV) {
    Get().addressModeV = addressModeV;
}

void JGPU::WGPUSamplerDescriptor::SetAddressModeW(WGPUAddressMode addressModeW) {
    Get().addressModeW = addressModeW;
}

void JGPU::WGPUSamplerDescriptor::SetMagFilter(WGPUFilterMode magFilter) {
    Get().magFilter = magFilter;
}

void JGPU::WGPUSamplerDescriptor::SetMinFilter(WGPUFilterMode minFilter) {
    Get().minFilter = minFilter;
}

void JGPU::WGPUSamplerDescriptor::SetMipmapFilter(WGPUMipmapFilterMode mipmapFilter) {
    Get().mipmapFilter = mipmapFilter;
}

void JGPU::WGPUSamplerDescriptor::SetLodMinClamp(float lodMinClamp) {
    Get().lodMinClamp = lodMinClamp;
}

void JGPU::WGPUSamplerDescriptor::SetLodMaxClamp(float lodMaxClamp) {
    Get().lodMaxClamp = lodMaxClamp;
}

void JGPU::WGPUSamplerDescriptor::SetCompare(WGPUCompareFunction compare) {
    Get().compare = compare;
}

void JGPU::WGPUSamplerDescriptor::SetMaxAnisotropy(int maxAnisotropy) {
    Get().maxAnisotropy = maxAnisotropy;
}

// JGPU::WGPUTextureViewDescriptor
JGPU::WGPUTextureViewDescriptor* JGPU::WGPUTextureViewDescriptor::Obtain() {
    static JGPU::WGPUTextureViewDescriptor obj;
    obj = JGPU::WGPUTextureViewDescriptor();
    return &obj;
}

void JGPU::WGPUTextureViewDescriptor::SetNextInChain(JGPU::WGPUChainedStruct* chainedStruct) {
    Get().nextInChain = chainedStruct != NULL ? chainedStruct->Get() : NULL;
}

void JGPU::WGPUTextureViewDescriptor::SetLabel(const char* value) {
    JGPU::WGPUStringView stringView(value);
    Get().label = stringView.Get();
}

void JGPU::WGPUTextureViewDescriptor::SetFormat(WGPUTextureFormat format) {
    Get().format = format;
}

void JGPU::WGPUTextureViewDescriptor::SetDimension(WGPUTextureViewDimension dimension) {
    Get().dimension = dimension;
}

void JGPU::WGPUTextureViewDescriptor::SetBaseMipLevel(int baseMipLevel) {
    Get().baseMipLevel = baseMipLevel;
}

void JGPU::WGPUTextureViewDescriptor::SetMipLevelCount(int mipLevelCount) {
    Get().mipLevelCount = mipLevelCount;
}

void JGPU::WGPUTextureViewDescriptor::SetBaseArrayLayer(int baseArrayLayer) {
    Get().baseArrayLayer = baseArrayLayer;
}

void JGPU::WGPUTextureViewDescriptor::SetArrayLayerCount(int arrayLayerCount) {
    Get().arrayLayerCount = arrayLayerCount;
}

void JGPU::WGPUTextureViewDescriptor::SetAspect(WGPUTextureAspect aspect) {
    Get().aspect = aspect;
}

void JGPU::WGPUTextureViewDescriptor::SetUsage(WGPUTextureUsage usage) {
    Get().usage = usage;
}

// JGPU::WGPUComputePassDescriptor
JGPU::WGPUComputePassDescriptor* JGPU::WGPUComputePassDescriptor::Obtain() {
    static JGPU::WGPUComputePassDescriptor obj;
    obj = JGPU::WGPUComputePassDescriptor();
    return &obj;
}

void JGPU::WGPUComputePassDescriptor::SetNextInChain(JGPU::WGPUChainedStruct* chainedStruct) {
    Get().nextInChain = chainedStruct != NULL ? chainedStruct->Get() : NULL;
}

void JGPU::WGPUComputePassDescriptor::SetLabel(const char* value) {
    JGPU::WGPUStringView stringView(value);
    Get().label = stringView.Get();
}

void JGPU::WGPUComputePassDescriptor::SetTimestampWrites(JGPU::WGPUComputePassTimestampWrites* timestampWrites) {
    Get().timestampWrites = timestampWrites != NULL ? &timestampWrites->Get() : NULL;
}

// JGPU::WGPUTextureDescriptor
JGPU::WGPUTextureDescriptor* JGPU::WGPUTextureDescriptor::Obtain() {
    static JGPU::WGPUTextureDescriptor obj;
    obj = JGPU::WGPUTextureDescriptor();
    return &obj;
}

void JGPU::WGPUTextureDescriptor::SetNextInChain(JGPU::WGPUChainedStruct* chainedStruct) {
    Get().nextInChain = chainedStruct != NULL ? chainedStruct->Get() : NULL;
}

void JGPU::WGPUTextureDescriptor::SetLabel(const char* value) {
    JGPU::WGPUStringView stringView(value);
    Get().label = stringView.Get();
}

void JGPU::WGPUTextureDescriptor::SetUsage(WGPUTextureUsage usage) {
    Get().usage = usage;
}

void JGPU::WGPUTextureDescriptor::SetDimension(WGPUTextureDimension dimension) {
    Get().dimension = dimension;
}

JGPU::WGPUExtent3D JGPU::WGPUTextureDescriptor::GetSize() {
    JGPU::WGPUExtent3D temp(&Get().size);
    return temp;
}

void JGPU::WGPUTextureDescriptor::SetFormat(WGPUTextureFormat format) {
    Get().format = format;
}

void JGPU::WGPUTextureDescriptor::SetMipLevelCount(int mipLevelCount) {
    Get().mipLevelCount = mipLevelCount;
}

void JGPU::WGPUTextureDescriptor::SetSampleCount(int sampleCount) {
    Get().sampleCount = sampleCount;
}

void JGPU::WGPUTextureDescriptor::SetViewFormats(WGPUVectorTextureFormat* viewFormats) {
    if(viewFormats != NULL) {
        Get().viewFormatCount = viewFormats->size();
        Get().viewFormats = reinterpret_cast<const WGPUTextureFormat*>(viewFormats->data());
    }
    else {
        Get().viewFormatCount = 0;
        Get().viewFormats = NULL;
    }
}

// JGPU::WGPURenderPassDescriptor
JGPU::WGPURenderPassDescriptor* JGPU::WGPURenderPassDescriptor::Obtain() {
    static JGPU::WGPURenderPassDescriptor obj;
    obj = JGPU::WGPURenderPassDescriptor();
    return &obj;
}

void JGPU::WGPURenderPassDescriptor::Reset() {
    mHandle.nextInChain = NULL;
    mHandle.label = ::WGPUStringView{};
    mHandle.colorAttachmentCount = 0;
    mHandle.colorAttachments = NULL;
    mHandle.depthStencilAttachment = NULL;
    mHandle.occlusionQuerySet = NULL;
    mHandle.timestampWrites = NULL;
}

void JGPU::WGPURenderPassDescriptor::SetNextInChain(JGPU::WGPUChainedStruct* chainedStruct) {
    Get().nextInChain = chainedStruct != NULL ? chainedStruct->Get() : NULL;
}

void JGPU::WGPURenderPassDescriptor::SetLabel(const char* value) {
    JGPU::WGPUStringView stringView(value);
    Get().label = stringView.Get();
}

void JGPU::WGPURenderPassDescriptor::SetColorAttachments(WGPUVectorRenderPassColorAttachment* values) {
    if(values != NULL) {
        Get().colorAttachmentCount = values->size();
        Get().colorAttachments = reinterpret_cast<const ::WGPURenderPassColorAttachment*>(values->data());
    }
    else {
        Get().colorAttachmentCount = 0;
        Get().colorAttachments = NULL;
    }
}

void JGPU::WGPURenderPassDescriptor::SetDepthStencilAttachment(JGPU::WGPURenderPassDepthStencilAttachment* attachment) {
    Get().depthStencilAttachment = attachment == NULL ? NULL : &(attachment->Get());
}

void JGPU::WGPURenderPassDescriptor::SetOcclusionQuerySet(JGPU::WGPUQuerySet* timestampWrites) {
    Get().occlusionQuerySet = timestampWrites == NULL ? NULL : timestampWrites->Get();
}

void JGPU::WGPURenderPassDescriptor::SetTimestampWrites(JGPU::WGPURenderPassTimestampWrites* timestampWrites) {
    Get().timestampWrites = timestampWrites == NULL ? NULL : &(timestampWrites->Get());
}

// JGPU::WGPUCommandEncoderDescriptor
JGPU::WGPUCommandEncoderDescriptor* JGPU::WGPUCommandEncoderDescriptor::Obtain() {
    static JGPU::WGPUCommandEncoderDescriptor obj;
    obj = JGPU::WGPUCommandEncoderDescriptor();
    return &obj;
}

void JGPU::WGPUCommandEncoderDescriptor::SetNextInChain(JGPU::WGPUChainedStruct* chainedStruct) {
    Get().nextInChain = chainedStruct != NULL ? chainedStruct->Get() : NULL;
}

void JGPU::WGPUCommandEncoderDescriptor::SetLabel(const char* value) {
    JGPU::WGPUStringView stringView(value);
    Get().label = stringView.Get();
}

// JGPU::WGPUCommandBufferDescriptor
JGPU::WGPUCommandBufferDescriptor* JGPU::WGPUCommandBufferDescriptor::Obtain() {
    static JGPU::WGPUCommandBufferDescriptor obj;
    obj = JGPU::WGPUCommandBufferDescriptor();
    return &obj;
}

void JGPU::WGPUCommandBufferDescriptor::SetNextInChain(JGPU::WGPUChainedStruct* chainedStruct) {
    Get().nextInChain = chainedStruct != NULL ? chainedStruct->Get() : NULL;
}

void JGPU::WGPUCommandBufferDescriptor::SetLabel(const char* value) {
    JGPU::WGPUStringView stringView(value);
    Get().label = stringView.Get();
}

// JGPU::WGPURenderPipelineDescriptor
JGPU::WGPURenderPipelineDescriptor* JGPU::WGPURenderPipelineDescriptor::Obtain() {
    static JGPU::WGPURenderPipelineDescriptor obj;
    obj = JGPU::WGPURenderPipelineDescriptor();
    return &obj;
}

void JGPU::WGPURenderPipelineDescriptor::SetNextInChain(JGPU::WGPUChainedStruct* chainedStruct) {
    Get().nextInChain = chainedStruct != NULL ? chainedStruct->Get() : NULL;
}

void JGPU::WGPURenderPipelineDescriptor::SetLabel(const char* value) {
    JGPU::WGPUStringView stringView(value);
    Get().label = stringView.Get();
}

JGPU::WGPUVertexState JGPU::WGPURenderPipelineDescriptor::GetVertex() {
    JGPU::WGPUVertexState temp;
    temp.Set(&Get().vertex);
    return temp;
}

JGPU::WGPUPrimitiveState JGPU::WGPURenderPipelineDescriptor::GetPrimitive() {
    JGPU::WGPUPrimitiveState temp;
    temp.Set(&Get().primitive);
    return temp;
}

void JGPU::WGPURenderPipelineDescriptor::SetFragment(JGPU::WGPUFragmentState* fragment) {
    Get().fragment = &fragment->Get();
}

void JGPU::WGPURenderPipelineDescriptor::SetDepthStencil(JGPU::WGPUDepthStencilState* depthStencilState) {
    Get().depthStencil = depthStencilState != NULL ? &depthStencilState->Get() : NULL;
}

JGPU::WGPUMultisampleState JGPU::WGPURenderPipelineDescriptor::GetMultisample() {
    JGPU::WGPUMultisampleState temp;
    temp.Set(&Get().multisample);
    return temp;
}

void JGPU::WGPURenderPipelineDescriptor::SetLayout(JGPU::WGPUPipelineLayout* pipelineLayout) {
    Get().layout = pipelineLayout != NULL ? pipelineLayout->Get() : NULL;
}

// JGPU::WGPUShaderModuleDescriptor
JGPU::WGPUShaderModuleDescriptor* JGPU::WGPUShaderModuleDescriptor::Obtain() {
    static JGPU::WGPUShaderModuleDescriptor obj;
    obj = JGPU::WGPUShaderModuleDescriptor();
    return &obj;
}

void JGPU::WGPUShaderModuleDescriptor::SetNextInChain(JGPU::WGPUChainedStruct* chainedStruct) {
    Get().nextInChain = chainedStruct != NULL ? chainedStruct->Get() : NULL;
}

void JGPU::WGPUShaderModuleDescriptor::SetLabel(const char* value) {
    JGPU::WGPUStringView stringView(value);
    Get().label = stringView.Get();
}

// JGPU::WGPUBindGroupLayoutDescriptor
JGPU::WGPUBindGroupLayoutDescriptor* JGPU::WGPUBindGroupLayoutDescriptor::Obtain() {
    static JGPU::WGPUBindGroupLayoutDescriptor obj;
    obj = JGPU::WGPUBindGroupLayoutDescriptor();
    return &obj;
}

void JGPU::WGPUBindGroupLayoutDescriptor::SetNextInChain(JGPU::WGPUChainedStruct* chainedStruct) {
    Get().nextInChain = chainedStruct != NULL ? chainedStruct->Get() : NULL;
}

void JGPU::WGPUBindGroupLayoutDescriptor::SetLabel(const char* value) {
    JGPU::WGPUStringView stringView(value);
    Get().label = stringView.Get();
}

void JGPU::WGPUBindGroupLayoutDescriptor::SetEntries(WGPUVectorBindGroupLayoutEntry* entries) {
    if(entries != NULL) {
        Get().entryCount = entries->size();
        Get().entries = reinterpret_cast<const ::WGPUBindGroupLayoutEntry*>(entries->data());
    }
    else {
        Get().entryCount = 0;
        Get().entries = NULL;
    }
}

// JGPU::WGPUProgrammableStageDescriptor
JGPU::WGPUProgrammableStageDescriptor* JGPU::WGPUProgrammableStageDescriptor::Obtain() {
    static JGPU::WGPUProgrammableStageDescriptor obj;
    obj = JGPU::WGPUProgrammableStageDescriptor();
    return &obj;
}

void JGPU::WGPUProgrammableStageDescriptor::SetNextInChain(JGPU::WGPUChainedStruct* chainedStruct) {
    Get()->nextInChain = chainedStruct != NULL ? chainedStruct->Get() : NULL;
}

// JGPU::WGPUComputePipelineDescriptor
JGPU::WGPUComputePipelineDescriptor* JGPU::WGPUComputePipelineDescriptor::Obtain() {
    static JGPU::WGPUComputePipelineDescriptor obj;
    obj = JGPU::WGPUComputePipelineDescriptor();
    return &obj;
}

void JGPU::WGPUComputePipelineDescriptor::SetNextInChain(JGPU::WGPUChainedStruct* chainedStruct) {
    Get().nextInChain = chainedStruct != NULL ? chainedStruct->Get() : NULL;
}

void JGPU::WGPUComputePipelineDescriptor::SetLabel(const char* value) {
    JGPU::WGPUStringView stringView(value);
    Get().label = stringView.Get();
}

JGPU::WGPUProgrammableStageDescriptor JGPU::WGPUComputePipelineDescriptor::GetCompute() {
    JGPU::WGPUProgrammableStageDescriptor temp;
    temp.Set(&Get().compute);
    return temp;
}

// JGPU::WGPUQueueDescriptor
JGPU::WGPUQueueDescriptor* JGPU::WGPUQueueDescriptor::Obtain() {
    static JGPU::WGPUQueueDescriptor obj;
    obj = JGPU::WGPUQueueDescriptor();
    return &obj;
}

void JGPU::WGPUQueueDescriptor::SetNextInChain(JGPU::WGPUChainedStruct* chainedStruct) {
    Get()->nextInChain = chainedStruct != NULL ? chainedStruct->Get() : NULL;
}

void JGPU::WGPUQueueDescriptor::SetLabel(const char* value) {
    JGPU::WGPUStringView stringView(value);
    Get()->label = stringView.Get();
}

// JGPU::WGPUBufferDescriptor
JGPU::WGPUBufferDescriptor* JGPU::WGPUBufferDescriptor::Obtain() {
    static JGPU::WGPUBufferDescriptor obj;
    obj = JGPU::WGPUBufferDescriptor();
    return &obj;
}

void JGPU::WGPUBufferDescriptor::SetNextInChain(JGPU::WGPUChainedStruct* chainedStruct) {
    Get().nextInChain = chainedStruct != NULL ? chainedStruct->Get() : NULL;
}

void JGPU::WGPUBufferDescriptor::SetLabel(const char* value) {
    JGPU::WGPUStringView stringView(value);
    Get().label = stringView.Get();
}

void JGPU::WGPUBufferDescriptor::SetUsage(WGPUBufferUsage usage) {
    Get().usage = usage;
}

void JGPU::WGPUBufferDescriptor::SetSize(int size) {
    Get().size = size;
}

void JGPU::WGPUBufferDescriptor::SetMappedAtCreation(bool mappedAtCreation) {
    Get().mappedAtCreation = mappedAtCreation;
}

// JGPU::WGPUBindGroupDescriptor
JGPU::WGPUBindGroupDescriptor* JGPU::WGPUBindGroupDescriptor::Obtain() {
    static JGPU::WGPUBindGroupDescriptor obj;
    obj = JGPU::WGPUBindGroupDescriptor();
    return &obj;
}

void JGPU::WGPUBindGroupDescriptor::SetNextInChain(JGPU::WGPUChainedStruct* chainedStruct) {
    Get().nextInChain = chainedStruct != NULL ? chainedStruct->Get() : NULL;
}

void JGPU::WGPUBindGroupDescriptor::SetLabel(const char* value) {
    JGPU::WGPUStringView stringView(value);
    Get().label = stringView.Get();
}

void JGPU::WGPUBindGroupDescriptor::SetLayout(JGPU::WGPUBindGroupLayout* layout) {
    Get().layout = layout->Get();
}

void JGPU::WGPUBindGroupDescriptor::SetEntries(WGPUVectorBindGroupEntry* entries) {
    if(entries != NULL) {
        Get().entryCount = entries->size();
        Get().entries = reinterpret_cast<const ::WGPUBindGroupEntry*>(entries->data());
    }
    else {
        Get().entryCount = 0;
        Get().entries = NULL;
    }
}

// JGPU::WGPUPipelineLayoutDescriptor
JGPU::WGPUPipelineLayoutDescriptor* JGPU::WGPUPipelineLayoutDescriptor::Obtain() {
    static JGPU::WGPUPipelineLayoutDescriptor obj;
    obj = JGPU::WGPUPipelineLayoutDescriptor();
    return &obj;
}

void JGPU::WGPUPipelineLayoutDescriptor::SetNextInChain(JGPU::WGPUChainedStruct* chainedStruct) {
    Get().nextInChain = chainedStruct != NULL ? chainedStruct->Get() : NULL;
}

void JGPU::WGPUPipelineLayoutDescriptor::SetLabel(const char* value) {
    JGPU::WGPUStringView stringView(value);
    Get().label = stringView.Get();
}

void JGPU::WGPUPipelineLayoutDescriptor::SetBindGroupLayouts(WGPUVectorBindGroupLayout* bindGroupLayouts) {
    if(bindGroupLayouts != NULL) {
        Get().bindGroupLayoutCount = bindGroupLayouts->size();
        Get().bindGroupLayouts = reinterpret_cast<const ::WGPUBindGroupLayout*>(bindGroupLayouts->data());
    }
    else {
        Get().bindGroupLayoutCount = 0;
        Get().bindGroupLayouts = NULL;
    }
}

// JGPU::WGPUDeviceDescriptor
JGPU::WGPUDeviceDescriptor* JGPU::WGPUDeviceDescriptor::Obtain() {
    static JGPU::WGPUDeviceDescriptor obj;
    obj = JGPU::WGPUDeviceDescriptor();
    return &obj;
}

void JGPU::WGPUDeviceDescriptor::SetNextInChain(JGPU::WGPUChainedStruct* chainedStruct) {
    Get().nextInChain = chainedStruct != NULL ? chainedStruct->Get() : NULL;
}

void JGPU::WGPUDeviceDescriptor::SetLabel(const char* value) {
    JGPU::WGPUStringView stringView(value);
    Get().label = stringView.Get();
}

void JGPU::WGPUDeviceDescriptor::SetRequiredLimits(JGPU::WGPULimits* limits) {
    Get().requiredLimits = &(limits->Get());
}

void JGPU::WGPUDeviceDescriptor::SetRequiredFeatures(WGPUVectorFeatureName* features) {
    if(features != NULL) {
        Get().requiredFeatureCount = features->size();
        Get().requiredFeatures = features->data();
    }
    else {
        Get().requiredFeatureCount = 0;
        Get().requiredFeatures = NULL;
    }
}

JGPU::WGPUQueueDescriptor JGPU::WGPUDeviceDescriptor::GetDefaultQueue() {
    JGPU::WGPUQueueDescriptor temp;
    temp.Set(&Get().defaultQueue);
    return temp;
}

// ################################### OPAQUE POINTER ###################################

// JGPU::WGPUSampler
void JGPU::WGPUSampler::AddRefInternal() {
    wgpuSamplerAddRef(Get());
}

void JGPU::WGPUSampler::ReleaseInternal() {
    wgpuSamplerRelease(Get());
}

void JGPU::WGPUSampler::SetLabel(const char* label) {
    JGPU::WGPUStringView stringView(label);
    wgpuSamplerSetLabel(Get(), stringView.Get());
}

// JGPU::WGPURenderBundleEncoder
JGPU::WGPURenderBundleEncoder* JGPU::WGPURenderBundleEncoder::Obtain() {
    static JGPU::WGPURenderBundleEncoder obj;
    obj = JGPU::WGPURenderBundleEncoder();
    return &obj;
}

void JGPU::WGPURenderBundleEncoder::AddRefInternal() {
    wgpuRenderBundleEncoderAddRef(Get());
}

void JGPU::WGPURenderBundleEncoder::ReleaseInternal() {
    wgpuRenderBundleEncoderRelease(Get());
}

void JGPU::WGPURenderBundleEncoder::SetPipeline(JGPU::WGPURenderPipeline* renderPipeline) {
    wgpuRenderBundleEncoderSetPipeline(Get(), renderPipeline->Get());
}

void JGPU::WGPURenderBundleEncoder::Draw(int vertexCount, int instanceCount, int firstVertex, int firstInstance) {
    wgpuRenderBundleEncoderDraw(Get(), vertexCount, instanceCount, firstVertex, firstInstance);
}

void JGPU::WGPURenderBundleEncoder::DrawIndexed(int indexCount, int instanceCount, int firstIndex, int baseVertex, int firstInstance) {
    wgpuRenderBundleEncoderDrawIndexed(Get(), indexCount, instanceCount, firstIndex, baseVertex, firstInstance);
}

void JGPU::WGPURenderBundleEncoder::DrawIndirect(JGPU::WGPUBuffer* indirectBuffer, int indirectOffset) {
    wgpuRenderBundleEncoderDrawIndirect(Get(), indirectBuffer->Get(), indirectOffset);
}

void JGPU::WGPURenderBundleEncoder::DrawIndexedIndirect(JGPU::WGPUBuffer* indirectBuffer, int indirectOffset) {
    wgpuRenderBundleEncoderDrawIndexedIndirect(Get(), indirectBuffer->Get(), indirectOffset);
}

void JGPU::WGPURenderBundleEncoder::SetBindGroup(int groupIndex, JGPU::WGPUBindGroup* group, WGPUVectorInt* offsets) {
    int dynamicOffsetCount = 0;
    uint32_t* dynamicOffsets = NULL;
    if(offsets != NULL) {
        dynamicOffsetCount = offsets->size();
        dynamicOffsets = (uint32_t*)offsets->data();
    }
    wgpuRenderBundleEncoderSetBindGroup(Get(), groupIndex, group->Get(), dynamicOffsetCount, dynamicOffsets);
}

void JGPU::WGPURenderBundleEncoder::SetVertexBuffer(int slot, JGPU::WGPUBuffer* buffer, int offset, int size) {
    wgpuRenderBundleEncoderSetVertexBuffer(Get(), slot, buffer->Get(), offset, size);
}

void JGPU::WGPURenderBundleEncoder::SetIndexBuffer(JGPU::WGPUBuffer* buffer, WGPUIndexFormat format, int offset, int size) {
    wgpuRenderBundleEncoderSetIndexBuffer(Get(), buffer->Get(), format, offset, size);
}

void JGPU::WGPURenderBundleEncoder::InsertDebugMarker(const char* label) {
    JGPU::WGPUStringView stringView(label);
    wgpuRenderBundleEncoderInsertDebugMarker(Get(), stringView.Get());
}

void JGPU::WGPURenderBundleEncoder::PopDebugGroup() {
    wgpuRenderBundleEncoderPopDebugGroup(Get());
}

void JGPU::WGPURenderBundleEncoder::PushDebugGroup(const char* label) {
    JGPU::WGPUStringView stringView(label);
    wgpuRenderBundleEncoderPushDebugGroup(Get(), stringView.Get());
}

void JGPU::WGPURenderBundleEncoder::SetLabel(const char* label) {
    JGPU::WGPUStringView stringView(label);
    wgpuRenderBundleEncoderSetLabel(Get(), stringView.Get());
}

void JGPU::WGPURenderBundleEncoder::Finish(JGPU::WGPURenderBundleDescriptor* descriptor, JGPU::WGPURenderBundle* bundle) {
    bundle->Set(wgpuRenderBundleEncoderFinish(Get(), descriptor != NULL ? &descriptor->Get() : NULL));
}

// JGPU::WGPUTextureView
JGPU::WGPUTextureView* JGPU::WGPUTextureView::Obtain() {
    static JGPU::WGPUTextureView obj;
    obj = JGPU::WGPUTextureView();
    return &obj;
}

void JGPU::WGPUTextureView::AddRefInternal() {
    wgpuTextureViewAddRef(Get());
}

void JGPU::WGPUTextureView::ReleaseInternal() {
    wgpuTextureViewRelease(Get());
}

// JGPU::WGPUTexture
JGPU::WGPUTexture* JGPU::WGPUTexture::Obtain() {
    static JGPU::WGPUTexture obj;
    obj = JGPU::WGPUTexture();
    return &obj;
}

void JGPU::WGPUTexture::AddRefInternal() {
    wgpuTextureAddRef(Get());
}

void JGPU::WGPUTexture::ReleaseInternal() {
    wgpuTextureRelease(Get());
}

void JGPU::WGPUTexture::CreateView(JGPU::WGPUTextureViewDescriptor* textureViewDescriptor, JGPU::WGPUTextureView* textureView) {
    textureView->Set(wgpuTextureCreateView(Get(), &(textureViewDescriptor->Get())));
}

WGPUTextureFormat JGPU::WGPUTexture::GetFormat() {
    return wgpuTextureGetFormat(Get());
}


void JGPU::WGPUTexture::Destroy() {
    wgpuTextureDestroy(Get());
}

// JGPU::WGPUShaderModule
JGPU::WGPUShaderModule* JGPU::WGPUShaderModule::Obtain() {
    static JGPU::WGPUShaderModule obj;
    obj = JGPU::WGPUShaderModule();
    return &obj;
}

void JGPU::WGPUShaderModule::AddRefInternal() {
    wgpuShaderModuleAddRef(Get());
}

void JGPU::WGPUShaderModule::ReleaseInternal() {
    wgpuShaderModuleRelease(Get());
}

bool JGPU::WGPUShaderModule::IsValid() {
    return Get();
}

// JGPU::WGPURenderPipeline
JGPU::WGPURenderPipeline* JGPU::WGPURenderPipeline::Obtain() {
    static JGPU::WGPURenderPipeline obj;
    obj = JGPU::WGPURenderPipeline();
    return &obj;
}

void JGPU::WGPURenderPipeline::AddRefInternal() {
    wgpuRenderPipelineAddRef(Get());
}

void JGPU::WGPURenderPipeline::ReleaseInternal() {
    wgpuRenderPipelineRelease(Get());
}

bool JGPU::WGPURenderPipeline::IsValid() {
    return Get();
}

// JGPU::WGPURenderPassEncoder
JGPU::WGPURenderPassEncoder* JGPU::WGPURenderPassEncoder::Obtain() {
    static JGPU::WGPURenderPassEncoder obj;
    obj = JGPU::WGPURenderPassEncoder();
    return &obj;
}

void JGPU::WGPURenderPassEncoder::AddRefInternal() {
    wgpuRenderPassEncoderAddRef(Get());
}

void JGPU::WGPURenderPassEncoder::ReleaseInternal() {
    wgpuRenderPassEncoderRelease(Get());
}

void JGPU::WGPURenderPassEncoder::End() {
    wgpuRenderPassEncoderEnd(Get());
}

void JGPU::WGPURenderPassEncoder::SetPipeline(JGPU::WGPURenderPipeline* renderPipeline) {
    wgpuRenderPassEncoderSetPipeline(Get(), renderPipeline->Get());
}

void JGPU::WGPURenderPassEncoder::BeginOcclusionQuery(int queryIndex) {
    wgpuRenderPassEncoderBeginOcclusionQuery(Get(), queryIndex);
}

void JGPU::WGPURenderPassEncoder::Draw(int vertexCount, int instanceCount, int firstVertex, int firstInstance) {
    wgpuRenderPassEncoderDraw(Get(), vertexCount, instanceCount, firstVertex, firstInstance);
}

void JGPU::WGPURenderPassEncoder::DrawIndexed(int indexCount, int instanceCount, int firstIndex, int baseVertex, int firstInstance) {
    wgpuRenderPassEncoderDrawIndexed(Get(), indexCount, instanceCount, firstIndex, baseVertex, firstInstance);
}

void JGPU::WGPURenderPassEncoder::DrawIndexedIndirect(JGPU::WGPUBuffer* indirectBuffer, int indirectOffset) {
    wgpuRenderPassEncoderDrawIndexedIndirect(Get(), indirectBuffer->Get(), indirectOffset);
}

void JGPU::WGPURenderPassEncoder::DrawIndirect(JGPU::WGPUBuffer* indirectBuffer, int indirectOffset) {
    wgpuRenderPassEncoderDrawIndirect(Get(), indirectBuffer->Get(), indirectOffset);
}

void JGPU::WGPURenderPassEncoder::EndOcclusionQuery() {
    wgpuRenderPassEncoderEndOcclusionQuery(Get());
}

void JGPU::WGPURenderPassEncoder::ExecuteBundles(WGPUVectorRenderBundle* values) {
    if(values != NULL) {
        int size = values->size();
        wgpuRenderPassEncoderExecuteBundles(Get(), size, reinterpret_cast<::WGPURenderBundle const * >(values->data()));
    }
    else {
        wgpuRenderPassEncoderExecuteBundles(Get(), 0, NULL);
    }
}

void JGPU::WGPURenderPassEncoder::InsertDebugMarker(const char* label) {
    JGPU::WGPUStringView stringView(label);
    wgpuRenderPassEncoderInsertDebugMarker(Get(), stringView.Get());
}

void JGPU::WGPURenderPassEncoder::PopDebugGroup() {
    wgpuRenderPassEncoderPopDebugGroup(Get());
}

void JGPU::WGPURenderPassEncoder::PushDebugGroup(const char* label) {
    JGPU::WGPUStringView stringView(label);
    wgpuRenderPassEncoderPushDebugGroup(Get(), stringView.Get());
}

void JGPU::WGPURenderPassEncoder::SetBindGroup(int groupIndex, JGPU::WGPUBindGroup* group, WGPUVectorInt* offsets) {
    int dynamicOffsetCount = 0;
    uint32_t* dynamicOffsets = NULL;
    if(offsets != NULL) {
        dynamicOffsetCount = offsets->size();
        dynamicOffsets = (uint32_t*)offsets->data();
    }
    wgpuRenderPassEncoderSetBindGroup(Get(), groupIndex, group->Get(), dynamicOffsetCount, dynamicOffsets);
}

void JGPU::WGPURenderPassEncoder::SetBlendConstant(JGPU::WGPUColor* color) {
    wgpuRenderPassEncoderSetBlendConstant(Get(), color != NULL ? color->Get() : NULL);
}

void JGPU::WGPURenderPassEncoder::SetIndexBuffer(JGPU::WGPUBuffer* buffer, WGPUIndexFormat format, int offset, int size) {
    wgpuRenderPassEncoderSetIndexBuffer(Get(), buffer->Get(), format, offset, size);
}

void JGPU::WGPURenderPassEncoder::SetLabel(const char* label) {
    JGPU::WGPUStringView stringView(label);
    wgpuRenderPassEncoderSetLabel(Get(), stringView.Get());
}

void JGPU::WGPURenderPassEncoder::SetScissorRect(int x, int y, int width, int height) {
    wgpuRenderPassEncoderSetScissorRect(Get(), x, y, width, height);
}

void JGPU::WGPURenderPassEncoder::SetStencilReference(int reference) {
    wgpuRenderPassEncoderSetStencilReference(Get(), reference);
}

void JGPU::WGPURenderPassEncoder::SetVertexBuffer(int slot, JGPU::WGPUBuffer* buffer, int offset, int size) {
    wgpuRenderPassEncoderSetVertexBuffer(Get(), slot, buffer->Get(), offset, size);
}

void JGPU::WGPURenderPassEncoder::SetViewport(float x, float y, float width, float height, float minDepth, float maxDepth) {
    wgpuRenderPassEncoderSetViewport(Get(), x, y, width, height, minDepth, maxDepth);
}

// JGPU::WGPUQuerySet
void JGPU::WGPUQuerySet::AddRefInternal() {
    wgpuQuerySetAddRef(Get());
}

void JGPU::WGPUQuerySet::ReleaseInternal() {
    wgpuQuerySetRelease(Get());
}

void JGPU::WGPUQuerySet::Destroy() {
    wgpuQuerySetDestroy(Get());
}

int JGPU::WGPUQuerySet::GetCount() {
    return wgpuQuerySetGetCount(Get());
}

WGPUQueryType JGPU::WGPUQuerySet::GetType() {
    return wgpuQuerySetGetType(Get());
}

void JGPU::WGPUQuerySet::SetLabel(const char* label) {
    JGPU::WGPUStringView stringView(label);
    wgpuQuerySetSetLabel(Get(), stringView.Get());
}

// JGPU::WGPUPipelineLayout
void JGPU::WGPUPipelineLayout::AddRefInternal() {
    wgpuPipelineLayoutAddRef(Get());
}

void JGPU::WGPUPipelineLayout::ReleaseInternal() {
    wgpuPipelineLayoutRelease(Get());
}

void JGPU::WGPUPipelineLayout::SetLabel(const char* value) {
    JGPU::WGPUStringView stringView(value);
    wgpuPipelineLayoutSetLabel(Get(), stringView.Get());
}

// JGPU::WGPUInstance
void JGPU::WGPUInstance::AddRefInternal() {
    wgpuInstanceAddRef(Get());
}

void JGPU::WGPUInstance::ReleaseInternal() {
    wgpuInstanceRelease(Get());
}

bool JGPU::WGPUInstance::IsValid() {
    return Get() ? true : false;
}

void JGPU::WGPUInstance::RequestAdapter(JGPU::WGPURequestAdapterOptions* options, WGPUCallbackMode mode, RequestAdapterCallback* callback) {
    WGPURequestAdapterCallbackInfo callbackInfo = {};
    callbackInfo.mode = mode;
    callbackInfo.callback = [](WGPURequestAdapterStatus status, ::WGPUAdapter ad, ::WGPUStringView message, void* callback_param, void*) {
        RequestAdapterCallback* cback = reinterpret_cast<RequestAdapterCallback*>(callback_param);
        JGPU::WGPUAdapter* adapter = new JGPU::WGPUAdapter();
        adapter->Set(ad);
        cback->OnCallback(status, adapter);
    };
    callbackInfo.userdata1 = reinterpret_cast<void*>(callback);
    callbackInfo.userdata2 = NULL;
    auto result = wgpuInstanceRequestAdapter(Get(), &(options->Get()), callbackInfo);
}

JGPU::WGPUSurface* JGPU::WGPUInstance::CreateWebSurface(const char* canvas) {
    #ifdef __EMSCRIPTEN__
        WGPUEmscriptenSurfaceSourceCanvasHTMLSelector canvasDesc {};
        JGPU::WGPUStringView stringView(canvas);
        canvasDesc.chain.sType = WGPUSType_EmscriptenSurfaceSourceCanvasHTMLSelector;
        canvasDesc.selector = stringView.Get();
        WGPUSurfaceDescriptor surfDesc{};
        surfDesc.nextInChain = (::WGPUChainedStruct*)&canvasDesc;
        JGPU::WGPUSurface* surface = new JGPU::WGPUSurface();
        surface->Set(wgpuInstanceCreateSurface(Get(), &surfDesc));
    #else
        JGPU::WGPUSurface* surface = NULL;
    #endif
    return surface;
}

JGPU::WGPUSurface* JGPU::WGPUInstance::CreateWindowsSurface(void * hwnd) {
    JGPU::WGPUSurface* surface = NULL;
    #if _WIN32
        surface = new JGPU::WGPUSurface();
        HINSTANCE hinstance = GetModuleHandle(NULL);
        WGPUSurfaceSourceWindowsHWND fromWindowsHWND;
        fromWindowsHWND.chain.next = NULL;
        fromWindowsHWND.chain.sType = WGPUSType_SurfaceSourceWindowsHWND;
        fromWindowsHWND.hinstance = hinstance;
        fromWindowsHWND.hwnd = hwnd;
        WGPUSurfaceDescriptor surfaceDescriptor{};
        surfaceDescriptor.nextInChain = &fromWindowsHWND.chain;
        surface->Set(wgpuInstanceCreateSurface(Get(), &surfaceDescriptor));
    #endif
    return surface;
}

JGPU::WGPUSurface* JGPU::WGPUInstance::CreateLinuxSurface(bool isWayland, void * windowOrSurface, void* display) {
    JGPU::WGPUSurface* surface = NULL;
    #if __linux__
        surface = new JGPU::WGPUSurface();
        if(isWayland) {
            WGPUSurfaceSourceWaylandSurface fromLinux;
            fromLinux.chain.next = NULL;
            fromLinux.chain.sType = WGPUSType_SurfaceSourceWaylandSurface;
            fromLinux.display = display;
            fromLinux.surface = windowOrSurface;
            WGPUSurfaceDescriptor surfaceDescriptor{};
            surfaceDescriptor.nextInChain = &fromLinux.chain;
            surface->Set(wgpuInstanceCreateSurface(Get(), &surfaceDescriptor));
        }
        else {
            WGPUSurfaceSourceXlibWindow fromLinux;
            fromLinux.chain.next = NULL;
            fromLinux.chain.sType = WGPUSType_SurfaceSourceXlibWindow;
            fromLinux.display = display;
            fromLinux.window = (uint64_t)windowOrSurface;
            WGPUSurfaceDescriptor surfaceDescriptor{};
            surfaceDescriptor.nextInChain = &fromLinux.chain;
            surface->Set(wgpuInstanceCreateSurface(Get(), &surfaceDescriptor));
        }
    #endif
    return surface;
}

JGPU::WGPUSurface* JGPU::WGPUInstance::CreateMacSurface(void * windowHandle) {
    JGPU::WGPUSurface* surface = NULL;
    #if TARGET_OS_MAC
        surface = new JGPU::WGPUSurface();
        id metal_layer = [CAMetalLayer layer];

        NSWindow* ns_window = (NSWindow*)windowHandle;
        [ns_window.contentView setWantsLayer : YES];
        [ns_window.contentView setLayer : metal_layer];

        WGPUSurfaceSourceMetalLayer fromMac;
        fromMac.chain.next = NULL;
        fromMac.chain.sType = WGPUSType_SurfaceSourceMetalLayer;
        fromMac.layer = metal_layer;
        WGPUSurfaceDescriptor surfaceDescriptor{};
        surfaceDescriptor.nextInChain = &fromMac.chain;
        surface->Set(wgpuInstanceCreateSurface(Get(), &surfaceDescriptor));
    #endif
    return surface;
}

JGPU::WGPUSurface* JGPU::WGPUInstance::CreateAndroidSurface(WGPUAndroidWindow* window) {
    JGPU::WGPUSurface* surface = NULL;
    #if __ANDROID__
        void* androidWindow = window->GetWindow();
        surface = new JGPU::WGPUSurface();
        WGPUSurfaceSourceAndroidNativeWindow androidSurfaceWindow;
        androidSurfaceWindow.chain.next = NULL;
        androidSurfaceWindow.chain.sType = WGPUSType_SurfaceSourceAndroidNativeWindow;
        androidSurfaceWindow.window = androidWindow;
        WGPUSurfaceDescriptor surfaceDescriptor{};
        surfaceDescriptor.nextInChain = &androidSurfaceWindow.chain;
        surface->Set(wgpuInstanceCreateSurface(Get(), &surfaceDescriptor));
    #endif
    return surface;
}

void JGPU::WGPUInstance::ProcessEvents() {
    #ifdef __EMSCRIPTEN__
        wgpuInstanceProcessEvents(Get());
//        emscripten_sleep(100);
    #else // __EMSCRIPTEN__
        wgpuInstanceProcessEvents(Get());
    #endif
}

WGPUWaitStatus JGPU::WGPUInstance::WaitAny(WGPUVectorFutureWaitInfo* futureVector, int timeoutNS) {
    int futureCount = 0;
    ::WGPUFutureWaitInfo* futures = NULL;
    if(futureVector != NULL) {
        futureCount = futureVector->size();
        futures = (::WGPUFutureWaitInfo*)futureVector->data();
    }
    return wgpuInstanceWaitAny(Get(), futureCount, futures, timeoutNS);
}

// JGPU::WGPUDevice
void JGPU::WGPUDevice::AddRefInternal() {
    wgpuDeviceAddRef(Get());
}

void JGPU::WGPUDevice::ReleaseInternal() {
    wgpuDeviceRelease(Get());
}

void JGPU::WGPUDevice::CreateBindGroup(JGPU::WGPUBindGroupDescriptor* descriptor, JGPU::WGPUBindGroup* valueOut) {
    valueOut->Set(wgpuDeviceCreateBindGroup(Get(), &descriptor->Get()));
}

void JGPU::WGPUDevice::CreateBindGroupLayout(JGPU::WGPUBindGroupLayoutDescriptor* descriptor, JGPU::WGPUBindGroupLayout* valueOut) {
    valueOut->Set(wgpuDeviceCreateBindGroupLayout(Get(), &descriptor->Get()));
}

void JGPU::WGPUDevice::CreateBuffer(JGPU::WGPUBufferDescriptor* descriptor, JGPU::WGPUBuffer* valueOut) {
    valueOut->Set(wgpuDeviceCreateBuffer(Get(), &descriptor->Get()));
}

void JGPU::WGPUDevice::CreateCommandEncoder(JGPU::WGPUCommandEncoderDescriptor* descriptor, JGPU::WGPUCommandEncoder* valueOut) {
    valueOut->Set(wgpuDeviceCreateCommandEncoder(Get(), descriptor != NULL ? &descriptor->Get() : NULL));
}

void JGPU::WGPUDevice::CreateComputePipeline(JGPU::WGPUComputePipelineDescriptor* descriptor, JGPU::WGPUComputePipeline* valueOut) {
    valueOut->Set(wgpuDeviceCreateComputePipeline(Get(), &(descriptor->Get())));
}

void JGPU::WGPUDevice::CreatePipelineLayout(JGPU::WGPUPipelineLayoutDescriptor* descriptor, JGPU::WGPUPipelineLayout* valueOut) {
    valueOut->Set(wgpuDeviceCreatePipelineLayout(Get(), &(descriptor->Get())));
}

void JGPU::WGPUDevice::CreateQuerySet(JGPU::WGPUQuerySetDescriptor* descriptor, JGPU::WGPUQuerySet* valueOut) {
    valueOut->Set(wgpuDeviceCreateQuerySet(Get(), &(descriptor->Get())));
}

void JGPU::WGPUDevice::CreateRenderBundleEncoder(JGPU::WGPURenderBundleEncoderDescriptor* descriptor, JGPU::WGPURenderBundleEncoder* valueOut) {
    valueOut->Set(wgpuDeviceCreateRenderBundleEncoder(Get(), &(descriptor->Get())));
}

void JGPU::WGPUDevice::CreateRenderPipeline(JGPU::WGPURenderPipelineDescriptor* descriptor, JGPU::WGPURenderPipeline* valueOut) {
    valueOut->Set(wgpuDeviceCreateRenderPipeline(Get(), &descriptor->Get()));
}

void JGPU::WGPUDevice::CreateSampler(JGPU::WGPUSamplerDescriptor* descriptor, JGPU::WGPUSampler* valueOut) {
    valueOut->Set(wgpuDeviceCreateSampler(Get(), &descriptor->Get()));
}

void JGPU::WGPUDevice::CreateShaderModule(JGPU::WGPUShaderModuleDescriptor* descriptor, JGPU::WGPUShaderModule* valueOut) {
    valueOut->Set(wgpuDeviceCreateShaderModule(Get(), &descriptor->Get()));
}

void JGPU::WGPUDevice::CreateTexture(JGPU::WGPUTextureDescriptor* descriptor, JGPU::WGPUTexture* valueOut) {
    valueOut->Set(wgpuDeviceCreateTexture(Get(), &descriptor->Get()));
}

void JGPU::WGPUDevice::Destroy() {
    wgpuDeviceDestroy(Get());
}

void JGPU::WGPUDevice::GetFeatures(JGPU::WGPUSupportedFeatures* features) {
    wgpuDeviceGetFeatures(Get(), &features->Get());
}

bool JGPU::WGPUDevice::HasFeature(WGPUFeatureName feature) {
    return wgpuDeviceHasFeature(Get(), feature);
}

void JGPU::WGPUDevice::GetLimits(JGPU::WGPULimits* limits) {
    wgpuDeviceGetLimits(Get(), &limits->Get());
}

JGPU::WGPUQueue* JGPU::WGPUDevice::GetQueue() {
    defaultQueue.Set(wgpuDeviceGetQueue(Get()));
    return &defaultQueue;
}

// JGPU::WGPUComputePassEncoder
JGPU::WGPUComputePassEncoder* JGPU::WGPUComputePassEncoder::Obtain() {
    static JGPU::WGPUComputePassEncoder obj;
    obj = JGPU::WGPUComputePassEncoder();
    return &obj;
}

void JGPU::WGPUComputePassEncoder::AddRefInternal() {
    wgpuComputePassEncoderAddRef(Get());
}

void JGPU::WGPUComputePassEncoder::ReleaseInternal() {
    wgpuComputePassEncoderRelease(Get());
}

void JGPU::WGPUComputePassEncoder::SetDispatchWorkgroups(int workgroupCountX, int workgroupCountY, int workgroupCountZ) {
    wgpuComputePassEncoderDispatchWorkgroups(Get(), workgroupCountX, workgroupCountY, workgroupCountZ);
}

void JGPU::WGPUComputePassEncoder::DispatchWorkgroupsIndirect(JGPU::WGPUBuffer* indirectBuffer, int indirectOffset) {
    wgpuComputePassEncoderDispatchWorkgroupsIndirect(Get(), indirectBuffer->Get(), indirectOffset);
}

void JGPU::WGPUComputePassEncoder::End() {
    wgpuComputePassEncoderEnd(Get());
}

void JGPU::WGPUComputePassEncoder::InsertDebugMarker(const char* markerLabel) {
    JGPU::WGPUStringView stringView(markerLabel);
    wgpuComputePassEncoderInsertDebugMarker(Get(), stringView.Get());
}

void JGPU::WGPUComputePassEncoder::PopDebugGroup() {
    wgpuComputePassEncoderPopDebugGroup(Get());
}

void JGPU::WGPUComputePassEncoder::PushDebugGroup(const char* groupLabel) {
    JGPU::WGPUStringView stringView(groupLabel);
    wgpuComputePassEncoderPushDebugGroup(Get(), stringView.Get());
}

void JGPU::WGPUComputePassEncoder::SetBindGroup(int groupIndex, JGPU::WGPUBindGroup* group, WGPUVectorInt* offsets) {
    int dynamicOffsetCount = 0;
    uint32_t* dynamicOffsets = NULL;
    if(offsets != NULL) {
        dynamicOffsetCount = offsets->size();
        dynamicOffsets = (uint32_t*)offsets->data();
    }
    wgpuComputePassEncoderSetBindGroup(Get(), groupIndex, group->Get(), dynamicOffsetCount, dynamicOffsets);
}

void JGPU::WGPUComputePassEncoder::SetLabel(const char* label) {
    JGPU::WGPUStringView stringView(label);
    wgpuComputePassEncoderSetLabel(Get(), stringView.Get());
}

void JGPU::WGPUComputePassEncoder::SetPipeline(JGPU::WGPUComputePipeline* pipeline) {
    wgpuComputePassEncoderSetPipeline(Get(), pipeline->Get());
}

// JGPU::WGPUCommandBuffer
JGPU::WGPUCommandBuffer* JGPU::WGPUCommandBuffer::Obtain() {
    static JGPU::WGPUCommandBuffer obj;
    obj = JGPU::WGPUCommandBuffer();
    return &obj;
}

void JGPU::WGPUCommandBuffer::AddRefInternal() {
    wgpuCommandBufferAddRef(Get());
}

void JGPU::WGPUCommandBuffer::ReleaseInternal() {
    wgpuCommandBufferRelease(Get());
}

// JGPU::WGPUCommandEncoder
JGPU::WGPUCommandEncoder* JGPU::WGPUCommandEncoder::Obtain() {
    static JGPU::WGPUCommandEncoder obj;
    obj = JGPU::WGPUCommandEncoder();
    return &obj;
}

void JGPU::WGPUCommandEncoder::AddRefInternal() {
    wgpuCommandEncoderAddRef(Get());
}

void JGPU::WGPUCommandEncoder::ReleaseInternal() {
    wgpuCommandEncoderRelease(Get());
}

void JGPU::WGPUCommandEncoder::BeginComputePass(JGPU::WGPUComputePassDescriptor* descriptor, JGPU::WGPUComputePassEncoder* encoder) {
    encoder->Set(wgpuCommandEncoderBeginComputePass(Get(), &descriptor->Get()));
}

void JGPU::WGPUCommandEncoder::BeginRenderPass(JGPU::WGPURenderPassDescriptor* renderPassDescriptor, JGPU::WGPURenderPassEncoder* encoder) {
    encoder->Set(wgpuCommandEncoderBeginRenderPass(Get(), &(renderPassDescriptor->Get())));
}

void JGPU::WGPUCommandEncoder::ClearBuffer(JGPU::WGPUBuffer* buffer, int offset, int size) {
    wgpuCommandEncoderClearBuffer(Get(), buffer->Get(), offset, size);
}

void JGPU::WGPUCommandEncoder::CopyBufferToBuffer(JGPU::WGPUBuffer* source, int sourceOffset, JGPU::WGPUBuffer* destination, int destinationOffset, int size) {
    wgpuCommandEncoderCopyBufferToBuffer(Get(), source->Get(), sourceOffset, destination->Get(), destinationOffset, size);
}

void JGPU::WGPUCommandEncoder::CopyBufferToTexture(JGPU::WGPUTexelCopyBufferInfo* source, JGPU::WGPUTexelCopyTextureInfo* destination, JGPU::WGPUExtent3D* copySize) {
    wgpuCommandEncoderCopyBufferToTexture(Get(), &source->Get(), &destination->Get(), copySize->Get());
}

void JGPU::WGPUCommandEncoder::TextureToBuffer(JGPU::WGPUTexelCopyTextureInfo* source, JGPU::WGPUTexelCopyBufferInfo* destination, JGPU::WGPUExtent3D* copySize) {
    wgpuCommandEncoderCopyTextureToBuffer(Get(), &source->Get(), &destination->Get(), copySize->Get());
}

void JGPU::WGPUCommandEncoder::CopyTextureToTexture(JGPU::WGPUTexelCopyTextureInfo* source, JGPU::WGPUTexelCopyTextureInfo* destination, JGPU::WGPUExtent3D* copySize) {
    wgpuCommandEncoderCopyTextureToTexture(Get(), &source->Get(), &destination->Get(), copySize->Get());
}

void JGPU::WGPUCommandEncoder::Finish(JGPU::WGPUCommandBufferDescriptor* commandBufferDescriptor, JGPU::WGPUCommandBuffer* commandBuffer) {
    commandBuffer->Set(wgpuCommandEncoderFinish(Get(), commandBufferDescriptor != NULL ? &commandBufferDescriptor->Get() : NULL));
}

void JGPU::WGPUCommandEncoder::InsertDebugMarker(const char* markerLabel) {
    JGPU::WGPUStringView stringView(markerLabel);
    wgpuCommandEncoderInsertDebugMarker(Get(), stringView.Get());
}

void JGPU::WGPUCommandEncoder::PopDebugGroup() {
    wgpuCommandEncoderPopDebugGroup(Get());
}

void JGPU::WGPUCommandEncoder::PushDebugGroup(const char* groupLabel) {
    JGPU::WGPUStringView stringView(groupLabel);
    wgpuCommandEncoderPushDebugGroup(Get(), stringView.Get());
}

void JGPU::WGPUCommandEncoder::ResolveQuerySet(JGPU::WGPUQuerySet* querySet, int firstQuery, int queryCount, JGPU::WGPUBuffer* destination, int destinationOffset) {
    wgpuCommandEncoderResolveQuerySet(Get(), querySet->Get(), firstQuery, queryCount, destination->Get(), destinationOffset);
}

void JGPU::WGPUCommandEncoder::SetLabel(const char* label) {
    JGPU::WGPUStringView stringView(label);
    wgpuCommandEncoderSetLabel(Get(), stringView.Get());
}

void JGPU::WGPUCommandEncoder::WriteTimestamp(JGPU::WGPUQuerySet* querySet, int queryIndex) {
    wgpuCommandEncoderWriteTimestamp(Get(), querySet->Get(), queryIndex);
}

// JGPU::WGPUBuffer
void JGPU::WGPUBuffer::AddRefInternal() {
    wgpuBufferAddRef(Get());
}

void JGPU::WGPUBuffer::ReleaseInternal() {
    wgpuBufferRelease(Get());
}

void JGPU::WGPUBuffer::Unmap() {
    wgpuBufferUnmap(Get());
}

int JGPU::WGPUBuffer::GetSize() {
    return wgpuBufferGetSize(Get());
}

WGPUBufferUsage JGPU::WGPUBuffer::GetUsage() {
    return static_cast<WGPUBufferUsage>(wgpuBufferGetUsage(Get()));
}

JGPU::WGPUFuture JGPU::WGPUBuffer::MapAsync(WGPUMapMode mode, int offset, int size, WGPUCallbackMode callbackMode, BufferMapCallback* callback) {
    WGPUBufferMapCallbackInfo callbackInfo = {};
    callbackInfo.mode = callbackMode;

    callbackInfo.callback = [](WGPUMapAsyncStatus status, ::WGPUStringView message, void* callback_param, void*) {
        BufferMapCallback* cback = reinterpret_cast<BufferMapCallback*>(callback_param);
        cback->OnCallback(status, JGPU::WGPUStringView(message).GetString().c_str());
    };
    callbackInfo.userdata1 = reinterpret_cast<void*>(callback);
    callbackInfo.userdata2 = NULL;

    JGPU::WGPUFuture temp;
    temp.Set(wgpuBufferMapAsync(Get(), mode, offset, size, callbackInfo));
    return temp;
}

void JGPU::WGPUBuffer::GetConstMappedRange(int offset, int size, void* bufferOut) {
    uint8_t* bufferData = (uint8_t*) wgpuBufferGetConstMappedRange(Get(), offset, size);
    std::memcpy(bufferOut, bufferData, size);
}

void JGPU::WGPUBuffer::Destroy() {
    wgpuBufferDestroy(Get());
}

// JGPU::WGPUBindGroup
void JGPU::WGPUBindGroup::AddRefInternal() {
    wgpuBindGroupAddRef(Get());
}

void JGPU::WGPUBindGroup::ReleaseInternal() {
    wgpuBindGroupRelease(Get());
}

void JGPU::WGPUBindGroup::SetLabel(const char* value) {
    JGPU::WGPUStringView stringView(value);
    wgpuBindGroupSetLabel(Get(), stringView.Get());
}

// JGPU::WGPUBindGroupLayout
void JGPU::WGPUBindGroupLayout::AddRefInternal() {
    wgpuBindGroupLayoutAddRef(Get());
}

void JGPU::WGPUBindGroupLayout::ReleaseInternal() {
    wgpuBindGroupLayoutRelease(Get());
}

void JGPU::WGPUBindGroupLayout::SetLabel(const char* value) {
    JGPU::WGPUStringView stringView(value);
    wgpuBindGroupLayoutSetLabel(Get(), stringView.Get());
}

// JGPU::WGPUComputePipeline
void JGPU::WGPUComputePipeline::AddRefInternal() {
    wgpuComputePipelineAddRef(Get());
}

void JGPU::WGPUComputePipeline::ReleaseInternal() {
    wgpuComputePipelineRelease(Get());
}

void JGPU::WGPUComputePipeline::SetLabel(const char* value) {
    JGPU::WGPUStringView stringView(value);
    wgpuComputePipelineSetLabel(Get(), stringView.Get());
}

JGPU::WGPUBindGroupLayout JGPU::WGPUComputePipeline::GetBindGroupLayout(int groupIndex) {
    JGPU::WGPUBindGroupLayout temp;
    temp.Set(wgpuComputePipelineGetBindGroupLayout(Get(), groupIndex));
    return temp;
}

// JGPU::WGPURenderBundle
void JGPU::WGPURenderBundle::AddRefInternal() {
    wgpuRenderBundleAddRef(Get());
}

void JGPU::WGPURenderBundle::ReleaseInternal() {
    wgpuRenderBundleRelease(Get());
}

void JGPU::WGPURenderBundle::SetLabel(const char* value) {
    JGPU::WGPUStringView stringView(value);
    wgpuRenderBundleSetLabel(Get(), stringView.Get());
}

// JGPU::WGPUAdapter
void JGPU::WGPUAdapter::AddRefInternal() {
    wgpuAdapterAddRef(Get());
}

void JGPU::WGPUAdapter::ReleaseInternal() {
    wgpuAdapterRelease(Get());
}

void JGPU::WGPUAdapter::RequestDevice(JGPU::WGPUDeviceDescriptor* descriptor, WGPUCallbackMode mode, RequestDeviceCallback* callback, UncapturedErrorCallback* errorCallback) {
    descriptor->Get().uncapturedErrorCallbackInfo.callback = [](const ::WGPUDevice* device, WGPUErrorType type, ::WGPUStringView message, void* callback_param, void* userdata_param) {
        UncapturedErrorCallback* cback = reinterpret_cast<UncapturedErrorCallback*>(callback_param);
        cback->OnCallback(type, JGPU::WGPUStringView(message).GetString().c_str());
    };
    descriptor->Get().uncapturedErrorCallbackInfo.userdata1 = reinterpret_cast<void*>(errorCallback);
    descriptor->Get().uncapturedErrorCallbackInfo.userdata2 = NULL;

    WGPURequestDeviceCallbackInfo callbackInfo = {};
    callbackInfo.mode = mode;
    callbackInfo.callback = [](WGPURequestDeviceStatus status, ::WGPUDevice dev, ::WGPUStringView message, void* callback_param, void*) {
        RequestDeviceCallback* cback = reinterpret_cast<RequestDeviceCallback*>(callback_param);
        JGPU::WGPUDevice* device = new JGPU::WGPUDevice();
        device->Set(dev);
        cback->OnCallback(status, device);
    };
    callbackInfo.userdata1 = reinterpret_cast<void*>(callback);
    callbackInfo.userdata2 = NULL;
    const ::WGPUDeviceDescriptor* desc = reinterpret_cast<::WGPUDeviceDescriptor const*>(descriptor);
    auto result = wgpuAdapterRequestDevice(Get(), desc, callbackInfo);
}

bool JGPU::WGPUAdapter::GetInfo(JGPU::WGPUAdapterInfo* adapterInfo) {
    WGPUStatus status = wgpuAdapterGetInfo(Get(), &(adapterInfo->Get()));
    return status == WGPUStatus_Success;
}

bool JGPU::WGPUAdapter::HasFeature(WGPUFeatureName featureName) {
    return wgpuAdapterHasFeature(Get(), featureName);
}

WGPUStatus JGPU::WGPUAdapter::GetLimits(JGPU::WGPULimits* limits) {
    return wgpuAdapterGetLimits(Get(), &limits->Get());
}

// JGPU::WGPUSurface
void JGPU::WGPUSurface::AddRefInternal() {
    wgpuSurfaceAddRef(Get());
}

void JGPU::WGPUSurface::ReleaseInternal() {
    wgpuSurfaceRelease(Get());
}

void JGPU::WGPUSurface::Unconfigure() {
    wgpuSurfaceUnconfigure(Get());
}

void JGPU::WGPUSurface::Configure(JGPU::WGPUSurfaceConfiguration* config) {
    wgpuSurfaceConfigure(Get(), reinterpret_cast<::WGPUSurfaceConfiguration const * >(config));
}

void JGPU::WGPUSurface::GetCapabilities(JGPU::WGPUAdapter* adapter, JGPU::WGPUSurfaceCapabilities* surfaceCapabilities) {
    wgpuSurfaceGetCapabilities(Get(), adapter->Get(), &(surfaceCapabilities->Get()));
}

void JGPU::WGPUSurface::GetCurrentTexture(JGPU::WGPUSurfaceTexture* tex) {
    wgpuSurfaceGetCurrentTexture(Get(), &tex->Get());
}

void JGPU::WGPUSurface::Present() {
    wgpuSurfacePresent(Get());
}

// JGPU::WGPUQueue
void JGPU::WGPUQueue::AddRefInternal() {
    wgpuQueueAddRef(Get());
}

void JGPU::WGPUQueue::ReleaseInternal() {
    wgpuQueueRelease(Get());
}

void JGPU::WGPUQueue::SetLabel(const char* value) {
    JGPU::WGPUStringView stringView(value);
    wgpuQueueSetLabel(Get(), stringView.Get());
}

void JGPU::WGPUQueue::Submit(int commandCount, JGPU::WGPUCommandBuffer* commandBuffer) {
    wgpuQueueSubmit(Get(), commandCount, &(commandBuffer->Get()));
}

void JGPU::WGPUQueue::WriteBuffer(JGPU::WGPUBuffer* buffer, int bufferOffset, WGPUByteBuffer* bytes, int dataSize) {
    int size = 0;
    void* data = NULL;
    if(bytes != NULL) {
        size = dataSize == -1 ? bytes->getLimit() : dataSize;
        data = (void*)bytes->data();
    }
    wgpuQueueWriteBuffer(Get(), buffer->Get(), bufferOffset, data, size);
}

void JGPU::WGPUQueue::WriteBuffer(JGPU::WGPUBuffer* buffer, int bufferOffset, WGPUFloatBuffer* bytes, int dataSize) {
    WriteBuffer(buffer, bufferOffset, bytes == NULL ? NULL : bytes->parent, dataSize);
}

void JGPU::WGPUQueue::WriteBuffer(JGPU::WGPUBuffer* buffer, int bufferOffset, void const * data, int size) {
    wgpuQueueWriteBuffer(Get(), buffer->Get(), bufferOffset, data, size);
}

void JGPU::WGPUQueue::WriteTexture(JGPU::WGPUTexelCopyTextureInfo* destination, WGPUByteBuffer* bytes, JGPU::WGPUTexelCopyBufferLayout* dataLayout, JGPU::WGPUExtent3D* writeSize) {
    int size = 0;
    void* data = NULL;
    if(bytes != NULL) {
        size = bytes->getLimit();
        data = (void*)bytes->data();
    }
    wgpuQueueWriteTexture(Get(), &destination->Get(), data, size, dataLayout->Get(), writeSize->Get());
}

void JGPU::WGPUQueue::WriteTexture(JGPU::WGPUTexelCopyTextureInfo* destination, void const * data, int size, JGPU::WGPUTexelCopyBufferLayout* dataLayout, JGPU::WGPUExtent3D* writeSize) {
    wgpuQueueWriteTexture(Get(), &destination->Get(), data, size, dataLayout->Get(), writeSize->Get());
}